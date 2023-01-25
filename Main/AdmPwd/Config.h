#pragma once
#ifndef ADMPWD_CONFIG
#define ADMPWD_CONFIG

//Logging levels
enum {
	LOGLEVEL_ERRORS_ONLY = 0,
	LOGLEVEL_ERRORS_WARNINGS,
	LOGLEVEL_ALL_EVENTS,
	LOGLEVEL_INVALID	//always at the end of enum
};

// extension registration place
#define GPEXT_REG_PATH L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\GPExtensions\\{D76B9641-3288-4f75-942D-087DE603E3EA}"
// extension policy store
#define GPEXT_REG_POLICY_PATH L"Software\\Policies\\Microsoft Services\\AdmPwd"

//name of registry value storing flag that enables password management
#define ADMIN_ACCOUNT_MANAGEMENT_ENABLED L"AdmPwdEnabled"
//name of registry value storing the admin account name
#define ADMIN_ACCOUNT_NAME L"AdminAccountName"
//name of registry value storing desired password length
#define PWD_LEN_REG_VALUE L"PasswordLength"
//name of registry value storing desired password length
#define PWD_COMPLEXITY_REG_VALUE L"PasswordComplexity"
//name of registry value storing desired password age in days
#define PWD_AGE_REG_VALUE L"PasswordAgeDays"
//name of registry value storing the logging level
#define LOG_LEVEL_REG_VALUE L"ExtensionDebugLevel"
//name of registry value storing flag whether or not to enforce password age policy on expiration time
#define PWD_EXPIRATION_PROTECTION_ENABLED_REG_VALUE L"PwdExpirationProtectionEnabled"

//Password quality and duration parameters
//default pwd complexity
#define MAX_PWD_COMPLEXITY	4
#define MIN_PWD_COMPLEXITY	1
#define MIN_PWD_LENGTH	8
//default pwd length
#define PWD_LENGTH		12
#define MAX_PWD_LENGTH	64
//default pwd age
#define PWD_AGE_DAYS	30
#define MAX_PWD_AGE_DAYS	365
#define MIN_PWD_AGE_DAYS	1

class Config
{
public:
	Config() noexcept;
	~Config();

	//password complexity
	__declspec(property(get = GET_PasswordComplexity)) DWORD PasswordComplexity;
	//password length
	__declspec(property(get = GET_PasswordLength)) DWORD PasswordLength;
	//max password age
	__declspec(property(get = GET_PasswordAge)) DWORD PasswordAge;
	//logging level
	__declspec(property(get = GET_LogLevel)) DWORD LogLevel;
	//admin account name
	__declspec(property(get = GET_AdminAccountName)) TCHAR* AdminAccountName;
	//management enabled
	__declspec(property(get = GET_AccountManagementEnabled)) bool AccountManagementEnabled;
	//password expiration protection
	__declspec(property(get = GET_PasswordExpirationProtectionRequired)) bool PasswordExpirationProtectionRequired;

	//accessors
	DWORD GET_PasswordComplexity() noexcept {
		return _pwdComplexity;
	};
	bool GET_AccountManagementEnabled() noexcept {
		return _AccountManagementEnabled;
	};
	DWORD GET_PasswordLength() noexcept {
		return _pwdLength;
	};
	DWORD GET_PasswordAge() noexcept {
		return _pwdAge;
	};
	DWORD GET_LogLevel() noexcept {
		return _logLevel;
	};
	TCHAR* GET_AdminAccountName() noexcept {
		return _adminName;
	};
	bool GET_PasswordExpirationProtectionRequired() noexcept {
		return _pwdExpirationProtectionRequired;
	};
	
private:
	bool _AccountManagementEnabled;
	DWORD _pwdComplexity;
	DWORD _pwdLength;
	DWORD _pwdAge;

	DWORD _logLevel;

	TCHAR* _adminName;
	bool _pwdExpirationProtectionRequired;

	//methods
	HRESULT GetRegistryDWORD(_In_ HKEY hReg, _In_ LPCTSTR regValueName, _Out_ DWORD *retVal) noexcept;
	HRESULT GetRegistryString(_In_ HKEY hReg, _In_ LPCTSTR regValueName, _Out_ TCHAR **retVal, _Out_ DWORD *dwStringLen) noexcept;
};

#endif // !ADMPWD_CONFIG
