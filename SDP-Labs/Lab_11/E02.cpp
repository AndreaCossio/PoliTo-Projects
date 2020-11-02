#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

typedef struct {
	HANDLE meProducers, meConsumers, notEmpty, notFull;
	DWORD size, first, last;
	LPDWORD arr;
} QUEUE, * LPQUEUE;

typedef struct {
	DWORD t;
	LPQUEUE q;
} TARG, * LPTARG;

static DWORD WINAPI threadConsumer(LPVOID arg);
static DWORD WINAPI threadProducer(LPVOID arg);
VOID ReportInfo(LPCTSTR str, ...);
VOID ReportError(LPCTSTR str);
VOID QueueInit(LPQUEUE q, DWORD size);
VOID QueueDestroy(LPQUEUE q);
VOID QueuePut(LPQUEUE q, DWORD n);
VOID QueueGet(LPQUEUE q, LPDWORD n);
BOOL QueueEmpty(LPQUEUE q);
BOOL QueueFull(LPQUEUE q);
VOID QueueRemove(LPQUEUE q, LPDWORD n);
VOID QueueInsert(LPQUEUE q, DWORD n);

INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD i, P, C, N, T;
	QUEUE q;
	LPHANDLE tConsumers, tProducers;
	TARG tArg;

	// Check and parse args
	if (argc != 5) {
		_ftprintf(stderr, _T("Usage: %s P C N T\n"), argv[0]);
		return 1;
	}
	P = _tstoi(argv[1]);
	C = _tstoi(argv[2]);
	N = _tstoi(argv[3]);
	T = _tstoi(argv[4]);

	// Create queue
	QueueInit(&q, N);
	if (q.arr == NULL) {
		ReportError(_T("QueueInit"));
		return 2;
	}

	// Allocate space for threads' arguments
	if ((tProducers = (LPHANDLE)malloc(P * sizeof(HANDLE))) == NULL || 
		(tConsumers = (LPHANDLE)malloc(C * sizeof(HANDLE))) == NULL) {
		ReportError(_T("malloc"));
		return 3;
	}

	tArg.q = &q;
	tArg.t = T;

	// Generate consumers
	for (i = 0; i < C; i++) {
		if ((tConsumers[i] = (HANDLE)CreateThread(NULL, 0, threadConsumer, &tArg, 0, NULL)) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(4);
		}
	}

	// Generate producers
	for (i = 0; i < P; i++) {
		if ((tProducers[i] = (HANDLE)CreateThread(NULL, 0, threadProducer, &tArg, 0, NULL)) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(5);
		}
	}

	// Wait producers
	WaitForMultipleObjects(P, tProducers, TRUE, INFINITE);

	for (i = 0; i < C; i++) {
		QueuePut(&q, T);
		_tprintf(_T("Main generated: %d\n"), T);
	}

	WaitForMultipleObjects(C, tConsumers, TRUE, INFINITE);

	// Release resources
	for (i = 0; i < C; i++) {
		CloseHandle(tConsumers[i]);
	}
	for (i = 0; i < P; i++) {
		CloseHandle(tProducers[i]);
	}
	free(tConsumers);
	free(tProducers);

	QueueDestroy(&q);

	return 0;
}

static DWORD WINAPI threadProducer(LPVOID arg) {

	int i;
	DWORD n;
	LPTARG param = (LPTARG)arg;

	// Set seed for random function
	srand(time(NULL));

	for (i = 0; i < 5; i++) {
		n = rand();
		Sleep(rand() % (param->t * 1000));
		QueuePut(param->q, n);
		ReportInfo(_T("Produced: %d\n"), n);
	}

	ExitThread(0);
	return 0;
}

static DWORD WINAPI threadConsumer(LPVOID arg) {
	
	int i;
	DWORD n;
	LPTARG param = (LPTARG)arg;

	// Set seed for random function
	srand(time(NULL));

	while (1) {
		Sleep(rand() % (param->t * 1000));
		QueueGet(param->q, &n);
		ReportInfo(_T("Consumed: %d\n"), n);
		if (n == param->t) {
			break;
		}
	}
	
	ExitThread(0);
	return 0;
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

VOID QueueInit(LPQUEUE q, DWORD size) {
	q->arr = (LPDWORD)calloc(size, sizeof(DWORD));
	q->first = 0;
	q->last = 0;
	q->size = size;
	q->meProducers = CreateMutex(NULL, FALSE, NULL);
	q->meConsumers = CreateMutex(NULL, FALSE, NULL);
	q->notEmpty = CreateEvent(NULL, TRUE, FALSE, NULL);
	q->notFull = CreateEvent(NULL, TRUE, FALSE, NULL);
}

VOID QueueDestroy(LPQUEUE q) {
	free(q->arr);
	q->arr = NULL;
	CloseHandle(q->notEmpty);
	CloseHandle(q->notFull);
	CloseHandle(q->meConsumers);
	CloseHandle(q->meProducers);
	return;
}

VOID QueuePut(LPQUEUE q, DWORD n) {

	// Gain mutex
	WaitForSingleObject(q->meProducers, INFINITE);

	// Until the queue is full
	while (QueueFull(q)) {
		// Unlock mutex and wait some consumer to signal notFull (through QueueGet)
		SignalObjectAndWait(q->meProducers, q->notFull, INFINITE, FALSE);
		// Gain mutex again
		WaitForSingleObject(q->meProducers, INFINITE);
	}

	// Insert one element
	QueueInsert(q, n);

	// Signal that the queue is not empty
	PulseEvent(q->notEmpty);

	// Release mutex
	ReleaseMutex(q->meProducers);

	return;
}

VOID QueueGet(LPQUEUE q, LPDWORD n) {

	// Gain mutex
	WaitForSingleObject(q->meConsumers, INFINITE);

	// Until the queue is empty
	while (QueueEmpty(q)) {
		// Unlock mutex and wait some producer to signal notEmpty (through QueuePut)
		SignalObjectAndWait(q->meConsumers, q->notEmpty, INFINITE, FALSE);
		// Gain mutex again
		WaitForSingleObject(q->meConsumers, INFINITE);
	}

	// Take one elemnt
	QueueRemove(q, n);

	// Signal that the queue is now not full
	PulseEvent(q->notFull);

	// Release mutex
	ReleaseMutex(q->meConsumers);

	return;
}

BOOL QueueEmpty(LPQUEUE q) {
	return (q->first == q->last);
}

BOOL QueueFull(LPQUEUE q) {
	return ((q->first - q->last) == 1 || (q->last == q->size - 1 && q->first == 0));
}

VOID QueueRemove(LPQUEUE q, LPDWORD n) {

	// Copy to n what's in the 'first' position of the array
	*n = *(q->arr + q->first);

	// Increase first
	q->first = (q->first + 1) % q->size;

	return;
}

VOID QueueInsert(LPQUEUE q, DWORD n) {

	// Copy n to the last position of the array
	*(q->arr + q->last) = n;

	// Increase last
	q->last = (q->last + 1) % q->size;
	
	return;
}