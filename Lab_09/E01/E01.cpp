#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <process.h>

// Thread argument
typedef struct {
	DWORD n, * arr;
	LPTSTR filename;
} TARG, * PTARG;

// Prototypes
static DWORD WINAPI threadRoutine(PTARG arg);
BOOL prepareFiles(int argc, LPTSTR argv[]);
VOID mergeWrapper(DWORD *arr, int n);

// Main
INT _tmain(INT argc, LPTSTR argv[]) {

	int i, j;
	DWORD tot = 0, got = 0, min, pos, *res;
	HANDLE* tHandles, check;
	PTARG tArgs;

	// Check arguments
	if (argc < 3) {
		_ftprintf(stderr, _T("Usage: %s inputFile [inputFiles] outputFile\n"), argv[0]);
		return 1;
	}

	// Create binary files if they do not exist
	if ((check = CreateFile(argv[1], GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		if (!prepareFiles(argc, argv)) {
			return 2;
		}
	}
	else CloseHandle(check);

	// Allocate arrays
	tHandles = (HANDLE*)malloc((argc - 2) * sizeof(HANDLE));
	tArgs = (PTARG)malloc((argc - 2) * sizeof(TARG));
	if (tHandles == NULL || tArgs == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		return 3;
	}

	// Generate threads
	for (i = 0; i < argc - 2; i++) {
		tArgs[i].filename = argv[i + 1];
		tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)threadRoutine, &(tArgs[i]), 0, NULL);
	}

	// Wait threads
	WaitForMultipleObjects(argc-2, tHandles, TRUE, INFINITE);
	for (i = 0; i < argc - 2; i++) {
		CloseHandle(tHandles[i]);
		for (j = 0; j < tArgs[i].n; j++) {
			_tprintf(_T("%d "), tArgs[i].arr[j]);
		}
		_tprintf(_T("\n"));
		tot += tArgs[i].n;
	}

	res = (DWORD*)malloc(tot * sizeof(*res));
	if (res == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		return 4;
	}
	while (got < tot) {
		for (i = 0; i < argc - 2; i++) {
			if (tArgs[i].n > 0) {
				if (i == 0) {
					min = *tArgs[i].arr;
					pos = 0;
				}
				else {
					if (*tArgs[i].arr < min) {
						min = *tArgs[i].arr;
						pos = i;
					}
				}
			}
		}
		res[got] = min;
		tArgs[pos].arr++;
		tArgs[pos].n--;
		got++;
	}

	for (i = 0; i < tot; i++) {
		_tprintf(_T("%d "), res[i]);
	}
	_tprintf(_T("\n"));

	free(tHandles);
	free(tArgs);
	free(res);

	return 0;
}

static DWORD WINAPI threadRoutine(PTARG arg) {

	DWORD * n, * arr, read;
	LPTSTR filename;
	HANDLE file;

	n = &(arg->n);
	filename = arg->filename;

	// Open file
	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		_ftprintf(stderr, _T("Cannot CreateFile() %s. Error: %x\n"), filename, GetLastError());
		_endthreadex(1);
	}

	// Read the size of the array
	if (!ReadFile(file, n, sizeof(*n), &read, NULL)) {
		_ftprintf(stderr, _T("Cannot ReadFile() %s. Error: %x\n"), filename, GetLastError());
		_endthreadex(2);
	}
	if (read != sizeof(*n)) {
		_ftprintf(stderr, _T("Read less bytes than requested.\n"));
		_endthreadex(3);
	}

	// Allocate the array
	arg->arr = (DWORD*)malloc(*n * sizeof(DWORD));
	arr = arg->arr;
	if (arr == NULL) {
		_ftprintf(stderr, _T("Cannot malloc(). Error: %x\n"), GetLastError());
		_endthreadex(4);
	}

	// Read the array
	if (!ReadFile(file, arr, *n * sizeof(*arr), &read, NULL)) {
		_ftprintf(stderr, _T("Cannot ReadFile() %s. Error: %x\n"), filename, GetLastError());
		_endthreadex(5);
	}
	if (read != *n * sizeof(*arr)) {
		_ftprintf(stderr, _T("Read less bytes than requested.\n"));
		_endthreadex(6);
	}

	// Sort the array
	mergeWrapper(arr, *n);

	_endthreadex(0);
	return 0;
}

BOOL prepareFiles(int argc, LPTSTR argv[]) {

	int i, j;
	DWORD n, r, written;
	HANDLE file;

	// Set seed
	srand((unsigned)time(NULL));

	// Iterate over each input file
	for (i = 0; i < argc - 2; i++) {
		// Open file
		file = CreateFile(argv[i + 1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			_ftprintf(stderr, _T("Cannot CreateFile() %s. Error: %x\n"), argv[i+1], GetLastError());
			return FALSE;
		}

		// Generate a random number of elements
		n = rand() % 20 + 1;

		// Write the number of elements in the file
		if (!WriteFile(file, &n, sizeof(n), &written, NULL)) {
			_ftprintf(stderr, _T("Cannot WriteFile() %s. Error: %x\n"), argv[i+1], GetLastError());
			return FALSE;
		}
		if (written != sizeof(n)) {
			_ftprintf(stderr, _T("Written less bytes than requested.\n"));
			return FALSE;
		}

		// Generate the random numbers and write them to the file
		for (j = 0; j < n; j++) {
			r = rand() % 100;
			if (!WriteFile(file, &r, sizeof(r), &written, NULL)) {
				_ftprintf(stderr, _T("Cannot WriteFile() %s. Error: %x\n"), argv[i + 1], GetLastError());
				return FALSE;
			}
			if (written != sizeof(r)) {
				_ftprintf(stderr, _T("Written less bytes than requested.\n"));
				return FALSE;
			}
		}

		// Close file
		CloseHandle(file);
	}

	return TRUE;
}

VOID merge(DWORD* arr, DWORD* arr2, int l, int r, int q) {
	int i = l, j = q + 1, k;
	for (k = l; k <= r; k++) {
		if (i > q) {
			arr2[k] = arr[j++];
		}
		else if (j > r || arr[i] <= arr[j]) {
			arr2[k] = arr[i++];
		}
		else {
			arr2[k] = arr[j++];
		}
	}
	for (k = l; k <= r; k++) {
		arr[k] = arr2[k];
	}
	return;
}

VOID mergeSort(DWORD* arr, DWORD* arr2, int l, int r) {
	int q = (l + r) / 2;
	if (r <= l) {
		return;
	}
	mergeSort(arr, arr2, l, q);
	mergeSort(arr, arr2, q + 1, r);
	merge(arr, arr2, l, r, q);
	return;
}

VOID mergeWrapper(DWORD* arr, int n) {
	int l = 0, r = n - 1;
	DWORD* arr2 = (DWORD*)malloc(n * sizeof(DWORD));
	mergeSort(arr, arr2, l, r);
	return;
}