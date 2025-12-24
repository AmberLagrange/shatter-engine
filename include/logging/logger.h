#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <stdio.h>

void enable_logging(void);
void disable_logging(void);

void log_message(FILE *file, const char *format, ...);

#endif // LOGGING_LOGGER_H
