// B-Tree of order M - Insert + Search
#include <stdio.h>
#include <stdlib.h>

#define M 3

struct BNode {
    int keys[M - 1];
    struct BNode* child[M];
    int n;
    int leaf;
};

struct BNode* createBNode(int leaf) {
    struct BNode* node = (struct BNode*)malloc(sizeof(struct BNode));
    node->n = 0;
    node->leaf = leaf;
    for (int i = 0; i < M; i++) node->child[i] = NULL;
    return node;
}

void splitChild(struct BNode* par, int i, struct BNode* fullChild) {
    int mid = (M - 1) / 2;
    struct BNode* newNode = createBNode(fullChild->leaf);
    newNode->n = (M - 1) - mid - 1;

    for (int j = 0; j < newNode->n; j++)
        newNode->keys[j] = fullChild->keys[j + mid + 1];
    if (!fullChild->leaf) {
        for (int j = 0; j <= newNode->n; j++)
            newNode->child[j] = fullChild->child[j + mid + 1];
    }
    fullChild->n = mid;

    for (int j = par->n; j >= i + 1; j--)
        par->child[j + 1] = par->child[j];
    par->child[i + 1] = newNode;

    for (int j = par->n - 1; j >= i; j--)
        par->keys[j + 1] = par->keys[j];
    par->keys[i] = fullChild->keys[mid];
    par->n++;
}

void insertNonFull(struct BNode* node, int key) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) { node->keys[i + 1] = node->keys[i]; i--; }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->child[i]->n == M - 1) {
            splitChild(node, i, node->child[i]);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->child[i], key);
    }
}

struct BNode* root = NULL;

void insert(int key) {
    if (root == NULL) {
        root = createBNode(1);
        root->keys[0] = key;
        root->n = 1;
        return;
    }
    if (root->n == M - 1) {
        struct BNode* newRoot = createBNode(0);
        newRoot->child[0] = root;
        splitChild(newRoot, 0, root);
        root = newRoot;
    }
    insertNonFull(root, key);
}

// Search for a key in B-tree
// Returns the node containing key, or NULL if not found
struct BNode* search(struct BNode* node, int key) {
    if (node == NULL) return NULL;

    int i = 0;

    // Find first key >= search key
    while (i < node->n && key > node->keys[i])
        i++;

    // Found the key in this node
    if (i < node->n && key == node->keys[i]) {
        printf("Key %d found in node with keys: ", key);
        for (int j = 0; j < node->n; j++) printf("%d ", node->keys[j]);
        printf("\n");
        return node;
    }

    // Key not in this node and this is a leaf - not found
    if (node->leaf) return NULL;

    // Go into appropriate child
    return search(node->child[i], key);
}

void display(struct BNode* node, int level) {
    if (node == NULL) return;
    printf("Level %d: [ ", level);
    for (int i = 0; i < node->n; i++) printf("%d ", node->keys[i]);
    printf("]\n");
    for (int i = 0; i <= node->n; i++) display(node->child[i], level + 1);
}

int main() {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = 8;

    printf("Building B-Tree of Order %d with keys: ", M);
    for (int i = 0; i < n; i++) { printf("%d ", keys[i]); insert(keys[i]); }
    printf("\n\n");

    printf("B-Tree structure:\n");
    display(root, 0);
    printf("\n");

    // Search examples
    int searchKeys[] = {6, 15, 30};
    for (int i = 0; i < 3; i++) {
        printf("Searching for %d: ", searchKeys[i]);
        struct BNode* result = search(root, searchKeys[i]);
        if (result == NULL) printf("Key %d NOT found\n", searchKeys[i]);
    }

    return 0;
}
