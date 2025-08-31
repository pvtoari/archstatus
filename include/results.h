#ifndef RESULTS_H
#define RESULTS_H

#define DAYS_AMOUNT 90
#define DAYS_TODAY_ORD 0

#include <stddef.h>

/*
    /api/getEventFeed STUFF
*/

typedef struct event_t {
    char *content;
    char *date;
    char *event_type;
    char *time;
    char *time_gmt;
    char *title;
    long timestamp;
} event_t;

typedef struct latest_events_result_t {
    size_t count;
    event_t *events;
} latest_events_result_t;

/*
    /api/getMonitorList STUFF
*/

typedef struct ratio_t {
    char *label;
    float ratio;
} ratio_t;

typedef struct monitor_t {
    long id;
    char *name;
    char *status;
    ratio_t monthly_ratio;
    ratio_t quarter_ratio;
    ratio_t daily_ratios[DAYS_AMOUNT];
} monitor_t;

typedef struct statistics_t {
    char *count_result;
    size_t count_down;
    size_t count_paused;
    size_t count_up;
} statistics_t;

typedef struct monitor_list_result_t {
    char *days[DAYS_AMOUNT];
    size_t total_monitors;
    monitor_t *monitors;
    statistics_t statistics;
} monitor_list_result_t;

#endif // RESULTS_H
