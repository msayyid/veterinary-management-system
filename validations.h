#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include <iostream>
#include "Pet.h"

// ===== Basic Validations =====

// Validates that the string represents a numeric ID
bool validateId(const std::string& idStr);
bool validateId(std::string& idStr);

// ===== Prompt-Based Validations =====

// Prompts user for a valid integer ID
int askForValidIntId(const std::string& prompt);

// Prompts for a valid age (positive integer), optional cancel
int askForValidAge(const std::string& prompt, bool allowCancel = false);

// Prompts for a valid name (non-empty, alpha), optional cancel
std::string askForValidName(const std::string& prompt, bool allowCancel = false);

// Prompts for a valid phone number format, optional cancel
std::string askForValidPhoneNumber(const std::string& prompt, bool allowCancel = false);

// Prompts for a valid email address, optional cancel
std::string askForValidEmail(const std::string& prompt, bool allowCancle = false);

// Prompts for a valid date (YYYY-MM-DD), with options for future dates and cancel
std::string askForValidDate(const std::string& prompt, bool allowFuture = true, bool allowCancel = false);

// Prompts for a valid 24-hour format time (HH:MM)
std::string askForValidTime(const std::string& prompt);

// Trims leading/trailing whitespace from a string
std::string trim(const std::string& s);

// Prompts for a valid address string, optional cancel
std::string askForValidAddress(const std::string& prompt, bool allowCancel = false);

// Prompts a yes/no question and returns true for yes
bool promptYesNo(const std::string& question);

// Prompts for and validates an ID input
int askForValidId(const std::string& prompt);

// Prompts for multi-line or long-form input with max length and optional cancel
std::string askForValidDetails(const std::string& prompt, size_t maxLength = 250, bool allowCancel = false);

// Prompts for a valid menu choice within range
int askForMenuChoice(int minOption, int maxOption, const std::string& prompt = "Enter your choice: ");

// ===== Update Prompts =====

// Prompts for updated values (used in update flows)
std::string askForUpdatedName(const std::string& prompt);
std::string askForUpdatedAddress(const std::string& prompt);
std::string askForUpdatedPhoneNumber(const std::string& prompt);
std::string askForUpdatedEmail(const std::string& prompt);

// ===== Vaccination =====

// Prompts for valid vaccination name
std::string askForValidVaccinationName(const std::string& prompt);

// Prompts for vaccination status (e.g., completed, pending)
std::string askForValidVaccinationStatus(const std::string& prompt);

// Prompts for updated vaccination status from current
std::string askForUpdatedVaccinationStatus(const std::string& currentStatus);

// ===== Lookup Helpers =====

// Finds a pet by ID from the pet vector
Pet* findPetById(std::vector<Pet>& pets, int id);

// Finds an owner by ID from the owner vector
Owner* findOwnerById(std::vector<Owner>& owners, int id);

// ===== Appointment Validation =====

// Prompts for valid appointment status (scheduled, completed, cancelled)
std::string askForValidAppointmentStatus(const std::string& prompt, bool allowCancel = true);

// Prompts for purpose of appointment
std::string askForValidAppointmentPurpose(const std::string& prompt, bool allowCancel = true);

// Date utilities
std::string toLower(const std::string& str);
bool isWeekend(int year, int month, int day);
bool isToday(int year, int month, int day);

// Prompts for valid appointment date with option to disallow weekends
std::string askForValidAppointmentDate(const std::string& prompt, bool disallowWeekends, bool allowCancel = true);

// Prompts for valid appointment time, optional cancel
std::string askForValidAppointmentTime(const std::string& prompt, bool allowCancel = true);

// Prompts for new date with cancel support (used in updates)
bool getNewDateWithCancel(std::string& newDate, const std::string& prompt = "Enter new date (YYYY-MM-DD): ", bool allowFuture = false);

// Prompts for updated vaccination status with cancel support
bool getNewVaccinationStatusWithCancel(std::string& newStatus);

// Prompts for updated date with optional future allowance
std::string askForUpdatedDate(const std::string& prompt, bool allowFuture);

// Prompts for updated record details
std::string askForUpdatedDetails(const std::string& prompt);

// Prompts for updated appointment fields
std::string askForUpdatedAppointmentDate(const std::string& currentDate, bool allowFuture = false);
std::string askForUpdatedAppointmentTime(const std::string& currentTime);
std::string askForUpdatedAppointmentPurpose(const std::string& currentPurpose);
std::string askForUpdatedAppointmentStatus(const std::string& currentStatus);

// ===== Authentication =====

// Prompts for and validates password input
std::string askForValidPassword(const std::string& prompt);

// Prompts for updated and new usernames
std::string askForUpdatedUsername(const std::string& current);
std::string askForValidUsername(const std::string& prompt);

// Prompts for updated user role (Admin, Vet, Staff)
std::string askForUpdatedRole(const std::string& currentRole);

// Prompts for password input in hidden/secure input mode
std::string getHiddenPassword(const std::string& prompt);

// Utility to capitalize each word in a string
std::string capitalizeWords(const std::string& input);

bool isPhoneNumberTaken(const std::string& phone);
bool isEmailTaken(const std::string& email);
std::string askForValidNameForUpdate(const std::string& prompt);
int askForValidAgeForUpdate(const std::string& prompt);
int askForOwnerIdOrOption(const std::string& prompt);
int askForMainMenuChoice(int minOption, int maxOption, const std::string& prompt);
int askForOwnerIdUpdate(const std::string& prompt, int currentId);



#endif  // VALIDATIONS_H