#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int count; // numarul total de noduri în subarborele curent
    struct Node* left;
    struct Node* right;
} Node;
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->count = 1; // initial un singur nod
    node->left = node->right = NULL;
    return node;
}
Node* insert(Node* root, int key) {
    if (!root)
        return newNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);

    // Actualizam count
    int leftCount = root->left ? root->left->count : 0;
    int rightCount = root->right ? root->right->count : 0;
    root->count = 1 + leftCount + rightCount;

    return root;
}
int findKthSmallest(Node* root, int k) {
    if (!root) return -1; // nu exist

    int leftCount = root->left ? root->left->count : 0;

    if (k == leftCount + 1)
        return root->key;
    else if (k <= leftCount)
        return findKthSmallest(root->left, k);
    else
        return findKthSmallest(root->right, k - leftCount - 1);
}
int main() {
    Node* root = NULL;
    int values[] = { 20, 10, 30, 5, 15, 25, 35 };

    for (int i = 0; i < 7; i++) {
        root = insert(root, values[i]);
    }

    int k = 3;
    int result = findKthSmallest(root, k);

    if (result != -1)
        printf("A %d-a cea mai mică cheie este: %d\n", k, result);
    else
        printf("Nu există %d elemente în arbore.\n", k);

    return 0;
}
