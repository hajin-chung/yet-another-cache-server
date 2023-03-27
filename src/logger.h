enum LogLevel {
	INFO,
	WARNING,
	ERROR
};

// TODO: input where to print out logs (stdout, specific file)
void logger(enum LogLevel level, const char* format, ...);