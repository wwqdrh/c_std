#ifndef TRACE_H
#define TRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <signal.h>

void printBacktrace() {
	void* callstack[128];
	int frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	printf("Backtrace: \n");
	for (int i = 0; i < frames; ++i) {
		printf("%s\n", strs[i]);
	}
	free(strs);
}

void handleSegfault(int signal) {
	printf("Segmentation fault occurred!\n");
	printBacktrace();
	exit(1);
}

#endif
