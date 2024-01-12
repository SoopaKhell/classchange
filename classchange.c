#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define MAX_CLASSES 8
#define MAX_DAY_NAME 4

typedef struct {
    int hour;
    int minute;
    int second;
    char class_name[50];
} Class;

typedef struct {
    char day_name[MAX_DAY_NAME];
    Class classes[MAX_CLASSES];
    int num_classes;
} DaySchedule;

DaySchedule schedule[5];

void load_schedule(const char* filename) {
    char path[PATH_MAX];
    if (filename[0] == '~') {
        snprintf(path, PATH_MAX, "%s%s", getenv("HOME"), filename+1);
    } else {
        strncpy(path, filename, PATH_MAX);
    }

    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open schedule file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int dayIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        DaySchedule day_schedule;
        char* token = strtok(line, ":");
        strncpy(day_schedule.day_name, token, MAX_DAY_NAME);
        day_schedule.num_classes = 0;

        token = strtok(NULL, ",");
        while (token) {
            sscanf(token, "%d:%d:%d-%s",
                   &day_schedule.classes[day_schedule.num_classes].hour,
                   &day_schedule.classes[day_schedule.num_classes].minute,
                   &day_schedule.classes[day_schedule.num_classes].second,
                   day_schedule.classes[day_schedule.num_classes].class_name);

            day_schedule.num_classes++;
            token = strtok(NULL, ",");
        }

        memcpy(&schedule[dayIndex++], &day_schedule, sizeof(DaySchedule));
    }

    fclose(file);
}

long get_seconds_left(DaySchedule day_schedule, time_t ts, int *class_index, int is_next_day) {
    struct tm *tm;
    long int seconds_left = LONG_MAX;
    int found = 0;

    for (int i = 0; i < day_schedule.num_classes; i++) {
        if ((tm = localtime(&ts))) {
            tm->tm_hour = day_schedule.classes[i].hour;
            tm->tm_min = day_schedule.classes[i].minute;
            tm->tm_sec = day_schedule.classes[i].second;

            time_t class_time = mktime(tm);

            if (class_time - ts > 0 && class_time - ts < seconds_left) {
                seconds_left = class_time - ts;
                *class_index = i;
                found = 1;
            }
        }
    }

    if (is_next_day && !found) {
        *class_index = 0;
        if ((tm = localtime(&ts))) {
            tm->tm_hour = day_schedule.classes[0].hour;
            tm->tm_min = day_schedule.classes[0].minute;
            tm->tm_sec = day_schedule.classes[0].second;
            tm->tm_mday += 1;

            seconds_left = mktime(tm) - ts;
        }
    }

    return seconds_left;
}

void send_notification(const char *title, const char *message) {
    char command[256];
    snprintf(command, sizeof(command), "/usr/bin/notify-send '%s' '%s' > /dev/null", title, message);
    system(command);
}

int main() {
    load_schedule("~/.config/classchange/schedule.cfg");
    time_t ts = time(NULL);
    struct tm *now = localtime(&ts);

    long seconds_left = -1;
    int class_index = -1;
    int is_next_day = 0;

    int last_class_index = schedule[now->tm_wday - 1].num_classes - 1;
    struct tm last_class_tm = *now;
    last_class_tm.tm_hour = schedule[now->tm_wday - 1].classes[last_class_index].hour;
    last_class_tm.tm_min = schedule[now->tm_wday - 1].classes[last_class_index].minute;
    last_class_tm.tm_sec = schedule[now->tm_wday - 1].classes[last_class_index].second;
    time_t last_class_time = mktime(&last_class_tm);

    if (difftime(ts, last_class_time) > 0) {
        is_next_day = 1;
        now->tm_wday = (now->tm_wday % 5) + 1;
    }

    seconds_left = get_seconds_left(schedule[now->tm_wday - 1], ts, &class_index, is_next_day);

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
        printf("%s (%02d:%02d:%02d)\n", schedule[now->tm_wday - 1].classes[class_index].class_name, hour, minute, seconds);
    }

    return 0;
}
