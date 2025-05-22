#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP 1000

typedef struct {
    int priority;
    int deadline;
} Task;

typedef struct {
    Task data[MAX_HEAP];
    int size;
} TaskHeap;
int compare(Task a, Task b) {
    if (a.priority != b.priority)
        return a.priority > b.priority; // prioritati mai mari vin primele
    return a.deadline < b.deadline;    // deadline-uri mai mici vin primele
}
void swap(Task* a, Task* b) {
    Task temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyDown(TaskHeap* h, int i) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;

    if (l < h->size && compare(h->data[l], h->data[largest]))
        largest = l;
    if (r < h->size && compare(h->data[r], h->data[largest]))
        largest = r;

    if (largest != i) {
        swap(&h->data[i], &h->data[largest]);
        heapifyDown(h, largest);
    }
}

void heapifyUp(TaskHeap* h, int i) {
    if (i == 0) return;
    int p = (i - 1) / 2;
    if (compare(h->data[i], h->data[p])) {
        swap(&h->data[i], &h->data[p]);
        heapifyUp(h, p);
    }
}

void insert(TaskHeap* h, Task t) {
    if (h->size >= MAX_HEAP) {
        printf("Heap overflow\n");
        return;
    }
    h->data[h->size] = t;
    heapifyUp(h, h->size);
    h->size++;
}

Task extractMax(TaskHeap* h) {
    if (h->size == 0) {
        printf("Heap gol\n");
        exit(1);
    }
    Task top = h->data[0];
    h->data[0] = h->data[--h->size];
    heapifyDown(h, 0);
    return top;
}

int main() {
    TaskHeap heap;
    heap.size = 0;

    insert(&heap, (Task) { 5, 10 });
    insert(&heap, (Task) { 3, 5 });
    insert(&heap, (Task) { 5, 8 });
    insert(&heap, (Task) { 4, 12 });
    insert(&heap, (Task) { 5, 15 });

    printf("Task-urile extrase în ordinea priorității:\n");
    while (heap.size > 0) {
        Task t = extractMax(&heap);
        printf("Priority: %d, Deadline: %d\n", t.priority, t.deadline);
    }

    return 0;
}
