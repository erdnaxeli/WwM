#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "logger.h"

static enum level level = ERROR;

void set_logger_level(enum level lvl)
{
    level = lvl;
}

char *lvl_to_str(enum level lvl)
{
    char *buffer = malloc(sizeof(char) * 9);

    if (buffer == NULL)
        goto lvl_to_str_out;

    switch (lvl) {
#define CASE(lvl) case lvl: sprintf(buffer, #lvl); break
        CASE(INFO);
        CASE(DEBUG);
        CASE(WARN);
        CASE(ERROR);
        CASE(CRITICAL);
        default:
            sprintf(buffer, "wtf ?");
#undef CASE
    }

lvl_to_str_out:
    return buffer;
}

void logger(enum level lvl, char* format, ...)
{
    if (lvl < level)
        return;

    time_t now;
    time(&now);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';

    char *lvl_str = lvl_to_str(lvl);

    va_list args;
    va_start(args, format);

    int size = strlen(format) + strlen(time_str) + strlen(lvl_str);
    char *buffer = malloc(sizeof(char) * (size + 6 + 1));

    if (buffer != NULL) {
        sprintf(buffer, "%s [%s]: %s\n", time_str, lvl_str, format);
        vprintf(buffer, args);
    }

    free(buffer);
    free(lvl_str);
}
