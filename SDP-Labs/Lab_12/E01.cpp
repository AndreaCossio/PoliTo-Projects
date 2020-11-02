#define _CRT_SECURE_NO_WARNINGS

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#include <time.h>
#include "myDateAndTime.h"
#include "myRecord.h"

#define DIRLEN 1024

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

// Server data for R&W
typedef struct {
	DWORD nReaders;
	HANDLE meReaders, meWriters, precedence;
} MERW, * LPMERW;

// Thread argument
typedef struct {
	DWORD id, nServer;
	LPHANDLE serverFile;
	LPMERW rwDataArray; // -> points to the array of all servers data
} TARG, * LPTARG;

static DWORD WINAPI tRoutine(LPVOID arg);
VOID traverseDir(LPTSTR dir, DWORD rw);
VOID getConnectionTime(LPTSTR filepath, LPMYDATETIME lastAccess, LPDWORD totalTime);
VOID modifyConnectionTime(LPTSTR filepath);
VOID ReportInfo(LPCTSTR str, ...);
VOID ReportError(LPCTSTR str);
static DWORD FileType(LPWIN32_FIND_DATA pFileData);

INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD i, nThread, nServer, read;
	HANDLE serverFile;
	LPHANDLE tHandles;
	LPTARG tArgs;
	LPMERW rwDataArray;

	// Check arguments
	if (argc < 3) {
		_ftprintf(stderr, _T("Usage: %s serverListFile nThread\n"), argv[0]);
		return 1;
	}
	nThread = _tstoi(argv[2]);

	// Open file
	if ((serverFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		return 2;
	}

	// Read number of servers
	if (!ReadFile(serverFile, &nServer, sizeof(nServer), &read, NULL) || read != sizeof(nServer)) {
		ReportError(_T("ReadFile"));
		return 3;
	}

	// Allocate space
	if ((tArgs = (LPTARG)malloc(nThread * sizeof(TARG))) == NULL ||
		(tHandles = (LPHANDLE)malloc(nThread * sizeof(HANDLE))) == NULL ||
		(rwDataArray = (LPMERW)malloc(nServer * sizeof(MERW))) == NULL) {
		ReportError(_T("malloc"));
		return 4;
	}

	// Create sempahores
	for (i = 0; i < nServer; i++) {
		rwDataArray[i].nReaders = 0;
		if ((rwDataArray[i].meReaders = CreateSemaphore(NULL, 1, 1, NULL)) == NULL ||
			(rwDataArray[i].meWriters = CreateSemaphore(NULL, 1, 1, NULL)) == NULL ||
			(rwDataArray[i].precedence = CreateSemaphore(NULL, 1, 1, NULL)) == NULL) {
			ReportError(_T("CreateSemaphore"));
			return 5;
		}
	}

	// Create threads
	for (i = 0; i < nThread; i++) {
		tArgs[i].nServer = nServer;
		tArgs[i].serverFile = &serverFile;
		tArgs[i].rwDataArray = rwDataArray;
		if ((tHandles[i] = (HANDLE)CreateThread(NULL, 0, tRoutine, &(tArgs[i]), 0, &(tArgs[i].id))) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(5);
		}
	}

	// Wait threads
	WaitForMultipleObjects(nThread, tHandles, TRUE, INFINITE);
	for (i = 0; i < nThread; i++) {
		CloseHandle(tHandles[i]);
	}

	// Close semaphores
	for (i = 0; i < nServer; i++) {
		CloseHandle(rwDataArray[i].meReaders);
		CloseHandle(rwDataArray[i].meWriters);
		CloseHandle(rwDataArray[i].precedence);
	}

	CloseHandle(serverFile);

	free(tHandles);
	free(tArgs);
	free(rwDataArray);

	return 0;
}

static DWORD WINAPI tRoutine(LPVOID arg) {

	DWORD n, read;
	TCHAR dir[DIRLEN + 1];
	FLOAT n1, n2, n3;
	LPTARG param = (LPTARG)arg;
	LPMERW meData = NULL;
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };
	LARGE_INTEGER pos;

	// Set the rand() seed
	srand(GetCurrentThreadId() * time(NULL));

	// Generate the three random numbers
	n1 = (FLOAT)rand() / RAND_MAX;
	n2 = (FLOAT)rand() / RAND_MAX;
	n3 = (FLOAT)rand() / RAND_MAX;
	n = (DWORD) floor(n3 * param->nServer);

	// Prepare params
	pos.QuadPart = (LONGLONG)n * DIRLEN * sizeof(TCHAR) + 4;
	ov.Offset = pos.LowPart;
	ov.OffsetHigh = pos.HighPart;
	meData = &(param->rwDataArray[n]);

	// Read the directory name
	if (ReadFile(*(param->serverFile), dir, DIRLEN * sizeof(TCHAR), &read, &ov) && read == DIRLEN * sizeof(TCHAR)) {
		dir[DIRLEN] = _T('\0');
		_stscanf(dir, _T("%s "), dir);
	}

	// Message
	ReportInfo(_T("Sleeping: %ds\t%s server n: %d\tDir: %s\n"), (DWORD)(n1 * 100.0), (n2 >= 0.5) ? _T("Writing") : _T("Reading"), n+1, dir);

	// Sleep n1 * 100 seconds
	Sleep(n1 * 100000);

	// Reader
	if (n2 < 0.5) {
		// Reader's entry
		WaitForSingleObject(meData->meReaders, INFINITE);
		meData->nReaders++;
		if (meData->nReaders == 1) {
			WaitForSingleObject(meData->precedence, INFINITE);
		}
		ReleaseSemaphore(meData->meReaders, 1, NULL);

		// Read
		traverseDir(dir, 0);

		// Reader's exit
		WaitForSingleObject(meData->meReaders, INFINITE);
		meData->nReaders--;
		if (meData->nReaders == 0) {
			ReleaseSemaphore(meData->precedence, 1, NULL);
		}
		ReleaseSemaphore(meData->meReaders, 1, NULL);
	}
	// Writer
	else {
		// Writer's entry
		WaitForSingleObject(meData->meWriters, INFINITE);
		WaitForSingleObject(meData->precedence, INFINITE);

		// Write
		traverseDir(dir, 1);

		// Writer's exit
		ReleaseSemaphore(meData->precedence, 1, NULL);
		ReleaseSemaphore(meData->meWriters, 1, NULL);
	}

	ExitThread(0);
}

// rw == 0: Iterates over the files of a directory and reports the total connection time and the last access
// rw == 1: Iterates over the files of a directory and randomly modifies them
VOID traverseDir(LPTSTR dir, DWORD rw) {

	DWORD totalTime = 0;
	TCHAR search[DIRLEN], filepath[DIRLEN];
	HANDLE searchHandle;
	MYDATETIME lastAccess = { { 0, 0, 0 }, { 0, 0, 0 } };
	WIN32_FIND_DATA findData;

	// Search path
	_stprintf(search, _T("%s\\*"), dir);

	// Search entries
	if ((searchHandle = FindFirstFile(search, &findData)) == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot FindFirstFile(). Error: %x\n"), GetLastError());
		return;
	}

	// Iterate over files
	do {
		if (FileType(&findData) == TYPE_FILE) {
			_stprintf(filepath, _T("%s\\%s"), dir, findData.cFileName);
			if (rw == 0) {
				getConnectionTime(filepath, &lastAccess, &totalTime);
			}
			else {
				modifyConnectionTime(filepath);
			}
		}
	} while (FindNextFile(searchHandle, &findData));

	// Close search
	FindClose(searchHandle);

	// Message
	if (rw == 0) {
		ReportInfo(_T("Directory: %s\tTotal time: %d\tLast access:  %04d/%02d/%02d:%02d:%02d:%02d\n"), dir, totalTime, lastAccess.date.year, lastAccess.date.month, lastAccess.date.day, lastAccess.time.hour, lastAccess.time.min, lastAccess.time.sec);
	}

	return;
}

// Returns the total access time of the access file specified in filepath and updates the last access
VOID getConnectionTime(LPTSTR filepath, LPMYDATETIME lastAccess, LPDWORD totalTime) {

	HANDLE file;
	MYRECORD r;

	// Open access file
	if ((file = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		return;
	}

	// Read access file
	while (readRecord(&r, &file)) {
		*totalTime += getTimeSeconds(r.connectionT);
		if (compareDateTime(r.accessDT, *lastAccess)) {
			*lastAccess = r.accessDT;
		}
	}

	// Close file
	CloseHandle(file);

	return;
}

// Randomly modifies the date and the time of each access in the given file
VOID modifyConnectionTime(LPTSTR filepath) {

	TCHAR copy[DIRLEN+2];
	HANDLE file, fileCopy;
	MYRECORD r;

	// Open access file
	if ((file = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		return;
	}

	// Open backup file
	_stprintf(copy, _T("%s_copy"), filepath);
	if ((fileCopy = CreateFile(copy, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		CloseHandle(file);
		return;
	}

	// Read access file
	while (readRecord(&r, &file)) {
		updateRecordAccess(&r, getRandomDateTime(2000, 2018));
		updateRecordConnection(&r, getRandomTime());
		if (!writeRecord(&r, &fileCopy)) {
			CloseHandle(file);
			CloseHandle(fileCopy);
			DeleteFile(copy);
			return;
		}
	}

	// Close files
	CloseHandle(file);
	CloseHandle(fileCopy);

	CopyFile(copy, filepath, FALSE);
	DeleteFile(copy);

	return;
}

VOID ReportInfo(LPCTSTR str, ...) {
	va_list arg;
	va_start(arg, str);
	_ftprintf(stderr, _T("(%d)\t(INFO)\t"), GetCurrentThreadId());
	_vftprintf(stderr, str, arg);
	va_end(arg);
	return;
}

VOID ReportError(LPCTSTR str) {
	LPTSTR message = 0;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, (LPTSTR)& message, 0, NULL);
	_ftprintf(stderr, _T("(%d)\t(ERROR)\tCannot %s(). Error: %s"), GetCurrentThreadId(), str, message);
	LocalFree(message);
	return;
}

// Returns the fileType of the given fileData
static DWORD FileType(LPWIN32_FIND_DATA pFileData) {

	DWORD fType = TYPE_FILE;

	if ((pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
		if (lstrcmp(pFileData->cFileName, _T(".")) == 0 || lstrcmp(pFileData->cFileName, _T("..")) == 0) {
			fType = TYPE_DOT;
		}
		else fType = TYPE_DIR;
	}

	return fType;
}