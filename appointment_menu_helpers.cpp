#include "appointment_menu_helpers.h"
#include <iostream>
#include "validations.h"
#include "globals.h"
#include "owner_menu_helpers.h"
#include "Appointment.h"
#include <vector>


void viewAllAppointments(const std::vector<Appointment>& appointments) {
    if (appointments.empty()) {
        std::cout << "ℹ️ No appointments found.\n";
        return;
    }

    Appointment::displayAppointmentsTable(appointments);
}

void searchAppointmentById(std::vector<Appointment>& appointments) {
    while (true) {
        int id = askForValidId("🔍 Enter Appointment ID to search (or press Enter/0 to return): ");
        if (id == 0) break;

        Appointment* appt = findAppointmentById(appointments, id);
        if (appt) {
            appt->displayAppointmentDetails();
            if (!promptYesNo("🔁 View another appointment?")) break;
        } else {
            std::cout << "❌ Appointment ID not found.\n";
        }
    }
}

void searchAppointmentsByOwnerId(const std::vector<Appointment>& appointments) {
    while (true) {
        int ownerId = askForValidId("🔍 Enter Owner ID to search appointments (or press Enter/0 to return): ");
        if (ownerId == 0) break;

        std::vector<Appointment> matches;
        for (const auto& appt : appointments) {
            if (appt.getOwnerId() == ownerId) {
                matches.push_back(appt);
            }
        }

        if (matches.empty()) {
            std::cout << "❌ No appointments found for Owner ID " << ownerId << ".\n";
            continue;
        }

        Appointment::displayAppointmentsTable(matches);
        promptToViewFullAppointment(matches);
        if (!promptYesNo("🔁 Search appointments for another owner?")) break;
    }
}


void searchAppointmentsByPetId(const std::vector<Appointment>& appointments) {
    while (true) {
        int petId = askForValidId("🔍 Enter Pet ID to search appointments (or press Enter/0 to return): ");
        if (petId == 0) break;

        std::vector<Appointment> matches;
        for (const auto& appt : appointments) {
            if (appt.getPetId() == petId) {
                matches.push_back(appt);
            }
        }

        if (matches.empty()) {
            std::cout << "❌ No appointments found for Pet ID " << petId << ".\n";
            continue;
        }

        Appointment::displayAppointmentsTable(matches);
        promptToViewFullAppointment(matches);


        if (!promptYesNo("🔁 Search appointments for another pet?")) break;
    }
}


void addAppointment(std::vector<Appointment>& appointments, std::vector<Owner>& owners, std::vector<Pet>& pets, int& nextAppointmentId) {
    while (true) {
        int ownerId = askForValidId("➕ Enter Owner ID to add appointment (or press Enter/0 to return): ");
        if (ownerId == 0) break;

        Owner* owner = findOwnerById(owners, ownerId);
        if (!owner) {
            std::cout << "❌ Owner ID " << ownerId << " not found.\n";
            continue;
        }

        auto& petIds = owner->getPetIdsRef();
        if (petIds.empty()) {
            std::cout << "⚠️ Owner has no linked pets. Cannot create appointment.\n";
            continue;
        }

        std::cout << "🐾 Linked pets:\n";
        for (int pid : petIds) {
            Pet* pet = findPetById(pets, pid);
            if (pet) {
                std::cout << "   - Pet ID: " << pet->getPetId() << " | Name: " << pet->getName() << "\n";
            }
        }

        int petId = 0;
        while (true) {
            petId = askForValidId("🐾 Enter Pet ID for appointment (or press Enter/0 to return): ");
            if (petId == 0) break;

            Pet* pet = findPetById(pets, petId);
            bool isLinked = std::find(petIds.begin(), petIds.end(), petId) != petIds.end();

            if (pet && isLinked) break;
            std::cout << "❌ Pet ID not valid or not linked to Owner ID " << ownerId << ".\n";
        }

        if (petId == 0) continue;

        std::string date = askForValidAppointmentDate("📅 Enter appointment date (YYYY-MM-DD) (or press Enter/0 to return): ", true);
        if (date.empty()) {
            std::cout << "❌ Appointment creation cancelled.\n";
            continue;
        }

        std::string time = askForValidAppointmentTime("⏰ Enter appointment time (HH:MM) (or press Enter/0 to return): ");
        if (time.empty()) {
            std::cout << "❌ Appointment creation cancelled.\n";
            continue;
        }

        std::string purpose = askForValidAppointmentPurpose("📝 Enter purpose (max 100 chars) (or press Enter/0 to return): ");
        if (purpose.empty()) {
            std::cout << "❌ Appointment creation cancelled.\n";
            continue;
        }

        std::string status = "scheduled";
        int appointmentId = nextAppointmentId++;

        appointments.emplace_back(appointmentId, ownerId, petId, date, time, purpose, status);
        saveAllAppointmentsToFile(appointments);

        std::cout << "✅ Appointment (ID: " << appointmentId << ") created successfully.\n";
        if (!promptYesNo("➕ Add another appointment?")) break;
    }

    std::cout << "↩️ Returning to Appointment Menu...\n";
}

void updateAppointment(std::vector<Appointment>& appointments) {
    while (true) {
        int id = askForValidId("✏️ Enter Appointment ID to update (or press Enter/0 to return): ");
        if (id == 0) break;

        Appointment* appt = findAppointmentById(appointments, id);
        if (!appt) {
            std::cout << "❌ Appointment ID not found.\n";
            continue;
        }

        std::cout << "\n📋 Current Appointment Details:\n";
        appt->displayAppointmentDetails();

        std::cout << "\n🔧 Update Fields (Enter to keep, 0 to cancel any field)\n";

        std::string newDate = askForUpdatedAppointmentDate(appt->getDate(), true);
        if (newDate == "0") {
            std::cout << "❌ Update cancelled.\n";
            continue;
        }

        std::string newTime = askForUpdatedAppointmentTime(appt->getTime());
        if (newTime == "0") {
            std::cout << "❌ Update cancelled.\n";
            continue;
        }

        std::string newPurpose = askForUpdatedAppointmentPurpose(appt->getPurpose());
        if (newPurpose == "0") {
            std::cout << "❌ Update cancelled.\n";
            continue;
        }

        std::string newStatus = askForUpdatedAppointmentStatus(appt->getStatus());
        if (newStatus == "0") {
            std::cout << "❌ Update cancelled.\n";
            continue;
        }

        if (!newDate.empty()) appt->setDate(newDate);
        if (!newTime.empty()) appt->setTime(newTime);
        if (!newPurpose.empty()) appt->setPurpose(newPurpose);
        if (!newStatus.empty()) appt->setStatus(newStatus);

        saveAllAppointmentsToFile(appointments);
        std::cout << "✅ Appointment updated successfully.\n";

        if (!promptYesNo("✏️ Update another appointment?")) break;
    }

    std::cout << "↩️ Returning to Appointment Menu...\n";
}

void deleteAppointment(std::vector<Appointment>& appointments) {
    while (true) {
        int id = askForValidId("🗑️ Enter Appointment ID to delete (or press Enter/0 to return): ");
        if (id == 0) break;

        auto it = std::find_if(appointments.begin(), appointments.end(),
            [id](const Appointment& a) {
                return a.getAppointmentId() == id;
            });

        if (it == appointments.end()) {
            std::cout << "❌ Appointment ID not found.\n";
            continue;
        }

        std::cout << "\n📋 Appointment Details:\n";
        it->displayAppointmentDetails();

        if (promptYesNo("⚠️ Are you sure you want to delete this appointment?")) {
            appointments.erase(it);
            saveAllAppointmentsToFile(appointments);
            std::cout << "✅ Appointment deleted successfully.\n";

            if (!promptYesNo("🗑️ Delete another appointment?")) break;
        } else {
            std::cout << "❌ Deletion cancelled.\n";
        }
    }

    std::cout << "↩️ Returning to Appointment Menu...\n";
}