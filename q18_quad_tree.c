// Quad Tree (2D)
// Divides 2D space into 4 quadrants: NW, NE, SW, SE
// Used for spatial data (maps, collision detection, image compression)
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 4 // max points before splitting a region

// A 2D point
struct Point {
    float x, y;
    char label[10];
};

// A rectangular boundary (center + half-width/height)
struct Boundary {
    float cx, cy;  // center
    float hw, hh;  // half-width, half-height
};

// Quad tree node
struct QNode {
    struct Boundary boundary;
    struct Point points[MAX_POINTS]; // points in this node
    int count;                        // current number of points
    int divided;                      // 1 if this node has been split
    struct QNode* NW; // North-West
    struct QNode* NE; // North-East
    struct QNode* SW; // South-West
    struct QNode* SE; // South-East
};

// Check if a point is inside a boundary
int contains(struct Boundary* b, float x, float y) {
    return (x >= b->cx - b->hw && x <= b->cx + b->hw &&
            y >= b->cy - b->hh && y <= b->cy + b->hh);
}

// Create a new quad tree node
struct QNode* createQNode(float cx, float cy, float hw, float hh) {
    struct QNode* node = (struct QNode*)malloc(sizeof(struct QNode));
    node->boundary.cx = cx;
    node->boundary.cy = cy;
    node->boundary.hw = hw;
    node->boundary.hh = hh;
    node->count = 0;
    node->divided = 0;
    node->NW = node->NE = node->SW = node->SE = NULL;
    return node;
}

// Subdivide a node into 4 children
void subdivide(struct QNode* node) {
    float cx = node->boundary.cx;
    float cy = node->boundary.cy;
    float hw = node->boundary.hw / 2;
    float hh = node->boundary.hh / 2;

    node->NW = createQNode(cx - hw, cy + hh, hw, hh); // top-left
    node->NE = createQNode(cx + hw, cy + hh, hw, hh); // top-right
    node->SW = createQNode(cx - hw, cy - hh, hw, hh); // bottom-left
    node->SE = createQNode(cx + hw, cy - hh, hw, hh); // bottom-right
    node->divided = 1;
}

// Insert a point into the quad tree
int insert(struct QNode* node, float x, float y, char* label) {
    // Point is outside this region
    if (!contains(&node->boundary, x, y))
        return 0;

    // If space available and not divided, store here
    if (node->count < MAX_POINTS && !node->divided) {
        node->points[node->count].x = x;
        node->points[node->count].y = y;
        sprintf(node->points[node->count].label, "%s", label);
        node->count++;
        return 1;
    }

    // Need to split if not already done
    if (!node->divided)
        subdivide(node);

    // Try inserting in children
    if (insert(node->NW, x, y, label)) return 1;
    if (insert(node->NE, x, y, label)) return 1;
    if (insert(node->SW, x, y, label)) return 1;
    if (insert(node->SE, x, y, label)) return 1;

    return 0; // shouldn't reach here
}

// Search: find all points within a search area
void search(struct QNode* node, struct Boundary* area, int* found) {
    if (node == NULL) return;

    // Check if search area overlaps with this node's boundary
    if (area->cx - area->hw > node->boundary.cx + node->boundary.hw ||
        area->cx + area->hw < node->boundary.cx - node->boundary.hw ||
        area->cy - area->hh > node->boundary.cy + node->boundary.hh ||
        area->cy + area->hh < node->boundary.cy - node->boundary.hh)
        return; // no overlap

    // Check each point in this node
    for (int i = 0; i < node->count; i++) {
        if (contains(area, node->points[i].x, node->points[i].y)) {
            printf("  Found: %s at (%.1f, %.1f)\n",
                   node->points[i].label, node->points[i].x, node->points[i].y);
            (*found)++;
        }
    }

    // Search children
    if (node->divided) {
        search(node->NW, area, found);
        search(node->NE, area, found);
        search(node->SW, area, found);
        search(node->SE, area, found);
    }
}

// Print tree structure
void printTree(struct QNode* node, int level) {
    if (node == NULL) return;
    printf("%*sRegion: center=(%.1f,%.1f) size=%.1fx%.1f, points=%d\n",
           level * 2, "", node->boundary.cx, node->boundary.cy,
           node->boundary.hw * 2, node->boundary.hh * 2, node->count);
    for (int i = 0; i < node->count; i++)
        printf("%*s  Point: %s (%.1f, %.1f)\n", level * 2, "",
               node->points[i].label, node->points[i].x, node->points[i].y);
    if (node->divided) {
        printTree(node->NW, level + 1);
        printTree(node->NE, level + 1);
        printTree(node->SW, level + 1);
        printTree(node->SE, level + 1);
    }
}

int main() {
    printf("=== Quad Tree (2D Spatial Tree) ===\n\n");

    // Create quad tree covering (0,0) to (100,100)
    // Center = (50,50), half-width = 50, half-height = 50
    struct QNode* qt = createQNode(50, 50, 50, 50);

    // Insert some 2D points (like locations on a map)
    struct { float x, y; char label[10]; } pts[] = {
        {10, 80, "Park"},
        {70, 90, "School"},
        {30, 40, "Hospital"},
        {80, 20, "Market"},
        {50, 50, "Center"},
        {15, 15, "Station"},
        {90, 70, "Mall"}
    };

    printf("Inserting locations:\n");
    for (int i = 0; i < 7; i++) {
        insert(qt, pts[i].x, pts[i].y, pts[i].label);
        printf("  Inserted %s at (%.0f, %.0f)\n", pts[i].label, pts[i].x, pts[i].y);
    }

    printf("\nQuad Tree Structure:\n");
    printTree(qt, 0);

    // Search in a specific area: center (20,20) with radius ~20
    printf("\nSearching in area (0,0) to (40,40):\n");
    struct Boundary searchArea = {20, 20, 20, 20}; // center(20,20) half=20
    int found = 0;
    search(qt, &searchArea, &found);
    printf("Total found: %d\n", found);

    return 0;
}
