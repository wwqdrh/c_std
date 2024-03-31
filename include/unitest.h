#ifndef UNITEST_H
#define UNITEST_H

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "trace.h"

#define UNITEST_PASS 0
#define UNITEST_FAIL 1

#define MAX_TESTS 100
static int num_tests = 0;
static void (*test_funcs[MAX_TESTS])();

#define UNITEST(func) \
    void func(); \
    void func##_unitest() { \
	unitest_begin(#func); \
	func(); \
	unitest_end(); \
    } \
    __attribute__((constructor)) void register_##func##_test() {\
        if (num_tests < MAX_TESTS) {\
	    test_funcs[num_tests++] = &func##_unitest; \
	}\
    }\
    void func()

typedef struct {
	int passed;
	int failed;
} unitest_status_t;

static const char* UNITEST_SUITE_NAME = NULL;
static unitest_status_t UNITEST_STATUS = {0, 0};

void unitest_begin(const char* name) {
	UNITEST_SUITE_NAME = name;
	printf("[%s]\n", name);
}

void unitest_end(void) {
	UNITEST_SUITE_NAME = NULL;
}

void unitest_assert(int condition, const char* description) {
	if (condition) {
		printf("PASS: %s\n", description);
		UNITEST_STATUS.passed++;
	} else {
		printf("FAIL: %s\n", description);
		UNITEST_STATUS.failed++;
	}
}

void unitest_run(void (*tests)(void)) {
	tests();
}

void unitest_summary(unitest_status_t* status) {
	*status = UNITEST_STATUS;
	printf("\nSummary:\n");
	printf("    Passed: %d\n", status->passed);
	printf("    Failed: %d\n", status->failed);
}

#ifdef UNITEST_MAIN

int main(void) {
    printf("Start Unitest...\n");
    signal(SIGSEGV, handleSegfault);
	unitest_status_t status = {0, 0};
    for (int i = 0; i < num_tests; i++) {
	test_funcs[i]();
    }
	unitest_summary(&status);
	return status.failed > 0 ? UNITEST_FAIL : UNITEST_PASS;
}

#endif

#endif
