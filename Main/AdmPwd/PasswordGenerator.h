
#pragma once
#include "stdafx.h"
extern wchar_t* DICTIONARY[MAX_PWD_COMPLEXITY];

typedef struct _DictionaryItem {
	size_t Length;				//length of charset
	wchar_t *Value;				//charset
	unsigned int Boundary;      //boundary for unbiased randoms
} DictionaryItem;

class PasswordGenerator
{
public:

	PasswordGenerator(_In_ unsigned int Complexity, _In_ unsigned int Length)
	{
		//complexity:
		//     1=large letters
		//     2=large_letters+small letters
		//     3=large_letters + small_letters + numbers
		//     4=large_letters + small_letters + numbers + spec_chars
		if (Complexity < 1 || Complexity > MAX_PWD_COMPLEXITY)
			_complexity = MAX_PWD_COMPLEXITY;
		else
			_complexity = Complexity;

		_complexityBoundary = (((unsigned int)(-1)) / _complexity)*_complexity;
		//length must be at least the same as complexity to fulfill complexity requirement
		_length = Length > _complexity ? Length : _complexity;

		_dictionary = new(std::nothrow) DictionaryItem[_complexity]();
		if (_dictionary == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

		for (unsigned int i = 0; i < _complexity; i++)
		{
			_dictionary[i].Value = DICTIONARY[i];
			//just be secure here, although we know which data we're processing
			HRESULT hr = StringCchLength(DICTIONARY[i], STRSAFE_MAX_CCH, &(_dictionary[i].Length));
			if (FAILED(hr))
				throw hr;
			//compute proper boundary for generated randoms that avoids bias when doing modulus
			_dictionary[i].Boundary = (unsigned int)((((unsigned int)-1) / _dictionary[i].Length)*_dictionary[i].Length);
		}

		_bUsed = new(std::nothrow) bool[_complexity]();
		if (_bUsed == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);

		NTSTATUS rslt = BCryptOpenAlgorithmProvider(&_hProv, BCRYPT_RNG_ALGORITHM, nullptr, 0);
		if (rslt != ERROR_SUCCESS)
			throw HRESULT_FROM_WIN32(rslt);
	}

	~PasswordGenerator()
	{
		if (_dictionary != nullptr)
			delete[] _dictionary;
		if (_bUsed != nullptr)
			delete[] _bUsed;

		ReleasePassword();

		if (_hProv != 0)
			BCryptCloseAlgorithmProvider(_hProv, 0);
	}

	void Generate() {

		unsigned int dict;
		unsigned int pos;

		ReleasePassword();
		AllocatePassword();

		for (unsigned int i = 0; i < _length; i++) {
			if (i >= _length - _complexity && _bUsed[_length - i - 1] == false)
			{
				//this ensures that all required dictionaries are used at least once with maximum amount of randomness
				dict = _length - i - 1;
			}
			else
			{
				dict = GetRandom(_complexity, _complexityBoundary);
			}
			_bUsed[dict] = true;
			pos = GetRandom((unsigned int)_dictionary[dict].Length, _dictionary[dict].Boundary);
			_pNewPwd[i] = _dictionary[dict].Value[pos];
		}
	}

	//Current password complexity
	__declspec(property(get = GET_PasswordComplexity)) unsigned int PasswordComplexity;
	unsigned int GET_PasswordComplexity() {
		return _complexity;
	};

	//Current password
	__declspec(property(get = GET_Password)) LPCTSTR Password;
	LPCTSTR GET_Password() {
		return _pNewPwd;
	};

private:
	DictionaryItem*  _dictionary = nullptr;

	unsigned int _complexity;
	unsigned int _length;
	unsigned int _complexityBoundary;

	bool *_bUsed = nullptr;

	TCHAR *_pNewPwd = nullptr;

	BCRYPT_ALG_HANDLE _hProv = 0;

	void ReleasePassword()
	{
		if (_pNewPwd != nullptr)
		{
			SecureZeroMemory(_pNewPwd, _length * sizeof(_pNewPwd[0]));
			delete[] _pNewPwd;
			_pNewPwd = nullptr;
		}
	}

	void AllocatePassword()
	{
		_pNewPwd = new(std::nothrow) TCHAR[_length + 1]();
		if (_pNewPwd == nullptr)
			throw HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
		ZeroMemory(_pNewPwd, _length * sizeof(_pNewPwd[0]));
		for (unsigned int i = 0; i < _complexity; i++)
			_bUsed[i] = false;
	}

	unsigned int GetRandom(unsigned int boundary, unsigned int rangeUpperExclusive)
	{
		unsigned int nRandom = INT_MAX;
		do {
			BCryptGenRandom(_hProv, (LPBYTE)&nRandom, sizeof(nRandom), 0);
			if (nRandom < rangeUpperExclusive)
			{
				return nRandom % boundary;
			}
		} while (true);
	}
};

