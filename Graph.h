#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>

// Forward declarations to avoid circular includes
class FlightManagement;
class Airport;

struct GraphEdge {
    std::string to;
    std::string flightName;
    int cost;
    GraphEdge* next;
    
    GraphEdge(const std::string& t, const std::string& name, int c) 
        : to(t), flightName(name), cost(c), next(nullptr) {}
};

struct GraphNode {
    std::string airport;
    GraphEdge* edges;
    GraphNode* next;
    
    GraphNode(const std::string& air) : airport(air), edges(nullptr), next(nullptr) {}
};

class Graph {
private:
    GraphNode* head;
    const int INF = 1000000000;

    GraphNode* findOrCreateNode(const std::string& airport) {
        GraphNode* current = head;
        while (current != nullptr) {
            if (current->airport == airport) return current;
            current = current->next;
        }
        
        GraphNode* newNode = new GraphNode(airport);
        newNode->next = head;
        head = newNode;
        return newNode;
    }

    GraphNode* findNode(const std::string& airport) {
        GraphNode* current = head;
        while (current != nullptr) {
            if (current->airport == airport) return current;
            current = current->next;
        }
        return nullptr;
    }

    void addEdgeToNode(GraphNode* node, const std::string& to, const std::string& flightName, int cost) {
        GraphEdge* newEdge = new GraphEdge(to, flightName, cost);
        newEdge->next = node->edges;
        node->edges = newEdge;
    }

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

public:
    Graph() : head(nullptr) {}

    ~Graph() {
        GraphNode* current = head;
        while (current != nullptr) {
            GraphEdge* edge = current->edges;
            while (edge != nullptr) {
                GraphEdge* nextEdge = edge->next;
                delete edge;
                edge = nextEdge;
            }
            GraphNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // NEW: Populate graph from FlightManagement data
    void populateFromFlightManagement(const FlightManagement& flightMgmt);

    // NEW: Populate graph from both FlightManagement and Airport data
    void populateFromBothSystems(const FlightManagement& flightMgmt, const Airport& airportMgmt);

    // Get all airport codes from the graph (for validation)
    void getAllAirportCodes(std::string airports[], int& count) {
        count = 0;
        GraphNode* current = head;
        while (current != nullptr && count < 50) {
            airports[count++] = current->airport;
            current = current->next;
        }
    }

    // Manual flight addition (keep for testing)
    void addFlight(const std::string& from, const std::string& to, const std::string& flightName, int cost) {
        GraphNode* fromNode = findOrCreateNode(from);
        addEdgeToNode(fromNode, to, flightName, cost);
        findOrCreateNode(to);
    }

    void findShortestPath(const std::string& start, const std::string& end) {
        if (findNode(start) == nullptr || findNode(end) == nullptr) {
            std::cout << "Error: Airport not found in graph!\n";
            return;
        }

        int nodeCount = 0;
        GraphNode* temp = head;
        while (temp != nullptr) {
            nodeCount++;
            temp = temp->next;
        }

        std::string* airports = new std::string[nodeCount];
        int* distance = new int[nodeCount];
        std::string* previous = new std::string[nodeCount];
        bool* visited = new bool[nodeCount];

        temp = head;
        for (int i = 0; i < nodeCount; i++) {
            airports[i] = temp->airport;
            distance[i] = INF;
            previous[i] = "";
            visited[i] = false;
            temp = temp->next;
        }

        int startIndex = -1;
        for (int i = 0; i < nodeCount; i++) {
            if (airports[i] == start) {
                startIndex = i;
                break;
            }
        }

        if (startIndex == -1) {
            std::cout << "Start airport not found!\n";
            delete[] airports; delete[] distance; delete[] previous; delete[] visited;
            return;
        }

        distance[startIndex] = 0;

        for (int count = 0; count < nodeCount - 1; count++) {
            int minDistance = INF;
            int minIndex = -1;

            for (int i = 0; i < nodeCount; i++) {
                if (!visited[i] && distance[i] < minDistance) {
                    minDistance = distance[i];
                    minIndex = i;
                }
            }

            if (minIndex == -1) break;

            visited[minIndex] = true;

            GraphNode* currentNode = findNode(airports[minIndex]);
            GraphEdge* edge = currentNode->edges;
            
            while (edge != nullptr) {
                int neighborIndex = -1;
                for (int i = 0; i < nodeCount; i++) {
                    if (airports[i] == edge->to) {
                        neighborIndex = i;
                        break;
                    }
                }

                if (neighborIndex != -1 && !visited[neighborIndex]) {
                    int newDist = distance[minIndex] + edge->cost;
                    if (newDist < distance[neighborIndex]) {
                        distance[neighborIndex] = newDist;
                        previous[neighborIndex] = airports[minIndex];
                    }
                }
                edge = edge->next;
            }
        }

        int endIndex = -1;
        for (int i = 0; i < nodeCount; i++) {
            if (airports[i] == end) {
                endIndex = i;
                break;
            }
        }

        if (endIndex == -1 || distance[endIndex] == INF) {
            std::cout << "No path exists between " << start << " and " << end << "!\n";
        } else {
            std::cout << "\n Shortest Path Found!\n";
            std::cout << "Route: ";
            
            std::string path[20];
            int pathSize = 0;
            std::string current = end;
            
            while (current != start) {
                path[pathSize++] = current;
                for (int i = 0; i < nodeCount; i++) {
                    if (airports[i] == current) {
                        current = previous[i];
                        break;
                    }
                }
            }
            path[pathSize++] = start;

            for (int i = pathSize - 1; i >= 0; i--) {
                std::cout << path[i];
                if (i > 0) std::cout << " → ";
            }
            
            std::cout << "\nTotal Cost: $" << distance[endIndex] << std::endl;
        }

        delete[] airports; delete[] distance; delete[] previous; delete[] visited;
    }

    void displayGraph() {
        if (head == nullptr) {
            std::cout << "Graph is empty! No flights available.\n";
            return;
        }

        std::cout << "\n=== Flight Network Graph ===\n";
        GraphNode* current = head;
        int flightCount = 0;
        
        while (current != nullptr) {
            std::cout << current->airport << " -> ";
            GraphEdge* edge = current->edges;
            
            if (edge == nullptr) {
                std::cout << "No connections";
            } else {
                while (edge != nullptr) {
                    std::cout << edge->to << "(" << edge->flightName << ",$" << edge->cost << ") ";
                    edge = edge->next;
                    flightCount++;
                }
            }
            std::cout << std::endl;
            current = current->next;
        }
        std::cout << "Total flights in graph: " << flightCount << std::endl;
    }

    void runRouteOptimization() {
        clearScreen();
        std::cout << "=========================================\n";
        std::cout << "      ROUTE OPTIMIZATION (Graph)        \n";
        std::cout << "=========================================\n";

        while (true) {
            std::cout << " [1] Find Shortest Path\n";
            std::cout << " [2] Display Flight Network\n";
            std::cout << " [3] Show Available Airports\n";
            std::cout << " [0] Back to Main Menu\n";
            std::cout << "-----------------------------------------\n";
            std::cout << " Enter your option: ";

            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    std::string start, end;
                    std::cout << "Enter start airport: ";
                    std::getline(std::cin, start);
                    std::cout << "Enter destination airport: ";
                    std::getline(std::cin, end);
                    findShortestPath(start, end);
                    pressEnterToContinue();
                    break;
                }
                case 2: {
                    displayGraph();
                    pressEnterToContinue();
                    break;
                }
                case 3: {
                    std::string airports[50];
                    int count = 0;
                    getAllAirportCodes(airports, count);
                    
                    std::cout << "\nAvailable Airports (" << count << "): ";
                    for (int i = 0; i < count; i++) {
                        std::cout << airports[i];
                        if (i < count - 1) std::cout << ", ";
                    }
                    std::cout << std::endl;
                    pressEnterToContinue();
                    break;
                }
                case 0: return;
                default: std::cout << "Invalid option!\n"; pressEnterToContinue();
            }
            clearScreen();
            std::cout << "=========================================\n";
            std::cout << "      ROUTE OPTIMIZATION (Graph)        \n";
            std::cout << "=========================================\n";
        }
    }
};

#endif