// AVL Tree - Insert with rotations + Inorder Traversal
// AVL Tree: balanced BST where |height(left) - height(right)| <= 1
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int height; // height of this node
};

// Get height of a node (0 if NULL)
int height(struct Node* n) {
    return (n == NULL) ? 0 : n->height;
}

// Get max of two numbers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Create a new node
struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = n->right = NULL;
    n->height = 1; // new node has height 1
    return n;
}

// Update height of a node
void updateHeight(struct Node* n) {
    if (n != NULL)
        n->height = 1 + max(height(n->left), height(n->right));
}

// Get balance factor: positive = left heavy, negative = right heavy
int getBalance(struct Node* n) {
    return (n == NULL) ? 0 : height(n->left) - height(n->right);
}

// Right Rotation (for left-left case)
//      y              x
//     / \            / \
//    x   T3  -->   T1   y
//   / \                / \
//  T1  T2            T2  T3
struct Node* rotateRight(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y; // rotation
    y->left = T2;

    updateHeight(y); // update y first (it's now lower)
    updateHeight(x);

    return x; // new root
}

// Left Rotation (for right-right case)
//    x                y
//   / \              / \
//  T1   y    -->    x   T3
//      / \         / \
//     T2  T3      T1  T2
struct Node* rotateLeft(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x; // rotation
    x->right = T2;

    updateHeight(x); // update x first
    updateHeight(y);

    return y; // new root
}

// Insert into AVL tree and rebalance
struct Node* insert(struct Node* root, int data) {
    // Step 1: Normal BST insert
    if (root == NULL)
        return createNode(data);

    if (data < root->data)
        root->left = insert(root->left, data);
    else if (data > root->data)
        root->right = insert(root->right, data);
    else
        return root; // duplicate, ignore

    // Step 2: Update height
    updateHeight(root);

    // Step 3: Check balance and rotate if needed
    int balance = getBalance(root);

    // Case 1: Left-Left (right rotation)
    if (balance > 1 && data < root->left->data)
        return rotateRight(root);

    // Case 2: Right-Right (left rotation)
    if (balance < -1 && data > root->right->data)
        return rotateLeft(root);

    // Case 3: Left-Right (left rotate left child, then right rotate)
    if (balance > 1 && data > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Case 4: Right-Left (right rotate right child, then left rotate)
    if (balance < -1 && data < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root; // no rotation needed
}

// Inorder traversal (gives sorted output for BST)
void inorder(struct Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d (h=%d) ", root->data, root->height);
    inorder(root->right);
}

int main() {
    struct Node* root = NULL;

    // Insert elements - AVL will auto-balance
    int elements[] = {30, 20, 10, 40, 50, 25};
    int n = 6;

    printf("Inserting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", elements[i]);
        root = insert(root, elements[i]);
    }
    printf("\n");

    printf("\nAVL Tree - Inorder Traversal (sorted):\n");
    inorder(root);
    printf("\n");

    printf("\nRoot: %d (balance = %d)\n", root->data, getBalance(root));

    return 0;
}
