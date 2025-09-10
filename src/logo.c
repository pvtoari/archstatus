#include <string.h>
#include <stdio.h>

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

char *get_logo_for(char *os_release) {
#define COMPARE(x) if (strcmp(os_release, x) == 0)
    COMPARE("\"ArchBang\"") return LOGO_ARCHBANG;
    COMPARE("\"ArchEx\"") return LOGO_ARCHEX;
    COMPARE("\"Archman\"") return LOGO_ARCHMAN;
    COMPARE("\"Arch Linux\"") return LOGO_ARCHLINUX;
    COMPARE("\"ArchStrike\"") return LOGO_ARCHSTRIKE;
    COMPARE("\"ArcoLinux\"") return LOGO_ARCOLINUX;
    COMPARE("\"Artix Linux\"") return LOGO_ARTIXLINUX;
    COMPARE("\"BlackArch Linux\"") return LOGO_BLACKARCH;
    COMPARE("\"Bluestar Linux\"") return LOGO_BLUESTARLINUX;
    COMPARE("\"ChimeraOS\"") return LOGO_CHIMERAOS;
    COMPARE("\"Ctlos Linux\"") return LOGO_CTLOSLINUX;
    COMPARE("\"Crystal Linux\"") return LOGO_CRYSTALLINUX;
    COMPARE("\"EndeavourOS\"") return LOGO_ENDEAVOUROS;
    COMPARE("\"Garuda Linux\"") return LOGO_GARUDALINUX;
    COMPARE("\"Hyperbola\"") return LOGO_HYPERBOLA;
    COMPARE("\"instantOS\"") return LOGO_INSTANTOS;
    COMPARE("\"KaOS\"") return LOGO_KAOS;
    COMPARE("\"Manjaro Linux\"") return LOGO_MANJARO;
    COMPARE("\"MSYS2\"") return LOGO_MSYS2;
    COMPARE("\"Obarun\"") return LOGO_OBARUN;
    COMPARE("\"Parabola\"") return LOGO_PARABOLA;
    COMPARE("\"PuppyRus-A\"") return LOGO_PUPPYRUSA;
    COMPARE("\"RebornOS\"") return LOGO_REBORNOS;
    COMPARE("\"Snal Linux\"") return LOGO_SNALLINUX;
    COMPARE("\"SteamOS 3\"") return LOGO_STEAMOS3;
    COMPARE("\"SystemRescue\"") return LOGO_SYSTEMRESCUE;
    COMPARE("\"TeArch Linux\"") return LOGO_TEARCHLINUX;
    COMPARE("\"UBOS\"") return LOGO_UBOS;

    return LOGO_ARCHLINUX;
}
