#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdarg.h>

#include "archstatus.h"

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

#define ARCH_LOGO_ASCII_ART "\
\n\033[38;2;23;147;209m\x1b[1m          .\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m         /#\\     \x1b[0m         #    \033[38;2;23;147;209m | *\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m        /###\\    \x1b[0m### #### #### \033[38;2;23;147;209m | | |---. |   | \\ /\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m       /p^###\\   \x1b[0m#   #    #  # \033[38;2;23;147;209m | | |   | |   |  X\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m      /##P^q##\\  \x1b[0m#   #### #  # \033[38;2;23;147;209m | | |   | ^._.| / \\\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m     /##(   )##\\\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m    /###P   q#,^\\\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m   /P^         ^q\\\x1b[0m"

output_config_t *init_output_config() {
    output_config_t *res = malloc(sizeof(output_config_t));
    memset(res, 0, sizeof(output_config_t));

    return res;
}

static size_t write_memory_callback(void *content, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    fetch_data_t *mem = (fetch_data_t *) userp;

    char *ptr = realloc(mem->memory, mem->size + real_size + 1);
    if (!ptr) {
        fprintf(stderr, "failed to realloc (returned NULL)");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), content, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;

    return real_size;
}

fetch_data_t *fetch_url(char *url) {
    CURL *curl;
    CURLcode curl_res;

    fetch_data_t data = {0};
    data.memory = malloc(1);
    data.size = 0;

    fetch_data_t *result = NULL;

    curl = curl_easy_init();
    if (!curl) {
        if (data.memory) free(data.memory);
        return result;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &data);
    curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    curl_res = curl_easy_perform(curl);
    if (curl_res == CURLE_OK) {
        result = malloc(sizeof(fetch_data_t));
        if (result) {
            result->memory = data.memory;
            result->size = data.size;
        } else free(data.memory);
    } else {
        fprintf(stderr, "failed to perform curl: %s\n", curl_easy_strerror(curl_res));
        free(data.memory);
    }

    curl_easy_cleanup(curl);
    return result;
}

static void free_latest_events_result(latest_events_result_t *res) {
    if (!res) return;

    if (res->events) {
        for (size_t i = 0; i < res->count; i++) {
            free(res->events[i].content);
            free(res->events[i].date);
            free(res->events[i].event_type);
            free(res->events[i].time);
            free(res->events[i].time_gmt);
            free(res->events[i].title);
        }
        free(res->events);
    }

    free(res);
}

static void free_monitor_list_result(monitor_list_result_t *res) {
    if (!res) return;

    for (size_t i = 0; i < DAYS_AMOUNT; i++) {
        free(res->days[i]);
    }

    if (res->monitors) {
        for (size_t i = 0; i < res->total_monitors; i++) {
            free(res->monitors[i].name);
            free(res->monitors[i].status);
            free(res->monitors[i].monthly_ratio.label);
            free(res->monitors[i].quarter_ratio.label);
            for (size_t j = 0; j < DAYS_AMOUNT; j++) {
                free(res->monitors[i].daily_ratios[j].label);
            }
        }

        free(res->monitors);
    }

    free(res->statistics.count_result);

    free(res);
}

latest_events_result_t *parse_events_data(fetch_data_t *data) {
    latest_events_result_t *result = malloc(sizeof(latest_events_result_t));
    if (!result) return NULL;

    result->events = NULL;

    cJSON *json_root = cJSON_ParseWithLength(data->memory, data->size);
    if (!json_root) goto end;

    cJSON *json_meta = cJSON_GetObjectItemCaseSensitive(json_root, "meta");
    if (!json_meta) goto end;

    cJSON *json_count = cJSON_GetObjectItemCaseSensitive(json_meta, "count");
    if (!json_meta || !cJSON_IsNumber(json_count)) goto end;
    result->count = json_count->valueint;

    cJSON *json_results = cJSON_GetObjectItemCaseSensitive(json_root, "results");
    if (!json_results || !cJSON_IsArray(json_results)) goto end;
     
    result->events = malloc(sizeof(event_t) * result->count);
    if (!result->events) goto end;

    for (size_t i = 0; i < result->count; i++) {
        result->events[i].content = NULL;
        result->events[i].date = NULL;
        result->events[i].event_type = NULL;
        result->events[i].time = NULL;
        result->events[i].time_gmt = NULL;
        result->events[i].title = NULL;
    }

    cJSON *event = NULL;
    size_t i = 0;
    cJSON_ArrayForEach(event, json_results) {
        cJSON *json_content = cJSON_GetObjectItemCaseSensitive(event, "content");
        if (!json_content || !cJSON_IsString(json_content)) goto end;
        result->events[i].content = strdup(json_content->valuestring);

        cJSON *json_date = cJSON_GetObjectItemCaseSensitive(event, "date");
        if (!json_date || !cJSON_IsString(json_date)) goto end;
        result->events[i].date = strdup(json_date->valuestring);

        cJSON *json_event_type = cJSON_GetObjectItemCaseSensitive(event, "eventType");
        if (!json_event_type || !cJSON_IsString(json_event_type)) goto end;
        result->events[i].event_type = strdup(json_event_type->valuestring);

        cJSON *json_time = cJSON_GetObjectItemCaseSensitive(event, "time");
        if (!json_time || !cJSON_IsString(json_time)) goto end;
        result->events[i].time = strdup(json_time->valuestring);

        cJSON *json_time_gmt = cJSON_GetObjectItemCaseSensitive(event, "timeGMT");
        if (!json_time_gmt || !cJSON_IsString(json_time_gmt)) goto end;
        result->events[i].time_gmt = strdup(json_time_gmt->valuestring);

        cJSON *json_title = cJSON_GetObjectItemCaseSensitive(event, "title");
        if (!json_title || !cJSON_IsString(json_title)) goto end;
        result->events[i].title = strdup(json_title->valuestring);

        cJSON *json_timestamp = cJSON_GetObjectItemCaseSensitive(event, "ts");
        if (!json_timestamp || !cJSON_IsNumber(json_timestamp)) goto end;
        result->events[i].timestamp = json_timestamp->valuedouble;

        i++;
    }

    return result;

end:
    if (result) free_latest_events_result(result);
    cJSON_Delete(json_root);
    return NULL;    
}

monitor_list_result_t *parse_monitor_list_data(fetch_data_t *data) {
    monitor_list_result_t *result = malloc(sizeof(monitor_list_result_t));
    if (!result) return NULL;

    result->monitors = NULL;
    for (size_t i = 0; i < DAYS_AMOUNT; i++) result->days[i] = NULL;
    result->statistics.count_result = NULL;


    cJSON *json_root = cJSON_ParseWithLength(data->memory, data->size);
    if (!json_root) goto end;

    cJSON *json_days = cJSON_GetObjectItemCaseSensitive(json_root, "days");
    if (!json_days) goto end;
    cJSON* day = NULL;
    size_t i = 0;
    cJSON_ArrayForEach(day, json_days) {
        if (!day || !cJSON_IsString(day)) goto end;
        result->days[i++] = strdup(day->valuestring);
    }

    cJSON *json_psp = cJSON_GetObjectItemCaseSensitive(json_root, "psp");
    if (!json_psp) goto end;

    cJSON *json_total_monitors = cJSON_GetObjectItemCaseSensitive(json_psp, "totalMonitors");
    if (!json_total_monitors || !cJSON_IsNumber(json_total_monitors)) goto end;
    result->total_monitors = json_total_monitors->valueint;

    result->monitors = malloc(sizeof(monitor_t) * result->total_monitors);
    for (size_t i = 0; i < result->total_monitors; i++) {
        result->monitors[i].name = NULL;
        result->monitors[i].status = NULL;
        result->monitors[i].monthly_ratio.label = NULL;
        result->monitors[i].quarter_ratio.label = NULL;
        for (size_t j = 0; j < DAYS_AMOUNT; j++) {
            result->monitors[i].daily_ratios[j].label = NULL;
        }
    }

    cJSON *json_monitors = cJSON_GetObjectItemCaseSensitive(json_psp, "monitors");
    if (!json_monitors || !cJSON_IsArray(json_monitors)) goto end;
    cJSON *monitor = NULL;
    i = 0;
    cJSON_ArrayForEach(monitor, json_monitors) {
        cJSON *json_monthly_ratio = cJSON_GetObjectItemCaseSensitive(monitor, "30dRatio");
        if (!json_monthly_ratio) goto end;

        cJSON *json_monthly_ratio_label = cJSON_GetObjectItemCaseSensitive(json_monthly_ratio, "label");
        if (!json_monthly_ratio_label || !cJSON_IsString(json_monthly_ratio_label)) goto end;
        result->monitors[i].monthly_ratio.label = strdup(json_monthly_ratio_label->valuestring);

        cJSON *json_monthly_ratio_value = cJSON_GetObjectItemCaseSensitive(json_monthly_ratio, "ratio");
        if (!json_monthly_ratio_value || !cJSON_IsString(json_monthly_ratio_value)) goto end;
        result->monitors[i].monthly_ratio.ratio = atof(json_monthly_ratio_value->valuestring);

        cJSON *json_quarter_ratio = cJSON_GetObjectItemCaseSensitive(monitor, "90dRatio");
        if (!json_quarter_ratio) goto end;

        cJSON *json_quarter_ratio_label = cJSON_GetObjectItemCaseSensitive(json_quarter_ratio, "label");
        if (!json_quarter_ratio_label || !cJSON_IsString(json_quarter_ratio_label)) goto end;
        result->monitors[i].quarter_ratio.label = strdup(json_quarter_ratio_label->valuestring);

        cJSON *json_quarter_ratio_value = cJSON_GetObjectItemCaseSensitive(json_quarter_ratio, "ratio");
        if (!json_quarter_ratio_value || !cJSON_IsString(json_quarter_ratio_value)) goto end;
        result->monitors[i].quarter_ratio.ratio = atof(json_quarter_ratio_value->valuestring);

        cJSON *json_daily_ratios = cJSON_GetObjectItemCaseSensitive(monitor, "dailyRatios");
        if (!json_daily_ratios || !cJSON_IsArray(json_daily_ratios)) goto end;
        cJSON *ratio = NULL;
        size_t j = 0;
        cJSON_ArrayForEach(ratio, json_daily_ratios) {
            cJSON *json_daily_ratio_label = cJSON_GetObjectItemCaseSensitive(ratio, "label");
            if (!json_daily_ratio_label || !cJSON_IsString(json_daily_ratio_label)) goto end;
            result->monitors[i].daily_ratios[j].label = strdup(json_daily_ratio_label->valuestring);

            cJSON *json_daily_ratio_value = cJSON_GetObjectItemCaseSensitive(ratio, "ratio");
            if (!json_daily_ratio_value || !cJSON_IsString(json_daily_ratio_value)) goto end;
            result->monitors[i].daily_ratios[j].ratio = atof(json_daily_ratio_value->valuestring);

            j++;
        }

        cJSON *json_id = cJSON_GetObjectItemCaseSensitive(monitor, "monitorId");
        if (!json_id || !cJSON_IsNumber(json_id)) goto end;
        result->monitors[i].id = json_id->valueint;

        cJSON *json_name = cJSON_GetObjectItemCaseSensitive(monitor, "name");
        if (!json_name || !cJSON_IsString(json_name)) goto end;
        result->monitors[i].name = strdup(json_name->valuestring);
        
        cJSON *json_status = cJSON_GetObjectItemCaseSensitive(monitor, "statusClass");
        if (!json_status || !cJSON_IsString(json_status)) goto end;
        result->monitors[i].status = strdup(json_status->valuestring);

        i++;
    }

    cJSON *json_statistics = cJSON_GetObjectItemCaseSensitive(json_root, "statistics");
    if (!json_statistics) goto end;

    cJSON *json_count_result = cJSON_GetObjectItemCaseSensitive(json_statistics, "count_result");
    if (!json_count_result || !cJSON_IsString(json_count_result)) goto end;
    result->statistics.count_result = strdup(json_count_result->valuestring);

    cJSON *json_counts = cJSON_GetObjectItemCaseSensitive(json_statistics, "counts");
    if (!json_counts) goto end;

    cJSON *json_down = cJSON_GetObjectItemCaseSensitive(json_counts, "down");
    if (!json_down || !cJSON_IsNumber(json_down)) goto end;
    result->statistics.count_down = json_down->valueint;

    cJSON *json_paused = cJSON_GetObjectItemCaseSensitive(json_counts, "paused");
    if (!json_paused || !cJSON_IsNumber(json_paused)) goto end;
    result->statistics.count_paused = json_paused->valueint;

    cJSON *json_up = cJSON_GetObjectItemCaseSensitive(json_counts, "up");
    if (!json_up || !cJSON_IsNumber(json_up)) goto end;
    result->statistics.count_up = json_up->valueint;

    return result;

end:
    if (result) free_monitor_list_result(result);
    cJSON_Delete(json_root);
    return NULL;    
}

//Frontend

void print_arch_logo() {
	printf(ARCH_LOGO_ASCII_ART);
	printf("\n\n");
}

void print_monitors_title() {
	printf("%s%s%s\n\n", ANSI_BOLD, "Monitors (default)", ANSI_COLOR_RESET);
}

void print_monitor_data(monitor_t *monitor) {
		printf("%s -> | %s", monitor->name, format_ratio(&(monitor->quarter_ratio)));
		printf("\t\t%s\n", format_monitor_status(monitor->status));
		ratio_t *daily_ratios = monitor->daily_ratios;
		for(int daily_ratio_i = sizeof(daily_ratios)-1; daily_ratio_i >= 0; daily_ratio_i--) {
			ratio_t daily_ratio = daily_ratios[daily_ratio_i];
			printf("%s ", ratio_to_colored_space(&daily_ratio));
		}
		printf("\n\n");
}

char* format_ratio(ratio_t *ratio) {
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

char* ratio_to_colored_space(ratio_t *ratio) {
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

char* format_monitor_status(char *status) {
	char buf[128];
	if(strcmp(status, STATUS_OPERATIONAL_CODE) == 0)
		 sprintf(buf, "%s%s %s%s", COLOR_GREEN_FOREGROUND, BIG_BLACK_CIRCLE, "Operational", ANSI_COLOR_RESET);
	else if(strcmp(status, STATUS_DOWN_CODE) == 0)
		 sprintf(buf, "%s%s %s%s", COLOR_RED_FOREGROUND, BIG_BLACK_CIRCLE, "Down", ANSI_COLOR_RESET);
	else 
		 sprintf(buf, "%s%s %s%s", COLOR_GREY_FOREGROUND, BIG_BLACK_CIRCLE, "Unknown", ANSI_COLOR_RESET);
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
