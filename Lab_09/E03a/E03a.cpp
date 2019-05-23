#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

#define BUFLEN 512

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

// Prototypes
static DWORD WINAPI threadRoutine(LPTSTR arg);
static VOID TraverseDirectoryRecursive(LPTSTR path);
static DWORD FileType(LPWIN32_FIND_DATA pFileData);

// Main
INT _tmain(INT argc, LPTSTR argv[]) {

	int i;
	DWORD len;
	TCHAR wd[BUFLEN], ** dirs;
	HANDLE* tHandles;

	// Check arguments
	if (argc < 2) {
		_ftprintf(stderr, _T("Usage: %s dir1 [dir2 dir3 ... dirN]\n"), argv[0]);
		return 1;
	}

	// Allocate arrays
	tHandles = (HANDLE*)malloc((argc - 1) * sizeof(HANDLE));
	dirs = (TCHAR * *)malloc((argc - 1) * sizeof(TCHAR*));
	if (tHandles == NULL || dirs == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		return 2;
	}

	// Get current directory
	if ((len = GetCurrentDirectory(BUFLEN, wd)) == 0 || len > BUFLEN) {
		_ftprintf(stderr, _T("Cannot GetCurrentDirectory(). Error: %x\n"), GetLastError());
		return 3;
	}

	// Generate threads
	for (i = 0; i < argc - 1; i++) {
		// Allocate space for temp dirname
		dirs[i] = (TCHAR*)malloc(BUFLEN * sizeof(TCHAR));
		if (dirs[i] == NULL) {
			_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
			return 4;
		}
		// Adjust paths: if they are not absolute, append them to the current directory
		if (_tcsstr(argv[i + 1], _T(":")) == NULL) {
			_stprintf(dirs[i], _T("%s\\%s"), wd, argv[i + 1]);
		}
		else _stprintf(dirs[i], _T("%s"), argv[i + 1]);
		tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadRoutine, dirs[i], 0, NULL);
	}

	// Wait for threads
	WaitForMultipleObjects(argc - 1, tHandles, TRUE, INFINITE);
	for (i = 0; i < argc - 1; i++) {
		// Free space and close handle
		free(dirs[i]);
		CloseHandle(tHandles[i]);
	}

	// Free space
	free(tHandles);
	free(dirs);

	return 0;
}

static DWORD WINAPI threadRoutine(LPTSTR arg) {

	// Display message
	_tprintf(_T("(%d)\t(INFO)\tTraversing %s.\n"), GetCurrentThreadId(), arg);
	
	// Traverse directory
	TraverseDirectoryRecursive(arg);

	_endthreadex(0);
	return 0;
}

static VOID TraverseDirectoryRecursive(LPTSTR path) {

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
			_tprintf(_T("(%d)\t(FILE)\t%s\n"), GetCurrentThreadId(), newPath);
		}

		// Directory
		if (FType == TYPE_DIR) {
			_tprintf(_T("(%d)\t(DIR)\t%s\n"), GetCurrentThreadId(), newPath);
			// Recur
			TraverseDirectoryRecursive(newPath);
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
