#ifndef CONTAINER_LINKLIST
#define CONTAINER_LINKLIST

typedef struct Node Node;

struct Node {
    int data;
	struct Node* next;
};

Node* createNode(int data);
void append(Node**head, int data);
Node* reverseList(Node* head);
void printList(Node* head);

#endif

