// Fully Threaded Binary Tree
// Left NULL -> inorder predecessor, Right NULL -> inorder successor
// Uses a HEADER node to handle edge cases
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
    int lthread; // 1 = thread, 0 = real link
    int rthread; // 1 = thread, 0 = real link
};

// Create a new node (both threads point to NULL by default)
struct Node* createNode(int data) {
    struct Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->lthread = 1; // no left child -> thread
    n->rthread = 1; // no right child -> thread
    return n;
}

// Global header node - helps handle first/last nodes in traversal
struct Node* header;

// Initialize the header node
void init() {
    header = createNode(-1);  // dummy value
    header->lthread = 0;      // real left link to root
    header->rthread = 1;      // right thread points to itself
    header->left = header;    // points to itself initially
    header->right = header;
}

// Insert into fully threaded BST
void insert(int data) {
    struct Node* newn = createNode(data);
    newn->left = header;  // default predecessor = header
    newn->right = header; // default successor = header

    // If tree is empty
    if (header->left == header) {
        header->left = newn;
        header->lthread = 0;
        return;
    }

    struct Node* curr = header->left; // root
    struct Node* par = NULL;

    // Find insertion position
    while (1) {
        par = curr;
        if (data < curr->data) {
            if (curr->lthread == 0) // real left link
                curr = curr->left;
            else
                break;
        } else {
            if (curr->rthread == 0) // real right link
                curr = curr->right;
            else
                break;
        }
    }

    if (data < par->data) {
        // Insert as left child of par
        newn->left = par->left;   // new node's predecessor = par's old predecessor
        newn->right = par;        // new node's successor = par
        par->left = newn;
        par->lthread = 0;         // par's left is now a real link
    } else {
        // Insert as right child of par
        newn->right = par->right; // new node's successor = par's old successor
        newn->left = par;         // new node's predecessor = par
        par->right = newn;
        par->rthread = 0;         // par's right is now a real link
    }
}

// Inorder traversal - no stack, no recursion!
void inorder() {
    struct Node* curr = header;

    // Go to leftmost node
    while (curr->lthread == 0)
        curr = curr->left;

    while (curr != header) { // stop when we come back to header
        printf("%d ", curr->data);

        if (curr->rthread == 1) {
            // Right is a thread: go to inorder successor directly
            curr = curr->right;
        } else {
            // Right is a real link: go right, then leftmost
            curr = curr->right;
            while (curr->lthread == 0)
                curr = curr->left;
        }
    }
    printf("\n");
}

int main() {
    init(); // setup header node

    insert(20);
    insert(10);
    insert(30);
    insert(5);
    insert(15);
    insert(25);
    insert(35);

    printf("Fully Threaded Binary Tree - Inorder Traversal:\n");
    inorder();
    printf("Expected: 5 10 15 20 25 30 35\n");

    return 0;
}
