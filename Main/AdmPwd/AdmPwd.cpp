// AdmPwd.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "Constants.h"
#include "Config.h"
#include "PasswordGenerator.h"
#include "Computer.h"
#include "AdminAccount.h"
#include "AdmPwd.h"
#include "AdmPwdMsg.h"
#include "resource.h"

extern HINSTANCE hDll;
//#pragma warning( disable : 4267 )
//#pragma warning( disable : 4995 )


_Use_decl_annotations_
ADMPWD_API DWORD APIENTRY ProcessGroupPolicy(
	DWORD dwFlags,
	HANDLE hToken,
	HKEY hKeyRoot,
	PGROUP_POLICY_OBJECT pDeletedGPOList,
	PGROUP_POLICY_OBJECT pChangedGPOList,
	ASYNCCOMPLETIONHANDLE pHandle,
	BOOL* pAbort,
	PFNSTATUSMESSAGECALLBACK pStatusCallback)
{
   
	if(*pAbort)
		return ERROR_SUCCESS;

	// We only work if refreshing machine policy
	if(!(dwFlags & GPO_INFO_FLAG_MACHINE))
		return (ERROR_SUCCESS);

	Config config;

	LOGDATA	LogData;
	::ZeroMemory(&LogData, sizeof(LogData));

	ULONGLONG	llTimestamp=0;

	FILETIME currentTime;

	try {
		LogData.dwLogLevel = config.LogLevel;

		if(!config.AccountManagementEnabled)
		{
			LogData.dwID=S_NOT_ENABLED;
			LogData.hr=ERROR_SUCCESS;
			LogAppEvent(&LogData);
			return ERROR_SUCCESS;
		}
		//begin processing
		LogData.dwID=S_STARTED;
		LogData.hr=ERROR_SUCCESS;
		LogAppEvent(&LogData);
		
		//Get computer account in AD
		LogData.dwID=S_GET_COMPUTER;
		Computer comp;	

		//Get timestamp of expected password expiration
		LogData.dwID=S_GET_TIMESTAMP;
		LogData.hr=comp.Load();
		if (FAILED(LogData.hr))
		{
			if (LogData.hr != HRESULT_FROM_WIN32(ERROR_NOT_FOUND))	//NOT_FOUND --> empty attribute, which is OK
				throw LogData.hr;
		}
		ULONGLONG lDaysToChange=CompareTimestamp(comp.PasswordExpirationTimestamp);
		//if timestampExpire is empty or timestampExpire < Now - we will change the password
		if (lDaysToChange > config.PasswordAge && config.PasswordExpirationProtectionRequired)
		{
			LogData.dwID = S_EXPIRATION_TOO_LONG;
			LogData.info1 = (DWORD) lDaysToChange;
			LogAppEvent(&LogData);

			lDaysToChange = 0;
		}
		if(lDaysToChange)
		{
			LogData.dwID=S_CHANGE_PWD_NOT_YET;
			LogData.info1=(DWORD)lDaysToChange;
			LogAppEvent(&LogData);
		}
		else
		{
			//it's time to change the password
			//get local Administrator account we're managing password for
			LogData.dwID = S_GET_ADMIN;
			AdminAccount admin(config.AdminAccountName);

			PasswordGenerator gen(config.PasswordComplexity, config.PasswordLength);
			TCHAR *newPwd = gen.Generate();
			
			//report new password and timestamp to AD
			GetSystemTimeAsFileTime(&currentTime);
			LogData.dwID = S_REPORT_PWD;
			LogData.hr = comp.ReportPassword(newPwd, &currentTime, config.PasswordAge);
			if (FAILED(LogData.hr))
				throw LogData.hr;
			else
			{
				LogData.dwID = S_REPORT_PWD_SUCCESS;
				LogAppEvent(&LogData);
			}

			//RESET password
			// Note: we reset password AFTER we successfully report it to AD
			//  so in case we fail to report password for any reason, we do no reset the password
			//  we may still fail reset the password, in this case, AD password and real password do not match, 
			//  but in this case we know that there must be previous password set on admin account
			LogData.dwID = S_CHANGE_PWD;

			LogData.hr=admin.ResetPassword(newPwd);
			if (FAILED(LogData.hr))
				throw LogData.hr;
			else
			{
				LogData.dwID = S_CHANGE_PWD_SUCCESS;
				LogAppEvent(&LogData);
			}
		}
		//everything succeeded
		LogData.dwID=S_FINISHED;
		LogAppEvent(&LogData);
	}
	catch (HRESULT hr)
	{
		LogData.hr = hr;
		LogAppEvent(&LogData);
	}
	
	//In my opinion returning non-success in case of our error disturbs winlogon/gpsvc too much...
	//so we're simply returning success every time and just reporting our errors into event log
	return ERROR_SUCCESS;
}


//compares timestamps of expected password expiration with current time and returns number of days to change the password
_Use_decl_annotations_
ULONGLONG CompareTimestamp(ULONGLONG pTimestampExpire)
{
	FILETIME ftNow;
	ULONGLONG daysToChange=0;
	
	//Get local time
	//compare with expiration time and return result
	::GetSystemTimeAsFileTime(&ftNow);
	
	ULARGE_INTEGER uli;
	uli.HighPart=ftNow.dwHighDateTime;
	uli.LowPart=ftNow.dwLowDateTime;

	if(uli.QuadPart<pTimestampExpire)
	{
		daysToChange=(pTimestampExpire-uli.QuadPart+TICKS_IN_DAY)/TICKS_IN_DAY;
		return daysToChange;
	}
	return 0;
}

//Gets current time as uint64
_Use_decl_annotations_
void GetTimestamp(ULONGLONG *pTimestamp)
{
	HRESULT hr=ERROR_SUCCESS;
	FILETIME ft;
	ULARGE_INTEGER uli;
	GetSystemTimeAsFileTime(&ft);
	
	uli.LowPart=ft.dwLowDateTime;
	uli.HighPart=ft.dwHighDateTime;
	*pTimestamp=uli.QuadPart;
}

//logs event to application event log in case loglevel allows logging it
_Use_decl_annotations_
void LogAppEvent(LPLOGDATA pLogData)
{
	//decide whether to log or no
	switch(pLogData->dwLogLevel) {
		case LOGLEVEL_ERRORS_ONLY:
			if(!FAILED(pLogData->hr)) return;			//success or warning in errors_only level -> do not log
			break;
		case LOGLEVEL_ERRORS_WARNINGS:
			if(pLogData->hr==ERROR_SUCCESS) return;	//success in errors_warnings level -> do not log
			break;
	}
	
	HANDLE h=NULL; 
	BSTR szBuff=NULL;
	
	szBuff=SysAllocStringLen(NULL, MAX_MSG_LEN);
	if(szBuff==NULL)
		goto Cleanup;
		
	h = RegisterEventSource(NULL, EXTENSION_NAME);
	if (h == NULL) 
		goto Cleanup;

	switch(pLogData->dwID)
	{
		case S_GET_COMPUTER:
		case S_GET_ADMIN:
		case S_GET_TIMESTAMP:
		case S_CHANGE_PWD:
		case S_REPORT_PWD:
			if(FAILED(StringCbPrintf(szBuff, MAX_MSG_LEN*sizeof(OLECHAR), TEXT("0x%x"), pLogData->hr)))
				goto Cleanup;
			if (!ReportEvent(h, EVENTLOG_ERROR_TYPE,	0, pLogData->dwID, NULL, 1, 0, (LPCTSTR*)&szBuff, NULL))
				goto Cleanup;
			break;
		case S_FINISHED:
		case S_STARTED:
		case S_NOT_ENABLED:
		case S_CHANGE_PWD_SUCCESS:
		case S_REPORT_PWD_SUCCESS:
			BOOL bRslt;
			bRslt=ReportEvent(h, EVENTLOG_INFORMATION_TYPE,	0, pLogData->dwID, NULL, 0, 0, NULL, NULL);
			if (!bRslt)
				goto Cleanup;
			break;
		case S_CHANGE_PWD_NOT_YET:
			if(FAILED(StringCbPrintf(szBuff, MAX_MSG_LEN*sizeof(OLECHAR), TEXT("%u"), pLogData->info1)))
				goto Cleanup;
			if (!ReportEvent(h, EVENTLOG_INFORMATION_TYPE,	0, pLogData->dwID, NULL, 1, 0, (LPCTSTR*)&szBuff, NULL))
				goto Cleanup;
			break;
		case S_EXPIRATION_TOO_LONG:
			if(FAILED(StringCbPrintf(szBuff, MAX_MSG_LEN*sizeof(OLECHAR), TEXT("%u"), pLogData->info1)))
				goto Cleanup;
			if (!ReportEvent(h, EVENTLOG_WARNING_TYPE,	0, pLogData->dwID, NULL, 1, 0, (LPCTSTR*)&szBuff, NULL))
				goto Cleanup;
	}
	
	Cleanup:
	if(h != NULL) DeregisterEventSource(h);
	if(szBuff != NULL) SysFreeString(szBuff);
}


