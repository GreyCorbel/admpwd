//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//	1 - 10 ... Error events
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: S_GET_COMPUTER
//
// MessageText:
//
// Could not get computer object from AD. Error %1.
//
#define S_GET_COMPUTER                   0xC0000002L

//
// MessageId: S_GET_ADMIN
//
// MessageText:
//
// Could not get local Administrator account. Error %1.
//
#define S_GET_ADMIN                      0xC0000003L

//
// MessageId: S_GET_TIMESTAMP
//
// MessageText:
//
// Could not get password expiration timestamp from computer account in AD. Error %1.
//
#define S_GET_TIMESTAMP                  0xC0000004L

//
// MessageId: S_CHANGE_PWD
//
// MessageText:
//
// Could not reset local Administrator's password. Error %1.
//
#define S_CHANGE_PWD                     0xC0000006L

//
// MessageId: S_REPORT_PWD
//
// MessageText:
//
// Could not write changed password to AD. Error %1.
//
#define S_REPORT_PWD                     0xC0000007L

//
// MessageId: S_EXPIRATION_TOO_LONG
//
// MessageText:
//
// Password expiration too long for computer (%1 days). Resetting password now.
//
#define S_EXPIRATION_TOO_LONG            0x8000000AL

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//	11 - 20 ... Informational events
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// MessageId: S_CHANGE_PWD_NOT_YET
//
// MessageText:
//
// It is not necessary to change password yet. Days to change: %1.
//
#define S_CHANGE_PWD_NOT_YET             0x4000000BL

//
// MessageId: S_CHANGE_PWD_SUCCESS
//
// MessageText:
//
// Local Administrator's password has been changed.
//
#define S_CHANGE_PWD_SUCCESS             0x4000000CL

//
// MessageId: S_REPORT_PWD_SUCCESS
//
// MessageText:
//
// Local Administrator's password has been reported to AD.
//
#define S_REPORT_PWD_SUCCESS             0x4000000DL

//
// MessageId: S_FINISHED
//
// MessageText:
//
// Finished successfully.
//
#define S_FINISHED                       0x4000000EL

//
// MessageId: S_STARTED
//
// MessageText:
//
// Beginning processing.
//
#define S_STARTED                        0x4000000FL

//
// MessageId: S_NOT_ENABLED
//
// MessageText:
//
// Admin account management not enabled, exiting.
//
#define S_NOT_ENABLED                    0x40000010L

//
// MessageId: S_NEVER_MANAGED
//
// MessageText:
//
// Admin account was never managed. Changing the password now.
//
#define S_NEVER_MANAGED                  0x40000011L

