#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int mon_schedule[6][3] = {
    {10, 20, 00},
    {11, 10, 00},
    {12, 40, 00},
    {13, 30, 00},
    {15, 00, 00},
    {15, 50, 00},
};

char *mon_classes[] = {"MATH", "MATH", "POLS", "POLS", "LING", "LING"};

int tue_thu_schedule[8][3] = {
    {9, 35, 00},
    {10, 50, 00},
    {11, 10, 00},
    {12, 25, 00},
    {14, 20, 00},
    {15, 10, 00},
    {15, 55, 00},
    {17, 10, 00},
};

char *tue_thu_classes[] = {"MATH", "MATH", "CSCI", "CSCI", "POLS", "POLS", "CSCI", "CSCI"};

int wed_fri_schedule[2][3] = {
    {15, 00, 00},
    {15, 50, 00},
};

char *wed_fri_classes[] = {"LING", "LING"};

long get_seconds_left(int schedule[][3], int schedule_length, time_t ts, int *class_index) {
    struct tm *tm;
    long int seconds_left = 0;
    for (int i = 0; i < schedule_length; i++) {
        if ((tm = localtime(&ts))) {
            tm->tm_hour = schedule[i][0];
            tm->tm_min = schedule[i][1];
            tm->tm_sec = schedule[i][2];

            seconds_left = mktime(tm) - ts;

            if (seconds_left > 0) {
                *class_index = i;
                break;
            }
        }
    }

    if (seconds_left > 100000)
        return -1;

    return seconds_left;
}

void send_notification(const char *title, const char *message) {
    char command[256];
    snprintf(command, sizeof(command), "/usr/bin/notify-send '%s' '%s' > /dev/null", title, message);
    system(command);
}

int main() {
    time_t ts = time(NULL);
    struct tm *now = localtime(&ts);

    long seconds_left = -1;
    int class_index = -1;
    char **classes = NULL;

    switch (now->tm_wday) {
        case 1: // Monday
            seconds_left = get_seconds_left(mon_schedule, 6, ts, &class_index);
            classes = mon_classes;
            break;
        case 2: // Tuesday
        case 4: // Thursday
            seconds_left = get_seconds_left(tue_thu_schedule, 8, ts, &class_index);
            classes = tue_thu_classes;
            break;
        case 3: // Wednesday
        case 5: // Friday
            seconds_left = get_seconds_left(wed_fri_schedule, 2, ts, &class_index);
            classes = wed_fri_classes;
            break;
    }

    if (seconds_left != -1 && class_index != -1) {
        if (seconds_left == 30)
            send_notification("pack up", "30 seconds until class change");

        if (seconds_left == 900)
            send_notification("go now", "15 minutes until class change");

        int minute = seconds_left / 60;
        int seconds = seconds_left % 60;
        int hour = minute / 60;
        if (minute > 59) {
            minute = minute - (60 * hour);
        }
        printf("%s (%02d:%02d:%02d)\n", classes[class_index], hour, minute, seconds);
    }

    return 0;
}
