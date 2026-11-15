
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define TABLE_SIZE 101

// ============================================================
// 1. ARRAY - PARKING SLOTS
// ============================================================

int slots[MAX];
int slotCount = 0;

void insertSlot(int slotID) 
{
    if (slotCount == MAX) 
    {
        printf("Parking slots are full.\n");
        return;
    }
    slots[slotCount] = slotID;
    slotCount++;
    printf("Slot %d added successfully.\n", slotID);
}

void displaySlots() 
{
    if (slotCount == 0) 
    {
        printf("No parking slots available.\n");
        return;
    }
    printf("\nParking Slots:\n");
    for (int i = 0; i < slotCount; i++) 
    {
        printf("%d ", slots[i]);
    }
    printf("\n");
}

void searchSlot(int id) 
{
    for (int i = 0; i < slotCount; i++) 
    {
        if (slots[i] == id)
        {
            printf("Slot %d found.\n", id);
            return;
        }
    }
    printf("Slot not found.\n");
}


// ============================================================
// 2. QUEUE - WAITING VEHICLES
// ============================================================

char waitingQueue[MAX][20];

int front = 0;
int rear = -1;
int queueCount = 0;

// Add vehicle at the rear
void enqueue(char vehicle[]) 
{
    if (queueCount == MAX) 
    {
        printf("Waiting queue is full.\n");
        return;
    }
    rear = (rear + 1) % MAX;
    strcpy(waitingQueue[rear], vehicle);
    queueCount++;
    printf("%s added to waiting queue.\n", vehicle);
}

// Remove vehicle from the front
void dequeue() 
{
    if (queueCount == 0) 
    {
        printf("Waiting queue is empty.\n");
        return;
    }
    printf("Vehicle removed: %s\n", waitingQueue[front]);
    front = (front + 1) % MAX;
    queueCount--;
}

// Display all waiting vehicles
void displayQueue() {

    if (queueCount == 0) {
        printf("Waiting queue is empty.\n");
        return;
    }

    printf("\nWaiting Vehicles:\n");

    for (int i = 0; i < queueCount; i++) {

        int index = (front + i) % MAX;

        printf("%s\n", waitingQueue[index]);
    }
}


// ============================================================
// 3. STACK - RECENT PARKING ACTIONS
// ============================================================

char actionStack[MAX][100];

int top = -1;

// Insert action into stack
void pushAction(char action[]) {

    if (top == MAX - 1) {
        printf("Action stack is full.\n");
        return;
    }

    top++;

    strcpy(actionStack[top], action);

    printf("Action recorded successfully.\n");
}

// Remove most recent action
void popAction() {

    if (top == -1) {
        printf("No actions available.\n");
        return;
    }

    printf("Removed Action: %s\n",
           actionStack[top]);

    top--;
}

// Display recent actions
void displayStack() {

    if (top == -1) {
        printf("No actions recorded.\n");
        return;
    }

    printf("\nRecent Actions:\n");

    for (int i = top; i >= 0; i--) {
        printf("%s\n", actionStack[i]);
    }
}


// ============================================================
// 4. SINGLY LINKED LIST - TRANSACTION HISTORY
// ============================================================

struct Node {

    int transactionID;

    char vehicleNumber[20];

    float fee;

    struct Node* next;
};

struct Node* head = NULL;
struct Node* tail = NULL;

// Insert transaction at the end
void insertTransaction(
    int id,
    char vehicle[],
    float amount
) {

    struct Node* newNode;

    newNode = (struct Node*)malloc(sizeof(struct Node));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->transactionID = id;

    strcpy(newNode->vehicleNumber, vehicle);

    newNode->fee = amount;

    newNode->next = NULL;

    // If linked list is empty
    if (head == NULL) {

        head = newNode;
        tail = newNode;
    }

    // Insert after the last node
    else {

        tail->next = newNode;
        tail = newNode;
    }

    printf("Transaction inserted successfully.\n");
}

// Display all transactions
void displayTransactions() {

    struct Node* temp = head;

    if (head == NULL) {
        printf("No transactions available.\n");
        return;
    }

    printf("\nTransaction History:\n");

    while (temp != NULL) {

        printf("\nTransaction ID: %d\n",
               temp->transactionID);

        printf("Vehicle Number: %s\n",
               temp->vehicleNumber);

        printf("Parking Fee: Rs. %.2f\n",
               temp->fee);

        printf("-------------------------\n");

        temp = temp->next;
    }
}

// Search transaction by ID
void searchTransaction(int id) {

    struct Node* temp = head;

    while (temp != NULL) {

        if (temp->transactionID == id) {

            printf("Transaction Found!\n");

            printf("Transaction ID: %d\n",
                   temp->transactionID);

            printf("Vehicle Number: %s\n",
                   temp->vehicleNumber);

            printf("Parking Fee: Rs. %.2f\n",
                   temp->fee);

            return;
        }

        temp = temp->next;
    }

    printf("Transaction not found.\n");
}

// Free linked list memory
void freeTransactions() {

    struct Node* temp = head;

    while (temp != NULL) {

        struct Node* nextNode = temp->next;

        free(temp);

        temp = nextNode;
    }

    head = NULL;
    tail = NULL;
}


// ============================================================
// 5. HASH TABLE - VEHICLE SEARCHING
// ============================================================

struct HashNode {

    char vehicleNumber[20];

    int slot;

    struct HashNode* next;
};

struct HashNode* hashTable[TABLE_SIZE] = {NULL};

// Hash function using polynomial accumulation
unsigned int hashFunction(char vehicle[]) {

    unsigned int hash = 0;

    for (int i = 0; vehicle[i] != '\0'; i++) {

        hash = (hash * 31 +
                (unsigned char)vehicle[i]) % TABLE_SIZE;
    }

    return hash;
}

// Insert vehicle into hash table
void insertVehicle(char vehicle[], int slot) {

    unsigned int index = hashFunction(vehicle);

    // Update existing vehicle if already present
    struct HashNode* temp = hashTable[index];

    while (temp != NULL) {

        if (strcmp(temp->vehicleNumber, vehicle) == 0) {

            temp->slot = slot;

            printf("Vehicle slot updated successfully.\n");
            return;
        }

        temp = temp->next;
    }

    struct HashNode* newNode;

    newNode = (struct HashNode*)malloc(sizeof(struct HashNode));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newNode->vehicleNumber, vehicle);

    newNode->slot = slot;

    newNode->next = hashTable[index];

    hashTable[index] = newNode;

    printf("Vehicle inserted successfully.\n");
}

// Search vehicle
void searchVehicle(char vehicle[]) {

    unsigned int index = hashFunction(vehicle);

    struct HashNode* temp = hashTable[index];

    while (temp != NULL) {

        if (strcmp(temp->vehicleNumber, vehicle) == 0) {

            printf("Vehicle Found!\n");

            printf("Vehicle Number: %s\n",
                   temp->vehicleNumber);

            printf("Parking Slot: %d\n",
                   temp->slot);

            return;
        }

        temp = temp->next;
    }

    printf("Vehicle not found.\n");
}

// Delete vehicle
void deleteVehicle(char vehicle[]) {

    unsigned int index = hashFunction(vehicle);

    struct HashNode* temp = hashTable[index];

    struct HashNode* previous = NULL;

    while (temp != NULL) {

        if (strcmp(temp->vehicleNumber, vehicle) == 0) {

            if (previous == NULL) {
                hashTable[index] = temp->next;
            }

            else {
                previous->next = temp->next;
            }

            free(temp);

            printf("Vehicle removed successfully.\n");

            return;
        }

        previous = temp;
        temp = temp->next;
    }

    printf("Vehicle not found.\n");
}

// Display all hash table entries
void displayHashTable() {

    int found = 0;

    printf("\nParked Vehicles:\n");

    for (int i = 0; i < TABLE_SIZE; i++) {

        struct HashNode* temp = hashTable[i];

        while (temp != NULL) {

            printf("Vehicle: %s | Slot: %d\n",
                   temp->vehicleNumber,
                   temp->slot);

            found = 1;

            temp = temp->next;
        }
    }

    if (!found) {
        printf("No vehicles stored.\n");
    }
}

// Free hash table memory
void freeHashTable() {

    for (int i = 0; i < TABLE_SIZE; i++) {

        struct HashNode* temp = hashTable[i];

        while (temp != NULL) {

            struct HashNode* nextNode = temp->next;

            free(temp);

            temp = nextNode;
        }

        hashTable[i] = NULL;
    }
}


// ============================================================
// 6. SORTING - VEHICLE NUMBERS
// ============================================================

void sortVehicles() {

    char vehicles[4][20] = {

        "UK07AK2349",
        "DL01A1234",
        "HR26AB1234",
        "UP16CD5678"
    };

    int n = 4;

    char temp[20];

    // Bubble Sort
    for (int i = 0; i < n - 1; i++) {

        int swapped = 0;

        for (int j = 0; j < n - i - 1; j++) {

            if (strcmp(vehicles[j], vehicles[j + 1]) > 0) {

                strcpy(temp, vehicles[j]);

                strcpy(vehicles[j], vehicles[j + 1]);

                strcpy(vehicles[j + 1], temp);

                swapped = 1;
            }
        }

        if (!swapped)
            break;
    }

    printf("\nSorted Vehicle Numbers:\n");

    for (int i = 0; i < n; i++) {

        printf("%s\n", vehicles[i]);
    }
}


// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{

    printf("\n========== ARRAY ==========\n");
    insertSlot(1);
    insertSlot(2);
    insertSlot(3);
    displaySlots();
    searchSlot(2);

    
    printf("\n========== QUEUE ==========\n");
    enqueue("UK07AK2349");
    enqueue("DL01A1234");
    enqueue("HR26AB1234");
    displayQueue();
    dequeue();
    displayQueue();


    printf("\n========== STACK ==========\n");
    pushAction("Vehicle UK07AK2349 Parked");
    pushAction("Vehicle DL01A1234 Parked");
    pushAction("Vehicle UK07AK2349 Released");
    displayStack();
    popAction();
    displayStack();


    printf("\n========== LINKED LIST ==========\n");
    insertTransaction(101, "UK07AK2349", 100);
    insertTransaction(102, "DL01A1234", 200);
    insertTransaction(103, "HR26AB1234", 150);
    displayTransactions();
    searchTransaction(102);


    printf("\n========== HASH TABLE ==========\n");
    insertVehicle("UK07AK2349", 1);
    insertVehicle("DL01A1234", 2);
    insertVehicle("HR26AB1234", 3);
    displayHashTable();
    searchVehicle("DL01A1234");
    deleteVehicle("HR26AB1234");


    printf("\n========== SORTING ==========\n");
    sortVehicles();


    freeTransactions();
    freeHashTable();
    printf("\nDSA DEMONSTRATION COMPLETED!\n");

    return 0;
}
