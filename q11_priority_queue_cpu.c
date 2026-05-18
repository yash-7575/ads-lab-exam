// CPU Scheduling using Priority Queue (Max Heap)
// Higher priority number = higher priority process
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

struct Process {
    int pid;         // process ID
    char name[20];   // process name
    int priority;    // higher = more important
    int burstTime;   // CPU time needed
};

struct PriorityQueue {
    struct Process heap[MAX];
    int size;
};

struct PriorityQueue pq;

// Swap two processes
void swap(struct Process* a, struct Process* b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

// Move a node up to maintain max-heap property
void heapifyUp(int i) {
    int parent = (i - 1) / 2;
    // Swap with parent if current has higher priority
    if (i > 0 && pq.heap[i].priority > pq.heap[parent].priority) {
        swap(&pq.heap[i], &pq.heap[parent]);
        heapifyUp(parent);
    }
}

// Move a node down to maintain max-heap property
void heapifyDown(int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < pq.size && pq.heap[left].priority > pq.heap[largest].priority)
        largest = left;
    if (right < pq.size && pq.heap[right].priority > pq.heap[largest].priority)
        largest = right;

    if (largest != i) {
        swap(&pq.heap[i], &pq.heap[largest]);
        heapifyDown(largest);
    }
}

// Add a process to the queue
void enqueue(int pid, char* name, int priority, int burstTime) {
    if (pq.size == MAX) {
        printf("Queue is full!\n");
        return;
    }
    pq.heap[pq.size].pid = pid;
    strcpy(pq.heap[pq.size].name, name);
    pq.heap[pq.size].priority = priority;
    pq.heap[pq.size].burstTime = burstTime;
    pq.size++;
    heapifyUp(pq.size - 1); // fix heap from bottom up
    printf("Added: %s (PID=%d, Priority=%d, Burst=%d)\n", name, pid, priority, burstTime);
}

// Remove and return highest priority process
struct Process dequeue() {
    struct Process top = pq.heap[0];
    pq.heap[0] = pq.heap[pq.size - 1]; // move last to top
    pq.size--;
    heapifyDown(0); // fix heap from top down
    return top;
}

// Display current queue
void display() {
    printf("\nCurrent queue (not sorted, heap structure):\n");
    printf("%-5s %-15s %-10s %-10s\n", "PID", "Name", "Priority", "Burst");
    for (int i = 0; i < pq.size; i++)
        printf("%-5d %-15s %-10d %-10d\n",
            pq.heap[i].pid, pq.heap[i].name,
            pq.heap[i].priority, pq.heap[i].burstTime);
}

int main() {
    pq.size = 0;

    printf("=== CPU Scheduling with Priority Queue ===\n\n");

    // Add processes
    printf("Adding processes:\n");
    enqueue(1, "Chrome",    3, 5);
    enqueue(2, "SystemOS",  9, 2);
    enqueue(3, "VSCode",    5, 8);
    enqueue(4, "Music",     1, 3);
    enqueue(5, "Compiler",  7, 6);

    display();

    // Simulate CPU scheduling
    printf("\n--- CPU Execution Order (highest priority first) ---\n");
    int time = 0;
    while (pq.size > 0) {
        struct Process p = dequeue();
        printf("Time %2d: Running %-15s (PID=%d, Priority=%d, Burst=%d) -> Done at %d\n",
               time, p.name, p.pid, p.priority, p.burstTime, time + p.burstTime);
        time += p.burstTime;
    }

    return 0;
}
