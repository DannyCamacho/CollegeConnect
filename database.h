#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QAbstractProxyModel>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
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
    Struct consisting of a name and a distance for the Dijkstra algo.
================================================================== */
struct College {
    std::string name;
    double dist = 0;
};

/* ==== Edge Struct =================================================
    Struct consisting of 2 city names and the distance between, used
    to build the discovery edge and back edge vectors for both
    AdjacencyList and AdjacencyMatrix.
================================================================== */
struct Edge {
    std::string city1;
    std::string city2;
    double dist = 0;
};

/* ==== AdjacencyMatrix Class =======================================
    Class used to build the AdjacencyMatrix with a Dijkstra algo.
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
    std::vector<Edge> discoveryEdge;
};

/* ==== Horizontal_proxy_model Class ================================
    Class used to swap columns and rows displayed on a model.
================================================================== */
class Horizontal_proxy_model : public QAbstractProxyModel {
public:
  Horizontal_proxy_model(QObject * parent = 0);
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // DATABASE_H
