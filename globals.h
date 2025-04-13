// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <memory>
#include <map>
#include "Pet.h"
#include "Owner.h"
#include "Appointment.h"
#include "User.h"

// Global vectors storing system-wide data
extern std::vector<Pet> pets;                        // List of all pets
extern std::vector<Owner> owners;                    // List of all owners
extern std::vector<Appointment> appointments;        // List of all appointments
extern std::vector<std::unique_ptr<User>> users;     // List of all system users (with roles)

// Global counters for assigning unique IDs
extern int nextPetId;
extern int nextOwnerId;
extern int nextAppointmentId;
extern int nextUserId;

// Saves all owner records to a CSV file (default: "owners.csv")
void saveAllOwnersToFile(const std::vector<Owner>& owners, const std::string& filename = "owners.csv");

// Saves all pet records to a CSV file (default: "pets.csv")
void saveAllPetsToFile(const std::vector<Pet>& pets, const std::string& filename = "pets.csv");

// Escapes commas in a string for safe CSV storage
std::string escapeCommas(const std::string& input);

// Reverses escaped commas in a string to restore original content
std::string unescapeCommas(const std::string& input);

// Truncates long strings for cleaner display (default limit: 40 characters)
std::string truncateDetails(const std::string& details, size_t maxLength = 40);

// Displays full details of a single record by ID
void displayFullRecord(const std::map<int, Record>& records, int recordId, const std::string& recordType = "Record");

// Saves all appointment records to file (appointments.csv)
void saveAllAppointmentsToFile(const std::vector<Appointment>& appointments);

// Loads all appointment records from file
std::vector<Appointment> loadAllAppointmentsFromFile(const std::string& filename);

// Returns all appointments associated with a specific pet ID
std::vector<Appointment> getAppointmentsForPet(int petId);

// Displays a list of pets that are not assigned to any owner
void displayUnassignedPets(const std::vector<Pet>& pets);

// Displays full record details if the specified record exists
bool displayFullRecordIfExists(const std::map<int, Record>& records, int recordId, const std::string& recordType);

// Prompts the user to view full details of a selected appointment
void promptToViewFullAppointment(const std::vector<Appointment>& appointments);

#endif  // GLOBALS_H
