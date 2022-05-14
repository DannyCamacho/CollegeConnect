#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QMainWindow>
#include "database.h"
#include <queue>
#include <algorithm>
#include <map>

namespace Ui { class GraphViewer; }
class GraphViewer : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphViewer(QWidget *parent = nullptr);
    ~GraphViewer();

private slots:

private:
    Ui::GraphViewer *ui;
    QSqlQueryModel* discoverEdges;
    QSqlQueryModel* backEdges;
    int start;
};


///* ==== City Struct =================================================
//    Struct consisting of a name and a distance for the AdjacencyList
//    and weighted BFS algorithm.
//================================================================== */
//struct City {
//    std::string name;
//    int dist = 0;
//};

///* ==== Edge Struct =================================================
//    Struct consisting of 2 city names and the distance between, used
//    to build the discovery edge and back edge vectors for both
//    AdjacencyList and AdjacencyMatrix.
//================================================================== */
//struct Edge {
//    std::string city1;
//    std::string city2;
//    int dist = 0;
//};

///* ==== AdjacencyList Class =========================================
//    Class used to build and display the AdjacencyList along with a
//    weighted DFS algorithm.
//================================================================== */
//class AdjacencyList {
//public:
//    AdjacencyList(int size);
//    void add(std::string city1, std::string city2, int dist);
//    void printList();
//    void printEdges();
//    void DFS(std::string name);
//    int DFS(std::string name, int total);
//private:
//    int size;
//    std::vector<std::vector<City>> cities;
//    std::map<std::string, bool> visited;
//    std::vector<Edge> discoveryEdge;
//    std::vector<Edge> backEdge;
//};

///* ==== AdjacencyMatrix Class =======================================
//    Class used to build and display the AdjacencyMatrix along with a
//    weighted BFS algorithm.
//================================================================== */
//class AdjacencyMatrix {
//public:
//    AdjacencyMatrix(int size);
//    void add(std::string city1, std::string city2, int dist);
//    void printMatrix();
//    void printEdges();
//    void BFS(std::string name);
//    ~AdjacencyMatrix();
//private:
//    int size;
//    int** distances;
//    std::map<std::string, int> cities;
//    std::vector<Edge> discoveryEdge;
//    std::vector<Edge> backEdge;
//};

///* ==== AdjacencyList Constructor with Parameters ===================
//    Constructor used to initialize size and create the first element
//    for each level of the cities vector.
//================================================================== */
//AdjacencyList::AdjacencyList(int size) : size(size) {
//    for (int i = 0; i < size; ++i) {
//        std::vector<City> row(1);
//        cities.push_back(row);
//    }
//}

///* ==== AdjacencyList add() =========================================
//    void-returning method used to add cities to the adjacency list,
//    first checks if each city exists in the list, if a city does not
//    exist then it is created and added to the list.
//================================================================== */
//void AdjacencyList::add(std::string city1, std::string city2, int dist) {
//    bool city1Exists = 0;
//    bool city2Exists = 0;

//    for (int i = 0; i < size; ++i) {
//        if (cities[i][0].name == city1) {
//            cities[i].push_back({city2, dist});
//            backEdge.push_back({ cities[i][0].name, city2, dist });
//            city1Exists = 1;
//        } else if (cities[i][0].name == city2) {
//            cities[i].push_back({city1, dist});
//            city2Exists = 1;
//        }
//    }
//    if (!city1Exists) {
//        for (int i = 0; i < size; ++i) {
//            if (cities[i][0].name == "") {
//                cities[i][0].name = city1;
//                cities[i].push_back({city2, dist});
//                backEdge.push_back({ cities[i][0].name, city2, dist });
//                visited.insert({ city1, false });
//                break;
//            }
//        }
//    }
//    if (!city2Exists) {
//        for (int i = 0; i < size; ++i) {
//            if (cities[i][0].name == "") {
//                cities[i][0].name = city2;
//                cities[i].push_back({ city1, dist });
//                visited.insert({ city2, false });
//                break;
//            }
//        }
//    }
//}

///* ==== AdjacencyList printList() ===================================
//    void-returning method used to print out the Adjacency list.
//================================================================== */
//void AdjacencyList::printList() {
//    std::cout << "Adjacency List:";
//    for (std::vector<City> city : cities) {
//        std::cout << "\nVertex " << city[0].name;

//        for (int i = 1; i < city.size(); ++i)
//            std::cout << " -> " << city[i].name << "(" << city[i].dist << ")";
//    }
//}

///* ==== AdjacencyList printEdges() ===================================
//    void-returning method used to print out the discovery edges and
//    back edges for the Adjacency list, the DFS method must be ran
//    in order to populate the discovery edges.
//================================================================== */
//void AdjacencyList::printEdges() {
//    std::cout << "\nDiscovery Edges:\n";
//    for (Edge edge : discoveryEdge) {
//        std::cout << edge.city1 << " -- " << edge.city2 << " (" << edge.dist << ")\n";
//        for (int i = 0; i < backEdge.size(); ++i) {
//            if ((edge.city1 == backEdge[i].city1 && edge.city2 == backEdge[i].city2) ||
//                (edge.city1 == backEdge[i].city2 && edge.city2 == backEdge[i].city1))
//                backEdge.erase(backEdge.begin() + i);
//        }
//    }
//    std::cout << "\nBack Edges:\n";
//    for (Edge edge : backEdge)
//        std::cout << edge.city1 << " -- " << edge.city2 << " (" << edge.dist << ")\n";
//}

///* ==== AdjacencyList DFS() =========================================
//    void-returning helper method to output the result of the DFS
//    algorithm used with the Adjacency list. lambda used to sort the
//    list in order by distance from starting city.
//================================================================== */
//void AdjacencyList::DFS(std::string name) {
//    for (int i = 0; i < size; ++i)
//        std::sort(cities[i].begin() + 1, cities[i].end(),
//            [](const City& lhs, const City& rhs) { return lhs.dist < rhs.dist; });

//    std::cout << "\n\nDFS:\n";
//    int total = DFS(name, 0);
//    std::cout << "\nTotal Distance: " << total << std::endl;
//}

///* ==== AdjacencyList DFS() =========================================
//    int-returning recursive method used to run the adjacency list
//    through the DFS algorithm. The distance from each starting city
//    is added to the running total and the total is returned to caller.
//================================================================== */
//int AdjacencyList::DFS(std::string name, int total) {
//    std::cout << name;
//    visited[name] = true;

//    for (int i = 0; i < size; ++i) {
//        if (cities[i][0].name == name) {
//            for (int j = 1; j < cities[i].size(); ++j) {
//                if (!visited[cities[i][j].name]) {
//                    std::cout << " -> ";
//                    discoveryEdge.push_back({ name, cities[i][j].name, cities[i][j].dist });
//                    total = DFS(cities[i][j].name, total) + cities[i][j].dist;
//                }
//            }
//        }
//    }
//    return total;
//}

///* ==== AdjacencyMatrix Constructor with Parameters =================
//    Constructor used to initialize size and create a 2D array to
//    store the distance matrix.
//================================================================== */
//AdjacencyMatrix::AdjacencyMatrix(int size) : size(size) {
//    distances = new int* [size];

//    for (int i = 0; i < size; ++i) {
//        distances[i] = new int[size];
//        for (int j = 0; j < size; ++j)
//            distances[i][j] = 0;
//    }
//}

///* ==== AdjacencyMatrix add() =======================================
//    void-returning method used to add cities to the adjacency matrix,
//    first checks if each city exists in the matrix, if a city does not
//    exist then it is created and added to the matrix.
//================================================================== */
//void AdjacencyMatrix::add(std::string city1, std::string city2, int dist) {
//    int i = 0;
//    int j = 0;

//    if (cities.find(city1) != cities.end())
//        i = cities[city1];
//    else i = cities[city1] = cities.size();

//    if (cities.find(city2) != cities.end())
//        j = cities[city2];
//    else j = cities[city2] = cities.size();

//    distances[i][j] = distances[j][i] = dist;
//    backEdge.push_back({ city1, city2, dist });
//}

///* ==== AdjacencyMatrix printMatrix() ===============================
//    void-returning method used to print out the Adjacency matrix.
//================================================================== */
//void AdjacencyMatrix::printMatrix() {
//    std::cout << "\nAdjacency Matrix:\t  ";
//    for (int i = 0; i < size; ++i)
//        std::cout << i << "\t";
//    std::cout << std::endl;

//    for (int i = 0; i < size; ++i) {
//        std::string city;

//        for (auto it = cities.begin(); it != cities.end(); ++it)
//            if (it->second == i) city = it->first;

//        std::cout << "Vertex " << i << ": " << city << "\t[ ";
//        for (int j = 0; j < size; ++j)
//            std::cout << distances[i][j] << "\t";
//        std::cout << "]\n\n";
//    }
//}

///* ==== AdjacencyList printEdges() ===================================
//    void-returning method used to print out the discovery edges and
//    back edges for the Adjacency matrix, the BFS method must be ran
//    in order to populate the discovery edges.
//================================================================== */
//void AdjacencyMatrix::printEdges() {
//    std::cout << "\nDiscovery Edges:\n";
//    for (Edge edge : discoveryEdge) {
//        std::cout << edge.city1 << " -- " << edge.city2 << " (" << edge.dist << ")\n";
//        for (int i = 0; i < backEdge.size(); ++i) {
//            if ((edge.city1 == backEdge[i].city1 && edge.city2 == backEdge[i].city2) ||
//                (edge.city1 == backEdge[i].city2 && edge.city2 == backEdge[i].city1))
//                backEdge.erase(backEdge.begin() + i);
//        }
//    }
//    std::cout << "\nBack Edges:\n";
//    for (Edge edge : backEdge)
//        std::cout << edge.city1 << " -- " << edge.city2 << " (" << edge.dist << ")\n";
//}

///* ==== AdjacencyList BFS() =========================================
//    void-returning method used to run the adjacency matrix through
//    the BFS algorithm. The distance from each starting cityis added
//    to the running total and the total displayed to screen.
//================================================================== */
//void AdjacencyMatrix::BFS(std::string name) {
//    std::queue<std::string> queue;
//    std::vector<City> tempCities;
//    std::map<std::string, bool> visited;
//    int total = 0;

//    for (auto it = cities.begin(); it != cities.end(); ++it) visited[it->first] = false;

//    std::cout << "\nBFS:\n";
//    visited[name] = true;
//    queue.push(name);

//    while (!queue.empty()) {
//        name = queue.front();
//        std::cout << name;
//        queue.pop();

//        for (int i = 0; i < size; ++i) {
//            std::string city;

//            for (auto it = cities.begin(); it != cities.end(); ++it)
//                if (it->second == i) city = it->first;

//            if (distances[cities[name]][i] != 0 && !visited[city]) {
//                tempCities.push_back({ city, distances[cities[name]][i] });
//                visited[city] = true;
//                total += distances[cities[name]][i];
//            }
//        }
//        std::sort(tempCities.begin(), tempCities.end(),
//            [](const City& lhs, const City& rhs) { return lhs.dist < rhs.dist; });

//        while (!tempCities.empty()) {
//            discoveryEdge.push_back({ name, tempCities[0].name, tempCities[0].dist });
//            queue.push(tempCities[0].name);
//            tempCities.erase(tempCities.begin());
//        }
//        if (!queue.empty()) std::cout << " -> ";
//    }
//    std::cout << "\nTotal Distance: " << total << std::endl;
//}

///* ==== AdjacencyList Destructor ====================================
//    Destructor used to delete the heap allocated memory used for the
//    adjacency matrix.
//================================================================== */
//AdjacencyMatrix::~AdjacencyMatrix() {
//    for (int i = 0; i < size; ++i)
//        delete[] distances[i];
//    delete[] distances;
//}


#endif // GRAPHVIEWER_H
