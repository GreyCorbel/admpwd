#pragma once
class AdminAccount
{
public:
	AdminAccount(_In_ TCHAR* AccountName);
	~AdminAccount();
	HRESULT ResetPassword(_In_ LPCTSTR NewPasword);
	HRESULT HasPasswordEverManaged(_Out_ bool *pResult);

	//Account name length
	__declspec(property(get = GET_AccountNameLength)) DWORD AccountNameLength;
	DWORD GET_AccountNameLength() {
		return _AccountNameLength;
	};
private:
	TCHAR * _AccountName;
	
	DWORD _AccountNameLength;
};

