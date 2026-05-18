// Hospital Patient Management using Priority Queue
// Critical patients treated first (higher priority = more urgent)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

struct Patient {
    int id;
    char name[30];
    int priority;    // 1=Normal, 5=Urgent, 10=Critical
    char condition[30];
};

struct Patient heap[MAX];
int size = 0;

void swap(struct Patient* a, struct Patient* b) {
    struct Patient t = *a; *a = *b; *b = t;
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

// Admit a patient
void admitPatient(int id, char* name, int priority, char* condition) {
    if (size == MAX) { printf("Hospital queue full!\n"); return; }
    heap[size].id = id;
    strcpy(heap[size].name, name);
    heap[size].priority = priority;
    strcpy(heap[size].condition, condition);
    size++;
    heapifyUp(size - 1);
    printf("Admitted: %-15s | Priority: %d | Condition: %s\n", name, priority, condition);
}

// Treat next patient (highest priority)
void treatPatient() {
    if (size == 0) { printf("No patients waiting.\n"); return; }
    struct Patient p = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    printf("Treating: %-15s | Priority: %d | Condition: %s\n", p.name, p.priority, p.condition);
}

void showQueue() {
    printf("\n--- Current Patient Queue ---\n");
    printf("%-5s %-15s %-10s %s\n", "ID", "Name", "Priority", "Condition");
    for (int i = 0; i < size; i++)
        printf("%-5d %-15s %-10d %s\n", heap[i].id, heap[i].name, heap[i].priority, heap[i].condition);
    printf("\n");
}

int main() {
    printf("=== Hospital Patient Management System ===\n\n");

    printf("Admitting patients:\n");
    admitPatient(1, "Alice",   1,  "Common Cold");
    admitPatient(2, "Bob",     10, "Heart Attack");
    admitPatient(3, "Charlie", 5,  "Broken Arm");
    admitPatient(4, "Diana",   8,  "Severe Burn");
    admitPatient(5, "Eve",     3,  "Fever");

    showQueue();

    printf("--- Calling patients for treatment ---\n");
    while (size > 0)
        treatPatient();

    return 0;
}
