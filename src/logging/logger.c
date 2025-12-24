#include <logging/logger.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

static bool s_logging_enabled = false;

void enable_logging(void) {
	
	s_logging_enabled = true;
}

void disable_logging(void) {
	
	s_logging_enabled = false;
}

void log_message(FILE *file, const char *format, ...) {
	
	if (!s_logging_enabled) {
		
		return;
	}
	
	va_list args;
	va_start(args, format);
	vfprintf(file, format, args);
	va_end(args);
}

