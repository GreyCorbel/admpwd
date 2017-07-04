
#include "stdafx.h"


UINT __stdcall GetAdminsGroupName(MSIHANDLE hInstall)
{
	HRESULT hr = S_OK;
	UINT er = ERROR_SUCCESS;

	//group name
	TCHAR* _name = nullptr;
	//authority name
	TCHAR* _domainName = nullptr;

	//sid of admins group
	SID* adminsSid = nullptr;

	DWORD dwNameSize = 0, dwDomainSize = 0;

	BOOL bRslt;
	DWORD err = 0;
	
	
	hr = WcaInitialize(hInstall, "GetAdminsGroupName");
	ExitOnFailure(hr, "Failed to initialize");

	WcaLog(LOGMSG_STANDARD, "Initialized.");

	
	bRslt = ConvertStringSidToSid(_T("BA"), (PSID*) &adminsSid);
	if (!bRslt)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}

	SID_NAME_USE snu;
	bRslt = LookupAccountSid(NULL, adminsSid, _name, &dwNameSize, _domainName, &dwDomainSize, &snu);
	if (!bRslt)
	{
		err = GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER)
		{
			hr = HRESULT_FROM_WIN32(err);
			goto Cleanup;
		}
	}
	_name = new(std::nothrow) TCHAR[dwNameSize];
	_domainName = new(std::nothrow) TCHAR[dwDomainSize];
	if (_name == nullptr || _domainName == nullptr)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	bRslt = LookupAccountSid(NULL, adminsSid, _name, &dwNameSize, _domainName, &dwDomainSize, &snu);
	if (!bRslt)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}

	hr = WcaSetProperty(L"ADMINSGROUPNAME", _name);
	ExitOnFailure(hr, "Failed to set AdminsGroupName property");

	WcaLog(LOGMSG_STANDARD, "AdminsGroupName property set.");


Cleanup:
	if (adminsSid != nullptr)
		LocalFree(adminsSid);
	if (_name != nullptr)
		delete [] _name;
	if (_domainName != nullptr)
		delete [] _domainName;

LExit:
	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
	return WcaFinalize(er);

}