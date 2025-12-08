#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <limits>
#include <algorithm>
#include "flightmanagement.h"
#include "airport.h"



class Graph {
private:
    static const int MAX_AIRPORTS = 20;
    static const int INF = 999999;
    
    // Adjacency matrix: matrix[i][j] = distance between airport i and j
    int matrix[MAX_AIRPORTS][MAX_AIRPORTS];
    
    // Airport names/codes from FlightManagement data
    std::string airportNames[MAX_AIRPORTS];
    int airportCount;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void pressEnterToContinue() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

  
    
    // Find airport index by code
    int findAirportIndex(const std::string& code) {
        for (int i = 0; i < airportCount; i++) {
            if (airportNames[i] == code) {
                return i;
            }
        }
        return -1;
    }
    
    // Add airport if not exists
    int addAirport(const std::string& code) {
        int idx = findAirportIndex(code);
        if (idx != -1) return idx;
        
        if (airportCount >= MAX_AIRPORTS) {
            std::cout << "Maximum airports reached!\n";
            return -1;
        }
        
        airportNames[airportCount] = code;
        airportCount++;
        return airportCount - 1;
    }

public:
    Graph() : airportCount(0) {
        // Initialize matrix with INF (no connection)
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            for (int j = 0; j < MAX_AIRPORTS; j++) {
                matrix[i][j] = INF;
            }
            matrix[i][i] = 0; // Distance to itself is 0
        }
    }
    
    // Populate graph ONLY from FlightManagement system
    void populateFromFlights(const FlightManagement& flightMgmt) {
        FlightManagement::Node* flightNode = flightMgmt.getHead();
        int addedCount = 0;
        
        // Clear existing data
        airportCount = 0;
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            for (int j = 0; j < MAX_AIRPORTS; j++) {
                matrix[i][j] = INF;
            }
            matrix[i][i] = 0;
        }
        
        if (flightNode == nullptr) {
            std::cout << "No flights found in FlightManagement system!\n";
            return;
        }
        
        while (flightNode != nullptr) {
            const FlightManagement::Flight& flight = flightNode->data;
            
            // Get or add airport indices
            int fromIdx = addAirport(flight.departureCity);
            int toIdx = addAirport(flight.arrivalCity);
            
            if (fromIdx != -1 && toIdx != -1) {
                int distance = static_cast<int>(flight.distance);
                
                // Add bidirectional connection
                matrix[fromIdx][toIdx] = distance;
                matrix[toIdx][fromIdx] = distance;
                addedCount++;
            }
            
            flightNode = flightNode->next;
        }
        
        std::cout << "\nGraph populated successfully!\n";
        std::cout << "   Flights loaded: " << addedCount << "\n";
        std::cout << "   Airports added: " << airportCount << "\n";
    }
    
    // Populate from both systems (FlightManagement + Airport validation)
    void populateFromBothSystems(const FlightManagement& flightMgmt, Airport airports[]) {
        // Clear existing data
        airportCount = 0;
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            for (int j = 0; j < MAX_AIRPORTS; j++) {
                matrix[i][j] = INF;
            }
            matrix[i][i] = 0;
        }
        
        // First, add all valid airports from Airport system
        for (int i = 0; i < 20; i++) {
            if (airports[i].status == 1) { // Active airport
                addAirport(airports[i].code);
            }
        }
        
        // Then add flights from FlightManagement
        FlightManagement::Node* flightNode = flightMgmt.getHead();
        int addedCount = 0;
        int skippedCount = 0;
        
        if (flightNode == nullptr) {
            std::cout << "No flights found in FlightManagement system!\n";
            return;
        }
        
        while (flightNode != nullptr) {
            const FlightManagement::Flight& flight = flightNode->data;
            
            int fromIdx = findAirportIndex(flight.departureCity);

            int toIdx = findAirportIndex(flight.arrivalCity);
            
            if (fromIdx != -1 && toIdx != -1) {
                int distance = static_cast<int>(flight.distance);
                matrix[fromIdx][toIdx] = distance;
                addedCount++;
            } else {
                std::cout<<fromIdx<<" ";
                skippedCount++;
            }
            
            flightNode = flightNode->next;
        }
        
        std::cout << "\nGraph populated from both systems!\n";
        std::cout << "   Valid flight connections: " << addedCount << "\n";
        std::cout << "   Skipped flights (invalid airports): " << skippedCount << "\n";
        std::cout << "   Total airports in graph: " << airportCount << "\n";
    }
    
    // Dijkstra's algorithm for shortest path
    void findShortestPath(const std::string& start, const std::string& end) {
        if (airportCount == 0) {
            std::cout << "Graph is empty! Please load flights first.\n";
            return;
        }
        
        int startIdx = findAirportIndex(start);
        int endIdx = findAirportIndex(end);
        
        if (startIdx == -1) {
            std::cout << "Airport '" << start << "' not found in graph!\n";
            return;
        }
        
        if (endIdx == -1) {
            std::cout << "Airport " << end << "' not found in graph!\n";
            return;
        }
        
        if (startIdx == endIdx) {
            std::cout << "Start and end airports are the same!\n";
            return;
        }
        
        // Dijkstra's algorithm
        int dist[MAX_AIRPORTS];
        bool visited[MAX_AIRPORTS];
        int prev[MAX_AIRPORTS];
        
        for (int i = 0; i < airportCount; i++) {
            dist[i] = INF;
            visited[i] = false;
            prev[i] = -1;
        }
        
        dist[startIdx] = 0;
        
        for (int count = 0; count < airportCount - 1; count++) {
            // Find unvisited vertex with minimum distance
            int minDist = INF;
            int minIdx = -1;
            
            for (int i = 0; i < airportCount; i++) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    minIdx = i;
                }
            }
            
            if (minIdx == -1 || minIdx == endIdx) break;
            
            visited[minIdx] = true;
            
            // Update distances of adjacent vertices
            for (int i = 0; i < airportCount; i++) {
                if (!visited[i] && matrix[minIdx][i] != INF) {
                    int newDist = dist[minIdx] + matrix[minIdx][i];
                    if (newDist < dist[i]) {
                        dist[i] = newDist;
                        prev[i] = minIdx;
                    }
                }
            }
        }
        
        // Check if path exists
        if (dist[endIdx] == INF) {
            std::cout << "No flight path exists between " << start << " and " << end << "!\n";
            return;
        }
        
        // Reconstruct path
        std::vector<std::string> path;
        int current = endIdx;
        
        while (current != -1) {
            path.push_back(airportNames[current]);
            current = prev[current];
        }
        
        reverse(path.begin(), path.end());
        
        // Display result
        std::cout << "\n\n";
        std::cout << "         SHORTEST PATH FOUND            \n";
        std::cout << "\n";
        std::cout << "   From: " << start << "\n";
        std::cout << "     To: " << end << "\n";
        std::cout << "\n";
        std::cout << "   Route: ";
        
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << " -> ";
        }
        
        std::cout << "\n\n   Total Distance: " << dist[endIdx] << " km\n";
        std::cout << "   Total Stops: " << path.size() - 2 << "\n";
        std::cout << "\n";
    }
    
    // Display flight connections
    void displayConnections() {
        if (airportCount == 0) {
            std::cout << "Graph is empty! No flights loaded.\n";
            return;
        }
        
        std::cout << "\n\n";
        std::cout << "        FLIGHT CONNECTIONS              \n";
        std::cout << "\n";
        
        int totalConnections = 0;
        
        for (int i = 0; i < airportCount; i++) {
            std::cout << "\n   " << airportNames[i] << ":\n";
            bool hasConnections = false;
            
            for (int j = 0; j < airportCount; j++) {
                if (i != j && matrix[i][j] != INF) {
                    std::cout << "     ->" << airportNames[j] 
                              << " (" << matrix[i][j] << " km)\n";
                    hasConnections = true;
                    totalConnections++;
                }
            }
            
            if (!hasConnections) {
                std::cout << "     No direct flights\n";
            }
        }
        
        std::cout << "\n\n";
        std::cout << "   Total Airports: " << airportCount << "\n";
        std::cout << "   Total Connections: " << totalConnections/2 << "\n";
    }
    
    // Display adjacency matrix
    void displayMatrix() {
        if (airportCount == 0) {
            std::cout << "Graph is empty! No flights loaded.\n";
            return;
        }
        
        std::cout << "\n\n";
        std::cout << "        ADJACENCY MATRIX                \n";
        std::cout << "\n\n";
        
        // Print header
        std::cout << std::setw(10) << " ";
        for (int i = 0; i < airportCount; i++) {
            std::cout << std::setw(6) << airportNames[i];
        }
        std::cout << "\n";
        
        // Print matrix
        for (int i = 0; i < airportCount; i++) {
            std::cout << std::setw(10) << airportNames[i];
            for (int j = 0; j < airportCount; j++) {
                if (matrix[i][j] == INF) {
                    std::cout << std::setw(6) << "INF";
                } else if (matrix[i][j] == 0) {
                    std::cout << std::setw(6) << "0";
                } else {
                    std::cout << std::setw(6) << matrix[i][j];
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "\nLegend: INF = No direct connection\n";
        std::cout << "        Numbers = Distance in km\n";
    }
    
    // Show all airports
    void showAirports() {
        if (airportCount == 0) {
            std::cout << "Graph is empty! No flights loaded.\n";
            return;
        }
        
        std::cout << "\n\n";
        std::cout << "        AVAILABLE AIRPORTS              \n";
        std::cout << "\n\n";
        
        for (int i = 0; i < airportCount; i++) {
            std::cout << "   [" << i + 1 << "] " << airportNames[i] << "\n";
        }
        
        std::cout << "\n\n";
        std::cout << "   Total: " << airportCount << " airports\n";
    }
    
    // Clear graph
    void clearGraph() {
        airportCount = 0;
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            for (int j = 0; j < MAX_AIRPORTS; j++) {
                matrix[i][j] = INF;
            }
        }
        std::cout << "Graph cleared successfully!\n";
    }
    
    // Main menu for route optimization
    void runRouteOptimization(FlightManagement& flightMgmt, Airport airports[]) {
        clearScreen();
        
        while (true) {
            std::cout << "\n";
            std::cout << "     ROUTE OPTIMIZATION SYSTEM          \n";
            std::cout << "      (Adjacency Matrix + Dijkstra)     \n";
            std::cout << "\n";
            std::cout << "   [1] Load Data\n";
            std::cout << "   [2] Find Shortest Path (Dijkstra)\n";
            std::cout << "   [3] Display Flight Connections\n";
            std::cout << "   [4] Display Adjacency Matrix\n";
            std::cout << "   [5] Show All Airports\n";
            std::cout << "   [6] Clear Graph Data\n";
            std::cout << "   [0] Back to Main Menu\n";
            std::cout << "\n";
            std::cout << "   Current: " << airportCount << " airports in graph\n";
            std::cout << "   Enter choice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();
            
            switch (choice) {
                    
                case 1:
                    populateFromBothSystems(flightMgmt, airports);
                    pressEnterToContinue();
                    break;
                    
                case 2: {
                    std::string start, end;
                    std::cout << "Enter start airport code: ";
                    std::getline(std::cin, start);
                    std::cout << "Enter destination airport code: ";
                    std::getline(std::cin, end);
                    findShortestPath(start, end);
                    pressEnterToContinue();
                    break;
                }
                    
                case 3:
                    displayConnections();
                    pressEnterToContinue();
                    break;
                    
                case 4:
                    displayMatrix();
                    pressEnterToContinue();
                    break;
                    
                case 5:
                    showAirports();
                    pressEnterToContinue();
                    break;
                    
                case 6:
                    clearGraph();
                    pressEnterToContinue();
                    break;
                    
                case 0:
                    return;
                    
                default:
                    std::cout << "Invalid choice!\n";
                    pressEnterToContinue();
            }
            
            clearScreen();
        }
    }
};

#endif