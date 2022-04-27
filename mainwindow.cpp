#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), collegeMap(20) {
    ui->setupUi(this);
    order = "collegeName ASC";
    schoolModel = new QSqlQueryModel;
    schoolDetailModel = new QSqlQueryModel;
    database.populateColleges("../CollegeConnect/Distances.csv");
    database.populateSouvenirs("../CollegeConnect/Souvenirs.csv");
    populateWindow();
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

    schoolModel->setQuery("SELECT collegeName, state FROM college ORDER BY " + order);
    ui->school_list_tableView->setModel(schoolModel);
    ui->toggle_name_order_descending->setVisible(false);
    ui->toggle_state_order_descending->setVisible(false);
    ui->select_state->setCurrentText("All States");
}

void  MainWindow::schoolTableUpdate() {
    QString state = ui->select_state->currentText() == "All States" ? "" : "WHERE state=\"" +  ui->select_state->currentText() + "\"";
    schoolModel->setQuery("SELECT collegeName, state FROM college " + state + " ORDER BY " + order);
    ui->school_list_tableView->setModel(schoolModel);
}

void MainWindow::on_select_state_currentTextChanged(const QString &arg1) {
    schoolTableUpdate();
}

void MainWindow::on_school_list_tableView_clicked(const QModelIndex &index) {
    QSqlQuery query("SELECT collegeNum FROM college WHERE collegeName=\"" +  index.siblingAtColumn(0).data().toString() + "\"");
    query.next();
    ui->college_name_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).collegeName));
    ui->state_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).state));
    ui->undergrad_label->setText(QString::number(collegeMap.at(query.value(0).toInt()).numsOfGrad));
}

void MainWindow::on_toggle_name_order_ascending_clicked() {
    order = "collegeName ASC";
    ui->toggle_name_order_ascending->setVisible(false);
    ui->toggle_name_order_descending->setVisible(true);
    schoolTableUpdate();
}

void MainWindow::on_toggle_name_order_descending_clicked() {
    order = "collegeName DESC";
    ui->toggle_name_order_descending->setVisible(false);
    ui->toggle_name_order_ascending->setVisible(true);
    schoolTableUpdate();
}

void MainWindow::on_toggle_state_order_ascending_clicked() {
    order = "state ASC";
    ui->toggle_state_order_ascending->setVisible(false);
    ui->toggle_state_order_descending->setVisible(true);
    schoolTableUpdate();
}

void MainWindow::on_toggle_state_order_descending_clicked() {
    order = "state DESC";
    ui->toggle_state_order_descending->setVisible(false);
    ui->toggle_state_order_ascending->setVisible(true);
    schoolTableUpdate();
}

void MainWindow::on_actionLogin_triggered() {

}

