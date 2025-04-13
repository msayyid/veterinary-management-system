#ifndef PET_H
#define PET_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "Vaccination.h"
#include "Record.h"
#include "Owner.h"
#include "globals.h"

class Appointment;
class Owner;

// Represents a pet in the veterinary management system.
// Stores basic info, medical/vaccination/general records, and appointment history.
class Pet {
    int petId;                                // Unique pet ID
    std::string name, breed;
    int age;
    int ownerId;                              // Linked owner's ID
    std::string vaccin_status;               // Calculated vaccination status

    std::vector<Vaccination> vaccinations;    // Vaccination records
    std::map<int, Record> medicalHistory;     // Medical records (vet only)
    std::map<int, Record> petRecords;         // General records (staff)
    std::vector<Appointment*> appointmentHistory;

    int nextMedicalRecordId = 1;
    int nextPetRecordId = 1;
    int nextVaccinationId = 1;

public:
    // Constructs a new pet with the given details
    Pet(int petId, const std::string& name, const std::string& breed, const int age, int onwerId);

    void setName(const std::string& newName);
    void setBreed(const std::string& newBreed);
    void setAge(int newAge);


    // Getters
    std::string getName() const;
    std::string getBreed() const;
    int getAge() const;
    int getPetId() const;
    int getOnwerId() const;
    const std::map<int, Record>& getPetRecords() const;
    const std::map<int, Record>& getMedicalHistory() const;

    // Utility checks
    bool hasRecords(const std::map<int, Record>& recordMap) const;
    bool hasRecord(int recordId, const std::map<int, Record>& recordMap) const;
    bool hasMedicalRecord(int recordId) const;
    bool hasVaccination(int vaccId) const;
    bool hasVaccinations() const;
    bool hasMedicalHistory() const;

    // Updates the owner ID
    void setOwnerId(int newOwnerId);

    // Calculates overall vaccination status
    std::string calculateVaccinationStatus() const;

    // ===== Medical History (Vet only) =====
    void addMedicalHistory(const std::string& date, const std::string& details);
    void addMedicalHistoryWithId(int id, const std::string& date, const std::string& details);
    void updateMedicalHistory(int recordId, const std::string& newDate, const std::string& newDetails);
    void removeMedicalHistory(int recordId);
    void displayMedicalHistory() const;
    void displayFullMedicalRecord(int recordId) const;
    void displayMedicalHistoryTable() const;

    // ===== General Records (Staff) =====
    void addPetRecord(const std::string& date, const std::string& details);
    void addPetRecordWithId(int id, const std::string& date, const std::string& details);
    void updatePetRecord(int recordId, const std::string& date, const std::string& newDetails);
    void updatePetRecordDate(int recordId, const std::string& newDate);
    void updatePetRecordDetails(int recordId, const std::string& newDetails);
    void removePetRecord(int recordId);
    void displayPetRecords() const;

    // ===== Vaccinations =====
    void addVaccination(const std::string& name, const std::string& date, const std::string& status);
    void addVaccinationWithId(int id, const std::string& name, const std::string& date, const std::string& status);
    void updateVaccination(int vaccId, const std::string& newDate, const std::string& newStatus);
    void removeVaccination(int vaccId);
    void displayVaccinationsTable() const;
    void displayFullVaccination(int vaccId) const;
    const Vaccination* getVaccinationById(int vaccinationId) const;

    // ===== Display and File I/O =====
    void displayRecordTable(const std::map<int, Record>& recordMap, const std::string& recordType) const;
    void displayPetDetails(const std::vector<Owner>& owners) const;
    void displayAsTableRow(const std::vector<Owner>& owners) const;
    std::string truncatePet(const std::string& text, size_t width) const;

    void writeToFileStream(std::ofstream& file) const;
    static std::vector<Pet> loadFromFile(const std::string& filename); // Loads pet records from file
};

// Displays a table header row for pet listings
void displayPetTableHeader();

#endif  // PET_H