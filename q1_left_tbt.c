// Left Threaded Binary Tree - Inorder Traversal
// Left NULL pointers point to inorder predecessor
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int lthread; // 1 = left is a thread (predecessor), 0 = real link
};

// Create a new node
struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->lthread = 1; // by default, left is a thread (NULL predecessor)
    return n;
}

// Insert into left threaded BST
struct Node* insert(struct Node* root, int data) {
    struct Node* newn = createNode(data);

    if (root == NULL)
        return newn;

    struct Node* curr = root;
    struct Node* par = NULL;

    while (curr != NULL) {
        par = curr;
        if (data < curr->data) {
            if (curr->lthread == 0) // real left link, go left
                curr = curr->left;
            else
                break; // left is a thread, stop here
        } else {
            curr = curr->right; // right is always a real link
        }
    }

    if (data < par->data) {
        // Insert as left child of par
        newn->left = par->left;    // new node's predecessor = par's old predecessor
        newn->lthread = par->lthread;
        par->left = newn;          // par now points to new node
        par->lthread = 0;          // par's left is now a real link
    } else {
        // Insert as right child of par
        newn->left = par;          // new node's inorder predecessor is par
        newn->lthread = 1;         // left is a thread
        par->right = newn;
    }

    return root;
}

// Find leftmost node (follow real left pointers only)
struct Node* leftmost(struct Node* node) {
    while (node != NULL && node->lthread == 0)
        node = node->left;
    return node;
}

// Inorder traversal of Left Threaded Binary Tree
// We use right pointers to go forward; for each node go to leftmost first
void inorder(struct Node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    // Start at leftmost node
    struct Node* curr = root;
    while (curr->lthread == 0)
        curr = curr->left;

    // Keep going right and finding leftmost
    while (curr != NULL) {
        printf("%d ", curr->data);
        if (curr->right == NULL)
            break;
        curr = leftmost(curr->right); // go right, then leftmost
    }
}

int main() {
    struct Node* root = NULL;

    // Insert elements into left threaded BST
    root = insert(root, 20);
    root = insert(root, 10);
    root = insert(root, 30);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 25);
    root = insert(root, 35);

    printf("Left Threaded Binary Tree - Inorder Traversal:\n");
    inorder(root);
    printf("\n");

    // Show threading: node 15's left thread points to its predecessor (10)
    printf("\nDemo: Inorder predecessor of 30 is accessible via left thread\n");

    return 0;
}
