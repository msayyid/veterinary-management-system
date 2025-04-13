// menu.h


#ifndef MENU_H
#define MENU_H
#include "User.h"


// Launches the main menu and routes the user to appropriate sections based on role.
void mainMenu(User& user);

// Displays the pet management menu, allowing role-based access to pet features.
void petMenu(User& user);

// Displays the owner management menu for adding, viewing, updating, or deleting owners.
void ownerMenu(User& user);

// Displays the appointment management menu, allowing scheduling, updates, and cancellations.
void appointmentMenu(User& user);

// Displays the user management menu, available only to admin users.
void userMenu(User& user);


#endif
