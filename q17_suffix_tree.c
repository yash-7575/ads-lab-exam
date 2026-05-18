// Suffix Tree for Pattern Matching
// Simple implementation: build all suffixes and store in a trie-like structure
// Then search for pattern by matching from root
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 256 // support all ASCII characters

struct SuffixNode {
    struct SuffixNode* children[MAXCHAR];
    int suffixIndex; // -1 if internal node, index if leaf
};

struct SuffixNode* createNode() {
    struct SuffixNode* node = (struct SuffixNode*)malloc(sizeof(struct SuffixNode));
    node->suffixIndex = -1;
    for (int i = 0; i < MAXCHAR; i++)
        node->children[i] = NULL;
    return node;
}

struct SuffixNode* root;

// Build suffix tree by inserting all suffixes
void buildSuffixTree(char* text) {
    int n = strlen(text);
    root = createNode();

    for (int i = 0; i < n; i++) {
        // Insert suffix starting at position i
        struct SuffixNode* curr = root;
        for (int j = i; j < n; j++) {
            int c = (unsigned char)text[j];
            if (curr->children[c] == NULL)
                curr->children[c] = createNode();
            curr = curr->children[c];
        }
        curr->suffixIndex = i; // mark which suffix ends here
    }
}

// Search for pattern - collect all positions where it occurs
int positions[100]; // store match positions
int posCount;

// After matching pattern, collect all suffix indices in subtree
void collectSuffixes(struct SuffixNode* node) {
    if (node == NULL) return;
    if (node->suffixIndex != -1)
        positions[posCount++] = node->suffixIndex; // found a match position
    for (int i = 0; i < MAXCHAR; i++)
        collectSuffixes(node->children[i]);
}

// Search for pattern in suffix tree
// Returns 1 if found, 0 if not
int search(char* pattern) {
    struct SuffixNode* curr = root;
    for (int i = 0; pattern[i]; i++) {
        int c = (unsigned char)pattern[i];
        if (curr->children[c] == NULL)
            return 0; // pattern not found
        curr = curr->children[c];
    }
    // Pattern found - now collect all suffix positions
    posCount = 0;
    collectSuffixes(curr);
    return 1;
}

// Print all suffixes (for understanding)
void printSuffixes(char* text) {
    int n = strlen(text);
    printf("All suffixes of '%s':\n", text);
    for (int i = 0; i < n; i++)
        printf("  [%d] %s\n", i, text + i);
}

int main() {
    char text[] = "banana";

    printf("=== Suffix Tree for Pattern Matching ===\n\n");
    printf("Text: '%s'\n\n", text);

    printSuffixes(text);

    buildSuffixTree(text);
    printf("\nSuffix tree built successfully.\n\n");

    // Search for patterns
    char* patterns[] = {"ana", "nan", "ban", "xyz", "a", "na"};
    int numPatterns = 6;

    printf("Pattern matching results:\n");
    for (int i = 0; i < numPatterns; i++) {
        printf("Pattern '%s': ", patterns[i]);
        if (search(patterns[i])) {
            printf("FOUND at positions: ");
            // Sort positions for clean output
            for (int p = 0; p < posCount; p++)
                printf("%d ", positions[p]);
            printf("\n");
        } else {
            printf("NOT FOUND\n");
        }
    }

    return 0;
}
