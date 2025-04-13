#include "hashing.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "User.h"
#include "validations.h"
#include "globals.h"
#include "Pet.h"

// Base Class
User::User(int userId, const std::string& username, const std::string& password)
    : userId(userId), username(username), password(password) {}

int User::getId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }

void User::setUsername(const std::string& newUsername) {
    username = newUsername;
}

void User::setPassword(const std::string& hashedPassword) {
    password = hashedPassword;
}

void User::displayUserInfo() const {
    std::cout << "ID: " << userId << "\nUsername: " << username << "\nRole: " << getRole() << "\n";
}

// Admin
Admin::Admin(int userId, const std::string& username, const std::string& password)
    : User(userId, username, password) {}

std::string Admin::getRole() const { return "Admin"; }

void Admin::showMenu() const {
    std::cout << "=== Admin Menu ===\n1. Manage Users\n2. View All Data\n...\n";
}

void Admin::saveToFile(std::ostream& out) const {
    out << userId << "," << username << "," << password << "," << getRole() << "\n";
}

bool Admin::canManageUsers() const { return true; }
bool Admin::canManageMedicalRecords() const { return true; }
bool Admin::canManageGeneralPetRecords() const { return true; }
bool Admin::canManageVaccinations() const { return true; }
bool Admin::canManageAppointments() const { return true; }
bool Admin::canDeletePet() const { return true; }
bool Admin::canDeleteOwner() const { return true; }
bool Admin::canManageOwnerRecords() const { return true; }


// Veterinarian
Veterinarian::Veterinarian(int userId, const std::string& username, const std::string& password)
    : User(userId, username, password) {}

std::string Veterinarian::getRole() const { return "Veterinarian"; }

void Veterinarian::showMenu() const {
    std::cout << "=== Veterinarian Menu ===\n1. View Pets\n2. Manage Medical Records\n...\n";
}

void Veterinarian::saveToFile(std::ostream& out) const {
    out << userId << "," << username << "," << password << "," << getRole() << "\n";
}
bool Veterinarian::canManageUsers() const { return false; }
bool Veterinarian::canManageMedicalRecords() const { return true; }
bool Veterinarian::canManageGeneralPetRecords() const { return false; }
bool Veterinarian::canManageVaccinations() const { return true; }
bool Veterinarian::canManageAppointments() const { return false; }
bool Veterinarian::canDeletePet() const { return false; }
bool Veterinarian::canDeleteOwner() const { return false; }
bool Veterinarian::canManageOwnerRecords() const { return false; }


// Staff
Staff::Staff(int userId, const std::string& username, const std::string& password)
    : User(userId, username, password) {}

std::string Staff::getRole() const { return "Staff"; }

void Staff::showMenu() const {
    std::cout << "=== Staff Menu ===\n1. Schedule Appointments\n2. View Owner Data\n...\n";
}

void Staff::saveToFile(std::ostream& out) const {
    out << userId << "," << username << "," << password << "," << getRole() << "\n";
}

bool Staff::canManageUsers() const { return false; }
bool Staff::canManageMedicalRecords() const { return false; }
bool Staff::canManageGeneralPetRecords() const { return true; }
bool Staff::canManageVaccinations() const { return false; }
bool Staff::canManageAppointments() const { return true; }
bool Staff::canDeletePet() const { return false; }
bool Staff::canDeleteOwner() const { return false; }
bool Staff::canManageOwnerRecords() const { return true; }


// Load Users
std::vector<std::unique_ptr<User>> User::loadFromFile(const std::string& filename) {
    std::vector<std::unique_ptr<User>> users;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error! Cannot open file: " << filename << "\n";
        return users;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, username, password, role;
        std::getline(ss, idStr, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, role);

        int id = std::stoi(idStr);
        std::string lowerRole = toLower(trim(role));

        try {
            if (lowerRole == "admin") {
                users.push_back(std::make_unique<Admin>(id, username, password));
            } else if (lowerRole == "staff") {
                users.push_back(std::make_unique<Staff>(id, username, password));
            } else if (lowerRole == "veterinarian") {
                users.push_back(std::make_unique<Veterinarian>(id, username, password));
            } else {
                throw std::invalid_argument("Unknown role: " + role);
            }

            // std::cout << "Loaded user: " << username << " | hash: " << password << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error loading user ID " << idStr << ": " << e.what() << "\n";
        }
    }

    // std::cout << "[loadFromFile] FINAL count: " << users.size() << "\n";
    return users;
}



// Authentication 
User* User::authenticateUser(const std::vector<std::unique_ptr<User>>& users,
                             const std::string& enteredUsername,
                             const std::string& enteredPassword) {
    std::string loweredEnteredUsername = enteredUsername;
    // std::cout << "Raw password entered: [" << enteredPassword << "]\n";

    std::transform(loweredEnteredUsername.begin(), loweredEnteredUsername.end(), loweredEnteredUsername.begin(), ::tolower);
    // std::cout << "Raw password entered: [" << enteredPassword << "]\n";

    std::string enteredHash = sha256(enteredPassword);

    for (const auto& u : users) {
        std::string storedUsername = u->getUsername();
        std::transform(storedUsername.begin(), storedUsername.end(), storedUsername.begin(), ::tolower);
        // std::cout << "AUTH Comparing with stored: " << u->getUsername() << " => " << u->getPassword() << "\n";

        if (storedUsername == loweredEnteredUsername) {

            // Temporary debug (optional)
            // std::cout << "Comparing hashes:\n";
            // std::cout << "Entered: " << enteredHash << "\n";
            // std::cout << "Stored : " << u->getPassword() << "\n";

            if (u->getPassword() == enteredHash) {
                return u.get();
            }
        }
    }
    return nullptr;
}




std::unique_ptr<User> createUser(int id, const std::string& username, const std::string& rawPassword, const std::string& role) {
    std::string hashedPassword = sha256(rawPassword);  // ✅ Hash once here
    std::string lowerRole = role;
    std::transform(lowerRole.begin(), lowerRole.end(), lowerRole.begin(), ::tolower);

    if (lowerRole == "admin") {
        return std::make_unique<Admin>(id, username, hashedPassword);
    } else if (lowerRole == "staff") {
        return std::make_unique<Staff>(id, username, hashedPassword);
    } else if (lowerRole == "veterinarian") {
        return std::make_unique<Veterinarian>(id, username, hashedPassword);
    } else {
        throw std::invalid_argument("Unknown role: " + role);
    }
}


User* findUserById(const std::vector<std::unique_ptr<User>>& users, int id) {
    for (const auto& u : users) {
        if (u->getId() == id) return u.get();
    }
    return nullptr;
}