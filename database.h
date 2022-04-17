#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>

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

#endif // DATABASE_H
