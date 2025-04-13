#ifndef owner_menu_helpers
#define owner_menu_helpers

#include "Owner.h"
#include "validations.h"
#include "globals.h"

// Adds a new pet owner by collecting validated user input.
void addNewOwner();

// Displays a list of all registered pet owners.
void viewAllOwners();

// Shows detailed information about a specific owner based on their ID.
void viewOwnerDetailsById();

// Updates contact details or other information of an existing owner.
void updateOwner();

// Deletes an owner from the system and unlinks their pets, with confirmation.
void deleteOwner();

// Displays all pets associated with a specific owner.
void viewPetsLinkedToOwner();

// Provides access to manage a specific owner's record (view, update, delete).
void manageOwnerRecords();

// Displays and manages appointments linked to a specific owner.
void manageOwnerAppointments();

#endif  // owner_helper