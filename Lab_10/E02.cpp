#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

#define BUFLEN 2048

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

// Globals
DWORD NThread, Finish;
HANDLE EventUnlock, Sem;

// Thread argument
typedef struct {
	BOOL finished;
	TCHAR basedir[BUFLEN], compare[BUFLEN];
} TARG, * LPTARG;

// Prototypes
static DWORD WINAPI threadRead(LPVOID arg);
static DWORD WINAPI threadCompare(LPVOID tArgs);
static VOID TraverseDirectoryRecursive(LPTSTR basedir, LPTSTR entry, LPTSTR compare);
static DWORD FileType(LPWIN32_FIND_DATA pFileData);

// Main
INT _tmain(INT argc, LPTSTR argv[]) {

	int i = 0;
	DWORD len, ec;
	TCHAR wd[BUFLEN];
	HANDLE* tReaders, tCompare;
	LPTARG tArgs;

	// Check arguments
	if (argc < 3) {
		_ftprintf(stderr, _T("Usage: %s dir1 dir2 [dir3 ... dirN]\n"), argv[0]);
		return 1;
	}
	NThread = argc - 1;
	Finish = 0;

	// Allocate arrays
	tReaders = (HANDLE*)malloc(NThread * sizeof(HANDLE));
	tArgs = (LPTARG)malloc(NThread * sizeof(TARG));
	if (tReaders == NULL || tArgs == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		return 2;
	}

	// Create event and semaphore
	if ((EventUnlock = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL) {
		_ftprintf(stderr, _T("Cannot CreateEvent(). Error: %x\n"), GetLastError());
		return 3;
	}
	if ((Sem = CreateSemaphore(NULL, 0, NThread, NULL)) == NULL) {
		_ftprintf(stderr, _T("Cannot CreateSemaphore(). Error: %x\n"), GetLastError());
		return 4;
	}

	// Get current directory
	if ((len = GetCurrentDirectory(BUFLEN, wd)) == 0 || len > BUFLEN) {
		_ftprintf(stderr, _T("Cannot GetCurrentDirectory(). Error: %x\n"), GetLastError());
		return 5;
	}

	// Generate threads
	for (i = 0; i < NThread; i++) {
		// Adjust paths: if they are not absolute, append them to the current directory
		if (_tcsstr(argv[i + 1], _T(":")) == NULL) {
			_stprintf(tArgs[i].basedir, _T("%s\\%s"), wd, argv[i + 1]);
		}
		else _stprintf(tArgs[i].basedir, _T("%s"), argv[i + 1]);
		tArgs[i].compare[0] = _T('\0');
		tArgs[i].finished = FALSE;
		tReaders[i] = CreateThread(NULL, 0, threadRead, &(tArgs[i]), 0, NULL);
	}

	// Compare thread
	tCompare = CreateThread(NULL, 0, threadCompare, tArgs, 0, NULL);

	// Wait for threads
	WaitForMultipleObjects(NThread, tReaders, TRUE, INFINITE);
	WaitForSingleObject(tCompare, INFINITE);
	GetExitCodeThread(tCompare, &ec);

	// Display result
	if (ec == 0) {
		_tprintf(_T("They ARE equals.\n"));
	}
	else if (ec == 1) {
		_tprintf(_T("They ARE PARTIALLY equals.\n"));
	}
	else {
		_tprintf(_T("They ARE NOT equals.\n"));
	}

	// Close handles
	for (i = 0; i < NThread; i++) {
		CloseHandle(tReaders[i]);
	}
	CloseHandle(tCompare);
	CloseHandle(EventUnlock);
	CloseHandle(Sem);

	// Free space
	free(tReaders);
	free(tArgs);

	return 0;
}

// Routine of reader threads
static DWORD WINAPI threadRead(LPVOID arg) {

	LPTARG param = (LPTARG)arg;

	// Traverse directory
	TraverseDirectoryRecursive(param->basedir, NULL, param->compare);

	// Set finished
	param->finished = TRUE;

	// This is to avoid the compare thread to reamin blocked
	ReleaseSemaphore(Sem, 1, NULL);

	ExitThread(0);
	return 0;
}

static DWORD WINAPI threadCompare(LPVOID tArgs) {

	int i, nFinished;
	LPTARG param = (LPTARG)tArgs;

	while (1) {
		nFinished = 0;

		// Wait nThread
		for (i = 0; i < NThread; i++) {
			WaitForSingleObject(Sem, INFINITE);
		}

		// Check if someone has finisched
		for (i = 0; i < NThread; i++) {
			if (param[i].finished) {
				nFinished++;
			}
		}

		// If someone has finished
		if (nFinished > 0) {
			// This global variable is used to tell the reader threads that they have to exit
			Finish = 1;
			// If all of the threads did finish, the directory trees are equals
			if (nFinished == NThread) {
				ExitThread(0);
			}
			// Othrewise the directory trees are partially equals
			else {
				PulseEvent(EventUnlock);
				ExitThread(1);
			}
		}

		// Compare entries
		for (i = 1; i < NThread; i++) {
			// If there is at least one entry that is different, exit
			if (_tcscmp(param[0].compare, param[i].compare) != 0) {
				Finish = 1;
				PulseEvent(EventUnlock);
				ExitThread(2);
			}
		}

		// Unlock reader threads
		PulseEvent(EventUnlock);
	}

	return 0;
}

static VOID TraverseDirectoryRecursive(LPTSTR basedir, LPTSTR relativedir, LPTSTR compare) {

	DWORD fType;
	TCHAR newPath[BUFLEN], search[BUFLEN];
	HANDLE searchHandle;
	WIN32_FIND_DATA findData;

	// Search path
	if (relativedir == NULL) {
		_stprintf(search, _T("%s\\*"), basedir);
	}
	else {
		_stprintf(search, _T("%s\\%s\\*"), basedir, relativedir);
	}

	// Search entries
	if ((searchHandle = FindFirstFile(search, &findData)) == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot FindFirstFile(). Error: %x\n"), GetLastError());
		exit(6);
	}

	// Iterate over entries
	do {
		// Check if compare thread has finished
		if (Finish == 1) {
			return;
		}
		fType = FileType(&findData);

		// Check entry
		if (fType == TYPE_FILE || fType == TYPE_DIR) {
			if (relativedir == NULL) {
				_stprintf(newPath, _T("%s"), findData.cFileName);
			}
			else {
				_stprintf(newPath, _T("%s\\%s"), relativedir, findData.cFileName);
			}
			_stprintf(compare, _T("%s"), newPath);
			// Unlock compare thread
			ReleaseSemaphore(Sem, 1, NULL);
			// Wait compare thread
			WaitForSingleObject(EventUnlock, INFINITE);
		}

		// Recursion
		if (fType == TYPE_DIR) {
			TraverseDirectoryRecursive(basedir, newPath, compare);
		}
	} while (FindNextFile(searchHandle, &findData));

	FindClose(searchHandle);
}

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