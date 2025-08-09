// hospital_management.cpp
// Full hospital management system for a beginner GitHub project.
// Features: Patients, Doctors, Appointments, Billing, Reports.
// Simple file persistence for patients, doctors, appointments, bills.
// Compile: g++ hospital_management.cpp -o hospital_management

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <climits>   // for INT_MIN/INT_MAX
using namespace std;

// ---------------------- Data Structures ----------------------
struct Patient {
    int id;
    string name;
    int age;
    string disease;
};

struct Doctor {
    int id;
    string name;
    string specialization;
    string contact;
};

struct Appointment {
    int id;
    int patientId;
    int doctorId;
    string date; // simple string like "2025-08-10 14:00"
};

struct Bill {
    int id;
    int patientId;
    double amount;
    string details;
};

// ---------------------- Globals ----------------------
vector<Patient> patients;
vector<Doctor> doctors;
vector<Appointment> appointments;
vector<Bill> bills;

int nextPatientId = 1;
int nextDoctorId = 1;
int nextAppointmentId = 1;
int nextBillId = 1;

const string PATIENTS_FILE = "patients.txt";
const string DOCTORS_FILE  = "doctors.txt";
const string APPOINTS_FILE = "appointments.txt";
const string BILLS_FILE    = "bills.txt";

// ---------------------- Helpers: Input Validation ----------------------
int getValidatedInt(const string &prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int x;
    while (true) {
        cout << prompt;
        if (!(cin >> x)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x < minVal || x > maxVal) {
            cout << "Please enter a number between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush rest of line
        return x;
    }
}

double getValidatedDouble(const string &prompt, double minVal = -1e308, double maxVal = 1e308) {
    double x;
    while (true) {
        cout << prompt;
        if (!(cin >> x)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x < minVal || x > maxVal) {
            cout << "Please enter a number between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return x;
    }
}

string getValidatedLine(const string &prompt) {
    string s;
    while (true) {
        cout << prompt;
        std::getline(cin, s);
        if (s.empty()) {
            cout << "This field cannot be empty. Please try again.\n";
            continue;
        }
        return s;
    }
}

// ---------------------- Persistence Helpers ----------------------
// We'll use '|' as a simple field separator to allow spaces in fields.

void savePatients() {
    ofstream f(PATIENTS_FILE);
    for (auto &p : patients) {
        // id|name|age|disease
        f << p.id << '|' << p.name << '|' << p.age << '|' << p.disease << '\n';
    }
}

void loadPatients() {
    patients.clear();
    ifstream f(PATIENTS_FILE);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Patient p;
        string ageStr;
        getline(ss, line, '|'); p.id = stoi(line);
        getline(ss, p.name, '|');
        getline(ss, ageStr, '|'); p.age = stoi(ageStr);
        getline(ss, p.disease);
        patients.push_back(p);
        if (p.id >= nextPatientId) nextPatientId = p.id + 1;
    }
}

void saveDoctors() {
    ofstream f(DOCTORS_FILE);
    for (auto &d : doctors) {
        // id|name|specialization|contact
        f << d.id << '|' << d.name << '|' << d.specialization << '|' << d.contact << '\n';
    }
}

void loadDoctors() {
    doctors.clear();
    ifstream f(DOCTORS_FILE);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Doctor d;
        getline(ss, line, '|'); d.id = stoi(line);
        getline(ss, d.name, '|');
        getline(ss, d.specialization, '|');
        getline(ss, d.contact);
        doctors.push_back(d);
        if (d.id >= nextDoctorId) nextDoctorId = d.id + 1;
    }
}

void saveAppointments() {
    ofstream f(APPOINTS_FILE);
    for (auto &a : appointments) {
        // id|patientId|doctorId|date
        f << a.id << '|' << a.patientId << '|' << a.doctorId << '|' << a.date << '\n';
    }
}

void loadAppointments() {
    appointments.clear();
    ifstream f(APPOINTS_FILE);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Appointment a;
        getline(ss, line, '|'); a.id = stoi(line);
        getline(ss, line, '|'); a.patientId = stoi(line);
        getline(ss, line, '|'); a.doctorId = stoi(line);
        getline(ss, a.date);
        appointments.push_back(a);
        if (a.id >= nextAppointmentId) nextAppointmentId = a.id + 1;
    }
}

void saveBills() {
    ofstream f(BILLS_FILE);
    for (auto &b : bills) {
        // id|patientId|amount|details
        f << b.id << '|' << b.patientId << '|' << b.amount << '|' << b.details << '\n';
    }
}

void loadBills() {
    bills.clear();
    ifstream f(BILLS_FILE);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Bill b;
        string amt;
        getline(ss, line, '|'); b.id = stoi(line);
        getline(ss, line, '|'); b.patientId = stoi(line);
        getline(ss, amt, '|'); b.amount = stod(amt);
        getline(ss, b.details);
        bills.push_back(b);
        if (b.id >= nextBillId) nextBillId = b.id + 1;
    }
}

void saveAll() {
    savePatients();
    saveDoctors();
    saveAppointments();
    saveBills();
}

void loadAll() {
    loadPatients();
    loadDoctors();
    loadAppointments();
    loadBills();
}

// ---------------------- Patient Management ----------------------
void addPatient() {
    Patient p;
    p.id = nextPatientId++;
    p.name = getValidatedLine("Enter patient name: ");
    p.age = getValidatedInt("Enter patient age: ", 0, 120);
    p.disease = getValidatedLine("Enter disease/diagnosis: ");
    patients.push_back(p);
    cout << "Patient added (ID = " << p.id << ").\n";
}

void displayPatients() {
    if (patients.empty()) {
        cout << "No patients to display.\n";
        return;
    }
    cout << "\n--- Patients ---\n";
    for (auto &p : patients) {
        cout << "ID: " << p.id << " | Name: " << p.name << " | Age: " << p.age
             << " | Disease: " << p.disease << '\n';
    }
}

Patient* findPatientById(int id) {
    for (auto &p : patients) if (p.id == id) return &p;
    return nullptr;
}

void searchPatient() {
    int id = getValidatedInt("Enter patient ID to search: ", 1);
    Patient* p = findPatientById(id);
    if (!p) { cout << "Patient not found.\n"; return; }
    cout << "Found: ID: " << p->id << " | Name: " << p->name << " | Age: "
         << p->age << " | Disease: " << p->disease << '\n';
}

void updatePatient() {
    int id = getValidatedInt("Enter patient ID to update: ", 1);
    Patient* p = findPatientById(id);
    if (!p) { cout << "Patient not found.\n"; return; }
    p->name = getValidatedLine("Enter new name: ");
    p->age = getValidatedInt("Enter new age: ", 0, 120);
    p->disease = getValidatedLine("Enter new disease: ");
    cout << "Patient updated.\n";
}

void deletePatient() {
    int id = getValidatedInt("Enter patient ID to delete: ", 1);
    for (auto it = patients.begin(); it != patients.end(); ++it) {
        if (it->id == id) {
            patients.erase(it);
            cout << "Patient deleted.\n";
            return;
        }
    }
    cout << "Patient not found.\n";
}

// ---------------------- Doctor Management ----------------------
void addDoctor() {
    Doctor d;
    d.id = nextDoctorId++;
    d.name = getValidatedLine("Enter doctor name: ");
    d.specialization = getValidatedLine("Enter specialization: ");
    d.contact = getValidatedLine("Enter contact number: ");
    doctors.push_back(d);
    cout << "Doctor added (ID = " << d.id << ").\n";
}

void displayDoctors() {
    if (doctors.empty()) {
        cout << "No doctors to display.\n";
        return;
    }
    cout << "\n--- Doctors ---\n";
    for (auto &d : doctors) {
        cout << "ID: " << d.id << " | Name: " << d.name
             << " | Specialization: " << d.specialization
             << " | Contact: " << d.contact << '\n';
    }
}

Doctor* findDoctorById(int id) {
    for (auto &d : doctors) if (d.id == id) return &d;
    return nullptr;
}

void searchDoctor() {
    int id = getValidatedInt("Enter doctor ID to search: ", 1);
    Doctor* d = findDoctorById(id);
    if (!d) { cout << "Doctor not found.\n"; return; }
    cout << "Found: ID: " << d->id << " | Name: " << d->name
         << " | Specialization: " << d->specialization << " | Contact: " << d->contact << '\n';
}

void updateDoctor() {
    int id = getValidatedInt("Enter doctor ID to update: ", 1);
    Doctor* d = findDoctorById(id);
    if (!d) { cout << "Doctor not found.\n"; return; }
    d->name = getValidatedLine("Enter new name: ");
    d->specialization = getValidatedLine("Enter new specialization: ");
    d->contact = getValidatedLine("Enter new contact: ");
    cout << "Doctor updated.\n";
}

void deleteDoctor() {
    int id = getValidatedInt("Enter doctor ID to delete: ", 1);
    for (auto it = doctors.begin(); it != doctors.end(); ++it) {
        if (it->id == id) {
            doctors.erase(it);
            cout << "Doctor deleted.\n";
            return;
        }
    }
    cout << "Doctor not found.\n";
}

// ---------------------- Appointment Booking ----------------------
void bookAppointment() {
    if (patients.empty()) { cout << "No patients available. Add patients first.\n"; return; }
    if (doctors.empty())  { cout << "No doctors available. Add doctors first.\n"; return; }

    int pid = getValidatedInt("Enter patient ID: ", 1);
    if (!findPatientById(pid)) { cout << "Patient not found.\n"; return; }
    int did = getValidatedInt("Enter doctor ID: ", 1);
    if (!findDoctorById(did)) { cout << "Doctor not found.\n"; return; }
    string date = getValidatedLine("Enter appointment date/time (e.g. 2025-08-10 14:00): ");

    Appointment a;
    a.id = nextAppointmentId++;
    a.patientId = pid;
    a.doctorId = did;
    a.date = date;
    appointments.push_back(a);
    cout << "Appointment booked (ID = " << a.id << ").\n";
}

void listAppointments() {
    if (appointments.empty()) { cout << "No appointments.\n"; return; }
    cout << "\n--- Appointments ---\n";
    for (auto &a : appointments) {
        cout << "ID: " << a.id
             << " | PatientID: " << a.patientId
             << " | DoctorID: " << a.doctorId
             << " | Date: " << a.date << '\n';
    }
}

void listAppointmentsForPatient() {
    int pid = getValidatedInt("Enter patient ID: ", 1);
    bool any = false;
    for (auto &a : appointments) {
        if (a.patientId == pid) {
            cout << "Appointment ID: " << a.id << " | DoctorID: " << a.doctorId << " | Date: " << a.date << '\n';
            any = true;
        }
    }
    if (!any) cout << "No appointments for this patient.\n";
}

void listAppointmentsForDoctor() {
    int did = getValidatedInt("Enter doctor ID: ", 1);
    bool any = false;
    for (auto &a : appointments) {
        if (a.doctorId == did) {
            cout << "Appointment ID: " << a.id << " | PatientID: " << a.patientId << " | Date: " << a.date << '\n';
            any = true;
        }
    }
    if (!any) cout << "No appointments for this doctor.\n";
}

// ---------------------- Billing ----------------------
void createBill() {
    int pid = getValidatedInt("Enter patient ID for billing: ", 1);
    if (!findPatientById(pid)) { cout << "Patient not found.\n"; return; }
    double amount = getValidatedDouble("Enter amount: ", 0.0);
    string details = getValidatedLine("Enter details (treatment/notes): ");

    Bill b;
    b.id = nextBillId++;
    b.patientId = pid;
    b.amount = amount;
    b.details = details;
    bills.push_back(b);
    cout << "Bill created (ID = " << b.id << ").\n";
}

void listBillsForPatient() {
    int pid = getValidatedInt("Enter patient ID: ", 1);
    bool any = false;
    double total = 0.0;
    for (auto &b : bills) {
        if (b.patientId == pid) {
            cout << "Bill ID: " << b.id << " | Amount: " << b.amount << " | Details: " << b.details << '\n';
            total += b.amount;
            any = true;
        }
    }
    if (!any) cout << "No bills for this patient.\n";
    else cout << "Total for patient: " << total << '\n';
}

void listAllBills() {
    if (bills.empty()) { cout << "No bills recorded.\n"; return; }
    double total = 0.0;
    for (auto &b : bills) {
        cout << "Bill ID: " << b.id << " | PatientID: " << b.patientId << " | Amount: " << b.amount << " | Details: " << b.details << '\n';
        total += b.amount;
    }
    cout << "Total revenue: " << total << '\n';
}

// ---------------------- Reports / Search ----------------------
void reportSummary() {
    cout << "\n--- Summary Report ---\n";
    cout << "Total patients: " << patients.size() << '\n';
    cout << "Total doctors: " << doctors.size() << '\n';
    cout << "Total appointments: " << appointments.size() << '\n';
    double revenue = 0;
    for (auto &b : bills) revenue += b.amount;
    cout << "Total revenue: " << revenue << '\n';
}

// ---------------------- Menus ----------------------
void patientMenu() {
    while (true) {
        cout << "\n-- Patient Menu --\n"
             << "1. Add Patient\n"
             << "2. Display Patients\n"
             << "3. Search Patient by ID\n"
             << "4. Update Patient\n"
             << "5. Delete Patient\n"
             << "0. Back\n";
        int c = getValidatedInt("Choice: ", 0, 5);
        if (c == 0) break;
        switch (c) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
        }
    }
}

void doctorMenu() {
    while (true) {
        cout << "\n-- Doctor Menu --\n"
             << "1. Add Doctor\n"
             << "2. Display Doctors\n"
             << "3. Search Doctor by ID\n"
             << "4. Update Doctor\n"
             << "5. Delete Doctor\n"
             << "0. Back\n";
        int c = getValidatedInt("Choice: ", 0, 5);
        if (c == 0) break;
        switch (c) {
            case 1: addDoctor(); break;
            case 2: displayDoctors(); break;
            case 3: searchDoctor(); break;
            case 4: updateDoctor(); break;
            case 5: deleteDoctor(); break;
        }
    }
}

void appointmentMenu() {
    while (true) {
        cout << "\n-- Appointment Menu --\n"
             << "1. Book Appointment\n"
             << "2. List All Appointments\n"
             << "3. List Appointments for Patient\n"
             << "4. List Appointments for Doctor\n"
             << "0. Back\n";
        int c = getValidatedInt("Choice: ", 0, 4);
        if (c == 0) break;
        switch (c) {
            case 1: bookAppointment(); break;
            case 2: listAppointments(); break;
            case 3: listAppointmentsForPatient(); break;
            case 4: listAppointmentsForDoctor(); break;
        }
    }
}

void billingMenu() {
    while (true) {
        cout << "\n-- Billing Menu --\n"
             << "1. Create Bill\n"
             << "2. List Bills for Patient\n"
             << "3. List All Bills & Revenue\n"
             << "0. Back\n";
        int c = getValidatedInt("Choice: ", 0, 3);
        if (c == 0) break;
        switch (c) {
            case 1: createBill(); break;
            case 2: listBillsForPatient(); break;
            case 3: listAllBills(); break;
        }
    }
}

void reportsMenu() {
    while (true) {
        cout << "\n-- Reports Menu --\n"
             << "1. Summary Report\n"
             << "2. Search Patient by ID\n"
             << "3. Search Doctor by ID\n"
             << "0. Back\n";
        int c = getValidatedInt("Choice: ", 0, 3);
        if (c == 0) break;
        switch (c) {
            case 1: reportSummary(); break;
            case 2: searchPatient(); break;
            case 3: searchDoctor(); break;
        }
    }
}

// ---------------------- Main ----------------------
int main() {
    loadAll();
    cout << "Welcome to Hospital Management System\n";
    while (true) {
        cout << "\n==== Main Menu ====\n"
             << "1. Patient Management\n"
             << "2. Doctor Management\n"
             << "3. Appointment Booking\n"
             << "4. Billing\n"
             << "5. Reports & Search\n"
             << "0. Save & Exit\n";
        int c = getValidatedInt("Choice: ", 0, 5);
        if (c == 0) {
            saveAll();
            cout << "Data saved. Goodbye!\n";
            break;
        }
        switch (c) {
            case 1: patientMenu(); break;
            case 2: doctorMenu(); break;
            case 3: appointmentMenu(); break;
            case 4: billingMenu(); break;
            case 5: reportsMenu(); break;
        }
    }
    return 0;
}
