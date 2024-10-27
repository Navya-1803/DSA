#include <stdio.h>
#include <stdlib.h>

// Structure for a process
struct Process {
    int processID;
    int burstTime;
    struct Process* next;
};

// Structure for Queue
struct Queue {
    struct Process* front;
    struct Process* rear;
};

// Function to create a new process
struct Process* newProcess(int id, int burstTime) {
    struct Process* temp = (struct Process*)malloc(sizeof(struct Process));
    temp->processID = id;
    temp->burstTime = burstTime;
    temp->next = NULL;
    return temp;
}

// Function to create a queue
struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to enqueue a process into the queue
void enqueue(struct Queue* q, int id, int burstTime) {
    struct Process* temp = newProcess(id, burstTime);
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

// Function to dequeue a process from the queue
struct Process* dequeue(struct Queue* q) {
    if (q->front == NULL) {
        return NULL;
    }
    struct Process* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

// Function to swap two processes (for sorting)
void swap(struct Process* a, struct Process* b) {
    int tempID = a->processID;
    int tempBurst = a->burstTime;
    a->processID = b->processID;
    a->burstTime = b->burstTime;
    b->processID = tempID;
    b->burstTime = tempBurst;
}

// Function to sort the queue by burst time (SJF behavior)
void sortQueue(struct Queue* q) {
    struct Process* i = q->front;
    struct Process* j = NULL;
    if (q->front == NULL) {
        return;
    }
    while (i != NULL) {
        j = i->next;
        while (j != NULL) {
            if (i->burstTime > j->burstTime) {
                swap(i, j);
            }
            j = j->next;
        }
        i = i->next;
    }
}

// Function to calculate waiting time
void findWaitingTime(struct Queue* q, int n, int waiting_time[]) {
    struct Process* temp = q->front;
    waiting_time[0] = 0;
    int i = 1;
    while (temp != NULL && temp->next != NULL) {
        waiting_time[i] = waiting_time[i - 1] + temp->burstTime;
        temp = temp->next;
        i++;
    }
}

// Function to calculate turnaround time
void findTurnaroundTime(struct Queue* q, int n, int waiting_time[], int turnaround_time[]) {
    struct Process* temp = q->front;
    int i = 0;
    while (temp != NULL) {
        turnaround_time[i] = temp->burstTime + waiting_time[i];
        temp = temp->next;
        i++;
    }
}

// Function to print the queue and average times
void findAvgTime(struct Queue* q, int n) {
    int waiting_time[n], turnaround_time[n];
    int total_waiting_time = 0, total_turnaround_time = 0;

    // Calculate waiting and turnaround time
    findWaitingTime(q, n, waiting_time);
    findTurnaroundTime(q, n, waiting_time, turnaround_time);

    printf("Process ID   Burst Time   Waiting Time   Turnaround Time\n");
    struct Process* temp = q->front;
    int i = 0;
    while (temp != NULL) {
        total_waiting_time += waiting_time[i];
        total_turnaround_time += turnaround_time[i];
        printf("    %d            %d              %d                %d\n", temp->processID, temp->burstTime, waiting_time[i], turnaround_time[i]);
        temp = temp->next;
        i++;
    }

    printf("Average waiting time = %.2f\n", (float)total_waiting_time / n);
    printf("Average turnaround time = %.2f\n", (float)total_turnaround_time / n);
}

// Function for SJF Scheduling
void sjfScheduling(struct Queue* q, int n) {
    // Sort the queue by burst time (SJF behavior)
    sortQueue(q);

    // Find average time
    findAvgTime(q, n);
}

int main() {
    int p = 0;

    // Input number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &p);

    struct Queue* q = createQueue();

    // Input burst time for each process
    printf("Enter the burst times of each process:\n");
    for (int i = 0; i < p; i++) {
        int burstTime;
        printf("Enter burst time of Process %d: ", (i + 1));
        scanf("%d", &burstTime);
        enqueue(q, i + 1, burstTime); // Enqueue each process with its ID and burst time
    }

    // Call SJF Scheduling function
    sjfScheduling(q, p);

    return 0;
}
