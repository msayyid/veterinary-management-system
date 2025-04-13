#ifndef RECORD_H
#define RECORD_H

#pragma once
#include <string>
#include <iostream>
#include <iomanip>

// Represents a general-purpose record linked to a pet or owner.
// Stores the date, details, and type of the record (e.g., "Pet", "Owner").
class Record {
    std::string date;     // Record date (e.g., "2025-04-08")
    std::string details;  // Description or notes about the record
    std::string type;     // Type of record ("Pet" or "Owner")

public:
    // Default constructor
    Record() : date(""), details(""), type("") {}

    // Parameterized constructor
    Record(const std::string& date, const std::string details, const std::string type) 
        : date(date), details(details), type(type) {}

    // Getter methods
    std::string getDate() const { return date; }
    std::string getDetails() const { return details; }
    std::string getType() const { return type; }

    // Setter methods
    void updateDetails(const std::string& newDetails) { details = newDetails; }
    void updateDate(const std::string& newDate) { date = newDate; }
    void setDate(const std::string& newDate) { date = newDate; }
    void setDetails(const std::string& newDetails) { details = newDetails; }

    // Displays the record as a compact summary (single-line)
    void displayRecord() const {
        std::cout << "[" << type << "] "
                  << "📅 Date: " << date
                  << " 📝 Details: " << details << ".\n";
    }

    // Displays the record as a row in a table, with an ID column
    void displayAsTableRow(int id) const {
        std::cout << std::left << std::setw(10) << id
                  << std::setw(15) << date
                  << details << "\n";
    }

    // Displays the full detailed view of the record with formatting
    void displayFull(int id) const {
        std::cout << "\n📄 ========= Record Details ==========\n";
        std::cout << "🆔 Record ID   : " << id << "\n";
        std::cout << "📂 Type        : " << type << "\n";
        std::cout << "📅 Date        : " << date << "\n";
        std::cout << "📝 Details     : " << details << "\n";
        std::cout << "====================================\n";
    }
};

#endif
