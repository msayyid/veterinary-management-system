#include "Owner.h"
#include "Pet.h"
#include "User.h"
#include "Appointment.h"
#include <vector>
#include <iostream>
#include <memory>
#include "menu.h"
#include "globals.h"
#include <unordered_map>
#include <algorithm>
#include "validations.h"
#include "hashing.h"

int main() {

    std::cout << "\033[1m;31mHElLo\033[0m\n";
    // Load global data once at startup
    users = User::loadFromFile("users.csv");
    pets = Pet::loadFromFile("pets.csv");
    owners = Owner::loadFromFile("owners.csv");
    appointments = Appointment::loadFromFile("appointments.csv");

    // std::cout << "[main] users.size(): " << users.size() << "\n";

    // (Optional) Debug: List loaded users
    // std::cout << "==== Loaded Users ====\n";
    // for (const auto& u : users) {
    //     std::cout << u->getUsername() << " => " << u->getPassword() << "\n";
    // }

    // Sync next ID counters
    for (const auto& p : pets)
        if (p.getPetId() >= nextPetId) nextPetId = p.getPetId() + 1;
    for (const auto& o : owners)
        if (o.getOwnerId() >= nextOwnerId) nextOwnerId = o.getOwnerId() + 1;
    for (const auto& a : appointments)
        if (a.getAppointmentId() >= nextAppointmentId) nextAppointmentId = a.getAppointmentId() + 1;
    for (const auto& u : users)
        if (u->getId() >= nextUserId) nextUserId = u->getId() + 1;

    // Login Section
    int totalLoginAttempts = 0;
    const int MAX_TOTAL_ATTEMPTS = 3;

    while (true) {
        std::cout << "=============================================\n";
        std::cout << "     🐾  Welcome to VetSys: Login Portal  🐾\n";
        std::cout << "=============================================\n";

        User* loggedInUser = nullptr;

        while (!loggedInUser) {
            std::string enteredUsername, rawPassword;

            std::cout << "\nEnter Username: ";
            std::getline(std::cin, enteredUsername);

            rawPassword = getHiddenPassword("Enter Password: ");
            std::string hashedInput = sha256(rawPassword);

            std::string trimmedUsername = trim(enteredUsername);
            std::string lowerUsername = toLower(trimmedUsername);

            for (const auto& u : users) {
                if (toLower(trim(u->getUsername())) == lowerUsername) {
                    // std::cout << "AUTH Comparing with stored: " << u->getUsername()
                    //           << " => " << u->getPassword() << "\n";
                    // std::cout << "Comparing hashes:\nEntered: " << hashedInput
                    //           << "\nStored : " << u->getPassword() << "\n";

                    if (hashedInput == u->getPassword()) {
                        loggedInUser = u.get();
                        break;
                    }
                }
            }

            if (!loggedInUser) {
                totalLoginAttempts++;
                if (totalLoginAttempts >= MAX_TOTAL_ATTEMPTS) {
                    std::cout << "\n❌ Too many failed login attempts. System is shutting down for security reasons.\n";
                    return 1;
                }

                int remaining = MAX_TOTAL_ATTEMPTS - totalLoginAttempts;
                std::cout << "❌ Invalid username or password. "
                          << "You have " << remaining << " attempt(s) left.\n";
            }
        }

        std::cout << "\n✅ Login successful!\n";
        std::cout << "=============================================\n";
        std::cout << "  Welcome to the Veterinary Management System\n";
        std::cout << "  Logged in as: " << loggedInUser->getUsername()
                  << "  (Role: " << loggedInUser->getRole() << ")\n";
        std::cout << "  Have a productive session!\n";
        std::cout << "=============================================\n";

        mainMenu(*loggedInUser); // Polymorphic menu based on role
    }

    return 0;
}