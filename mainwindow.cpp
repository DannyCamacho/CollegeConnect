#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), collegeMap(20) {
    ui->setupUi(this);
    schoolModel = new QSqlQueryModel;
    database.populateColleges("../CollegeConnect/Distances.csv");
    database.populateSouvenirs("../CollegeConnect/Souvenirs.csv");
    populateWindow();
    schoolTableUpdate();
    //database.populateColleges("C:/csv/Distances.csv");
    //database.populateSouvenirs("C:/csv/Souvenirs.csv");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::populateWindow() {
    QSqlQuery query("SELECT * FROM college");

    while (query.next())
        collegeMap.insert({ query.value(1).toInt(), query.value(0).toString().toStdString(),
                            query.value(2).toString().toStdString(), query.value(3).toInt() });

    query.exec("SELECT SUM(X.TOTAL) FROM (SELECT undergrads as TOTAL FROM college) X;");
    if (query.next()) ui->undergrad_total_display->setText(QString::number(query.value(0).toInt()));

    ui->select_state->addItem("All States");
    query.exec("SELECT DISTINCT state FROM college ORDER BY state ASC");
    while (query.next()) ui->select_state->addItem(query.value(0).toString());
}

void  MainWindow::schoolTableUpdate() {
    QString state = ui->select_state->currentText() == "All States" ? "" : "WHERE state=\"" +  ui->select_state->currentText() + "\"";
    QString order = "collegeName ASC";
    schoolModel->setQuery("SELECT collegeName, state FROM college " + state + " ORDER BY " + order);
}
