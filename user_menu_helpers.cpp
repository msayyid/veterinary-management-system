#include "user_menu_helpers.h"
#include "validations.h"
#include "globals.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "hashing.h"

extern std::unique_ptr<User> createUser(int id, const std::string& username, const std::string& rawPassword, const std::string& role);

void viewAllUsers(const std::vector<std::unique_ptr<User>>& users) {
    if (users.empty()) {
        std::cout << "No users found.\n";
        return;
    }

    std::cout << "\nUser ID   Username            Role\n";
    std::cout << "-------------------------------------------\n";

    for (const auto& user : users) {
        std::cout << std::left
                  << std::setw(10) << user->getId()
                  << std::setw(20) << user->getUsername()
                  << std::setw(15) << user->getRole()
                  << "\n";
    }

    std::cout << "-------------------------------------------\n";

    while (promptYesNo("Would you like to view full info of a user?")) {
        int id = askForValidId("Enter User ID (or press Enter/0 to return): ");
        if (id == 0) break;

        User* found = findUserById(users, id);
        if (found) {
            std::cout << "\n--- User Details ---\n";
            found->displayUserInfo();
            std::cout << "---------------------\n";
        } else {
            std::cout << "User ID not found.\n";
        }
    }

    std::cout << "Returning to User Management Menu...\n";
}

void addNewUser(std::vector<std::unique_ptr<User>>& users, int& nextUserId) {
    while (true) {
        std::string username = askForValidUsername("Enter username (or press Enter/0 to return): ");
        if (username.empty() || username == "0") {
            std::cout << "❌ User creation cancelled.\n";
            break;
        }

        bool userNameExists = false;
        for (const auto& user : users) {
            if (user->getUsername() == username) {
                userNameExists = true;
                break;
            }
        }

        if (userNameExists) {
            std::cout << "❌ A user with that username already exists. Please choose another.\n";
            continue;
        }

        std::string password;
        while (true) {
            password = askForValidPassword("Enter password (min 8 chars) (or press Enter/0 to return): ");
            if (password.empty() || password == "0") {
                std::cout << "User creation cancelled.\n";
                return;
            }
            std::string confirm = getHiddenPassword("Confirm password: ");
            if (password != confirm) {
                std::cout << "❌ Passwords do not match. Try again.\n";
                continue;
            }
            break;
        }

        std::string role;
        while (true) {
            std::cout << "Enter role (admin/staff/veterinarian) (or press Enter/0 to return): ";
            std::getline(std::cin, role);
            role = trim(role);
            std::transform(role.begin(), role.end(), role.begin(), ::tolower);
            if (role.empty() || role == "0") {
                std::cout << "User creation cancelled.\n";
                return;
            }
            if (role == "admin" || role == "staff" || role == "veterinarian") break;
            std::cout << "Invalid role. Try again.\n";
        }
        // Hash the password before creating the user
        // std::string hashedPassword = sha256(password);
        // users.push_back(createUser(nextUserId, username, password, role));
        // When creating a user:
        std::string hashedPassword = sha256(password);
        if (role == "admin") {
            users.emplace_back(std::make_unique<Admin>(nextUserId, username, hashedPassword));
        } else if (role == "staff") {
            users.emplace_back(std::make_unique<Staff>(nextUserId, username, hashedPassword));
        } else if (role == "veterinarian") {
            users.emplace_back(std::make_unique<Veterinarian>(nextUserId, username, hashedPassword));
        }

        // users.emplace_back(std::make_unique<Admin>(nextUserId, username, hashedPassword));

        nextUserId++;
        saveAllUsersToFile(users);

        std::cout << "User added successfully.\n";
        if (!promptYesNo("Would you like to add another user?")) break;
    }

    std::cout << "Returning to User Management Menu...\n";
}


void updateUser(std::vector<std::unique_ptr<User>>& users) {
    while (true) {
        int id = askForValidId("Enter User ID to update (or press Enter/0 to return): ");
        if (id == 0) {
            std::cout << "Returning to User Management Menu...\n";
            break;
        }

        User* user = findUserById(users, id);
        if (!user) {
            std::cout << "User ID not found.\n";
            continue;
        }

        std::cout << "\n--- Current User Info ---\n";
        user->displayUserInfo();

        std::string newUsernameTemp = askForUpdatedUsername(user->getUsername());
        if (newUsernameTemp == "0") {
            std::cout << "Update cancelled.\n";
            continue; // Back to User ID
        }
        if (!newUsernameTemp.empty() && newUsernameTemp != user->getUsername()) {
            bool usernameExists = false;
            for (const auto& u : users) {
                if (u->getUsername() == newUsernameTemp && u->getId() != user->getId()) {
                    usernameExists = true;
                    break;
                }
            }
            if (usernameExists) {
                std::cout << "❌ That username is already taken. Try a different one.\n";
                continue;
            }
        }

        std::string newPasswordTemp = askForValidPassword("Enter new password (Enter to keep, 0 to cancel): ");
        if (newPasswordTemp == "0") {
            std::cout << "Update cancelled.\n";
            continue; // Back to User ID
        }
        if (!newPasswordTemp.empty()) {
            std::string confirm = getHiddenPassword("Confirm password: ");
            if (newPasswordTemp != confirm) {
                std::cout << "❌ Passwords do not match. Update cancelled.\n";
                continue;
            }
        }

        std::string newRoleTemp = askForUpdatedRole(user->getRole());
        if (newRoleTemp == "0") {
            std::cout << "Update cancelled.\n";
            continue; // Back to User ID
        }

        std::string finalUsername = newUsernameTemp.empty() ? user->getUsername() : newUsernameTemp;
        std::string finalPassword = newPasswordTemp.empty() ? user->getPassword() : sha256(newPasswordTemp);
        std::string finalRole = newRoleTemp.empty() ? user->getRole() : newRoleTemp;

        std::string lowerOldRole = toLower(user->getRole());
        std::string lowerNewRole = toLower(finalRole);

        size_t index = 0;
        for (; index < users.size(); ++index) {
            if (users[index]->getId() == id) break;
        }

        if (lowerOldRole != lowerNewRole) {
            if (lowerNewRole == "admin") {
                users[index] = std::make_unique<Admin>(id, finalUsername, finalPassword);
            } else if (lowerNewRole == "staff") {
                users[index] = std::make_unique<Staff>(id, finalUsername, finalPassword);
            } else if (lowerNewRole == "veterinarian") {
                users[index] = std::make_unique<Veterinarian>(id, finalUsername, finalPassword);
            } else {
                std::cout << "❌ Unknown role. Update aborted.\n";
                continue;
            }
        } else {
            user->setUsername(finalUsername);
            user->setPassword(finalPassword);
        }

        saveAllUsersToFile(users);
        std::cout << "✅ User updated successfully.\n";

        if (!promptYesNo("Would you like to update another user?")) {
            std::cout << "Returning to User Management Menu...\n";
            break;
        }
    }
}



void deleteUser(std::vector<std::unique_ptr<User>>& users) {
    while (true) {
        int id = askForValidId("Enter User ID to delete (or Enter/0 to cancel): ");
        if (id == 0) break;

        User* user = findUserById(users, id);
        if (!user) {
            std::cout << "User ID not found.\n";
            continue;
        }

        std::cout << "\nYou are about to delete the following user:\n\n";
        user->displayUserInfo();

        if (promptYesNo("\nAre you sure you want to delete this user?")) {
            users.erase(std::remove_if(users.begin(), users.end(),
                [id](const std::unique_ptr<User>& u) { return u->getId() == id; }),
                users.end());

            saveAllUsersToFile(users);
            std::cout << "User deleted successfully.\n";

            if (!promptYesNo("Would you like to delete another user?")) break;
        } else {
            std::cout << "Deletion cancelled.\n";
            break;
        }
    }
    std::cout << "Returning to User Management Menu...\n";
}

void saveAllUsersToFile(const std::vector<std::unique_ptr<User>>& users) {
    std::ofstream file("users.csv");
    if (!file) {
        std::cerr << "Could not open users.csv for writing.\n";
        return;
    }

    file << "user_id,username,password,role\n";
    for (const auto& u : users) {
        u->saveToFile(file);
    }
    file.close();
}