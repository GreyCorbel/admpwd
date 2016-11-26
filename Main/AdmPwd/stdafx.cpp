// stdafx.cpp : source file that includes just the standard includes
// AdmPwd.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information 
#include "stdafx.h"
#include "Constants.h"
#include "Config.h"

HINSTANCE hDll=(HINSTANCE)INVALID_HANDLE_VALUE;
wchar_t* DICTIONARY [] = { L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", L"abcdefghijklmnopqrstuvwxyz", L"0123456789", L",.-+;!#&@{}[]+$/()%|" };

//DllMain, Installation, Uninstallation
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason, LPVOID lpReserved) {
	if(ul_reason==DLL_PROCESS_ATTACH) {
		hDll=(HINSTANCE)hModule;
	}
    return TRUE;
}

//unregistration routine
 STDAPI DllUnregisterServer() {
	LSTATUS status=ERROR_SUCCESS;
	HRESULT hr=S_OK;

	status=SHDeleteKey(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH);
	if(status!=ERROR_SUCCESS) {
		//CSE not registered and we run unregistration
		if(status==ERROR_FILE_NOT_FOUND)
			hr=S_OK;
		else
			hr=HRESULT_FROM_WIN32(status);
	}
	status=SHDeleteKey(HKEY_LOCAL_MACHINE, GPEXT_REG_PATH_EVT);
	if(status!=ERROR_SUCCESS) {
		//CSE not registered and we run unregistration
		if(status==ERROR_FILE_NOT_FOUND)
			hr=S_OK;
		else
			hr=HRESULT_FROM_WIN32(status);
	}
	return hr;
}

 //registration routine
 //registers CSE with winlogon/gpsvc
 //registers DLL as event source for application event log
STDAPI DllRegisterServer()
{
	HKEY hKey=0;
	LONG lResult;
	size_t size;
	DWORD dwData;

	LPTSTR szPathBuff=NULL;

	__try {
		szPathBuff=new(std::nothrow) TCHAR[MAX_PATH];
		lResult=::GetModuleFileName((HINSTANCE)hDll, szPathBuff, MAX_PATH);
		// Register extension.
		if( lResult == 0 ) __leave;

		//this is to handle the scenario when replacing existing dll 
		//and registering the new version using code running within module with temporary name
		//installer might replaced extension ".new", so we're replacing here back to ".dll"
		PathRenameExtension(szPathBuff,FINAL_FILE_EXTENSION);

		lResult = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
								GPEXT_REG_PATH,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL);

		if( lResult != ERROR_SUCCESS ) __leave;
		
		//Name of entry point
		size=sizeof(ENTRY_POINT);
		lResult = RegSetValueEx( hKey,
					L"ProcessGroupPolicy",
					0,
					REG_SZ,
					(BYTE*)ENTRY_POINT,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		//Name of extension
		size=sizeof(EXTENSION_NAME);
		lResult = RegSetValueEx( hKey,
					NULL,
					0,
					REG_SZ,
					(BYTE*)EXTENSION_NAME,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		// Path to DLL
		::StringCbLength(szPathBuff,MAX_PATH,&size);
		lResult = RegSetValueEx( hKey,
					L"DllName",
					0,
					REG_EXPAND_SZ,
					(BYTE*)szPathBuff,
					size );
		if( lResult != ERROR_SUCCESS ) __leave;

		//we don't want be called when refreshing user policy
		dwData=1;
		lResult = RegSetValueEx( hKey,
					L"NoUserPolicy",
					0,
					REG_DWORD,
					(BYTE*)&dwData,
					sizeof(DWORD) );
		if( lResult != ERROR_SUCCESS ) __leave;

		RegCloseKey(hKey);

		//Register event log message file
		lResult = RegCreateKeyEx( HKEY_LOCAL_MACHINE,
								GPEXT_REG_PATH_EVT,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL);

		if( lResult != ERROR_SUCCESS ) __leave;

		//Path to DLL
		::StringCbLength(szPathBuff,MAX_PATH,&size);
		lResult = RegSetValueEx( hKey,
					L"EventMessageFile",
					0,
					REG_EXPAND_SZ,
					(BYTE*)szPathBuff,
					size);
		if( lResult != ERROR_SUCCESS ) __leave;

		//we provide event log with errors and successes
		dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_INFORMATION_TYPE | EVENTLOG_WARNING_TYPE;
		lResult = RegSetValueEx( hKey,
					L"TypesSupported",
					0,
					REG_DWORD,
					(BYTE*)&dwData,
					sizeof(DWORD) );
		if( lResult != ERROR_SUCCESS ) __leave;

	}
	__finally {
		//Close the registry key if necessary
		if(hKey) RegCloseKey( hKey );
	}

	return lResult;
}

