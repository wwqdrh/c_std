#include <stddef.h>

#include "container/linklist.h"
#include "unitest.h"

UNITEST(test_linklist) {
    struct list_list mylist;
    hello_list_init(&mylist);
    hello_list_push(&mylist, 10);
    unitest_assert(hello_list_head_val(&mylist) == 10, "check head value");
    hello_list_push(&mylist, 20);
    unitest_assert(hello_list_head_val(&mylist) == 10, "check head value");
    hello_list_print(&mylist);
}

