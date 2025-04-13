# Veterinary Management System (VMS)

This is a C++ terminal-based Veterinary Management System developed as part of the **Software Development 2 (CMP020C104S)** coursework. It enables clinic staff, veterinarians, and admins to manage pets, owners, appointments, medical records, vaccinations, and user roles in a structured and role-based environment. Data is persisted across sessions via CSV files, and role-based access ensures users can only perform permitted actions.

---

## 🧩 Features

- **Pet Management**: Add, view, update, and delete pet records. Link pets to owners.
- **Owner Management**: Store and manage owner contact info and records. View linked pets.
- **Appointment Scheduling**: Schedule, update, or cancel appointments. View appointment history.
- **Vaccination Tracking**: Add, update, and view pet vaccination status.
- **Medical & General Records**: Record pet medical history (vet-only) and general updates (staff).
- **User Management**: Admin can create, update, and delete users.
- **Role-Based Access Control**: Menus and features adapt to user roles (Admin / Vet / Staff).
- **Input Validation**: Ensures valid IDs, names, emails, phone numbers, dates, and times.
- **Password Hashing**: User passwords are stored securely using SHA-256 via OpenSSL.
- **Data Persistence**: All information is saved in `.csv` files and loaded at startup.
- **Terminal Menu Interface**: Clear, user-friendly command-line interface.

---

## 👤 User Roles & Permissions

| Role             | Capabilities                                                                 |
|--------------    |------------------------------------------------------------------------------|
| **Admin**        | Full access, including user creation, deletion, and role assignment.         |
| **Veterinarian** | Can manage medical records, vaccinations, and appointments.                  |
| **Staff**        | Can manage general pet info, appointments, and owner records.                |

---

## ⚙️ How to Compile and Run

### 🧱 Prerequisites

- C++17 compiler (`clang++` or `g++`)
- `openssl` installed (for SHA-256 password hashing)

> On macOS, install OpenSSL with:
> ```bash
> brew install openssl
> ```

---

### 🛠️ Option 1: Using the Makefile (Recommended)

If the `Makefile` is included:

```bash
make
./vet_system
```

To clean the build:
```bash
make clean
```

---

### ⚙️ Option 2: Manual Compilation (If not using Makefile)

```bash
clang++ -std=c++17 \
  -I/opt/homebrew/opt/openssl/include \
  -L/opt/homebrew/opt/openssl/lib \
  main.cpp menu.cpp Owner.cpp Pet.cpp Appointment.cpp User.cpp validations.cpp globals.cpp \
  pet_menu_helpers.cpp owner_menu_helpers.cpp appointment_menu_helpers.cpp user_menu_helpers.cpp \
  hashing.cpp \
  -lssl -lcrypto -o vet_system
```

Then run:

```bash
./vet_system
```

> Ensure the required `.csv` files (`pets.csv`, `owners.csv`, `appointments.csv`, `users.csv`) are present in the same directory.

---

## 📁 File Structure

| File/Folder                         | Description                                            |
|-------------------------------------|--------------------------------------------------------|
| `main.cpp`                          | Entry point of the system                              |
| `Pet.*`, `Owner.*`, `Appointment.*` | Core domain logic for each entity                      |
| `User.*`                            | Abstract user class and role-specific subclasses       |
| `*_menu_helpers.*`                  | Modularized menu logic (pet, owner, appointment, user) |
| `validations.*`                     | Input validation and prompts                           |
| `globals.*`                         | Shared data and persistence logic                      |
| `hashing.*`                         | SHA-256 password hashing using OpenSSL                 |
| `Makefile`                          | Automates the compilation process                      |
| `README.md`                         | This documentation file                                |
| `*.csv`                             | Data files used to load/save records                   |

---

## 📝 Notes & Assumptions

- Appointment time is restricted between 08:00 and 20:00.
- Dates must be valid `YYYY-MM-DD`, time format is `HH:MM`.
- Weekend scheduling can be restricted based on system validation.
- Passwords are stored using SHA-256 hashes, not in plain text.
- The system is menu-driven and terminal-based only — no GUI.
- No duplicates of usernames, owner emails or owner phone numbers can be created.

---

## 👨‍💻 Author

Developed by **Mukhammadsaiid Norbaev & Shamsul Huda Samim**  
Spring 2025 — SD2 Coursework  
University of Roehampton, London

```
