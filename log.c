#include "log.h"

#include <stdio.h>
#include <stdarg.h> /* for operating va_list */
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void _msg(char *tag ,unsigned int level, const char *fmt, ...)
{
    FILE *output;
    va_list ap;
    static const char level_msg[LEVEL_MAX][6] = {
	[CRIT] = "crit",
	[ERR] = "err",
	[WARN] = "warn",
	[NOTE] = "note",
	[INFO] = "info",
	[DEBUG] = "debug"
    };
    static const char level_color[LEVEL_MAX][9] = {
	[CRIT] = ANSI_COLOR_RED,
	[ERR] = ANSI_COLOR_RED,
	[WARN] = ANSI_COLOR_YELLOW,
	[NOTE] = ANSI_COLOR_GREEN,
	[INFO] = "",
	[DEBUG] = "",
    };

    if (level <= WARN) {
	output = stderr;
    } else if (level <= DEBUG){
	output = stdout;
    } else {
	output = stderr;
	level = ERR;
    }

    if (level > DEFAULT_LOGLEVEL)
	return;

    if (strncmp(LOG_TAG, tag, 6)) {
	fprintf(output, "%s[%s][%s]:", level_color[level], level_msg[level], tag);
    } else {
	fprintf(output, "%s[%s]: ", level_color[level], level_msg[level]);
    }
    va_start(ap, fmt);
    vfprintf(output, fmt, ap);
    va_end(ap);
    fprintf(output, "%s", ANSI_COLOR_RESET);
    fflush(output);
}
