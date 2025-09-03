#ifndef FRONT_H
#define FRONT_H

#include "results.h"

#define COLOR_GREEN_FOREGROUND "\033[38;2;59;214;113m"
#define COLOR_GREEN_BACKGROUND "\033[48;2;59;214;113m"
#define COLOR_ORANGE_FOREGROUND "\033[38;2;242;144;48m"
#define COLOR_ORANGE_BACKGROUND "\033[48;2;242;144;48m"
#define COLOR_RED_FOREGROUND "\033[38;2;223;72;74m"
#define COLOR_RED_BACKGROUND "\033[48;2;223;72;74m"
#define COLOR_GREY_FOREGROUND "\033[38;2;104;119;144m"
#define COLOR_GREY_BACKGROUND "\033[48;2;104;119;144m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define BIG_BLACK_CIRCLE "\u25cf"

#define BAR_RATIO_GREEN_THRESHOLD 99.0f
#define BAR_RATIO_ORANGE_THRESHOLD 95.0f

#define STATUS_OPERATIONAL_CODE "success"
#define STATUS_DOWN_CODE "danger"

void print_os_logo(char *os_release, char *status);

void print_monitors_title(char *from_date, char *to_date);

void print_monitor_data(monitor_t *monitor, int n_ratios);

char *format_ratio(ratio_t *ratio);

char *ratio_to_colored_space(ratio_t *ratio);

char *format_monitor_status(char *status);

void print_events(latest_events_result_t *result);

char* format_event(event_t *event);

#endif // FRONT_H
