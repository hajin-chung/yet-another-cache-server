#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

void logger(enum LogLevel level, const char* format, ...) {
	va_list args;

	time_t rawTime = time(NULL);
	char* timeStr = ctime(&rawTime);
	timeStr[strlen(timeStr)-1] = '\0';
	printf("[%s]\t", timeStr);

	va_start(args, format);
	switch (level) // TODO: handle different types of log
	{
	case INFO:
	case WARNING:
	case ERROR:
	default:
		vprintf(format, args);
		printf("\n");
		break;
	}
	va_end(args);
}