#include "map.h"

/* ==== Map Constructor with Parameters =============================
    Constructor used to initialize map to size N and size to N. Used
    by derived class constructors.
================================================================== */
Map::Map(int N) : map(new Pair[N]), size(N) {}

/* ==== Map h1() ====================================================
    int-returning method used to calulate the element position for
    map. The position of the Pair is determined by the following
    equation: h(k) = k mod N.
================================================================== */
int Map::h1(int key) { return key % size; }

/* ==== Map insert() ================================================
    void-returning method used to insert a Pair into the map. The
    method first attempts to find the element using the h1 method.
    If h1 position of map is populated or is not the same key value
    as the Pair, the position is then found using an endless for loop
    incrementing j and calling the h2() method. Once a position is
    found the method is ended using a return call.
================================================================== */
void Map::insert(Pair pair) {
    int k = h1(pair.key);
    if (map[k].key == -1 || map[k].key == pair.key) {
        map[k] = pair;
    } else {
        for (int j = 1; ; ++j) {
            int i = h2(j, pair.key);
            if (map[i].key == -1 || map[i].key == pair.key) {
                map[i] = pair;
                return;
            }
        }
    }
}

/* ==== Map erase() =================================================
    void-returning method used to erase an existing key value stored
    within the map. The method first attempts to erase the element
    using the h1 method. If h1 position of map does not match the key
    value provided, the key is then found using an endless for loop
    incrementing j and calling the h2() method. Once the key is found
    and erased, the method is ended using a return call.
================================================================== */
void Map::erase(int key) {
    int k = h1(key);
    if (map[k].key == key) {
        map[k] = { -1, "" , "" , 0};
    } else {
        for (int j = 1; ; ++j) {
            int i = h2(j, key);
            if (map[i].key == key) {
                map[i] = { -1, "" , "" , 0};
                return;
            }
        }
    }
}

/* ==== Map printMap() ==============================================
    void-returning method used to print the key and value held at
    each populated element of map.
================================================================== */
void Map::printMap() {
    for (int i = 0; i < size; ++i) {
        if (map[i].collegeName != "") {

           // std::cout << "Element: [" << i << "]\t\tKey: " << map[i].key
           //           << " \t  Value: " << map[i].collegeName << std::endl;
            std::cout << "ID: " << map[i].key
                      << "\tCollege name: " << map[i].collegeName
                      << "\tState: " << map[i].state
                      << "\tNo. of undergrats: " << map[i].numsOfGrad << std::endl;
        }
    }
}

/* ==== Map Destructor ==============================================
    Destructor used to delete heap memory allocated for map. Used
    by derived classes as destructor.
================================================================== */
Map::~Map() { delete[] map; }

/* ==== DoubleHashMap Constructor with parameters ===================
    Constructor used to initialize map to size N and size to N. Uses
    the base class constructor.
================================================================== */
DoubleHashMap::DoubleHashMap(int N) : Map(N) {}

/* ==== DoubleHashMap h1() ==========================================
    int-returning method used to calulate the element position for
    map. The position of the Pair is determined by the following
    equation: h'(k) = (h(k) + (j * (13 - (k mod 13)))) mod N.
================================================================== */
int DoubleHashMap::h2(int j, int key) { return (h1(key) + (j * (13 - (key % 13)))) % size; }

/* ==== QuadraticMap Constructor with parameters ====================
    Constructor used to initialize map to size N and size to N. Uses
    the base class constructor.
================================================================== */
QuadraticMap::QuadraticMap(int N) : Map(N) {}

/* ==== QuadraticMap h1() ===========================================
    int-returning method used to calulate the element position for
    map. The position of the Pair is determined by the following
    equation: h'(k) = (h(k) + j^2) mod N.
================================================================== */
int QuadraticMap::h2(int j, int key) { return (h1(key) + j * j) % size; }
