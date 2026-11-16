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

void displayQueue()
{
    if (queueCount == 0)
    {
        printf("Waiting queue is empty.\n");
        return;
    }

    printf("\nWaiting Vehicles:\n");

    for (int i = 0; i < queueCount; i++)
    {
        int index = (front + i) % MAX;

        printf("%s\n", waitingQueue[index]);
    }
}


// ============================================================
// 3. STACK - RECENT PARKING ACTIONS
// ============================================================

char actionStack[MAX][100];

int top = -1;

void pushAction(char action[])
{
    if (top == MAX - 1)
    {
        printf("Action stack is full.\n");
        return;
    }

    top++;

    strcpy(actionStack[top], action);

    printf("Action recorded successfully.\n");
}

void popAction()
{
    if (top == -1)
    {
        printf("No actions available.\n");
        return;
    }

    printf("Removed Action: %s\n", actionStack[top]);

    top--;
}

void displayStack()
{
    if (top == -1)
    {
        printf("No actions recorded.\n");
        return;
    }

    printf("\nRecent Actions:\n");

    for (int i = top; i >= 0; i--)
    {
        printf("%s\n", actionStack[i]);
    }
}


// ============================================================
// 4. SINGLY LINKED LIST - TRANSACTION HISTORY
// ============================================================

struct Node
{
    int transactionID;
    char vehicleNumber[20];
    float fee;
    struct Node* next;
};

struct Node* head = NULL;
struct Node* tail = NULL;

void insertTransaction(int id, char vehicle[], float amount)
{
    struct Node* newNode;

    newNode = (struct Node*)malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->transactionID = id;

    strcpy(newNode->vehicleNumber, vehicle);

    newNode->fee = amount;

    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    printf("Transaction inserted successfully.\n");
}

void displayTransactions()
{
    struct Node* temp = head;

    if (head == NULL)
    {
        printf("No transactions available.\n");
        return;
    }

    printf("\nTransaction History:\n");

    while (temp != NULL)
    {
        printf("\nTransaction ID: %d\n", temp->transactionID);

        printf("Vehicle Number: %s\n", temp->vehicleNumber);

        printf("Parking Fee: Rs. %.2f\n", temp->fee);

        printf("-------------------------\n");

        temp = temp->next;
    }
}

void searchTransaction(int id)
{
    struct Node* temp = head;

    while (temp != NULL)
    {
        if (temp->transactionID == id)
        {
            printf("Transaction Found!\n");

            printf("Transaction ID: %d\n", temp->transactionID);

            printf("Vehicle Number: %s\n", temp->vehicleNumber);

            printf("Parking Fee: Rs. %.2f\n", temp->fee);

            return;
        }

        temp = temp->next;
    }

    printf("Transaction not found.\n");
}

void freeTransactions()
{
    struct Node* temp = head;

    while (temp != NULL)
    {
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

struct HashNode
{
    char vehicleNumber[20];
    int slot;
    struct HashNode* next;
};

struct HashNode* hashTable[TABLE_SIZE] = {NULL};

unsigned int hashFunction(char vehicle[])
{
    unsigned int hash = 0;

    for (int i = 0; vehicle[i] != '\0'; i++)
    {
        hash = (hash * 31 +
                (unsigned char)vehicle[i]) % TABLE_SIZE;
    }

    return hash;
}

void insertVehicle(char vehicle[], int slot)
{
    unsigned int index = hashFunction(vehicle);

    struct HashNode* temp = hashTable[index];

    while (temp != NULL)
    {
        if (strcmp(temp->vehicleNumber, vehicle) == 0)
        {
            temp->slot = slot;

            printf("Vehicle slot updated successfully.\n");
            return;
        }

        temp = temp->next;
    }

    struct HashNode* newNode;

    newNode = (struct HashNode*)malloc(sizeof(struct HashNode));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newNode->vehicleNumber, vehicle);

    newNode->slot = slot;

    newNode->next = hashTable[index];

    hashTable[index] = newNode;

    printf("Vehicle inserted successfully.\n");
}

void searchVehicle(char vehicle[])
{
    unsigned int index = hashFunction(vehicle);

    struct HashNode* temp = hashTable[index];

    while (temp != NULL)
    {
        if (strcmp(temp->vehicleNumber, vehicle) == 0)
        {
            printf("Vehicle Found!\n");

            printf("Vehicle Number: %s\n", temp->vehicleNumber);

            printf("Parking Slot: %d\n", temp->slot);

            return;
        }

        temp = temp->next;
    }

    printf("Vehicle not found.\n");
}

void deleteVehicle(char vehicle[])
{
    unsigned int index = hashFunction(vehicle);

    struct HashNode* temp = hashTable[index];

    struct HashNode* previous = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->vehicleNumber, vehicle) == 0)
        {
            if (previous == NULL)
            {
                hashTable[index] = temp->next;
            }
            else
            {
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

void displayHashTable()
{
    int found = 0;

    printf("\nParked Vehicles:\n");

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct HashNode* temp = hashTable[i];

        while (temp != NULL)
        {
            printf("Vehicle: %s | Slot: %d\n",
                   temp->vehicleNumber,
                   temp->slot);

            found = 1;

            temp = temp->next;
        }
    }

    if (!found)
    {
        printf("No vehicles stored.\n");
    }
}

void freeHashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct HashNode* temp = hashTable[i];

        while (temp != NULL)
        {
            struct HashNode* nextNode = temp->next;

            free(temp);

            temp = nextNode;
        }

        hashTable[i] = NULL;
    }
}


// ============================================================
// 6. SORTING - VEHICLE NUMBERS (BUBBLE SORT)
// ============================================================

void sortVehicles()
{
    char vehicles[MAX][20];
    char temp[20];

    int n;

    printf("Enter number of vehicles to sort: ");
    if (scanf("%d", &n) != 1)
    {
        printf("Invalid input.\n");
        return;
    }

    if (n <= 0 || n > MAX)
    {
        printf("Enter a number between 1 and %d.\n", MAX);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("Enter vehicle number %d: ", i + 1);
        scanf("%19s", vehicles[i]);
    }

    // Bubble Sort
    for (int i = 0; i < n - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < n - i - 1; j++)
        {
            if (strcmp(vehicles[j], vehicles[j + 1]) > 0)
            {
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

    for (int i = 0; i < n; i++)
    {
        printf("%s\n", vehicles[i]);
    }
}


// ============================================================
// MAIN FUNCTION - INTERACTIVE MENU
// ============================================================

int main()
{
    int choice;

    int slotID;
    int transactionID;
    int slot;

    float fee;

    char vehicle[20];
    char action[100];

    do
    {
        printf("\n====================================\n");
        printf("       SMART PARKING SIMULATOR\n");
        printf("====================================\n");

        printf("\n1. Insert Parking Slot");
        printf("\n2. Display Parking Slots");
        printf("\n3. Search Parking Slot");

        printf("\n4. Add Vehicle to Waiting Queue");
        printf("\n5. Remove Vehicle from Waiting Queue");
        printf("\n6. Display Waiting Queue");

        printf("\n7. Add Parking Action");
        printf("\n8. Remove Recent Parking Action");
        printf("\n9. Display Recent Actions");

        printf("\n10. Insert Transaction");
        printf("\n11. Display Transaction History");
        printf("\n12. Search Transaction");

        printf("\n13. Insert Vehicle in Hash Table");
        printf("\n14. Search Vehicle");
        printf("\n15. Delete Vehicle");
        printf("\n16. Display Hash Table");

        printf("\n17. Sort Vehicle Numbers");

        printf("\n0. Exit");

        printf("\n\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        switch (choice)
        {
            // ARRAY

            case 1:
                printf("Enter parking slot ID: ");
                if (scanf("%d", &slotID) != 1)
                {
                    printf("Invalid slot ID.\n");
                    break;
                }
                insertSlot(slotID);
                break;

            case 2:
                displaySlots();
                break;

            case 3:
                printf("Enter slot ID to search: ");
                if (scanf("%d", &slotID) != 1)
                {
                    printf("Invalid slot ID.\n");
                    break;
                }
                searchSlot(slotID);
                break;


            // QUEUE

            case 4:
                printf("Enter vehicle number: ");
                scanf("%19s", vehicle);
                enqueue(vehicle);
                break;

            case 5:
                dequeue();
                break;

            case 6:
                displayQueue();
                break;


            // STACK

            case 7:
                printf("Enter parking action: ");

                if (scanf(" %99[^\n]", action) != 1)
                {
                    printf("Invalid action.\n");
                    break;
                }

                pushAction(action);
                break;

            case 8:
                popAction();
                break;

            case 9:
                displayStack();
                break;


            // LINKED LIST

            case 10:
                printf("Enter transaction ID: ");
                if (scanf("%d", &transactionID) != 1)
                {
                    printf("Invalid transaction ID.\n");
                    break;
                }

                printf("Enter vehicle number: ");
                scanf("%19s", vehicle);

                printf("Enter parking fee: ");
                if (scanf("%f", &fee) != 1)
                {
                    printf("Invalid fee.\n");
                    break;
                }

                insertTransaction(transactionID, vehicle, fee);
                break;

            case 11:
                displayTransactions();
                break;

            case 12:
                printf("Enter transaction ID to search: ");
                if (scanf("%d", &transactionID) != 1)
                {
                    printf("Invalid transaction ID.\n");
                    break;
                }

                searchTransaction(transactionID);
                break;


            // HASH TABLE

            case 13:
                printf("Enter vehicle number: ");
                scanf("%19s", vehicle);

                printf("Enter parking slot: ");
                if (scanf("%d", &slot) != 1)
                {
                    printf("Invalid slot.\n");
                    break;
                }

                insertVehicle(vehicle, slot);
                break;

            case 14:
                printf("Enter vehicle number to search: ");
                scanf("%19s", vehicle);

                searchVehicle(vehicle);
                break;

            case 15:
                printf("Enter vehicle number to delete: ");
                scanf("%19s", vehicle);

                deleteVehicle(vehicle);
                break;

            case 16:
                displayHashTable();
                break;


            // SORTING

            case 17:
                sortVehicles();
                break;


            case 0:
                printf("\nExiting Smart Parking Simulator...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);


    // Free dynamically allocated memory

    freeTransactions();

    freeHashTable();

    printf("\nDSA DEMONSTRATION COMPLETED!\n");

    return 0;
}
