#ifndef pet_menu_helpers
#define pet_menu_helpers

#include "Pet.h"
#include "validations.h"
#include "globals.h"

// Adds a new pet to the system with validated user input.
void addNewPet();

// Displays a list of all pets currently registered in the system.
void viewAllPets();

// Shows detailed information about a specific pet by its ID.
void viewPetDetailsById();

// Allows a veterinarian to view or update a pet's medical records.
void manageMedicalRecords();

// Allows staff or veterinarians to update general pet information (name, breed, etc.).
void manageGeneralRecords();

// Allows veterinarians to update or review vaccination records for a pet.
void manageVaccinations();

// Displays and manages appointments associated with a specific pet.
void manageAppointments();

// Deletes a pet from the system and unlinks all associated records.
void deletePet();

// Links an existing pet to a registered owner.
void linkPetToOwner();
// updates an existing pet data
void updatePet();


#endif  // pet_helper
