// globals.cpp
#include "globals.h"
#include "Owner.h"
#include <fstream>
#include "Pet.h"
#include "Appointment.h"
#include "validations.h"

std::vector<Pet> pets;
std::vector<Owner> owners;
std::vector<Appointment> appointments;
// std::vector<User> users;
std::vector<std::unique_ptr<User>> users;


int nextPetId = 1;
int nextOwnerId = 1;
int nextAppointmentId = 1;
int nextUserId = 1;


std::string truncateDetails(const std::string& details, size_t maxLength) {
    if (details.length() <= maxLength) return details;
    return details.substr(0, maxLength - 3) + "...";
}

void saveAllOwnersToFile(const std::vector<Owner>& owners, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening " << filename << " for writing.\n";
        return;
    }
    // Write header
    file << "owner_id,name,address,phone_number,email,pet_ids,records\n";
    // Write each owner
    for (const auto& o : owners) {
        o.writeToFileStream(file);
    }
    file.close();
}

void saveAllPetsToFile(const std::vector<Pet>& pets, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening " << filename << " for writing.\n";
        return;
    }
    // headers
    file << "pet_id,name,breed,age,owner_id,vaccination_status,vaccinations,medical_history,pet_records\n";
    // fill each pet
    for (const auto& p : pets) {
        p.writeToFileStream(file);
    }
    std::cout << "\n";
    file.close();
}

std::string escapeCommas(const std::string& input) {
    std::string output = input;
    size_t pos = 0;
    while ((pos = output.find(",", pos)) != std::string::npos) {
        output.replace(pos, 1, "[comma]");
        pos += 7; // move past the replaced part
    }
    return output;
}

std::string unescapeCommas(const std::string& input) {
    std::string output = input;
    size_t pos = 0;
    while ((pos = output.find("[comma]", pos)) != std::string::npos) {
        output.replace(pos, 7, ",");
        pos += 1;
    }
    return output;
}

bool displayFullRecordIfExists(const std::map<int, Record>& records, int recordId, const std::string& recordType) {
    auto it = records.find(recordId);
    if (it != records.end()) {
        displayFullRecord(records, recordId, recordType);
        return true;
    }
    return false;
}

void displayFullRecord(const std::map<int, Record>& records, int recordId, const std::string& recordType) {
    auto it = records.find(recordId);
    if (it != records.end()) {
        std::cout << "\n📄 ----- " << recordType << " Details -----\n";
        std::cout << "🆔 Record ID : " << it->first << "\n";
        std::cout << "📅 Date      : " << it->second.getDate() << "\n";
        std::cout << "📝 Details   : " << it->second.getDetails() << "\n";
        std::cout << "------------------------------\n";
    } else {
        std::cout << "❌ " << recordType << " with ID " << recordId << " not found.\n";
    }
}



// appointments
void saveAllAppointmentsToFile(const std::vector<Appointment>& appointments) {
    std::ofstream file("appointments.csv");
    if (!file) {
        std::cerr << "Error opening appointments.csv for writing.\n";
        return;
    }

    file << "appointment_id,owner_id,pet_id,date,time,purpose,status\n";
    for (const auto& appt : appointments) {
        file << appt.getAppointmentId() << ","
             << appt.getOwnerId() << ","
             << appt.getPetId() << ","
             << appt.getDate() << ","
             << appt.getTime() << ","
             << escapeCommas(appt.getPurpose()) << ","
             << appt.getStatus() << "\n";
    }
    file.close();
}

std::vector<Appointment> loadAllAppointmentsFromFile(const std::string& filename) {
    return Appointment::loadFromFile(filename);
}


std::vector<Appointment> getAppointmentsForPet(int petId) {
    std::vector<Appointment> result;
    for (const auto& appt : appointments) {
        if (appt.getPetId() == petId) {
            result.push_back(appt);
        }
    }
    return result;
}


void displayUnassignedPets(const std::vector<Pet>& pets) {
    bool found = false;
    std::cout << "\n🐾 --- Unassigned Pets ---\n";
    std::cout << std::left << std::setw(6)  << "ID" 
              << std::setw(20) << "Name" 
              << std::setw(22) << "Breed" 
              << std::setw(6)  << "Age" << "\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& pet : pets) {
        if (pet.getOnwerId() == -1) {
            found = true;
            std::cout << std::setw(6)  << pet.getPetId() 
                      << std::setw(20) << pet.getName() 
                      << std::setw(20) << pet.getBreed() 
                      << std::setw(6)  << pet.getAge() << "\n";
        }
    }
    if (!found) {
        std::cout << "📭 No unassigned pets found.\n";
    } else {
        std::cout << std::string(50, '-') << "\n";
    }
}


void promptToViewFullAppointment(const std::vector<Appointment>& appointments) {
    if (appointments.empty()) return;

    if (promptYesNo("🔍 View full details of any appointment?")) {
        while (true) {
            int apptId = askForValidId("🔢 Enter Appointment ID to view (or press Enter/0 to return): ");
            if (apptId == 0) break;

            auto it = std::find_if(appointments.begin(), appointments.end(), [apptId](const Appointment& appt) {
                return appt.getAppointmentId() == apptId;
            });

            if (it != appointments.end()) {
                it->displayAppointmentDetails();
                break;
            } else {
                std::cout << "❌ Appointment ID not found in this list. Please try again.\n";
            }
        }
    }
}
