#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <memory>
#include <stdexcept>
using namespace std;

// ============================================================
// 1. HELPER FUNCTION
// ============================================================

string formatTime(time_t t) {
    tm* timeInfo = localtime(&t);
    stringstream ss;
    if (timeInfo != nullptr) {
        ss << put_time(timeInfo, "%d-%m-%Y %H:%M:%S");
    }
    return ss.str();
}

// ============================================================
// 2. VEHICLE CLASS
// ============================================================

class Vehicle {
private:
    string vehicleNumber;
    string ownerName;
    string vehicleType;
    time_t entryTime;
public:
    // Default constructor
    Vehicle() {
        vehicleNumber = "";
        ownerName = "";
        vehicleType = "CAR";
        entryTime = time(nullptr);
    }
    // Parameterized constructor
    Vehicle(string number, string owner,
            string type, time_t entry = time(nullptr)) {
        vehicleNumber = number;
        ownerName = owner;
        vehicleType = type;
        entryTime = entry;
    }
    // Setter functions
    void setVehicleNumber(string number) {
        vehicleNumber = number;
    }
    void setOwnerName(string owner) {
        ownerName = owner;
    }
    void setVehicleType(string type) {
        vehicleType = type;
    }
    string getVehicleNumber() const {
        return vehicleNumber;
    }
    string getOwnerName() const {
        return ownerName;
    }
    string getVehicleType() const {
        return vehicleType;
    }
    time_t getEntryTime() const {
        return entryTime;
    }

    void displayVehicle() const {
        cout << "\nVehicle Number : " << vehicleNumber;
        cout << "\nOwner Name     : " << ownerName;
        cout << "\nVehicle Type   : " << vehicleType;
        cout << "\nEntry Time     : "
             << formatTime(entryTime) << endl;
    }
};

// ============================================================
// 3. ABSTRACT BASE CLASS
// ============================================================

class ParkingVehicle {
protected:
    string number;
public:
    ParkingVehicle(string n) {
        number = n;
    }
    // Pure virtual function
    virtual double calculateRate(int hours) const = 0;
    virtual string getType() const = 0;
    // Common function
    void displayNumber() const {
        cout << "Vehicle Number: " << number << endl;
    }
    // Virtual destructor
    virtual ~ParkingVehicle() {
        cout << "ParkingVehicle destructor called.\n";
    }
};

// ============================================================
// 4. BIKE CLASS
// ============================================================

class Bike : public ParkingVehicle {
public:
    Bike(string n) : ParkingVehicle(n) {
    }
    // Bike parking rate = Rs. 20 per hour
    double calculateRate(int hours) const override {
        return hours * 20.0;
    }
    string getType() const override {
        return "BIKE";
    }
};


// ============================================================
// 5. CAR CLASS
// ============================================================

class Car : public ParkingVehicle {
public:
    Car(string n) : ParkingVehicle(n) {
    }
    // Car parking rate = Rs. 40 per hour
    double calculateRate(int hours) const override {
        return hours * 40.0;
    }
    string getType() const override {
        return "CAR";
    }
};


// ============================================================
// 6. EV CLASS
// ============================================================
class EV : public ParkingVehicle {
public:
    EV(string n) : ParkingVehicle(n) {
    }
    // EV parking rate = Rs. 30 per hour
    double calculateRate(int hours) const override {
        return hours * 30.0;
    }
    string getType() const override {
        return "EV";
    }
};


// ============================================================
// 7. FACTORY FUNCTION
// ============================================================

unique_ptr<ParkingVehicle> createParkingVehicle(
    string type, string number) {
    if (type == "BIKE") {
        return make_unique<Bike>(number);
    }
    else if (type == "CAR") {
        return make_unique<Car>(number);
    }
    else if (type == "EV") {
        return make_unique<EV>(number);
    }
    throw invalid_argument("Invalid vehicle type.");
}

// ============================================================
// 8. PARKING SLOT CLASS
// ============================================================

class ParkingSlot {
private:
    int slotID;
    bool occupied;
    string vehicleNumber;
    string allowedType;
public:
    // Default constructor
    ParkingSlot() {
        slotID = 0;
        occupied = false;
        vehicleNumber = "";
        allowedType = "CAR";
    }
    // Parameterized constructor
    ParkingSlot(int id, string type) {
        slotID = id;
        occupied = false;
        vehicleNumber = "";
        allowedType = type;
    }
    // Getter functions
    int getSlotID() const {
        return slotID;
    }
    bool isOccupied() const {
        return occupied;
    }
    string getVehicleNumber() const {
        return vehicleNumber;
    }
    string getAllowedType() const {
        return allowedType;
    }
    // Occupy parking slot
    void occupy(string number) {
        if (occupied) {
            throw runtime_error("Slot already occupied.");
        }
        occupied = true;
        vehicleNumber = number;
    }
    // Release parking slot
    void release() {
        occupied = false;
        vehicleNumber = "";
    }
    // Display slot details
    void displaySlot() const {
        cout << "\nSlot ID: " << slotID;
        cout << "\nVehicle Type: " << allowedType;
        cout << "\nStatus: ";
        if (occupied) {
            cout << "OCCUPIED";
            cout << "\nVehicle Number: " << vehicleNumber;
        }
        else {
            cout << "EMPTY";
        }
        cout << endl;
    }
};


// ============================================================
// 9. PARKING TRANSACTION CLASS
// ============================================================

class ParkingTransaction {
private:
    string vehicleNumber;
    int slotID;
    string action;
    double fee;
    string dateTime;
public:
    // Default constructor
    ParkingTransaction() {
        slotID = 0;
        fee = 0;
    }
    // Parameterized constructor
    ParkingTransaction(string vehicle, int slot,
                       string act, double amount) {
        vehicleNumber = vehicle;
        slotID = slot;
        action = act;
        fee = amount;
        dateTime = formatTime(time(nullptr));
    }
    // Getter functions
    string getVehicleNumber() const {
        return vehicleNumber;
    }
    int getSlotID() const {
        return slotID;
    }
    double getFee() const {
        return fee;
    }
    string getAction() const {
        return action;
    }
    // Display transaction
    void displayTransaction() const {
        cout << left
             << setw(15) << vehicleNumber
             << setw(10) << slotID
             << setw(15) << action
             << setw(12) << fixed << setprecision(2)
             << fee
             << dateTime << endl;
    }
};

// ============================================================
// 10. PARKING MANAGER CLASS
// ============================================================

class ParkingManager {
private:
    static const int MAX_SLOTS = 20;
    ParkingSlot slots[MAX_SLOTS];
    Vehicle vehicles[MAX_SLOTS];
    ParkingTransaction transactions[MAX_SLOTS * 2];
    int totalSlots;
    int vehicleCount;
    int transactionCount;
public:
    // Constructor
    ParkingManager(int total = 20) {
        if (total <= 0 || total > MAX_SLOTS) {
            throw invalid_argument("Invalid parking capacity.");
        }
        totalSlots = total;
        vehicleCount = 0;
        transactionCount = 0;
        int carSlots = total / 2;
        int bikeSlots = total / 4;
        for (int i = 0; i < totalSlots; i++) {
            if (i < carSlots) {
                slots[i] = ParkingSlot(i + 1, "CAR");
            }
            else if (i < carSlots + bikeSlots) {
                slots[i] = ParkingSlot(i + 1, "BIKE");
            }
            else {
                slots[i] = ParkingSlot(i + 1, "EV");
            }
        }
    }
    // Find vehicle
    int findVehicle(string number) const {
        for (int i = 0; i < vehicleCount; i++) {
            if (vehicles[i].getVehicleNumber() == number) {
                return i;
            }
        }
        return -1;
    }
    // Find available slot
    int findAvailableSlot(string type) const {
        for (int i = 0; i < totalSlots; i++) {
            if (!slots[i].isOccupied() &&
                slots[i].getAllowedType() == type) {
                return i;
            }
        }
        return -1;
    }
    // Add transaction
    void addTransaction(string number, int slot,
                        string action, double fee) {
        if (transactionCount < MAX_SLOTS * 2) {
            transactions[transactionCount] =
                ParkingTransaction(number, slot, action, fee);
            transactionCount++;
        }
    }
    // Park vehicle
    void parkVehicle() {
        string number, owner, type;
        cout << "\nEnter Vehicle Number: ";
        cin >> number;
        if (findVehicle(number) != -1) {
            cout << "Vehicle already parked.\n";
            return;
        }
        cout << "Enter Owner Name: ";
        cin >> owner;
        int choice;
        cout << "\n1. Bike\n2. Car\n3. EV\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1)
            type = "BIKE";
        else if (choice == 2)
            type = "CAR";
        else if (choice == 3)
            type = "EV";
        else {
            cout << "Invalid vehicle type.\n";
            return;
        }
        int index = findAvailableSlot(type);
        if (index == -1) {
            cout << "No suitable parking slot available.\n";
            return;
        }
        vehicles[vehicleCount] = Vehicle(number, owner, type);
        slots[index].occupy(number);
        vehicleCount++;
        addTransaction(number, index + 1, "PARK", 0);
        cout << "\nVehicle parked successfully!";
        cout << "\nAssigned Slot: " << index + 1 << endl;
    }
    // Release vehicle
    void releaseVehicle() {
        string number;
        cout << "\nEnter Vehicle Number: ";
        cin >> number;
        int index = findVehicle(number);
        if (index == -1) {
            cout << "Vehicle not found.\n";
            return;
        }
        int slotIndex = -1;
        for (int i = 0; i < totalSlots; i++) {
            if (slots[i].getVehicleNumber() == number) {
                slotIndex = i;
                break;
            }
        }
        if (slotIndex == -1) {
            cout << "Parking slot not found.\n";
            return;
        }
        time_t exitTime = time(nullptr);
        double seconds = difftime(
            exitTime, vehicles[index].getEntryTime()
        );
        int hours = max(1, static_cast<int>(
            ceil(seconds / 3600.0)
        ));
        // Runtime polymorphism
        unique_ptr<ParkingVehicle> pv =
            createParkingVehicle(
                vehicles[index].getVehicleType(), number
            );
        double fee = pv->calculateRate(hours);
        slots[slotIndex].release();
        addTransaction(number, slotIndex + 1, "RELEASE", fee);
        cout << "\nVehicle released successfully!";
        cout << "\nDuration: " << hours << " hour(s)";
        cout << "\nParking Fee: Rs. "
             << fixed << setprecision(2) << fee << endl;
        for (int i = index; i < vehicleCount - 1; i++) {
            vehicles[i] = vehicles[i + 1];
        }
        vehicleCount--;
    }
    // Search vehicle
    void searchVehicle() {
        string number;
        cout << "\nEnter Vehicle Number: ";
        cin >> number;
        int index = findVehicle(number);
        if (index == -1) {
            cout << "Vehicle not found.\n";
            return;
        }
        vehicles[index].displayVehicle();
        for (int i = 0; i < totalSlots; i++) {
            if (slots[i].getVehicleNumber() == number) {
                cout << "Parking Slot: "
                     << slots[i].getSlotID() << endl;
                return;
            }
        }
    }
    // Display parking lot
    void displayParkingLot() {
        cout << "\n========== PARKING LOT ==========\n";
        for (int i = 0; i < totalSlots; i++) {
            slots[i].displaySlot();
        }
    }
    // Display transactions
    void displayTransactions() {
        cout << "\n========== TRANSACTION HISTORY ==========\n";
        for (int i = 0; i < transactionCount; i++) {
            transactions[i].displayTransaction();
        }
        if (transactionCount == 0) {
            cout << "No transactions available.\n";
        }
    }
    // Menu
    void menu() {
        int choice;
        do {
            cout << "\n\n===== SMART PARKING SIMULATOR =====\n";
            cout << "1. Park Vehicle\n";
            cout << "2. Release Vehicle\n";
            cout << "3. Search Vehicle\n";
            cout << "4. Display Parking Lot\n";
            cout << "5. Transaction History\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;
            switch (choice) {
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
                    displayParkingLot();
                    break;
                case 5:
                    displayTransactions();
                    break;
                case 0:
                    cout << "Thank you!\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {
    try {
        ParkingManager parking(20);
        parking.menu();
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
