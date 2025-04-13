#include "menu.h"
#include "User.h"
#include <iostream>
#include <vector>
#include <map>
#include "Pet.h"
#include "validations.h"
#include "globals.h"
#include "pet_menu_helpers.h"
#include "owner_menu_helpers.h"
#include "appointment_menu_helpers.h"
#include "user_menu_helpers.h"

void mainMenu(User& user) {
    while (true) {
        std::cout << "\n<<<<< 🐾 Veterinary Management System 🐾 >>>>>\n";
        std::cout << "\n<<<<< 📋 Main Menu >>>>>\n";
        std::cout << "1. 🐶 Manage Pets\n";
        std::cout << "2. 👤 Manage Owners\n";
        std::cout << "3. 📅 Manage Appointments\n";
        if (user.canManageUsers()) {
            std::cout << "4. 🛠️  Manage Users\n";
        }
        std::cout << "0. 🔒 Logout\n";

        int maxOption = user.canManageUsers() ? 4 : 3;
        int choice = askForMainMenuChoice(0, maxOption, "Enter your choice (Enter/0 to Logout): ");

        switch (choice) {
            case 1:
                petMenu(user);
                break;
            case 2:
                ownerMenu(user);
                break;
            case 3:
                appointmentMenu(user);
                break;
            case 4:
                if (user.canManageUsers()) {
                    userMenu(user);
                }
                break;
            case 0:
                std::cout << "👋 Logging out...\n";
                return;
            default:
                // This shouldn’t trigger since askForMainMenuChoice validates, but kept for safety
                std::cout << "❌ Unexpected error. Please try again.\n";
                break;
        }
    }
}


void petMenu(User& user) {
    int choice;
    do {
        std::cout << "\n<<<<< 🐾 Pet Menu >>>>>\n";
        std::cout << "1. ➕ Add a New Pet\n";
        std::cout << "2. 📋 View All Pets\n";
        std::cout << "3. 🔍 View Pet Details (By ID)\n";

        std::cout << (user.canManageMedicalRecords() ? "4. 🏥 Manage Medical Records\n" : "4. 🏥 Manage Medical Records (🚫 Restricted)\n");
        std::cout << (user.canManageGeneralPetRecords() || user.getRole() == "Veterinarian" ? "5. 🛠️  Update Pet Information\n" : "5. 🛠️  Update Pet Information (🚫 Restricted)\n");
        std::cout << (user.canManageGeneralPetRecords() ? "6. 📁 Manage General Pet Records\n" : "6. 📁 Manage General Pet Records (🚫 Restricted)\n");

        std::cout << (user.canManageVaccinations() ? "7. 💉 Manage Pet Vaccinations\n" : "7. 💉 Manage Pet Vaccinations (🚫 Restricted)\n");
        std::cout << (user.canManageAppointments() ? "8. 📅 Manage Pet Appointments\n" : "8. 📅 Manage Pet Appointments (🚫 Restricted)\n");
        std::cout << (user.canDeletePet() ? "9. 🗑️  Delete a Pet\n" : "9. 🗑️  Delete a Pet (🚫 Restricted)\n");
        std::cout << (user.canDeletePet() || user.getRole() == "Staff" ? "10. 🔗 Link Unassigned Pet to Owner\n" : "10. 🔗 Link Unassigned Pet to Owner (🚫 Restricted)\n");

        std::cout << "0. 🔙 Return to Main Menu\n";

        choice = askForMenuChoice(0, 10, "Enter your choice: ");

        switch (choice) {
            case 1: addNewPet(); break;
            case 2: viewAllPets(); break;
            case 3: viewPetDetailsById(); break;
            case 4:
                if (user.canManageMedicalRecords()) manageMedicalRecords();
                else std::cout << "❌ Access denied.\n";
                break;
            
            case 5:
                if (user.canManageGeneralPetRecords() || user.getRole() == "Veterinarian") {
                    updatePet();
                } else {
                    std::cout << "❌ Access denied.\n";
                }
                break;
            case 6:
                if (user.canManageGeneralPetRecords()) manageGeneralRecords();
                else std::cout << "❌ Access denied.\n";
                break;
            case 7:
                if (user.canManageVaccinations()) manageVaccinations();
                else std::cout << "❌ Access denied.\n";
                break;
            case 8:
                if (user.canManageAppointments()) manageAppointments();
                else std::cout << "❌ Access denied.\n";
                break;
            case 9:
                if (user.canDeletePet()) deletePet();
                else std::cout << "❌ Access denied.\n";
                break;
            case 10:
                if (user.canDeletePet() || user.getRole() == "Staff") {
                    linkPetToOwner();
                } else {
                    std::cout << "❌ Access denied.\n";
                }
                break;
            case 0:
                std::cout << "🔙 Returning to Main Menu...\n";
                break;
            default:
                std::cout << "❌ Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 0);
}


void ownerMenu(User& user) {
    int choice;
    do {
        std::cout << "\n<<<<< 👤 Owner Menu >>>>>\n";

        std::cout << "1. ➕ Add New Owner" << (user.canManageOwnerRecords() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "2. 📋 View All Owners\n";
        std::cout << "3. 🔍 View Owner Details (By ID)\n";
        std::cout << "4. ✏️  Update Owner Info" << (user.canManageOwnerRecords() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "5. 🗑️  Delete Owner" << (user.canDeleteOwner() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "6. 🐾 View Pets Linked to an Owner\n";
        std::cout << "7. 📁 Manage Owner Records" << (user.canManageOwnerRecords() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "8. 📅 Manage Appointments" << (user.canManageAppointments() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "0. 🔙 Return to Main Menu\n";

        choice = askForMenuChoice(0, 8, "Enter your choice: ");

        switch (choice) {
            case 1:
                if (user.canManageOwnerRecords()) addNewOwner();
                else std::cout << "❌ Access denied.\n";
                break;
            case 2: viewAllOwners(); break;
            case 3: viewOwnerDetailsById(); break;
            case 4:
                if (user.canManageOwnerRecords()) updateOwner();
                else std::cout << "❌ Access denied.\n";
                break;
            case 5:
                if (user.canDeleteOwner()) deleteOwner();
                else std::cout << "❌ Access denied.\n";
                break;
            case 6: viewPetsLinkedToOwner(); break;
            case 7:
                if (user.canManageOwnerRecords()) manageOwnerRecords();
                else std::cout << "❌ Access denied.\n";
                break;
            case 8:
                if (user.canManageAppointments()) manageOwnerAppointments();
                else std::cout << "❌ Access denied.\n";
                break;
            case 0: std::cout << "🔙 Returning to Main Menu...\n"; break;
            default: std::cout << "❌ Invalid choice.\n"; break;
        }
    } while (choice != 0);
}


void appointmentMenu(User& user) {
    int choice;
    do {
        std::cout << "\n<<<<< 📅 Appointment Menu >>>>>\n";
        std::cout << "1. 📋 View All Appointments\n";
        std::cout << "2. 🔍 Search Appointment by Appointment ID\n";
        std::cout << "3. 🔎 Search Appointments by Owner ID\n";
        std::cout << "4. 🐾 Search Appointments by Pet ID\n";
        std::cout << "5. ➕ Add Appointment" << (user.canManageAppointments() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "6. ✏️  Update Appointment Status" << (user.canManageAppointments() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "7. 🗑️  Delete Appointment" << (user.canDeletePet() ? "" : " (🚫 Restricted)") << "\n";
        std::cout << "0. 🔙 Return to Main Menu\n";

        choice = askForMenuChoice(0, 7, "Enter your choice: ");

        switch (choice) {
            case 1: viewAllAppointments(appointments); break;
            case 2: searchAppointmentById(appointments); break;
            case 3: searchAppointmentsByOwnerId(appointments); break;
            case 4: searchAppointmentsByPetId(appointments); break;
            case 5:
                if (user.canManageAppointments()) addAppointment(appointments, owners, pets, nextAppointmentId);
                else std::cout << "❌ Access denied.\n";
                break;
            case 6:
                if (user.canManageAppointments()) updateAppointment(appointments);
                else std::cout << "❌ Access denied.\n";
                break;
            case 7:
                if (user.canDeletePet()) deleteAppointment(appointments);
                else std::cout << "❌ Access denied.\n";
                break;
            case 0: std::cout << "🔙 Returning to Main Menu...\n"; break;
        }
    } while (choice != 0);
}


void userMenu(User& currentUser) {
    if (!currentUser.canManageUsers()) {
        std::cout << "❌ Access denied. Only admins can manage users.\n";
        return;
    }

    int choice;
    do {
        std::cout << "\n<<<<< 🛠️ User Management Menu >>>>>\n";
        std::cout << "1. 📋 View All Users\n";
        std::cout << "2. ➕ Add New User\n";
        std::cout << "3. ✏️  Update Existing User\n";
        std::cout << "4. 🗑️  Delete User\n";
        std::cout << "0. 🔙 Return to Main Menu\n";
        choice = askForMenuChoice(0, 4, "Enter your choice: ");

        switch (choice) {
            case 1: viewAllUsers(users); break;
            case 2: addNewUser(users, nextUserId); break;
            case 3: updateUser(users); break;
            case 4: deleteUser(users); break;
        }
    } while (choice != 0);
}
