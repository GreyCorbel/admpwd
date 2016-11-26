SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
               Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
               Warning=0x2:STATUS_SEVERITY_WARNING
               Error=0x3:STATUS_SEVERITY_ERROR
              )

LanguageNames=(English=0x409:MSG00409)

;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;//	1 - 10 ... Error events
;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MessageId=2 Severity=Error SymbolicName=S_GET_COMPUTER
Language=English
Could not get computer object from AD. Error %1.
.

MessageId=3 Severity=Error SymbolicName=S_GET_ADMIN
Language=English
Could not get local Administrator account. Error %1.
.

MessageId=4 Severity=Error SymbolicName=S_GET_TIMESTAMP
Language=English
Could not get password expiration timestamp from computer account in AD. Error %1.
.

MessageId=6 Severity=Error SymbolicName=S_CHANGE_PWD
Language=English
Could not reset local Administrator's password. Error %1.
.

MessageId=7 Severity=Error SymbolicName=S_REPORT_PWD
Language=English
Could not write changed password to AD. Error %1.
.

MessageId=10 Severity=Warning SymbolicName=S_EXPIRATION_TOO_LONG
Language=English
Password expiration too long for computer (%1 days). Resetting password now.
.

;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;//	11 - 20 ... Informational events
;//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MessageId=11 Severity=Informational SymbolicName=S_CHANGE_PWD_NOT_YET
Language=English
It is not necessary to change password yet. Days to change: %1.
.

MessageId=12 Severity=Informational SymbolicName=S_CHANGE_PWD_SUCCESS
Language=English
Local Administrator's password has been changed.
.

MessageId=13 Severity=Informational SymbolicName=S_REPORT_PWD_SUCCESS
Language=English
Local Administrator's password has been reported to AD.
.

MessageId=14 Severity=Informational SymbolicName=S_FINISHED
Language=English
Finished successfully.
.

MessageId=15 Severity=Informational SymbolicName=S_STARTED
Language=English
Beginning processing.
.

MessageId=16 Severity=Informational SymbolicName=S_NOT_ENABLED
Language=English
Admin account management not enabled, exiting.
.
