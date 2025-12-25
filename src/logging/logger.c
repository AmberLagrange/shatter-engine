#include <logging/logger.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

static bool s_logging_enabled = false;
static bool s_log_prefixes_enabled = false;
static bool s_log_colors_enabled = false;

static const char *LOG_LEVEL_PREFIXES[NUM_LOG_LEVELS] = {
	
	"Debug",
	"Trace",
	"Info",
	"Warning",
	"Error",
	"Critical",
};

// Hacky
// TODO: Do this properly

static const char *LOG_LEVEL_SUFFIXES[NUM_LOG_LEVELS] = {
	
	"    ",  // Debug
	"    ",  // Trace
	"     ", // Info
	"  ",    // Warning
	"    ",  // Error
	" ",     // Critical
};

#ifdef ENABLE_NON_STANDARD_ESCAPE_SEQUENCES
	
	static const char *LOG_LEVEL_COLOR_CODES[NUM_LOG_COLORS] = {
		
		"\e[0;37m",  // White
		"\e[0;34m",  // Blue
		"\e[0;32m",  // Green
		"\e[0;33m",  // Yellow
		"\e[0;31m",  // Red
		"\e[0;101m", // Bold Red
		"\e[0m",     // Reset
	};

#else
	
	static const char *LOG_LEVEL_COLOR_CODES[NUM_LOG_COLORS] = {
		
		"",
		"",
		"",
		"",
		"",
		"",
		"",
	};

#endif

void enable_logging(void)  { s_logging_enabled = true;  }
void disable_logging(void) { s_logging_enabled = false; }

void enable_log_prefixes(void)  { s_log_prefixes_enabled = true;  }
void disable_log_prefixes(void) { s_log_prefixes_enabled = false; }

void enable_log_colors(void)  { s_log_colors_enabled = true;  }
void disable_log_colors(void) { s_log_colors_enabled = false; }

void swap_color(FILE *file, log_color_t log_color) {
	
	if (!s_log_colors_enabled) {
		
		return;
	}
	
	fprintf(file, "%s", LOG_LEVEL_COLOR_CODES[log_color]);
}

void v_log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, va_list args) {
	
	if (!s_logging_enabled) {
		
		return;
	}
	
	if (s_log_prefixes_enabled) {
		
		fprintf(file, "[");
		swap_color(file, log_color);
		fprintf(file, "%s", LOG_LEVEL_PREFIXES[log_level]);
		swap_color(file, LOG_COLOR_RESET);
		fprintf(file, "]");
		fprintf(file, "%s", LOG_LEVEL_SUFFIXES[log_level]); // Still hacky
	}
	
	swap_color(file, log_color);
	vfprintf(file, format, args);
	swap_color(file, LOG_COLOR_RESET);
}

void log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(file, log_level, log_color, format, args);
	va_end(args);
}

void log_debug(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stdout, LOG_LEVEL_DEBUG, LOG_COLOR_WHITE, format, args);
	va_end(args);
}

void log_trace(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stdout, LOG_LEVEL_TRACE, LOG_COLOR_BLUE, format, args);
	va_end(args);
}

void log_info(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stdout, LOG_LEVEL_INFO, LOG_COLOR_GREEN, format, args);
	va_end(args);
}

void log_warning(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stdout, LOG_LEVEL_WARNING,  LOG_COLOR_YELLOW, format, args);
	va_end(args);
}

void log_error(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stderr, LOG_LEVEL_ERROR, LOG_COLOR_RED, format, args);
	va_end(args);
}
void log_critical(const char *format, ...) {
	
	va_list args;
	va_start(args, format);
	v_log_message(stderr, LOG_LEVEL_CRITICAL, LOG_COLOR_INTENSE_RED, format, args);
	va_end(args);
}

void test_logging(void) {
	
	log_debug("Test\n");
	log_trace("Test\n");
	log_info("Test\n");
	log_warning("Test\n");
	log_error("Test\n");
	log_critical("Test\n");
}

