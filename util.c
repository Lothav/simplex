

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "util.h"

char** regexRow(char* row_src, long row_size, const char* row_pattern)
{
    int res, len;
    char **return_value = malloc(row_size * sizeof(char *)),
            *src_ptr = row_src;

    regex_t preg;
    regmatch_t pmatch[10];

    if( (res = regcomp(&preg, row_pattern, REG_EXTENDED)) != 0)
    {
        char err_buf[BUFSIZ];
        regerror(res, &preg, err_buf, BUFSIZ);
        printf("regcomp() error: %s\n", err_buf);
        exit(res);
    }

    long offset = 0, i;
    for (i = 0; i < row_size; i++)
    {
        src_ptr += offset;
        res = regexec(&preg, src_ptr, 10, pmatch, REG_NOTBOL);
        if (res == REG_NOMATCH) break;

        offset = pmatch[0].rm_eo - pmatch[0].rm_so;
        len    = pmatch[1].rm_eo - pmatch[1].rm_so;

        return_value[i] = malloc((size_t)len + 1);
        memcpy(return_value[i], src_ptr + pmatch[1].rm_so, (size_t)len);
        return_value[i][len] = '\0';
    }

    regfree(&preg);
    return return_value;
}

