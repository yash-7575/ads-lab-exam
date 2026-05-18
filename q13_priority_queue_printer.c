// Printer Queue Management using Priority Queue
// Higher priority jobs print first (e.g., admin > user > guest)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

struct PrintJob {
    int jobId;
    char owner[20];
    char filename[30];
    int priority;   // 1=Low, 5=Medium, 10=High
    int pages;
};

struct PrintJob heap[MAX];
int size = 0;

void swap(struct PrintJob* a, struct PrintJob* b) {
    struct PrintJob t = *a; *a = *b; *b = t;
}

void heapifyUp(int i) {
    int par = (i - 1) / 2;
    if (i > 0 && heap[i].priority > heap[par].priority) {
        swap(&heap[i], &heap[par]);
        heapifyUp(par);
    }
}

void heapifyDown(int i) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < size && heap[l].priority > heap[largest].priority) largest = l;
    if (r < size && heap[r].priority > heap[largest].priority) largest = r;
    if (largest != i) { swap(&heap[i], &heap[largest]); heapifyDown(largest); }
}

// Add a print job to queue
void addJob(int id, char* owner, char* filename, int priority, int pages) {
    if (size == MAX) { printf("Printer queue full!\n"); return; }
    heap[size].jobId = id;
    strcpy(heap[size].owner, owner);
    strcpy(heap[size].filename, filename);
    heap[size].priority = priority;
    heap[size].pages = pages;
    size++;
    heapifyUp(size - 1);
    printf("Queued: Job#%d %-15s by %-10s (%d pages, Priority=%d)\n",
           id, filename, owner, pages, priority);
}

// Print the highest priority job
void printNextJob() {
    if (size == 0) { printf("No jobs in queue.\n"); return; }
    struct PrintJob job = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    printf("Printing: Job#%d %-15s by %-10s (%d pages) [Priority=%d]\n",
           job.jobId, job.filename, job.owner, job.pages, job.priority);
}

void showQueue() {
    printf("\n--- Print Queue Status ---\n");
    printf("%-6s %-15s %-12s %-6s %s\n", "Job#", "File", "Owner", "Pages", "Priority");
    for (int i = 0; i < size; i++)
        printf("%-6d %-15s %-12s %-6d %d\n",
               heap[i].jobId, heap[i].filename, heap[i].owner,
               heap[i].pages, heap[i].priority);
    printf("\n");
}

int main() {
    printf("=== Printer Queue Management System ===\n\n");

    printf("Adding print jobs:\n");
    addJob(1, "Student1", "report.pdf",     3,  20);
    addJob(2, "Admin",    "payroll.pdf",    9,   5);
    addJob(3, "Guest",    "photo.jpg",      1,   1);
    addJob(4, "Manager",  "contract.pdf",   8,  15);
    addJob(5, "Student2", "assignment.pdf", 3,  10);

    showQueue();

    printf("--- Printing jobs in priority order ---\n");
    while (size > 0)
        printNextJob();

    return 0;
}
