#ifndef ARCHSTATUS_H
#define ARCHSTATUS_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "results.h"
#include "constants.h"

#define PROGRAM_NAME "archstatus"

typedef struct output_config_t {
    bool do_wiki;
    bool do_aur;
    bool do_site;
    bool do_forum;
    bool do_last_events;
} output_config_t;

typedef struct fetch_data_t {
    char *memory;
    size_t size;
} fetch_data_t;

output_config_t *init_output_config();

fetch_data_t *fetch_url(char *url);

latest_events_result_t *parse_events_data(fetch_data_t *data);

monitor_list_result_t *parse_monitor_list_data(fetch_data_t *data);

void print_monitors_title();

void print_monitor_data(monitor_t *monitor);

char* format_ratio(ratio_t *ratio);

char* ratio_to_colored_space(ratio_t *ratio);

char* format_monitor_status(char *status);

void print_events(latest_events_result_t *result);

char* format_event(event_t *event);

void print_arch_logo(void);
#endif // ARCHSTATUS_H
