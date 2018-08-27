#include "stdafx.h"
#include "Computer.h"
#include "Constants.h"
#include "resource.h"


Computer::Computer()
{
	ULONG retVal=LDAP_SUCCESS;
	ULONG buffLen;
	
	//initialize members
	_pwdExpiration = 0;
	_conn=nullptr;
	_dn = nullptr;
	//end of init

	if (!GetComputerObjectName(NameFullyQualifiedDN, _dn, &buffLen))
		throw HRESULT_FROM_WIN32(GetLastError());
	_dn = new(std::nothrow) TCHAR[buffLen];
	if (!GetComputerObjectName(NameFullyQualifiedDN, _dn, &buffLen))
		throw HRESULT_FROM_WIN32(GetLastError());

	//initialize connection
	_conn = ldap_init(nullptr, LDAP_PORT);
	if (_conn==nullptr)
		throw HRESULT_FROM_WIN32(LdapGetLastError());
	//require encryption
	retVal = ldap_set_option(_conn, LDAP_OPT_ENCRYPT, LDAP_OPT_ON);
	if(retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
	//require signing
	retVal = ldap_set_option(_conn, LDAP_OPT_SIGN, LDAP_OPT_ON);
	if (retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
	//require writable DC
	ULONG flags = DS_WRITABLE_REQUIRED;
	retVal = ldap_set_option(_conn, LDAP_OPT_GETDSNAME_FLAGS, &flags);
	if (retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);

	//bind
	retVal = ldap_bind_s(_conn, nullptr, nullptr, LDAP_AUTH_NEGOTIATE);
	if (retVal != LDAP_SUCCESS)
		throw HRESULT_FROM_WIN32(retVal);
}

Computer::~Computer()
{
	if (_dn != nullptr)
		delete [] _dn;
	if (_conn != nullptr)
		ldap_unbind_s(_conn);
}


HRESULT Computer::Load()
{
	HRESULT hr = S_OK;
	BerElement *pBer = nullptr;
	LDAPMessage* rslt = nullptr;
	TCHAR* pAttribute = nullptr;
	TCHAR** ppValues = nullptr;

	TCHAR attrName[MAX_ATTR_NAME];
	ZeroMemory(attrName, MAX_ATTR_NAME*sizeof(TCHAR));

	int nChars = 0;

	extern HINSTANCE hDll;

	//retrieve timestamp
	nChars = ::LoadString(hDll, IDS_ATTR_PWD_TIMESTAMP, attrName, MAX_ATTR_NAME);
	if (nChars == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	TCHAR *attrs[2];
	attrs[0] = attrName;
	attrs[1] = nullptr;
	//load the computer object
	if (LDAP_SUCCESS != ldap_search_s(_conn, _dn, LDAP_SCOPE_BASE, _T("(objectClass=*)"), attrs, 0, &rslt))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	//should be always one entry
	ULONG numEntries = ldap_count_entries(_conn, rslt);
	if (numEntries < 1)
	{
		if (_conn->ld_errno!=0)
			hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		else
			hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
		goto Cleanup;
	}

	//get the entry. Does not to be explicitly freed as it's freed with rslt
	LDAPMessage *entry = ldap_first_entry(_conn, rslt);

	//get attribute name
	pAttribute = ldap_first_attribute(_conn, entry, &pBer);
	if (pAttribute == NULL)
	{
		if (_conn->ld_errno != 0)
			//error in processing
			hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		else
			//processing OK but attribute not found
			hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
		goto Cleanup;
	}

	ppValues = ldap_get_values(_conn, entry, pAttribute);
	if (_conn->ld_errno != 0)
	{
		hr = HRESULT_FROM_WIN32(_conn->ld_errno);
		goto Cleanup;
	}
	ULONG numValues = ldap_count_values(ppValues);

	if (numValues > 0)
		_stscanf_s(*ppValues, _T("%I64u"), &_pwdExpiration);

Cleanup:
	if (rslt!=nullptr)
		ldap_msgfree(rslt);
	if (pBer != nullptr)
		ber_free(pBer,0);
	if (ppValues != nullptr)
		ldap_value_free(ppValues);
	if (pAttribute != nullptr)
		ldap_memfree(pAttribute);

	return hr;
}


_Use_decl_annotations_
HRESULT Computer::ReportPassword(LPCTSTR password, FILETIME* timestamp, DWORD dwPasswordAge)
{
	LDAPMod ts, pwd;
	ULARGE_INTEGER uli;
	HRESULT hr = S_OK;
	int nChars = 0;

	TCHAR attrTSName[MAX_ATTR_NAME];
	ZeroMemory(attrTSName, MAX_ATTR_NAME*sizeof(TCHAR));

	TCHAR attrPwdName[MAX_ATTR_NAME];
	ZeroMemory(attrPwdName, MAX_ATTR_NAME*sizeof(TCHAR));
	
	extern HINSTANCE hDll;

	//timestamp
	//get attribute name from string table
	nChars = ::LoadString(hDll, IDS_ATTR_PWD_TIMESTAMP, attrTSName, MAX_ATTR_NAME);
	if (nChars == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	TCHAR tsBuff[32];
	uli.LowPart = timestamp->dwLowDateTime;
	uli.HighPart = timestamp->dwHighDateTime;
	uli.QuadPart += (ULONGLONG) dwPasswordAge*TICKS_IN_DAY;
	_stprintf_s(tsBuff, 31, _T("%I64u"), uli.QuadPart);
	TCHAR* pTimestamp [] = { tsBuff, nullptr };
	ts.mod_op = LDAP_MOD_REPLACE;
	ts.mod_type = attrTSName;
	ts.mod_values = pTimestamp;

	//password
	nChars = ::LoadString(hDll, IDS_ATTR_PWD, attrPwdName, MAX_ATTR_NAME);
	if (nChars == 0)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		goto Cleanup;
	}
	TCHAR* pPassword[2] = { const_cast<TCHAR*>(password), nullptr };
	pwd.mod_op = LDAP_MOD_REPLACE;
	pwd.mod_type = attrPwdName;
	pwd.mod_values = pPassword;

	LDAPMod *mods [] = { &ts, &pwd, nullptr};
	
	ULONG rslt=ldap_modify_s(_conn, _dn, mods);
	if (rslt != LDAP_SUCCESS)
		hr= HRESULT_FROM_WIN32(rslt);
	
Cleanup:

	return hr;
}

