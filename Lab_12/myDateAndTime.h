#pragma once

#include <Windows.h>

typedef struct {
	DWORD year, month, day;
} MYDATE, * LPMYDATE;

typedef struct {
	DWORD hour, min, sec;
} MYTIME, * LPMYTIME;

typedef struct {
	MYDATE date;
	MYTIME time;
} MYDATETIME, * LPMYDATETIME;

DWORD compareDate(MYDATE d1, MYDATE d2);
DWORD compareTime(MYTIME t1, MYTIME t2);
BOOL compareDateTime(MYDATETIME dt1, MYDATETIME dt2);
MYDATE getRandomDate(DWORD minYear, DWORD maxYear);
MYTIME getRandomTime();
MYDATETIME getRandomDateTime(DWORD minYear, DWORD maxYear);
DWORD getTimeSeconds(MYTIME t);