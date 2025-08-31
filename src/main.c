#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "front.h"
#include "archstatus.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

void usage() {
    printf("Usage: %s [options]\n", PROGRAM_NAME);
    printf("Options:\n");
    printf("  -e, --events          Show latest status events\n");
    printf("  -f, --forum           Show forum status\n");
    printf("  -s, --site            Show site status\n");
    printf("  -r, --aur             Show AUR status\n");
    printf("  -w, --wiki	        Show wiki status\n");
    printf("  -d, --ratio-amount    Amount of daily ratios to show (default: %d)\n", DAYS_AMOUNT);
    printf("  -h, --help            Shows this help message\n");
}

int main(int argc, char *argv[]) {
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"ratio-amount", required_argument, NULL, 'd'},
        {"wiki", no_argument, NULL, 'w'},
        {"aur", no_argument, NULL, 'r'},
        {"site", no_argument, NULL, 's'},
        {"forum", no_argument, NULL, 'f'},
        {"latest-events", no_argument, NULL, 'e'},
        {0, 0, 0, 0}
    };

    output_config_t *config = init_output_config();

    bool any = true;
    int opt;
    while ((opt = getopt_long(argc, argv, "hd:wrsfe", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                any = false;
                usage();
                exit(EXIT_SUCCESS);
            case 'd':
                config->daily_ratio_amount = MIN(DAYS_AMOUNT, atoi(optarg));
                break;
            case 'w':
                any = false;
                config->do_wiki = true;
                break;
            case 'r':
                any = false;
                config->do_aur = true;
                break;
            case 's':
                any = false;
                config->do_site = true;
                break;
            case 'f':
                any = false;
                config->do_forum = true;
                break;
            case 'e':
                any = false;
                config->do_last_events = true;
                break;
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

	if(any || config->do_aur || config->do_forum || config->do_site || config->do_wiki) {
        fetch_data_t *monitors_data = fetch_url(ARCHLINUX_STATUS_MONITOR_LIST_ENDPOINT);
        if(!monitors_data) {
            fprintf(stderr, "error: failed to fetch monitors data.\n");
            exit(EXIT_FAILURE);
        }

        monitor_list_result_t *monitors = parse_monitor_list_data(monitors_data);
        if (!monitors) {
            fprintf(stderr, "error: failed to fetch parse monitors data.\n");
            exit(EXIT_FAILURE);
        }

		print_arch_logo(monitors->statistics.count_result);

		print_monitors_title(monitors->days[config->daily_ratio_amount - 1], monitors->days[DAYS_TODAY_ORD]);
		if(any || config->do_aur) {
			print_monitor_data(&(monitors->monitors[MONITOR_ORD_AUR]), config->daily_ratio_amount);
		}
		if(any || config->do_forum) {
			print_monitor_data(&(monitors->monitors[MONITOR_ORD_FORUM]), config->daily_ratio_amount);
		}
		if(any || config->do_site) {
			print_monitor_data(&(monitors->monitors[MONITOR_ORD_SITE]), config->daily_ratio_amount);
		}
		if(any || config->do_wiki) {
			print_monitor_data(&(monitors->monitors[MONITOR_ORD_WIKI]), config->daily_ratio_amount);
		}
	} else 
		print_arch_logo("");

    if (any || config->do_last_events) {
        fetch_data_t *events_data = fetch_url(ARCHLINUX_STATUS_EVENT_FEED_ENDPOINT);
        if (!events_data) {
            fprintf(stderr, "error: failed to fetch events data.\n");
            exit(EXIT_FAILURE);
        }

        latest_events_result_t *events = parse_events_data(events_data);
        if (!events) {
            fprintf(stderr, "error: failed to parse events data.\n");
            exit(EXIT_FAILURE);
        }

		print_events(events);
	}
}
