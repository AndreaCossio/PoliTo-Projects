#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define MAXLEN 30

typedef struct student {
	DWORD id;
	DWORD rnum;
	TCHAR surname[MAXLEN];
	TCHAR name[MAXLEN];
	DWORD mark;
} student_t;

INT _tmain(INT argc, LPTSTR argv[]) {

	FILE* fIn;
	HANDLE hOut;
	DWORD nOut;
	student_t s;

	if (argc != 3) {
		_ftprintf(stderr, _T("Usage: %s file1 file2\n"), argv[0]);
		return 1;
	}

	// Open input file with generic fopen
	fIn = _tfopen(argv[1], _T("r"));
	if (fIn == NULL) {
		_ftprintf(stderr, _T("Cannot open input file. Error: %x\n"), GetLastError());
		return 2;
	}

	// Open output file with CreateFile
	hOut = CreateFile(argv[2], GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOut == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot open output file. Error: %x\n"), GetLastError());
		fclose(fIn);
		return 3;
	}

	// Iterate over lines
	while (_ftscanf(fIn, _T("%d %d %s %s %d"), &s.id, &s.rnum, s.surname, s.name, &s.mark) == 5) {
		// Write to bin file
		WriteFile(hOut, &s, sizeof(student_t), &nOut, NULL);
		// Re-read to verify correctness
		if (ReadFile(hOut, &s, sizeof(student_t), NULL, NULL)) {
			_tprintf(_T("%d %d %s %s %d\n"), s.id, s.rnum, s.surname, s.name, s.mark);
		}
	}

	fclose(fIn);
	CloseHandle(hOut);
	return 0;
}