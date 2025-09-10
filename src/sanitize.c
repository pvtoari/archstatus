#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "sanitize.h"

char *replace_str(const char *str, const char *radix, const char *rep) {
    size_t radix_len = strlen(radix);
    size_t rep_len  = strlen(rep);

    int count = 0;
    const char *p = str;
    while ((p = strstr(p, radix)) != NULL) {
        count++;
        p += radix_len;
    }

    size_t new_len = strlen(str) + count * (rep_len - radix_len) + 1;
    char *result = malloc(new_len);
    if (!result) return NULL;

    char *out = result;
    const char *in = str;
    while ((p = strstr(in, radix)) != NULL) {
        size_t len_before = p - in;
        memcpy(out, in, len_before);
        out += len_before;
        memcpy(out, rep, rep_len);
        out += rep_len;
        in = p + radix_len;
    }

    strcpy(out, in);

    return result;
}

char *sanitize_content_str(char *content_str) {
    char *nobrs = replace_str(content_str, "<br />", "");
    char *quoted = replace_str(nobrs, "&quot;", "\"");
    if(nobrs) free(nobrs);

    return quoted;
}

char *sanitize_os_release_str(char *os_release_str) {
    char *nonewlines = replace_str(os_release_str, "\n", "");
    if (nonewlines) free(os_release_str);
    char *noquotes = replace_str(nonewlines, "\"", "");
    if (nonewlines) free(nonewlines);
    char *nospaces = replace_str(noquotes, " ", "");
    if (noquotes) free(noquotes);
	for (char *p = nospaces; *p; ++p) *p = tolower(*p);

    return nospaces;
}
