#include <logging/logger.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool s_logging_enabled            = false;

static bool s_log_prefixes_enabled       = false;

static bool s_log_colors_enabled         = false;
static bool s_log_prefix_colors_enabled  = false;
static bool s_log_message_colors_enabled = false;

static const char *LOG_LEVEL_PREFIXES[NUM_LOG_LEVELS] = {
	
	"Debug",
	"Trace",
	"Info",
	"Warning",
	"Error",
	"Critical",
};

static size_t max_prefix_len = 0;

static const char *LOG_LEVEL_COLOR_CODES[NUM_LOG_COLORS] = {
	
	"\033[0;37m",  // White
	"\033[0;34m",  // Blue
	"\033[0;32m",  // Green
	"\033[0;33m",  // Yellow
	"\033[0;31m",  // Red
	"\033[0;101m", // Bold Red
	"\033[0m",     // Reset
};

void init_logging(void) {
	
	for (size_t prefix_index = 0; prefix_index < NUM_LOG_LEVELS; ++prefix_index) {
		
		size_t prefix_len = strlen(LOG_LEVEL_PREFIXES[prefix_index]);
		max_prefix_len = (prefix_len > max_prefix_len) ? prefix_len : max_prefix_len;
	}
}

void enable_logging(void)  { s_logging_enabled = true;  }
void disable_logging(void) { s_logging_enabled = false; }

void enable_log_prefixes(void)  { s_log_prefixes_enabled = true;  }
void disable_log_prefixes(void) { s_log_prefixes_enabled = false; }

void enable_log_colors(void)  { s_log_colors_enabled = true;  }
void disable_log_colors(void) { s_log_colors_enabled = false; }

void enable_log_prefix_colors(void)  { s_log_prefix_colors_enabled = true;  }
void disable_log_prefix_colors(void) { s_log_prefix_colors_enabled = false; }

void enable_log_message_colors(void)  { s_log_message_colors_enabled = true;  }
void disable_log_message_colors(void) { s_log_message_colors_enabled = false; }

static void swap_prefix_color(FILE *file, log_color_t log_color) {
	
	if (!s_log_colors_enabled || !s_log_prefix_colors_enabled) {
		
		return;
	}
	
	fprintf(file, "%s", LOG_LEVEL_COLOR_CODES[log_color]);
}

static void swap_message_color(FILE *file, log_color_t log_color) {
	
	if (!s_log_colors_enabled || !s_log_message_colors_enabled) {
		
		return;
	}
	
	fprintf(file, "%s", LOG_LEVEL_COLOR_CODES[log_color]);
	fflush(file);
}

static void print_prefix(FILE *file, log_level_t log_level, log_color_t log_color) {
	
	if (!s_log_prefixes_enabled) {
		
		return;
	}
	
	fprintf(file, "[");

	swap_prefix_color(file, log_color);
	const char *prefix = LOG_LEVEL_PREFIXES[log_level];
	fprintf(file, "%s", prefix);
	swap_prefix_color(file, LOG_COLOR_RESET);
	
	size_t missing_padding = max_prefix_len - strlen(prefix) + 1; // Include a space for the largest prefix
	fprintf(file, "]%-*s", (int)(missing_padding), " ");
}

static void print_message(FILE *file, log_color_t log_color, const char *format, va_list args) {
	
	swap_message_color(file, log_color);
	vfprintf(file, format, args);
	swap_message_color(file, LOG_COLOR_RESET);
}

void v_log_message(FILE *file, log_level_t log_level, log_color_t log_color, const char *format, va_list args) {
	
	if (!s_logging_enabled) {
		
		return;
	}
	
	print_prefix(file, log_level, log_color);	
	print_message(file, log_color, format, args);
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
	
	log_debug("Debug test\n");
	log_trace("Trace test\n");
	log_info("Info test\n");
	log_warning("Warning test\n");
	log_error("Error test\n");
	log_critical("Critical test\n");
}

