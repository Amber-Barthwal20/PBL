#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// ==================================================
// CONVERT VEHICLE NUMBER TO UPPERCASE
// ==================================================

string toUpper(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = toupper((unsigned char)text[i]);

    return text;
}

// ==================================================
// BASE CLASS
// ==================================================

class Vehicle
{
protected:
    string userName;
    string vehicleNumber;
    string vehicleType;

public:
    Vehicle(string name, string number, string type)
    {
        userName = name;
        vehicleNumber = toUpper(number);
        vehicleType = type;
    }

    string getName()
    {
        return userName;
    }

    string getNumber()
    {
        return vehicleNumber;
    }

    string getType()
    {
        return vehicleType;
    }

    void displayDetails()
    {
        cout << "User Name      : " << userName << endl;
        cout << "Vehicle Number : " << vehicleNumber << endl;
        cout << "Vehicle Type   : " << vehicleType << endl;
    }
};

// ==================================================
// DERIVED CLASSES
// ==================================================

class Car : public Vehicle
{
public:
    Car(string name, string number)
        : Vehicle(name, number, "CAR") {}
};

class Bike : public Vehicle
{
public:
    Bike(string name, string number)
        : Vehicle(name, number, "BIKE") {}
};

class ElectricVehicle : public Vehicle
{
public:
    ElectricVehicle(string name, string number)
        : Vehicle(name, number, "EV") {}
};

// ==================================================
// PARKING SLOT
// ==================================================

class ParkingSlot
{
public:
    int slotNumber;
    Vehicle* vehicle;

    ParkingSlot()
    {
        slotNumber = 0;
        vehicle = NULL;
    }
};

// ==================================================
// STACK: PARKING HISTORY
// ==================================================

class HistoryStack
{
private:
    string history[100];
    int top;

public:
    HistoryStack()
    {
        top = -1;
    }

    void push(string entry)
    {
        if (top < 99)
            history[++top] = entry;
    }

    void display()
    {
        if (top == -1)
        {
            cout << "\nNo parking history available.\n";
            return;
        }

        cout << "\n========== PARKING HISTORY ==========\n";

        for (int i = top; i >= 0; i--)
            cout << history[i] << endl;
    }

    void clear()
    {
        top = -1;
    }
};

// ==================================================
// QUEUE: WAITING VEHICLES
// ==================================================

struct WaitingVehicle
{
    string userName;
    string vehicleNumber;
    int type;
};

class WaitingQueue
{
private:
    WaitingVehicle queue[50];
    int front;
    int rear;
    int count;

public:
    WaitingQueue()
    {
        front = 0;
        rear = -1;
        count = 0;
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool isFull()
    {
        return count == 50;
    }

    // Function to return the number of waiting vehicles
    int getCount()
    {
        return count;
    }

    void enqueue(string name, string number, int type)
    {
        if (isFull())
        {
            cout << "\nWaiting queue is full.\n";
            return;
        }

        rear = (rear + 1) % 50;

        queue[rear].userName = name;
        queue[rear].vehicleNumber = toUpper(number);
        queue[rear].type = type;

        count++;

        cout << "\nParking lot is full.\n";
        cout << "Vehicle added to waiting queue.\n";
        cout << "Queue Position: " << count << endl;
    }

    WaitingVehicle dequeue()
    {
        WaitingVehicle vehicle;

        vehicle.userName = queue[front].userName;
        vehicle.vehicleNumber = queue[front].vehicleNumber;
        vehicle.type = queue[front].type;

        front = (front + 1) % 50;
        count--;

        return vehicle;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "\nNo vehicles are waiting.\n";
            return;
        }

        cout << "\n========== WAITING QUEUE ==========\n";

        for (int i = 0; i < count; i++)
        {
            int index = (front + i) % 50;

            cout << "\nQueue Position : " << i + 1 << endl;
            cout << "User Name      : "
                 << queue[index].userName << endl;
            cout << "Vehicle Number : "
                 << queue[index].vehicleNumber << endl;
            cout << "Vehicle Type   : ";

            if (queue[index].type == 1)
                cout << "CAR\n";
            else if (queue[index].type == 2)
                cout << "BIKE\n";
            else
                cout << "EV\n";
        }
    }

    void clear()
    {
        front = 0;
        rear = -1;
        count = 0;
    }
};

// ==================================================
// LINKED LIST: TRANSACTION RECORDS
// ==================================================

class Node
{
public:
    string data;
    Node* next;

    Node(string value)
    {
        data = value;
        next = NULL;
    }
};

class TransactionList
{
private:
    Node* head;

public:
    TransactionList()
    {
        head = NULL;
    }

    void add(string data)
    {
        Node* newNode = new Node(data);

        if (head == NULL)
        {
            head = newNode;
            return;
        }

        Node* temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }

    void display()
    {
        if (head == NULL)
        {
            cout << "\nNo transactions recorded.\n";
            return;
        }

        cout << "\n========== TRANSACTION RECORDS ==========\n";

        Node* temp = head;

        while (temp != NULL)
        {
            cout << temp->data << endl;
            temp = temp->next;
        }
    }

    void clear()
    {
        while (head != NULL)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~TransactionList()
    {
        clear();
    }
};

// ==================================================
// PARKING LOT
// ==================================================

class ParkingLot
{
private:
    static const int TOTAL_SLOTS = 20;

    ParkingSlot slots[TOTAL_SLOTS];
    Vehicle* vehicles[TOTAL_SLOTS];

    HistoryStack history;
    WaitingQueue waitingQueue;
    TransactionList transactions;

    // SEARCH VEHICLE USING LINEAR SEARCH
    int findVehicle(string number)
    {
        number = toUpper(number);

        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            if (vehicles[i] != NULL &&
                vehicles[i]->getNumber() == number)
                return i;
        }

        return -1;
    }

    // FIND AN EMPTY SLOT
    int findEmptySlot()
    {
        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            if (vehicles[i] == NULL)
                return i;
        }

        return -1;
    }

    // CREATE AN OBJECT OF THE REQUIRED DERIVED CLASS
    Vehicle* createVehicle(string name, string number, int type)
    {
        if (type == 1)
            return new Car(name, number);

        if (type == 2)
            return new Bike(name, number);

        return new ElectricVehicle(name, number);
    }

    // DELETE THE CORRECT DERIVED OBJECT WITHOUT VIRTUAL FUNCTIONS
    void destroyVehicle(Vehicle* vehicle)
    {
        if (vehicle == NULL)
            return;

        if (vehicle->getType() == "CAR")
            delete static_cast<Car*>(vehicle);
        else if (vehicle->getType() == "BIKE")
            delete static_cast<Bike*>(vehicle);
        else
            delete static_cast<ElectricVehicle*>(vehicle);
    }

    // PARK THE NEXT WAITING VEHICLE
    void parkWaitingVehicle()
    {
        if (waitingQueue.isEmpty())
            return;

        int slot = findEmptySlot();

        if (slot == -1)
            return;

        WaitingVehicle waiting = waitingQueue.dequeue();

        vehicles[slot] = createVehicle(
            waiting.userName,
            waiting.vehicleNumber,
            waiting.type
        );

        slots[slot].vehicle = vehicles[slot];

        cout << "\nNext waiting vehicle parked automatically.\n";
        cout << "Vehicle Number : "
             << waiting.vehicleNumber << endl;
        cout << "Vehicle Type   : "
             << vehicles[slot]->getType() << endl;
        cout << "Parking Slot   : "
             << slot + 1 << endl;

        history.push("Parked: " + waiting.vehicleNumber);

        transactions.add(
            "Vehicle " + waiting.vehicleNumber +
            " parked in Slot " + to_string(slot + 1)
        );
    }

public:
    // CONSTRUCTOR
    ParkingLot()
    {
        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            slots[i].slotNumber = i + 1;
            slots[i].vehicle = NULL;
            vehicles[i] = NULL;
        }
    }

    // FUNCTION: TOTAL WAITING VEHICLES
    int totalWaitingVehicles()
    {
        return waitingQueue.getCount();
    }

    // FUNCTION: PARK VEHICLE
    void parkVehicle()
    {
        string name, number;
        int type;

        cout << "\n========== PARK A VEHICLE ==========\n";

        cout << "Enter User Name: ";
        getline(cin >> ws, name);

        cout << "Enter Vehicle Number: ";
        cin >> number;

        number = toUpper(number);

        if (findVehicle(number) != -1)
        {
            cout << "\nThis vehicle is already parked.\n";
            return;
        }

        cout << "\nSelect Vehicle Type:\n";
        cout << "1. CAR\n";
        cout << "2. BIKE\n";
        cout << "3. EV\n";
        cout << "Enter choice: ";
        cin >> type;

        if (type < 1 || type > 3)
        {
            cout << "\nInvalid vehicle type.\n";
            return;
        }

        int slot = findEmptySlot();

        if (slot == -1)
        {
            waitingQueue.enqueue(name, number, type);
            return;
        }

        vehicles[slot] = createVehicle(name, number, type);
        slots[slot].vehicle = vehicles[slot];

        cout << "\nVehicle parked successfully!\n";
        cout << "User Name      : " << name << endl;
        cout << "Vehicle Number : " << number << endl;
        cout << "Vehicle Type   : "
             << vehicles[slot]->getType() << endl;
        cout << "Parking Slot   : " << slot + 1 << endl;

        history.push("Parked: " + number);

        transactions.add(
            "Vehicle " + number +
            " parked in Slot " + to_string(slot + 1)
        );
    }

    // FUNCTION: RELEASE VEHICLE
    void releaseVehicle()
    {
        string number;

        cout << "\nEnter Vehicle Number to Release: ";
        cin >> number;

        number = toUpper(number);

        int index = findVehicle(number);

        if (index == -1)
        {
            cout << "\nVehicle not found in the parking lot.\n";
            return;
        }

        cout << "\nVehicle released successfully!\n";
        cout << "Vehicle Number : "
             << vehicles[index]->getNumber() << endl;
        cout << "Vehicle Type   : "
             << vehicles[index]->getType() << endl;
        cout << "Released Slot  : " << index + 1 << endl;

        history.push("Released: " + number);

        transactions.add(
            "Vehicle " + number +
            " released from Slot " + to_string(index + 1)
        );

        destroyVehicle(vehicles[index]);

        vehicles[index] = NULL;
        slots[index].vehicle = NULL;

        parkWaitingVehicle();
    }

    // FUNCTION: SEARCH VEHICLE
    void searchVehicle()
    {
        string number;

        cout << "\nEnter Vehicle Number to Search: ";
        cin >> number;

        int index = findVehicle(number);

        if (index == -1)
        {
            cout << "\nVehicle not found.\n";
            return;
        }

        cout << "\n========== VEHICLE FOUND ==========\n";

        vehicles[index]->displayDetails();

        cout << "Parking Slot   : " << index + 1 << endl;
    }

    // FUNCTION: DISPLAY PARKING LOT TABLE
    void displayParkingStatus()
    {
        int occupied = 0;
        int available = 0;

        cout << "\n";
        cout << "============================================================\n";
        cout << "                         PARKING LOT\n";
        cout << "============================================================\n";

        cout << "Slot\t\tStatus\t\tVehicle Number\t\tType\n";
        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            cout << "Slot " << i + 1 << "\t| ";

            if (vehicles[i] != NULL)
            {
                occupied++;

                cout << "[ OCCUPIED ]  | ";
                cout << vehicles[i]->getNumber();

                if (vehicles[i]->getNumber().length() < 8)
                    cout << "\t\t\t";
                else
                    cout << "\t";

                cout << "| " << vehicles[i]->getType() << endl;
            }
            else
            {
                available++;

                cout << "[   EMPTY   ] | ";
                cout << "-\t\t\t| -\n";
            }
        }

        cout << "============================================================\n";

        cout << "\nTotal Slots      : " << TOTAL_SLOTS << endl;
        cout << "Occupied Slots   : " << occupied << endl;
        cout << "Available Slots  : " << available << endl;
        cout << "Waiting Vehicles : "
             << totalWaitingVehicles() << endl;
    }

    // FUNCTION: DISPLAY ALL PARKED VEHICLES
    void displayVehicles()
    {
        bool found = false;

        cout << "\n========== PARKED VEHICLES ==========\n";

        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            if (vehicles[i] != NULL)
            {
                found = true;

                cout << "\nParking Slot : " << i + 1 << endl;
                vehicles[i]->displayDetails();
            }
        }

        if (!found)
            cout << "No vehicles are currently parked.\n";
    }

    // FUNCTION: DISPLAY STACK
    void displayHistory()
    {
        history.display();
    }

    // FUNCTION: DISPLAY QUEUE
    void displayWaitingQueue()
    {
        waitingQueue.display();
    }

    // FUNCTION: DISPLAY LINKED LIST
    void displayTransactions()
    {
        transactions.display();
    }

    // FUNCTION: RESET PARKING LOT
    void resetParkingLot()
    {
        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            destroyVehicle(vehicles[i]);

            vehicles[i] = NULL;
            slots[i].vehicle = NULL;
        }

        history.clear();
        waitingQueue.clear();
        transactions.clear();

        cout << "\nParking lot reset successfully.\n";
    }

    // MAIN MENU
    void run()
    {
        int choice;

        do
        {
            cout << "\n\n";
            cout << "============================================\n";
            cout << "          SMART PARKING SIMULATOR\n";
            cout << "============================================\n";
            cout << "1. Park a Vehicle\n";
            cout << "2. Release a Vehicle\n";
            cout << "3. Search for a Vehicle\n";
            cout << "4. Display Parking Lot\n";
            cout << "5. Display Parked Vehicles\n";
            cout << "6. View Parking History\n";
            cout << "7. Display Waiting Queue\n";
            cout << "8. Display Transactions\n";
            cout << "9. Reset Parking Lot\n";
            cout << "0. Exit\n";
            cout << "--------------------------------------------\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input. Enter a number.\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    parkVehicle();
                    break;

                case 2:
                    releaseVehicle();
                    break;

                case 3:
                    searchVehicle();
                    break;

                case 4:
                    displayParkingStatus();
                    break;

                case 5:
                    displayVehicles();
                    break;

                case 6:
                    displayHistory();
                    break;

                case 7:
                    displayWaitingQueue();
                    break;

                case 8:
                    displayTransactions();
                    break;

                case 9:
                    resetParkingLot();
                    break;

                case 0:
                    cout << "\nThank you for using the Parking Simulator!\n";
                    break;

                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 0);
    }

    // DESTRUCTOR
    ~ParkingLot()
    {
        for (int i = 0; i < TOTAL_SLOTS; i++)
        {
            destroyVehicle(vehicles[i]);
            vehicles[i] = NULL;
        }
    }
};

// ==================================================
// MAIN FUNCTION
// ==================================================

int main()
{
    ParkingLot parking;
    parking.run();

    return 0;
}