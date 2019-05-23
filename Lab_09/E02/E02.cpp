#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define N 512

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_DOT 3

static VOID TraverseDirectoryRecursive(LPTSTR path, LPTSTR destPath);
static DWORD FileType(LPWIN32_FIND_DATA pFileData);

// Main
INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD len;
	TCHAR wd[N], destPath[N];

	// Check arguments
	if (argc != 3) {
		_ftprintf(stderr, _T("Usage: %s dir1 dir2\n"), argv[0]);
		return 1;
	}

	// Get current directory
	if ((len = GetCurrentDirectory(N, wd)) == 0 || len > N) {
		_ftprintf(stderr, _T("Cannot GetCurrentDirectory(). Error: %x\n"), GetLastError());
		return 2;
	}

	// Adjust destination path: if it is not absolute, append it to the current directory
	if (_tcsstr(argv[2], _T(":")) == NULL) {
		_stprintf(destPath, _T("%s\\%s"), wd, argv[2]);
	}
	else _stprintf(destPath, _T("%s"), argv[2]);
	_tprintf(_T("Destination path: %s\n"), destPath);

	// Traverse directory
	TraverseDirectoryRecursive(argv[1], destPath);

	return 0;
}

static VOID TraverseDirectoryRecursive(LPTSTR path, LPTSTR destPath) {
	
	DWORD FType, i;
	TCHAR newPath[N];
	HANDLE SearchHandle;
	WIN32_FIND_DATA FindData;

	// Move inside the directory
	if (!SetCurrentDirectory(path)) {
		_ftprintf(stderr, _T("Cannot SetCurrentDirectory(). Error: %x\n"), GetLastError());
		return;
	}

	// Create the destination directory
	if (!CreateDirectory(destPath, NULL)) {
		if (GetLastError() != ERROR_ALREADY_EXISTS) {
			_ftprintf(stderr, _T("Cannot SetCurrentDirectory(). Error: %x\n"), GetLastError());
			return;
		}
	}
	else _tprintf(_T("(DIR)\t%s\n"), destPath);

	// Search entries
	if ((SearchHandle = FindFirstFile(_T("*"), &FindData)) == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot FindFirstFile(). Error: %x\n"), GetLastError());
		return;
	}

	// Iterate over entries
	do {
		FType = FileType(&FindData);

		// newPath = destPath + entry
		if (destPath[_tcslen(destPath) - 1] == _T('\\')) {
			_stprintf(newPath, _T("%s%s"), destPath, FindData.cFileName);
		}
		else {
			_stprintf(newPath, _T("%s\\%s"), destPath, FindData.cFileName);
		}

		// File
		if (FType == TYPE_FILE) {
			// Copy
			if (!CopyFile(FindData.cFileName, newPath, FALSE)) {
				_ftprintf(stderr, _T("Cannot CopyFile(). Error: %x\n"), GetLastError());
				return;
			}
			_tprintf(_T("(COPY)\t%s\n"), newPath);
		}

		// Directory
		if (FType == TYPE_DIR) {
			// Recur
			TraverseDirectoryRecursive(FindData.cFileName, newPath);

			// Go back of one dir
			if (!SetCurrentDirectory(_T(".."))) {
				_ftprintf(stderr, _T("Cannot SetCurrentDirectory(). Error: %x\n"), GetLastError());
				return;
			}
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