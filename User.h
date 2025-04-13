#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <memory>

// Abstract base class representing a system user with role-based permissions.
// Subclasses include Admin, Veterinarian, and Staff.
class User {
protected:
    int userId;
    std::string username;
    std::string password;

public:
    // Constructs a user with the given ID, username, and password
    User(int userId, const std::string& username, const std::string& password);
    virtual ~User() = default;

    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;

    // Setters
    void setUsername(const std::string& newUsername);
    void setPassword(const std::string& newPassword);

    // Role information 
    virtual std::string getRole() const = 0;
    virtual void showMenu() const = 0;

    // Permissions =
    virtual bool canManageUsers() const = 0;
    virtual bool canManageMedicalRecords() const = 0;
    virtual bool canManageGeneralPetRecords() const = 0;
    virtual bool canManageVaccinations() const = 0;
    virtual bool canManageAppointments() const = 0;
    virtual bool canDeletePet() const = 0;
    virtual bool canDeleteOwner() const = 0;
    virtual bool canManageOwnerRecords() const = 0;

    // File I/O for saving user info
    virtual void saveToFile(std::ostream& out) const = 0;
    static std::vector<std::unique_ptr<User>> loadFromFile(const std::string& filename);

    // Authenticate
    static User* authenticateUser(const std::vector<std::unique_ptr<User>>& users,
                                  const std::string& username,
                                  const std::string& password);

    // Displays user information
    virtual void displayUserInfo() const;
};

// Represents an admin user with full permissions
class Admin : public User {
public:
    Admin(int userId, const std::string& username, const std::string& password);

    std::string getRole() const override;
    void showMenu() const override;
    void saveToFile(std::ostream& out) const override;

    // Admin permissions
    bool canManageUsers() const override;
    bool canManageMedicalRecords() const override;
    bool canManageGeneralPetRecords() const override;
    bool canManageVaccinations() const override;
    bool canManageAppointments() const override;
    bool canDeletePet() const override;
    bool canDeleteOwner() const override;
    bool canManageOwnerRecords() const override;
};

// Represents a veterinarian user with permissions related to medical data
class Veterinarian : public User {
public:
    Veterinarian(int userId, const std::string& username, const std::string& password);

    std::string getRole() const override;
    void showMenu() const override;
    void saveToFile(std::ostream& out) const override;

    // Veterinarian permissions
    bool canManageUsers() const override;
    bool canManageMedicalRecords() const override;
    bool canManageGeneralPetRecords() const override;
    bool canManageVaccinations() const override;
    bool canManageAppointments() const override;
    bool canDeletePet() const override;
    bool canDeleteOwner() const override;
    bool canManageOwnerRecords() const override;
};

// Represents a staff user with limited permissions
class Staff : public User {
public:
    Staff(int userId, const std::string& username, const std::string& password);

    std::string getRole() const override;
    void showMenu() const override;
    void saveToFile(std::ostream& out) const override;

    // Staff permissions
    bool canManageUsers() const override;
    bool canManageMedicalRecords() const override;
    bool canManageGeneralPetRecords() const override;
    bool canManageVaccinations() const override;
    bool canManageAppointments() const override;
    bool canDeletePet() const override;
    bool canDeleteOwner() const override;
    bool canManageOwnerRecords() const override;
};

// Utility functions

// Finds and returns a user pointer by ID from a vector of unique_ptrs
User* findUserById(const std::vector<std::unique_ptr<User>>& users, int id);

// Overload for raw User object references
User* findUserById(std::vector<User>& users, int id);

// function to create a user object dynamically based on role
std::unique_ptr<User> createUser(int id, const std::string& username, const std::string& rawPassword, const std::string& role);

#endif  // USER_H