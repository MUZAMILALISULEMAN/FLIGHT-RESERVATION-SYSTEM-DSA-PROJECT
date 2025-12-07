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
    std::cout << "\n";
    std::cout << "     FLIGHT RESERVATION SYSTEM          \n";
    std::cout << "        DSA PROJECT                        \n";
    std::cout << "\n";
    std::cout << "   [1] Flight Management (Linked List)\n";
    std::cout << "   [2] Airport Management (Hash Table)\n";
    std::cout << "   [3] Route Optimization (Graph + Dijkstra)\n";
    std::cout << "   [0] Exit\n";
    std::cout << "\n";
    std::cout << "   Enter your choice: ";
}

int main() {
    FlightManagement flightMgmt;
    Airport airportMgmt;
    Graph routeOptimizer;

    Airport airports[20];  // Initialize all airports with default constructor
    
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
                airportMgmt.runAirportManagement(airports);
                break;
            case 3:
                // Pass both flight management and airports array to route optimizer
                routeOptimizer.runRouteOptimization(flightMgmt, airports);
                break;
            case 0:
                clearScreen();
                std::cout << "\n\n";
                std::cout << "          MUZAMIL ALI 24K-1023            \n";
                std::cout << "         MUZAMMIL ZAIDI 24K-0887          \n";
                std::cout << "         GHULAM MUJTABA 24K-XXXX          \n"; 
                std::cout << "           THANK YOU! BA BYE!            \n";
                std::cout << "\n\n";
                return 0;
            default:
                std::cout << "Invalid choice! Press Enter to continue...";
                std::cin.get();
        }
    }

    return 0;
}