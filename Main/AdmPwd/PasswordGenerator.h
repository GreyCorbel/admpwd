
#pragma once
#include "stdafx.h"
extern wchar_t* DICTIONARY[MAX_PWD_COMPLEXITY];

typedef struct _DictionaryItem {
	size_t Length;
	wchar_t *Value;
} DictionaryItem;

class PasswordGenerator
{
public:

	PasswordGenerator(_In_ unsigned int Complexity, _In_ unsigned int Length)
	{
		//initialize members
		_dictionary = nullptr;

		_bUsed = nullptr;

		_pNewPwd = nullptr;

		_hProv = 0;

		//end of init

		//complexity:
		//	1=large letters
		//	2=large_letters+small letters
		//	3=large_letters + small_letters + numbers
		//	4=large_letters + small_letters + numbers + spec_chars
		if (Complexity < 1 || Complexity > MAX_PWD_COMPLEXITY)
			_complexity = MAX_PWD_COMPLEXITY;
		else
			_complexity = Complexity;

		_length = Length;

		_dictionary = new(std::nothrow) DictionaryItem[_complexity];
		if (_dictionary == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

		for (unsigned int i = 0; i < _complexity; i++)
		{
			_dictionary[i].Value = DICTIONARY[i];
			//just be secure here, although we know which data we're processing
			HRESULT hr=StringCchLength(DICTIONARY[i], STRSAFE_MAX_CCH, &(_dictionary[i].Length));
			if (FAILED(hr))
				throw hr;
		}

		_bUsed = new(std::nothrow) bool[_complexity]();
		if (_bUsed == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

		_pNewPwd = new(std::nothrow) TCHAR[_length + 1]();
		if (_pNewPwd == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

		if (!CryptAcquireContext(&_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
			throw HRESULT_FROM_WIN32(GetLastError());
	}

	~PasswordGenerator()
	{
		if (_dictionary != nullptr)
			delete [] _dictionary;
		if (_bUsed != nullptr)
			delete [] _bUsed;
		if (_pNewPwd != nullptr) {
			SecureZeroMemory(_pNewPwd, _length*sizeof(_pNewPwd[0]));
			delete [] _pNewPwd;
		}
		if (_hProv != 0)
			CryptReleaseContext(_hProv, 0);
	}


	TCHAR* Generate() {

		int dict;
		int pos;
		UINT nRandom = 0;

		for (unsigned int i = 0; i < _length; i++) {
			if (i >= _length - _complexity && _bUsed[_length - i - 1] == false) {
				//this ensures that all required dictionaries are used at least once
				//with maximum amount of randomness
				dict = _length - i - 1;
			}
			else {
				CryptGenRandom(_hProv, sizeof(nRandom), (LPBYTE) &nRandom);
				dict = nRandom % _complexity;
			}
			_bUsed[dict] = true;
			CryptGenRandom(_hProv, sizeof(nRandom), (LPBYTE) &nRandom);
			pos = nRandom % (_dictionary[dict].Length - 1);
			_pNewPwd[i] = _dictionary[dict].Value[pos];
		}
		
		return _pNewPwd;
	}

	//Current password complexity
	__declspec(property(get = GET_PasswordComplexity)) unsigned int PasswordComplexity;
	unsigned int GET_PasswordComplexity() {
		return _complexity;
	};

private:
	DictionaryItem*  _dictionary;

	unsigned int _complexity;
	unsigned int _length;

	bool *_bUsed;

	TCHAR *_pNewPwd;

	HCRYPTPROV _hProv;

};

