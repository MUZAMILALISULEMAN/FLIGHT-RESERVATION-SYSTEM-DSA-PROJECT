#ifndef AIRPORT_H
#define AIRPORT_H

#include <iostream>
#include <cstring>

class Airport {
private:
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
public:
    char name[50];
    char code[20];
    int status;

    Airport() {
        name[0] = '\0';
        code[0] = '\0';
        status = -1;
    }

    int hashFunc(const char code[]) {
        int sum = 0;
        for (int i = 0; code[i] != '\0'; i++) {
            sum += code[i];
        }
        return sum % 20;
    }

    void createAirport(Airport airports[], const char name[], const char code[]) {
        clearScreen();
        int index = hashFunc(code);
        int original = index;

        while (airports[index].status == 1) {
            index = (index + 1) % 20;
            if (index == original) {
                std::cout << "Hash table full.\n";
                return;
            }
        }

        strcpy(airports[index].name, name);
        strcpy(airports[index].code, code);
        airports[index].status = 1;

        std::cout << "Airport created.\n";
        pressEnterToContinue();
    }

    void searchAirport(Airport airports[], const char key[]) {
        clearScreen();
        std::cout << "Search results:\n";
        for (int i = 0; i < 20; i++) {
            if (airports[i].status != 1) continue;

            if (strstr(airports[i].name, key) != nullptr || strstr(airports[i].code, key) != nullptr) {
                std::cout << airports[i].name << " - " << airports[i].code << std::endl;
            }
        }
        pressEnterToContinue();
    }

    public:
    // Display all airports function
    void displayAllAirports(Airport airports[]) {
        clearScreen();
        std::cout << "\n=== ALL AIRPORTS ===\n";
        std::cout << "+---------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(30) << "Airport Name" 
                  << " | " << std::setw(15) << "Code" 
                  << " | " << std::setw(8) << "Status" << " |\n";
        std::cout << "+---------------------------------------------------+\n";
        
        bool found = false;
        for (int i = 0; i < 20; i++) {
            if (airports[i].status == 1) { // Only show active airports
                std::cout << "| " << std::setw(30) << airports[i].name 
                          << " | " << std::setw(15) << airports[i].code 
                          << " | " << std::setw(8) << "Active" << " |\n";
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "|" << std::setw(47) << "No airports found" << " |\n";
        }
        std::cout << "+---------------------------------------------------+\n";

        pressEnterToContinue();
        
    }

    // Update airport data function
    void updateAirportData(Airport airports[]) {
        clearScreen();
        char oldCode[20];
        std::cout << "Enter airport code to update: ";
        std::cin.getline(oldCode, 20);
        
        // Find the airport
        int index = -1;
        for (int i = 0; i < 20; i++) {
            if (airports[i].status == 1 && strcmp(airports[i].code, oldCode) == 0) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            std::cout << "Airport with code '" << oldCode << "' not found!\n";
            return;
        }
        
        std::cout << "\nCurrent Airport Data:\n";
        std::cout << "Name: " << airports[index].name << "\n";
        std::cout << "Code: " << airports[index].code << "\n";
        
        std::cout << "\nWhat do you want to update?\n";
        std::cout << "1. Airport Name\n";
        std::cout << "2. Airport Code\n";
        std::cout << "3. Both Name and Code\n";
        std::cout << "0. Cancel\n";
        std::cout << "Enter choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: {
                char newName[50];
                std::cout << "Enter new airport name: ";
                std::cin.getline(newName, 50);
                strcpy(airports[index].name, newName);
                std::cout << "Airport name updated successfully!\n";
                break;
            }
            case 2: {
                char newCode[20];
                std::cout << "Enter new airport code: ";
                std::cin.getline(newCode, 20);
                
                // Check if new code already exists
                bool codeExists = false;
                for (int i = 0; i < 20; i++) {
                    if (i != index && airports[i].status == 1 && strcmp(airports[i].code, newCode) == 0) {
                        codeExists = true;
                        break;
                    }
                }
                
                if (codeExists) {
                    std::cout << "Error: Airport code '" << newCode << "' already exists!\n";
                } else {
                    strcpy(airports[index].code, newCode);
                    std::cout << "Airport code updated successfully!\n";
                }
                break;
            }
            case 3: {
                char newName[50], newCode[20];
                std::cout << "Enter new airport name: ";
                std::cin.getline(newName, 50);
                std::cout << "Enter new airport code: ";
                std::cin.getline(newCode, 20);
                
                // Check if new code already exists
                bool codeExists = false;
                for (int i = 0; i < 20; i++) {
                    if (i != index && airports[i].status == 1 && strcmp(airports[i].code, newCode) == 0) {
                        codeExists = true;
                        break;
                    }
                }
                
                if (codeExists) {
                    std::cout << "Error: Airport code '" << newCode << "' already exists!\n";
                } else {
                    strcpy(airports[index].name, newName);
                    strcpy(airports[index].code, newCode);
                    std::cout << "Airport data updated successfully!\n";
                }
                break;
            }
            case 0:
                std::cout << "Update cancelled.\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
        pressEnterToContinue();
    }

    // Delete airport with confirmation
    void deleteAirportWithConfirm(Airport airports[]) {
        clearScreen();
        char code[20];
        std::cout << "Enter airport code to delete: ";
        std::cin.getline(code, 20);
        
        int index = -1;
        for (int i = 0; i < 20; i++) {
            if (airports[i].status == 1 && strcmp(airports[i].code, code) == 0) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            std::cout << "Airport with code '" << code << "' not found!\n";
            return;
        }
        
        std::cout << "\nAirport to delete:\n";
        std::cout << "Name: " << airports[index].name << "\n";
        std::cout << "Code: " << airports[index].code << "\n";
        
        std::cout << "\nAre you sure you want to delete this airport? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();
        
        if (confirm == 'y' || confirm == 'Y') {
            airports[index].status = -2; // Mark as deleted
            airports[index].name[0] = '\0';
            airports[index].code[0] = '\0';
            std::cout << "Airport deleted successfully!\n";
        } else {
            std::cout << "Deletion cancelled.\n";
        }
        pressEnterToContinue();
    }

    // Enhanced search with better formatting
    void searchAirportEnhanced(Airport airports[], const char key[]) {
        clearScreen();
        std::cout << "\n=== SEARCH RESULTS ===\n";
        std::cout << "+---------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(30) << "Airport Name" 
                  << " | " << std::setw(15) << "Code" << " |\n";
        std::cout << "+---------------------------------------------------+\n";
        
        bool found = false;
        for (int i = 0; i < 20; i++) {
            if (airports[i].status == 1) {
                // Search in name
                if (strstr(airports[i].name, key) != nullptr) {
                    std::cout << "| " << std::setw(30) << airports[i].name 
                              << " | " << std::setw(15) << airports[i].code << " |\n";
                    found = true;
                }
                // Search in code
                else if (strstr(airports[i].code, key) != nullptr) {
                    std::cout << "| " << std::setw(30) << airports[i].name 
                              << " | " << std::setw(15) << airports[i].code << " |\n";
                    found = true;
                }
            }
        }
        
        if (!found) {
            std::cout << "|" << std::setw(47) << "No airports found" << " |\n";
        }
        std::cout << "+---------------------------------------------------+\n";
        pressEnterToContinue();
    }


    // Enhanced runAirportManagement function
    void runAirportManagement() {
        clearScreen();
        Airport airports[20];
        Airport obj;

        

        while (true) {
            std::cout << "\n=========================================\n";
            std::cout << "       AIRPORT MANAGEMENT (Hash Table)   \n";
            std::cout << "=========================================\n";
            std::cout << " [1] Create Airport\n";
            std::cout << " [2] Update Airport Data\n";
            std::cout << " [3] Delete Airport\n";
            std::cout << " [4] Search Airport\n";
            std::cout << " [5] Display All Airports\n";
            std::cout << " [6] Display Hash Table Structure\n";
            std::cout << " [0] Back to Main Menu\n";
            std::cout << "-----------------------------------------\n";
            std::cout << " Enter your option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    char name[50], code[20];
                    std::cout << "Enter airport name: ";
                    std::cin.getline(name, 50);
                    std::cout << "Enter airport code: ";
                    std::cin.getline(code, 20);
                    obj.createAirport(airports, name, code);
                    break;
                }
                case 2: {
                    obj.updateAirportData(airports);
                    break;
                }
                case 3: {
                    obj.deleteAirportWithConfirm(airports);
                    break;
                }
                case 4: {
                    char key[50];
                    std::cout << "Enter search key (name or code): ";
                    std::cin.getline(key, 50);
                    obj.searchAirportEnhanced(airports, key);
                    break;
                }
                case 5: {
                    obj.displayAllAirports(airports);
                    break;
                }
                case 6: {
                    obj.displayHashTableStructure(airports);
                    break;
                }
                case 0: return;
                default: std::cout << "Invalid option.\n";
            }
        }
    }

    // Bonus: Display hash table structure for educational purposes
    void displayHashTableStructure(Airport airports[]) {
        std::cout << "\n=== HASH TABLE STRUCTURE ===\n";
        std::cout << "+-----+---------------------------------------------------+\n";
        std::cout << "| " << std::left << std::setw(3) << "Idx" 
                  << " | " << std::setw(30) << "Airport Data" 
                  << " | " << std::setw(8) << "Status" << " |\n";
        std::cout << "+-----+---------------------------------------------------+\n";
        
        for (int i = 0; i < 20; i++) {
            std::cout << "| " << std::setw(3) << i << " | ";
            
            if (airports[i].status == 1) {
                std::string airportInfo = std::string(airports[i].name) + " (" + airports[i].code + ")";
                std::cout << std::setw(30) << airportInfo << " | " << std::setw(8) << "Active" << " |";
            } else if (airports[i].status == -2) {
                std::cout << std::setw(30) << "DELETED" << " | " << std::setw(8) << "Tombstone" << " |";
            } else {
                std::cout << std::setw(30) << "EMPTY" << " | " << std::setw(8) << "Empty" << " |";
            }
            std::cout << "\n";
        }
        std::cout << "+-----+---------------------------------------------------+\n";
        
        std::cout << "\nHash Function: sum(ASCII of code) % 20\n";
        std::cout << "Collision Resolution: Linear Probing\n";
    }
};

#endif