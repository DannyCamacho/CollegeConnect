#include "tripplanner.h"
#include "ui_tripplanner.h"

/* ==== TripPlanner Constructor =====================================
    Constructor used to initialized necessary variables, populate the
    route dropdown, and connect the widget change buttons to the main
    window.
================================================================== */
TripPlanner::TripPlanner(QWidget *parent) : QMainWindow(parent), ui(new Ui::TripPlanner) {
    ui->setupUi(this);
    availableModel = new QSqlQueryModel;
    selectedModel = new QSqlQueryModel;
    start = 4;
    ui->starting_location_dropdown_2->addItem("Efficient Route");
    ui->starting_location_dropdown_2->addItem("Custom Route");
    ui->starting_location_dropdown_2->setCurrentText("Efficient Route");
    connect(parent, SIGNAL(updateTripPlanner()), this, SLOT(populateWindow()));
    connect(this, SIGNAL(moveToSchoolStore(QString)), parent, SLOT(moveToSchoolStoreFromTrip(QString)));
}

/* ==== TripPlanner Destructor ======================================
    Destructor used to delete heap allocated memory.
================================================================== */
TripPlanner::~TripPlanner() {
    delete ui;
    delete availableModel;
    delete selectedModel;
}

/* ==== TripPlanner populateWindow() ================================
    void-returning method used to populate the window, uses the main
    college table to determine the college dropdown and uses the edge
    table to populate the distance array.
================================================================== */
void TripPlanner::populateWindow() {
    spinBoxMax = 0;
    std::map<QString, int> collegeMap;
    ui->starting_location_dropdown->clear();
    QSqlQuery query("SELECT collegeName, collegeNum FROM college ORDER BY collegeName");
    while (query.next()) {
        ++spinBoxMax;
        ui->starting_location_dropdown->addItem(query.value(0).toString());
        collegeMap[query.value(0).toString()] = query.value(1).toInt();
    }
    ui->spinBox->setRange(1, spinBoxMax - 1);
    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            d[i][j] = 0.0;

    query.exec("SELECT * FROM edge");
    while (query.next()) d[collegeMap[query.value(0).toString()]][collegeMap[query.value(1).toString()]] = query.value(2).toDouble();
    dijkstra(spinBoxMax + 1);
    tableViewUpdate();
}

/* ==== TripPlanner updateTrip() ====================================
    void-returning method used to prepare the model based off the
    selected institutions.
================================================================== */
void TripPlanner::updateTrip() {
    if (ui->starting_location_dropdown_2->currentText() == "Custom Route") {
        QSqlQuery query("INSERT INTO tripRoute (collegeName, collegeNum) SELECT collegeName, collegeNum FROM tripSelected;");
        query.exec("SELECT * FROM tripRoute");
        query.next();
        QString prevName = query.value(0).toString();
        int prevNum = query.value(1).toInt();
        while (query.next()) {
            int i = query.value(1).toInt();
            QSqlQuery query2("UPDATE tripRoute SET distToNext =\"" + QString::number(d[prevNum][i]) + "\" WHERE collegeName =\"" + prevName + "\";");
            prevName = query.value(0).toString();
            prevNum = i;
        }
        query.exec("UPDATE tripRoute SET distToNext = 0 WHERE collegeName =\"" + prevName + "\";");
    } else {
        for (int i = 0; i < 20; ++i) isSelected[i] = false;
        QSqlQuery query("SELECT collegeNum FROM tripSelected");
        while (query.next()) isSelected[query.value(0).toInt()] = true;
        order.clear();
        calculateTrip(start);
    }
}

/* ==== TripPlanner tableViewUpdate() ===============================
    void-returning recursive method used to determine the most efficient
    route.
================================================================== */
void TripPlanner::calculateTrip(int start) {
    if (start == -1) return;

    QString collegeName;
    order.push_back(start);
    isSelected[start] = false;
    int idx = -1;
    double dist = INT_MAX;

    for (int i = 0; i < 20; ++i) {
        if (isSelected[i] && optimal[start][i] < dist) {
            idx = i;
            dist = optimal[start][i];
            }
    }

    QSqlQuery query("SELECT collegeName FROM college WHERE collegeNum =\"" + QString::number(start) + "\"");
    if (query.next()) collegeName = query.value(0).toString();
    dist = dist == INT_MAX ? 0.0 : dist;
    query.exec("INSERT INTO tripRoute (collegeName, collegeNum, routeOrder, distToNext) VALUES (\"" + collegeName + "\", \"" + QString::number(start) + "\", \"" + QString::number(order.size()) + "\", \"" + QString::number(dist) + "\");");
    calculateTrip(idx);
}

/* ==== TripPlanner tableViewUpdate() ===============================
    void-returning method used to update the tables based off the
    selected route.
================================================================== */
void TripPlanner::tableViewUpdate() {
    selectedModel->setQuery("SELECT collegeName, distToNext FROM tripRoute ORDER BY routeOrder");
    ui->selected_route_tableView->setModel(selectedModel);

    availableModel->setQuery("DROP TABLE tripSelected;");
    availableModel->setQuery("CREATE TABLE tripSelected(collegeName TEXT);");
    availableModel->setQuery("INSERT INTO tripSelected (collegeName) SELECT collegeName FROM college;");
    availableModel->setQuery("DELETE FROM tripSelected WHERE EXISTS (SELECT collegeName FROM tripRoute WHERE tripSelected.collegeName = tripRoute.collegeName);");
    availableModel->setQuery("SELECT collegeName FROM tripSelected");
    ui->available_routes_tableView->setModel(availableModel);

    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT distToNext as TOTAL FROM tripRoute) X;");
    if (query.next()) ui->distance_label->setText(QString::number(query.value(0).toDouble(), 'f', 2));
    if (ui->distance_label->text() == "") ui->distance_label->setText("0.00");
}

/* ==== TripPlanner on_available_routes_tableView_clicked() =========
    void-returning method used to update 'name' based off the institution
    selected on the available table.
================================================================== */
void TripPlanner::on_available_routes_tableView_clicked(const QModelIndex &index) {
    ui->selected_route_tableView->clearSelection();
    name = index.siblingAtColumn(0).data().toString();
}

/* ==== TripPlanner on_selected_route_tableView_clicked() ===========
    void-returning method used to update 'name' based off the institution
    selected on the route table.
================================================================== */
void TripPlanner::on_selected_route_tableView_clicked(const QModelIndex &index) {
    ui->available_routes_tableView->clearSelection();
    name = index.siblingAtColumn(0).data().toString();
}

/* ==== TripPlanner on_add_pushButton_clicked() =====================
    void-returning method used to add an institution to the route
    table.
================================================================== */
void TripPlanner::on_add_pushButton_clicked() {
    QSqlQuery query("SELECT collegeName FROM tripSelected WHERE collegeName =\"" + name + "\";");

    if (!query.next()) {
       QMessageBox messageBox;
       messageBox.critical(0,"Invalid Selection","Please Select a Valid Institution!");
       messageBox.setFixedSize(500,200);
       return;
    }

    QString collegeNum;
    query.exec("SELECT collegeNum FROM college WHERE collegeName =\"" + name + "\";");
    if (query.next()) collegeNum = query.value(0).toString();

    query.exec("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) SELECT collegeName, collegeNum FROM tripRoute;");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) VALUES (\"" + name + "\", \"" + collegeNum + "\");");

    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");

    updateTrip();
    tableViewUpdate();
}

/* ==== TripPlanner on_remove_pushButton_clicked() =================
    void-returning method used to remove an institution from the route
    table.
================================================================== */
void TripPlanner::on_remove_pushButton_clicked() {
    QSqlQuery query("SELECT collegeName FROM tripRoute WHERE collegeName =\"" + name + "\";");

    if (!query.next()) {
       QMessageBox messageBox;
       messageBox.critical(0,"Invalid Selection","Please Select a Valid Institution!");
       messageBox.setFixedSize(500,200);
       return;
    }

    query.exec("SELECT collegeNum FROM tripRoute WHERE collegeName =\"" + name + "\";");
    query.next();
    if (query.value(0).toInt() == start) return;

    query.exec("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) SELECT collegeName, collegeNum FROM tripRoute;");
    query.exec("DELETE FROM tripSelected WHERE collegeName =\"" + name + "\";");

    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");

    updateTrip();
    tableViewUpdate();
}

/* ==== TripPlanner on_starting_location_dropdown_currentTextChanged() ==
    void-returning method used to set the starting college to the
    selected institution from the dropdown.
================================================================== */
void TripPlanner::on_starting_location_dropdown_currentTextChanged(const QString &arg1) {
    QSqlQuery query("SELECT collegeNum FROM college WHERE collegeName =\"" + arg1 + "\";");
    if (query.next()) start = query.value(0).toInt();

    query.exec("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");

    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");

    if (ui->starting_location_dropdown_2->currentText() == "Custom Route") {
        QSqlQuery query("INSERT INTO tripSelected (collegeName, collegeNum) VALUES (\"" + arg1 + "\", \"" + QString::number(start) + "\");");
    }

    updateTrip();
    tableViewUpdate();
}

/* ==== TripPlanner on_irvine_pushButton_clicked() ==================
    void-returning method used to set the starting college to University
    of California, Irvine (UCI).
================================================================== */
void TripPlanner::on_irvine_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("University of California, Irvine (UCI)");
    on_starting_location_dropdown_currentTextChanged("University of California, Irvine (UCI)");
}

/* ==== TripPlanner on_arizona_pushButton_clicked() =================
    void-returning method used to set the starting college to Arizona
    State University.
================================================================== */
void TripPlanner::on_arizona_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("Arizona State University");
    on_starting_location_dropdown_currentTextChanged("Arizona State University");
}

/* ==== TripPlanner on_michigan_pushButton_clicked() ================
    void-returning method used to set the starting college to University
    of Michigan.
================================================================== */
void TripPlanner::on_michigan_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("University of  Michigan");
    on_starting_location_dropdown_currentTextChanged("University of  Michigan");
}

/* ==== TripPlanner on_view_auto_select_pushButton_clicked() ========
    void-returning method used to select the closest colleges based
    on the spinBox.
================================================================== */
void TripPlanner::on_view_auto_select_pushButton_clicked() {
    QSqlQuery query("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) SELECT collegeName, collegeNum FROM college;");
    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");
    updateTrip();
    query.exec("DELETE FROM tripRoute WHERE routeOrder > " + QString::number(ui->spinBox->value() + 1) + ";");
    query.exec("UPDATE tripRoute SET distToNext=0 WHERE routeOrder = " + QString::number(ui->spinBox->value() + 1) + ";");
    tableViewUpdate();
}

/* ==== TripPlanner on_school_store_pushButton_clicked() ============
    void-returning method used to move to the graph viewer.
================================================================== */
void TripPlanner::on_pushButton_2_clicked() {
    emit moveToGraphViewer();
}

/* ==== TripPlanner on_school_store_pushButton_clicked() ============
    void-returning method used to move to the school store when a
    school is selected. If no school is selected the method is returned.
================================================================== */
void TripPlanner::on_school_store_pushButton_clicked() {
    if (name == "") return;

    emit moveToSchoolStore(name);
}

/* ==== TripPlanner on_return_home_pushButton_clicked() =============
    void-returning method used to return to the main window.
================================================================== */
void TripPlanner::on_return_home_pushButton_clicked() {
    emit moveToMainWindow();
}

/* ==== TripPlanner dijkstra() ======================================
    void-returning method used to run the adjacency matrix through
    the Dijkstra algo. The distance and path for each city is
    determined and the totals are displayed to screen.
================================================================== */
void TripPlanner::dijkstra(int size) {
    for (int j = 0; j < size; ++j) {
        std::vector<double> dist(size, INT_MAX);
        std::vector<bool> sptSet(size, false);
        std::vector<std::vector<College>> path(12, std::vector<College>());
        dist[j] = 0;

        for (int count = 0; count < size - 1; ++count) {
            int u = minKey(dist, sptSet, size);
            sptSet[u] = true;

            for (int i = 0; i < size; ++i)
                if (!sptSet[i] && d[u][i] && dist[u] != INT_MAX && dist[u] + d[u][i] < dist[i]) dist[i] = dist[u] + d[u][i];
        }
        for (int i = 0; i < size; ++i) optimal[j][i] = dist[i];
    }
}

/* ==== TripPlanner minKey() ========================================
    int-returning method used by both the dijkstra and mst algorithms
    respectively. Used to determine the next minimum index.
================================================================== */
int TripPlanner::minKey(std::vector<double> key, std::vector<bool> set, int size) {
    int min = INT_MAX;
    int minIdx = 0;

    for (int i = 0; i < size; ++i) {
        if (!set[i] && key[i] < min) {
            min = key[i];
            minIdx = i;
        }
    }
    return minIdx;
}

/* ==== TripPlanner on_starting_location_dropdown_2_currentTextChanged() ==
    void-returning method called when the route dropdown is changed,
    disables the closest college route frame when Custom Route is
    selected.
================================================================== */
void TripPlanner::on_starting_location_dropdown_2_currentTextChanged(const QString &arg1) {
    ui->populate_frame->setDisabled(arg1 == "Custom Route" ? true : false);
    updateTrip();
}
