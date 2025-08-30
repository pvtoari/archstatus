#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "archstatus.h"

void usage() {
    printf("Usage: %s [options]\n", PROGRAM_NAME);
    printf("Options:\n");
    printf("  -e, --events      Show latest status events\n");
    printf("  -f, --forum       Show forum status\n");
    printf("  -s, --site        Show site status\n");
    printf("  -r, --aur         Show AUR status\n");
    printf("  -w, --wiki	    Show wiki status\n");
    printf("  -h, --help        Shows this help message\n");
}

int main(int argc, char *argv[]) {
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
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
    while ((opt = getopt_long(argc, argv, "hwrsfe", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                any = false;
                usage();
                exit(EXIT_SUCCESS);
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
                printf("Unrecognized option: %c", opt);
                usage();
                exit(EXIT_FAILURE);
        }
    }

    if (any) {
        fetch_data_t *events_data = fetch_url(ARCHLINUX_STATUS_EVENT_FEED_ENDPOINT);
        if (!events_data) {
            fprintf(stderr, "error: failed to fetch events data.\n");
            exit(EXIT_FAILURE);
        }

        fetch_data_t *monitors_data = fetch_url(ARCHLINUX_STATUS_MONITOR_LIST_ENDPOINT);
        if(!monitors_data) {
            fprintf(stderr, "error: failed to fetch monitors data.\n");
            exit(EXIT_FAILURE);
        }

        latest_events_result_t *events = parse_events_data(events_data);
        if (!events) {
            fprintf(stderr, "error: failed to parse events data.\n");
            exit(EXIT_FAILURE);
        }

        monitor_list_result_t *monitors = parse_monitor_list_data(monitors_data);
        if (!monitors) {
            fprintf(stderr, "error: failed to fetch parse monitors data.\n");
            exit(EXIT_FAILURE);
        }

        // WIP: Terminal frontend stuff
		printf("%s", format_monitors_data(parse_monitor_list_data(monitors_data)));
    }
}
