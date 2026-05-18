// Trie (Prefix Tree) - Store and display a set of words
// Each node represents one character
// Path from root to marked node = one word
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26 // only lowercase a-z

struct TrieNode {
    struct TrieNode* children[ALPHABET]; // one child per letter
    int isEnd; // 1 if this node marks end of a word
};

// Create a new trie node
struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->isEnd = 0;
    for (int i = 0; i < ALPHABET; i++)
        node->children[i] = NULL;
    return node;
}

// Insert a word into the trie
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a'; // convert char to 0-25
        if (curr->children[index] == NULL)
            curr->children[index] = createNode(); // create node if missing
        curr = curr->children[index];
    }
    curr->isEnd = 1; // mark end of word
    printf("Inserted: %s\n", word);
}

// Search for a word in the trie
int search(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL)
            return 0; // character path doesn't exist
        curr = curr->children[index];
    }
    return curr->isEnd; // word exists only if this is a marked end
}

// Display all words in the trie (DFS)
void displayAll(struct TrieNode* root, char* buffer, int depth) {
    if (root->isEnd) {
        buffer[depth] = '\0';
        printf("  %s\n", buffer);
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i] != NULL) {
            buffer[depth] = 'a' + i; // put character in buffer
            displayAll(root->children[i], buffer, depth + 1);
        }
    }
}

int main() {
    struct TrieNode* root = createNode();

    char* words[] = {"apple", "app", "apply", "apt", "bat", "ball", "band"};
    int n = 7;

    printf("=== Trie - Storing Words ===\n\n");
    printf("Inserting words:\n");
    for (int i = 0; i < n; i++)
        insert(root, words[i]);

    printf("\nAll words stored in Trie:\n");
    char buffer[100];
    displayAll(root, buffer, 0);

    printf("\nSearch tests:\n");
    char* searchWords[] = {"app", "apple", "ap", "bat", "cat"};
    for (int i = 0; i < 5; i++)
        printf("  Search '%s': %s\n", searchWords[i], search(root, searchWords[i]) ? "FOUND" : "NOT FOUND");

    return 0;
}
