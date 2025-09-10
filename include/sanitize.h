#ifndef SANITIZE_H
#define SANITIZE_H

char *replace_str(const char *str, const char *radix, const char *rep);

char *sanitize_content_str(char *content_str);

char *sanitize_os_release_str(char *os_release_str);

#endif // SANITIZE_H
