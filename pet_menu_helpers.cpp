#include "Pet.h"
#include "validations.h"
#include "globals.h"
void addNewPet() {
    while (true) {
        std::string ownerIdStr;
        int ownerId = -1, petId;
        bool petAlreadySaved = false;

        std::string name = askForValidName("📛 Enter the name of the pet (or press Enter/0 to cancel): ", true);
        if (name.empty()) {
            std::cout << "❌ Pet creation canceled.\n";
            break;
        }
        std::string breed = askForValidName("🧬 Enter the breed of the pet (or press Enter/0 to cancel): ", true);
        if (breed.empty()) {
            std::cout << "❌ Pet creation canceled.\n";
            break;
        }
        int age = askForValidAge("🎂 Enter the age of the pet (or press Enter/0 to cancel): ", true);
        if (age == -1) {
            std::cout << "❌ Pet creation canceled.\n";
            break;
        }

        // Handle owner
        while (true) {
            owner_creation:
            int ownerInput = askForOwnerIdOrOption("👤 Enter owner ID, 'c/C' to create new owner, 'n/N' for no owner (or Enter/0 to cancel): ");
            
            if (ownerInput == 0) {
                std::cout << "❌ Pet creation canceled.\n";
                goto exit_pet_creation;
            }
            else if (ownerInput == -1) {  // 'n' or 'N'
                std::cout << "✅ Pet created successfully.\n";
                ownerId = -1;
                break;
            }
            else if (ownerInput == -2) {  // 'c' or 'C'
                // Create new owner
                std::string owner_name = askForValidName("👤 Enter Owner name (or press Enter/0 to cancel): ", true);
                if (owner_name.empty()) {
                    std::cout << "❌ Owner creation cancelled.\n";
                    continue;
                }

                std::string owner_address = askForValidAddress("🏠 Enter Owner Address (or press Enter/0 to cancel): ", true);
                if (owner_address.empty()) {
                    std::cout << "❌ Owner creation cancelled.\n";
                    continue;
                }

                std::string owner_phone;
                while (true) {
                    owner_phone = askForValidPhoneNumber("📞 Enter Owner phone number (07XXXXXXXXX)(or press Enter/0 to cancel): ", true);
                    if (owner_phone.empty()) {
                        std::cout << "❌ Owner creation cancelled.\n";
                        goto owner_creation;
                        // continue;
                    }
                    if (isPhoneNumberTaken(owner_phone)) {
                        std::cout << "⚠️ Phone number already exists. Please enter a different one.\n";
                        continue;
                    }
                    break;
                }

                std::string owner_email;
                while (true) {
                    owner_email = askForValidEmail("📧 Enter Owner Email (user@example.com)(or press Enter/0 to cancel): ", true);
                    if (owner_email.empty()) {
                        std::cout << "🚫 Owner creation cancelled.\n";
                        goto owner_creation;
                        // continue;
                    }
                    if (isEmailTaken(owner_email)) {
                        std::cout << "⚠️ Email already exists. Please enter a different one.\n";
                        continue;
                    }
                    break;
                }

                ownerId = nextOwnerId;
                Owner newOwner(ownerId, owner_name, owner_address, owner_phone, owner_email);

                petId = nextPetId;
                Pet pet(petId, name, breed, age, ownerId);
                pets.push_back(pet);
                saveAllPetsToFile(pets);
                nextPetId++;
                petAlreadySaved = true;

                newOwner.addPetId(petId);
                owners.push_back(newOwner);
                saveAllOwnersToFile(owners);
                nextOwnerId++;

                std::cout << "✅ Owner created with ID " << ownerId << " and pet linked.\n";
                break;
            }
            else {  // Valid numeric ID
                ownerId = ownerInput;
                bool found = false;
                for (auto& o : owners) {
                    if (o.getOwnerId() == ownerId) {
                        std::cout << "✅ Owner ID " << ownerId << " found:\n";
                        o.displayOwnerDetails();

                        if (promptYesNo("🤔 Are you sure you want to link this pet to this owner?")) {
                            found = true;
                            break;
                        }
                        else {
                            std::cout << "❌ Linking canceled. Please enter another owner ID.\n";
                            continue;
                        }
                    }
                }

                if (found) {
                    std::cout << "✅ Pet created successfully.\n";
                    break;
                }
                else {
                    std::cout << "⚠️ Owner with ID " << ownerId << " not found. Try again.\n";
                }
            }
        }

        if (!petAlreadySaved) {
            petId = nextPetId;
            Pet pet(petId, name, breed, age, ownerId);
            pets.push_back(pet);
            saveAllPetsToFile(pets);
            nextPetId++;

            if (ownerId != -1) {
                for (auto& o : owners) {
                    if (o.getOwnerId() == ownerId) {
                        o.addPetId(petId);
                        saveAllOwnersToFile(owners);
                        break;
                    }
                }
            }
        }

        if (!promptYesNo("🔁 Would you like to add another pet?")) {
            break;
        }
    }

exit_pet_creation:
    return;
}




void viewAllPets() {
    if (pets.empty()) {
        std::cout << "🚫 No pets found in the system.\n";
    } else {
        std::cout << "\n📄 All the pets from 'pets.csv' file:\n";
        std::cout << "📊 Displaying pet records...\n";
        displayPetTableHeader();
        for (auto& p : pets) {
            p.displayAsTableRow(owners);
        }
        std::cout << "--------------------------------------------------------------------------\n";
        std::cout << "➕ Total pets: 🐾 " << pets.size() << " #️⃣\n";
    }
}


void viewPetDetailsById() {
    while (true) {
        int id = askForValidId("🔎 Enter Pet 🆔 to view details (or press Enter/0 to return): ");
        if (id == 0) break;

        auto it = std::find_if(pets.begin(), pets.end(), [&](const Pet& p) {
            return p.getPetId() == id;
        });

        if (it == pets.end()) {
            std::cout << "❌ Pet with ID " << id << " not found. Please try again.\n";
            continue;
        }

        std::cout << "\n📄 ----- Pet Details 🐾 -----\n";
        it->displayPetDetails(owners);
        std::cout << "-----------------------------\n";

        if (!promptYesNo("🔁 Would you like to view another pet?")) break;
    }
}

void updatePet() {
    std::cout << "🔄 Updating a pet...\n";

    while (true) {
        Pet* pet = nullptr;
        int petId = askForValidId("🔍 Enter Pet ID to update (or Enter/0 to cancel): ");
        if (petId == 0) {
            std::cout << "❌ Pet update canceled.\n";
            return; // Exit function entirely
        }

        pet = findPetById(pets, petId);
        if (!pet) {
            std::cout << "❌ Pet with ID " << petId << " not found. Please try again.\n";
            continue; // Back to Pet ID prompt
        }

        std::cout << "\n📋 Current Pet Details:\n";
        pet->displayPetDetails(owners);

        std::cout << "\n🔧 Press Enter to keep current value, or type a new one. (0 to cancel)\n";

        // Name
        std::string newNameTemp = askForValidNameForUpdate("📛 Enter new name (or press Enter to keep, 0 to cancel): ");
        if (newNameTemp == "0") {
            std::cout << "❌ Pet update canceled.\n";
            continue; // Back to Pet ID
        }

        // Breed
        std::string newBreedTemp = askForValidNameForUpdate("🧬 Enter new breed (or press Enter to keep, 0 to cancel): ");
        if (newBreedTemp == "0") {
            std::cout << "❌ Pet update canceled.\n";
            continue; // Back to Pet ID
        }

        // Age
        int newAgeTemp = askForValidAgeForUpdate("🎂 Enter new age (or press Enter to keep, 0 to cancel): ");
        if (newAgeTemp == -1) {
            std::cout << "❌ Pet update canceled.\n";
            continue; // Back to Pet ID
        }
        if (newAgeTemp == -2) {
            newAgeTemp = pet->getAge(); // Keep current age
        }

        // Owner update prompt with retry on invalid ID
        int currentOwnerId = pet->getOnwerId();
        std::string ownerPrompt = currentOwnerId == -1 
            ? "👤 Enter new owner ID (Enter for no owner, 0 to cancel): "
            : "👤 Enter new owner ID [current: " + std::to_string(currentOwnerId) + "] (Enter to keep, 0 to cancel): ";

        int newOwnerIdTemp;
        bool ownerInputDone = false;
        while (!ownerInputDone) {
            newOwnerIdTemp = askForOwnerIdUpdate(ownerPrompt, currentOwnerId);
            if (newOwnerIdTemp == -1) { // "0" entered, cancel
                std::cout << "❌ Pet update canceled.\n";
                ownerInputDone = true;
                continue; // This will trigger the outer continue
            }

            // If keeping current ID or setting to no owner (-1), proceed
            if (newOwnerIdTemp == currentOwnerId || newOwnerIdTemp == -1) {
                ownerInputDone = true;
                break;
            }

            // Validate new owner ID
            Owner* newOwner = findOwnerById(owners, newOwnerIdTemp);
            if (!newOwner) {
                std::cout << "❌ No owner found with 🆔 " << newOwnerIdTemp << ". Please try again.\n";
                continue; // Reprompt for owner ID
            }
            ownerInputDone = true; // Valid owner ID found
        }

        // If cancelled from owner prompt, restart from Pet ID
        if (newOwnerIdTemp == -1) {
            continue; // Back to Pet ID
        }

        // Prepare final values
        std::string newName = newNameTemp.empty() ? pet->getName() : newNameTemp;
        std::string newBreed = newBreedTemp.empty() ? pet->getBreed() : newBreedTemp;
        int finalOwnerId = newOwnerIdTemp; // Validated or kept
        bool ownerChanged = (finalOwnerId != currentOwnerId);

        // Display proposed changes and confirm
        std::cout << "\n--- Proposed Pet Updates ---\n";
        std::cout << "Name: " << newName << "\n";
        std::cout << "Breed: " << newBreed << "\n";
        std::cout << "Age: " << newAgeTemp << "\n";
        std::cout << "Owner ID: " << (finalOwnerId == -1 ? "None" : std::to_string(finalOwnerId)) << "\n";

        if (!promptYesNo("🤔 Are you sure you want to save these changes?")) {
            std::cout << "❌ Pet update canceled.\n";
            continue; // Back to Pet ID
        }

        // Apply updates if confirmed
        pet->setName(newName);
        pet->setBreed(newBreed);
        pet->setAge(newAgeTemp);

        if (ownerChanged) {
            if (currentOwnerId != -1) {
                Owner* oldOwner = findOwnerById(owners, currentOwnerId);
                if (oldOwner) oldOwner->removePetId(petId);
            }
            if (finalOwnerId != -1) {
                Owner* newOwner = findOwnerById(owners, finalOwnerId);
                if (newOwner) newOwner->addPetId(petId);
            }
            pet->setOwnerId(finalOwnerId);
            saveAllOwnersToFile(owners);
        }

        saveAllPetsToFile(pets);
        std::cout << "✅ Pet updated successfully.\n";
        break; // Exit loop after successful update
    }
}



void manageMedicalRecords() {
    int choice_m;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        std::cout << "\n<<<<< 🩺 Manage Medical Records >>>>>\n";
        std::cout << "1. ➕ Add Medical Record (By Pet 🆔)\n"; 
        std::cout << "2. 📄 View Medical History (By Pet 🆔)\n";  
        std::cout << "3. ✏️  Update Medical Record (By Pet 🆔 & Record 🆔)\n";  
        std::cout << "4. 🗑️  Delete Medical Record (By Pet 🆔 & Record 🆔)\n";  
        std::cout << "0. 🔙 Return to Pet Menu\n";
        choice_m = askForMenuChoice(0, 4);
        if (choice_m == 0) break;

        switch (choice_m)
        {
        case 1: // adding medical record
            {   
                bool addMore = true;
                while (addMore) {
                    int id = askForValidId("Enter Pet 🆔 to add medical record (or press Enter or '0' to return): ");
                    if (id == 0) break;
                    
                    bool found = false;
                    
                    for (auto& p : pets) {
                        if (p.getPetId() == id) {
                            found = true;
                            std::string date = askForValidDate("Enter the date (YYYY-MM-DD) (or press Enter/0 to return): ", false, true);
                            if (date.empty() || date == "0") {
                                std::cout << "❌ Record creation cancelled.\n";
                                break;
                            }
                            std::string details = askForValidDetails("Enter the details (max 200 characters) (or press Enter/0 to return): ", 200, true);
                            if (details.empty() || details == "0") {
                                std::cout << "❌ Record creation cancelled.\n";
                                break;
                            }
                            p.addMedicalHistory(date, details);
                            saveAllPetsToFile(pets);
                            std::cout << "✅ Medical record added successfully.\n";

                            addMore = promptYesNo("🔁 Would you like to add another record for another pet?");
                            break;
                        }
                    }

                    if (!found) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    }
                }

                break;
            }
        case 2: // view med history
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to view medical history (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    const auto& history = pet->getMedicalHistory();
                    if (!pet->hasRecords(history)) {
                        std::cout << "📭 No medical history found for Pet 🆔 " << id << ".\n";
                        continue;
                    }

                    pet->displayRecordTable(history, "Medical History");

                    if (promptYesNo("📋 Would you like to view full details of any record?")) {
                        while (true) {
                            int recId = askForValidId("Enter the Record 🆔 to view full details (or press Enter/0 to return): ");
                            if (recId == 0) break;

                            if (displayFullRecordIfExists(history, recId, "Medical Record")) {
                                break;
                            } else {
                                std::cout << "❌ Record 🆔 not found. Try again.\n";
                            }
                        }
                    }
                }
                break;
            }
    
        case 3: // update medical record
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to update medical record (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    if (!pet->hasMedicalHistory()) {
                        std::cout << "📭 No medical history found for Pet 🆔 " << id << ".\n";
                        continue;
                    }

                    std::cout << "\n📄 --- Medical History for Pet 🆔 " << id << " ---\n";
                    pet->displayMedicalHistoryTable();

                    while (true) {
                        int recId = askForValidId("Enter the Record 🆔 to update (or press Enter/0 to return): ");
                        if (recId == 0) break;

                        if (!pet->hasMedicalRecord(recId)) {
                            std::cout << "❌ Record 🆔 " << recId << " not found for Pet 🆔 " << id << ". Try again.\n";
                            continue;
                        }

                        const Record& currentRecord = pet->getMedicalHistory().at(recId);

                        std::string newDate = askForUpdatedDate("Enter new date (YYYY-MM-DD) (Enter to keep, 0 to cancel): ", false);
                        if (newDate == "0") {
                            std::cout << "❌ Update cancelled.\n";
                            break;
                        }

                        std::string newDetails = askForUpdatedDetails("Enter new details (Enter to keep, 0 to cancel): ");
                        if (newDetails == "0") {
                            std::cout << "❌ Update cancelled.\n";
                            break;
                        }

                        std::string finalDate = newDate.empty() ? currentRecord.getDate() : newDate;
                        std::string finalDetails = newDetails.empty() ? currentRecord.getDetails() : newDetails;

                        pet->updateMedicalHistory(recId, finalDate, finalDetails);
                        saveAllPetsToFile(pets);
                        std::cout << "✅ Medical record updated successfully.\n";

                        if (promptYesNo("🔁 Would you like to update another record for the same pet?")) {
                            std::cout << "\n📄 --- Updated Medical History for Pet 🆔 " << id << " ---\n";
                            pet->displayRecordTable(pet->getMedicalHistory(), "Medical History");
                        } else {
                            break;
                        }
                    }
                }
                break;
            }
        
        case 4: // delete medical record
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to view medical records (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    if (!pet->hasMedicalHistory()) {
                        std::cout << "📭 No medical history found for Pet 🆔 " << id << ".\n";
                        continue;
                    }

                    std::cout << "\n📄 --- Medical History for Pet 🆔 " << id << " ---\n";
                    pet->displayMedicalHistoryTable();

                    while (true) {
                        int recId = askForValidId("Enter the Record 🆔 to delete (or press Enter/0 to return): ");
                        if (recId == 0) break;

                        if (!pet->hasMedicalRecord(recId)) {
                            std::cout << "❌ Record 🆔 " << recId << " not found for Pet 🆔 " << id << ". Please try again.\n";
                            continue;
                        }

                        if (promptYesNo("⚠️ Are you sure you want to delete this record?")) {
                            pet->removeMedicalHistory(recId);
                            saveAllPetsToFile(pets);
                            std::cout << "✅ Medical record deleted successfully.\n";

                            if (pet->hasMedicalHistory()) {
                                if (promptYesNo("🔁 Would you like to delete another record for the same pet?")) {
                                    std::cout << "\n📄 --- Updated Medical History for Pet 🆔 " << id << " ---\n";
                                    pet->displayMedicalHistoryTable();
                                    continue;
                                }
                            } else {
                                std::cout << "📭 No more medical records found for this pet.\n";
                            }

                            break;
                        } else {
                            std::cout << "❌ Deletion cancelled.\n";
                        }
                    }
                }

                std::cout << "🔙 Returning to Manage Medical Records Menu...\n";
                break;
            }
        } 
    }

    std::cout << "🔙 Returning to Pet Menu...\n";
    return;
}


void manageGeneralRecords() {
    int choice;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        std::cout << "\n<<<<< 🗂️ Manage General Pet Records >>>>>\n";
        std::cout << "1. ➕ Add General Record (By Pet 🆔)\n"; 
        std::cout << "2. 📄 View General Records (By Pet 🆔)\n";  
        std::cout << "3. ✏️  Update General Record (By Pet 🆔 & Record 🆔)\n";  
        std::cout << "4. 🗑️  Delete General Record (By Pet 🆔 & Record 🆔)\n";  
        std::cout << "0. 🔙 Return to Pet Menu\n";
        choice = askForMenuChoice(0, 4);
        if (choice == 0) break;
        
        switch (choice)
        {
            case 1: // add general record by pet id
            {
                bool addMore = true;
                while (addMore) {
                    int id = askForValidId("Enter Pet 🆔 to add general record (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    std::string date = askForValidDate("Enter the date (YYYY-MM-DD) (or press Enter/0 to return): ", false, true);
                    if (date.empty() || date == "0") {
                        std::cout << "❌ Record creation cancelled.\n";
                        break;
                    }

                    std::string details = askForValidDetails("Enter the details (max 200 characters) (or press Enter/0 to return): ", 200, true);
                    if (details.empty() || details == "0") {
                        std::cout << "❌ Record creation cancelled.\n";
                        break;
                    }

                    pet->addPetRecord(date, details);
                    saveAllPetsToFile(pets);
                    std::cout << "✅ General record added successfully.\n";

                    addMore = promptYesNo("🔁 Would you like to add another record for another pet?");
                }
                break;
            }

            case 2: // view general records
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to view general records (or press Enter/0 to return): ");
                    if (id == 0) break;
                    
                    bool found = false;
                    for (auto& p : pets) {
                        if (p.getPetId() == id) {
                            found = true;
                            if (p.hasRecords(p.getPetRecords())) {
                                p.displayRecordTable(p.getPetRecords(), "General Pet Records");

                                if (promptYesNo("📋 Would you like to view full details of any record?")) {
                                    int recId = askForValidId("Enter the Record 🆔 to view full details: ");
                                    displayFullRecord(p.getPetRecords(), recId, "General Record");
                                }
                            } else {
                                std::cout << "📭 No general records found for Pet 🆔 " << id << ".\n";
                            }
                            break;
                        }
                    }
                    if (!found) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    }
                }
                break;
            }

            case 3: // update general record
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to update general record (Press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please, try again.\n";
                        continue;
                    }

                    if (!pet->hasRecords(pet->getPetRecords())) {
                        std::cout << "📭 No General Records found for Pet 🆔 " << id << ".\n";
                        continue;
                    }

                    std::cout << "\n📄 --- General Records for Pet 🆔 " << id << " ---\n";
                    pet->displayRecordTable(pet->getPetRecords(), "General Pet Records");

                    while (true) {
                        int recId = askForValidId("Enter the Record 🆔 to update (or press Enter/0 to return): ");
                        if (recId == 0) break;

                        if (!pet->hasRecord(recId, pet->getPetRecords())) {
                            std::cout << "❌ Record 🆔 " << recId << " not found for Pet 🆔 " << id << ". Please try again.\n";
                            continue;
                        }

                        std::string newDate = askForUpdatedDate("Enter new date (YYYY-MM-DD) (Enter to keep, 0 to cancel): ", false);
                        if (newDate == "0") {
                            std::cout << "❌ Update cancelled.\n";
                            break;
                        }

                        std::string newDetails = askForUpdatedDetails("Enter new details (max 200 characters) (Enter to keep, 0 to cancel): ");
                        if (newDetails == "0") {
                            std::cout << "❌ Update cancelled.\n";
                            break;
                        }

                        if (!newDate.empty()) pet->updatePetRecordDate(recId, newDate);
                        if (!newDetails.empty()) pet->updatePetRecordDetails(recId, newDetails);

                        saveAllPetsToFile(pets);
                        std::cout << "✅ General record updated successfully.\n";

                        if (promptYesNo("🔁 Would you like to update another record for the same pet?")) {
                            std::cout << "\n📄 --- Updated General Records for Pet 🆔 " << id << " ---\n";
                            pet->displayRecordTable(pet->getPetRecords(), "General Records");
                        } else {
                            break;
                        }
                    }
                }
                break;
            }

            case 4: // pet record deletion
            {
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to delete general record (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    if (!pet->hasRecords(pet->getPetRecords())) {
                        std::cout << "📭 No general records found for Pet 🆔 " << id << ".\n";
                        continue;
                    }

                    std::cout << "\n📄 --- General Records for Pet 🆔 " << id << " ---\n";
                    pet->displayRecordTable(pet->getPetRecords(), "General Records");

                    while (true) {
                        int recId = askForValidId("Enter the Record 🆔 to delete (or press Enter/0 to return): ");
                        if (recId == 0) break;

                        if (!pet->hasRecord(recId, pet->getPetRecords())) {
                            std::cout << "❌ Record 🆔 " << recId << " not found for Pet 🆔 " << id << ". Please try again.\n";
                            continue;
                        }

                        if (promptYesNo("⚠️ Are you sure you want to delete this record?")) {
                            pet->removePetRecord(recId);
                            saveAllPetsToFile(pets);
                            std::cout << "✅ General record deleted successfully.\n";

                            if (pet->hasRecords(pet->getPetRecords())) {
                                if (promptYesNo("🔁 Would you like to delete another record for the same pet?")) {
                                    std::cout << "\n📄 --- Updated General Records for Pet 🆔 " << id << " ---\n";
                                    pet->displayRecordTable(pet->getPetRecords(), "General Records");
                                    continue;
                                }
                            } else {
                                std::cout << "📭 No more general records for this pet.\n";
                            }

                            break; // done with this pet
                        } else {
                            std::cout << "❌ Deletion cancelled.\n";
                        }
                    }
                }

                std::cout << "🔙 Returning to Manage General Pet Records Menu...\n";
                break;
            }
        }
    }

    std::cout << "🔙 Returning to Pet Menu...\n";
    return;
}


void manageVaccinations() {
    // manage vaccinations
    while (true) {
        std::cout << "\n<<<<< 💉 Manage Pet Vaccinations >>>>>\n";
        std::cout << "1. ➕ Add Vaccination (By Pet 🆔)\n";  
        std::cout << "2. 📄 View Vaccinations (By Pet 🆔)\n";  
        std::cout << "3. ✏️  Update Vaccination Status (By Pet 🆔 & Vaccination Name)\n";  
        std::cout << "4. 🗑️  Delete Vaccination (By Pet 🆔 & Vaccination 🆔)\n";  
        std::cout << "5. 📋 View Overall Vaccination Status (By Pet 🆔)\n";  
        std::cout << "0. 🔙 Return to Pet Menu\n";
        int choice_m = askForMenuChoice(0, 5);
        if (choice_m == 0) break;

        switch (choice_m)
        {
        case 1:
            while (true) {
                int id = askForValidId("Enter Pet 🆔 to add a vaccination (or press Enter/0 to return): ");
                if (id == 0) break;

                Pet* pet = findPetById(pets, id);
                if (!pet) {
                    std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    continue;
                }

                std::string name = askForValidVaccinationName("Enter the name of the vaccination (or press Enter/0 to return): ");
                if (name.empty() || name == "0") {
                    std::cout << "❌ Vaccination creation cancelled.\n";
                    continue;
                }

                std::string date = askForValidDate("Enter the date (YYYY-MM-DD) (or press Enter/0 to return): ", false, true);
                if (date.empty() || date == "0") {
                    std::cout << "❌ Vaccination creation cancelled.\n";
                    continue;
                }

                std::string status = askForValidVaccinationStatus("Enter the vaccination status (or press Enter/0 to return): ");
                if (status.empty() || status == "0") {
                    std::cout << "❌ Vaccination creation cancelled.\n";
                    continue;
                }

                pet->addVaccination(name, date, status);
                saveAllPetsToFile(pets);
                std::cout << "✅ Vaccination added successfully.\n";

                if (!promptYesNo("🔁 Would you like to add another vaccination for another pet?")) break;
            }

            std::cout << "🔙 Returning to Manage Vaccinations Menu...\n";
            break;

        case 2: // view vaccinations
            while (true) {
                int id = askForValidId("Enter Pet 🆔 to view vaccinations (or press Enter/0 to return): ");
                if (id == 0) break;

                Pet* pet = findPetById(pets, id);
                if (!pet) {
                    std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    continue;
                }

                if (!pet->hasVaccinations()) {
                    std::cout << "📭 No vaccinations found for Pet 🆔 " << id << ".\n";
                    continue;
                }

                pet->displayVaccinationsTable();

                while (promptYesNo("📋 Would you like to view full details of any vaccination?")) {
                    int vaccId = askForValidId("Enter Vaccination 🆔 to view full details (or press Enter/0 to return): ");
                    if (vaccId == 0) break;

                    if (pet->hasVaccination(vaccId)) {
                        pet->displayFullVaccination(vaccId);
                    } else {
                        std::cout << "❌ Vaccination 🆔 " << vaccId << " not found for this pet.\n";
                    }
                }
            }

            std::cout << "🔙 Returning to Manage Vaccinations Menu...\n";
            break;

        case 3:
            while (true) {
                int id = askForValidId("Enter Pet 🆔 to update vaccination (or press Enter/0 to return): ");
                if (id == 0) break;

                Pet* pet = findPetById(pets, id);
                if (!pet) {
                    std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    continue;
                }

                if (!pet->hasVaccinations()) {
                    std::cout << "📭 No vaccinations found for Pet 🆔 " << id << ".\n";
                    continue;
                }

                std::cout << "\n📄 --- Vaccination Records for Pet 🆔 " << id << " ---\n";
                pet->displayVaccinationsTable();

                while (true) {
                    int vaccId = askForValidId("Enter Vaccination 🆔 to update (or press Enter/0 to return): ");
                    if (vaccId == 0) break;

                    if (!pet->hasVaccination(vaccId)) {
                        std::cout << "❌ Vaccination 🆔 " << vaccId << " not found for Pet 🆔 " << id << ". Please try again.\n";
                        continue;
                    }

                    const Vaccination* v = pet->getVaccinationById(vaccId);
                    std::cout << "\n✏️ Updating vaccination: " << v->getName() << " (🆔: " << v->getId() << ")\n";
                    std::cout << "📅 Current date: " << v->getDate() << " | 📌 Status: " << v->getStatus() << "\n";

                    std::string newDate = askForUpdatedDate("Enter new date (YYYY-MM-DD) (Enter to keep, 0 to cancel): ", false);
                    if (newDate == "0") {
                        std::cout << "❌ Update cancelled.\n";
                        break;
                    }

                    std::string newStatus = askForUpdatedVaccinationStatus(v->getStatus());
                    if (newStatus == "0") {
                        std::cout << "❌ Update cancelled.\n";
                        break;
                    }

                    if (!newDate.empty() || !newStatus.empty()) {
                        pet->updateVaccination(vaccId, newDate.empty() ? v->getDate() : newDate,
                                                        newStatus.empty() ? v->getStatus() : newStatus);
                        saveAllPetsToFile(pets);
                        std::cout << "✅ Vaccination updated successfully.\n";
                    } else {
                        std::cout << "⚠️ No changes made.\n";
                    }

                    if (!promptYesNo("🔁 Would you like to update another vaccination for the same pet?")) break;

                    std::cout << "\n📄 --- Updated Vaccinations for Pet 🆔 " << id << " ---\n";
                    pet->displayVaccinationsTable();
                }
            }

            std::cout << "🔙 Returning to Manage Vaccinations Menu...\n";
            break;

        case 4:
            while (true) {
                int id = askForValidId("Enter Pet 🆔 to delete a vaccination (or press Enter/0 to return): ");
                if (id == 0) break;

                Pet* pet = findPetById(pets, id);
                if (!pet) {
                    std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    continue;
                }

                if (!pet->hasVaccinations()) {
                    std::cout << "📭 No vaccinations found for Pet 🆔 " << id << ".\n";
                    continue;
                }

                std::cout << "\n📄 --- Vaccination Records for Pet 🆔 " << id << " ---\n";
                pet->displayVaccinationsTable();

                while (true) {
                    int vaccId = askForValidId("Enter Vaccination 🆔 to delete (or press Enter/0 to return): ");
                    if (vaccId == 0) break;

                    if (!pet->hasVaccination(vaccId)) {
                        std::cout << "❌ Vaccination 🆔 " << vaccId << " not found for Pet 🆔 " << id << ". Please try again.\n";
                        continue;
                    }

                    std::cout << "\n⚠️ You are about to delete the following vaccination:\n";
                    pet->displayFullVaccination(vaccId);

                    if (promptYesNo("⚠️ Are you sure you want to delete this vaccination?")) {
                        pet->removeVaccination(vaccId);
                        saveAllPetsToFile(pets);
                        std::cout << "✅ Vaccination deleted successfully.\n";

                        if (pet->hasVaccinations()) {
                            if (promptYesNo("🔁 Would you like to delete another vaccination for the same pet?")) {
                                std::cout << "\n📄 --- Updated Vaccinations for Pet 🆔 " << id << " ---\n";
                                pet->displayVaccinationsTable();
                            } else {
                                break;
                            }
                        } else {
                            std::cout << "📭 No more vaccinations found for this pet.\n";
                            break;
                        }
                    } else {
                        std::cout << "❌ Deletion cancelled.\n";
                    }
                }
            }

            std::cout << "🔙 Returning to Manage Vaccinations Menu...\n";
            break;

        case 5:
            while (true) {
                int id = askForValidId("Enter Pet 🆔 to view overall vaccination status (or press Enter/0 to return): ");
                if (id == 0) break;

                Pet* pet = findPetById(pets, id);
                if (!pet) {
                    std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                    continue;
                }

                std::string overallStatus = capitalizeWords(pet->calculateVaccinationStatus());
                std::cout << "\n📋 --- Vaccination Summary for Pet: " << pet->getName()
                          << " (🆔: " << pet->getPetId() << ") ---\n";
                std::cout << "💡 Overall Vaccination Status: " << overallStatus << "\n";
                std::cout << "---------------------------------------------------------\n";

                if (pet->hasVaccinations()) {
                    pet->displayVaccinationsTable();
                } else {
                    std::cout << "📭 No vaccination records found for this pet.\n";
                }

                if (!promptYesNo("🔁 Would you like to check another pet?")) {
                    break;
                }
            }

            std::cout << "🔙 Returning to Manage Vaccinations Menu...\n";
            break;
        }
    }

    std::cout << "🔙 Qaytyapmiz main menuga hozir...\n";
    return;
}


void manageAppointments() 
// Manage Appointments 
{
    // code coming
    while (true) {
        std::cout << "\n<<<<< 📅 Manage Pet Appointments >>>>>\n";
        std::cout << "1. ➕ Add Appointment (By Pet 🆔)\n";
        std::cout << "2. 📄 View Appointment History (By Pet 🆔)\n";
        std::cout << "3. ✏️  Update Appointment Status (By Pet 🆔 & Appointment 🆔)\n";
        // std::cout << "4. Delete Appointment (By Pet ID & Appointment ID)\n";
        std::cout << "0. 🔙 Return to Pet Menu\n";
        int choice_m = askForMenuChoice(0, 3);
        if (choice_m == 0) break;

        switch (choice_m)
        {
            case 1:
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to add appointment (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    std::cout << "📅 Scheduling appointment for: " << pet->getName() << " (🆔 " << id << ")\n";

                    int ownerId = pet->getOnwerId();

                    std::string date = askForValidAppointmentDate("Enter appointment date (YYYY-MM-DD) (or press Enter/0 to return): ", true, true);
                    if (date.empty()) {
                        std::cout << "❌ Appointment creation cancelled.\n";
                        continue;
                    }

                    std::string time = askForValidAppointmentTime("Enter appointment time (HH:MM) (or press Enter/0 to return): ", true);
                    if (time.empty()) {
                        std::cout << "❌ Appointment creation cancelled.\n";
                        continue;
                    }

                    std::string purpose = askForValidAppointmentPurpose("Enter purpose of appointment (max 100 chars) (or press Enter/0 to return): ", true);
                    if (purpose.empty()) {
                        std::cout << "❌ Appointment creation cancelled.\n";
                        continue;
                    }

                    int appointmentId = nextAppointmentId++;
                    std::string status = "scheduled";

                    appointments.emplace_back(appointmentId, ownerId, id, date, time, purpose, status);
                    saveAllAppointmentsToFile(appointments);

                    std::cout << "✅ Appointment (🆔 " << appointmentId << ") added successfully.\n";

                    if (!promptYesNo("🔁 Would you like to add another appointment?")) break;
                }

                std::cout << "🔙 Returning to Manage Appointments Menu...\n";
                break;

            case 2:
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to view appointments (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    std::vector<Appointment> petAppointments;
                    for (const auto& appt : appointments) {
                        if (appt.getPetId() == id) {
                            petAppointments.push_back(appt);
                        }
                    }

                    if (petAppointments.empty()) {
                        std::cout << "📭 No appointments found for 🐾 " << pet->getName() << " (🆔 " << id << ").\n";
                    } else {
                        std::cout << "\n📋 Appointments for Pet: " << pet->getName() << " (🆔 " << id << ")\n";
                        Appointment::displayAppointmentsTable(petAppointments);

                        if (promptYesNo("🔎 Would you like to view full details of any appointment?")) {
                            while (true) {
                                int apptId = askForValidId("Enter Appointment 🆔 (or press Enter/0 to return): ");
                                if (apptId == 0) break;

                                auto it = std::find_if(petAppointments.begin(), petAppointments.end(),
                                    [apptId](const Appointment& a) { return a.getAppointmentId() == apptId; });

                                if (it != petAppointments.end()) {
                                    it->displayAppointmentDetails();
                                } else {
                                    std::cout << "❌ Appointment 🆔 " << apptId << " not found for this pet.\n";
                                }
                            }
                        }
                    }
                }

                std::cout << "🔙 Returning to Manage Appointments Menu...\n";
                break;

            case 3:
                while (true) {
                    int id = askForValidId("Enter Pet 🆔 to update appointment status (or press Enter/0 to return): ");
                    if (id == 0) break;

                    Pet* pet = findPetById(pets, id);
                    if (!pet) {
                        std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
                        continue;
                    }

                    std::vector<Appointment> petAppts;
                    for (const auto& appt : appointments) {
                        if (appt.getPetId() == id) {
                            petAppts.push_back(appt);
                        }
                    }

                    if (petAppts.empty()) {
                        std::cout << "📭 No appointments found for 🐾 " << pet->getName() << " (🆔 " << id << ").\n";
                        continue;
                    }

                    std::cout << "\n📅 Appointments for Pet: " << pet->getName() << " (🆔 " << id << ")\n";
                    Appointment::displayAppointmentsTable(petAppts);

                    while (true) {
                        int apptId = askForValidId("Enter Appointment 🆔 to update status (or press Enter/0 to return): ");
                        if (apptId == 0) break;

                        auto it = std::find_if(petAppts.begin(), petAppts.end(),
                            [apptId](const Appointment& a) { return a.getAppointmentId() == apptId; });

                        if (it != petAppts.end()) {
                            std::cout << "🔁 You are updating the following appointment:\n";
                            it->displayFullAppointment();

                            std::string newStatus = askForValidAppointmentStatus("Enter a new status (or Enter/0 to return)");
                            if (newStatus.empty()) {
                                std::cout << "❌ Update cancelled.\n";
                                break;
                            }

                            auto mainIt = std::find_if(appointments.begin(), appointments.end(),
                                [apptId](const Appointment& a) { return a.getAppointmentId() == apptId; });

                            if (mainIt != appointments.end()) {
                                mainIt->updateStatus(newStatus);
                                saveAllAppointmentsToFile(appointments);

                                std::cout << "✅ Appointment (🆔 " << apptId << ") status updated successfully.\n";

                                if (!promptYesNo("🔁 Would you like to update another appointment status for the same pet?")) {
                                    break;
                                }

                                petAppts.clear();
                                for (const auto& appt : appointments) {
                                    if (appt.getPetId() == id) {
                                        petAppts.push_back(appt);
                                    }
                                }

                                std::cout << "\n📅 Updated Appointments for Pet: " << pet->getName() << "\n";
                                Appointment::displayAppointmentsTable(petAppts);
                            }

                        } else {
                            std::cout << "❌ Appointment 🆔 " << apptId << " not found for this pet. Please try again.\n";
                        }
                    }
                }

                std::cout << "🔙 Returning to Manage Appointments Menu...\n";
                break;
        }
    }

    std::cout << "🔙 Returning to Pet Menu...\n";
    return;
}


void deletePet() 
// case 8: // delete a pet (all pet data)
{
    while (true) {
        int id = askForValidId("Enter Pet 🆔 to delete (or press Enter/0 to return): ");
        if (id == 0) break;

        Pet* pet = findPetById(pets, id);
        if (pet) {
            std::cout << "\n⚠️ You are about to delete the following pet:\n";

            std::cout << "\n📋 Pet Info\n";
            std::cout << "----------------------------------------\n";
            std::cout << "🐾 Pet ID   : " << pet->getPetId() << "\n";
            std::cout << "📛 Name     : " << pet->getName() << "\n";
            std::cout << "🧬 Breed    : " << pet->getBreed() << "\n";
            std::cout << "🎂 Age      : " << pet->getAge() << "\n";
            std::cout << "----------------------------------------\n";

            std::cout << "⚠️ This will also delete all appointments and unlink from owners.\n";
            if (promptYesNo("⚠️ Are you sure you want to delete this pet from the entire system?")) {
                // remove pets from pets vector
                pets.erase(std::remove_if(pets.begin(), pets.end(), 
                            [id](const Pet& p) {return p.getPetId() == id;}),
                            pets.end());

                // remove appointments
                appointments.erase(std::remove_if(appointments.begin(), appointments.end(),
                                    [id](const Appointment& a) {return a.getPetId() == id;}),
                                    appointments.end());

                // remove pet id from owners
                for (auto& owner : owners) {
                    auto& petIds = owner.getPetIdsRef();
                    petIds.erase(std::remove(petIds.begin(), petIds.end(), id), petIds.end());
                }

                // save
                saveAllPetsToFile(pets);
                saveAllOwnersToFile(owners);
                saveAllAppointmentsToFile(appointments);

                std::cout << "✅ Pet and all associated records deleted successfully.\n";

                if (!promptYesNo("🔁 Would you like to delete another pet?")) {
                    break;
                }

            } else {
                std::cout << "❌ Deletion cancelled.\n";
            }

        } else {
            std::cout << "❌ Pet 🆔 " << id << " not found. Please try again.\n";
        }
    }

    std::cout << "🔙 Returning to Pet Menu...\n";
    return;
}

void linkPetToOwner() {
    displayUnassignedPets(pets);

    bool hasUnassigned = std::any_of(pets.begin(), pets.end(), [](const Pet& p) { return p.getOnwerId() == -1; });
    if (!hasUnassigned) {
        std::cout << "📭 No unassigned pets found. 🔙 Returning to Pet Menu...\n";
        return;
    }

    while (true) {
        int petId = askForValidId("Enter Pet 🆔 to assign (or press Enter/0 to return): ");
        if (petId == 0) {
            break;  // Exit to menu
        }

        Pet* pet = findPetById(pets, petId);
        if (!pet || pet->getOnwerId() != -1) {
            std::cout << "❌ Invalid or already assigned Pet 🆔. Please try again.\n";
            continue;
        }

        std::cout << "\n📋 --- Available Owners ---\n";
        for (const auto& owner : owners) {
            std::cout << std::left 
                      << "👤 Owner 🆔: " << std::setw(4) << owner.getOwnerId() 
                      << " | Name: " << owner.getName() << "\n";
        }

        int ownerId;
        bool validOwner = false;
        while (!validOwner) {
            ownerId = askForValidId("Enter Owner 🆔 to link with this pet (or press Enter/0 to return): ");
            if (ownerId == 0) {
                std::cout << "❌ Linking cancelled.\n";
                break;  // Exit owner selection, back to pet selection
            }

            auto ownerIt = std::find_if(owners.begin(), owners.end(), [ownerId](const Owner& o) { return o.getOwnerId() == ownerId; });
            if (ownerIt == owners.end()) {
                std::cout << "❌ Owner 🆔 not found. Please try again.\n";
                continue;  // Re-prompt for owner ID
            }

            validOwner = true;  // Found a valid owner, proceed

            std::cout << "\n🔗 You're about to link:\n";
            std::cout << "🐾 Pet: " << pet->getName() << " (🆔 " << petId << ")\n";
            std::cout << "👤 Owner: " << ownerIt->getName() << " (🆔 " << ownerId << ")\n";

            if (promptYesNo("⚠️ Are you sure you want to link this pet to the selected owner?")) {
                pet->setOwnerId(ownerId);
                ownerIt->addPetId(petId);
                saveAllPetsToFile(pets);
                saveAllOwnersToFile(owners);
                std::cout << "✅ Pet successfully linked to owner.\n";

                if (!promptYesNo("🔁 Would you like to link another unassigned pet?")) {
                    std::cout << "🔙 Returning to Pet Menu...\n";
                    return;  // Exit function entirely
                }
            } else {
                std::cout << "❌ Linking cancelled.\n";
            }
        }
        if (ownerId == 0) {
            continue;  // If owner selection was cancelled, go back to pet selection
        }
    }
    std::cout << "🔙 Returning to Pet Menu...\n";
}

