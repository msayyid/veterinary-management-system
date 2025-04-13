# Makefile for Veterinary Management System
CXX = clang++
CXXFLAGS = -std=c++17 -Wall
OPENSSL_FLAGS = -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto
SRC = main.cpp menu.cpp Owner.cpp Pet.cpp Appointment.cpp User.cpp validations.cpp globals.cpp \
      pet_menu_helpers.cpp owner_menu_helpers.cpp appointment_menu_helpers.cpp user_menu_helpers.cpp \
      hashing.cpp
TARGET = vet_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(OPENSSL_FLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)