#include "tripplanner.h"
#include "ui_tripplanner.h"

TripPlanner::TripPlanner(QWidget *parent) : QMainWindow(parent), ui(new Ui::TripPlanner) {
    ui->setupUi(this);
    availableModel = new QSqlQueryModel;
    selectedModel = new QSqlQueryModel;
    start = 0;
    populateWindow();
}

TripPlanner::~TripPlanner() {
    delete ui;
    delete availableModel;
    delete selectedModel;
}

void TripPlanner::populateWindow() {
    std::map<QString, int> collegeMap;
    QSqlQuery query("SELECT collegeName, collegeNum FROM college");
    while (query.next()) {
        ui->starting_location_dropdown->addItem(query.value(0).toString());
        collegeMap[query.value(0).toString()] = query.value(1).toInt();
    }

    for (int i = 0; i < 20; ++i)
        for (int j = 0; j < 20; ++j)
            d[i][j] = 0.0;

    query.exec("SELECT * FROM edge");
    while (query.next()) d[collegeMap[query.value(0).toString()]][collegeMap[query.value(1).toString()]] = query.value(2).toDouble();
}

void TripPlanner::updateQuantity() {
    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT quantity as TOTAL FROM cart) X;");
    if (query.next()) ui->cart_quantity_display->setText(query.value(0).toInt() == 0 ? "0" : query.value(0).toString());
}
