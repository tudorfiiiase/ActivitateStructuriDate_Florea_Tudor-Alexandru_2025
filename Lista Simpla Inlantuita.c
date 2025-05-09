#include <stdio.h>
#include <stdlib.h>
#define CRT_SECURE_NO_WARNINGS
#define MAXN 100005

int n;
int next[MAXN];

int findCycleStart(int* next, int n) {
    int slow = next[1];
    int fast = next[next[1]];

    while (fast != 0 && next[fast] != 0) {
        if (slow == fast) {
            // Ciclu detectat
            slow = 1;
            while (slow != fast) {
                slow = next[slow];
                fast = next[fast];
            }
            return slow; // inceputul ciclului
        }
        slow = next[slow];
        fast = next[next[fast]];
    }

    return 0; // fara ciclu
}

int main() {
    int values[MAXN];
    scanf_s("%d", &n);

    for (int i = 1; i <= n; i++)
        scanf_s("%d", &values[i]); // valorile nu sunt folosite

    for (int i = 1; i <= n; i++)
        scanf_s("%d", &next[i]);

    int start = findCycleStart(next, n);
    if (start)
        printf("DA\n%d\n", start);
    else
        printf("NU\n");

    return 0;
}



