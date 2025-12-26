#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdio.h>

typedef enum log_level_e {
	
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_INFO,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_CRITICAL,
	NUM_LOG_LEVELS		// Has to be last, will assign this value to the number of log levels
	
} log_level_t;

typedef enum log_color_e {
	
	LOG_COLOR_WHITE = 0,
	LOG_COLOR_BLUE,
	LOG_COLOR_GREEN,
	LOG_COLOR_YELLOW,
	LOG_COLOR_RED,
	LOG_COLOR_INTENSE_RED,
	LOG_COLOR_RESET,
	NUM_LOG_COLORS		// Same as above
	
} log_color_t;

extern void init_logging(void);

extern void enable_logging(void);
extern void disable_logging(void);

extern void enable_log_prefixes(void);
extern void disable_log_prefixes(void);

extern void enable_log_colors(void);
extern void disable_log_colors(void);

extern void enable_log_prefix_colors(void);
extern void disable_log_prefix_colors(void);

extern void enable_log_message_colors(void);
extern void disable_log_message_colors(void);

extern void set_log_level(log_level_t log_level);

extern void v_log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, va_list args);
extern void log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, ...);

extern void log_debug(const char *format, ...);
extern void log_trace(const char *format, ...);
extern void log_info(const char *format, ...);
extern void log_warning(const char *format, ...);
extern void log_error(const char *format, ...);
extern void log_critical(const char *format, ...);

extern void test_logging(void);

#endif // LOGGER_H
