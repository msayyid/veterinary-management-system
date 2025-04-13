#ifndef OWNER_H
#define OWNER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "Record.h"
#include "globals.h"
#include "Pet.h"

class Appointment;
class Pet;

// Represents a pet owner in the system, storing personal info, pets, records, and appointments.
class Owner {
private:
    int ownerId;                          // Unique ID for the owner
    std::string name, address, phone_number, email;

    std::map<int, Record> records;        // Medical/general records linked to the owner
    std::vector<const Appointment*> appointments; // Appointments linked to this owner
    int nextRecordId = 1;                 // ID counter for records
    std::vector<int> petIds;              // IDs of pets owned by this owner

public:
    // Constructor to initialize a new owner with details
    Owner(int ownerId, const std::string& name, const std::string& address, const std::string& ph_no, const std::string& email);

    // Getters
    int getOwnerId() const;
    std::string getName() const;
    std::string getAddress() const;
    std::string getPhoneNumber() const;
    std::string getEmail() const;
    std::vector<int> getPetIds() const;
    std::vector<int>& getPetIdsRef();
    int getRecordCount() const;                           // Returns total number of records
    const std::map<int, Record>& getRecords() const;      // Returns reference to owner's records

    void removePetId(int petId);

    // Setters
    void setName(const std::string& newName);
    void setAddress(const std::string& newAddress);
    void setPhoneNumber(const std::string& newPhone);
    void setEmail(const std::string& newEmail);

    // Record handling
    void addRecord(const std::string& date, const std::string& details);                  // Adds a new record with auto ID
    void addRecordWithId(int id, const std::string& date, const std::string& details);    // Loads a record with existing ID
    void updateRecord(int recordId, const std::string& newDate, const std::string& newDetails); // Updates a record
    void removeRecord(int recordId);                      // Removes a record
    void displayRecords() const;                          // Displays all records in a table
    bool hasRecord(int recordId) const;                   // Checks if a record with given ID exists
    bool hasRecords() const;                              // Checks if the owner has any records
    void deleteRecord(int recordId);                      // Deletes a specific record

    // Appointment handling
    void addAppointment(const Appointment* appt);         // Adds a linked appointment to the owner
    void displayAppointments() const;                     // Displays all appointments for this owner

    // File I/O
    static std::vector<Owner> loadFromFile(const std::string& filename); // Loads owners from a file
    void writeToFileStream(std::ofstream& file) const;    // Writes the owner's data to a file stream

    // Pet and display-related methods
    void addPetId(int PetId);                             // Links a pet ID to this owner
    void displayPets() const;                             // Displays all pet IDs owned
    void displayOwnerDetails() const;                     // Displays detailed owner information

    void displayRecordTable() const;                      // Shows all records in tabular form
    void displayFullRecord(int recordId) const;           // Displays full details of a specific record
    void displayLinkedPets(const std::vector<Pet>& pets) const; // Displays pet details linked to this owner
};

#endif  // OWNER_H



