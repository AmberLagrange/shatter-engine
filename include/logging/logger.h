#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <stdarg.h>
#include <stdio.h>

typedef enum log_level_e {
	
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_CRITICAL,
	
} log_level_t;

#define NUM_LOG_LEVELS 6

typedef enum log_color_e {
	
	LOG_COLOR_WHITE = 0,
	LOG_COLOR_BLUE,
	LOG_COLOR_GREEN,
	LOG_COLOR_YELLOW,
	LOG_COLOR_RED,
	LOG_COLOR_INTENSE_RED,
	LOG_COLOR_RESET,
	
} log_color_t;

#define NUM_LOG_COLORS 7

void enable_logging(void);
void disable_logging(void);

void enable_log_prefixes(void);
void disable_log_prefixes(void);

void enable_log_colors(void);
void disable_log_colors(void);

void v_log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, va_list args);
void log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, ...);

void log_debug(const char *format, ...);
void log_trace(const char *format, ...);
void log_info(const char *format, ...);
void log_warning(const char *format, ...);
void log_error(const char *format, ...);
void log_critical(const char *format, ...);

void test_logging(void);

#endif // LOGGING_LOGGER_H
