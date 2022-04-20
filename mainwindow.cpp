#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "map.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    Database database;
    //database.populateColleges("../CollegeConnect/Distances.csv");
    //database.populateSouvenirs("../CollegeConnect/Souvenirs.csv");

    database.populateColleges("C:/csv/Distances.csv");
    database.populateSouvenirs("C:/csv/Souvenirs.csv");

}

MainWindow::~MainWindow() {
    delete ui;
}
