#ifndef ARCHSTATUS_H
#define ARCHSTATUS_H

#include <stdbool.h>
#include "results.h"

#define PROGRAM_NAME "archstatus"
#define DEFAULT_DAILY_RATIO_AMOUNT 30
#define DEFAULT_OS_LOGO "archlinux"

typedef struct output_config_t {
    bool do_wiki;
    bool do_aur;
    bool do_site;
    bool do_forum;
    bool do_last_events;
    int daily_ratio_amount;
    char *os;
} output_config_t;

typedef struct fetch_data_t {
    char *memory;
    size_t size;
} fetch_data_t;

output_config_t *init_output_config();

fetch_data_t *fetch_url(char *url);

latest_events_result_t *parse_events_data(fetch_data_t *data);

monitor_list_result_t *parse_monitor_list_data(fetch_data_t *data);

#endif // ARCHSTATUS_H
