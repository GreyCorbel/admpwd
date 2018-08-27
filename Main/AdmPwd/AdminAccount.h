#pragma once
class AdminAccount
{
public:
	AdminAccount(_In_ TCHAR* AccountName);
	~AdminAccount();
	HRESULT ResetPassword(_In_ LPCTSTR NewPasword);
	//Account name length
	__declspec(property(get = GET_AccountNameLength)) DWORD AccountNameLength;
	DWORD GET_AccountNameLength() {
		return _AccountNameLength;
	};
private:
	TCHAR * _AccountName;
	
	DWORD _AccountNameLength;
};

