#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define MAXLEN 30
#define BUFLEN 83

typedef struct student {
	DWORD id;
	DWORD rnum;
	TCHAR surname[MAXLEN];
	TCHAR name[MAXLEN];
	DWORD mark;
} student_t;

BOOL displayMenu(HANDLE file);
BOOL readStudent(HANDLE file, DWORD n);
BOOL writeStudent(HANDLE file, DWORD n);

INT _tmain(INT argc, LPTSTR argv[]) {

	HANDLE file;

	// Check arguments
	if (argc != 2) {
		_ftprintf(stderr, _T("Usage: %s dbFile\n"), argv[0]);
		return 1;
	}

	// Open dbFile with CreateFile
	file = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot open dbFile. Error: %x\n"), GetLastError());
		return 2;
	}

	// Main loop
	while (displayMenu(file));

	// Close file
	CloseHandle(file);

	return 0;
}

BOOL displayMenu(HANDLE file) {

	DWORD n;
	TCHAR letter, buf[MAXLEN];

	// Display menu
	_tprintf(_T("*** MENU ***\n1. R n: read student n\n2. W n: write student n\nChoice: "));

	// Read input
	_fgetts(buf, MAXLEN, stdin);
	if (_stscanf(buf, _T("%c"), &letter) != 1) {
		_ftprintf(stderr, _T("Cannot _stscanf(). Error: %x\n"), GetLastError());
		return FALSE;
	}

	// Switch command
	switch (letter) {

		//Read student
	case 'R':
		if (_stscanf(buf, _T("%*c %d"), &n) != 1) {
			_ftprintf(stderr, _T("Missing second argument!\n\n"));
			return TRUE;
		}
		if ((n - 1) * sizeof(student_t) >= GetFileSize(file, NULL)) {
			_ftprintf(stderr, _T("Student ID not present in the database!\n\n"));
			return TRUE;
		}
		return readStudent(file, n);

		// Overwrite or add student
	case 'W':
		if (_stscanf(buf, _T("%*c %d"), &n) != 1) {
			_ftprintf(stderr, _T("Missing second argument!\n\n"));
			return TRUE;
		}
		return writeStudent(file, n);

		// Exit
	case 'E':
		return FALSE;

		// Default
	default:
		break;
	}

	return TRUE;
}

BOOL readStudent(HANDLE file, DWORD n) {

	DWORD read;
	LARGE_INTEGER offset;
	student_t s;

	// Prepare pointers
	ZeroMemory(&offset, sizeof(offset));
	offset.QuadPart = (LONGLONG)(n - 1) * sizeof(student_t);

	/* Version B */
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };
	ov.Offset = offset.LowPart;
	ov.OffsetHigh = offset.HighPart;

	/* Version C */
	LARGE_INTEGER range;
	range.QuadPart = sizeof(student_t);

	// LockFile
	if (LockFileEx(file, LOCKFILE_EXCLUSIVE_LOCK, 0, range.LowPart, range.HighPart, &ov) == 0) {
		_ftprintf(stderr, _T("Cannot LockFileEx(). Error: %x\n"), GetLastError());
		return TRUE;
	}

	// Read student information from file
	if (!ReadFile(file, &s, sizeof(student_t), &read, &ov)) {
		_ftprintf(stderr, _T("Cannot ReadFile(). Error: %x\n"), GetLastError());
		if (UnlockFileEx(file, 0, range.LowPart, range.HighPart, &ov) == 0) {
			_ftprintf(stderr, _T("Cannot UnlockFileEx(). Error: %x\n"), GetLastError());
		}
		return FALSE;
	}

	// UnlockFile
	if (UnlockFileEx(file, 0, range.LowPart, range.HighPart, &ov) == 0) {
		_ftprintf(stderr, _T("Cannot UnlockFileEx(). Error: %x\n"), GetLastError());
	}

	if (read != sizeof(student_t)) {
		_ftprintf(stderr, _T("Read less bytes than requested.\n"));
		return FALSE;
	}

	// Print student information
	_tprintf(_T("\nStudent %d:\n%d %d %s %s %d\n\n"), n, s.id, s.rnum, s.surname, s.name, s.mark);

	return TRUE;
}

BOOL writeStudent(HANDLE file, DWORD n) {

	DWORD written;
	TCHAR buf[BUFLEN];
	LARGE_INTEGER offset;
	student_t s;

	// Prepare pointers
	ZeroMemory(&offset, sizeof(offset));
	offset.QuadPart = (LONGLONG)(n - 1) * sizeof(student_t);

	// Read student information from stdin
	_tprintf(_T("Insert user data: "));
	_fgetts(buf, BUFLEN, stdin);
	if (_stscanf(buf, _T("%d %d %s %s %d"), &s.id, &s.rnum, s.surname, s.name, &s.mark) != 5) {
		_ftprintf(stderr, _T("Not enough fields!\n\n"));
		return TRUE;
	}

	// Constrain n and id to be equals
	if (s.id != n) {
		_ftprintf(stderr, _T("Error! n and student id are not equals.\n\n"));
		return TRUE;
	}

	/* Version B */
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };
	ov.Offset = offset.LowPart;
	ov.OffsetHigh = offset.HighPart;

	/* Version C */
	LARGE_INTEGER range;
	range.QuadPart = sizeof(student_t);

	// LockFile
	if (LockFileEx(file, LOCKFILE_EXCLUSIVE_LOCK, 0, range.LowPart, range.HighPart, &ov) == 0) {
		_ftprintf(stderr, _T("Cannot LockFileEx(). Error: %x\n"), GetLastError());
		return TRUE;
	}

	// Write student information to file
	if (!WriteFile(file, &s, sizeof(student_t), &written, &ov)) {
		_ftprintf(stderr, _T("Cannot WriteFile(). Error: %x\n"), GetLastError());
		if (UnlockFileEx(file, 0, range.LowPart, range.HighPart, &ov) == 0) {
			_ftprintf(stderr, _T("Cannot UnlockFileEx(). Error: %x\n"), GetLastError());
		}
		return FALSE;
	}

	// UnlockFile
	if (UnlockFileEx(file, 0, range.LowPart, range.HighPart, &ov) == 0) {
		_ftprintf(stderr, _T("Cannot UnlockFileEx(). Error: %x\n"), GetLastError());
	}

	if (written != sizeof(student_t)) {
		_ftprintf(stderr, _T("Written less bytes than requested.\n"));
		return FALSE;
	}

	_tprintf(_T("\n"));

	return TRUE;
}