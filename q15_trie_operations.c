// Trie - Insert, Search, and Delete operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

struct TrieNode {
    struct TrieNode* children[ALPHABET];
    int isEnd;
};

struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->isEnd = 0;
    for (int i = 0; i < ALPHABET; i++) node->children[i] = NULL;
    return node;
}

// Insert word
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (!curr->children[idx])
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->isEnd = 1;
}

// Search word - returns 1 if found, 0 if not
int search(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (!curr->children[idx]) return 0;
        curr = curr->children[idx];
    }
    return curr->isEnd;
}

// Check if a node has any children
int hasChildren(struct TrieNode* node) {
    for (int i = 0; i < ALPHABET; i++)
        if (node->children[i]) return 1;
    return 0;
}

// Delete a word from trie
// Returns 1 if current node should be deleted, 0 otherwise
int deleteWord(struct TrieNode* root, char* word, int depth) {
    if (root == NULL) return 0;

    // Base case: reached end of word
    if (word[depth] == '\0') {
        if (root->isEnd) root->isEnd = 0; // unmark end
        // Delete this node only if it has no children
        return !hasChildren(root);
    }

    int idx = word[depth] - 'a';
    if (deleteWord(root->children[idx], word, depth + 1)) {
        // Child can be deleted
        free(root->children[idx]);
        root->children[idx] = NULL;
        // Delete this node only if it's not end of another word and has no children
        return !root->isEnd && !hasChildren(root);
    }
    return 0;
}

// Display all stored words
void display(struct TrieNode* root, char* buf, int d) {
    if (root->isEnd) { buf[d] = '\0'; printf("  %s\n", buf); }
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i]) {
            buf[d] = 'a' + i;
            display(root->children[i], buf, d + 1);
        }
    }
}

int main() {
    struct TrieNode* root = createNode();

    printf("=== Trie Operations: Insert, Search, Delete ===\n\n");

    // INSERT
    printf("--- Inserting words ---\n");
    char* words[] = {"hello", "help", "world", "word", "he", "her"};
    for (int i = 0; i < 6; i++) {
        insert(root, words[i]);
        printf("Inserted: %s\n", words[i]);
    }

    char buf[100];
    printf("\nAll words in trie:\n");
    display(root, buf, 0);

    // SEARCH
    printf("\n--- Search ---\n");
    char* toSearch[] = {"he", "hello", "hell", "world", "words"};
    for (int i = 0; i < 5; i++)
        printf("Search '%s': %s\n", toSearch[i], search(root, toSearch[i]) ? "FOUND" : "NOT FOUND");

    // DELETE
    printf("\n--- Deleting 'help' ---\n");
    deleteWord(root, "help", 0);

    printf("\n--- Deleting 'he' ---\n");
    deleteWord(root, "he", 0);

    printf("\nWords after deletion:\n");
    display(root, buf, 0);

    printf("\nSearch 'help' after delete: %s\n", search(root, "help") ? "FOUND" : "NOT FOUND");
    printf("Search 'hello' after delete: %s\n", search(root, "hello") ? "FOUND" : "NOT FOUND");

    return 0;
}
