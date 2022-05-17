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
        //db.setDatabaseName("C:\\Users\\vyngu\\OneDrive\\Desktop\\dbCollege.sqlite");
        if(!db.open()) qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    } else qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void Database::init() {
    QSqlQuery query("CREATE TABLE college (collegeName TEXT, collegeNum INTEGER, state TEXT, undergrads INTEGER);");
    query.exec("CREATE TABLE edge (collegeName TEXT, endingCollege TEXT, distance INTEGER);");
    query.exec("CREATE TABLE souvenir (collegeName TEXT, collegeNum INTEGER, item TEXT, price INTEGER);");
    query.exec("CREATE TABLE cart (collegeName TEXT, souvenirItem TEXT, souvenirPrice INTEGER, quantity INTEGER);");
    query.exec("CREATE TABLE tripSelected (restName TEXT, restNum INTEGER);");
    query.exec("CREATE TABLE tripRoute (restName TEXT, restNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");
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

/* ==== AdjacencyMatrix Constructor with Parameters =================
    Constructor used to initialize size and create a 2D vector to
    store the distance matrix.
================================================================== */
AdjacencyMatrix::AdjacencyMatrix(int size) : size(size), distances(size, std::vector<double>(size)) {}

/* ==== AdjacencyMatrix setSize() ===================================
    void-returning method used to set the size of the adjacency matrix.
================================================================== */
void AdjacencyMatrix::setSize(int size) { this->size = size; }

/* ==== AdjacencyMatrix add() =======================================
    void-returning method used to add cities to the adjacency matrix,
    first checks if each city exists in the matrix, if a city does not
    exist then it is created and added to the matrix.
================================================================== */
void AdjacencyMatrix::add(std::string city1, std::string city2, double dist) {
    int i = 0;
    int j = 0;

    if (colleges.find(city1) != colleges.end()) { i = colleges[city1]; }
    else { i = colleges[city1] = colleges.size(); collegesIdx[i] = city1; }

    if (colleges.find(city2) != colleges.end()) { j = colleges[city2]; }
    else { j = colleges[city2] = colleges.size(); collegesIdx[j] = city2; }

    distances[i][j] = distances[j][i] = dist;
}

/* ==== AdjacencyMatrix minKey() ====================================
    int-returning method used to get the starting index using the
    name of an institution.
================================================================== */
int AdjacencyMatrix::getStartingIndex(std::string college) { return colleges[college]; }

/* ==== AdjacencyMatrix BFS() =======================================
    void-returning method used to run the adjacency matrix through
    the BFS algorithm. The distance from each starting cityis added
    to the running total and the total displayed to screen.
================================================================== */
void AdjacencyMatrix::BFS(int src) {
    std::queue<std::pair<int, int>> queue;
    std::vector<bool> visited(12, false);
    std::priority_queue<pi, std::vector<pi>, std::greater<pi> > tempQueue;

    visited[src] = true;
    queue.push({ src, 0 });

    while (!queue.empty()) {
        src = queue.front().first;
        std::string collegeName = collegesIdx[src];
        queue.pop();

        for (int i = 0; i < 12; ++i) {
            if (!visited[i] && distances[src][i]) {
                visited[i] = true;
                tempQueue.push({ distances[src][i], i });
                QSqlQuery query("INSERT INTO discoveryEdges(collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegesIdx[src]) + + "\", \"" + QString::fromStdString(collegesIdx[i]) + + "\", \"" + QString::number(distances[src][i]) + "\");");

            }
        }

        while (!tempQueue.empty()) {
            queue.push({ tempQueue.top().second, tempQueue.top().first });
            QSqlQuery query("INSERT INTO path(collegeName, distToNext) VALUES (\"" + QString::fromStdString(collegesIdx[tempQueue.top().second]) + + "\", \"" + QString::number(tempQueue.top().first) + "\");");
            tempQueue.pop();
        }
    }
}

/* ==== AdjacencyMatrix DFS() =======================================
    void-returning helper method to output the result of the DFS
    algorithm used with the adjacency matrix. lambda used to sort the
    list in order by distance from starting city.
================================================================== */
void AdjacencyMatrix::DFS(int src) {
    std::vector<bool> visited(size, false);
    src = DFS(src, visited);
    QSqlQuery query("INSERT INTO path(collegeName, distToNext) VALUES (\"" + QString::fromStdString(collegesIdx[src]) + + "\", \"" + QString::number(0) + "\");");
}

/* ==== AdjacencyMatrix DFS() =======================================
    int-returning recursive method used to run the adjacency matrix
    through the DFS algorithm. The distance from each starting city
    is added to the running total and the total is returned to caller.
================================================================== */
int AdjacencyMatrix::DFS(int src, std::vector<bool>& visited) {
    std::priority_queue<pi, std::vector<pi>, std::greater<pi> > tempQueue;
    visited[src] = true;

    for (int i = 0; i < size; ++i)
        if (!visited[i] && distances[src][i])
            tempQueue.push({ distances[src][i], i });

    while (!tempQueue.empty()) {
        if (!visited[tempQueue.top().second]) {
            QSqlQuery query("INSERT INTO path(collegeName, distToNext) VALUES (\"" + QString::fromStdString(collegesIdx[src]) + + "\", \"" + QString::number(tempQueue.top().first) + "\");");
            query.exec("INSERT INTO discoveryEdges(collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegesIdx[src]) + + "\", \"" + QString::fromStdString(collegesIdx[tempQueue.top().second]) + "\", \"" + QString::number(tempQueue.top().first) + "\");");
            src = DFS(tempQueue.top().second, visited);
        }
        tempQueue.pop();
    }
    return src;
}

/* ==== AdjacencyMatrix dijkstra() ==================================
    void-returning method used to run the adjacency matrix through
    the Dijkstra algo. The distance and path for each city is
    determined and the totals are displayed to screen.
================================================================== */
void AdjacencyMatrix::dijkstra(int src) {
    std::vector<double> dist(size, INT_MAX);
    std::vector<bool> sptSet(size, false);
    std::vector<std::vector<College>> path(size, std::vector<College>());

    dist[src] = 0;

    for (int count = 0; count < size - 1; ++count) {
        int u = minKey(dist, sptSet);
        sptSet[u] = true;

        for (int i = 0; i < size; ++i) {
            if (!sptSet[i] && distances[u][i] && dist[u] != INT_MAX && dist[u] + distances[u][i] < dist[i]) {
                dist[i] = dist[u] + distances[u][i];
                path[i].clear();
                for (int k = 0; k < path[u].size(); k++) path[i].push_back(path[u][k]);
                path[i].push_back({ collegesIdx[u], distances[u][i] });
                QSqlQuery query("INSERT INTO discoveryEdges (collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegesIdx[u]) + + "\", \"" + QString::fromStdString(collegesIdx[i]) + + "\", \"" + QString::number(distances[u][i]) + "\");");
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        std::string dijkstraPath;
        for (int j = 1; j < path[i].size(); ++j)
            dijkstraPath += path[i][j].name + " 🡲 ";
        dijkstraPath += collegesIdx[i];
        QSqlQuery query("INSERT INTO path (collegeName, distToNext) VALUES (\"" + QString::fromStdString(dijkstraPath) + + "\", \"" + QString::number(dist[i]) + "\");");
    }
}

/* ==== AdjacencyMatrix mst() =======================================
    void-returning method used to run the adjacency matrix through
    the Prim MST algorithm. Each edge is displayed to screen with the
    distance between, followed by the total of all edge distances.
================================================================== */
void AdjacencyMatrix::mst() {
    std::vector<int> parent(size, 0);
    std::vector<double> key(size, INT_MAX);
    std::vector<bool> mstSet(size, false);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < size - 1; ++count) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int i = 0; i < size; ++i) {
            if (distances[u][i] && !mstSet[i] && distances[u][i] < key[i]) {
                parent[i] = u;
                key[i] = distances[u][i];
            }
        }
    }

    for (int i = 1; i < size; ++i) {
        QSqlQuery query("INSERT INTO path (collegeName, distToNext) VALUES (\"" + QString::fromStdString(collegesIdx[parent[i]] + " 🡲 " + collegesIdx[i]) + + "\", \"" + QString::number(distances[i][parent[i]]) + "\");");
        query.exec("INSERT INTO discoveryEdges (collegeName, endingCollege, distance) VALUES (\"" + QString::fromStdString(collegesIdx[parent[i]]) + + "\", \"" + QString::fromStdString(collegesIdx[i]) + + "\", \"" + QString::number(distances[i][parent[i]]) + "\");");
    }
}

/* ==== AdjacencyMatrix minKey() ====================================
    int-returning method used by both the dijkstra and mst algorithms
    respectively. Used to determine the next minimum index.
================================================================== */
int AdjacencyMatrix::minKey(std::vector<double> key, std::vector<bool> set) {
    double min = INT_MAX;
    int minIdx = 0;

    for (int i = 0; i < size; ++i) {
        if (!set[i] && key[i] < min) {
            min = key[i];
            minIdx = i;
        }
    }
    return minIdx;
}
