#include <stdio.h>
#include <stdlib.h>
#define CRT_SECURE_NO_WARNINGS
#define MAXN 1000

int n, m;
int mat[MAXN][MAXN];
int visited[MAXN][MAXN];

typedef struct {
    int x, y, dist;
} Node;

int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

// Coada simpla
Node queue[MAXN * MAXN];
int front = 0, rear = 0;

void enqueue(int x, int y, int dist) {
    queue[rear++] = (Node){ x, y, dist };
}

Node dequeue() {
    return queue[front++];
}

int isEmpty() {
    return front == rear;
}

int isValid(int x, int y) {
    return x >= 0 && x < n && y >= 0 && y < m;
}

int bfs() {
    if (mat[0][0] == 1 || mat[n - 1][m - 1] == 1)
        return -1;

    visited[0][0] = 1;
    enqueue(0, 0, 0);

    while (!isEmpty()) {
        Node curr = dequeue();

        if (curr.x == n - 1 && curr.y == m - 1)
            return curr.dist;

        for (int d = 0; d < 4; d++) {
            int nx = curr.x + dx[d];
            int ny = curr.y + dy[d];

            if (isValid(nx, ny) && !visited[nx][ny] && mat[nx][ny] == 0) {
                visited[nx][ny] = 1;
                enqueue(nx, ny, curr.dist + 1);
            }
        }
    }

    return -1; // drum imposibil
}

int main() {
    scanf_s("%d %d", &n, &m);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf_s("%d", &mat[i][j]);

    int result = bfs();
    printf("%d\n", result);

    return 0;
}

