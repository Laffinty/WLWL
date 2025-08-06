#ifndef WL_LOGGER_H
#define WL_LOGGER_H

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

void wl_log(LogLevel level, const char* format, ...);

#endif