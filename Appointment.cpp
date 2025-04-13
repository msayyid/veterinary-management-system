#include <iostream>
#include <string>
#include "Appointment.h"
#include "Pet.h"
#include "Owner.h"
#include <fstream>
#include <sstream>
#include "globals.h"
#include "validations.h"


Appointment::Appointment(int id, int ownerId, int petId, const std::string& date, const std::string& time, const std::string& purpose, const std::string& status) 
        : appointmentId(id), ownerId(ownerId), petId(petId), date(date), time(time), purpose(purpose), status(status) {}


// getter methods
int Appointment::getAppointmentId() const {return appointmentId;}
int Appointment::getOwnerId() const {return ownerId;}
int Appointment::getPetId() const {return petId;}
std::string Appointment::getDate() const {return date;}
std::string Appointment::getTime() const {return time;}
std::string Appointment::getPurpose() const {return purpose;}
std::string Appointment::getStatus() const {return status;}

// setter methods

void Appointment::setDate(const std::string& newDate) {
    date = newDate;
}

void Appointment::setTime(const std::string& newTime) {
    time = newTime;
}

void Appointment::setPurpose(const std::string& newPurpose) {
    purpose = newPurpose;
}

void Appointment::setStatus(const std::string& newStatus) {
    status = newStatus;
}


void Appointment::setOwnerId(int newOwnerId) {
    ownerId = newOwnerId;
}
void Appointment::updateStatus(const std::string& newStatus) {
    if (newStatus == "scheduled" || newStatus == "completed" || newStatus == "cancelled") {
        status = newStatus;
    } else {
        std::cerr << "Invalid status. Must be 'scheduled', 'completed', or 'cancelled'.\n";
    }
}

void Appointment::updateDate(const std::string& newDate) { 
    date = newDate;
}

void Appointment::updateTime(const std::string& newTime) {
    time = newTime;
}

// display
void Appointment::displayAppointmentDetails() const {
    std::cout << "\n📋 ====== Appointment Details ======\n";
    std::cout << "🆔 Appointment ID : " << appointmentId << "\n";
    std::cout << "👤 Owner ID       : " << ownerId << "\n";
    std::cout << "🐾 Pet ID         : " << petId << "\n";
    std::cout << "📅 Date           : " << date << "\n";
    std::cout << "⏰ Time           : " << time << "\n";
    std::cout << "📝 Purpose        : " << purpose << "\n";
    std::cout << "📌 Status         : " << status << "\n";
    std::cout << "====================================\n";
}


void Appointment::displayAsTableRow() const {
    std::cout << std::setw(10) << appointmentId
              << std::setw(10) << petId
              << std::setw(10) << ownerId
              << std::setw(12) << date
              << std::setw(8) << time
              << std::setw(20) << purpose.substr(0, 18) 
              << std::setw(12) << status << "\n";
}

void Appointment::displayFullAppointment() const {
    std::cout << "\n📋 ====== Appointment Details ======\n";
    std::cout << "🆔 Appointment ID : " << appointmentId << "\n";
    std::cout << "👤 Owner ID       : " << ownerId << "\n";
    std::cout << "🐾 Pet ID         : " << petId << "\n";
    std::cout << "📅 Date           : " << date << "\n";
    std::cout << "⏰ Time           : " << time << "\n";
    std::cout << "📝 Purpose        : " << purpose << "\n";
    std::cout << "📌 Status         : " << status << "\n";
    std::cout << "====================================\n";

}

void Appointment::displayAppointmentsTable(const std::vector<Appointment>& appointments) {
    if (appointments.empty()) {
        std::cout << "📭 No appointments to display.\n";
        return;
    }

    // Column widths
    const int purposeWidth = 30;  // Total width for Purpose column
    const int statusWidth = 12;   // Width for Status column
    const int maxPurposeLength = 25; // Total display length for Purpose (including ellipsis if needed)
    const int maxPurposeTextLength = maxPurposeLength - 3; // Truncate text to 17 chars to leave room for "..."

    // Header
    std::cout << std::left
              << std::setw(10) << "Appt ID"
              << std::setw(10) << "Pet ID"
              << std::setw(10) << "Owner ID"
              << std::setw(12) << "Date"
              << std::setw(8)  << "Time"
              << std::setw(purposeWidth) << "Purpose"
              << std::setw(statusWidth)  << "Status"
              << "\n";

    std::cout << std::string(10 + 10 + 10 + 12 + 8 + purposeWidth + statusWidth, '-') << "\n";

    // Rows
    for (const auto& appt : appointments) {
        std::string purpose = appt.getPurpose();
        std::string displayPurpose;

        if ((int)purpose.length() > maxPurposeTextLength) {
            displayPurpose = purpose.substr(0, maxPurposeTextLength) + "...";
        } else {
            displayPurpose = purpose;
        }

        std::cout << std::left
                  << std::setw(10) << appt.getAppointmentId()
                  << std::setw(10) << appt.getPetId()
                  << std::setw(10) << appt.getOwnerId()
                  << std::setw(12) << appt.getDate()
                  << std::setw(8)  << appt.getTime()
                  << std::setw(purposeWidth) << displayPurpose
                  << std::setw(statusWidth)  << appt.getStatus()
                  << "\n";
    }

    std::cout << std::string(10 + 10 + 10 + 12 + 8 + purposeWidth + statusWidth, '-') << "\n";
    std::cout << "📊 Total number of appointments: " << appointments.size() << "\n";
}






// save to file
void Appointment::saveToFile(const std::string& filename) const {
    // check if file empty to write header 
    bool isEmpty = false;
    std::ifstream checkFile(filename); // open in read mode
    if (checkFile.peek() == std::ifstream::traits_type::eof()) {
        isEmpty = true; // file is empty
    }
    checkFile.close();

    // open file in open mode
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Error! No such file!\n";
        return;
    }

    // write header
    if (isEmpty) {
        file << "appointment_id,owner_id,pet_id,date,time,purpose,status\n";
    }

    // write appointment data
    file << appointmentId << "," 
         << ownerId << "," 
         << petId << "," 
         << date << "," 
         << time << "," 
         << escapeCommas(purpose) << "," 
         << status << "\n";

    file.close();
    
}


std::vector<Appointment> Appointment::loadFromFile(const std::string& filename) {
    std::vector<Appointment> appointments;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error. No such file!\n";
        return appointments;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, ownerStr, petStr, date, time, restOfLine;

        std::getline(ss, idStr, ',');
        std::getline(ss, ownerStr, ',');
        std::getline(ss, petStr, ',');
        std::getline(ss, date, ',');
        std::getline(ss, time, ',');
        std::getline(ss, restOfLine);  // remainder: purpose and status

        if (idStr.empty() || ownerStr.empty() || petStr.empty() || restOfLine.empty()) {
            std::cerr << "Malformed appointment line: " << line << "\n";
            continue;
        }

        // find last comma — separate purpose and status
        size_t lastComma = restOfLine.rfind(',');
        if (lastComma == std::string::npos) {
            std::cerr << "Malformed line (status missing): " << line << "\n";
            continue;
        }

        std::string purposePart = restOfLine.substr(0, lastComma);
        std::string statusPart = restOfLine.substr(lastComma + 1);

        appointments.emplace_back(
            std::stoi(idStr),
            std::stoi(ownerStr),
            std::stoi(petStr),
            date, time,
            unescapeCommas(purposePart),
            statusPart
        );
    }

    return appointments;
}

Appointment* findAppointmentById(std::vector<Appointment>& appointments, int id) {
    for (auto& appt : appointments) {
        if (appt.getAppointmentId() == id) {
            return &appt;
        }
    }
    return nullptr;
}

void Appointment::displayAppointmentsTable(const std::vector<Appointment*>& appts) {
    if (appts.empty()) {
        std::cout << "ℹ️ No appointments to display.\n";
        return;
    }

    std::cout << "\n📅 --- Appointment List ---\n";
    std::cout << std::left
              << std::setw(6)  << "🆔"
              << std::setw(12) << "📅 Date"
              << std::setw(8)  << "🕒 Time"
              << std::setw(20) << "🐾 Pet Name"
              << std::setw(35) << "📝 Purpose"
              << std::setw(14) << "📌 Status" << "\n";

    std::cout << std::string(6 + 12 + 8 + 20 + 35 + 14, '-') << "\n";

    for (const Appointment* appt : appts) {
        const Pet* pet = findPetById(pets, appt->getPetId());
        std::string petName = pet ? pet->getName() : "(Unknown)";
        std::string shortPurpose = truncateDetails(appt->getPurpose(), 34);

        std::string statusDisplay;
        if (appt->getStatus() == "scheduled") statusDisplay = "📅 scheduled";
        else if (appt->getStatus() == "completed") statusDisplay = "✅ completed";
        else if (appt->getStatus() == "cancelled") statusDisplay = "❌ cancelled";
        else statusDisplay = appt->getStatus(); // fallback

        std::cout << std::left
                  << std::setw(6)  << appt->getAppointmentId()
                  << std::setw(12) << appt->getDate()
                  << std::setw(8)  << appt->getTime()
                  << std::setw(20) << petName
                  << std::setw(35) << shortPurpose
                  << std::setw(14) << statusDisplay << "\n";
    }

    std::cout << std::string(6 + 12 + 8 + 20 + 35 + 14, '-') << "\n";
}


