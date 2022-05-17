#ifndef MAP_H
#define MAP_H

#include <iostream>

/*! \struct Pair
 *  \brief Pair Struct for Map classes
 *  This Struct consists of a key mapping to a collegeName, state, number of undergrads, and distance from Saddleback College, used for the custom double hashing Map class.
 */
struct Pair {

    /*! @var int key
     *
     *  int used to hold the key of a mapped data.
     */
    int key = -1;

    /*! @var std::string collegeName
     *
     *  String used to hold the name of a college.
     */
    std::string collegeName;

    /*! @var std::string state
     *
     *  String used to hold the name of a state.
     */
    std::string state;

    /*! @var int numsOfGrad
     *
     *  int used to hold the number of undergrads.
     */
    int numsOfGrad;

    /*! @var double distFromSaddleback
     *
     *  double used to hold the distance from Saddleback College.
     */
    double distFromSaddleback;
};


/*! \class Map
 *  \brief Abstract class for maps
 *  Abstract base class used for derived class DoubleHashMap. Defines all methods except for h2(). Map class destructor is also used by the derived class.
 */
class Map {

public:

    /*! Map Constructor
     *
     *  Constructor used to initialize map to size N and size to N. Used by derived class constructors.
     *  @param int N
     */
    Map(int N);

    /*! Map Destructor
     *
     *  Destructor used to delete heap memory allocated for map. Used by derived classes as destructor.
     */
    ~Map();

    /*! @fn int h1(int key)
     *
     *  Int-returning method used to calulate the element position for map. The position of the Pair is determined by the following equation: h(k) = k mod N.
     *  @param int key
     */
    int h1(int key);

    /*! @fn virtual int h2(int j, int key) = 0
     *
     *  Virtual method defined within derived classes, used as the hashing method after failed first attempts using h1(int key).
     *  @param int j
     *  @param int key
     */
    virtual int h2(int j, int key) = 0;

    /*! @fn insert(Pair pair)
     *
     *  Void-returning method used to insert a Pair into the map. The method first attempts to find the element using the h1 method. If h1 position of map is populated or is not the same key value as the Pair, the position is then found using an endless for loop incrementing j and calling the h2() method. Once a position is found the method is ended using a return call.
     *  @param Pair pair
     */
    void insert(Pair pair);

    /*! @fn void erase(int key);
     *
     *  Void-returning method used to erase an existing key value stored within the map. The method first attempts to erase the element using the h1 method. If h1 position of map does not match the key value provided, the key is then found using an endless for loop incrementing j and calling the h2() method. Once the key is found and erased, the method is ended using a return call.
     *  @param int key
     */
    void erase(int key);

    /*! @fn void printMap();
     *
     *  Void-returning method used to print the key and value held at each populated element of map.
     */
    void printMap();

    /*! @fn Pair at(int i);
     *
     *  Pair-returning method used get the map value at a specific index.
     *  @param int i
     */
    Pair at(int i);

protected:

    /*! @var Pair* map
     *
     *  Pair used to store the elements of the map.
     */
    Pair* map;

    /*! @var int size
     *
     *  Int used to hold the size of the map.
     */
    int size;
};


/*! \class DoubleHashMap
 *  \brief Derived Map class using double hashing
 *  Derived class of the Map class. h2() is defined within this class as: h'(k) = 13 - k mod 13.
 *  @see map.h
 */
class DoubleHashMap : public Map {
public:

    /*! DoubleHashMap Constructor
     *
     *  Constructor used to initialize map to size N and size to N. Uses the base class constructor.
     *  @param int N
     */
    DoubleHashMap(int N);

    /*! @fn int h2(int j, int key)
     *
     *  int-returning method used to calulate the element position for map. The position of the Pair is determined by the following equation: h'(k) = (h(k) + (j * (13 - (k mod 13)))) mod N.
     *  @param int j
     *  @param int key
     */
    int h2(int j, int key);
};

#endif // MAP_H
