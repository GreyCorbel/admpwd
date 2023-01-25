#include "stdafx.h"
#include "Config.h"

Config::Config() noexcept
{
	DWORD data;
	HKEY hPolicyKey = 0;
	HKEY hGPExtKey = 0;

	//initialize default values
	_AccountManagementEnabled=false;
	_pwdComplexity = MAX_PWD_COMPLEXITY;
	_pwdLength=PWD_LENGTH;
	_pwdAge = PWD_AGE_DAYS;
	_pwdExpirationProtectionRequired=false;

	_logLevel=0;

	_adminName=nullptr;
	//end of init

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH, 0, KEY_QUERY_VALUE, &hGPExtKey);
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, GPEXT_REG_POLICY_PATH, 0, KEY_QUERY_VALUE, &hPolicyKey);
	
	if (hGPExtKey != 0)
	{
		GetRegistryDWORD(hGPExtKey, LOG_LEVEL_REG_VALUE, &_logLevel);
		if (_logLevel >= LOGLEVEL_INVALID)
			_logLevel = LOGLEVEL_ALL_EVENTS;

		RegCloseKey(hGPExtKey);
	}

	if (hPolicyKey != 0)
	{
		GetRegistryDWORD(hPolicyKey, PWD_LEN_REG_VALUE, &_pwdLength);
		if (_pwdLength > MAX_PWD_LENGTH || _pwdLength < MIN_PWD_LENGTH)
			_pwdLength = PWD_LENGTH;

		GetRegistryDWORD(hPolicyKey, PWD_AGE_REG_VALUE, &_pwdAge);
		if (_pwdAge > MAX_PWD_AGE_DAYS)
			_pwdAge = MAX_PWD_AGE_DAYS;

		GetRegistryDWORD(hPolicyKey, PWD_COMPLEXITY_REG_VALUE, &_pwdAge);
		if (_pwdComplexity < MIN_PWD_COMPLEXITY || _pwdComplexity > MAX_PWD_COMPLEXITY)
			_pwdComplexity = MAX_PWD_COMPLEXITY;

		data = 0;
		GetRegistryDWORD(hPolicyKey, ADMIN_ACCOUNT_MANAGEMENT_ENABLED, &data);
		_AccountManagementEnabled = (data != 0);

		data = 0;
		GetRegistryString(hPolicyKey, ADMIN_ACCOUNT_NAME, &_adminName, &data);

		data = 0;
		GetRegistryDWORD(hPolicyKey, PWD_EXPIRATION_PROTECTION_ENABLED_REG_VALUE, &data);
		_pwdExpirationProtectionRequired = (data != 0);

		RegCloseKey(hPolicyKey);
	}
}

Config::~Config()
{
	if (_adminName != nullptr)
	{
		delete [] _adminName;
	}
}

_Use_decl_annotations_
HRESULT Config::GetRegistryDWORD(HKEY hReg, LPCTSTR regValueName, DWORD *retVal) {
	LONG lResult;
	DWORD dwBuffLen = sizeof(*retVal);

	if (!hReg)
		HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	lResult = RegQueryValueEx(hReg, regValueName, NULL, NULL, (LPBYTE) retVal, &dwBuffLen);

	if (lResult == ERROR_MORE_DATA)
	{
		lResult = ERROR_BAD_ARGUMENTS;	//value stored in registry is not REG_DWORD
	}

	return HRESULT_FROM_WIN32(lResult);
}

_Use_decl_annotations_
HRESULT Config::GetRegistryString(HKEY hReg, LPCTSTR regValueName, TCHAR **retVal, DWORD *dwStringLen)
{
	LONG lResult;
	DWORD dwBuffLen = 0;

	lResult = RegQueryValueEx(hReg, regValueName, NULL, NULL, NULL, &dwBuffLen);

	if (lResult == ERROR_MORE_DATA || lResult == ERROR_SUCCESS)
	{
		//seems to return ERROR_SUCCESS instead of ERROR_MORE_DATA when trying to get buffer length (at least on W2K8R2)
		*dwStringLen = dwBuffLen / sizeof(TCHAR);
		*retVal = new(std::nothrow) TCHAR[*dwStringLen]();	//allocate buffer; caller is responsible for releasing it
		if (*retVal == NULL)
			return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
		
		lResult = RegQueryValueEx(hReg, regValueName, NULL, NULL, (LPBYTE) *retVal, &dwBuffLen);
	}
	return HRESULT_FROM_WIN32(lResult);
}

