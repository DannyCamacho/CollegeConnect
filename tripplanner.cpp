#include "tripplanner.h"
#include "ui_tripplanner.h"

TripPlanner::TripPlanner(QWidget *parent) : QMainWindow(parent), ui(new Ui::TripPlanner) {
    ui->setupUi(this);
    availableModel = new QSqlQueryModel;
    selectedModel = new QSqlQueryModel;
    start = 4;
    spinBoxMax = 0;
    connect(parent, SIGNAL(updateTripPlanner()), this, SLOT(populateWindow()));
    connect(this, SIGNAL(moveToSchoolStore(QString)), parent, SLOT(moveToSchoolStoreFromTrip(QString)));
    connect(this, SIGNAL(moveToShoppingCart()), parent, SLOT(moveToShoppingCart()));
}

TripPlanner::~TripPlanner() {
    delete ui;
    delete availableModel;
    delete selectedModel;
}

void TripPlanner::populateWindow() {
    spinBoxMax = 0;
    std::map<QString, int> collegeMap;
    ui->starting_location_dropdown->clear();
    QSqlQuery query("SELECT collegeName, collegeNum FROM college");
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
    tableViewUpdate();
    updateQuantity();
}

void TripPlanner::updateTrip() {
    for (int i = 0; i < 20; ++i) isSelected[i] = false;
    QSqlQuery query("SELECT collegeNum FROM tripSelected");
    while (query.next()) isSelected[query.value(0).toInt()] = true;
    order.clear();
    calculateTrip(start);
}

void TripPlanner::calculateTrip(int start) {
    if (start == -1) return;

    QString collegeName;
    order.push_back(start);
    isSelected[start] = false;
    int idx = -1;
    double dist = INT_MAX;

    for (int i = 0; i < 20; ++i) {
        if (isSelected[i] && d[start][i] < dist) {
            idx = i;
            dist = d[start][i];
            }
    }

    QSqlQuery query("SELECT collegeName FROM college WHERE collegeNum =\"" + QString::number(start) + "\"");
    if (query.next()) collegeName = query.value(0).toString();
    dist = dist == INT_MAX ? 0.0 : dist;
    query.exec("INSERT INTO tripRoute (collegeName, collegeNum, routeOrder, distToNext) VALUES (\"" + collegeName + "\", \"" + QString::number(start) + "\", \"" + QString::number(order.size()) + "\", \"" + QString::number(dist) + "\");");
    calculateTrip(idx);
}

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

void TripPlanner::updateQuantity() {
    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT quantity as TOTAL FROM cart) X;");
    if (query.next()) ui->cart_quantity_display->setText(query.value(0).toInt() == 0 ? "0" : query.value(0).toString());
}

void TripPlanner::on_available_routes_tableView_clicked(const QModelIndex &index) {
    ui->selected_route_tableView->clearSelection();
    name = index.siblingAtColumn(0).data().toString();
}

void TripPlanner::on_selected_route_tableView_clicked(const QModelIndex &index) {
    ui->available_routes_tableView->clearSelection();
    name = index.siblingAtColumn(0).data().toString();
}

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

void TripPlanner::on_remove_pushButton_clicked() {
    QSqlQuery query("SELECT collegeName FROM tripRoute WHERE collegeName =\"" + name + "\";");

    if (!query.next()) {
       QMessageBox messageBox;
       messageBox.critical(0,"Invalid Selection","Please Select a Valid Institution!");
       messageBox.setFixedSize(500,200);
       return;
    }

    query.exec("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) SELECT collegeName, collegeNum FROM tripRoute;");
    query.exec("DELETE FROM tripSelected WHERE collegeName =\"" + name + "\";");

    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");

    updateTrip();
    tableViewUpdate();
}

void TripPlanner::on_starting_location_dropdown_currentTextChanged(const QString &arg1) {
    QSqlQuery query("SELECT collegeNum FROM college WHERE collegeName =\"" + arg1 + "\";");
    if (query.next()) start = query.value(0).toInt();

    query.exec("DROP TABLE tripSelected;");
    query.exec("CREATE TABLE tripSelected (collegeName TEXT, collegeNum INTEGER);");
    query.exec("INSERT INTO tripSelected (collegeName, collegeNum) SELECT collegeName, collegeNum FROM tripRoute;");

    query.exec("DROP TABLE tripRoute;");
    query.exec("CREATE TABLE tripRoute (collegeName TEXT, collegeNum INTEGER, routeOrder INTEGER, distToNext INTEGER);");

    updateTrip();
    tableViewUpdate();
}

void TripPlanner::on_irvine_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("University of California, Irvine (UCI)");
    on_starting_location_dropdown_currentTextChanged("University of California, Irvine (UCI)");
}

void TripPlanner::on_arizona_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("Arizona State University");
    on_starting_location_dropdown_currentTextChanged("Arizona State University");
}

void TripPlanner::on_michigan_pushButton_clicked() {
    ui->starting_location_dropdown->setCurrentText("University of  Michigan");
    on_starting_location_dropdown_currentTextChanged("University of  Michigan");
}

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

void TripPlanner::on_pushButton_2_clicked() {
    emit moveToGraphViewer();
}

void TripPlanner::on_school_store_pushButton_clicked() {
    if (name == "") return;

    emit moveToSchoolStore(name);
}

void TripPlanner::on_shopping_cart_pushButton_clicked() {
    emit moveToShoppingCart();
}

void TripPlanner::on_return_home_pushButton_clicked() {
    emit moveToMainWindow();
}

