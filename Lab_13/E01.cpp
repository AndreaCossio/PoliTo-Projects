#define _CRT_SECURE_NO_WARNINGS

#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

// Notes
// Direction:	0 = clockwise
//				1 = anticlockwise

// Typedefs
typedef struct station STATION, * LPSTATION;
typedef struct train TRAIN, * LPTRAIN;
typedef struct common COMMON, * LPCOMMON;
typedef struct tsa THREAD_STATION_ARG, * LPTHREAD_STATION_ARG;
typedef struct tta THREAD_TRAIN_ARG, * LPTHREAD_TRAIN_ARG;
typedef struct train_fifo TRAIN_FIFO, * LPTRAIN_FIFO;

// Structs
struct station {
	DWORD stationId, historyIndex[2], historyCount[2], commuters[2], trains[2], history[2][3];
	HANDLE varMutex[2], trainMutex[2];
	LPTRAIN_FIFO trainQueue[2], last[2];
};

struct train {
	DWORD trainId, capacity, direction, line, currentStation;
};

struct common {
	DWORD times[3], trainId;
	STATION stations[3];
	BOOL exit;
};

struct tsa {
	DWORD stationId;
	LPCOMMON common;
};

struct tta {
	TRAIN train;
	LPCOMMON common;
	BOOL exit;
};

struct train_fifo {
	HANDLE tTrain;
	THREAD_TRAIN_ARG tTrainArg;
	LPTRAIN_FIFO next;
};

static DWORD WINAPI commuters(LPVOID arg);
static DWORD WINAPI station(LPVOID arg);
static DWORD WINAPI train(LPVOID arg);
VOID addTrain(LPSTATION station, LPCOMMON common, DWORD direction);
VOID removeTrain(LPSTATION station, DWORD direction);
DWORD findNextStation(TRAIN train);
VOID ReportInfo(LPCTSTR str, ...);
VOID ReportError(LPCTSTR str);

INT _tmain(INT argc, LPTSTR argv[]) {

	DWORD i, j;
	HANDLE tCommuter, tStation[3];
	COMMON common;
	THREAD_STATION_ARG tStationArg[3];

	// Check arguments
	if (argc < 4) {
		_ftprintf(stderr, _T("Usage: %s Time1 Time2 Time3.\n"), argv[0]);
		return 1;
	}

	// Init common variables
	for (i = 0; i < 3; i++) {

		// Times
		common.times[i] = _tstoi(argv[i + 1]);

		// Stations
		common.stations[i].stationId = i;
		for (j = 0; j < 2; j++) {
			common.stations[i].historyIndex[j] = 0;
			common.stations[i].historyCount[j] = 0;
			common.stations[i].commuters[j] = 0;
			common.stations[i].trains[j] = 0;
			common.stations[i].history[j][0] = 0;
			common.stations[i].history[j][1] = 0;
			common.stations[i].history[j][2] = 0;
			if ((common.stations[i].varMutex[j] = CreateSemaphore(NULL, 1, 1, NULL)) == NULL ||
				(common.stations[i].trainMutex[j] = CreateSemaphore(NULL, 1, 1, NULL)) == NULL) {
				ReportError(_T("CreateSemaphore"));
				return 2;
			}
			common.stations[i].last[j] = common.stations[i].trainQueue[j] = (LPTRAIN_FIFO)malloc(sizeof(TRAIN_FIFO));
		}

		common.trainId = 0;
		common.exit = FALSE;
	}

	// Message
	ReportInfo(_T("MAIN\t\tSTATION\t\tTRAIN\t\tCOMMUTERS\tON BOARD\tIN\t\tOUT\n"));

	// Create Commuter thread
	if ((tCommuter = CreateThread(NULL, 0, commuters, &common, 0, NULL)) == NULL) {
		ReportError(_T("CreateThread"));
		return 3;
	}

	// Create Station threads
	for (i = 0; i < 3; i++) {
		tStationArg[i].stationId = i;
		tStationArg[i].common = &common;
		if ((tStation[i] = CreateThread(NULL, 0, station, &(tStationArg[i]), 0, NULL)) == NULL) {
			ReportError(_T("CreateThread"));
			ExitProcess(4);
		}
	}

	// Exit condition after 10 time1 cycles
	// TODO exit condition
	/*Sleep(common.times[0] * 1000 * 10);
	common.exit = TRUE;*/

	// Wait threads
	WaitForSingleObject(tCommuter, INFINITE);
	CloseHandle(tCommuter);
	WaitForMultipleObjects(3, tStation, TRUE, INFINITE);
	for (i = 0; i < 3; i++) {
		CloseHandle(tStation[i]);
	}

	// Close sempahores
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 2; j++) {
			CloseHandle(common.stations[i].varMutex[j]);
			CloseHandle(common.stations[i].trainMutex[j]);
			free(common.stations[i].trainQueue[j]);
		}
	}

	return 0;
}

static DWORD WINAPI commuters(LPVOID arg) {
	
	DWORD i, j, commutersIn;
	LPCOMMON common = (LPCOMMON)arg;

	srand(GetCurrentThreadId() * time(NULL));

	while (!common->exit) {
		// Generate commuters for all of the three stations
		for (i = 0; i < 3; i++) {
			// For all the directions
			for (j = 0; j < 2; j++) {
				// Random number of commuters enter the station
				commutersIn = rand() % 25;//101;

				// Update commuters in the station
				WaitForSingleObject(common->stations[i].varMutex[j], INFINITE);
				common->stations[i].commuters[j] += commutersIn;

				// Message
				ReportInfo(_T("COMMUTER\tS%d%c\t\t\t\t%d\n"), common->stations[i].stationId + 1, (j == 0) ? _T('c') : _T('a'), common->stations[i].commuters[j]);
				ReleaseSemaphore(common->stations[i].varMutex[j], 1, NULL);
			}
		}

		// Sleep Time1
		Sleep(common->times[0] * 1000);
	};

	ExitThread(0);
}

static DWORD WINAPI station(LPVOID arg) {

	DWORD i, stationId;
	LPTHREAD_STATION_ARG param = (LPTHREAD_STATION_ARG)arg;
	stationId = param->stationId;
	LPCOMMON common = param->common;
	LPSTATION station = &(common->stations[stationId]);

	srand(GetCurrentThreadId() * time(NULL));

	// Little delay with respect to the commuter thread
	Sleep(common->times[0] * 100);

	while (!common->exit) {
		// Update history of both directions
		for (i = 0; i < 2; i++) {
			// Wait to access var
			WaitForSingleObject(station->varMutex[i], INFINITE);

			// Update the history with the current number of commuters that are waiting
			station->history[i][station->historyIndex[i]] = station->commuters[i];
			station->historyIndex[i] = (station->historyIndex[i] + 1) % 3;
			station->historyCount[i]++;

			// If it's 3 times that there are more than 75 commuters waiting, add train
			if (station->historyCount[i] >= 3 && station->history[i][0] > 75 && station->history[i][1] > 75 && station->history[i][2] > 75) {
				addTrain(station, common, i);
				// Release var
				ReleaseSemaphore(station->varMutex[i], 1, NULL);
			}
			// Else, if it's 3 times that there are less than 30 commuters waiting, remove train
			else if (station->historyCount[i] >= 3 && station->trains[i] > 0 && station->history[i][0] < 30 && station->history[i][1] < 30 && station->history[i][2] < 30) {
				removeTrain(station, i);
			}
			else {
				// Release var
				ReleaseSemaphore(station->varMutex[i], 1, NULL);
			}
		}

		// Sleep Time1
		Sleep(common->times[0] * 1000);
	}

	ExitThread(0);
}

static DWORD WINAPI train(LPVOID arg) {

	DWORD commutersOut, commutersIn;
	LPTHREAD_TRAIN_ARG param = (LPTHREAD_TRAIN_ARG)arg;
	LPSTATION station;

	srand(GetCurrentThreadId() * time(NULL));

	while (!param->exit) {
		// Go to the next station (sleep Time2 before getting to the next station)
		Sleep(param->common->times[1] * 1000);

		param->train.currentStation = findNextStation(param->train);

		// Enter station
		station = &(param->common->stations[param->train.currentStation]);
		WaitForSingleObject(station->trainMutex[param->train.direction], INFINITE);

		// Stay in the station (sleep Time3)
		Sleep(param->common->times[2] * 1000);

		// Passengers out (also out of the station)
		commutersOut = rand() % (100 - param->train.capacity + 1);
		param->train.capacity += commutersOut;

		// Passengers in (from the station)
		if (param->train.capacity < station->commuters[param->train.direction]) {
			commutersIn = rand() % (param->train.capacity + 1);
		}
		else {
			commutersIn = rand() % (station->commuters[param->train.direction] + 1);
		}
		param->train.capacity -= commutersIn;

		// Update commuters in the station
		WaitForSingleObject(station->varMutex[param->train.direction], INFINITE);
		if (param->exit) {
			ReleaseSemaphore(station->varMutex[param->train.direction], 1, NULL);
			ReleaseSemaphore(station->trainMutex[param->train.direction], 1, NULL);
			break;
		}
		station->commuters[param->train.direction] -= commutersIn;

		// Message
		ReportInfo(_T("TRAIN\t\tS%d%c\t\tT%d\t\t%d\t\t%d\t\t%d\t\t%d\n"), station->stationId + 1, (param->train.direction == 0) ? _T('c') : _T('a'), param->train.trainId + 1, station->commuters[param->train.direction], 100 - param->train.capacity, commutersIn, commutersOut);
		ReleaseSemaphore(station->varMutex[param->train.direction], 1, NULL);

		// Leave station
		ReleaseSemaphore(station->trainMutex[param->train.direction], 1, NULL);
	}

	ExitThread(0);
}

VOID addTrain(LPSTATION station, LPCOMMON common, DWORD direction) {
	
	DWORD commutersIn;
	LPTHREAD_TRAIN_ARG tArg = &(station->last[direction]->tTrainArg);

	// Number of commuters that enter the train initially
	if (station->commuters[direction] <= 100) {
		commutersIn = rand() % (station->commuters[direction] + 1);
	}
	else {
		commutersIn = rand() % 101;
	}

	// Create train
	tArg->train.trainId = common->trainId++;
	tArg->train.capacity = 100 - commutersIn;
	tArg->train.direction = direction;
	tArg->train.line = (station->stationId == 0) ? 0 : ((station->stationId == 2) ? 1 : rand() % 2);
	tArg->train.currentStation = station->stationId;
	tArg->common = common;
	tArg->exit = FALSE;

	// Update station vars
	station->commuters[direction] -= commutersIn;
	station->historyCount[direction] = 0;
	station->trains[direction]++;

	// Create and add the train
	ReportInfo(_T("STATION\t\tS%d%c\t\tCREATE: T%d\t%d\t\t%d\n"), station->stationId + 1, (direction == 0) ? _T('c') : _T('a'), tArg->train.trainId + 1, station->commuters[direction], commutersIn);
	station->last[direction]->tTrain = CreateThread(NULL, 0, train, tArg, 0, NULL);
	station->last[direction] = station->last[direction]->next = (LPTRAIN_FIFO)malloc(sizeof(TRAIN_FIFO));

	return;
}

VOID removeTrain(LPSTATION station, DWORD direction) {

	LPTRAIN_FIFO tmp;

	// Update station vars
	station->historyCount[direction] = 0;
	station->trains[direction]--;

	// Wait and remove the train
	tmp = station->trainQueue[direction];
	station->trainQueue[direction] = station->trainQueue[direction]->next;
	tmp->tTrainArg.exit = TRUE;

	ReleaseSemaphore(station->varMutex[direction], 1, NULL);
	ReportInfo(_T("STATION\t\tS%d%C\t\tKILL: T%d\n"), station->stationId + 1, (direction == 0) ? _T('c') : _T('a'), tmp->tTrainArg.train.trainId + 1);
	WaitForSingleObject(tmp->tTrain, INFINITE);
	CloseHandle(tmp->tTrain);
	free(tmp);

	return;
}

DWORD findNextStation(TRAIN train) {

	DWORD nextStation = 0;

	switch (train.currentStation) {
	case 0:
		nextStation = 1;
		break;
	case 1:
		nextStation = (train.line == 0) ? 0 : 2;
		break;
	case 2:
		nextStation = 1;
		break;
	default:
		break;
	}

	return nextStation;
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