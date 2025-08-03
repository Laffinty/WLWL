#ifndef WL_LOGGER_H
#define WL_LOGGER_H

// Enum for different log levels
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} LogLevel;

/**
 * @brief Logs a message with a specific level.
 * * @param level The logging level.
 * @param format The message format string (like printf).
 * @param ... The arguments for the format string.
 */
void wl_log(LogLevel level, const char* format, ...);

#endif // WL_LOGGER_H
