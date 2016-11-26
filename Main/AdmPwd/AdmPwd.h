// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ADMPWD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ADMPWD_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ADMPWD_EXPORTS
#define ADMPWD_API __declspec(dllexport)
#else
#define ADMPWD_API __declspec(dllimport)
#endif

//forward declarations
ADMPWD_API DWORD APIENTRY ProcessGroupPolicy(_In_ DWORD dwFlags, _In_ HANDLE hToken, _In_ HKEY hKeyRoot, _In_ PGROUP_POLICY_OBJECT pDeletedGPOList, _In_ PGROUP_POLICY_OBJECT pChangedGPOList, _In_ ASYNCCOMPLETIONHANDLE pHandle, _In_ BOOL* pbAbort, _In_ PFNSTATUSMESSAGECALLBACK pStatusCallback);

void LogAppEvent(_In_ LPLOGDATA pLogData);

ULONGLONG CompareTimestamp(_In_ ULONGLONG pTimestamp);
void GetTimestamp(_Out_ ULONGLONG *pTimestamp);

