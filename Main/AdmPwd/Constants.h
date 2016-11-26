//Constants used in the project
#ifndef ADMPWD_CONSTANTS
	#define ADMPWD_CONSTANTS
	//extension Eventlog message file registration place
	#define GPEXT_REG_PATH_EVT L"System\\CurrentControlSet\\Services\\EventLog\\Application\\AdmPwd"
	//installation support
	#define FINAL_FILE_EXTENSION _T(".dll")
	//name of entry point
	#define ENTRY_POINT L"ProcessGroupPolicy"
	//name of extension - event source for application log
	#define EXTENSION_NAME L"AdmPwd"


	//buffer size for event log messages
	#define MAX_MSG_LEN			256
	//maximum length of AD attribute name
	#define MAX_ATTR_NAME	256
	//maximum lenghts of directory time string - yyyyMMddhhmmss + trailing null
	#define MAX_TIMESTAMP_LENGTH	18

	//ticks in single day
	#define TICKS_IN_DAY ((ULONGLONG)86400 * (ULONGLONG)10000000)

	// structure for Event logging
	typedef struct _LOGDATA {
		DWORD dwID;
		HRESULT hr;
		DWORD dwLogLevel;
		DWORD info1;
	} LOGDATA, *LPLOGDATA;

#endif	// !ADMPWD_CONSTANTS