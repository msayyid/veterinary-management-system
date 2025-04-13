#include "Owner.h"
#include "Appointment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>




// constructor
Owner::Owner(int id, const std::string& name, const std::string& address, const std::string& ph_no, const std::string& email) 
    : ownerId(id), name(name), address(address), phone_number(ph_no), email(email) {}

int Owner::getOwnerId() const {return ownerId;}
std::string Owner::getName() const {return name;}
std::string Owner::getAddress() const {return address;}
std::string Owner::getPhoneNumber() const {return phone_number;}
std::string Owner::getEmail() const {return email;}


    // add owner record, auto assign id
void Owner::addRecord(const std::string& date, const std::string& details) { // just reading, no modification
    records[nextRecordId] = Record(date, details, "Owner"); // id:data; dict
    std::cout << "Record added with ID: " << nextRecordId << "\n";
    nextRecordId ++;
}

void Owner::addRecordWithId(int id, const std::string& date, const std::string& details) {
    records[id] = Record(date, details, "Owner");
    if (id >= nextRecordId) nextRecordId = id + 1;
}

void Owner::updateRecord(int recordId, const std::string& newDate, const std::string& newDetails) {
    auto it = records.find(recordId);
    if (it != records.end()) {
        it->second.updateDate(newDate);
        it->second.updateDetails(newDetails);
    } else {
        std::cerr << "❌ Record ID not found!\n";
    }
}

const std::map<int, Record>& Owner::getRecords() const {
    return records;
}



bool Owner::hasRecord(int recordId) const {
    return records.find(recordId) != records.end();
}



void Owner::removeRecord(int recordId) {
    if (records.erase(recordId)) {
        std::cout << "✅ Record " << recordId << " removed successfully.\n";
    } else {
        std::cerr << "❌ Record ID not found!\n";
    }
}

void Owner::displayRecords() const {
    if (records.empty()) {
        std::cout << "❌ No records for " << name << ".\n";
        return;
    }
    std::cout << "Owner records for " << name << ":\n";
    for (const auto& pair : records) {
        std::cout << "ID " << pair.first << ": ";
        pair.second.displayRecord();
    }
}

// setters
void Owner::setName(const std::string& newName) {
    name = newName;
}

void Owner::setAddress(const std::string& newAddress) {
    address = newAddress;
}

void Owner::setPhoneNumber(const std::string& newPhone) {
    phone_number = newPhone;
}

void Owner::setEmail(const std::string& newEmail) {
    email = newEmail;
}



    // add an appointement
void Owner::addAppointment(const Appointment* appt) {
    appointments.push_back(appt);
}

void Owner::displayAppointments() const {
    if (appointments.empty()) {
        std::cout << "❌ No appointments for " << name << ".\n";
    }
    std::cout << "Appointments for " << name << ":\n";
    for (const auto& appt : appointments) {
        appt->displayAppointmentDetails();
    }
}


// add pets to owner
void Owner::addPetId(int petId) {
    petIds.push_back(petId);
    // std::cout << "Pet ID " << petId << " linked to owner " << name << ".\n";
}

void Owner::displayPets() const {
    if (petIds.empty()) {
        std::cout << "❌ No pets linked to owner " << name << ".\n";
        return;
    }
    std::cout << "Pets of " << name << ": ";
    for (int id : petIds) {
        std::cout << id << " ";
    }
    std::cout << "\n";
}

std::vector<int> Owner::getPetIds() const {
    return petIds;
}

std::vector<int>& Owner::getPetIdsRef() { return petIds; }


std::vector<Owner> Owner::loadFromFile(const std::string& filename) {
    std::vector<Owner> owners; // to store owners

    std::ifstream file(filename); // open file to read from
    if (!file) {
        std::cerr << "Erro. No such file!\n";
        return owners; // return empty list if nothing
    }

    std::string line;
    std::getline(file, line); // skip header, do not process

    // std::cout << "\nFile Content:\n";
    while (std::getline(file, line)) { // loop over every line, each line is one owner
        if(line.empty()) continue; // skip empty lines

        std::stringstream ss(line);
        std::string idStr, name, address, phone, email, petIdsData, recordData;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, address, ',');
        address = unescapeCommas(address);
        std::getline(ss, phone, ',');
        std::getline(ss, email, ',');
        std::getline(ss, petIdsData, ',');
        std::getline(ss, recordData); // full record string

        if (idStr.empty()) {
            std::cerr << "skipping malformed owner record: " << line << "\n";
            continue;
        }
        

        // int ownerId = std::stoi(idStr);
        int ownerId;
        try {
            ownerId = std::stoi(idStr);
        } catch (const std::exception& e) {
            std::cerr << "Invalid owner ID in line: " << line << "\n";
            continue;
        }

        Owner owner(ownerId, name, address, phone, email);

        // pet ids
        std::stringstream petIdStream(petIdsData);
        std::string petIdStr;


        while (std::getline(petIdStream, petIdStr, ';')) {
            if (!petIdStr.empty()) {
                try {
                    owner.addPetId(std::stoi(petIdStr));
                } catch (...) {
                    std::cerr << "INvalid pet ID for owner " << ownerId << "\n";
                }
            }
        }

        // split records by ";"
        std::stringstream recordStream(recordData);
        std::string record;
        while (std::getline(recordStream, record, ';')) { // ??????????????
            std::stringstream recordParts(record);
            std::string recIdStr, date, details;
            std::getline(recordParts, recIdStr, '|');
            std::getline(recordParts, date, '|');
            std::getline(recordParts, details, '|');
            details = unescapeCommas(details);

            if (!recIdStr.empty()) { // avoid empty records
                try {
                    int recordId = std::stoi(recIdStr);
                    owner.addRecordWithId(recordId, date, details);

                } catch (...) {
                    std::cerr << "Invalid record id for owner " << ownerId << "\n";
                }
                
            }
        }

        owners.push_back(owner); // add owner to the list
    }
    
    file.close();
    return owners;
}


void Owner::writeToFileStream(std::ofstream& file) const {
    if (!file) {
        std::cerr << "File stream is not open!\n";
        return;
    }
    file << ownerId << "," << name << "," << escapeCommas(address) << "," << phone_number << "," << email << ",";
    // Pet IDs
    for (size_t i = 0; i < petIds.size(); i++) {
        file << petIds[i];
        if (i != petIds.size() - 1) file << ";";
    }
    file << ",";
    // Records
    bool first = true;
    for (const auto& [id, rec] : records) {
        if (!first) file << ";";
        file << id << "|" << rec.getDate() << "|" << escapeCommas(rec.getDetails());
        first = false;
    }
    file << "\n";
}

void Owner::displayOwnerDetails() const {
    std::cout << "\n📄 ====== Owner Details ======\n";
    std::cout << "🆔 Owner ID     : " << ownerId << "\n";
    std::cout << "👤 Name         : " << name << "\n";
    std::cout << "🏠 Address      : " << address << "\n";
    std::cout << "📞 Phone Number : " << phone_number << "\n";
    std::cout << "📧 Email        : " << email << "\n";
    std::cout << "===============================\n";
}


void Owner::displayRecordTable() const {
    if (records.empty()) {
        std::cout << "No records for " << name << ".\n";
        return;
    }

    std::cout << "\n--- Owner Records for " << name << " ---\n";
    std::cout << std::left << std::setw(10) << "Rec ID"
              << std::setw(15) << "Date"
              << "Details (truncated)\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& [id, rec] : records) {
        std::cout << std::setw(10) << id
                  << std::setw(15) << rec.getDate()
                  << truncateDetails(rec.getDetails(), 40) << "\n";
    }

    std::cout << std::string(70, '-') << "\n";
}

void Owner::displayFullRecord(int recordId) const {
    auto it = records.find(recordId);
    if (it != records.end()) {
        it->second.displayFull(recordId);
    } else {
        std::cout << "Record with ID " << recordId << " not found.\n";
    }
}

bool Owner::hasRecords() const {
    return !records.empty();
}

void Owner::deleteRecord(int recordId) {
    auto it = records.find(recordId);
    if (it != records.end()) {
        records.erase(it);
    } else {
        std::cerr << "Record ID not found for this owner.\n";
    }
}

void Owner::removePetId(int petId) {
    petIds.erase(std::remove(petIds.begin(), petIds.end(), petId), petIds.end());
}


void Owner::displayLinkedPets(const std::vector<Pet>& pets) const {
    const auto& petIds = getPetIds();

    if (petIds.empty()) {
        std::cout << "🐾 Linked Pets: None\n";
        return;
    }

    // Collect found pets
    std::vector<const Pet*> linkedPets;
    for (int petId : petIds) {
        auto it = std::find_if(pets.begin(), pets.end(), [petId](const Pet& p) {
            return p.getPetId() == petId;
        });
        if (it != pets.end()) {
            linkedPets.push_back(&(*it));
        }
    }

    // Sort by pet ID
    std::sort(linkedPets.begin(), linkedPets.end(), [](const Pet* a, const Pet* b) {
        return a->getPetId() < b->getPetId();
    });

    std::cout << "🐾 Linked Pets (sorted by ID):\n";
    for (const Pet* pet : linkedPets) {
        std::cout << "   - Pet ID: " << pet->getPetId()
                  << " | Name: " << pet->getName() << "\n";
    }

    // Show missing pets (not found in pets list)
    for (int petId : petIds) {
        if (std::none_of(linkedPets.begin(), linkedPets.end(), [petId](const Pet* p) {
            return p->getPetId() == petId;
        })) {
            std::cout << "   - Pet ID: " << petId << " | (Not found in pet list)\n";
        }
    }
}
