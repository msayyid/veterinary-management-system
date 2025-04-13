#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include <string>
#include <vector>

class Pet;
class Owner;

// Represents a veterinary appointment linked to a specific pet and owner.
// Stores details like date, time, purpose, and current status (scheduled, completed, etc.).
class Appointment {
    int appointmentId;
    int ownerId;
    int petId;
    // int nextAppointmentId;
    std::string date;    // Format: YYYY-MM-DD
    std::string time;    // Format: HH:MM
    std::string purpose; // Reason for appointment: vaccination, check-up, etc.
    std::string status;  // Appointment status: scheduled, completed, cancelled

public:
    // Constructs an Appointment with specified details.
    Appointment(int id, int ownerId, int petId, const std::string& date, const std::string& time, const std::string& purpose, const std::string& status);

    // Getter methods
    int getAppointmentId() const;           // Returns the appointment ID
    int getOwnerId() const;                 // Returns the owner ID
    int getPetId() const;                   // Returns the pet ID
    std::string getDate() const;            // Returns the appointment date
    std::string getTime() const;            // Returns the appointment time
    std::string getPurpose() const;         // Returns the purpose of the appointment
    std::string getStatus() const;          // Returns the status of the appointment

    // Setter methods for updating individual fields
    void updateStatus(const std::string& newStatus);     // Updates the status
    void updateDate(const std::string& newDate);         // Updates the date
    void updateTime(const std::string& newTime);         // Updates the time
    void setOwnerId(int newOwnerId);                     // Updates the owner ID

    void setDate(const std::string& newDate);            // Sets a new date
    void setTime(const std::string& newTime);            // Sets a new time
    void setPurpose(const std::string& newPurpose);      // Sets a new purpose
    void setStatus(const std::string& newStatus);        // Sets a new status

    // Display methods for showing appointment details
    void displayAppointmentDetails() const;              // Prints full appointment details
    void displayAsTableRow() const;                      // Displays one row (compact format)
    void displayFullAppointment() const;                 // Full appointment display for one record
    static void displayAppointmentsTable(const std::vector<Appointment>& appointments); // Table view of multiple appointments

    // File handling methods
    void saveToFile(const std::string& filename) const;                          // Saves this appointment to file
    static std::vector<Appointment> loadFromFile(const std::string& filename);   // Loads all appointments from file
    static void displayAppointmentsTable(const std::vector<Appointment*>& appts); // Table view from vector of pointers
};

// Finds and returns a pointer to an appointment by its ID from the given list.
Appointment* findAppointmentById(std::vector<Appointment>& appointments, int id);

#endif  // APPOINTMENT_H