#include <stdio.h>
#include <stdlib.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} QueueSLL;

void initQueue(QueueSLL* q) {
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(QueueSLL* q) {
    return q->front == NULL;
}

void enqueue(QueueSLL* q, int val) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Eroare alocare memorie!\n");
        exit(1);
    }
    newNode->data = val;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = q->rear = newNode;
    }
    else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(QueueSLL* q) {
    if (isEmpty(q)) {
        printf("Eroare: coada este goală.\n");
        exit(1);
    }

    Node* temp = q->front;
    int val = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL; // coada devine goală
    free(temp);
    return val;
}

void printQueue(QueueSLL* q) {
    Node* curr = q->front;
    printf("Coada: ");
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    QueueSLL q;
    initQueue(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);

    printQueue(&q);  // Coada: 10 20 30

    printf("Dequeued: %d\n", dequeue(&q));  // 10
    printQueue(&q);  // Coada: 20 30

    return 0;
}


