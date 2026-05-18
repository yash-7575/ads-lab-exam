// Trie with Word Frequency
// Each word's end node stores how many times it was inserted
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

struct TrieNode {
    struct TrieNode* children[ALPHABET];
    int frequency; // how many times this word was inserted (0 = not a complete word)
};

struct TrieNode* createNode() {
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    node->frequency = 0;
    for (int i = 0; i < ALPHABET; i++) node->children[i] = NULL;
    return node;
}

// Insert word and increment its frequency
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (!curr->children[idx])
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->frequency++; // increment count each time word is inserted
}

// Get frequency of a word (0 if not present)
int getFrequency(struct TrieNode* root, char* word) {
    struct TrieNode* curr = root;
    for (int i = 0; word[i]; i++) {
        int idx = word[i] - 'a';
        if (!curr->children[idx]) return 0; // word not found
        curr = curr->children[idx];
    }
    return curr->frequency;
}

// Display all words with their frequency (DFS)
void displayWithFreq(struct TrieNode* root, char* buf, int depth) {
    if (root->frequency > 0) {
        buf[depth] = '\0';
        printf("  %-15s frequency: %d\n", buf, root->frequency);
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i]) {
            buf[depth] = 'a' + i;
            displayWithFreq(root->children[i], buf, depth + 1);
        }
    }
}

// Find top N frequent words (simple linear scan after collecting all)
#define MAX_WORDS 100
char allWords[MAX_WORDS][50];
int allFreqs[MAX_WORDS];
int wordCount = 0;

void collectWords(struct TrieNode* root, char* buf, int depth) {
    if (root->frequency > 0) {
        buf[depth] = '\0';
        strcpy(allWords[wordCount], buf);
        allFreqs[wordCount] = root->frequency;
        wordCount++;
    }
    for (int i = 0; i < ALPHABET; i++) {
        if (root->children[i]) {
            buf[depth] = 'a' + i;
            collectWords(root->children[i], buf, depth + 1);
        }
    }
}

// Find and print most frequent word
void topFrequent(struct TrieNode* root) {
    wordCount = 0;
    char buf[100];
    collectWords(root, buf, 0);

    int maxFreq = 0, maxIdx = 0;
    for (int i = 0; i < wordCount; i++) {
        if (allFreqs[i] > maxFreq) {
            maxFreq = allFreqs[i];
            maxIdx = i;
        }
    }
    printf("Most frequent word: '%s' (count = %d)\n", allWords[maxIdx], maxFreq);
}

int main() {
    struct TrieNode* root = createNode();

    printf("=== Trie with Word Frequency ===\n\n");

    // Simulate inserting words (like processing a text)
    char* text[] = {
        "apple", "banana", "apple", "cherry", "apple",
        "banana", "date", "cherry", "banana", "apple",
        "date", "apple", "cherry"
    };
    int n = 13;

    printf("Inserting words from text:\n");
    for (int i = 0; i < n; i++) {
        insert(root, text[i]);
        printf("  %s\n", text[i]);
    }

    printf("\nWord frequencies:\n");
    char buf[100];
    displayWithFreq(root, buf, 0);

    printf("\nSpecific word lookups:\n");
    char* check[] = {"apple", "banana", "cherry", "date", "fig"};
    for (int i = 0; i < 5; i++)
        printf("  '%s': %d time(s)\n", check[i], getFrequency(root, check[i]));

    printf("\n");
    topFrequent(root);

    return 0;
}
