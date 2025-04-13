#include "owner_menu_helpers.h"
#include <iostream>
#include "validations.h"
#include "globals.h"

void addNewOwner() {
    while (true) {
        int ownerId;

        std::string name = askForValidName("👤 Enter the name of the Owner (or press Enter/0 to cancel): ", true);
        if (name.empty()) {
            std::cout << "❌ Owner creation cancelled.\n";
            break;
        }

        std::string address = askForValidAddress("🏠 Enter the address of the Owner (or press Enter/0 to cancel): ", true);
        if (address.empty()) {
            std::cout << "❌ Owner creation cancelled.\n";
            break;
        }
        std::string phone;
        while (true) {

        
            phone = askForValidPhoneNumber("📞 Enter the phone number (07XXXXXXXXX) (or press Enter/0 to cancel): ", true);
            if (phone.empty()) {
                std::cout << "❌ Owner creation cancelled.\n";
                return;
            }
            bool exists = false;
            for (const auto& o : owners) {
                if (o.getPhoneNumber() == phone) {
                    std::cout << "❌ An owner with that phone number already exists. Please enter a different one.\n";
                    exists = true;
                    break;
                }
            }
            if (!exists) break; // valid, not duplicate
        }


        std::string email;
        while (true) {

        
            email = askForValidEmail("📧 Enter the email (user@example.com) (or press Enter/0 to cancel): ", true);
            if (email.empty()) {
                std::cout << "❌ Owner creation cancelled.\n";
                return;
            }
            bool exists = false;
            for (const auto& o : owners) {
                if (o.getEmail() == email) {
                    std::cout << "❌ An owner with that email already exists. Please enter a different one.\n";
                    exists = true;
                    break;
                }
            }
            if (!exists) break; // valid, not duplicate

        }

        ownerId = nextOwnerId;
        Owner owner(ownerId, name, address, phone, email);
        owners.push_back(owner);
        saveAllOwnersToFile(owners);
        nextOwnerId++;

        std::cout << "✅ New owner added successfully! (🆔: " << ownerId << ")\n";

        if (!promptYesNo("➕ Would you like to add another owner?")) {
            break;
        }
    }

    std::cout << "🔙 Returning to Manage Owners Menu...\n";
    return;
}



void viewAllOwners() {
    if (owners.empty()) {
        std::cout << "⚠️  No owners found in the system.\n";
        return;
    }

    std::cout << "\n📋 All Registered Owners:\n";
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << std::left
              << std::setw(7)  << "ID"
              << std::setw(25) << "Name"
              << std::setw(18) << "Phone"
              << "Email" << "\n";
    std::cout << "-----------------------------------------------------------------------------------\n";

    for (const auto& o : owners) {
        std::cout << std::left
                  << std::setw(7)  << o.getOwnerId()
                  << std::setw(25) << o.getName()
                  << std::setw(18) << o.getPhoneNumber()
                  << o.getEmail() << "\n";
    }

    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "📊 Total number of owners: " << owners.size() << "\n";
}


void viewOwnerDetailsById() {
    while (true) {
        int id = askForValidId("🔍 Enter Owner 🆔 to view details (or press Enter/0 to return): ");
        if (id == 0) break;

        auto it = std::find_if(owners.begin(), owners.end(), [&](const Owner& o) {
            return o.getOwnerId() == id;
        });

        if (it == owners.end()) {
            std::cout << "❌ Owner with 🆔 " << id << " not found. Please try again.\n";
            continue;
        }

        it->displayOwnerDetails();
        it->displayLinkedPets(pets);

        std::cout << "===============================\n";

        if (!promptYesNo("🔁 Would you like to view another owner?")) break;
    }

    std::cout << "🔙 Returning to Manage Owners Menu...\n";
}



void updateOwner() {
    while (true) {
        int id = askForValidId("🛠️ Enter Owner 🆔 to update (press Enter/0 to return): ");
        if (id == 0) break;

        Owner* owner = findOwnerById(owners, id);
        if (!owner) {
            std::cout << "❌ No owner found with 🆔 " << id << ". Please try again.\n";
            continue;
        }

        std::cout << "\n📋 You are updating the following owner:\n";
        owner->displayOwnerDetails();

        std::string newName = askForUpdatedName("👤 Enter new name (press Enter to keep the existing, 0 to cancel): ");
        if (newName == "0") {
            std::cout << "❌ Update cancelled.\n";
            return;
        }

        std::string newAddress = askForUpdatedAddress("🏠 Enter new address (press Enter to keep the existing, 0 to cancel): ");
        if (newAddress == "0") {
            std::cout << "❌ Update cancelled.\n";
            return;
        }

        std::string newPhone;
        while (true) {

        
            newPhone = askForUpdatedPhoneNumber("📞 Enter new phone number (press Enter to keep existing, 0 to cancel): ");
            if (newPhone == "0") {
                std::cout << "❌ Update cancelled.\n";
                return;
            }
            // Only check uniqueness if user provided a new value
            if (!newPhone.empty() && newPhone != owner->getPhoneNumber()) {
                bool exists = false;
                for (const auto& o : owners) {
                    if (o.getPhoneNumber() == newPhone && o.getOwnerId() != owner->getOwnerId()) {
                        std::cout << "❌ Another owner already has that phone number. Please enter a different one.\n";
                        exists = true;
                        break;
                    }
                }
                if (exists) continue;
            }
            break;
        }
        std::string newEmail;
        while (true) {

        
            newEmail = askForUpdatedEmail("📧 Enter new email (press Enter to keep existing, 0 to cancel): ");
            if (newEmail == "0") {
                std::cout << "❌ Update cancelled.\n";
                return;
            }

            if (!newEmail.empty() && newEmail != owner->getEmail()) {
                bool exists = false;
                for (const auto& o : owners) {
                    if (o.getEmail() == newEmail && o.getOwnerId() != owner->getOwnerId()) {
                        std::cout << "❌ Another owner already has that email. Please enter a different one.\n";
                        exists = true;
                        break;
                    }
                }
                if (exists) continue;
            }
            break;
        }

        // Preview changes
        std::cout << "\n🔍 Review the changes before applying:\n";
        std::cout << "👤 Name         : " << (newName.empty() ? owner->getName() : newName) << "\n";
        std::cout << "🏠 Address      : " << (newAddress.empty() ? owner->getAddress() : newAddress) << "\n";
        std::cout << "📞 Phone Number : " << (newPhone.empty() ? owner->getPhoneNumber() : newPhone) << "\n";
        std::cout << "📧 Email        : " << (newEmail.empty() ? owner->getEmail() : newEmail) << "\n";

        if (promptYesNo("✅ Apply these changes?")) {
            if (!newName.empty()) owner->setName(newName);
            if (!newAddress.empty()) owner->setAddress(newAddress);
            if (!newPhone.empty()) owner->setPhoneNumber(newPhone);
            if (!newEmail.empty()) owner->setEmail(newEmail);

            saveAllOwnersToFile(owners);
            std::cout << "✅ Owner updated successfully.\n";
        } else {
            std::cout << "❌ Update cancelled.\n";
            return;
        }

        if (!promptYesNo("🔁 Would you like to update another owner?")) {
            break;
        }
    }

    std::cout << "🔙 Returning to Manage Owners Menu...\n";
}




void deleteOwner() {
    while (true) {
        int id = askForValidId("🗑️ Enter Owner 🆔 to delete (press Enter/0 to return): ");
        if (id == 0) break;

        Owner* owner = findOwnerById(owners, id);
        if (!owner) {
            std::cout << "❌ Owner 🆔 " << id << " not found. Please try again.\n";
            continue;
        }

        std::cout << "\n📋 You are about to delete the following owner:\n";
        owner->displayOwnerDetails();
        owner->displayLinkedPets(pets);

        const auto& linkedPetIds = owner->getPetIds();
        if (!linkedPetIds.empty()) {
            std::cout << "\n⚠️ This owner has " << linkedPetIds.size()
                      << " linked pet(s). They will be unassigned but NOT deleted.\n";
        }

        int linkedApptCount = std::count_if(appointments.begin(), appointments.end(),
            [id](const Appointment& appt) { return appt.getOwnerId() == id; });
        if (linkedApptCount > 0) {
            std::cout << "📅 This owner is linked to " << linkedApptCount
                      << " appointment(s). They will be unassigned as well.\n";
        }

        if (!promptYesNo("⚠️ Are you sure you want to delete this owner?")) {
            std::cout << "❌ Deletion cancelled.\n";
            break;
        }

        for (int petId : linkedPetIds) {
            Pet* pet = findPetById(pets, petId);
            if (pet) pet->setOwnerId(-1);
        }

        for (auto& appt : appointments) {
            if (appt.getOwnerId() == id) {
                appt.setOwnerId(-1);
            }
        }

        owners.erase(std::remove_if(owners.begin(), owners.end(),
            [id](const Owner& o) { return o.getOwnerId() == id; }),
            owners.end());

        saveAllOwnersToFile(owners);
        saveAllPetsToFile(pets);
        saveAllAppointmentsToFile(appointments);

        std::cout << "✅ Owner and all links removed successfully.\n";

        if (!promptYesNo("🗑️ Would you like to delete another owner?")) {
            break;
        }
    }

    std::cout << "🔙 Returning to Manage Owners Menu...\n";
}



void viewPetsLinkedToOwner() {
    while (true) {
        int id = askForValidId("🐾 Enter Owner 🆔 to view linked pets (or press Enter/0 to return): ");
        if (id == 0) break;

        Owner* owner = findOwnerById(owners, id);
        if (!owner) {
            std::cout << "❌ Owner with 🆔 " << id << " not found. Please try again.\n";
            continue;  
        }

        const std::vector<int>& petIds = owner->getPetIds();
        std::cout << "\n📋 Owner: " << owner->getName() << " (🆔 " << owner->getOwnerId() << ")\n";

        if (petIds.empty()) {
            std::cout << "🐾 This owner has no pets linked.\n";
        } else {
            std::cout << "\n🐾 Pets linked to this owner:\n";
            displayPetTableHeader();
            for (int petId : petIds) {
                Pet* pet = findPetById(pets, petId);
                if (pet) {
                    pet->displayAsTableRow(owners);
                } else {
                    std::cout << std::left
                              << std::setw(8)  << petId
                              << std::setw(20) << "(Missing Pet)"
                              << "⚠️ Not found in pet list\n";
                }
            }
            std::cout << "--------------------------------------------------------------------------\n";
        }

        if (!promptYesNo("🔁 Would you like to check another owner?")) break;
    }

    std::cout << "🔙 Returning to Manage Owners Menu...\n";
}





void manageOwnerRecords() {
    while (true) {
        std::cout << "\n📂 <<<<< Manage Owner Records >>>>>\n";
        std::cout << "1. ➕ Add Record\n";
        std::cout << "2. 📄 View Records\n";
        std::cout << "3. ✏️  Update Record\n";
        std::cout << "4. 🗑️  Delete Record\n";
        std::cout << "0. ↩️  Return to Owner Menu\n";

        int choice = askForMenuChoice(0, 4);
        if (choice == 0) break;

        switch (choice) {
        case 1: { // Add Record
            while (true) {
                int id = askForValidId("🆔 Enter Owner ID to add record (or press Enter/0 to return): ");
                if (id == 0) break;

                Owner* owner = findOwnerById(owners, id);
                if (!owner) {
                    std::cout << "❌ Owner ID " << id << " not found.\n";
                    break;
                }

                std::cout << "📝 Adding record for: " << owner->getName() << " (ID: " << id << ")\n";

                std::string date = askForValidDate("📅 Enter record date (YYYY-MM-DD) (or press Enter/0 to cancel): ", false, true);
                if (date.empty()) {
                    std::cout << "❌ Record creation cancelled.\n";
                    break;
                }

                std::string details = askForValidDetails("🧾 Enter record details (max 200 characters) (or press Enter/0 to cancel): ", 200, true);
                if (details.empty()) {
                    std::cout << "❌ Record creation cancelled.\n";
                    break;
                }

                owner->addRecord(date, details);
                saveAllOwnersToFile(owners);
                std::cout << "✅ Record added successfully.\n";

                if (!promptYesNo("➕ Add another record?")) break;
            }
            break;
        }

        case 2: { // View Records
                    while (true) {
                        int id = askForValidId("🆔 Enter Owner ID to view records (or press Enter/0 to return): ");
                        if (id == 0) break;

                        Owner* owner = findOwnerById(owners, id);
                        if (!owner) {
                            std::cout << "❌ Owner ID " << id << " not found.\n";
                            continue;
                        }

                        owner->displayRecordTable();

                        // ✅ Skip full view prompt if no records
                        if (owner->getRecords().empty()) {
                            if (!promptYesNo("📄 View records for another owner?")) break;
                            continue;
                        }

                        if (promptYesNo("🔍 View full details of a record?")) {
                            while (true) {
                                int recId = askForValidId("🔢 Enter Record ID (or press Enter/0 to return): ");
                                if (recId == 0) break;

                                if (!owner->hasRecord(recId)) {
                                    std::cout << "❌ Record with ID " << recId << " not found. Please try again.\n";
                                    continue;
                                }

                                owner->displayFullRecord(recId);
                                break; // success, exit view loop
                            }
                        }

                        if (!promptYesNo("📄 View records for another owner?")) break;
                    }
                    break;
                }


        case 3: { // Update Record
                while (true) {
                    int id = askForValidId("🆔 Enter Owner ID to update a record (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Owner* owner = findOwnerById(owners, id);
                    if (!owner) {
                        std::cout << "❌ Owner ID " << id << " not found.\n";
                        continue;
                    }

                    if (owner->getRecords().empty()) {
                        std::cout << "ℹ️ No records found for this owner.\n";
                        continue;
                    }

                    owner->displayRecordTable();

                    int recordId;
                    while (true) {
                        recordId = askForValidId("🔢 Enter Record ID to update (or press Enter/0 to return): ");
                        if (recordId == 0) break;

                        if (!owner->hasRecord(recordId)) {
                            std::cout << "❌ Record ID " << recordId << " not found. Please try again.\n";
                            continue;
                        }
                        break;
                    }

                    if (recordId == 0) continue;

                    const Record& current = owner->getRecords().at(recordId);
                    std::cout << "\n🛠️ Updating record:\n";
                    current.displayFull(recordId);

                    std::string newDate = askForUpdatedDate("📅 New date (Enter to keep, 0 to cancel): ", false);
                    if (newDate == "0") {
                        std::cout << "❌ Update cancelled.\n";
                        continue;
                    }
                    if (newDate.empty()) newDate = current.getDate();

                    std::string newDetails = askForUpdatedDetails("🧾 New details (Enter to keep, 0 to cancel): ");
                    if (newDetails == "0") {
                        std::cout << "❌ Update cancelled.\n";
                        continue;
                    }
                    if (newDetails.empty()) newDetails = current.getDetails();

                    owner->updateRecord(recordId, newDate, newDetails);
                    saveAllOwnersToFile(owners);
                    std::cout << "✅ Record updated successfully.\n";

                    if (!promptYesNo("✏️  Update another record?")) break;
                }
                break;
            }


        case 4: { // Delete Record
            while (true) {
                int ownerId = askForValidId("🆔 Enter Owner ID to delete a record (or press Enter/0 to return): ");
                if (ownerId == 0) break;

                Owner* owner = findOwnerById(owners, ownerId);
                if (!owner) {
                    std::cout << "❌ Owner ID " << ownerId << " not found.\n";
                    continue;
                }

                if (owner->getRecords().empty()) {
                    std::cout << "ℹ️ No records found for this owner.\n";
                    continue;
                }

                while (true) {
                    owner->displayRecordTable();

                    int recordId = askForValidId("🗑️ Enter Record ID to delete (or press Enter/0 to return): ");
                    if (recordId == 0) break;

                    if (!owner->hasRecord(recordId)) {
                        std::cout << "❌ Record ID " << recordId << " not found.\n";
                        continue;
                    }

                    std::cout << "⚠️ You are about to delete this record:\n";
                    owner->displayFullRecord(recordId);

                    if (promptYesNo("❗ Are you sure you want to delete this record?")) {
                        owner->deleteRecord(recordId);
                        saveAllOwnersToFile(owners);
                        std::cout << "✅ Record deleted successfully.\n";

                        if (!owner->hasRecords()) {
                            std::cout << "ℹ️ No more records left for this owner.\n";
                            break;
                        }

                        if (!promptYesNo("🗑️ Delete another record for this owner?")) break;
                    } else {
                        std::cout << "❌ Deletion cancelled.\n";
                    }
                }
            }
            break;
        }

        default:
            break;
        }

        std::cout << "\n↩️ Returning to Manage Owner Records Menu...\n";
    }
}


void manageOwnerAppointments() {
    while (true) {
        std::cout << "\n📅 <<<<< Manage Owner Appointments >>>>>\n";
        std::cout << "1. ➕ Add Appointment (By Owner ID)\n";
        std::cout << "2. 📄 View Appointment History (By Owner ID)\n";
        std::cout << "3. ✏️  Update Appointment Status (By Owner ID & Appointment ID)\n";
        std::cout << "0. ↩️  Return to Owner Menu\n";

        int choice = askForMenuChoice(0, 3);
        if (choice == 0) break;

        switch (choice) {
        case 1: { // Add Appointment
            while (true) {
                int ownerId = askForValidId("🆔 Enter Owner ID to add appointment (or press Enter/0 to return): ");
                if (ownerId == 0) break;

                Owner* owner = findOwnerById(owners, ownerId);
                if (!owner) {
                    std::cout << "❌ Owner ID " << ownerId << " not found.\n";
                    continue;
                }

                if (owner->getPetIdsRef().empty()) {
                    std::cout << "⚠️ Owner has no linked pets. Cannot create appointment.\n";
                    continue;
                }

                std::cout << "🐾 Linked pets:\n";
                for (int pid : owner->getPetIdsRef()) {
                    Pet* pet = findPetById(pets, pid);
                    if (pet) {
                        std::cout << "   - Pet ID: " << pet->getPetId() << " | Name: " << pet->getName() << "\n";
                    }
                }

                int petId = 0;
                while (true) {
                    petId = askForValidId("🐾 Enter Pet ID for the appointment (or press Enter/0 to return): ");
                    if (petId == 0) break;

                    bool isLinked = std::find(owner->getPetIdsRef().begin(), owner->getPetIdsRef().end(), petId) != owner->getPetIdsRef().end();
                    Pet* pet = findPetById(pets, petId);

                    if (pet && isLinked) break;
                    std::cout << "❌ Invalid or unlinked Pet ID. Please try again.\n";
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
            break;
        }

        case 2: { // View Appointment History
            while (true) {
                int ownerId = askForValidId("🆔 Enter Owner ID to view appointments (or press Enter/0 to return): ");
                if (ownerId == 0) break;

                std::vector<Appointment> ownerAppointments;
                for (const auto& appt : appointments) {
                    if (appt.getOwnerId() == ownerId) {
                        ownerAppointments.push_back(appt);
                    }
                }

                if (ownerAppointments.empty()) {
                    std::cout << "ℹ️ No appointments found for Owner ID " << ownerId << ".\n";
                    continue;
                }

                Appointment::displayAppointmentsTable(ownerAppointments);

                if (promptYesNo("🔍 View full details of any appointment?")) {
                    while (true) {
                        int apptId = askForValidId("🔢 Enter Appointment ID (or press Enter/0 to return): ");
                        if (apptId == 0) break;

                        auto it = std::find_if(ownerAppointments.begin(), ownerAppointments.end(),
                            [apptId](const Appointment& a) { return a.getAppointmentId() == apptId; });

                        if (it != ownerAppointments.end()) {
                            it->displayAppointmentDetails();
                        } else {
                            std::cout << "❌ Appointment ID not found. Please try again.\n";
                        }
                    }
                }

                if (!promptYesNo("📄 View appointments for another owner?")) break;
            }
            break;
        }

        case 3: { // Update Appointment Status
            while (true) {
                int ownerId = askForValidId("🆔 Enter Owner ID to update appointment (or press Enter/0 to return): ");
                if (ownerId == 0) break;

                std::vector<Appointment*> ownerAppointments;
                for (auto& appt : appointments) {
                    if (appt.getOwnerId() == ownerId) {
                        ownerAppointments.push_back(&appt);
                    }
                }

                if (ownerAppointments.empty()) {
                    std::cout << "ℹ️ No appointments found for Owner ID " << ownerId << ".\n";
                    continue;
                }

                while (true) {
                    Appointment::displayAppointmentsTable(ownerAppointments);

                    int apptId = askForValidId("🔢 Enter Appointment ID to update (or press Enter/0 to return): ");
                    if (apptId == 0) break;

                    Appointment* appt = nullptr;
                    for (Appointment* a : ownerAppointments) {
                        if (a->getAppointmentId() == apptId) {
                            appt = a;
                            break;
                        }
                    }

                    if (!appt) {
                        std::cout << "❌ Appointment ID not found. Please try again.\n";
                        continue;
                    }

                    appt->displayFullAppointment();

                    std::string status = askForValidAppointmentStatus("✏️ Enter new appointment status (or press Enter/0 to cancel): ");
                    if (status.empty()) {
                        std::cout << "❌ Status update cancelled.\n";
                        continue;
                    }

                    appt->updateStatus(status);
                    saveAllAppointmentsToFile(appointments);
                    std::cout << "✅ Appointment status updated successfully.\n";

                    if (!promptYesNo("✏️ Update another appointment for this owner?")) break;
                }
            }
            break;
        }
        }
    }

    std::cout << "↩️ Returning to the previous menu...\n";
}