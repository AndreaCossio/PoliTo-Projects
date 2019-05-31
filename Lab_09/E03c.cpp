#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

#define BUFLEN 512

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

// Globals
DWORD id, nThread;
HANDLE event1, event2;
CRITICAL_SECTION cs;

// Thread argument
typedef struct {
	int index;
	TCHAR dir[BUFLEN], filename[20];
	FILE* file;
} TARG, * PTARG;

// Prototypes
static DWORD WINAPI threadRoutine(PTARG arg);
static DWORD WINAPI threadOutput(PTARG tArgs);
static VOID TraverseDirectoryRecursive(LPTSTR path, FILE* file);
static DWORD FileType(LPWIN32_FIND_DATA pFileData);

// Main
INT _tmain(INT argc, LPTSTR argv[]) {

	int i;
	DWORD len;
	TCHAR wd[BUFLEN];
	HANDLE* tHandles, tOut;
	PTARG tArgs;

	// Check arguments
	if (argc < 2) {
		_ftprintf(stderr, _T("Usage: %s dir1 [dir2 dir3 ... dirN]\n"), argv[0]);
		return 1;
	}
	nThread = argc - 1;

	// Create events
	event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (event1 == NULL || event2 == NULL) {
		_ftprintf(stderr, _T("Cannot CreateEvent(). Error: %x\n"), GetLastError());
		return 2;
	}

	// Initialize Critical Section
	InitializeCriticalSection(&cs);

	// Allocate arrays
	tHandles = (HANDLE*)malloc(nThread * sizeof(HANDLE));
	tArgs = (PTARG)malloc(nThread * sizeof(TARG));
	if (tHandles == NULL || tArgs == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		return 3;
	}

	// Get current directory
	if ((len = GetCurrentDirectory(BUFLEN, wd)) == 0 || len > BUFLEN) {
		_ftprintf(stderr, _T("Cannot GetCurrentDirectory(). Error: %x\n"), GetLastError());
		return 4;
	}

	// Generate threads
	for (i = 0; i < nThread; i++) {
		// Adjust paths: if they are not absolute, append them to the current directory
		if (_tcsstr(argv[i + 1], _T(":")) == NULL) {
			_stprintf(tArgs[i].dir, _T("%s\\%s"), wd, argv[i + 1]);
		}
		else _stprintf(tArgs[i].dir, _T("%s"), argv[i + 1]);
		tArgs[i].index = i;
		tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadRoutine, &(tArgs[i]), 0, NULL);
	}

	// Output thread
	tOut = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadOutput, tArgs, 0, NULL);

	// Wait for threads
	WaitForMultipleObjects(nThread, tHandles, TRUE, INFINITE);
	WaitForSingleObject(tOut, INFINITE);

	// Close handles
	for (i = 0; i < nThread; i++) {
		CloseHandle(tHandles[i]);
	}
	CloseHandle(tOut);
	CloseHandle(event1);
	CloseHandle(event2);

	// Free space
	free(tHandles);
	free(tArgs);
	DeleteCriticalSection(&cs);

	return 0;
}

static DWORD WINAPI threadRoutine(PTARG arg) {

	// Create filename for output
	_stprintf(arg->filename, _T("%d.txt"), GetCurrentThreadId());

	// Display message
	_tprintf(_T("(%d)\t(INFO)\tTraversing %s. Output filename: %s\n"), GetCurrentThreadId(), arg->dir, arg->filename);

	// Open file
	if ((arg->file = _tfopen(arg->filename, _T("w"))) == NULL) {
		_ftprintf(stderr, _T("Cannot _tfopen(). Error: %x\n"), GetLastError());
		_endthreadex(1);
	}

	// Traverse directory
	TraverseDirectoryRecursive(arg->dir, arg->file);
	
	fclose(arg->file);

	// Enter critical section
	EnterCriticalSection(&cs);
	_tprintf(_T("(%d)\t(INFO)\tEntering critical section.\n"), GetCurrentThreadId());

	id = arg->index;

	// Signal event1
	SetEvent(event1);
	_tprintf(_T("(%d)\t(INFO)\tSignaled event1.\n"), GetCurrentThreadId());

	// Wait event2
	WaitForSingleObject(event2, INFINITE);
	_tprintf(_T("(%d)\t(INFO)\tReceived event2.\n"), GetCurrentThreadId());

	// Leave critical section
	LeaveCriticalSection(&cs);
	_tprintf(_T("(%d)\t(INFO)\tLeaving critical section.\n"), GetCurrentThreadId());

	_endthreadex(0);
	return 0;
}

static DWORD WINAPI threadOutput(PTARG tArgs) {

	int i;
	TCHAR row[BUFLEN];

	// Display message
	_tprintf(_T("(%d)\t(INFO)\tOutput thread.\n"), GetCurrentThreadId());

	// Thread loop
	for (i = 0; i < nThread; i++) {
		// Wait event1
		WaitForSingleObject(event1, INFINITE);
		_tprintf(_T("(%d)\t(INFO)\tReceived event1.\n"), GetCurrentThreadId());

		// Open file
		if ((tArgs[id].file = _tfopen(tArgs[id].filename, _T("r"))) == NULL) {
			_ftprintf(stderr, _T("Cannot _tfopen(). Error: %x\n"), GetLastError());
			_endthreadex(1);
		}

		// Scan file
		while (_fgetts(row, BUFLEN, tArgs[id].file) != NULL) {
			_tprintf(_T("(%d)\t(INFO)\t%s"), GetCurrentThreadId(), row);
		}

		// Close file and delete
		fclose(tArgs[id].file);
		if (!DeleteFile(tArgs[id].filename)) {
			_ftprintf(stderr, _T("Cannot DeleteFile(). Error: %x\n"), GetLastError());
			_endthreadex(2);
		}
		
		// Signal event2
		SetEvent(event2);
		_tprintf(_T("(%d)\t(INFO)\tSignaled event2.\n"), GetCurrentThreadId());
	}

	_endthreadex(0);
	return 0;
}

static VOID TraverseDirectoryRecursive(LPTSTR path, FILE * file) {

	DWORD FType, i;
	TCHAR newPath[BUFLEN], search[BUFLEN];
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;

	// newPath = destPath + entry
	if (path[_tcslen(path) - 1] == _T('\\')) {
		_stprintf(search, _T("%s*"), path);
	}
	else {
		_stprintf(search, _T("%s\\*"), path);
	}

	// Search entries
	if ((SearchHandle = FindFirstFile(search, &FindData)) == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot FindFirstFile(). Error: %x\n"), GetLastError());
		return;
	}

	// Iterate over entries
	do {
		FType = FileType(&FindData);

		// newPath = destPath + entry
		if (path[_tcslen(path) - 1] == _T('\\')) {
			_stprintf(newPath, _T("%s%s"), path, FindData.cFileName);
		}
		else {
			_stprintf(newPath, _T("%s\\%s"), path, FindData.cFileName);
		}

		// File
		if (FType == TYPE_FILE) {
			_ftprintf(file, _T("(%d)\t(FILE)\t%s\n"), GetCurrentThreadId(), newPath);
		}

		// Directory
		if (FType == TYPE_DIR) {
			_ftprintf(file, _T("(%d)\t(DIR)\t%s\n"), GetCurrentThreadId(), newPath);
			// Recur
			TraverseDirectoryRecursive(newPath, file);
		}
	} while (FindNextFile(SearchHandle, &FindData));

	FindClose(SearchHandle);
}

static DWORD FileType(LPWIN32_FIND_DATA pFileData) {

	DWORD FType = TYPE_FILE;

	if ((pFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
		if (lstrcmp(pFileData->cFileName, _T(".")) == 0 || lstrcmp(pFileData->cFileName, _T("..")) == 0) {
			FType = TYPE_DOT;
		}
		else FType = TYPE_DIR;
	}

	return FType;
}