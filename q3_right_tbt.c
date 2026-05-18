// Right Threaded Binary Tree - Inorder Traversal
// Right NULL pointers point to inorder successor
// Advantage: inorder traversal WITHOUT stack or recursion
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int rthread; // 1 = right is a thread (successor), 0 = real link
};

struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->rthread = 1; // by default right is a thread (NULL)
    return n;
}

// Insert into right threaded BST
struct Node* insert(struct Node* root, int data) {
    struct Node* newn = createNode(data);
    if (root == NULL) return newn;

    struct Node* curr = root;
    struct Node* par = NULL;

    while (curr != NULL) {
        par = curr;
        if (data < curr->data) {
            curr = curr->left; // left is always a real link (or NULL)
        } else {
            if (curr->rthread == 0) // real right link
                curr = curr->right;
            else
                break; // right is a thread, stop
        }
    }

    if (data < par->data) {
        // Insert as left child
        newn->right = par;     // new node's inorder successor is par
        newn->rthread = 1;     // right is a thread
        par->left = newn;
    } else {
        // Insert as right child
        newn->right = par->right; // inherit par's right thread
        newn->rthread = par->rthread;
        par->right = newn;        // par points to new node
        par->rthread = 0;         // par's right is now a real link
    }

    return root;
}

// Inorder traversal of Right Threaded Binary Tree - NO STACK NEEDED!
// Algorithm:
// 1. Go to leftmost node
// 2. Visit it
// 3. If rthread == 1: right is the inorder successor, follow it
// 4. If rthread == 0: right is a real link, go right and find leftmost again
void inorder(struct Node* root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    // Go to leftmost node
    struct Node* curr = root;
    while (curr->left != NULL)
        curr = curr->left;

    while (curr != NULL) {
        printf("%d ", curr->data); // visit current node

        if (curr->rthread == 1) {
            // Right is a thread - directly go to inorder successor
            curr = curr->right;
        } else {
            // Right is a real link - go right and find leftmost
            curr = curr->right;
            while (curr != NULL && curr->left != NULL)
                curr = curr->left;
        }
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

    printf("Right Threaded Binary Tree - Inorder Traversal (no stack):\n");
    inorder(root);
    printf("\n");
    printf("Expected: 5 10 15 20 25 30 35\n");

    return 0;
}
