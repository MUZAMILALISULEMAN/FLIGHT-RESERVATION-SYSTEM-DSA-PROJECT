#ifndef FLIGHTMANAGEMENT_H
#define FLIGHTMANAGEMENT_H

#include <iostream>
#include <string>
#include <iomanip>

class FlightManagement {
public:
    struct Flight {
        int flightId;
        std::string airline;
        std::string departureCity;
        std::string arrivalCity;
        double distance;
        double price;

        Flight() : flightId(0), distance(0.0), price(0.0) {}
        Flight(int id, const std::string& air, const std::string& dep, 
               const std::string& arr, double dist, double p)
            : flightId(id), airline(air), departureCity(dep), 
              arrivalCity(arr), distance(dist), price(p) {}
    };
    struct Node {
        Flight data;
        Node* next;
        Node(const Flight& f) : data(f), next(nullptr) {}
    };

private:
    

    Node* head;
    int nextFlightId;

    std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\n\r");
        size_t end = s.find_last_not_of(" \t\n\r");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }

    void pressEnterToContinue() {
        std::cout << "\nPress Enter to continue...";
        std::string tmp;
        std::getline(std::cin, tmp);
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    Node* findNodeById(int id, Node*& prevOut) {
        prevOut = nullptr;
        Node* cur = head;
        Node* prev = nullptr;
        while (cur != nullptr) {
            if (cur->data.flightId == id) {
                prevOut = prev;
                return cur;
            }
            prev = cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void printTableHeader() {
        std::cout << "+---------------------------------------------------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(6) << "ID"
                  << " | " << std::setw(12) << "Airline"
                  << " | " << std::setw(12) << "From"
                  << " | " << std::setw(12) << "To"
                  << " | " << std::setw(10) << "Distance"
                  << " | " << std::right << std::setw(10) << "Price"
                  << " |\n";
        std::cout << "+---------------------------------------------------------------------------------------------+\n";
    }
    private:
    // Add these helper functions
    bool stringToInt(const std::string& str, int& result) {
        try {
            result = std::stoi(str);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool stringToDouble(const std::string& str, double& result) {
        try {
            result = std::stod(str);
            return true;
        } catch (...) {
            return false;
        }
    }

    bool equalsIgnoreCase(const std::string& a, const std::string& b) {
        if (a.length() != b.length()) return false;
        for (size_t i = 0; i < a.length(); ++i) {
            if (std::tolower(a[i]) != std::tolower(b[i])) return false;
        }
        return true;
    }

public:
    FlightManagement() : head(nullptr), nextFlightId(1) {}

    ~FlightManagement() {
        Node* cur = head;
        while (cur != nullptr) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

    void addFlightDirect(const Flight& f) {
        Flight copy = f;
        if (copy.flightId >= nextFlightId) nextFlightId = copy.flightId + 1;

        Node* node = new Node(copy);
        if (head == nullptr) {
            head = node;
        } else {
            Node* it = head;
            while (it->next != nullptr) it = it->next;
            it->next = node;
        }
    }

    void addFlightInteractive() {
        std::cout << "\n--- Add New Flight ---\n";
        std::string airline, dep, arr;
        double dist = 0.0, price = 0.0;

        std::cout << "Airline: ";
        std::getline(std::cin, airline);
        airline = trim(airline);
        if (airline.empty()) airline = "Unknown";

        std::cout << "Departure city Code: ";
        std::getline(std::cin, dep);
        dep = trim(dep);

        std::cout << "Arrival city Code: ";
        std::getline(std::cin, arr);
        arr = trim(arr);

        std::cout << "Distance in Km: ";
        std::cin >> dist;
        std::cin.ignore();

        std::cout << "Price: ";
        std::cin >> price;
        std::cin.ignore();

        Flight f(nextFlightId++, airline, dep, arr, dist, price);

        Node* node = new Node(f);
        if (head == nullptr) head = node;
        else {
            Node* it = head;
            while (it->next != nullptr) it = it->next;
            it->next = node;
        }

        std::cout << "\nFlight added successfully.\n";
        pressEnterToContinue();
    }

    void displayAllFlights() {
        std::cout << "\n=== All Flights ===\n";

        if (head == nullptr) {
            std::cout << "No flights available.\n";
            pressEnterToContinue();
            return;
        }

        printTableHeader();
        Node* it = head;

        while (it != nullptr) {
            const Flight& f = it->data;
            std::cout << "| " << std::left << std::setw(6) << f.flightId
                      << " | " << std::setw(12) << f.airline
                      << " | " << std::setw(12) << f.departureCity
                      << " | " << std::setw(12) << f.arrivalCity
                      << " | " << std::setw(10) << std::fixed << std::setprecision(2) << f.distance
                      << " | " << std::right << std::setw(10) << std::fixed << std::setprecision(2) << f.price
                      << " |\n";
            it = it->next;
        }

        std::cout << "+---------------------------------------------------------------------------------------------+\n";
        pressEnterToContinue();
    }
    public:
    // Update Flight function
    void updateFlight() {
        if (head == nullptr) {
            std::cout << "No flights available to update.\n";
            pressEnterToContinue();
            return;
        }

        displayAllFlights();
        
        std::cout << "Enter Flight ID to update: ";
        std::string idStr;
        std::getline(std::cin, idStr);
        
        int flightId;
        if (!stringToInt(idStr, flightId)) {
            std::cout << "Invalid Flight ID.\n";
            pressEnterToContinue();
            return;
        }

        // Find the flight
        Node* prev = nullptr;
        Node* current = head;
        while (current != nullptr) {
            if (current->data.flightId == flightId) {
                break;
            }
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            std::cout << "Flight ID " << flightId << " not found.\n";
            pressEnterToContinue();
            return;
        }

        Flight& flight = current->data;

        std::cout << "\nUpdating Flight " << flightId << ":\n";
        std::cout << "1. Airline (" << flight.airline << ")\n";
        std::cout << "2. Departure City Code (" << flight.departureCity << ")\n";
        std::cout << "3. Arrival City Code (" << flight.arrivalCity << ")\n";
        std::cout << "4. Distance (" << flight.distance << ")\n";
        std::cout << "5. Price (" << flight.price << ")\n";
        std::cout << "0. Cancel Update\n";
        std::cout << "Choose field to update: ";

        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        int choice;
        if (!stringToInt(choiceStr, choice)) {
            std::cout << "Invalid choice.\n";
            pressEnterToContinue();
            return;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter new airline: ";
                std::getline(std::cin, flight.airline);
                flight.airline = trim(flight.airline);
                if (flight.airline.empty()) flight.airline = "Unknown";
                break;
            }
            case 2: {
                std::cout << "Enter new departure city code: ";
                std::getline(std::cin, flight.departureCity);
                flight.departureCity = trim(flight.departureCity);
                break;
            }
            case 3: {
                std::cout << "Enter new arrival city code: ";
                std::getline(std::cin, flight.arrivalCity);
                flight.arrivalCity = trim(flight.arrivalCity);
                break;
            }
            case 4: {
                std::cout << "Enter new distance: ";
                std::string distStr;
                std::getline(std::cin, distStr);
                double newDist;
                if (stringToDouble(distStr, newDist) && newDist >= 0) {
                    flight.distance = newDist;
                } else {
                    std::cout << "Invalid distance.\n";
                }
                break;
            }
            case 5: {
                std::cout << "Enter new price: ";
                std::string priceStr;
                std::getline(std::cin, priceStr);
                double newPrice;
                if (stringToDouble(priceStr, newPrice) && newPrice >= 0) {
                    flight.price = newPrice;
                } else {
                    std::cout << "Invalid price.\n";
                }
                break;
            }
            case 0: {
                std::cout << "Update cancelled.\n";
                pressEnterToContinue();
                return;
            }
            default: {
                std::cout << "Invalid choice.\n";
                pressEnterToContinue();
                return;
            }
        }

        std::cout << "Flight updated successfully!\n";
        pressEnterToContinue();
    }
    // Add to FlightManagement.h public section
    Node* getHead() const {
    return head;
}
    // Delete Flight function
    void deleteFlight() {
        if (head == nullptr) {
            std::cout << "No flights available to delete.\n";
            pressEnterToContinue();
            return;
        }

        displayAllFlights();
        
        std::cout << "Enter Flight ID to delete: ";
        std::string idStr;
        std::getline(std::cin, idStr);
        
        int flightId;
        if (!stringToInt(idStr, flightId)) {
            std::cout << "Invalid Flight ID.\n";
            pressEnterToContinue();
            return;
        }

        // Find the flight and its previous node
        Node* prev = nullptr;
        Node* current = head;
        
        while (current != nullptr && current->data.flightId != flightId) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            std::cout << "Flight ID " << flightId << " not found.\n";
            pressEnterToContinue();
            return;
        }

        // Confirm deletion
        std::cout << "Are you sure you want to delete Flight " << flightId 
                  << " (" << current->data.airline << " " 
                  << current->data.departureCity << " -> " 
                  << current->data.arrivalCity << ")? (y/n): ";
        
        std::string confirm;
        std::getline(std::cin, confirm);
        
        if (confirm == "y" || confirm == "Y") {
            // Remove the node
            if (prev == nullptr) {
                // Deleting head
                head = current->next;
            } else {
                prev->next = current->next;
            }
            
            delete current;
            std::cout << "Flight deleted successfully!\n";
        } else {
            std::cout << "Deletion cancelled.\n";
        }
        
        pressEnterToContinue();
    }

    // Search flights by departure city
    void searchByDeparture() {
        if (head == nullptr) {
            std::cout << "No flights available.\n";
            pressEnterToContinue();
            return;
        }

        std::cout << "Enter departure city code to search: ";
        std::string city;
        std::getline(std::cin, city);
        city = trim(city);

        std::cout << "\nFlights from " << city << ":\n";
        printTableHeader();

        Node* current = head;
        bool found = false;

        while (current != nullptr) {
            if (equalsIgnoreCase(current->data.departureCity, city)) {
                const Flight& f = current->data;
                std::cout << "| " << std::left << std::setw(6) << f.flightId
                          << " | " << std::setw(12) << f.airline
                          << " | " << std::setw(12) << f.departureCity
                          << " | " << std::setw(12) << f.arrivalCity
                          << " | " << std::setw(10) << std::fixed << std::setprecision(2) << f.distance
                          << " | " << std::right << std::setw(10) << std::fixed << std::setprecision(2) << f.price
                          << " |\n";
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            std::cout << "| No flights found from " << city << " |\n";
        }
        
        std::cout << "+---------------------------------------------------------------------------------------------+\n";
        pressEnterToContinue();
    }

    void runFlightManagement() {
    while (true) {
        clearScreen();
        std::cout << "=============================================================\n";
        std::cout << "              FLIGHT MANAGEMENT SYSTEM (Linked List)        \n";
        std::cout << "=============================================================\n";
        std::cout << " [1] Add New Flight\n";
        std::cout << " [2] Display All Flights\n";
        std::cout << " [3] Update Flight Details\n";
        std::cout << " [4] Delete a Flight\n";
        std::cout << " [5] Search by Departure City\n";
        std::cout << " [0] Back to Main Menu\n";
        std::cout << "-------------------------------------------------------------\n";
        std::cout << " Enter your option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: addFlightInteractive(); break;
            case 2: displayAllFlights(); break;
            case 3: updateFlight(); break;
            case 4: deleteFlight(); break;
            case 5: searchByDeparture(); break;
            case 0: return;
            default: std::cout << "Invalid option.\n"; pressEnterToContinue();
        }
    }
}
    



};

#endif