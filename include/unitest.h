#ifndef UNITEST_H
#define UNITEST_H

#include <stdio.h>
#include <string.h>

#define UNITEST_PASS 0
#define UNITEST_FAIL 1

#define UNITEST(func) \
    void func(); \
    __attribute__((constructor)) void func##_unitest() { \
	unitest_begin(#func); \
	func(); \
    } \
    void func()

typedef struct {
	int passed;
	int failed;
} unitest_status_t;

static const char* UNITEST_SUITE_NAME = NULL;
static unitest_status_t UNITEST_STATUS = {0, 0};
static void (*__unitest_funcs[])(void) = {NULL};

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

void unitest_run_all(void) {
	size_t i = 0;
	while (__unitest_funcs[i] != NULL) {
		__unitest_funcs[i]();
		i++;
	}
}

void unitest_summary(unitest_status_t* status) {
	*status = UNITEST_STATUS;
	printf("\nSummary:\n");
	printf("    Passed: %d\n", status->passed);
	printf("    Failed: %d\n", status->failed);
}

#ifdef UNITEST_MAIN

int main(void) {
	unitest_status_t status = {0, 0};
	unitest_run_all();
	unitest_summary(&status);
	return status.failed > 0 ? UNITEST_FAIL : UNITEST_PASS;
}

#endif

#endif
