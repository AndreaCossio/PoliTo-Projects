#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

typedef struct {
	DWORD id, time_A, time_T, cars;
	DWORD n;
	LPHANDLE sem, busy;
} TARG, * LPTARG;

static DWORD WINAPI tSpawn(LPVOID arg);
static DWORD WINAPI tCar(LPVOID arg);
VOID ReportInfo(LPCTSTR str, ...);
VOID ReportError(LPCTSTR str);

INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD i;
	HANDLE semaphores[2], busy;
	HANDLE tSpawns[2];
	TARG tArgs[2];

	// Check args
	if (argc != 7) {
		_ftprintf(stderr, _T("Usage: %s time_A_L2R time_A_R2L time_T_L2R time_T_R2L number_L2R number_R2L\n"), argv[0]);
		return 1;
	}

	// Create busy semaphore
	if ((busy = CreateSemaphore(NULL, 1, 1, NULL)) == NULL) {
		ReportError(_T("CreateSemaphore"));
		ExitProcess(2);
	}

	// Generate master threads
	for (i = 0; i < 2; i++) {
		// Mutex semaphore
		if ((semaphores[i] = CreateSemaphore(NULL, 1, 1, NULL)) == NULL) {
			ReportError(_T("CreateSemaphore"));
			ExitProcess(3);
		}
		// Arguments
		tArgs[i].id = i;
		tArgs[i].time_A = _tstoi(argv[1+i]);
		tArgs[i].time_T = _tstoi(argv[3+i]);
		tArgs[i].cars = _tstoi(argv[5+i]);
		tArgs[i].n = 0;
		tArgs[i].sem = &(semaphores[i]);
		tArgs[i].busy = &busy;
		// Thread
		if ((tSpawns[i] = (HANDLE)CreateThread(NULL, 0, tSpawn, &(tArgs[i]), 0, NULL)) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(4);
		}
	}

	// Wait producers
	WaitForMultipleObjects(2, tSpawns, TRUE, INFINITE);

	// Release resources
	CloseHandle(tSpawns[0]);
	CloseHandle(tSpawns[1]);

	return 0;
}

static DWORD WINAPI tSpawn(LPVOID arg) {

	DWORD i, n;
	LPTARG param = (LPTARG)arg;
	LPHANDLE carHandles;

	srand(time(NULL));

	ReportInfo(_T("%s thread. t_A: %d t_T: %d number: %d\n"), (param->id == 0) ? _T("L2R") : _T("R2L"), param->time_A, param->time_T, param->cars);

	if ((carHandles = (LPHANDLE)malloc(param->cars * sizeof(HANDLE))) == NULL) {
		ReportError(_T("malloc"));
		ExitProcess(4);
	}

	for (i = 0; i < param->cars; i++) {
		Sleep(rand() % (param->time_A * 1000));
		if ((carHandles[i] = (HANDLE)CreateThread(NULL, 0, tCar, arg, 0, NULL)) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(5);
		}
	}

	WaitForMultipleObjects(param->cars, carHandles, TRUE, INFINITE);

	ExitThread(0);
}

static DWORD WINAPI tCar(LPVOID arg) {

	LPTARG param = (LPTARG)arg;

	srand(time(NULL));

	// Random arrival

	WaitForSingleObject(*(param->sem), INFINITE);
	param->n++;
	if (param->n == 1) {
		WaitForSingleObject(*(param->busy), INFINITE);
	}
	ReportInfo(_T("Entered %s\t%d\n"), (param->id == 0) ? _T("L2R") : _T("R2L"), param->n);
	ReleaseSemaphore(*(param->sem), 1, NULL);

	// Random passing time
	Sleep(param->time_T * 1000);

	WaitForSingleObject(*(param->sem), INFINITE);
	param->n--;
	if (param->n == 0) {
		ReleaseSemaphore(*(param->busy), 1, NULL);
	}
	ReportInfo(_T("Exited %s\t%d\n"), (param->id == 0) ? _T("L2R") : _T("R2L"), param->n);
	ReleaseSemaphore(*(param->sem), 1, NULL);

	ExitThread(0);
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