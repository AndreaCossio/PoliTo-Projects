#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <tchar.h>
#include "myRecord.h"

BOOL readRecord(LPMYRECORD lpRecord, LPHANDLE lpFile) {

	DWORD read;

	if (!ReadFile(*lpFile, lpRecord->ip, 17 * sizeof(TCHAR), &read, NULL) || read != 17 * sizeof(TCHAR)) {
		return FALSE;
	}

	if (!ReadFile(*lpFile, lpRecord->nickname, 82 * sizeof(TCHAR), &read, NULL) || read != 82 * sizeof(TCHAR)) {
		return FALSE;
	}

	if (!ReadFile(*lpFile, lpRecord->access, 21 * sizeof(TCHAR), &read, NULL) || read != 21 * sizeof(TCHAR)) {
		return FALSE;
	}

	if (!ReadFile(*lpFile, lpRecord->connection, 8 * sizeof(TCHAR), &read, NULL) || read != 8 * sizeof(TCHAR)) {
		return FALSE;
	}

	lpRecord->ip[17] = _T('\0');
	lpRecord->nickname[82] = _T('\0');
	lpRecord->access[21] = _T('\0');
	lpRecord->connection[8] = _T('\0');

	if (!setRecordAccessDateTimeFromString(lpRecord) || !setRecordConnectionTimeFromString(lpRecord)) {
		return FALSE;
	}

	return TRUE;
}

BOOL writeRecord(LPMYRECORD lpRecord, LPHANDLE lpFile) {

	DWORD written;

	if (!WriteFile(*lpFile, lpRecord->ip, 17 * sizeof(TCHAR), &written, NULL) || written != 17 * sizeof(TCHAR)) {
		return FALSE;
	}
	if (!WriteFile(*lpFile, lpRecord->nickname, 82 * sizeof(TCHAR), &written, NULL) || written != 82 * sizeof(TCHAR)) {
		return FALSE;
	}
	if (!WriteFile(*lpFile, lpRecord->access, 21 * sizeof(TCHAR), &written, NULL) || written != 21 * sizeof(TCHAR)) {
		return FALSE;
	}
	if (!WriteFile(*lpFile, lpRecord->connection, 8 * sizeof(TCHAR), &written, NULL) || written != 8 * sizeof(TCHAR)) {
		return FALSE;
	}

	return TRUE;
}

BOOL updateRecordAccess(LPMYRECORD lpRecord, MYDATETIME accessDT) {

	lpRecord->accessDT = accessDT;

	if (!setRecordAccessStringFromDateTime(lpRecord)) {
		return FALSE;
	}
	
	return TRUE;
}

BOOL updateRecordConnection(LPMYRECORD lpRecord, MYTIME connectionT) {

	lpRecord->connectionT = connectionT;

	if (!setRecordConnectionStringFromTime(lpRecord)) {
		return FALSE;
	}

	return TRUE;
}

BOOL setRecordAccessStringFromDateTime(LPMYRECORD lpRecord) {

	if (_stprintf(lpRecord->access, _T("%04d/%02d/%02d:%02d:%02d:%02d  "),
		lpRecord->accessDT.date.year,
		lpRecord->accessDT.date.month,
		lpRecord->accessDT.date.day,
		lpRecord->accessDT.time.hour,
		lpRecord->accessDT.time.min,
		lpRecord->accessDT.time.sec) != 6) {
		return FALSE;
	}

	return TRUE;
}

BOOL setRecordConnectionStringFromTime(LPMYRECORD lpRecord) {

	if (_stprintf(lpRecord->connection, _T("%02d:%02d:%02d"),
		lpRecord->connectionT.hour,
		lpRecord->connectionT.min,
		lpRecord->connectionT.sec) != 3) {
		return FALSE;
	}

	return TRUE;
}

BOOL setRecordAccessDateTimeFromString(LPMYRECORD lpRecord) {

	if (_stscanf(lpRecord->access, _T("%04d/%02d/%02d:%02d:%02d:%02d  "),
		&lpRecord->accessDT.date.year,
		&lpRecord->accessDT.date.month,
		&lpRecord->accessDT.date.day,
		&lpRecord->accessDT.time.hour,
		&lpRecord->accessDT.time.min,
		&lpRecord->accessDT.time.sec) != 6) {
		return FALSE;
	}

	return TRUE;
}

BOOL setRecordConnectionTimeFromString(LPMYRECORD lpRecord) {

	if (_stscanf(lpRecord->connection, _T("%02d:%02d:%02d"),
		&lpRecord->connectionT.hour,
		&lpRecord->connectionT.min,
		&lpRecord->connectionT.sec) != 3) {
		return FALSE;
	}

	return TRUE;
}