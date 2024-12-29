#ifndef _LOG_H_
#define _LOG_H_


#ifndef LOG_TAG
#define LOG_TAG "NO-TAG"
#endif

enum {
  CRIT = 0,
  ERR,
  WARN,
  NOTE,
  INFO,
  DEBUG,
  LEVEL_MAX
};

#ifndef DEFAULT_LOGLEVEL
#define DEFAULT_LOGLEVEL DEBUG
#endif


#include <stdlib.h>

void _msg(char *tag ,unsigned int level, const char *fmt, ...);

#define die(...) (_msg(LOG_TAG, CRIT, __VA_ARGS__), exit(1))
#define err(...) _msg(LOG_TAG, ERR, __VA_ARGS__)
#define warn(...) _msg(LOG_TAG, WARN, __VA_ARGS__)
#define note(...) _msg(LOG_TAG, NOTE, __VA_ARGS__)
#define info(...) _msg(LOG_TAG, INFO, __VA_ARGS__)
#define dbg(...) _msg(LOG_TAG, DEBUG, __VA_ARGS__)


#endif
