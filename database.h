#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QAbstractProxyModel>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <map>

typedef std::pair<double, int> pi;

class Database {
public:
    Database();
    void populateColleges(std::string fileName);
    void populateSouvenirs(std::string fileName);
    void importColleges(std::string fileName);
    void importSouvenirs(std::string fileName);
private:
    void connect();
    void init();
    static bool isInitialized;
};

/* ==== City Struct =================================================
    Struct consisting of a name and a distance for AdjacencyMatrix.
================================================================== */
struct College {
    std::string name;
    double dist = 0;
};

/* ==== Edge Struct =================================================
    Struct consisting of 2 city names and the distance between, used
    to build the discovery edge and back edge vectors for the
    AdjacencyMatrix.
================================================================== */
struct Edge {
    std::string city1;
    std::string city2;
    double dist = 0;
};

/* ==== AdjacencyMatrix Class =======================================
    Class used to build the AdjacencyMatrix with graph algorithms.
================================================================== */
class AdjacencyMatrix {
public:
    AdjacencyMatrix(int size);
    void add(std::string city1, std::string city2, double dist);
    void setSize(int size);
    int getStartingIndex(std::string college);
    void BFS(int src);
    void DFS(int src);
    int DFS(int src, std::vector<bool>& visited);
    void dijkstra(int src);
    void mst();
    int minKey(std::vector <double> key, std::vector <bool> set);
private:
    int size;
    std::vector<std::vector<double>> distances;
    std::map<std::string, int> colleges;
    std::map<int, std::string> collegesIdx;
};

#endif // DATABASE_H
