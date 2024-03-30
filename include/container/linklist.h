#ifndef CONTAINER_LINKLIST
#define CONTAINER_LINKLIST

struct node {
    int value;
    struct node *prev;
    struct node *next;
};

struct list_list {
    struct node *head;
    struct node *tail;
};

void hello_list_init(struct list_list *list);
int hello_list_head_val(struct list_list *list);
void hello_list_push(struct list_list *list, int value);
void hello_list_del(struct list_list *list, struct node *node);
void hello_list_print(struct list_list* list);

#endif
