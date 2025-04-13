 // Owner owner1(1, "Muhammad Ali", "1 Roehampton", "123434534", "sample1@google.com");
    // owner1.addRecord("2024-04-07", "Paid £8750.");
    // owner1.addRecord("2025-02-03", "just checking brother.");
    // owner1.saveToFile("owners.csv");
    // owner1.displayRecords();
    // owner1.getAddress();
    // Owner owner(2, "Harry Potter", "345 Severus", "777434534", "sampl23e@google.com");
    // owner.addRecord("2025-04-07", "Paid £5400.");
    // owner.addRecord("2023-10-07", "Paid £89.");
    // owner.saveToFile("owners.csv");
    // owner.displayRecords();

    // load
    // Owner loadedOwner("", "", "", "");
    // loadedOwner.loadFromFile("owners.csv");
    // loadedOwner.displayRecords();

    // load owners from file
    // std::vector<Owner> owners = Owner::loadFromFile("owners.csv");
    // for (const auto& owner : owners) {
    //     std::cout << "\nOwner: " << owner.getName() << "\n";
    //     owner.displayRecords();
    // }


    // pet class test
    // // Create Pets
    // Pet pet1(1, "Tommy", "Golden Retriever", 5, 1);  // Pet ID 1, Owner ID 1
    // Pet pet2(2, "Mittens", "British Shorthair", 3, 2); // Pet ID 2, Owner ID 2
    // Pet pet3(3, "Rex", "German Shepherd", 4, 1); // Pet ID 3, Owner ID 1

    // // Add Vaccinations
    // pet1.addVaccination("Rabies", "2023-01-01", "completed");
    // pet1.addVaccination("Distemper", "2023-06-01", "completed");

    // pet2.addVaccination("Rabies", "2023-02-15", "completed");
    // pet2.addVaccination("Leukemia", "2023-03-20", "pending");

    // pet3.addVaccination("Parvo", "2023-05-05", "completed");

    // // Add Medical History
    // pet1.addMedicalHistory("2022-12-01", "Annual checkup");
    // pet1.addMedicalHistory("2023-08-01", "Ear infection treatment");

    // pet2.addMedicalHistory("2023-04-10", "Dental cleaning");

    // pet3.addMedicalHistory("2023-06-15", "Leg surgery");
    // pet3.addMedicalHistory("2023-09-01", "Vaccination booster");

    // // Add Pet Records
    // pet1.addPetRecord("2023-07-01", "Grooming");
    // pet1.addPetRecord("2023-11-01", "Special diet prescribed");

    // pet2.addPetRecord("2023-05-05", "Behavioral assessment");

    // pet3.addPetRecord("2023-10-10", "Training session");

    // // Save pets to file
    // pet1.saveToFile("pets.csv");
    // pet2.saveToFile("pets.csv");
    // pet3.saveToFile("pets.csv");

    // std::cout << "Pets data saved to pets.csv.\n";

    // owner class test
    // Create owners
     // Owner 1 with multiple pets
    // Owner owner1(1, "John Doe", "123 Main St", "555-1234", "john@example.com");
    // owner1.addPetId(100); // Pet IDs linked to this owner
    // owner1.addPetId(101);
    // owner1.addRecord("2023-05-01", "Address updated");
    // owner1.addRecord("2023-06-15", "Annual health check");

    // // Owner 2 with one pet
    // Owner owner2(2, "Jane Smith", "456 Elm St", "555-5678", "jane@example.com");
    // owner2.addPetId(102);
    // owner2.addRecord("2022-12-10", "New pet registration");

    // // Owner 3 with no pets (yet)
    // Owner owner3(3, "Robert Brown", "789 Oak St", "555-9999", "robert@example.com");
    // owner3.addRecord("2024-01-20", "Inquiry about pet adoption");

    // // Save owners to file
    // owner1.saveToFile("owners.csv");
    // owner2.saveToFile("owners.csv");
    // owner3.saveToFile("owners.csv");

    // // Load back and display to verify
    // auto owners = Owner::loadFromFile("owners.csv");

    // std::cout << "\n=== Owners and their Pets & Records ===\n";
    // for (const auto& o : owners) {
    //     std::cout << "\nOwner: " << o.getName() << "\n";
    //     o.displayPets();
    //     o.displayRecords();
    // }


    // appointment class test

    // std::vector<Appointment> appointments;

    // // 1. Create Appointments
    // Appointment a1(1, 1, 101, "2025-03-20", "10:00", "Vaccination", "scheduled");
    // Appointment a2(2, 2, 102, "2025-03-21", "14:30", "Check-up", "completed");
    // Appointment a3(3, 3, 103, "2025-03-22", "16:00", "Surgery follow-up", "scheduled");

    // // 2. Display them
    // std::cout << "=== Created Appointments ===\n";
    // a1.displayAppointmentDetails();
    // a2.displayAppointmentDetails();
    // a3.displayAppointmentDetails();

    // // 3. Save to file
    // a1.saveToFile("appointments.csv");
    // a2.saveToFile("appointments.csv");
    // a3.saveToFile("appointments.csv");

    // // 4. Load back from file
    // std::vector<Appointment> loadedAppointments = Appointment::loadFromFile("appointments.csv");

    // // 5. Display loaded appointments
    // std::cout << "\n=== Loaded Appointments from File ===\n";
    // for (const auto& appt : loadedAppointments) {
    //     appt.displayAppointmentDetails();
    // }