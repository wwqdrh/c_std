#include <stddef.h>
#include "container/linklist.h"

int main() {
    struct list_list mylist;
    hello_list_init(&mylist);
    hello_list_push(&mylist, 10);
    hello_list_push(&mylist, 20);
    hello_list_print(&mylist);
    return 0;
}
