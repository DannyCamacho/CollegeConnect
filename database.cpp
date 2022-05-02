#include "database.h"

bool Database::isInitialized = false;

Database::Database() {
    if (!isInitialized) {
        connect();
        init();
        populateColleges("../CollegeConnect/Distances.csv");
        populateSouvenirs("../CollegeConnect/Souvenirs.csv");
        isInitialized = true;
    }
}

void Database::connect() {
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName(":memory:");
        //db.setDatabaseName("C:\\Users\\thyva\\OneDrive\\Desktop\\db\\College.sqlite");
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
    QSqlQuery query;
    std::ifstream infile;
    std::string collegeName;
    std::string endingCollege;
    std::string distance;
    std::string state;
    std::string undergrads;
    size_t collegeNum = 11;

    infile.open(fileName);
    if (!infile) { std::cout << "Error: File not found or corrupt. " << std::endl; return; }
    getline(infile, collegeName);

    for (int i = 0; i < 2; ++i) {
        getline(infile, collegeName, ',');
        getline(infile, endingCollege, ',');
        getline(infile, distance, ',');
        getline(infile, state, ',');
        getline(infile, undergrads);

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

        for (int i = 0; i < 11; ++i) {
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
        }
    }

    while(getline(infile, collegeName, ',') && getline(infile, endingCollege, ',') && getline(infile, distance)) {
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
    } infile.close();
}

void Database::importSouvenirs(std::string fileName) {
    QSqlQuery query;
    std::ifstream infile;
    std::string tempName;
    std::string collegeName;
    std::string item;
    std::string price;
    QString collegeNum;

    infile.open(fileName);
    if (!infile) { std::cout << "Error: File not found or corrupt. " << std::endl; return; }

    while (getline(infile, tempName, ',') && getline(infile, item, ',') && getline(infile, price)) {
        if (tempName[0] == ' ') tempName = tempName.substr(1, tempName.size());
        if (tempName != "") {
            if (tempName[0] == '\"') {
                tempName += "," + item;
                tempName.erase(remove(tempName.begin(), tempName.end(), '\"'),tempName.end());
            }
            collegeName = tempName;
            query.exec("SELECT collegeNum FROM college WHERE collegeName =\"" + QString::fromStdString(collegeName) + "\"");
            if (query.next()) collegeNum = query.value(0).toString();
        } else {
        price = price.substr(1, price.size());
        price.erase(remove(price.begin(), price.end(), ','),price.end());
        query.exec("INSERT INTO souvenir(collegeName, collegeNum, item, price) VALUES (\"" + QString::fromStdString(collegeName) + "\", \"" + collegeNum + "\", \"" + QString::fromStdString(item) + "\", \"" + QString::number(std::stod(price)) + "\");");
        }
    } infile.close();
}

Horizontal_proxy_model::Horizontal_proxy_model(QObject *parent) : QAbstractProxyModel(parent) {}

QModelIndex Horizontal_proxy_model::mapToSource(const QModelIndex &proxyIndex) const {
  if (sourceModel()) return sourceModel()->index(proxyIndex.column(), proxyIndex.row());
  else return QModelIndex();
}

QModelIndex Horizontal_proxy_model::mapFromSource(const QModelIndex &sourceIndex) const {
  return index(sourceIndex.column(), sourceIndex.row());
}

QModelIndex Horizontal_proxy_model::index(int row, int column, const QModelIndex &) const {
  return createIndex(row, column, (void*) 0);
}

QModelIndex Horizontal_proxy_model::parent(const QModelIndex &) const {
  return QModelIndex();
}

int Horizontal_proxy_model::rowCount(const QModelIndex &) const {
  return sourceModel() ? sourceModel()->columnCount() : 0;
}

int Horizontal_proxy_model::columnCount(const QModelIndex &) const {
  return sourceModel() ? sourceModel()->rowCount() : 0;
}

QVariant Horizontal_proxy_model::headerData(int section, Qt::Orientation orientation, int role) const {
  if (!sourceModel()) return QVariant();
  Qt::Orientation new_orientation = orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal;
  return sourceModel()->headerData(section, new_orientation, role);
}
