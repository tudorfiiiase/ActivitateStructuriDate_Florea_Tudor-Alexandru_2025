#include <stdio.h>
#include <stdlib.h>
#define CRT_SECURE_NO_WARNINGS

typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

int height(Node* n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Node* minValueNode(Node* node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}
Node* deleteAVL(Node* root, int key) {
    if (!root) return root;

    if (key < root->key)
        root->left = deleteAVL(root->left, key);
    else if (key > root->key)
        root->right = deleteAVL(root->right, key);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {
            // 2 copii: inlocuim cu succesor
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteAVL(root->right, temp->key);
        }
    }

    if (!root) return NULL;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // 4 cazuri:
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* insertAVL(Node* node, int key) {
    if (!node) return newNode(key);
    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateRight(node);
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}
void inorder(Node* root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

int main() {
    Node* root = NULL;
    int values[] = { 20, 10, 30, 5, 15, 25, 35 };
    for (int i = 0; i < 7; i++) {
        root = insertAVL(root, values[i]);
    }

    printf("Inorder inainte de stergere: ");
    inorder(root);
    printf("\n");

    root = deleteAVL(root, 30);

    printf("Inorder dupa stergere 30: ");
    inorder(root);
    printf("\n");

    return 0;
}
