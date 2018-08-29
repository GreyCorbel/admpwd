
#include "stdafx.h"
#include "..\AdmPwd\PasswordGenerator.h"


UINT __stdcall GenerateRandomPassword(MSIHANDLE hInstall)
{
	HRESULT hr = S_OK;
	UINT er = ERROR_SUCCESS;
	PasswordGenerator gen(MAX_PWD_COMPLEXITY, PWD_LENGTH);

	hr = WcaInitialize(hInstall, "GenerateRandomPassword");
	ExitOnFailure(hr, "Failed to initialize");

	WcaLog(LOGMSG_STANDARD, "Initialized.");

	gen.Generate();
	LPCTSTR newPwd = gen.Password;
	WcaLog(LOGMSG_STANDARD, "Password generated.");

	hr=WcaSetProperty(L"INITIALPASSWORD", newPwd);
	ExitOnFailure(hr, "Failed to set InitialPassword property");
	
	WcaLog(LOGMSG_STANDARD, "InitialPassword property set.");

LExit:
	er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
	return WcaFinalize(er);
}


