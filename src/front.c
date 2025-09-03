#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "front.h"
#include "logo.h"

void print_os_logo(char *os_release, char *status) {
	char *logo = get_logo_for(os_release);

	if(strcmp(status, "") == 0) {
		printf(logo);
	} else if(strcmp(status, "All Clear") == 0) {
		char *str;
		asprintf(&str, "%s%s%s All systems %sOperational%s", COLOR_GREEN_FOREGROUND, BIG_BLACK_CIRCLE, ANSI_COLOR_RESET, COLOR_GREEN_FOREGROUND, ANSI_COLOR_RESET);
		printf(logo, str);
	} else {
		char *str;
		asprintf(&str, "%s%s%s%d monitor/s %sDown%s", COLOR_RED_FOREGROUND, BIG_BLACK_CIRCLE, ANSI_COLOR_RESET, status[0], COLOR_RED_FOREGROUND, ANSI_COLOR_RESET);
		printf(logo, str); 
	}
	printf("\n\n");
}

void print_monitors_title(char *from_date, char *to_date) {
	printf("%sMonitors (default)%s    %sDate range: [%s to %s]%s\n\n", ANSI_BOLD, ANSI_COLOR_RESET, COLOR_GREY_FOREGROUND, from_date, to_date, ANSI_COLOR_RESET);
}

void print_monitor_data(monitor_t *monitor, int n_ratios) {
		printf("%s -> | %s\t", monitor->name, format_ratio(&(monitor->quarter_ratio)));
		for(int i = 0; i < n_ratios-8; i++)
			printf(" ");
		printf("%s\n", format_monitor_status(monitor->status));

		for(int i = n_ratios - 1; i >= 0; i--) {
			ratio_t daily_ratio = monitor->daily_ratios[i];
			printf("%s ", ratio_to_colored_space(&daily_ratio));
		}

		printf("\n\n");
}

char *format_ratio(ratio_t *ratio) {
	char buf[128];
	char *color; 
	float value = ratio->ratio;
	if(value > BAR_RATIO_GREEN_THRESHOLD) {
		color = COLOR_GREEN_FOREGROUND;
	} else if (value > BAR_RATIO_ORANGE_THRESHOLD) {
		color = COLOR_ORANGE_FOREGROUND;
	} else {
		color = COLOR_RED_FOREGROUND;
	}
	sprintf(buf, "%s%.3f%%%s", color, value, ANSI_COLOR_RESET);
	return strdup(buf);
}

char *ratio_to_colored_space(ratio_t *ratio) {
	char buf[128];
	float value = ratio->ratio;
	char *color;
	if(value > BAR_RATIO_GREEN_THRESHOLD) {
		color = COLOR_GREEN_BACKGROUND;
	} else if (value > BAR_RATIO_ORANGE_THRESHOLD) {
		color = COLOR_ORANGE_BACKGROUND;
	} else {
		color = COLOR_RED_BACKGROUND;
	}
	sprintf(buf, "%s %s", color, ANSI_COLOR_RESET);
	return strdup(buf);
}

char *format_monitor_status(char *status) {
	char buf[128];
	if(strcmp(status, STATUS_OPERATIONAL_CODE) == 0)
		 sprintf(buf, "%s%s Operational%s", COLOR_GREEN_FOREGROUND, BIG_BLACK_CIRCLE, ANSI_COLOR_RESET);
	else if(strcmp(status, STATUS_DOWN_CODE) == 0)
		 sprintf(buf, "%s%s Down%s", COLOR_RED_FOREGROUND, BIG_BLACK_CIRCLE, ANSI_COLOR_RESET);
	else 
		 sprintf(buf, "%s%s Unknown%s", COLOR_GREY_FOREGROUND, BIG_BLACK_CIRCLE, ANSI_COLOR_RESET);
	return strdup(buf);
}

void print_events(latest_events_result_t *result) {
	printf("\n%sStatus updates%s %sLast 30 days%s\n\n", ANSI_BOLD, ANSI_COLOR_RESET, COLOR_GREY_FOREGROUND, ANSI_COLOR_RESET);
	for(int i = 0; i < result->count; i++) {
		printf("%s\n", format_event(&((result->events)[i])));
	}
}

char* format_event(event_t *event) {
	char buf[2048] = "";
	sprintf(buf, "%s[%s]%s\n", COLOR_GREY_FOREGROUND, event->date, ANSI_COLOR_RESET);
	sprintf(buf + strlen(buf), "%s%s%s\n\n", ANSI_BOLD, event->title, ANSI_COLOR_RESET);
	sprintf(buf + strlen(buf), "%s\n", event->content);
	sprintf(buf + strlen(buf), "%sUpdated on %s%s\n", COLOR_GREY_FOREGROUND, event->time_gmt, ANSI_COLOR_RESET);
	return strdup(buf);
}
