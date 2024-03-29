#ifdef KERNELMODE
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include "container/linklist.h"

/* 初始化链表 */
void hello_list_init(struct list_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

/* 添加节点到链表尾部 */
void hello_list_push(struct list_list *list, int value)
{
#ifdef KERNELMODE
    struct node *new_node = kmalloc(sizeof(struct node), GFP_KERNEL);
#else
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
#endif
    if (!new_node)
         return;

    new_node->value = value;
    new_node->next = NULL;

    if (list->tail) {
        new_node->prev = list->tail;
        list->tail->next = new_node;
    } else {
        new_node->prev = NULL;
        list->head = new_node;
    }
    list->tail = new_node;
}

/* 从链表中删除节点 */
void hello_list_del(struct list_list *list, struct node *node)
{
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;
#ifdef KERNELMODE
    kfree(node);
#else
    free(node);
    node = NULL;
#endif
}

/* 遍历链表并打印节点值 */
void hello_list_print(struct list_list *list)
{
    struct node *curr = list->head;
#ifdef KERNELMODE
    while (curr) {
        printk(KERN_INFO "Value: %d\n", curr->value);
        curr = curr->next;
    }
#else
    while (curr) {
        printf("Value: %d\n", curr->value);
        curr = curr->next;
    }
#endif
}

