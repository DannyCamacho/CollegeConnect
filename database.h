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

/*! \class Database
 *  \brief Handles the SQLite database
 *  This class handles the creation, connection, and modification of the SQLite database.
 */
class Database {

public:

    /*! Database Constructor
     *
     *  Constructor used to connect, initialize tables, populate inital colleges and souvenirs, and set isInitialized to true. Additional instances will not be reinialized.
     */
    Database();

    /*! @fn void populateColleges()
     *
     *  void-returning method used to populate the initial colleges from the selected file.
     *  @param std::string fileName
     */
    void populateColleges(std::string fileName);

    /*! @fn void populateSouvenirs()
     *
     *  void-returning method used to populate the initial souvenirs from the selected file.
     */
    void populateSouvenirs(std::string fileName);

    /*! @fn importColleges(std::string fileName)
     *
     *  void-returning method used to import colleges from the selected file.
     *  @param std::string fileName
     */
    void importColleges(std::string fileName);

    /*! @fn importSouvenirs(std::string fileName)
     *
     *  void-returning method used to import souvenirs from the selected file.
     *  @param std::string fileName
     */
    void importSouvenirs(std::string fileName);

private:

    /*! @fn void connect()
     *
     *  void-returning method used to create and connect to a SQLite database. Warning messages are printed to the console if issues arise.
     */
    void connect();

    /*! @fn void init()
     *
     *  void-returning method used to initialize the tables used throughout the program.
     */
    void init();

    /*! @var static bool isInitialized
     *
     *  static bool variable used to determine whether the database has been previously connected.
     */
    static bool isInitialized;
};


/*! \struct College
 *  \brief College Struct for AdjacencyMatrix
 *  This Struct consists of a name and a distance for the AdjacencyMatrix.
 */
struct College {

    /*! @var std::string name
     *
     *  String used to hold the name of a college.
     */
    std::string name;

    /*! @var double dist
     *
     *  Double used to hold a distance for a college.
     */
    double dist = 0;
};


/*! \struct Edge
 *  \brief Edge Struct for AdjacencyMatrix
 *  This Struct consists of 2 names and the distance between, used to build the discovery edge and back edge vectors for the AdjacencyMatrix.
 */
struct Edge {

    /*! @var std::string city1
     *
     *  String used to hold the starting name of an Edge.
     */
    std::string city1;

    /*! @var std::string city2
     *
     *  String used to hold the ending name of an Edge.
     */
    std::string city2;

    /*! @var double dist
     *
     *  Double used to hold a distance for an Edge.
     */
    double dist = 0;
};


/*! \class AdjacencyMatrix
 *  \brief Manages the graphs
 *  This class is used to build the AdjacencyMatrix with graph algorithms.
 */
class AdjacencyMatrix {
public:

    /*! Database AdjacencyMatrix
     *
     *  Constructor used to initialize size and create a 2D vector to store the distance matrix.
     *  @param int size
     */
    AdjacencyMatrix(int size);

    /*! @fn void add(std::string city1, std::string city2, double dist)
     *
     *  void-returning method used to add cities to the adjacency matrix, first checks if each city exists in the matrix, if a city does not exist then it is created and added to the matrix.
     *  @param std::string city1
     *  @param std::string city2
     *  @param double dist
     */
    void add(std::string city1, std::string city2, double dist);

    /*! @fn void setSize(int size)
     *
     *  void-returning method used to set the size of the adjacency matrix.
     *  @param int size
     */
    void setSize(int size);

    /*! @fn void getStartingIndex(std::string college)
     *
     *  int-returning method used to get the starting index using the name of an institution.
     *  @param std::string college
     */
    int getStartingIndex(std::string college);

    /*! @fn void BFS(int src)
     *
     *  void-returning method used to run the adjacency matrix through the BFS algorithm. The distance from each starting cityis added to the running total and the total displayed to screen.
     *  @param int src
     */
    void BFS(int src);

    /*! @fn void DFS(int src)
     *
     *  void-returning helper method to output the result of the DFS algorithm used with the adjacency matrix. lambda used to sort the list in order by distance from starting city.
     *  @param int src
     */
    void DFS(int src);

    /*! @fn int DFS(int src, std::vector<bool>& visited)
     *
     *  int-returning recursive method used to run the adjacency matrix through the DFS algorithm. The distance from each starting city is added to the running total and the total is returned to caller.
     *  @param int src
     *  @param std::vector<bool>& visited
     */
    int DFS(int src, std::vector<bool>& visited);

    /*! @fn void dijkstra(int src)
     *
     *  void-returning method used to run the adjacency matrix through the Dijkstra algo. The distance and path for each city is determined and the totals are displayed to screen.void-returning method used to run the adjacency matrix through the Dijkstra algo. The distance and path for each city is determined and the totals are displayed to screen.
     *  @param int src
     */
    void dijkstra(int src);

    /*! @fn void mst();
     *
     *  void-returning method used to run the adjacency matrix through the Prim MST algorithm. Each edge is displayed to screen with the distance between, followed by the total of all edge distances.
     */
    void mst();

    /*! @fn int minKey(std::vector <double> key, std::vector <bool> set)
     *
     *  int-returning method used by both the dijkstra and mst algorithms respectively. Used to determine the next minimum index.
     *  @param std::vector <double> key
     *  @param std::vector <bool> set
     */
    int minKey(std::vector <double> key, std::vector <bool> set);

private:

    /*! @var int size
     *
     *  Int used to hold a size of the adjacency matrix.
     */
    int size;

    /*! @var std::vector<std::vector<double>> distances
     *
     *  2D-vector used to hold the distances of the adjacency matrix.
     */
    std::vector<std::vector<double>> distances;

    /*! @var std::map<std::string, int> colleges
     *
     *  Map used to get the collegeNum using the collegeName.
     */
    std::map<std::string, int> colleges;

    /*! @var std::map<int, std::string> collegesIdx
     *
     *  Map used to get the collegeName using the collegeNum.
     */
    std::map<int, std::string> collegesIdx;
};

#endif // DATABASE_H
