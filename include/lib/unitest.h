#ifndef UNITEST_H
#define UNITEST_H

#include <signal.h>
#include <stdio.h>
#include <string.h>

void unitest_register_test(void (*test_func)());

#define UNITEST(func) \
    void func(); \
    void func##_unitest() { \
	unitest_begin(#func); \
	func(); \
	unitest_end(); \
    } \
    __attribute__((constructor)) void register_##func##_test() {\
	unitest_register_test(&func##_unitest); \
    }\
    void func()

typedef struct {
	int passed;
	int failed;
} unitest_status_t;

void unitest_begin(const char* name);

void unitest_end(void);

void unitest_assert(int condition, const char* description);

void unitest_run(void (*tests)(void));

void unitest_summary(unitest_status_t* status);

#endif
