// Right Threaded Binary Tree - Postorder Traversal
// Right NULL pointers point to inorder successor
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int rthread; // 1 = right is a thread, 0 = real link
};

struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->rthread = 1;
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
            curr = curr->left;
        } else {
            if (curr->rthread == 0)
                curr = curr->right;
            else
                break;
        }
    }

    if (data < par->data) {
        newn->right = par;
        newn->rthread = 1;
        par->left = newn;
    } else {
        newn->right = par->right;
        newn->rthread = par->rthread;
        par->right = newn;
        par->rthread = 0;
    }

    return root;
}

// Stack for postorder traversal
#define MAX 100
struct Node* stack[MAX];
int top = -1;

void push(struct Node* n) { stack[++top] = n; }
struct Node* pop() { return stack[top--]; }
int isEmpty() { return top == -1; }

// Postorder: Left -> Right -> Root
// Two-stack method: easy to understand
void postorder(struct Node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    struct Node* result[MAX]; // stores postorder nodes in reverse
    int resTop = -1;

    push(root);

    while (!isEmpty()) {
        struct Node* curr = pop();
        result[++resTop] = curr; // collect in reverse order

        // Push left first, then right (so right is processed first)
        if (curr->left != NULL)
            push(curr->left);
        if (curr->rthread == 0 && curr->right != NULL)
            push(curr->right);
    }

    // Print in reverse (postorder)
    printf("Postorder: ");
    for (int i = resTop; i >= 0; i--)
        printf("%d ", result[i]->data);
    printf("\n");
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

    printf("Right Threaded Binary Tree - Postorder Traversal:\n");
    postorder(root);
    printf("Expected: 5 15 10 25 35 30 20\n");

    return 0;
}
