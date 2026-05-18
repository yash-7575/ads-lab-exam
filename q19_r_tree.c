// R-Tree for storing rectangles and performing spatial queries
// Simplified R-Tree: each node stores rectangles with MBR (Minimum Bounding Rectangle)
// Used in databases for spatial indexing (e.g., "find all places within area")
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define MAX_ENTRIES 4 // max rectangles per node

// A rectangle defined by bottom-left and top-right corners
struct Rect {
    float x1, y1; // bottom-left
    float x2, y2; // top-right
    char label[20];
    int used;     // 1 if this slot is occupied
};

// R-Tree node
struct RNode {
    struct Rect entries[MAX_ENTRIES]; // rectangles stored in this node
    int count;                         // number of entries
    struct Rect mbr;                   // Minimum Bounding Rectangle of all entries
};

#define MAX_NODES 50
struct RNode nodes[MAX_NODES];
int nodeCount = 0;

// Create a new R-Tree node
int createRNode() {
    int idx = nodeCount++;
    nodes[idx].count = 0;
    nodes[idx].mbr.x1 = FLT_MAX;
    nodes[idx].mbr.y1 = FLT_MAX;
    nodes[idx].mbr.x2 = -FLT_MAX;
    nodes[idx].mbr.y2 = -FLT_MAX;
    for (int i = 0; i < MAX_ENTRIES; i++)
        nodes[idx].entries[i].used = 0;
    return idx;
}

// Update MBR of a node to include a new rectangle
void updateMBR(int nodeIdx, struct Rect* r) {
    struct Rect* mbr = &nodes[nodeIdx].mbr;
    if (r->x1 < mbr->x1) mbr->x1 = r->x1;
    if (r->y1 < mbr->y1) mbr->y1 = r->y1;
    if (r->x2 > mbr->x2) mbr->x2 = r->x2;
    if (r->y2 > mbr->y2) mbr->y2 = r->y2;
}

// Insert a rectangle into a node
int insertRect(int nodeIdx, float x1, float y1, float x2, float y2, char* label) {
    if (nodes[nodeIdx].count >= MAX_ENTRIES) {
        printf("Node full! (overflow handling simplified)\n");
        return 0;
    }
    int i = nodes[nodeIdx].count;
    nodes[nodeIdx].entries[i].x1 = x1;
    nodes[nodeIdx].entries[i].y1 = y1;
    nodes[nodeIdx].entries[i].x2 = x2;
    nodes[nodeIdx].entries[i].y2 = y2;
    nodes[nodeIdx].entries[i].used = 1;
    sprintf(nodes[nodeIdx].entries[i].label, "%s", label);
    nodes[nodeIdx].count++;
    updateMBR(nodeIdx, &nodes[nodeIdx].entries[i]);
    return 1;
}

// Check if two rectangles overlap
int overlaps(struct Rect* a, struct Rect* b) {
    return !(a->x2 < b->x1 || b->x2 < a->x1 ||
             a->y2 < b->y1 || b->y2 < a->y1);
}

// Check if rectangle b is completely inside rectangle a
int contains(struct Rect* a, struct Rect* b) {
    return (b->x1 >= a->x1 && b->y1 >= a->y1 &&
            b->x2 <= a->x2 && b->y2 <= a->y2);
}

// Spatial Query: find all rectangles that overlap with search area
void spatialSearch(int nodeIdx, struct Rect* query, int* found) {
    // Check if query overlaps with this node's MBR
    if (!overlaps(query, &nodes[nodeIdx].mbr)) return;

    for (int i = 0; i < nodes[nodeIdx].count; i++) {
        if (nodes[nodeIdx].entries[i].used &&
            overlaps(query, &nodes[nodeIdx].entries[i])) {
            struct Rect* r = &nodes[nodeIdx].entries[i];
            printf("  Found: %-15s at (%.0f,%.0f)-(%.0f,%.0f)\n",
                   r->label, r->x1, r->y1, r->x2, r->y2);
            (*found)++;
        }
    }
}

// Display all rectangles in a node
void displayNode(int nodeIdx) {
    printf("Node MBR: (%.0f,%.0f)-(%.0f,%.0f)\n",
           nodes[nodeIdx].mbr.x1, nodes[nodeIdx].mbr.y1,
           nodes[nodeIdx].mbr.x2, nodes[nodeIdx].mbr.y2);
    printf("Stored rectangles (%d):\n", nodes[nodeIdx].count);
    for (int i = 0; i < nodes[nodeIdx].count; i++) {
        struct Rect* r = &nodes[nodeIdx].entries[i];
        printf("  [%d] %-15s (%.0f,%.0f) to (%.0f,%.0f)\n",
               i + 1, r->label, r->x1, r->y1, r->x2, r->y2);
    }
}

int main() {
    printf("=== R-Tree for Rectangle Storage and Spatial Query ===\n\n");

    int root = createRNode();

    // Insert rectangles (x1,y1 = bottom-left, x2,y2 = top-right)
    printf("Inserting rectangles:\n");
    insertRect(root,  0,  0, 10, 10, "Park");
    insertRect(root, 15,  5, 25, 20, "School");
    insertRect(root, 30, 30, 50, 50, "Hospital");
    insertRect(root,  5, 40, 20, 60, "Mall");

    printf("\nR-Tree structure:\n");
    displayNode(root);

    // Spatial queries
    printf("\n--- Spatial Query 1: Find rectangles in area (0,0)-(20,25) ---\n");
    struct Rect query1 = {0, 0, 20, 25};
    int found1 = 0;
    spatialSearch(root, &query1, &found1);
    printf("Total matches: %d\n", found1);

    printf("\n--- Spatial Query 2: Find rectangles in area (25,25)-(55,55) ---\n");
    struct Rect query2 = {25, 25, 55, 55};
    int found2 = 0;
    spatialSearch(root, &query2, &found2);
    printf("Total matches: %d\n", found2);

    printf("\n--- Spatial Query 3: Find rectangles overlapping (10,10)-(35,45) ---\n");
    struct Rect query3 = {10, 10, 35, 45};
    int found3 = 0;
    spatialSearch(root, &query3, &found3);
    printf("Total matches: %d\n", found3);

    printf("\nNote: In a full R-Tree, nodes overflow is handled by splitting.\n");
    printf("This implementation shows the core concept with a single node.\n");

    return 0;
}
