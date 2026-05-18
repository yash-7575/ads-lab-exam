// Left Threaded Binary Tree - Preorder Traversal
// Left NULL pointers point to inorder predecessor
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int lthread; // 1 = left is a thread, 0 = real link
};

struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->lthread = 1;
    return n;
}

struct Node* insert(struct Node* root, int data) {
    struct Node* newn = createNode(data);
    if (root == NULL) return newn;

    struct Node* curr = root;
    struct Node* par = NULL;

    while (curr != NULL) {
        par = curr;
        if (data < curr->data) {
            if (curr->lthread == 0)
                curr = curr->left;
            else
                break;
        } else {
            curr = curr->right;
        }
    }

    if (data < par->data) {
        newn->left = par->left;
        newn->lthread = par->lthread;
        par->left = newn;
        par->lthread = 0;
    } else {
        newn->left = par;
        newn->lthread = 1;
        par->right = newn;
    }

    return root;
}

// Preorder: Root -> Left -> Right
// We use a stack to traverse
#define MAX 100
struct Node* stack[MAX];
int top = -1;

void push(struct Node* n) { stack[++top] = n; }
struct Node* pop() { return stack[top--]; }
int isEmpty() { return top == -1; }

void preorder(struct Node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    push(root);

    while (!isEmpty()) {
        struct Node* curr = pop();
        printf("%d ", curr->data); // visit root first

        // Push right first so left is processed first (LIFO)
        if (curr->right != NULL)
            push(curr->right);

        // Only push real left links (not threads)
        if (curr->lthread == 0 && curr->left != NULL)
            push(curr->left);
    }
}

int main() {
    struct Node* root = NULL;

    root = insert(root, 20);
    root = insert(root, 10);
    root = insert(root, 30);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);
    root = insert(root, 35);

    printf("Left Threaded Binary Tree - Preorder Traversal:\n");
    preorder(root);
    printf("\n");
    printf("Expected: 20 10 5 15 30 25 35\n");

    return 0;
}
