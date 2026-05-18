// AVL Tree - Preorder and Postorder Traversals
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int height;
};

int height(struct Node* n) { return (n == NULL) ? 0 : n->height; }
int max(int a, int b) { return (a > b) ? a : b; }

struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

void updateHeight(struct Node* n) {
    if (n) n->height = 1 + max(height(n->left), height(n->right));
}

int getBalance(struct Node* n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}

struct Node* rotateRight(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

struct Node* rotateLeft(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

struct Node* insert(struct Node* root, int data) {
    if (root == NULL) return createNode(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);
    else
        return root;

    updateHeight(root);
    int balance = getBalance(root);

    if (balance > 1 && data < root->left->data)   return rotateRight(root);
    if (balance < -1 && data > root->right->data)  return rotateLeft(root);
    if (balance > 1 && data > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && data < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

// Inorder: Left -> Root -> Right (sorted)
void inorder(struct Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

// Preorder: Root -> Left -> Right
void preorder(struct Node* root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preorder(root->left);
    preorder(root->right);
}

// Postorder: Left -> Right -> Root
void postorder(struct Node* root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->data);
}

// Display tree structure (sideways)
void display(struct Node* root, int space) {
    if (root == NULL) return;
    space += 4;
    display(root->right, space);
    printf("\n%*s%d\n", space, "", root->data);
    display(root->left, space);
}

int main() {
    struct Node* root = NULL;

    int elements[] = {50, 30, 70, 20, 40, 60, 80, 10};
    int n = 8;

    printf("Inserting elements: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", elements[i]);
        root = insert(root, elements[i]);
    }
    printf("\n");

    printf("\nTree structure (rotated 90 degrees):\n");
    display(root, 0);

    printf("\nInorder   (sorted): "); inorder(root);   printf("\n");
    printf("Preorder  (root first): "); preorder(root);  printf("\n");
    printf("Postorder (root last):  "); postorder(root); printf("\n");

    return 0;
}
