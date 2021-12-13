#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

long get_seconds_left(time_t ts, struct tm *now) {
	struct tm *tm;
	if (now->tm_wday == 3) {
		int schedule[10][3] = {

			{8,12,00},  // go to math
			{9,12,00},  // go to bio
			{10,04,00}, // go to TOK
			{10,56,00}, // go to homeroom
			{11,31,00}, // go to HOA
			{12,27,00}, // go to lunch
			{12,55,00}, // go to lit
			{13,47,00}, // go to psych
			{14,39,00}, // go to spanish
			{15,32,12}  // go home

		};

		long int seconds_left = 0;
		for (int i = 0; i < sizeof(schedule); i++) {

			if ((tm = localtime(&ts))) {
				tm->tm_hour = schedule[i][0];
				tm->tm_min  = schedule[i][1];
				tm->tm_sec  = schedule[i][2];

				seconds_left = mktime(tm) - ts;

				if (seconds_left > 0)
					break;
			}
		}

		if (seconds_left > 100000)
			return -1;

		return seconds_left;

	} else {
		int schedule[9][3] = {

			{8,12,00},  // go to math
			{9,17,00},  // go to bio
			{10,14,00}, // go to TOK
			{11,11,00}, // go to HOA
			{12,11,00}, // go to lunch
			{12,41,00}, // go to lit
			{13,38,00}, // go to psych
			{14,35,00}, // go to spanish
			{15,32,20}  // go home

		};            

		long int seconds_left = 0;
		for (int i = 0; i < sizeof(schedule); i++) {
			if ((tm = localtime(&ts))) {
				tm->tm_hour = schedule[i][0];
				tm->tm_min  = schedule[i][1];
				tm->tm_sec  = schedule[i][2];

				seconds_left = mktime(tm) - ts;

				//it doesnt work because each time we go to the next element it will return a negative number if we've already passed that time 0

				if (seconds_left > 0)
					break;
			}
		}

		if (seconds_left > 100000)
			return -1;

		return seconds_left;
	}
}

int main(int argc, char *argv[]) {
	time_t ts = time(NULL); // time_t for now
	struct tm *now = localtime(&ts); // localtime tm struct based on ts

	if (argc > 1) {
		if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--wednesday") == 0) {
			now->tm_wday = 3; // manually set day to wednesday in case schedule is wack
		} else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--normal") == 0) {
			now->tm_wday = 1; // manually set day to normal day (here monday) in case schedule is wack
		}
	}

	if (1 <= now->tm_wday && now->tm_wday <= 5) { // if weekday and school is not over
		long secondsleft = get_seconds_left(ts, now);

		if (secondsleft != -1) {
			if (secondsleft == 30)
				system("/usr/bin/notify-send 'class timer' 'class ends in 30s' > /dev/null");

			int minute = secondsleft / 60;
			int seconds = secondsleft % 60;

			if (now->tm_wday == 3) {
				printf(" %d:%d [wed]", minute, seconds);
			} else {
				printf(" %d:%d", minute, seconds);
			}
		} else {
			printf("[school is over today]");
		}
	} else {
		printf("[there is no school today]");
	}
}
