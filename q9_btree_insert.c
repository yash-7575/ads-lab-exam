// B-Tree of order M - Insert keys and display
// B-Tree properties:
// - Every node has at most M-1 keys and M children
// - Every node (except root) has at least ceil(M/2)-1 keys
// - All leaves are at same level
#include <stdio.h>
#include <stdlib.h>

#define M 3 // Order of B-Tree (3 means max 2 keys, 3 children per node)

struct BNode {
    int keys[M - 1];       // max M-1 keys
    struct BNode* child[M]; // max M children
    int n;                  // current number of keys
    int leaf;               // 1 if leaf node, 0 otherwise
};

// Create a new B-tree node
struct BNode* createBNode(int leaf) {
    struct BNode* node = (struct BNode*)malloc(sizeof(struct BNode));
    node->n = 0;
    node->leaf = leaf;
    for (int i = 0; i < M; i++)
        node->child[i] = NULL;
    return node;
}

// Split a full child node
// par = parent node, i = index of full child
void splitChild(struct BNode* par, int i, struct BNode* fullChild) {
    int mid = (M - 1) / 2; // middle key index

    // Create new node for right half
    struct BNode* newNode = createBNode(fullChild->leaf);
    newNode->n = (M - 1) - mid - 1;

    // Copy right half of fullChild to newNode
    for (int j = 0; j < newNode->n; j++)
        newNode->keys[j] = fullChild->keys[j + mid + 1];

    // Copy children too if not leaf
    if (!fullChild->leaf) {
        for (int j = 0; j <= newNode->n; j++)
            newNode->child[j] = fullChild->child[j + mid + 1];
    }

    fullChild->n = mid; // reduce size of fullChild

    // Shift parent's children to make room for newNode
    for (int j = par->n; j >= i + 1; j--)
        par->child[j + 1] = par->child[j];
    par->child[i + 1] = newNode;

    // Move middle key up to parent
    for (int j = par->n - 1; j >= i; j--)
        par->keys[j + 1] = par->keys[j];
    par->keys[i] = fullChild->keys[mid];
    par->n++;
}

// Insert key into a non-full node
void insertNonFull(struct BNode* node, int key) {
    int i = node->n - 1;

    if (node->leaf) {
        // Shift keys to make room
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        // Find correct child
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;

        // If child is full, split it first
        if (node->child[i]->n == M - 1) {
            splitChild(node, i, node->child[i]);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->child[i], key);
    }
}

struct BNode* root = NULL;

// Insert key into B-tree
void insert(int key) {
    if (root == NULL) {
        root = createBNode(1); // first node is a leaf
        root->keys[0] = key;
        root->n = 1;
        return;
    }

    if (root->n == M - 1) {
        // Root is full, create new root and split
        struct BNode* newRoot = createBNode(0);
        newRoot->child[0] = root;
        splitChild(newRoot, 0, root);
        root = newRoot;
    }

    insertNonFull(root, key);
}

// Display B-tree level by level
void display(struct BNode* node, int level) {
    if (node == NULL) return;

    printf("Level %d: [ ", level);
    for (int i = 0; i < node->n; i++)
        printf("%d ", node->keys[i]);
    printf("]\n");

    for (int i = 0; i <= node->n; i++)
        display(node->child[i], level + 1);
}

// Inorder traversal of B-tree (gives sorted output)
void inorder(struct BNode* node) {
    if (node == NULL) return;
    for (int i = 0; i < node->n; i++) {
        inorder(node->child[i]);
        printf("%d ", node->keys[i]);
    }
    inorder(node->child[node->n]);
}

int main() {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = 8;

    printf("B-Tree of Order %d\n", M);
    printf("Inserting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
        insert(keys[i]);
    }
    printf("\n\n");

    printf("B-Tree structure (level by level):\n");
    display(root, 0);

    printf("\nInorder traversal (sorted): ");
    inorder(root);
    printf("\n");

    return 0;
}
