#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

long get_seconds_left(time_t ts, struct tm *now) {
	struct tm *tm;
	if (now->tm_wday == 2 || now->tm_wday == 4) { // tuesday thursday
		int schedule[5][3] = {

			{14,20,00},
			{15,35,00},
			{17,30,00},
			{18,45,00},
			{23,59,59},

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

	} else if (now->tm_wday == 3 || now->tm_wday == 5) { // wed friday
		int schedule[7][3] = {
			{10,20,00},
			{11,10,00},
			{12,40,00},
			{13,30,00},
			{15,00,00},
			{15,50,00},
			{23,59,59},
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
	} else if (now->tm_wday == 1) { // monday
		int schedule[9][3] = {
			 {9,10,00},
			{10,00,00},
			{10,20,00},
			{11,10,00},
			{12,40,00},
			{13,30,00},
			{15,00,00},
			{15,50,00},
			{23,59,59},
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
			printf(" ");
			
			if (secondsleft == 30)
				system("/usr/bin/notify-send 'pack up' '30 seconds until classchange' > /dev/null");
			
			if (secondsleft == 900)
				system("/usr/bin/notify-send 'go now' '15 minutes until classchange' > /dev/null");

			int minute = secondsleft / 60;
			int seconds = secondsleft % 60;
			int hour = minute / 60;
			if (minute > 59) {
				minute = minute-(60*hour);
			}
			printf("%02d:%02d:%02d", hour, minute, seconds);
		}
	}
}
