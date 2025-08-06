#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "common.h"

void wl_log(LogLevel level, const char* format, ...) {
#ifndef WL_DEBUG_LOG
    if (level == LOG_LEVEL_DEBUG) {
        return;
    }
#endif

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char time_buf[32];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    switch (level) {
        case LOG_LEVEL_DEBUG: fprintf(stdout, "[%s] [DEBUG] ", time_buf); break;
        case LOG_LEVEL_INFO:  fprintf(stdout, "[%s] [INFO]  ", time_buf); break;
        case LOG_LEVEL_WARN:  fprintf(stderr, "[%s] [WARN]  ", time_buf); break;
        case LOG_LEVEL_ERROR: fprintf(stderr, "[%s] [ERROR] ", time_buf); break;
        case LOG_LEVEL_FATAL: fprintf(stderr, "[%s] [FATAL] ", time_buf); break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(level >= LOG_LEVEL_WARN ? stderr : stdout, format, args);
    va_end(args);

    fprintf(level >= LOG_LEVEL_WARN ? stderr : stdout, "\n");
}