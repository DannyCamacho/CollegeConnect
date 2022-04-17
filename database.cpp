#include "database.h"

#include <fstream>
#include <string>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

bool Database::isInitialized = false;

Database::Database() {
    if (!isInitialized) {
        connect();
        init();
        isInitialized = true;
    }
}

void Database::connect() {
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName(":memory:");
        if(!db.open()) qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    } else qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void Database::init() {
    QSqlQuery query("CREATE TABLE college (collegeName TEXT, collegeNum INTEGER, state TEXT, undergrads INTEGER);");
    query.exec("CREATE TABLE edge (collegeName TEXT, endingCollege TEXT, distance INTEGER);");
    query.exec("CREATE TABLE souvenir (collegeName TEXT, collegeNum INTEGER, item TEXT, price INTEGER);");
}

void Database::populateColleges(std::string fileName) {
    QSqlQuery query;
    std::ifstream infile;
    std::string collegeName;
    std::string endingCollege;
    std::string distance;
    std::string state;
    std::string undergrads;
    size_t collegeNum = 0;

    infile.open(fileName);
    if (!infile) { std::cout << "Error: File not found or corrupt. " << std::endl; return; }
    getline(infile, collegeName);

    while (getline(infile, collegeName, ',') && getline(infile, endingCollege, ',') && getline(infile, distance, ',') && getline(infile, state, ',') && getline(infile, undergrads)) {
        if (collegeName[0] == '\"') {
            collegeName += "," + endingCollege;
            endingCollege = distance;
            distance = state;
            size_t pos = 0;
            while (undergrads[pos] != ',') ++pos;
            state = undergrads.substr(0, pos);
            undergrads.erase(0, pos + 1);
        }

        if (endingCollege[0] == '\"') {
            endingCollege += "," + distance;
            distance = state;
            size_t pos = 0;
            while (undergrads[pos] != ',') ++pos;
            state = undergrads.substr(0, pos);
            undergrads.erase(0, pos + 1);
        }

        collegeName.erase(remove(collegeName.begin(), collegeName.end(), '\"'),collegeName.end());
        endingCollege.erase(remove(endingCollege.begin(), endingCollege.end(), '\"'),endingCollege.end());
        undergrads.erase(remove(undergrads.begin(), undergrads.end(), ','),undergrads.end());
        if (state[0] == ' ') state = state.substr(1, state.size());

        query.exec("INSERT INTO college(collegeName, collegeNum, state, undergrads) VALUES (\"" + QString::fromStdString(collegeName) + "\", \"" + QString::number(collegeNum++) + "\", \"" + QString::fromStdString(state) + "\", \"" + QString::number(std::stoi(undergrads)) + "\");");
        query.exec("INSERT INTO edge(collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegeName) + "\", \"" + QString::fromStdString(endingCollege) + "\", \"" + QString::number(std::stod(distance)) + "\");");

        //FOLLOWING CODE USED FOR FILE I/O AND SQLITE TESTING PURPOSES:
        //std::cout << collegeName << " " << collegeNum << " " << state << " " << std::stoi(undergrads) << "\n";
        //query.exec("SELECT * FROM college WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\"");
        //query.next();
        //std::cout << query.value(0).toString().toStdString() << " " << query.value(1).toString().toStdString() << " " << query.value(2).toString().toStdString() << " " << query.value(3).toString().toStdString() << std::endl;
        //std::cout << collegeName << " " << endingCollege << " " << std::stod(distance) << "\n";
        //query.exec("SELECT * FROM edge WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\" AND endingCollege =\"" + QString::fromStdString(endingCollege) + "\";");
        //query.next();
        //std::cout << query.value(0).toString().toStdString() << " " << query.value(1).toString().toStdString() << " " << query.value(2).toString().toStdString() << std::endl;

        for (int i = 0; i < 9; ++i) {
            getline(infile, collegeName, ',');
            getline(infile, endingCollege, ',');
            getline(infile, distance);

            if (collegeName[0] == '\"') {
                collegeName += "," + endingCollege;
                size_t pos = 0;
                while (distance[pos] != ',') ++pos;
                endingCollege = distance.substr(0, pos);
                distance.erase(0, pos + 1);
            }

            if (endingCollege[0] == '\"') {
                size_t pos = 0;
                while (distance[pos] != '\"') ++pos;
                endingCollege += "," + distance.substr(0, pos);
                distance.erase(0, pos + 1);
            }

            collegeName.erase(remove(collegeName.begin(), collegeName.end(), '\"'),collegeName.end());
            endingCollege.erase(remove(endingCollege.begin(), endingCollege.end(), '\"'),endingCollege.end());
            distance.erase(remove(distance.begin(), distance.end(), ','),distance.end());
            query.exec("INSERT INTO edge(collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegeName) + "\", \"" + QString::fromStdString(endingCollege) + "\", \"" + QString::number(std::stod(distance)) + "\");");

            //FOLLOWING CODE USED FOR FILE I/O AND SQLITE TESTING PURPOSES:
            //std::cout << collegeName << " " << endingCollege << " " << distance << "\n";
            //query.exec("SELECT * FROM edge WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\" AND endingCollege =\"" + QString::fromStdString(endingCollege) + "\";");
            //query.next();
            //std::cout << query.value(0).toString().toStdString() << " " << query.value(1).toString().toStdString() << " " << query.value(2).toString().toStdString() << " " << std::endl;
        }
    } infile.close();
}

void Database::populateSouvenirs(std::string fileName) {
    QSqlQuery query;
    std::ifstream infile;
    std::string tempName;
    std::string collegeName;
    std::string item;
    std::string price;
    QString collegeNum;

    infile.open(fileName);
    if (!infile) { std::cout << "Error: File not found or corrupt. " << std::endl; return; }
    getline(infile, collegeName);

    while (getline(infile, tempName, ',') && getline(infile, item, ',') && getline(infile, price)) {
        if (tempName[0] == ' ') tempName = tempName.substr(1, tempName.size());
        if (tempName != "") {
            if (tempName[0] == '\"') {
                tempName += "," + item;
                tempName.erase(remove(tempName.begin(), tempName.end(), '\"'),tempName.end());
            }
            collegeName = tempName;

            query.exec("SELECT collegeNum FROM college WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\"");
            query.next();
            collegeNum = query.value(0).toString();
        } else {
        price = price.substr(1, price.size());
        price.erase(remove(price.begin(), price.end(), ','),price.end());

        query.exec("INSERT INTO souvenir(collegeName, collegeNum, item, price) VALUES (\"" + QString::fromStdString(collegeName) + "\", \"" + collegeNum + "\", \"" + QString::fromStdString(item) + "\", \"" + QString::number(std::stod(price)) + "\");");

        //FOLLOWING CODE USED FOR FILE I/O AND SQLITE TESTING PURPOSES:
        //std::cout << collegeName << " " << collegeNum.toStdString() << " " << item << " " << std::stod(price) << "\n";
        //query.exec("SELECT * FROM souvenir WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\" AND item =\"" + QString::fromStdString(item) + "\";");
        //query.next();
        //std::cout << query.value(0).toString().toStdString() << " " << query.value(1).toString().toStdString() << " " << query.value(2).toString().toStdString() << " " << query.value(3).toString().toStdString() << std::endl;
        }
    } infile.close();
}

void Database::importColleges(std::string fileName) {
}

void Database::importSouvenirs(std::string fileName) {
}
