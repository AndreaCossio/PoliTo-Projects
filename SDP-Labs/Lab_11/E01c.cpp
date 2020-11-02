#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>

#define MAXLEN 30+1

typedef struct {
	DWORD id, accountNumber;
	TCHAR surname[MAXLEN], name[MAXLEN];
	INT balance;
} ACCOUNT, * LPACCOUNT;

typedef struct {
	DWORD id, accountNumber;
	TCHAR surname[MAXLEN], name[MAXLEN];
	INT operationImport;
} OPERATION, * LPOPERATION;

typedef struct {
	DWORD id;
	LPTSTR operationFile;
	LPHANDLE accountFile, mutex;
} TARG, * LPTARG;

static DWORD WINAPI threadOperation(LPVOID arg);
VOID printAccount(LPACCOUNT acc);
VOID printOperation(LPOPERATION op);
VOID ReportInfo(LPCTSTR str, ...);
VOID ReportError(LPCTSTR str);

INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD i, nThreads, read;
	HANDLE accountFile, mutex;
	LPHANDLE threadHandles;
	LPTARG threadArguments;
	ACCOUNT account;
	LARGE_INTEGER filePos;

	// Check args
	if (argc < 3) {
		_ftprintf(stderr, _T("Usage: %s ACCOUNTfile OPERATIONfile [OPERATIONfile1 [OPERATIONfile2 ... [OPERATIONfileN]]]"), argv[0]);
		return 1;
	}
	nThreads = argc - 2;

	// Open account file
	if ((accountFile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		return 2;
	}

	// Create mutex
	if ((mutex = CreateMutex(NULL, FALSE, NULL)) == NULL) {
		ReportError(_T("CreateMutex"));
		return 3;
	}

	// Show balance before operations
	ReportInfo(_T("Accounts file status before operations:\n"));
	while (ReadFile(accountFile, &account, sizeof(ACCOUNT), &read, NULL) && read == sizeof(ACCOUNT)) {
		printAccount(&account);
	}

	// Allocate space for threads' arguments
	if ((threadArguments = (LPTARG)malloc(nThreads * sizeof(TARG))) == NULL ||
		(threadHandles = (LPHANDLE)malloc(nThreads * sizeof(HANDLE))) == NULL) {
		ReportError(_T("malloc"));
		return 4;
	}

	// Generate threads
	for (i = 0; i < nThreads; i++) {
		threadArguments[i].operationFile = argv[i + 2];
		threadArguments[i].accountFile = &accountFile;
		threadArguments[i].mutex = &mutex;
		if ((threadHandles[i] = (HANDLE)CreateThread(NULL, 0, threadOperation, &(threadArguments[i]), 0, &(threadArguments[i].id))) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(5);
		}
	}

	// Wait threads
	WaitForMultipleObjects(nThreads, threadHandles, TRUE, INFINITE);

	// Display account file
	ReportInfo(_T("Accounts file status after operations:\n"));
	filePos.QuadPart = 0;
	SetFilePointerEx(accountFile, filePos, NULL, FILE_BEGIN);
	while (ReadFile(accountFile, &account, sizeof(ACCOUNT), &read, NULL) && read == sizeof(ACCOUNT)) {
		printAccount(&account);
	}

	// Release resources
	for (i = 0; i < nThreads; i++) {
		CloseHandle(threadHandles[i]);
	}
	CloseHandle(accountFile);
	CloseHandle(mutex);
	free(threadArguments);
	free(threadHandles);

	return 0;
}

static DWORD WINAPI threadOperation(LPVOID arg) {

	DWORD readOp, readAcc, written;
	HANDLE operationFile;
	LPTARG param = (LPTARG)arg;
	OPERATION operation;
	ACCOUNT account;
	LARGE_INTEGER filePos;
	OVERLAPPED ov = { 0, 0, 0, 0, NULL };

#ifdef MY_DEBUG
	ReportInfo(_T("Filename: %s\n"), param->operationFile);
#endif

	// Open operation file
	if ((operationFile = CreateFile(param->operationFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE) {
		ReportError(_T("CreateFile"));
		ExitThread(1);
	}

	// Scan operations
	while (ReadFile(operationFile, &operation, sizeof(OPERATION), &readOp, NULL) && readOp == sizeof(OPERATION)) {

#ifdef MY_DEBUG
		ReportInfo(_T("Operation: "));
		printOperation(&operation);
#endif

		// Prepare size and offset to lock
		filePos.QuadPart = (LONGLONG)(operation.id - 1) * sizeof(OPERATION);
		ov.Offset = filePos.LowPart;
		ov.OffsetHigh = filePos.HighPart;
		ov.hEvent = 0;

#ifdef MY_DEBUG
		ReportInfo(_T("Waiting mutex\n"), operation.id);
#endif

		WaitForSingleObject(*(param->mutex), INFINITE);

#ifdef MY_DEBUG
		ReportInfo(_T("Entered mutex\n"), operation.id);
#endif

		// Read account
		if (!ReadFile(*(param->accountFile), &account, sizeof(ACCOUNT), &readAcc, &ov) || readAcc != sizeof(ACCOUNT)) {
			ReportError(_T("ReadFile"));
			ReleaseMutex(*(param->mutex));
			CloseHandle(operationFile);
			ExitThread(4);
		}

#ifdef MY_DEBUG
		ReportInfo(_T("Account before operation: "));
		printAccount(&account);
#endif

		// Update account
		account.balance += operation.operationImport;

		// Write account
		if (!WriteFile(*(param->accountFile), &account, sizeof(ACCOUNT), &written, &ov) || written != sizeof(ACCOUNT)) {
			ReportError(_T("WriteFile"));
			ReleaseMutex(*(param->mutex));
			CloseHandle(operationFile);
			ExitThread(5);
		}

#ifdef MY_DEBUG
		ReportInfo(_T("Account after operation: "));
		printAccount(&account);
#endif

		ReleaseMutex(*(param->mutex));

#ifdef MY_DEBUG
		ReportInfo(_T("Released mutex\n"), operation.id);
#endif
	}

	// Close file
	CloseHandle(operationFile);

	ExitThread(0);
}

VOID printAccount(LPACCOUNT acc) {
	_tprintf(_T("%d %d %s %s %d\n"), acc->id, acc->accountNumber, acc->surname, acc->name, acc->balance);
}

VOID printOperation(LPOPERATION op) {
	_tprintf(_T("%d %d %s %s %s%d\n"), op->id, op->accountNumber, op->surname, op->name, (op->operationImport >= 0) ? _T("+") : _T(""), op->operationImport);
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