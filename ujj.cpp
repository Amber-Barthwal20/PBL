#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

const string DATA = "parking_data.txt";
const string QUEUE = "waiting_queue.txt";
const string TRANS = "transactions.txt";

struct Record {
    string number, owner, type;
    int slot;

    string toString() {
        return number + "|" + owner + "|" + type + "|" + to_string(slot);
    }

    void display() {
        cout << left << setw(15) << number
             << setw(15) << owner
             << setw(10) << type
             << slot << endl;
    }
};

class FileHandling {
public:
    void createFiles() {
        ofstream(DATA, ios::app).close();
        ofstream(QUEUE, ios::app).close();
        ofstream(TRANS, ios::app).close();
        cout << "Files created successfully.\n";
    }

    void writeData() {
        ofstream file(DATA);
        if (!file) {
            cout << "File opening error.\n";
            return;
        }

        int n;
        cout << "Enter number of vehicles: ";
        cin >> n;

        for (int i = 0; i < n; i++) {
            Record r;
            cout << "Vehicle Number: ";
            cin >> r.number;
            cout << "Owner Name: ";
            cin >> r.owner;
            cout << "Vehicle Type: ";
            cin >> r.type;
            cout << "Slot ID: ";
            cin >> r.slot;
            file << r.toString() << endl;
        }

        file.close();
        cout << "Data saved successfully.\n";
    }

    void readData() {
        ifstream file(DATA);
        string line;

        if (!file) {
            cout << "File not found.\n";
            return;
        }

        cout << "\nVehicle Records:\n";
        while (getline(file, line)) {
            stringstream ss(line);
            Record r;
            string slot;
            getline(ss, r.number, '|');
            getline(ss, r.owner, '|');
            getline(ss, r.type, '|');
            getline(ss, slot);
            try {
                r.slot = stoi(slot);
                r.display();
            } catch (...) {
                cout << "Invalid record skipped.\n";
            }
        }
        file.close();
    }

    void appendData() {
        ofstream file(DATA, ios::app);
        Record r;

        cout << "Vehicle Number: ";
        cin >> r.number;
        cout << "Owner Name: ";
        cin >> r.owner;
        cout << "Vehicle Type: ";
        cin >> r.type;
        cout << "Slot ID: ";
        cin >> r.slot;

        file << r.toString() << endl;
        file.close();
        cout << "Record appended successfully.\n";
    }

    void searchData() {
        ifstream file(DATA);
        string number, line;
        bool found = false;

        cout << "Enter vehicle number: ";
        cin >> number;

        while (getline(file, line)) {
            stringstream ss(line);
            Record r;
            string slot;
            getline(ss, r.number, '|');
            getline(ss, r.owner, '|');
            getline(ss, r.type, '|');
            getline(ss, slot);

            if (r.number == number) {
                r.slot = stoi(slot);
                r.display();
                found = true;
                break;
            }
        }

        if (!found) cout << "Vehicle not found.\n";
        file.close();
    }

    void deleteData() {
        ifstream file(DATA);
        ofstream temp("temp.txt");
        string number, line;
        bool found = false;

        cout << "Enter vehicle number to delete: ";
        cin >> number;

        while (getline(file, line)) {
            if (line.substr(0, line.find('|')) == number) {
                found = true;
                continue;
            }
            temp << line << endl;
        }

        file.close();
        temp.close();

        remove(DATA.c_str());
        rename("temp.txt", DATA.c_str());

        cout << (found ? "Record deleted.\n" : "Record not found.\n");
    }

    void appendQueue() {
        ofstream file(QUEUE, ios::app);
        Record r;

        cout << "Vehicle Number: ";
        cin >> r.number;
        cout << "Owner Name: ";
        cin >> r.owner;
        cout << "Vehicle Type: ";
        cin >> r.type;

        file << r.number << "|" << r.owner << "|" << r.type << endl;
        file.close();
        cout << "Waiting record saved.\n";
    }

    void readQueue() {
        ifstream file(QUEUE);
        string line;

        cout << "\nWaiting Queue:\n";
        while (getline(file, line))
            cout << line << endl;

        file.close();
    }

    void appendTransaction() {
        ofstream file(TRANS, ios::app);
        string number, action;
        int slot;
        double fee;

        cout << "Vehicle Number: ";
        cin >> number;
        cout << "Slot ID: ";
        cin >> slot;
        cout << "Action (PARK/RELEASE): ";
        cin >> action;
        cout << "Fee: ";
        cin >> fee;

        file << number << "|" << slot << "|" << action
             << "|" << fixed << setprecision(2) << fee << endl;

        file.close();
        cout << "Transaction saved.\n";
    }

    void readTransactions() {
        ifstream file(TRANS);
        string line;

        cout << "\nTransaction History:\n";
        while (getline(file, line))
            cout << line << endl;

        file.close();
    }

    void resetFiles() {
        ofstream(DATA, ios::trunc).close();
        ofstream(QUEUE, ios::trunc).close();
        ofstream(TRANS, ios::trunc).close();
        cout << "All files reset successfully.\n";
    }

    void menu() {
        int choice;

        do {
            cout << "\n===== PARKING FILE HANDLING =====\n";
            cout << "1. Create Files\n";
            cout << "2. Write Parking Data\n";
            cout << "3. Read Parking Data\n";
            cout << "4. Append Parking Data\n";
            cout << "5. Search Vehicle\n";
            cout << "6. Delete Vehicle Record\n";
            cout << "7. Append Waiting Record\n";
            cout << "8. Read Waiting Queue\n";
            cout << "9. Append Transaction\n";
            cout << "10. Read Transactions\n";
            cout << "11. Reset Files\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input.\n";
                continue;
            }

            switch (choice) {
                case 1: createFiles(); break;
                case 2: writeData(); break;
                case 3: readData(); break;
                case 4: appendData(); break;
                case 5: searchData(); break;
                case 6: deleteData(); break;
                case 7: appendQueue(); break;
                case 8: readQueue(); break;
                case 9: appendTransaction(); break;
                case 10: readTransactions(); break;
                case 11: resetFiles(); break;
                case 0: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

int main() {
    FileHandling parking;
    parking.menu();
    return 0;
}
