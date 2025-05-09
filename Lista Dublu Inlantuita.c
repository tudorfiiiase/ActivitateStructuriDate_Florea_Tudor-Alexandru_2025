#include <stdio.h>
#include <stdlib.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct Node {
    int val;
    struct Node* prev;
    struct Node* next;
  } Node;
Node* createList(int n) {
    Node* head = NULL, * tail = NULL;
    for (int i = 0; i < n; i++) {
        int val;
        scanf_s("%d", &val);
        Node* node = (Node*)malloc(sizeof(Node));
        node->val = val;
        node->next = NULL;
        node->prev = tail;
        if (tail)
            tail->next = node;
        else
            head = node;
        tail = node;
    }
    return head;
}
Node* mergeLists(Node* a, Node* b) {
    Node* head = NULL, * tail = NULL;

    while (a && b) {
        Node* chosen;
        if (a->val <= b->val) {
            chosen = a;
            a = a->next;
        }
        else {
            chosen = b;
            b = b->next;
        }

        chosen->prev = tail;
        chosen->next = NULL;

        if (tail)
            tail->next = chosen;
        else
            head = chosen;

        tail = chosen;
    }

    Node* remaining = (a) ? a : b;
    while (remaining) {
        Node* next = remaining->next;
        remaining->prev = tail;
        remaining->next = NULL;
        if (tail)
            tail->next = remaining;
        else
            head = remaining;
        tail = remaining;
        remaining = next;
    }

    return head;
}
void printList(Node* head) {
    while (head) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}
int main() {
    int n, m;
    scanf_s("%d %d", &n, &m);

    Node* list1 = createList(n);
    Node* list2 = createList(m);

    Node* merged = mergeLists(list1, list2);

    printList(merged);

    return 0;
}




