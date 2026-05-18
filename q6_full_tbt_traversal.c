// Fully Threaded Binary Tree - Inorder, Preorder, Postorder Traversals
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int lthread; // 1 = thread to predecessor, 0 = real left link
    int rthread; // 1 = thread to successor, 0 = real right link
};

struct Node* header; // dummy header node

struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = n->right = NULL;
    n->lthread = n->rthread = 1;
    return n;
}

void init() {
    header = createNode(-1);
    header->lthread = 0;
    header->rthread = 1;
    header->left = header;
    header->right = header;
}

void insert(int data) {
    struct Node* newn = createNode(data);
    newn->left = header;
    newn->right = header;

    if (header->left == header) {
        header->left = newn;
        header->lthread = 0;
        return;
    }

    struct Node* curr = header->left;
    struct Node* par = NULL;

    while (1) {
        par = curr;
        if (data < curr->data) {
            if (curr->lthread == 0) curr = curr->left;
            else break;
        } else {
            if (curr->rthread == 0) curr = curr->right;
            else break;
        }
    }

    if (data < par->data) {
        newn->left = par->left;
        newn->right = par;
        par->left = newn;
        par->lthread = 0;
    } else {
        newn->right = par->right;
        newn->left = par;
        par->right = newn;
        par->rthread = 0;
    }
}

// ---- INORDER: Left -> Root -> Right ----
// Using right threads to avoid stack
void inorder() {
    printf("Inorder:   ");
    struct Node* curr = header;
    while (curr->lthread == 0) curr = curr->left; // go to leftmost

    while (curr != header) {
        printf("%d ", curr->data);
        if (curr->rthread == 1)
            curr = curr->right; // follow thread to successor
        else {
            curr = curr->right;
            while (curr->lthread == 0) curr = curr->left;
        }
    }
    printf("\n");
}

// ---- PREORDER: Root -> Left -> Right ----
// Using stack (threads help skip NULL checks)
#define MAX 100
struct Node* stk[MAX];
int top = -1;
void push(struct Node* n) { stk[++top] = n; }
struct Node* pop() { return stk[top--]; }
int isEmpty() { return top == -1; }

void preorder() {
    printf("Preorder:  ");
    if (header->left == header) { printf("\n"); return; }

    push(header->left); // push root

    while (!isEmpty()) {
        struct Node* curr = pop();
        if (curr == header) continue;
        printf("%d ", curr->data);

        // Push right then left (so left is processed first)
        if (curr->rthread == 0) push(curr->right);
        if (curr->lthread == 0) push(curr->left);
    }
    printf("\n");
}

// ---- POSTORDER: Left -> Right -> Root ----
// Two-stack method
void postorder() {
    printf("Postorder: ");
    if (header->left == header) { printf("\n"); return; }

    struct Node* result[MAX];
    int resTop = -1;

    push(header->left);
    while (!isEmpty()) {
        struct Node* curr = pop();
        if (curr == header) continue;
        result[++resTop] = curr;
        if (curr->lthread == 0) push(curr->left);
        if (curr->rthread == 0) push(curr->right);
    }

    for (int i = resTop; i >= 0; i--)
        printf("%d ", result[i]->data);
    printf("\n");
}

int main() {
    init();

    insert(20);
    insert(10);
    insert(30);
    insert(5);
    insert(15);
    insert(25);
    insert(35);

    printf("Fully Threaded Binary Tree - All Traversals:\n");
    inorder();   // 5 10 15 20 25 30 35
    preorder();  // 20 10 5 15 30 25 35
    postorder(); // 5 15 10 25 35 30 20

    return 0;
}
