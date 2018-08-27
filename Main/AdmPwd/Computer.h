#pragma once
class Computer
{
public:
	Computer();
	~Computer();

	HRESULT ReportPassword(_In_ LPCTSTR password, _In_ FILETIME* timestamp, _In_ DWORD dwPasswordAge);
	HRESULT Load();
	//password complexity
	__declspec(property(get = GET_PasswordExpirationTimestamp)) unsigned __int64 PasswordExpirationTimestamp;
	unsigned __int64 GET_PasswordExpirationTimestamp() {
		return _pwdExpiration;
	};

private:
	unsigned __int64 _pwdExpiration;

	LDAP* _conn;
	LPTSTR _dn;

};

