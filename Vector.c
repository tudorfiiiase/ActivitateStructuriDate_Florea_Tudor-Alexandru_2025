#include <stdio.h>
#define CRT_SECURE_NO_WARNINGS
#define MAXN 1000000

int A[MAXN];

int main() {
    int n, k;
    scanf_s("%d %d", &n, &k);

    for (int i = 0; i < n; i++) {
        scanf_s("%d", &A[i]);
    }

    long long sum = 0, maxSum;

    // Suma initiala pe prima fereastra
    for (int i = 0; i < k; i++)
        sum += A[i];
    maxSum = sum;

    // Glisam fereastra de la stanga la dreapta
    for (int i = k; i < n; i++) {
        sum = sum - A[i - k] + A[i];
        if (sum > maxSum)
            maxSum = sum;
    }

    printf("%lld\n", maxSum);
    return 0;
}

