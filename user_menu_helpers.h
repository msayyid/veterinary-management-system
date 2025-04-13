#ifndef USER_MENU_HELPERS_H
#define USER_MENU_HELPERS_H

#include "User.h"
#include <vector>
#include <memory>

// Displays a list of all registered users along with their roles.
void viewAllUsers(const std::vector<std::unique_ptr<User>>& users);

// Adds a new user (Admin, Veterinarian, or Staff) to the system.
void addNewUser(std::vector<std::unique_ptr<User>>& users, int& nextUserId);

// Updates an existing user's username, password, or role.
void updateUser(std::vector<std::unique_ptr<User>>& users);

// Deletes a user from the system based on their ID.
void deleteUser(std::vector<std::unique_ptr<User>>& users);

// Saves all user data to a CSV file for persistence.
void saveAllUsersToFile(const std::vector<std::unique_ptr<User>>& users);

#endif  // USER_MENU_HELPERS_H
