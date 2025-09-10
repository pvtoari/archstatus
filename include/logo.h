#ifndef LOGO_H
#define LOGO_H

#define LOGO_ARCHLINUX "\
\n\033[38;2;23;147;209m\x1b[1m          .\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m         /#\\     \x1b[0m         #    \033[38;2;23;147;209m | *\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m        /###\\    \x1b[0m### #### #### \033[38;2;23;147;209m | | |---. |   | \\ /\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m       /p^###\\   \x1b[0m#   #    #  # \033[38;2;23;147;209m | | |   | |   |  X\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m      /##P^q##\\  \x1b[0m#   #### #  # \033[38;2;23;147;209m | | |   | ^._.| / \\\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m     /##(   )##\\\x1b[0m\
\n\033[38;2;23;147;209m\x1b[1m    /###P   q#,^\\\x1b[0m     %s\
\n\033[38;2;23;147;209m\x1b[1m   /P^         ^q\\\x1b[0m"

#define LOGO_ARCHBANG ""
#define LOGO_ARCHEX ""
#define LOGO_ARCHMAN ""
#define LOGO_ARCHSTRIKE ""

#define LOGO_ARCOLINUX ""

#define LOGO_ARTIXLINUX "\n\
\033[36m      /\\             \033[0m \n\
\033[36m     /  \\            \033[0m   #          #             #                           \n\
\033[36m    /`'.,\\           \033[0m  # #    ##   #   #  #   #  #     #  ###   #  #  #   #  \n\
\033[36m   /     ',          \033[0m #   #  #    ###      # #   #        #  #  #  #   # #   \n\
\033[36m  /      ,`\\         \033[0m #####  #     #   #    #    #     #  #  #  #  #    #    \n\
\033[36m /   ,.'`.  \\        \033[0m #   #  #     #   #   # #   #     #  #  #  #  #   # #   \n\
\033[36m/.,'`     `'.\\       \033[0m #   #  #     ##  #  #   #  ####  #  #  #   ###  #   #  \n\
\n\
                                %s"

#define LOGO_BLACKARCH ""
#define LOGO_BLUESTARLINUX ""
#define LOGO_CHIMERAOS ""
#define LOGO_CTLOSLINUX ""
#define LOGO_CRYSTALLINUX ""

#define LOGO_ENDEAVOUROS "\n\
\033[31m           /*            \033[0m \n\
\033[31m         #\033[35m****\033[34m*          \033[0m ####                                                       ###    ####  \n\
\033[31m       #\033[35m********\033[34m#        \033[0m #     ###       #   ###    ###   #   #   ###   #  #   ##  #   #  #      \n\
\033[31m     ##\033[35m**********\033[34m##      \033[0m ###   #  #      #  #   #      #  #   #  #   #  #  #  #    #   #   ###   \n\
\033[31m   ##*\033[35m************\033[34m##     \033[0m #     #  #   ####  #####   ####   # #   #   #  #  #  #    #   #      #  \n\
\033[31m ###*\033[35m*************\033[34m##     \033[0m #     #  #  #   #  #      #   #   # #   #   #  #  #  #    #   #      #  \n\
\033[34m   ###############       \033[0m ####  #  #   ####   ###    ####    #     ###    ###  #     ###   ####   \n\
\n\
                                 %s"

#define LOGO_GARUDALINUX ""
#define LOGO_HYPERBOLA ""
#define LOGO_INSTANTOS ""
#define LOGO_KAOS ""
#define LOGO_MANJARO ""
#define LOGO_MSYS2 ""
#define LOGO_OBARUN ""
#define LOGO_PARABOLA ""
#define LOGO_PUPPYRUSA ""
#define LOGO_REBORNOS ""
#define LOGO_SNALLINUX ""
#define LOGO_STEAMOS3 ""
#define LOGO_SYSTEMRESCUE ""
#define LOGO_TEARCHLINUX ""
#define LOGO_UBOS ""

char *get_os_release();

char *read_fallback_file(char *path);

char *get_logo_for(char *os_release);

#endif // LOGO_H
