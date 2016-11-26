// stdafx.cpp : source file that includes just the standard includes
// AdmPwd.Setup.CustomActions.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

wchar_t* DICTIONARY [] = { L"ABCDEFGHIJKLMNOPQRSTUVWXYZ", L"abcdefghijklmnopqrstuvwxyz", L"0123456789", L",.-+;!#&@{}[]+$/()%|" };

// DllMain - Initialize and cleanup WiX custom action utils.
extern "C" BOOL WINAPI DllMain(
	__in HINSTANCE hInst,
	__in ULONG ulReason,
	__in LPVOID
	)
{
	switch (ulReason)
	{
	case DLL_PROCESS_ATTACH:
		WcaGlobalInitialize(hInst);
		break;

	case DLL_PROCESS_DETACH:
		WcaGlobalFinalize();
		break;
	}

	return TRUE;
}

