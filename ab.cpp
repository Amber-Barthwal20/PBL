#include <iostream>
#include <cctype>
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_map>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <ctime>
#include <string>
#include <stdexcept>
using namespace std;

// ============================================================
//                  PARKING SIMULATOR (FIXED)
//       DSA + OOP + STL + FILE HANDLING PROJECT
// ============================================================

const string DATA_FILE = "parking_data.txt";
const string QUEUE_FILE = "waiting_queue.txt";
const string TRANSACTION_FILE = "transactions.txt";

// ------------------------------------------------------------
// HELPER FUNCTIONS
// ------------------------------------------------------------

string toUpperStr(string s) {
    for (char& c : s)
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return s;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// '|' is our file delimiter, so it must never appear in a field.
string sanitize(string s) {
    replace(s.begin(), s.end(), '|', '-');
    return trim(s);
}

// ------------------------------------------------------------
// INPUT VALIDATION (uses only <cctype>: isalpha, isdigit, toupper)
// ------------------------------------------------------------

// Collapses runs of spaces into a single space.
string normalizeSpaces(const string& s) {
    string out;
    for (char c : s) {
        if (c == ' ' && !out.empty() && out.back() == ' ')
            continue;
        out += c;
    }
    return trim(out);
}

// Name: alphabets only (single spaces allowed between words).
// No digits, no alphanumeric mix, no special characters.
bool isValidName(const string& name) {
    if (name.empty())
        return false;

    for (char ch : name) {
        unsigned char c = static_cast<unsigned char>(ch);
        if (!isalpha(c) && c != ' ')
            return false;
    }
    return true;
}

// Vehicle number format: 2 letters + 2 digits + 1-2 letters + 4 digits
// Example: UK07AK2349 (10 chars) or DL01A1234 (9 chars)
bool isValidVehicleNumber(const string& v) {
    size_t n = v.size();
    if (n < 9 || n > 10)
        return false;

    size_t i = 0;

    for (; i < 2; i++)                        // state code
        if (!isalpha(static_cast<unsigned char>(v[i])))
            return false;

    for (; i < 4; i++)                        // RTO number
        if (!isdigit(static_cast<unsigned char>(v[i])))
            return false;

    size_t seriesLen = n - 8;                 // 1 or 2 series letters
    for (size_t k = 0; k < seriesLen; k++, i++)
        if (!isalpha(static_cast<unsigned char>(v[i])))
            return false;

    for (; i < n; i++)                        // 4-digit number
        if (!isdigit(static_cast<unsigned char>(v[i])))
            return false;

    return true;
}

vector<string> split(const string& line, char delim) {
    vector<string> parts;
    string item;
    stringstream ss(line);
    while (getline(ss, item, delim))
        parts.push_back(item);
    return parts;
}

// Thread-safe replacement for localtime()
string formatTime(time_t t) {
    tm result{};
#ifdef _WIN32
    localtime_s(&result, &t);
#else
    localtime_r(&t, &result);
#endif
    stringstream ss;
    ss << put_time(&result, "%d-%m-%Y %H:%M:%S");
    return ss.str();
}

// Reads a full line. Throws if input stream is closed (EOF),
// which prevents infinite loops.
string readLine(const string& prompt, bool allowEmpty = false) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line))
            throw runtime_error("Input stream closed.");
        line = trim(line);
        if (!line.empty() || allowEmpty)
            return line;
        cout << "Input cannot be empty.\n";
    }
}

// Reads an integer safely; keeps asking until valid.
int readInt(const string& prompt) {
    while (true) {
        string line = readLine(prompt);
        try {
            size_t pos = 0;
            int value = stoi(line, &pos);
            if (pos != line.size())
                throw invalid_argument("extra characters");
            return value;
        }
        catch (...) {
            cout << "Please enter a valid number.\n";
        }
    }
}

// Keeps asking until the name contains alphabets only.
string readName(const string& prompt) {
    while (true) {
        string name = normalizeSpaces(readLine(prompt));

        if (isValidName(name))
            return name;

        cout << "Invalid name! Use alphabets only "
                "(no numbers or special characters).\n";
    }
}

// Keeps asking until the vehicle number is in a valid format.
// Lower case letters are converted to upper case with toupper().
// Returns "" if the user types 0 to cancel.
string readVehicleNumber(const string& prompt) {
    while (true) {
        string number = toUpperStr(readLine(prompt));

        if (number == "0")
            return "";

        if (isValidVehicleNumber(number))
            return number;

        cout << "Invalid vehicle number! Format: 2 letters, 2 digits, "
                "1-2 letters, 4 digits (e.g. UK07AK2349).\n"
                "Type 0 to cancel.\n";
    }
}

// ------------------------------------------------------------
// ENUMS
// ------------------------------------------------------------

enum class VehicleType {
    BIKE,
    CAR,
    EV
};

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::BIKE: return "BIKE";
        case VehicleType::CAR:  return "CAR";
        case VehicleType::EV:   return "EV";
    }
    return "UNKNOWN";
}

VehicleType stringToVehicleType(const string& type) {
    if (type == "BIKE") return VehicleType::BIKE;
    if (type == "CAR")  return VehicleType::CAR;
    if (type == "EV")   return VehicleType::EV;
    throw invalid_argument("Invalid vehicle type.");
}

// ------------------------------------------------------------
// VEHICLE CLASS
// Demonstrates: Encapsulation, Constructors, Abstraction
// ------------------------------------------------------------

class Vehicle {
private:
    string vehicleNumber;
    string ownerName;
    VehicleType type;
    time_t entryTime;

public:
    Vehicle() : type(VehicleType::CAR), entryTime(time(nullptr)) {}

    Vehicle(string number, string owner, VehicleType vehicleType,
            time_t entry = time(nullptr))
        : vehicleNumber(number), ownerName(owner),
          type(vehicleType), entryTime(entry) {}

    string getVehicleNumber() const { return vehicleNumber; }
    string getOwnerName() const { return ownerName; }
    VehicleType getType() const { return type; }
    time_t getEntryTime() const { return entryTime; }

    void displayVehicle() const {
        cout << "Owner Name     : " << ownerName << endl;
        cout << "Vehicle Type   : " << vehicleTypeToString(type) << endl;
        cout << "Entry Time     : " << formatTime(entryTime) << endl;
    }
};

// ------------------------------------------------------------
// WAITING VEHICLE (stored in the queue)
// ------------------------------------------------------------

struct WaitingVehicle {
    string number;
    string owner;
    VehicleType type;
};

// ------------------------------------------------------------
// BASE CLASS FOR POLYMORPHISM
// ------------------------------------------------------------

class ParkingVehicle {
protected:
    string number;

public:
    ParkingVehicle(string n) : number(n) {}
    virtual double calculateRate(int hours) const = 0;
    virtual string getType() const = 0;
    virtual ~ParkingVehicle() {}
};

// ------------------------------------------------------------
// DERIVED CLASSES - INHERITANCE + POLYMORPHISM
// ------------------------------------------------------------

class Bike : public ParkingVehicle {
public:
    Bike(string n) : ParkingVehicle(n) {}
    double calculateRate(int hours) const override { return hours * 20.0; }
    string getType() const override { return "BIKE"; }
};

class Car : public ParkingVehicle {
public:
    Car(string n) : ParkingVehicle(n) {}
    double calculateRate(int hours) const override { return hours * 40.0; }
    string getType() const override { return "CAR"; }
};

class EV : public ParkingVehicle {
public:
    EV(string n) : ParkingVehicle(n) {}
    double calculateRate(int hours) const override { return hours * 30.0; }
    string getType() const override { return "EV"; }
};

// Factory: picks the right derived class for a vehicle type
unique_ptr<ParkingVehicle> createParkingVehicle(VehicleType type,
                                                const string& number) {
    switch (type) {
        case VehicleType::BIKE: return make_unique<Bike>(number);
        case VehicleType::CAR:  return make_unique<Car>(number);
        case VehicleType::EV:   return make_unique<EV>(number);
    }
    throw invalid_argument("Invalid vehicle type.");
}

// ------------------------------------------------------------
// PARKING SLOT CLASS
// ------------------------------------------------------------

class ParkingSlot {
private:
    int slotID;
    bool occupied;
    string vehicleNumber;
    VehicleType allowedType;

public:
    ParkingSlot()
        : slotID(0), occupied(false), allowedType(VehicleType::CAR) {}

    ParkingSlot(int id, VehicleType type)
        : slotID(id), occupied(false), vehicleNumber(""), allowedType(type) {}

    int getSlotID() const { return slotID; }
    bool isOccupied() const { return occupied; }
    string getVehicleNumber() const { return vehicleNumber; }
    VehicleType getAllowedType() const { return allowedType; }

    void occupy(const string& vehicleNo) {
        if (occupied)
            throw runtime_error("Slot is already occupied.");
        occupied = true;
        vehicleNumber = vehicleNo;
    }

    void release() {
        occupied = false;
        vehicleNumber = "";
    }

    void displaySlot() const {
        cout << "Slot " << setw(2) << slotID << " | ";
        if (occupied)
            cout << "[ OCCUPIED ] | " << setw(12) << vehicleNumber;
        else
            cout << "[   EMPTY  ] | " << setw(12) << "-";
        cout << " | " << vehicleTypeToString(allowedType) << endl;
    }
};

// ------------------------------------------------------------
// TRANSACTION CLASS (file handling / history)
// ------------------------------------------------------------

class ParkingTransaction {
private:
    string vehicleNumber;
    int slotID;
    string action;
    double fee;
    string dateTime;

public:
    ParkingTransaction() : slotID(0), fee(0) {}

    // New transaction (timestamp = now)
    ParkingTransaction(string vehicle, int slot, string act, double amount)
        : vehicleNumber(vehicle), slotID(slot), action(act), fee(amount),
          dateTime(formatTime(time(nullptr))) {}

    // Transaction restored from file (keeps the original timestamp)
    ParkingTransaction(string vehicle, int slot, string act, double amount,
                       string when)
        : vehicleNumber(vehicle), slotID(slot), action(act), fee(amount),
          dateTime(when) {}

    void display() const {
        cout << left
             << setw(15) << vehicleNumber
             << setw(10) << slotID
             << setw(15) << action
             << setw(12) << fixed << setprecision(2) << fee
             << dateTime << endl;
    }

    string toFileString() const {
        stringstream ss;
        ss << vehicleNumber << "|" << slotID << "|" << action << "|"
           << fixed << setprecision(2) << fee << "|" << dateTime;
        return ss.str();
    }

    static ParkingTransaction fromFileString(const string& line) {
        vector<string> p = split(line, '|');
        if (p.size() < 5)
            throw invalid_argument("Bad transaction record.");
        return ParkingTransaction(p[0], stoi(p[1]), p[2], stod(p[3]), p[4]);
    }
};

// ------------------------------------------------------------
// PARKING MANAGER - MAIN CLASS
//
// DS USED: vector, queue, unordered_map, stack
// ------------------------------------------------------------

class ParkingManager {
private:
    vector<ParkingSlot> slots;                    // all parking slots
    unordered_map<string, int> vehicleSlotMap;    // plate -> slot (avg O(1))
    unordered_map<string, Vehicle> vehicles;      // plate -> vehicle record
    queue<WaitingVehicle> waitingQueue;           // vehicles waiting for a slot
    stack<string> historyStack;                   // recent actions
    vector<ParkingTransaction> transactions;      // transaction history

    // ----------------------------------------------------
    // INTERNAL HELPERS
    // ----------------------------------------------------

    // Slot IDs are 1..n, so slot lookup is O(1).
    ParkingSlot* slotById(int id) {
        if (id < 1 || id > static_cast<int>(slots.size()))
            return nullptr;
        return &slots[id - 1];
    }

    bool inWaitingQueue(const string& number) const {
        queue<WaitingVehicle> temp = waitingQueue;
        while (!temp.empty()) {
            if (temp.front().number == number)
                return true;
            temp.pop();
        }
        return false;
    }

    // Places a vehicle into a slot and updates every structure.
    void assignSlot(const string& number, const string& owner,
                    VehicleType type, int slotID) {
        ParkingSlot* slot = slotById(slotID);
        if (!slot)
            throw runtime_error("Invalid slot.");

        slot->occupy(number);
        vehicles[number] = Vehicle(number, owner, type, time(nullptr));
        vehicleSlotMap[number] = slotID;

        historyStack.push("PARKED " + number + " AT SLOT " +
                          to_string(slotID));
        transactions.push_back(
            ParkingTransaction(number, slotID, "PARK", 0));
    }

public:

    // --------------------------------------------------------
    // CONSTRUCTOR
    // Layout scales with totalSlots: ~50% CAR, ~25% BIKE, rest EV
    // (20 slots -> 10 CAR, 5 BIKE, 5 EV, same as before)
    // --------------------------------------------------------

    ParkingManager(int totalSlots = 20) {
        if (totalSlots <= 0)
            throw invalid_argument("Total slots must be positive.");

        int carSlots = max(1, totalSlots / 2);
        int bikeSlots = max(1, totalSlots / 4);

        for (int i = 1; i <= totalSlots; i++) {
            if (i <= carSlots)
                slots.emplace_back(i, VehicleType::CAR);
            else if (i <= carSlots + bikeSlots)
                slots.emplace_back(i, VehicleType::BIKE);
            else
                slots.emplace_back(i, VehicleType::EV);
        }

        loadData();
    }

    // --------------------------------------------------------
    // CHECKS / SEARCH
    // --------------------------------------------------------

    bool vehicleExists(const string& number) const {
        return vehicleSlotMap.find(number) != vehicleSlotMap.end();
    }

    // Linear search: O(n)
    int findAvailableSlot(VehicleType type) const {
        for (const auto& slot : slots) {
            if (!slot.isOccupied() && slot.getAllowedType() == type)
                return slot.getSlotID();
        }
        return -1;
    }

    // --------------------------------------------------------
    // PARK VEHICLE
    // --------------------------------------------------------

    void parkVehicle() {
        cout << "\n========== PARK VEHICLE ==========\n";

        string number = readVehicleNumber(
            "Enter vehicle number (e.g. UK07AK2349, 0 to cancel): ");

        if (number.empty()) {
            cout << "Cancelled.\n";
            return;
        }

        if (vehicleExists(number)) {
            cout << "\nERROR: Vehicle is already parked!\n";
            return;
        }

        if (inWaitingQueue(number)) {
            cout << "\nERROR: Vehicle is already in the waiting queue!\n";
            return;
        }

        string owner = readName("Enter owner name (alphabets only): ");

        cout << "\nVehicle Type:\n";
        cout << "1. Bike\n";
        cout << "2. Car\n";
        cout << "3. EV\n";
        int typeChoice = readInt("Enter choice: ");

        VehicleType type = VehicleType::CAR;   // always initialised

        if (typeChoice == 1)
            type = VehicleType::BIKE;
        else if (typeChoice == 2)
            type = VehicleType::CAR;
        else if (typeChoice == 3)
            type = VehicleType::EV;
        else {
            cout << "\nERROR: Invalid vehicle type.\n";
            return;
        }

        int slotID = findAvailableSlot(type);

        if (slotID == -1) {
            cout << "\nNo suitable parking slot available.\n";
            cout << "Vehicle added to waiting queue.\n";
            waitingQueue.push({number, owner, type});
            saveData();
            return;
        }

        assignSlot(number, owner, type, slotID);
        saveData();

        cout << "\n==================================\n";
        cout << "Vehicle parked successfully!\n";
        cout << "Vehicle Number : " << number << endl;
        cout << "Assigned Slot  : " << slotID << endl;
        cout << "==================================\n";
    }

    // --------------------------------------------------------
    // RELEASE VEHICLE
    // Fee uses real elapsed time unless a simulated duration
    // is entered (handy for demos, since real time is ~0).
    // --------------------------------------------------------

    void releaseVehicle() {
        cout << "\n========== RELEASE VEHICLE ==========\n";

        string number = readVehicleNumber(
            "Enter vehicle number (0 to cancel): ");

        if (number.empty()) {
            cout << "Cancelled.\n";
            return;
        }

        auto it = vehicleSlotMap.find(number);
        if (it == vehicleSlotMap.end()) {
            cout << "\nERROR: Vehicle not found.\n";
            return;
        }

        int slotID = it->second;
        const Vehicle& vehicle = vehicles.at(number);
        VehicleType type = vehicle.getType();

        // Automatically calculate actual parking duration
time_t exitTime = time(nullptr);

double elapsedSeconds =
    difftime(exitTime, vehicle.getEntryTime());

// Calculate duration in hours (round up)
int hours = max(1, static_cast<int>(
    ceil(elapsedSeconds / 3600.0)
));

        // Polymorphism
        unique_ptr<ParkingVehicle> parkingVehicle =
            createParkingVehicle(type, number);
        double fee = parkingVehicle->calculateRate(hours);

        // Release slot
        if (ParkingSlot* slot = slotById(slotID))
            slot->release();

        vehicleSlotMap.erase(number);
        vehicles.erase(number);

        historyStack.push("RELEASED " + number + " FROM SLOT " +
                          to_string(slotID));
        transactions.push_back(
            ParkingTransaction(number, slotID, "RELEASE", fee));

        cout << "\n==================================\n";
        cout << "Vehicle released successfully!\n";
        cout << "Vehicle Number : " << number << endl;
        cout << "Slot           : " << slotID << endl;
        cout << "Duration       : " << hours << " hour(s)\n";
        cout << "Parking Fee    : Rs. " << fixed << setprecision(2)
             << fee << endl;
        cout << "==================================\n";

        // Give the freed slot to a waiting vehicle (if any matches)
        processWaitingQueue(type);

        saveData();
    }

    // --------------------------------------------------------
    // SEARCH VEHICLE
    // --------------------------------------------------------

    void searchVehicle() {
        cout << "\n========== SEARCH VEHICLE ==========\n";

        string number = readVehicleNumber(
            "Enter vehicle number (0 to cancel): ");

        if (number.empty()) {
            cout << "Cancelled.\n";
            return;
        }

        auto it = vehicleSlotMap.find(number);

        if (it == vehicleSlotMap.end()) {
            if (inWaitingQueue(number))
                cout << "\nVehicle is in the waiting queue.\n";
            else
                cout << "\nVehicle not found.\n";
            return;
        }

        cout << "\nVehicle Found!\n";
        cout << "Vehicle Number : " << number << endl;
        cout << "Parking Slot   : " << it->second << endl;
        vehicles.at(number).displayVehicle();
    }

    // --------------------------------------------------------
    // DISPLAY PARKING LOT
    // --------------------------------------------------------

    void displayParkingLot() {
        cout << "\n============================================\n";
        cout << "              PARKING LOT\n";
        cout << "============================================\n";

        for (const auto& slot : slots)
            slot.displaySlot();

        cout << "============================================\n";

        showStatistics();
    }

    // --------------------------------------------------------
    // DISPLAY AVAILABLE SLOTS
    // --------------------------------------------------------

    void displayAvailableSlots() {
        cout << "\n========== AVAILABLE SLOTS ==========\n";

        bool found = false;

        for (const auto& slot : slots) {
            if (!slot.isOccupied()) {
                cout << "Slot " << slot.getSlotID() << " -> "
                     << vehicleTypeToString(slot.getAllowedType()) << endl;
                found = true;
            }
        }

        if (!found)
            cout << "No slots available.\n";
    }

    // --------------------------------------------------------
    // DISPLAY WAITING QUEUE
    // --------------------------------------------------------

    void displayWaitingQueue() {
        cout << "\n========== WAITING QUEUE ==========\n";

        if (waitingQueue.empty()) {
            cout << "Waiting queue is empty.\n";
            return;
        }

        queue<WaitingVehicle> temp = waitingQueue;
        int position = 1;

        while (!temp.empty()) {
            const WaitingVehicle& w = temp.front();
            cout << position++ << ". " << w.number
                 << " (" << w.owner << ", "
                 << vehicleTypeToString(w.type) << ")" << endl;
            temp.pop();
        }
    }

    // --------------------------------------------------------
    // PROCESS WAITING QUEUE
    // Parks the FIRST waiting vehicle whose type matches the
    // type of the slot that was just freed. Others keep their
    // relative order.
    // --------------------------------------------------------

    void processWaitingQueue(VehicleType freedType) {
        if (waitingQueue.empty())
            return;

        queue<WaitingVehicle> remaining;
        bool parked = false;

        while (!waitingQueue.empty()) {
            WaitingVehicle w = waitingQueue.front();
            waitingQueue.pop();

            if (!parked && w.type == freedType) {
                int slotID = findAvailableSlot(w.type);

                if (slotID != -1) {
                    assignSlot(w.number, w.owner, w.type, slotID);
                    parked = true;

                    cout << "\nWaiting vehicle " << w.number
                         << " has been parked at slot " << slotID << ".\n";
                    continue;
                }
            }

            remaining.push(w);
        }

        waitingQueue = remaining;
    }

    // --------------------------------------------------------
    // SHOW STATISTICS
    // --------------------------------------------------------

    void showStatistics() {
        int occupied = 0;
        int available = 0;

        for (const auto& slot : slots) {
            if (slot.isOccupied())
                occupied++;
            else
                available++;
        }

        cout << "\nTotal Slots     : " << slots.size();
        cout << "\nOccupied Slots  : " << occupied;
        cout << "\nAvailable Slots : " << available;
        cout << "\nWaiting Vehicles: " << waitingQueue.size() << endl;
    }

    // --------------------------------------------------------
    // SORT VEHICLES BY VEHICLE NUMBER (STL sort)
    // --------------------------------------------------------

    void sortVehicles() {
        if (vehicles.empty()) {
            cout << "\nNo vehicles currently parked.\n";
            return;
        }

        vector<string> vehicleNumbers;
        for (const auto& pair : vehicles)
            vehicleNumbers.push_back(pair.first);

        sort(vehicleNumbers.begin(), vehicleNumbers.end());

        cout << "\n========== SORTED VEHICLES ==========\n";

        for (const string& number : vehicleNumbers)
            cout << number << " -> Slot " << vehicleSlotMap.at(number) << endl;
    }

    // --------------------------------------------------------
    // SHOW RECENT ACTIONS (STACK)
    // Prints the last 5 actions, most recent first.
    // --------------------------------------------------------

    void showLastAction() {
        cout << "\n========== RECENT ACTIONS ==========\n";

        if (historyStack.empty()) {
            cout << "No actions recorded.\n";
            return;
        }

        stack<string> temp = historyStack;
        int count = 0;

        while (!temp.empty() && count < 5) {
            cout << (count + 1) << ". " << temp.top() << endl;
            temp.pop();
            count++;
        }
    }

    // --------------------------------------------------------
    // DISPLAY TRANSACTION HISTORY
    // --------------------------------------------------------

    void displayTransactions() {
        cout << "\n================ TRANSACTION HISTORY ================\n";

        if (transactions.empty()) {
            cout << "No transactions recorded.\n";
            return;
        }

        cout << left
             << setw(15) << "Vehicle"
             << setw(10) << "Slot"
             << setw(15) << "Action"
             << setw(12) << "Fee"
             << "Date/Time" << endl;

        cout << "------------------------------------------------------\n";

        for (const auto& transaction : transactions)
            transaction.display();
    }

    // --------------------------------------------------------
    // FILE HANDLING - SAVE
    // parking_data.txt : number|owner|type|slot|entryTime
    // waiting_queue.txt: number|owner|type
    // transactions.txt : vehicle|slot|action|fee|dateTime
    // --------------------------------------------------------

    void saveData() {
        // Parked vehicles
        ofstream file(DATA_FILE);
        if (!file) {
            cerr << "Error opening " << DATA_FILE << "\n";
        }
        else {
            for (const auto& pair : vehicleSlotMap) {
                const Vehicle& v = vehicles.at(pair.first);

                file << pair.first << "|"
                     << v.getOwnerName() << "|"
                     << vehicleTypeToString(v.getType()) << "|"
                     << pair.second << "|"
                     << static_cast<long long>(v.getEntryTime()) << "\n";
            }
        }

        // Waiting queue
        ofstream qFile(QUEUE_FILE);
        if (!qFile) {
            cerr << "Error opening " << QUEUE_FILE << "\n";
        }
        else {
            queue<WaitingVehicle> temp = waitingQueue;
            while (!temp.empty()) {
                const WaitingVehicle& w = temp.front();
                qFile << w.number << "|" << w.owner << "|"
                      << vehicleTypeToString(w.type) << "\n";
                temp.pop();
            }
        }

        // Transactions
        ofstream tFile(TRANSACTION_FILE);
        if (!tFile) {
            cerr << "Error opening " << TRANSACTION_FILE << "\n";
        }
        else {
            for (const auto& t : transactions)
                tFile << t.toFileString() << "\n";
        }
    }

    // --------------------------------------------------------
    // FILE HANDLING - LOAD
    // Every record is validated; bad records are skipped so the
    // maps and slots can never become inconsistent.
    // --------------------------------------------------------

    void loadData() {
        // ---- Parked vehicles ----
        ifstream file(DATA_FILE);
        string line;

        while (file && getline(file, line)) {
            if (trim(line).empty())
                continue;

            try {
                vector<string> p = split(line, '|');
                if (p.size() < 4)
                    throw invalid_argument("Too few fields");

                string number = p[0];
                string owner = p[1];
                VehicleType type = stringToVehicleType(p[2]);
                int slotID = stoi(p[3]);

                time_t entry = time(nullptr);   // fallback for old files
                if (p.size() >= 5 && !p[4].empty())
                    entry = static_cast<time_t>(stoll(p[4]));

                ParkingSlot* slot = slotById(slotID);

                if (number.empty() || owner.empty())
                    throw invalid_argument("Empty field");
                if (!slot)
                    throw invalid_argument("Slot out of range");
                if (slot->isOccupied())
                    throw invalid_argument("Slot already occupied");
                if (slot->getAllowedType() != type)
                    throw invalid_argument("Slot type mismatch");
                if (vehicleSlotMap.count(number))
                    throw invalid_argument("Duplicate vehicle");

                slot->occupy(number);
                vehicles[number] = Vehicle(number, owner, type, entry);
                vehicleSlotMap[number] = slotID;
            }
            catch (...) {
                cout << "Invalid record skipped.\n";
            }
        }
        file.close();

        // ---- Waiting queue ----
        ifstream qFile(QUEUE_FILE);

        while (qFile && getline(qFile, line)) {
            if (trim(line).empty())
                continue;

            try {
                vector<string> p = split(line, '|');
                if (p.size() < 3 || p[0].empty())
                    throw invalid_argument("Bad queue record");

                if (vehicleSlotMap.count(p[0]) || inWaitingQueue(p[0]))
                    throw invalid_argument("Duplicate vehicle");

                waitingQueue.push({p[0], p[1], stringToVehicleType(p[2])});
            }
            catch (...) {
                cout << "Invalid queue record skipped.\n";
            }
        }
        qFile.close();

        // ---- Transactions ----
        ifstream tFile(TRANSACTION_FILE);

        while (tFile && getline(tFile, line)) {
            if (trim(line).empty())
                continue;

            try {
                transactions.push_back(
                    ParkingTransaction::fromFileString(line));
            }
            catch (...) {
                cout << "Invalid transaction record skipped.\n";
            }
        }
        tFile.close();
    }

    // --------------------------------------------------------
    // RESET PARKING LOT
    // --------------------------------------------------------

    void resetParking() {
        string choice = readLine(
            "\nAre you sure you want to reset the entire parking lot? (Y/N): ");

        if (choice != "Y" && choice != "y") {
            cout << "Reset cancelled.\n";
            return;
        }

        for (auto& slot : slots)
            slot.release();

        vehicleSlotMap.clear();
        vehicles.clear();

        while (!waitingQueue.empty())
            waitingQueue.pop();

        while (!historyStack.empty())
            historyStack.pop();

        transactions.clear();

        saveData();   // writes three empty files

        cout << "\nParking lot has been reset.\n";
    }

    // --------------------------------------------------------
    // MAIN MENU
    // --------------------------------------------------------

    void menu() {
        int choice = -1;

        do {
            cout << "\n\n";
            cout << "==============================================\n";
            cout << "          SMART PARKING SIMULATOR\n";
            cout << "==============================================\n";

            cout << "1.  Park Vehicle\n";
            cout << "2.  Release Vehicle\n";
            cout << "3.  Search Vehicle\n";
            cout << "4.  Display Parking Lot\n";
            cout << "5.  Show Available Slots\n";
            cout << "6.  Show Waiting Queue\n";
            cout << "7.  Show Statistics\n";
            cout << "8.  Sort Vehicles\n";
            cout << "9.  Show Recent Actions\n";
            cout << "10. Transaction History\n";
            cout << "11. Reset Parking Lot\n";
            cout << "0.  Exit\n";

            cout << "==============================================\n";

            choice = readInt("Enter your choice: ");

            switch (choice) {
                case 1:  parkVehicle();          break;
                case 2:  releaseVehicle();       break;
                case 3:  searchVehicle();        break;
                case 4:  displayParkingLot();    break;
                case 5:  displayAvailableSlots();break;
                case 6:  displayWaitingQueue();  break;
                case 7:  showStatistics();       break;
                case 8:  sortVehicles();         break;
                case 9:  showLastAction();       break;
                case 10: displayTransactions();  break;
                case 11: resetParking();         break;
                case 0:
                    cout << "\nThank you for using "
                         << "Smart Parking Simulator!\n";
                    break;
                default:
                    cout << "\nInvalid choice. Please try again.\n";
            }

        } while (choice != 0);
    }
};

// ============================================================
//                         MAIN
// ============================================================

int main() {
    try {
        ParkingManager parking(20);
        parking.menu();
    }
    catch (const exception& e) {
        cerr << "\nFatal Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}