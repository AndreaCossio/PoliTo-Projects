#pragma once

#include <Windows.h>
#include "myDateAndTime.h"

// Record struct
typedef struct {
	TCHAR ip[18];
	TCHAR nickname[83];
	TCHAR access[22];
	TCHAR connection[9];
	MYDATETIME accessDT;
	MYTIME connectionT;
} MYRECORD, * LPMYRECORD;

BOOL readRecord(LPMYRECORD lpRecord, LPHANDLE lpFile);
BOOL writeRecord(LPMYRECORD lpRecord, LPHANDLE lpFile);
BOOL updateRecordAccess(LPMYRECORD lpRecord, MYDATETIME accessDT);
BOOL updateRecordConnection(LPMYRECORD lpRecord, MYTIME connectionT);
BOOL setRecordAccessStringFromDateTime(LPMYRECORD lpRecord);
BOOL setRecordConnectionStringFromTime(LPMYRECORD lpRecord);
BOOL setRecordAccessDateTimeFromString(LPMYRECORD lpRecord);
BOOL setRecordConnectionTimeFromString(LPMYRECORD lpRecord);