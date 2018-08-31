#include "stdafx.h"
#include "AdminAccount.h"

_Use_decl_annotations_
AdminAccount::AdminAccount(TCHAR* AccountName)
{
	PSID pSID = NULL;
	BOOL bResult = false;
	TCHAR *RefDomain = nullptr;
	HRESULT hr=S_OK;
	
	//initialize members
	_AccountName = nullptr;
	_AccountNameLength = 0;
	//end of init

	if (AccountName != nullptr && _tcscmp(AccountName, _T(""))!=0)
	{
		//account name specified in GPO --> use it
		size_t dwNameLen = 0;
		hr = StringCchLength(AccountName, STRSAFE_MAX_CCH, &dwNameLen);
		if (FAILED(hr))
			goto Cleanup;
		dwNameLen++;	//to include trailing NULL
		_AccountName = new(std::nothrow)TCHAR[dwNameLen];
		if (_AccountName == nullptr)
		{
			hr = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
			goto Cleanup;	//not enough memory, bad...
		}
		_tcscpy_s(_AccountName,dwNameLen,AccountName);
		_AccountNameLength = dwNameLen;
	}
	else
	{
		//account name from gpo null or empty --> use builtin admin account
		DWORD dwNameSize=0, dwRefDomainSize=0;
		SID_NAME_USE snu;

		bResult = ConvertStringSidToSid(L"LA", &pSID);
		bResult = LookupAccountSid(NULL, pSID, NULL, &dwNameSize, NULL, &dwRefDomainSize, &snu);
		if (!bResult)
		{
			DWORD dwErr = GetLastError();
			if (dwErr != ERROR_INSUFFICIENT_BUFFER)
			{
				hr = HRESULT_FROM_WIN32(dwErr);
				goto Cleanup;
			}
		}

		_AccountName = new(std::nothrow) TCHAR[dwNameSize];
		RefDomain = new(std::nothrow) TCHAR[dwRefDomainSize];
		if (_AccountName == NULL || RefDomain == NULL)
		{
			hr = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
			goto Cleanup;	//not enough memory, bad...
		}

		bResult = LookupAccountSid(NULL, pSID, _AccountName, &dwNameSize, RefDomain, &dwRefDomainSize, &snu);
		if (!bResult)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
			goto Cleanup;	//could not resolve the SID to account, bad...
		}
		_AccountNameLength = dwNameSize;
	}
Cleanup:
	if (pSID)
		LocalFree(pSID);
	if (RefDomain != nullptr)
		delete [] RefDomain;

	if (FAILED(hr))
		throw hr;
}


AdminAccount::~AdminAccount()
{
	if (_AccountName != nullptr)
		delete [] _AccountName;
}

_Use_decl_annotations_
HRESULT AdminAccount::ResetPassword(LPCTSTR NewPasword)
{
	HRESULT hr = S_OK;
	USER_INFO_1003 ui1003;
	DWORD dwErr = 0;
	NET_API_STATUS rslt = NERR_Success;

	ui1003.usri1003_password = const_cast<TCHAR*>(NewPasword);

	rslt=NetUserSetInfo(nullptr, _AccountName, 1003, (LPBYTE) &ui1003, &dwErr);
	if (rslt != NERR_Success)
		hr = HRESULT_FROM_WIN32(rslt);
	
	//flag password change with user account
	USER_INFO_1012 ui;

	ui.usri1012_usr_comment = _T("1");
	rslt = NetUserSetInfo(nullptr, _AccountName, 1012, (LPBYTE)&ui, &dwErr);
	if (rslt != NERR_Success)
		hr = HRESULT_FROM_WIN32(rslt);

	return hr;
}

_Use_decl_annotations_
HRESULT AdminAccount::HasPasswordEverManaged(_Out_ bool *pResult)
{
	HRESULT hr = S_OK;
	LPUSER_INFO_1012 pui = nullptr;
	NET_API_STATUS rslt = NERR_Success;

	//this is to prevent ever changing of password in case that NetUserGetInfo() fails
	*pResult = true;

	rslt = NetUserGetInfo(nullptr, _AccountName, 1012, (LPBYTE*)&pui);
	if (rslt == NERR_Success)
	{
		*pResult = _tcscmp(pui->usri1012_usr_comment,_T("1"))==0;
	}
	else
	{
		hr = HRESULT_FROM_WIN32(rslt);
	}

	if (pui != nullptr)
		NetApiBufferFree(pui);

	return hr;
}