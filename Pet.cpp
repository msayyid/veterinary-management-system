#include "Pet.h"
#include "Owner.h"
#include "globals.h"
#include <fstream>
#include "Vaccination.h"
#include "validations.h"



// constructor
Pet::Pet(int petId, const std::string& name, const std::string& breed, const int age, int ownerId) 
    : petId(petId), name(name), breed(breed), age(age), ownerId(ownerId) {}

// getters
std::string Pet::getName() const {return name;}
std::string Pet::getBreed() const {return breed;}
int Pet::getAge() const {return age;}
int Pet::getPetId() const {return petId;}
int Pet::getOnwerId() const {return ownerId;}
const std::map<int, Record>& Pet::getPetRecords() const { return petRecords; }
const std::map<int, Record>& Pet::getMedicalHistory() const { return medicalHistory; }


bool Pet::hasMedicalHistory() const {
    return !medicalHistory.empty();
}

bool Pet::hasRecords(const std::map<int, Record>& recordMap) const {
    return !recordMap.empty();
}

bool Pet::hasRecord(int recordId, const std::map<int, Record>& recordMap) const {
    return recordMap.find(recordId) != recordMap.end();
}

void Pet::setName(const std::string& newName) { name = newName; }
void Pet::setBreed(const std::string& newBreed) { breed = newBreed; }
void Pet::setAge(int newAge) { age = newAge; }


void Pet::setOwnerId(int newOwnerId) {
    ownerId = newOwnerId;
}


std::string Pet::calculateVaccinationStatus() const {
    if (vaccinations.empty()) return "none";

    for (const auto& v : vaccinations) {
        if (v.getStatus() == "pending" || v.getStatus() == "booster required")
            return "pending";
    }
    return "completed";
}




void Pet::displayRecordTable(const std::map<int, Record>& recordMap, const std::string& recordType) const {
    if (recordMap.empty()) {
        std::cout << "No " << recordType << " found for this pet.\n";
        return;
    }

    std::cout << "\n--- " << recordType << " ---\n";
    std::cout << std::left << std::setw(10) << "Rec ID"
              << std::setw(15) << "Date"
              << "Details (truncated)\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& recordPair : recordMap) {
        std::cout << std::setw(10) << recordPair.first
                  << std::setw(15) << recordPair.second.getDate()
                  << truncateDetails(recordPair.second.getDetails(), 40) << "\n";
    }
    std::cout << std::string(70, '-') << "\n";
}



void Pet::displayMedicalHistoryTable() const {
    if (medicalHistory.empty()) {
        std::cout << "No medical records found for this pet.\n";
        return;
    }

    std::cout << std::left << std::setw(10) << "Rec ID"
              << std::setw(15) << "Date"
              << "Details (truncated)\n";
    std::cout << std::string(70, '-') << "\n";

    for (const auto& recordPair : medicalHistory) {
        std::cout << std::setw(10) << recordPair.first
                  << std::setw(15) << recordPair.second.getDate()
                  << truncateDetails(recordPair.second.getDetails()) << "\n";
    }
    std::cout << std::string(70, '-') << "\n";
}


void Pet::displayPetDetails(const std::vector<Owner>& owners) const {
    std::cout << "📛 Name : " << name << "\n";
    std::cout << "🧬 Breed: " << breed << "\n";
    std::cout << "🎂 Age  : " << age << "\n";
    
    if (ownerId != -1) {
        // look for match of ownerid
        bool found = false;
        for (const auto& o : owners) {
            if (o.getOwnerId() == ownerId) {
                std::cout << "👤 Owner: " << o.getName() << "\n";
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "👤 Owner: Unknown (🆔 " << ownerId << " not found)\n";
        }
    } else {
        std::cout << "🚫 No owner linked.\n";
    }
}


// vaccinations handling
void Pet::addVaccination(const std::string& name, const std::string& date, const std::string& status) {
    Vaccination vacc(nextVaccinationId++, name, date, status);
    vaccinations.push_back(vacc);
    std::cout << "💉 Vaccination '" << name << "' added with 🆔 " << vacc.getId() << ".\n";
}

void Pet::displayVaccinationsTable() const {
    if (vaccinations.empty()) {
        std::cout << "📭 No vaccinations found for this pet.\n";
        return;
    }
    std::cout << std::setw(8)  << "ID" 
              << std::setw(20) << "Name"
              << std::setw(15) << "Date"
              << std::setw(20) << "Status" << "\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& v : vaccinations) {
        v.displayAsTableRow();
    }
    std::cout << std::string(65, '-') << "\n";
}


bool Pet::hasVaccination(int vaccId) const {
    for (const auto& v : vaccinations) {
        if (v.getId() == vaccId) return true;
    }
    return false;
}
void Pet::updateVaccination(int vaccinationId, const std::string& newDate, const std::string& newStatus) {
    for (auto& v : vaccinations) {
        if (v.getId() == vaccinationId) {
            v.setDate(newDate);
            v.setStatus(newStatus);
            std::cout << "✅ Vaccination (🆔 " << vaccinationId << ") updated successfully.\n";
            return;
        }
    }
    std::cout << "❌ Vaccination with 🆔 " << vaccinationId << " not found.\n";
}


const Vaccination* Pet::getVaccinationById(int vaccinationId) const {
    for (const auto& v : vaccinations) {
        if (v.getId() == vaccinationId) {
            return &v;
        }
    }
    return nullptr;
}



void Pet::removeVaccination(int vaccId) {
    vaccinations.erase(
        std::remove_if(vaccinations.begin(), vaccinations.end(), 
                       [vaccId](const Vaccination& v) { return v.getId() == vaccId; }),
        vaccinations.end()
    );
    std::cout << "✅ Vaccination with ID " << vaccId << " removed successfully.\n";
}



// medical history handling, handled by veterinarians only
void Pet::addMedicalHistory(const std::string& date, const std::string& details) {
    medicalHistory[nextMedicalRecordId] = Record(date, details, "Pet");
    std::cout << "Record added with ID: " << nextMedicalRecordId << "\n";
    nextMedicalRecordId ++;
}

bool Pet::hasMedicalRecord(int recordId) const {
    return medicalHistory.find(recordId) != medicalHistory.end();
}

void Pet::displayFullMedicalRecord(int recordId) const {
    auto it = medicalHistory.find(recordId);
    if (it != medicalHistory.end()) {
        std::cout << "\n📄 ----- Full Medical Record -----\n";
        std::cout << "🆔 Record ID : " << recordId << "\n";
        std::cout << "📅 Date      : " << it->second.getDate() << "\n";
        std::cout << "📝 Details   : " << it->second.getDetails() << "\n";
        std::cout << "--------------------------------\n";
    } else {
        std::cout << "❌ Record with 🆔 " << recordId << " not found.\n";
    }
}




void Pet::addMedicalHistoryWithId(int id, const std::string& date, const std::string& details) {
    medicalHistory[id] = Record(date, details, "Pet");
    if (id >= nextMedicalRecordId) nextMedicalRecordId = id + 1; // update next id 

}

void Pet::updateMedicalHistory(int recordId, const std::string& newDate, const std::string& newDetails) {
    if (medicalHistory.find(recordId) != medicalHistory.end()) {
        medicalHistory[recordId].updateDetails(newDetails);
        medicalHistory[recordId].updateDate(newDate);
        std::cout << "✅ Med history " << recordId << " updated successfully.\n";
    } else {
        std::cerr << "❌ Record ID not found!\n";
    }
}

void Pet::removeMedicalHistory(int recordId) {
    if (medicalHistory.erase(recordId)) {
        std::cout << "✅ Med history " << recordId << " removed successfully.\n";
    } else {
        std::cerr << "❌ Record ID not found!\n";
    }
}

void Pet::displayMedicalHistory() const {
    if (medicalHistory.empty()) {
        std::cout << "❌ No med history found for " << name << ".\n";
    } else {
        std::cout << "Medical History for " << name << ":\n";
        for (const auto& pair : medicalHistory) {
            std::cout << "ID " << pair.first << ": ";
            pair.second.displayRecord();
        }
    }
}

// general record handling, handled by staff
void Pet::addPetRecord(const std::string& date, const std::string& details) {
    petRecords[nextPetRecordId] = Record(date, details, "Pet");
    std::cout << "Record added with ID: " << nextPetRecordId << "\n";
    nextPetRecordId ++;
}

void Pet::addPetRecordWithId(int id, const std::string& date, const std::string& details) {
    petRecords[id] = Record(date, details, "Pet");
    if (id >= nextPetRecordId) nextPetRecordId = id + 1;
}

void Pet::updatePetRecord(int recordId, const std::string& newDate, const std::string& newDetails) {
    if (petRecords.find(recordId) != petRecords.end()) {
        petRecords[recordId].updateDetails(newDetails);
        petRecords[recordId].updateDate(newDate);
        std::cout << "✅ Pet record " << recordId << " updated successfully.\n";
    } else {
        std::cerr << "❌ Pet record id not found!\n";
    }
}

void Pet::removePetRecord(int recordId) {
    if (petRecords.erase(recordId)) {
        std::cout << "✅ Pet record ID " << recordId << " removed successfully.\n";
    } else {
        std::cerr << "Pet record ID not found!\n";
    }
}
void Pet::displayPetRecords() const {
    if (petRecords.empty()) {
        std::cout << "❌ No pet record found for " << name << ".\n";
    } else {
        std::cout << "Pet records for " << name << ": ";
        for (const auto& pair : petRecords) {
            std::cout << "ID " << pair.first << ": ";
            pair.second.displayRecord();
        }
    }
}
void Pet::updatePetRecordDate(int recordId, const std::string& newDate) {
    auto it = petRecords.find(recordId);
    if (it != petRecords.end()) {
        it->second.setDate(newDate);
    }
}

void Pet::updatePetRecordDetails(int recordId, const std::string& newDetails) {
    auto it = petRecords.find(recordId);
    if (it != petRecords.end()) {
        it->second.setDetails(newDetails);
    }
}


void Pet::addVaccinationWithId(int id, const std::string& name, const std::string& date, const std::string& status) {
    vaccinations.emplace_back(id, name, date, status);
}
bool Pet::hasVaccinations() const {
    return !vaccinations.empty();
}

void Pet::displayFullVaccination(int vaccId) const {
    auto it = std::find_if(vaccinations.begin(), vaccinations.end(),
                           [vaccId](const Vaccination& v) { return v.getId() == vaccId; });
    if (it != vaccinations.end()) {
        std::cout << "\n💉 --- Vaccination Details ---\n";
        std::cout << "🆔 ID     : " << it->getId() << "\n";
        std::cout << "📛 Name   : " << it->getName() << "\n";
        std::cout << "📅 Date   : " << it->getDate() << "\n";
        std::cout << "📌 Status : " << it->getStatus() << "\n";
        std::cout << "---------------------------\n";
    } else {
        std::cout << "❌ Vaccination with 🆔 " << vaccId << " not found.\n";
    }
}




std::vector<Pet> Pet::loadFromFile(const std::string& filename) {
    std::vector<Pet> pets;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << "\n";
        return pets;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        // debugging
        // std::cout << "Processing line: " << line << std::endl;   // <== Add this

        std::stringstream ss(line); // ???
        std::string petIdStr, name, breed, ageStr, ownerIdStr, vaccStatus, vaccData, medHistData, petRecData;
        
        std::getline(ss, petIdStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, breed, ',');
        std::getline(ss, ageStr, ',');
        std::getline(ss, ownerIdStr, ',');
        std::getline(ss, vaccStatus, ',');
        std::getline(ss, vaccData, ',');
        std::getline(ss, medHistData, ',');
        std::getline(ss, petRecData);

        // debugging
        // std::cout << "petIdStr: '" << petIdStr << "', ageStr: '" << ageStr << "', ownerIdStr: '" << ownerIdStr << "'" << std::endl;


        if (petIdStr.empty() || ageStr.empty() || ownerIdStr.empty()) {
            std::cerr << "Skipping malformed pet record line: " << line << "\n";
            std::cerr << "Values: petIdStr='" << petIdStr << "' ageStr='" << ageStr << "' ownerIdStr='" << ownerIdStr << "'\n";

            continue;
        }

        int petId = std::stoi(petIdStr);
        int age = std::stoi(ageStr);
        // int ownerId = std::stoi(ownerIdStr);
        // pets may not always have owners
        int ownerId;
        if (ownerIdStr.empty() || ownerIdStr == "-1") {
            ownerId = -1;
        } else {
            ownerId = std::stoi(ownerIdStr);
        }



        Pet pet(petId, name, breed, age, ownerId);
        pet.vaccin_status = vaccStatus;

       // parse vaccinations
        std::stringstream vaccStream(vaccData);
        std::string vaccEntry;
        while (std::getline(vaccStream, vaccEntry, ';')) {
            std::stringstream vaccParts(vaccEntry);
            std::string idStr, vaccName, vaccDate, vaccStatus;
            std::getline(vaccParts, idStr, '|');
            std::getline(vaccParts, vaccName, '|');
            std::getline(vaccParts, vaccDate, '|');
            std::getline(vaccParts, vaccStatus, '|');
            if (!idStr.empty()) {
                int vaccId = std::stoi(idStr);
                pet.addVaccinationWithId(vaccId, vaccName, vaccDate, vaccStatus);
                if (vaccId >= pet.nextVaccinationId) pet.nextVaccinationId = vaccId + 1;
            }
        }

        // parse med history
        std::stringstream medStream(medHistData);
        std::string medEntry; // takes each med record
        while (std::getline(medStream, medEntry, ';')) { // takes one med record at a time and parses with ";"
            if (medEntry.empty()) continue;
            std::stringstream medParts(medEntry); // this one takes one part of one record, id, name ...
            std::string idStr, date, details;
            std::getline(medParts, idStr, '|');
            std::getline(medParts, date, '|');
            std::getline(medParts, details, '|');
            details = unescapeCommas(details);
            if (!idStr.empty()) {
                pet.addMedicalHistoryWithId(std::stoi(idStr), date, details); 
            }
            

        }

        // parse pet records

        std::stringstream petRecStream(petRecData);
        std::string recEntry;
        while (std::getline(petRecStream, recEntry, ';')) {
            if (recEntry.empty()) continue;
            std::stringstream recParts(recEntry);
            std::string idStr, date, details;
            std::getline(recParts, idStr, '|');
            std::getline(recParts, date, '|');
            std::getline(recParts, details, '|');
            details = unescapeCommas(details);
            if (!idStr.empty()) {
                pet.addPetRecordWithId(std::stoi(idStr), date, details);
            }
        }

        pets.push_back(pet);

    }

    file.close();
    return pets;
}

void Pet::displayAsTableRow(const std::vector<Owner>& owners) const {
    std::string ownerName = "None";
    if (ownerId != -1) {
        for (const auto& o : owners) {
            if (o.getOwnerId() == ownerId) {
                ownerName = o.getName();
                break;
            }
        }
    }
    std::cout << std::left 
              << std::setw(8) << petId 
              << std::setw(20) << truncatePet(capitalizeWords(name), 20) 
              << std::setw(21) << truncatePet(capitalizeWords(breed), 20) 
              << std::setw(6) << age 
              << std::setw(20) << truncatePet(capitalizeWords(ownerName), 20) 
              << "\n";
}


void Pet::writeToFileStream(std::ofstream& file) const {

    // serialize vectors into a single string
    std::ostringstream vaccinationsStream;
    for (size_t i = 0; i < vaccinations.size(); ++i) {
        vaccinationsStream << vaccinations[i].getId() << "|"
                        << vaccinations[i].getName() << "|"
                        << vaccinations[i].getDate() << "|"
                        << vaccinations[i].getStatus();
        if (i != vaccinations.size() - 1) vaccinationsStream << ";";
    }
     // Serialize medicalHistory
    std::ostringstream medicalHistoryStream;
    for (auto it = medicalHistory.begin(); it != medicalHistory.end(); ++it) {
        medicalHistoryStream << it->first << "|" 
                             << it->second.getDate() << "|" 
                             << escapeCommas(it->second.getDetails());
        auto nextIt = it; ++nextIt;
        if (nextIt != medicalHistory.end()) medicalHistoryStream << ";";
    }

    // Serialize petRecords
    std::ostringstream petRecordsStream;
    for (auto it = petRecords.begin(); it != petRecords.end(); ++it) {
        petRecordsStream << it->first << "|"
                         << it->second.getDate() << "|"
                         << escapeCommas(it->second.getDetails());
        auto nextIt = it; ++nextIt;
        if (nextIt != petRecords.end()) petRecordsStream << ";";
    }

    file << petId << "," << name << "," << breed << "," << age << "," << ownerId << "," << calculateVaccinationStatus() << ","
     << vaccinationsStream.str() << "," << medicalHistoryStream.str() << "," << petRecordsStream.str() << "\n";
}


void displayPetTableHeader() {
    std::cout << std::left
              << std::setw(8) << "Pet ID" 
              << std::setw(20) << "Name" 
              << std::setw(20) << "Breed" 
              << std::setw(6) << "Age" 
              << std::setw(20) << "Owner" 
              << "\n";
    std::cout << std::string(74, '-') << "\n";
}

std::string Pet::truncatePet(const std::string& text, size_t width) const {
    if (text.length() <= width) return text;
    return text.substr(0, width - 2) + "..";  // ASCII dots instead of Unicode
}
