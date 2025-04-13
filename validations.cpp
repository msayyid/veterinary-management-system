#include "validations.h"
#include <iostream>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include <cctype>
#include <sstream>
#include <ctime>
#include <termios.h>
#include <unistd.h>

std::string getHiddenPassword(const std::string& prompt) {
    std::string password;
    std::cout << prompt;

    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;

    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::getline(std::cin, password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    std::cout << std::endl;
    return password;
}

std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return std::string(start, end + 1);
}


bool validateId(std::string& idStr) {
    idStr = trim(idStr);

    if (idStr.empty()) {
        std::cout << "Input cannot be empty. Please enter a valid ID.\n";
        return false;
    }

    if (idStr.length() > 5) {
        std::cout << "ID too long! Please enter a number up to 5 digits.\n";
        return false;
    }

    for (char c : idStr) {
        if (!isdigit(c)) {
            std::cout << "ID must contain only digits. Try again.\n";
            return false;
        }
    }

    return true;
}

int askForValidId(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input); // get input and trim leading whitespace
        input = trim(input);


        if (input.empty()) {
            return 0;
        }

        // Check if all digits
        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Invalid input! ID should be numeric only. Try again.\n";
            continue;
        }

        // Check length (max 5 digits)
        if (input.length() > 5) {
            std::cout << "ID too long! Please enter an ID with up to 5 digits.\n";
            continue;
        }

        // If all checks pass, convert and return
        return std::stoi(input);
    }
}

std::string askForValidVaccinationName(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name);
        name = trim(name);

        if (name.empty() || name == "0") {
            return "";
        }


        if (name.length() > 50) {
            std::cout << "Invalid name. Must not be longer than 50 characters.\n";
            continue;
        }

        // Disallow commas and semicolons
        if (name.find(',') != std::string::npos || name.find(';') != std::string::npos) {
            std::cout << "Invalid vaccination name. Cannot contain commas or semicolons.\n";
            continue;
        }

        return name;
    }
}

std::string askForValidVaccinationStatus(const std::string& prompt) {
    std::string status;
    while (true) {
        std::cout << prompt << " (completed / pending / booster required): ";
        std::getline(std::cin, status);
        status = trim(status);

        if (status.empty() || status == "0") {
            return "";
        }

        // Convert to lowercase for checking
        std::string lowerStatus = status;
        std::transform(lowerStatus.begin(), lowerStatus.end(), lowerStatus.begin(), ::tolower);

        if (lowerStatus == "completed" || lowerStatus == "pending" || lowerStatus == "booster required") {
            return lowerStatus;
        } else {
            std::cout << "Invalid status. Please enter either 'completed', 'pending', or 'booster required'.\n";
        }
    }
}


Pet* findPetById(std::vector<Pet>& pets, int id) { 
    for (auto& p : pets) { 
        if (p.getPetId() == id) { 
            return &p; 
        } 
    } 
    return nullptr; 
}

Owner* findOwnerById(std::vector<Owner>& owners, int id) {
    for (auto& o : owners) {
        if (o.getOwnerId() == id) {
            return &o;
        }
    }
    return nullptr;
}


int askForValidIntId(const std::string& prompt) {
    std::string idStr;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, idStr);
        idStr = trim(idStr); // cleaner than repeating erase logic

        if (idStr.empty()) {
            std::cout << "Invalid ID. Please try again.\n";
            continue;
        }

        if (std::all_of(idStr.begin(), idStr.end(), ::isdigit)) {
            try {
                return std::stoi(idStr);
            } catch (...) {
                std::cout << "Invalid ID. Number is too large.\n";
            }
        } else {
            std::cout << "Invalid ID. Please enter digits only.\n";
        }
    }
}


int askForValidAge(const std::string& prompt, bool allowCancel) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);

        if (allowCancel && (line.empty() || line == "0")) {
            return -1; // canceled
        }

        std::stringstream ss(line);
        int age;
        char extra;
        if (!(ss >> age) || (ss >> extra)) {
            // failed conversion or there was extra junk
            std::cout << "Invalid input. Please enter a valid integer.\n";
            continue;
        }

        if (age < 0 || age > 50) {
            std::cout << "Invalid age. Please enter a number between 0 and 50.\n";
            continue;
        }

        return age;
    }
}


std::string toLower(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}


std::string askForValidName(const std::string& prompt, bool allowCancel) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name); // no std::ws
        name = trim(name);

        if (allowCancel && (name.empty() || name == "0" || toLower(name) == "return")) {
            return "";
        }

        if (name.empty()) {
            std::cout << "Invalid name. Must not be empty.\n";
            continue;
        }
        if (name.length() > 50) {
            std::cout << "Invalid name. Must not be longer than 50 characters.\n";
            continue;
        }

        bool valid = true;
        for (auto& c : name) {
            if (!isalpha(c) && c != '\'' && c != '-' && c != ' ') {
                valid = false;
                break;
            }
        }
        if (valid) {
            return name;
        } else {
            std::cout << "Invalid name. Only letters, spaces, apostrophes (') and hyphens (-) are allowed.\n";
        }
    }
}


std::string askForValidPhoneNumber(const std::string& prompt, bool allowCancel) {
    std::string phone;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, phone);
        phone = trim(phone);

        if (allowCancel && (phone.empty() || phone == "0" || toLower(phone) == "return")) {
            return "";
        }

        bool isValid = true;

        if (phone.length() != 11) {
            std::cout << "Invalid length! Must be exactly 11 digits.\n";
            isValid = false;
            continue;
        }

        if (!std::all_of(phone.begin(), phone.end(), ::isdigit)) {
            std::cout << "Only digits allowed!\n";
            isValid = false;
            continue;
        }

        if (phone.substr(0, 2) != "07") {
            std::cout << "Phone number must start with '07'.\n";
            isValid = false;
            continue;
        }

        if (isValid) {
            return phone;
        } else {
            std::cout << "Please try again.\n";
        }
    }
}



std::string askForValidEmail(const std::string& prompt, bool allowCancel) {
    std::regex email_pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    std::string email;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, email);
        email = trim(email);

        if (allowCancel && (email.empty() || email == "0" || toLower(email) == "return")) {
            return "";
        }


        if (email.empty()) {
            std::cout << "Email cannot be empty.\n";
            continue;
        }

        if (email.length() > 254) {
            std::cout << "Email too long! Must be under 255 characters.\n";
            continue;
        }

        if (!std::regex_match(email, email_pattern)) {
            std::cout << "Invalid email format! Example: user@example.com\n";
            continue;
        }

        return email;
    }
}



int getCurrentYear() {
    time_t now = time(0); // get current system time
    tm* localTime = localtime(&now); // convert to local time structure
    return 1900 + localTime->tm_year; // tm_year is years since 1900
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}


bool isValidDate(int day, int month, int year, bool allowFuture) {
    int currentYear = getCurrentYear();
    int maxYear = allowFuture ? currentYear + 2 : currentYear;

    if (year < 2000 || year > maxYear) {
        std::cout << "Year out of valid range (2000-" << maxYear << "). Try again.\n";
        return false;
    }

    if (month > 12 || month < 1) { return false; }

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    return (day >= 1 && day <= daysInMonth[month - 1]);
}


bool isFutureDate(int year, int month, int day) {
    // check ??????
    time_t now = time(0);
    tm* localTime = localtime(&now);
    int currentYear = 1900 + localTime->tm_year;
    int currentMonth = 1 + localTime->tm_mon;
    int currentDay = localTime->tm_mday;

    if (year > currentYear) return true;
    if (year == currentYear && month > currentMonth) return true;
    if (year == currentYear && month == currentMonth && day > currentDay) return true;

    return false;
}



std::string askForValidDate(const std::string& prompt, bool allowFuture, bool allowCancel) {
    std::string date;
    std::regex date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, date);
        date = trim(date);

        if (allowCancel && (date.empty() || date == "0" || toLower(date) == "return")) {
            return "";
        }

        if (!std::regex_match(date, date_pattern)) {
            std::cout << "Invalid format! Use YYYY-MM-DD.\n";
            continue;
        }

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (!isValidDate(day, month, year, allowFuture)) {
            std::cout << "Invalid logical date or year out of allowed range.\n";
            continue;
        }

        if (!allowFuture && isFutureDate(year, month, day)) {
            std::cout << "Date cannot be in the future.\n";
            continue;
        }

        return date;
    }
}






std::string askForValidAddress(const std::string& prompt, bool allowCancel) {
    // Allowed characters: letters, numbers, spaces, commas, periods, hyphens, apostrophes, slashes
    std::regex address_pattern(R"(^[A-Za-z0-9\s,.\-'/]+$)");
    std::string address;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, address);
        address = trim(address); // remove leading/trailing spaces

        if (allowCancel && (address.empty() || address == "0" || toLower(address) == "return")) {
            return "";
        }

        // Check if empty after trimming
        if (address.empty()) {
            std::cout << "Address cannot be empty. Please try again.\n";
            continue;
        }

        // Length check
        if (address.length() > 100) {
            std::cout << "Address too long! Must be under 100 characters.\n";
            continue;
        }

        // Validate using regex pattern
        if (!std::regex_match(address, address_pattern)) {
            std::cout << "Invalid address format! Allowed characters: letters, numbers, spaces, commas (,), periods (.), hyphens (-), apostrophes ('), slashes (/).\n";
            continue;
        }

        // If all checks pass, return
        return address;
    }
}


std::string askForValidTime(const std::string& prompt) {
    std::string time;
    std::regex time_pattern(R"(^\d{2}:\d{2}$)");
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, time);
        time = trim(time);

        if (time.empty()) {
            std::cout << "Time cannot be empty.\n";
            continue;
        }

        if (!std::regex_match(time, time_pattern)) {
            std::cout << "Invalid format! Use HH:MM.\n";
            continue;
        }

        int hour = std::stoi(time.substr(0, 2));
        int minute = std::stoi(time.substr(3, 2));

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            std::cout << "Invalid time values! Hours: 0-23, Minutes: 0-59.\n";
            continue;
        }

        return time;
    }

}


std::string askForValidAppointmentStatus(const std::string& prompt, bool allowCancel) {
    std::string status;
    while (true) {
        std::cout << prompt << " (scheduled / completed / cancelled): ";
        std::getline(std::cin, status);
        status = trim(status);

        if (allowCancel && (status.empty() || status == "0" || toLower(status) == "return")) {
            return ""; // Return empty string to signal cancellation
        }

        if (status == "scheduled" || status == "completed" || status == "cancelled") {
            return status;
        } else {
            std::cout << "Invalid status. Please enter 'scheduled', 'completed', or 'cancelled'.\n";
        }
    }
}


std::string askForValidAppointmentPurpose(const std::string& prompt, bool allowCancel) {
    std::string purpose;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, purpose);
        purpose = trim(purpose);

        if (allowCancel && (purpose.empty() || purpose == "0" || toLower(purpose) == "return")) {
            return ""; // Return empty string to signal cancellation
        }

        if (purpose.length() > 100) {
            std::cout << "Purpose too long. Please limit to 100 characters.\n";
            continue;
        }

        return purpose;
    }
}



bool promptYesNo(const std::string& question) {
    std::string response;
    while (true) {
        std::cout << question << " (y/n): ";
        std::getline(std::cin, response);

        // trim
        response.erase(0, response.find_first_not_of(" \t\n\r\f\v"));
        response.erase(response.find_last_not_of(" \t\n\r\f\v") + 1);

        if (response.empty()) {
            std::cout << "Response cannot be empty. Please enter 'y' or 'n'.\n";
            continue;
        }

        // convert to lowercase for consistency
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);

        if (response == "y") {
            return true;
        } else if (response == "n") {
            return false;
        } else {
            std::cout << "Invalid input! Please respond with 'y' or 'n'.\n";
        }
    }
}
std::string askForValidDetails(const std::string& prompt, size_t maxLength, bool allowCancel) {
    std::string details;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, details);  // ✅ allow empty input
        details = trim(details);

        if (allowCancel && (details.empty() || details == "0")) {
            return "";
        }

        if (details.length() > maxLength) {
            std::cout << "Details too long! Maximum allowed length is " << maxLength << " characters.\n";
            continue;
        }

        bool invalidChar = false;
        for (char c : details) {
            if (iscntrl(c) && c != '\n' && c != '\t') {
                invalidChar = true;
                break;
            }
        }
        if (invalidChar) {
            std::cout << "Invalid characters detected. Please re-enter.\n";
            continue;
        }

        return details;
    }
}



int askForMenuChoice(int minOption, int maxOption, const std::string& prompt) {
    while (true) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty()) {
            std::cout << "Input cannot be empty! Please enter a choice.\n";
            continue;
        }

        if (!validateId(input)) { 
            std::cout << "Invalid input! Please enter a number between " << minOption << " and " << maxOption << ".\n";
            continue;
        }

        int choice = std::stoi(input);
        if (choice < minOption || choice > maxOption) {
            std::cout << "Invalid choice! Please select a number between " << minOption << " and " << maxOption << ".\n";
            continue;
        }

        return choice;
    }
}

bool isToday(int year, int month, int day) {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    int currentYear = 1900 + localTime->tm_year;
    int currentMonth = 1 + localTime->tm_mon;
    int currentDay = localTime->tm_mday;

    return (year == currentYear && month == currentMonth && day == currentDay);
}


bool isWeekend(int year, int month, int day) {
    std::tm timeStruct = {};
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_mday = day;
    std::mktime(&timeStruct);
    return (timeStruct.tm_wday == 0 || timeStruct.tm_wday == 6); // Sunday=0, Saturday=6
}

std::string askForValidAppointmentDate(const std::string& prompt, bool disallowWeekends, bool allowCancel) {
    std::string date;
    std::regex date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, date);
        date = trim(date);

        if (allowCancel && (date.empty() || date == "0" || toLower(date) == "return")) {
            return ""; // Return empty string to signal cancellation
        }

        if (!std::regex_match(date, date_pattern)) {
            std::cout << "Invalid format! Use YYYY-MM-DD.\n";
            continue;
        }

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (!isValidDate(day, month, year, true)) {
            std::cout << "Invalid logical date or year out of range.\n";
            continue;
        }

        if (!isFutureDate(year, month, day) && !isToday(year, month, day)) {
            std::cout << "Appointments cannot be scheduled in the past.\n";
            continue;
        }

        if (disallowWeekends && isWeekend(year, month, day)) {
            std::cout << "Appointments cannot be scheduled on weekends.\n";
            continue;
        }

        return date;
    }
}






std::string askForValidAppointmentTime(const std::string& prompt, bool allowCancel) {
    std::string time;
    std::regex time_pattern(R"(^\d{2}:\d{2}$)");
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, time);
        time = trim(time);

        if (allowCancel && (time.empty() || time == "0" || toLower(time) == "return")) {
            return ""; // Return empty string to signal cancellation
        }

        if (!std::regex_match(time, time_pattern)) {
            std::cout << "Invalid format! Use HH:MM.\n";
            continue;
        }

        int hour = std::stoi(time.substr(0, 2));
        int minute = std::stoi(time.substr(3, 2));

        if (hour < 8 || hour > 20 || minute < 0 || minute > 59) {
            std::cout << "Appointments can only be scheduled between 08:00 and 20:00.\n";
            continue;
        }

        return time;
    }
}



bool getNewDateWithCancel(std::string& newDate, const std::string& prompt, bool allowFuture) {
    std::string input;
    std::cout << prompt << "(or press Enter/0 to cancel): ";
    std::getline(std::cin, input);
    input = trim(input);

    if (input.empty() || input == "0") {
        return false;  // user cancelled
    }

    // Now pass it to the original askForValidDate logic manually
    std::stringstream ss(input);
    ss >> input; // reuse the same trimmed input
    if (std::regex_match(input, std::regex(R"(^\d{4}-\d{2}-\d{2}$)"))) {
        int year = std::stoi(input.substr(0, 4));
        int month = std::stoi(input.substr(5, 2));
        int day = std::stoi(input.substr(8, 2));

        if (!isValidDate(day, month, year, allowFuture)) return false;
        if (!allowFuture && isFutureDate(year, month, day)) return false;

        newDate = input;
        return true;
    }

    std::cout << "Invalid date format.\n";
    return false;
}

bool getNewVaccinationStatusWithCancel(std::string& newStatus) {
    std::cout << "Enter new vaccination status (completed / pending / booster required) (or press Enter/0 to cancel): ";
    std::getline(std::cin, newStatus);
    newStatus = trim(newStatus);
    if (newStatus.empty() || newStatus == "0") return false;

    std::vector<std::string> allowed = { "completed", "pending", "booster required" };
    if (std::find(allowed.begin(), allowed.end(), newStatus) == allowed.end()) {
        std::cout << "Invalid status entered.\n";
        return false;
    }

    return true;
}


std::string askForUpdatedName(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name);
        name = trim(name);

        if (name.empty()) return "";  // keep existing
        if (name == "0") return "0";  // cancel update

        if (name.length() > 50) {
            std::cout << "Name too long. Max 50 characters.\n";
            continue;
        }

        bool valid = std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalpha(c) || c == ' ' || c == '\'' || c == '-';
        });

        if (valid) return name;
        std::cout << "Invalid name. Only letters, spaces, apostrophes (') and hyphens (-) are allowed.\n";
    }
}

std::string askForUpdatedAddress(const std::string& prompt) {
    std::regex address_pattern(R"(^[A-Za-z0-9\s,.\-'/]+$)");
    std::string address;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, address);
        address = trim(address);

        if (address.empty()) return "";
        if (address == "0") return "0";

        if (address.length() > 100) {
            std::cout << "Address too long! Must be under 100 characters.\n";
            continue;
        }

        if (!std::regex_match(address, address_pattern)) {
            std::cout << "Invalid address format. Allowed: letters, numbers, spaces, , . - ' /\n";
            continue;
        }

        return address;
    }
}


std::string askForUpdatedPhoneNumber(const std::string& prompt) {
    std::string phone;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, phone);
        phone = trim(phone);

        if (phone.empty()) return "";
        if (phone == "0") return "0";

        if (phone.length() != 11) {
            std::cout << "Phone number must be exactly 11 digits.\n";
            continue;
        }

        if (!std::all_of(phone.begin(), phone.end(), ::isdigit)) {
            std::cout << "Phone number must contain only digits.\n";
            continue;
        }

        if (phone.substr(0, 2) != "07") {
            std::cout << "Phone number must start with '07'.\n";
            continue;
        }

        return phone;
    }
}


std::string askForUpdatedEmail(const std::string& prompt) {
    std::regex email_pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    std::string email;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, email);
        email = trim(email);

        if (email.empty()) return "";
        if (email == "0") return "0";

        if (email.length() > 254) {
            std::cout << "Email too long! Must be under 255 characters.\n";
            continue;
        }

        if (!std::regex_match(email, email_pattern)) {
            std::cout << "Invalid email format! Example: user@example.com\n";
            continue;
        }

        return email;
    }
}

std::string askForUpdatedDate(const std::string& prompt, bool allowFuture) {
    std::regex date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    std::string date;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, date);
        date = trim(date);

        if (date.empty()) return "";       // keep existing
        if (date == "0") return "0";       // cancel update

        if (!std::regex_match(date, date_pattern)) {
            std::cout << "Invalid format! Use YYYY-MM-DD.\n";
            continue;
        }

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (!isValidDate(day, month, year, allowFuture)) {
            std::cout << "Invalid logical date or year out of allowed range.\n";
            continue;
        }

        if (!allowFuture && isFutureDate(year, month, day)) {
            std::cout << "Date cannot be in the future.\n";
            continue;
        }

        return date;
    }
}

std::string askForUpdatedDetails(const std::string& prompt) {
    std::string details;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, details);
        details = trim(details);

        if (details.empty()) return "";   // keep existing
        if (details == "0") return "0";   // cancel update

        if (details.length() > 200) {
            std::cout << "Details too long. Max 200 characters.\n";
            continue;
        }

        return details;
    }
}

std::string askForUpdatedAppointmentDate(const std::string& currentDate, bool allowFuture) {
    std::regex date_pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    std::string date;

    while (true) {
        std::cout << "Enter new appointment date [current: " << currentDate << "] (YYYY-MM-DD) (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, date);
        date = trim(date);

        if (date.empty()) return "";      // keep current
        if (date == "0") return "0";      // cancel

        if (!std::regex_match(date, date_pattern)) {
            std::cout << "Invalid format! Use YYYY-MM-DD.\n";
            continue;
        }

        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (!isValidDate(day, month, year, allowFuture)) {
            std::cout << "Invalid date.\n";
            continue;
        }

        if (!allowFuture && isFutureDate(year, month, day)) {
            std::cout << "Date cannot be in the future.\n";
            continue;
        }

        return date;
    }
}

std::string askForUpdatedAppointmentTime(const std::string& currentTime) {
    std::regex time_pattern(R"(^\d{2}:\d{2}$)");
    std::string time;

    while (true) {
        std::cout << "Enter new appointment time [current: " << currentTime << "] (HH:MM) (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, time);
        time = trim(time);

        if (time.empty()) return "";
        if (time == "0") return "0";

        if (!std::regex_match(time, time_pattern)) {
            std::cout << "Invalid format! Use HH:MM.\n";
            continue;
        }

        int hour = std::stoi(time.substr(0, 2));
        int minute = std::stoi(time.substr(3, 2));

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            std::cout << "Invalid time.\n";
            continue;
        }

        return time;
    }
}

std::string askForUpdatedAppointmentPurpose(const std::string& currentPurpose) {
    std::string purpose;

    while (true) {
        std::cout << "Enter new purpose [current: " << currentPurpose << "] (max 100 chars) (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, purpose);
        purpose = trim(purpose);

        if (purpose.empty()) return "";
        if (purpose == "0") return "0";

        if (purpose.length() > 100) {
            std::cout << "Purpose too long. Max 100 characters allowed.\n";
            continue;
        }

        return purpose;
    }
}

std::string askForUpdatedAppointmentStatus(const std::string& currentStatus) {
    std::string status;

    while (true) {
        std::cout << "Enter new status [current: " << currentStatus << "] (scheduled/completed/cancelled) (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, status);
        status = trim(status);

        if (status.empty()) return "";
        if (status == "0") return "0";

        if (status == "scheduled" || status == "completed" || status == "cancelled") {
            return status;
        } else {
            std::cout << "Invalid status. Must be: scheduled, completed, or cancelled.\n";
        }
    }
}


#include <regex>

std::string askForValidPassword(const std::string& prompt) {
    std::string password;
    std::regex strongPasswordRegex(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z\d]).{8,}$)");

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, password);
        password = trim(password);

        if (password.empty()) return ""; // keep current
        if (password == "0") return "0"; // cancel

        if (!std::regex_match(password, strongPasswordRegex)) {
            std::cout << "Password must:\n"
                      << "- Be at least 8 characters long\n"
                      << "- Include uppercase and lowercase letters\n"
                      << "- Include at least one number\n"
                      << "- Include at least one special character (!@#$%^&*, etc.)\n";
            continue;
        }

        return password;
    }
}


std::string askForUpdatedUsername(const std::string& current) {
    std::string input;
    std::regex usernameRegex(R"(^[a-zA-Z0-9_-]{3,20}$)");

    while (true) {
        std::cout << "Enter new username [current: " << current << "] (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty()) return ""; // keep current
        if (input == "0") return "0"; // cancel

        if (!std::regex_match(input, usernameRegex)) {
            std::cout << "Invalid username. Use only letters, digits, underscores, or hyphens (3–20 chars).\n";
            continue;
        }

        return input;
    }
}

std::string askForValidUsername(const std::string& prompt) {
    std::string input;
    std::regex usernameRegex(R"(^[a-zA-Z0-9_-]{3,20}$)");

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty() || input == "0") return input;  // allow cancel or return

        if (!std::regex_match(input, usernameRegex)) {
            std::cout << "Invalid username. Use only letters, digits, underscores (_), or hyphens (-), 3–20 characters long.\n";
            continue;
        }

        return input;
    }
}

std::string askForUpdatedRole(const std::string& currentRole) {
    std::string input;

    while (true) {
        std::cout << "Enter new role [current: " << currentRole << "] (Enter to keep, 0 to cancel): ";
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty()) return "";  // keep current
        if (input == "0") return "0";  // cancel

        if (input == "admin" || input == "staff" || input == "veterinarian")
            return input;

        std::cout << "Invalid role. Must be admin, staff, or veterinarian.\n";
    }
}

std::string capitalizeWords(const std::string& input) {
    std::string result = input;
    bool capitalizeNext = true;

    for (size_t i = 0; i < result.length(); ++i) {
        if (std::isspace(result[i])) {
            capitalizeNext = true;
        } else if (capitalizeNext) {
            result[i] = std::toupper(result[i]);
            capitalizeNext = false;
        } else {
            result[i] = std::tolower(result[i]);
        }
    }
    return result;
}


std::string askForUpdatedVaccinationStatus(const std::string& currentStatus) {
    std::string status;
    while (true) {
        std::cout << "Enter new vaccination status (completed / pending / booster required) (or press Enter to keep, 0 to cancel): ";
        std::getline(std::cin, status);
        status = trim(status);
        if (status.empty()) return "";      // keep current
        if (status == "0") return "0";      // cancel

        std::string lowerStatus = status;
        std::transform(lowerStatus.begin(), lowerStatus.end(), lowerStatus.begin(), ::tolower);
        if (lowerStatus == "completed" || lowerStatus == "pending" || lowerStatus == "booster required") {
            return lowerStatus;
        }

        std::cout << "Invalid status. Please enter either 'completed', 'pending', or 'booster required'.\n";
    }
}


bool isPhoneNumberTaken(const std::string& phone) {
    for (const auto& o : owners) {
        if (toLower(trim(o.getPhoneNumber())) == toLower(trim(phone))) {
            return true;
        }
    }
    return false;
}

bool isEmailTaken(const std::string& email) {
    for (const auto& o : owners) {
        if (toLower(trim(o.getEmail())) == toLower(trim(email))) {
            return true;
        }
    }
    return false;
}


std::string askForValidNameForUpdate(const std::string& prompt) {
    std::string name;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, name);
        name = trim(name);

        if (name == "0") {
            return "0";  // Explicitly return "0" for cancellation
        }
        if (name.empty()) {
            return "";  // Empty means keep current value
        }

        if (name.length() > 50) {
            std::cout << "Invalid name. Must not be longer than 50 characters.\n";
            continue;
        }

        bool valid = true;
        for (auto& c : name) {
            if (!isalpha(c) && c != '\'' && c != '-' && c != ' ') {
                valid = false;
                break;
            }
        }
        if (valid) {
            return name;
        } else {
            std::cout << "Invalid name. Only letters, spaces, apostrophes (') and hyphens (-) are allowed.\n";
        }
    }
}

int askForValidAgeForUpdate(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        line = trim(line);

        if (line == "0") {
            return -1;  // Use -1 to indicate cancellation, consistent with your original
        }
        if (line.empty()) {
            return -2;  // Use -2 to indicate "keep current value" (distinct from cancellation)
        }

        std::stringstream ss(line);
        int age;
        char extra;
        if (!(ss >> age) || (ss >> extra)) {
            std::cout << "Invalid input. Please enter a valid integer.\n";
            continue;
        }

        if (age < 0 || age > 50) {
            std::cout << "Invalid age. Please enter a number between 0 and 50.\n";
            continue;
        }

        return age;
    }
}

int askForOwnerIdOrOption(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty() || input == "0") {
            return 0;  // Cancellation
        }
        if (input == "n" || input == "N") {
            return -1;  // No owner
        }
        if (input == "c" || input == "C") {
            return -2;  // Create new owner
        }

        // Check if all digits
        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Invalid input! ID should be numeric only, or use 'c/C', 'n/N', or '0'. Try again.\n";
            continue;
        }

        // Check length (max 5 digits)
        if (input.length() > 5) {
            std::cout << "ID too long! Please enter an ID with up to 5 digits.\n";
            continue;
        }

        // Convert and return valid ID
        return std::stoi(input);
    }
}

int askForOwnerIdUpdate(const std::string& prompt, int currentId) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty()) {
            return currentId; // Keep current ID
        }
        if (input == "0") {
            return -1; // Cancel (distinct from any valid ID or current ID)
        }

        if (!std::all_of(input.begin(), input.end(), ::isdigit)) {
            std::cout << "Invalid input! ID should be numeric only. Try again.\n";
            continue;
        }

        if (input.length() > 5) {
            std::cout << "ID too long! Please enter an ID with up to 5 digits.\n";
            continue;
        }

        return std::stoi(input); // Valid new ID
    }
}

int askForMainMenuChoice(int minOption, int maxOption, const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        input = trim(input);

        if (input.empty() || input == "0") {
            return 0; // Logout for empty or "0"
        }

        if (!std::all_of(input.begin(), input.end(), ::isdigit)) { 
            std::cout << "Invalid input! Please enter a number between " << minOption << " and " << maxOption << ".\n";
            continue;
        }

        int choice;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input! Please enter a number between " << minOption << " and " << maxOption << ".\n";
            continue;
        }

        if (choice < minOption || choice > maxOption) {
            std::cout << "Invalid choice! Please select a number between " << minOption << " and " << maxOption << ".\n";
            continue;
        }

        return choice;
    }
}