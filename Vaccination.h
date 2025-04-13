#ifndef VACCINATION_H
#define VACCINATION_H
#include <string>
#include <iostream>
#include <iomanip>


// Represents a vaccination record for a pet.
// Stores the vaccine name, date administered, and status (e.g., completed, pending).

class Vaccination {
private:
    int id;                         // Unique vaccination ID
    std::string name;               // Name of the vaccine
    std::string date;               // Date of vaccination (e.g., "2025-04-08")
    std::string status;             // Vaccination status (e.g., "completed", "pending")


public:
    Vaccination(int id, const std::string& name, const std::string& date, const std::string& status)
            : id(id), name(name), date(date), status(status) {}

    // getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getDate() const { return date; }
    std::string getStatus() const { return status; }

    // Setters (optional, if you want to allow updates)
    void setDate(const std::string& newDate) { date = newDate; }
    void setStatus(const std::string& newStatus) { status = newStatus; }

    // Display in table row
    void displayAsTableRow() const {
        std::cout << std::setw(8) << id
                  << std::setw(20) << name
                  << std::setw(15) << date
                  << std::setw(20) << status << "\n";
    }

    // Serialization (optional, if needed)
    std::string serialize() const {
        return name + "|" + date + "|" + status;
    }


};

#endif