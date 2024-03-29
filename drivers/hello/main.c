#include <linux/init.h>
#include <linux/module.h>

#include "container/linklist.h"

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
    printk(KERN_ALERT "Hello, world\n");
    struct list_list mylist;
    hello_list_init(&mylist);
    hello_list_push(&mylist, 10);
    hello_list_push(&mylist, 20);
    hello_list_print(&mylist);
    while (mylist.head) {
	hello_list_del(&mylist, mylist.head);
    }
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
