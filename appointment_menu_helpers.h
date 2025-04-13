#ifndef APPOINTMENT_MENU_HELPERS_H
#define APPOINTMENT_MENU_HELPERS_H

#include "Appointment.h"
#include <vector>

// Displays all appointments in the system along with their details and status.
void viewAllAppointments(const std::vector<Appointment>& appointments);

// Searches for a specific appointment by its unique ID and displays it.
void searchAppointmentById(std::vector<Appointment>& appointments);

// Displays all appointments linked to a specific owner by owner ID.
void searchAppointmentsByOwnerId(const std::vector<Appointment>& appointments);

// Displays all appointments linked to a specific pet by pet ID.
void searchAppointmentsByPetId(const std::vector<Appointment>& appointments);

// Adds a new appointment, linking it to a pet and owner with validated input.
void addAppointment(std::vector<Appointment>& appointments, std::vector<Owner>& owners, std::vector<Pet>& pets, int& nextAppointmentId);

// Updates the date, time, or status of an existing appointment.
void updateAppointment(std::vector<Appointment>& appointments);

// Deletes an appointment from the system by ID, with user confirmation.
void deleteAppointment(std::vector<Appointment>& appointments);

#endif // APPOINTMENT_MENU_HELPERS_H
