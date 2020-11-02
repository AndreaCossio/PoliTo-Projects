#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <time.h>
#include "myDateAndTime.h"

DWORD compareDate(MYDATE d1, MYDATE d2) {
	if (d1.year > d2.year) return 1;
	else if (d1.year < d2.year) return -1;
	else if (d1.month > d2.month) return 1;
	else if (d1.month < d2.month) return -1;
	else if (d1.day > d2.day) return 1;
	else if (d1.day < d2.day) return -1;
	else return 0;
}

DWORD compareTime(MYTIME t1, MYTIME t2) {
	if (t1.hour > t2.hour) return 1;
	else if (t1.hour < t2.hour) return -1;
	else if (t1.min > t2.min) return 1;
	else if (t1.min < t2.min) return -1;
	else if (t1.sec > t2.sec) return 1;
	else return 0;
}

BOOL compareDateTime(MYDATETIME dt1, MYDATETIME dt2) {
	INT d = compareDate(dt1.date, dt2.date);
	INT t = compareTime(dt1.time, dt2.time);
	if (d > 0) return TRUE;
	else if (d < 0) return FALSE;
	else if (t >= 0) return TRUE;
	else return FALSE;
}

MYDATE getRandomDate(DWORD minYear, DWORD maxYear) {
	MYDATE d;
	d.year = rand() % (maxYear - minYear) + minYear;
	d.month = rand() % 12;
	d.day = rand() % 31;
	return d;
}

MYTIME getRandomTime() {
	MYTIME t;
	t.hour = rand() % 24;
	t.min = rand() % 60;
	t.sec = rand() % 60;
	return t;
}

MYDATETIME getRandomDateTime(DWORD minYear, DWORD maxYear) {
	MYDATETIME dt;
	dt.date = getRandomDate(minYear, maxYear);
	dt.time = getRandomTime();
	return dt;
}

DWORD getTimeSeconds(MYTIME t) {
	return t.hour * 3600 + t.min * 60 + t.sec;
}