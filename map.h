#ifndef MAP_H
#define MAP_H

#include <iostream>

/* ==== Pair Struct =================================================
    Struct consisting of a key and a data value.
================================================================== */
struct Pair {
    int key = -1;  // college num is key
    //std::string value;
    std::string collegeName;
    std::string state;
    int numsOfGrad;
};

/* ==== Map Class ===================================================
    Abstract base class used for derived classes DoubleHashMap and
    QuadraticMap. Defines all methods except for h2(). Map class
    destructor is also used by the derived classes.
================================================================== */
class Map {
public:
    Map(int N);
    int h1(int key);
    virtual int h2(int j, int key) = 0;
    void insert(Pair pair);
    void erase(int key);
    void printMap();
    ~Map();
protected:
    Pair* map;
    int size;
};

/* ==== DoubleHashMap Class =========================================
    Derived class of the Map class. h2() is defined within this
    class as: h'(k) = 13 - k mod 13.
================================================================== */
class DoubleHashMap : public Map {
public:
    DoubleHashMap(int N);
    int h2(int j, int key);
};

/* ==== QuadraticMap Class ==========================================
    Derived class of the Map class. h2() is defined within this
    class as: h'(k) = h(k) + j^2 mod N.
================================================================== */
class QuadraticMap : public Map {
public:
    QuadraticMap(int N);
    int h2(int j, int key);
};

#endif // MAP_H
