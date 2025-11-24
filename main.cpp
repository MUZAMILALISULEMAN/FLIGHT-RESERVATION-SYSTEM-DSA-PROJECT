#include <iostream>
#include "FlightManagement.h"
#include "Airport.h"
#include "Graph.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMainMenu() {
    clearScreen();
    std::cout << "=========================================\n";
    std::cout << "     FLIGHT RESERVATION SYSTEM\n";
    std::cout << "        DSA PROJECT - MUZZY\n";
    std::cout << "=========================================\n";
    std::cout << " [1] Flight Management (Linked List)\n";
    std::cout << " [2] Airport Management (Hash Table)\n";
    std::cout << " [3] Route Optimization (Graph)\n";
    std::cout << " [0] Exit\n";
    std::cout << "-----------------------------------------\n";
    std::cout << " Enter your choice: ";
}

int main() {
    FlightManagement flightMgmt;
    Airport airportMgmt;
    Graph routeOptimizer;

    while (true) {
        displayMainMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                flightMgmt.runFlightManagement();
                break;
            case 2:
                airportMgmt.runAirportManagement();
                break;
            case 3:
                routeOptimizer.runRouteOptimization();
                break;
            case 0:
                std::cout << "\nThank you! Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice!\n";
        }
    }

    return 0;
}