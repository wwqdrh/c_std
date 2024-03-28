#include <stddef.h>
#include "container/linklist.h"

int main() {
    Node* head = NULL; // 初始化链表头节点为NULL

    // 插入节点
    append(&head, 1);
    append(&head, 2);
    append(&head, 3);
    append(&head, 4);

    // 打印链表
    printList(head);

	head = reverseList(head);
	printList(head);

    head = reverseList(head);
	printList(head);
    return 0;
}
