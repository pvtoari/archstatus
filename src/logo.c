#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logo.h"

char *get_os_release() {
    FILE *fp;
    char buffer[64] = "";

    fp = popen("/usr/bin/lsb_release -ds", "r");
    if (fgets(buffer, sizeof buffer, fp) == NULL) {
    	buffer[0] = '\0'; /* Safe value if error or EOF */
    }
    pclose(fp);

    return strdup(buffer);
}

char *read_fallback_file(char *path) {
    if (access(path, F_OK) != 0) {
        fprintf(stderr, "warning: no logo found for '%s', using archlinux logo as fallback.\n", path);
        return LOGO_ARCHLINUX;
    }

    FILE *file;
    file = fopen(path, "r");

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char ch;
    size_t i = 0;
    char *content = malloc(size);
    while ((ch = fgetc(file)) != EOF)
        content[i++] = ch;

    content[i] = '\0';

    fclose(file);
    return content;
}

char *get_logo_for(char *os_release) {
#define COMPARE(x) if (strcmp(os_release, x) == 0)
    COMPARE("archbang") return LOGO_ARCHBANG;
    COMPARE("archex") return LOGO_ARCHEX;
    COMPARE("archman") return LOGO_ARCHMAN;
    COMPARE("archlinux") return LOGO_ARCHLINUX;
    COMPARE("archstrike") return LOGO_ARCHSTRIKE;
    COMPARE("arco") return LOGO_ARCOLINUX;
    COMPARE("artix") return LOGO_ARTIXLINUX;
    COMPARE("blackarch") return LOGO_BLACKARCH;
    COMPARE("bluestar") return LOGO_BLUESTARLINUX;
    COMPARE("chimeraos") return LOGO_CHIMERAOS;
    COMPARE("ctlos") return LOGO_CTLOSLINUX;
    COMPARE("crystal") return LOGO_CRYSTALLINUX;
    COMPARE("endeavouros") return LOGO_ENDEAVOUROS;
    COMPARE("garuda") return LOGO_GARUDALINUX;
    COMPARE("hyperbola") return LOGO_HYPERBOLA;
    COMPARE("instantos") return LOGO_INSTANTOS;
    COMPARE("kaos") return LOGO_KAOS;
    COMPARE("manjaro") return LOGO_MANJARO;
    COMPARE("msys2") return LOGO_MSYS2;
    COMPARE("obarun") return LOGO_OBARUN;
    COMPARE("parabola") return LOGO_PARABOLA;
    COMPARE("puppyrus-a") return LOGO_PUPPYRUSA;
    COMPARE("rebornos") return LOGO_REBORNOS;
    COMPARE("snal") return LOGO_SNALLINUX;
    COMPARE("steamos") return LOGO_STEAMOS3;
    COMPARE("systemrescue") return LOGO_SYSTEMRESCUE;
    COMPARE("tearch") return LOGO_TEARCHLINUX;
    COMPARE("ubos") return LOGO_UBOS;

    return read_fallback_file(os_release);
}
