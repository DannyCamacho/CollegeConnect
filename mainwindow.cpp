#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), collegeMap(20) {
    ui->setupUi(this);
    order = "collegeName ASC";
    schoolModel = new QSqlQueryModel;
    populateWindow();
    adminMenu = new AdminMenu(this);
    schoolStore = new SchoolStore(this);
    shoppingCart = new ShoppingCart(this);
    tripPlanner = new TripPlanner(this);
    ui->main_stackedWidget->insertWidget(1, adminMenu);
    ui->main_stackedWidget->insertWidget(2, schoolStore);
    ui->main_stackedWidget->insertWidget(3, shoppingCart);
    ui->main_stackedWidget->insertWidget(4, tripPlanner);
    connect(adminMenu, SIGNAL(adminLogout()), this, SLOT(returnToMainWindow()));
    connect(schoolStore, SIGNAL(leaveSchoolStore()), this, SLOT(returnToMainWindow()));
    connect(schoolStore, SIGNAL(moveToShoppingCart()), this, SLOT(moveToShoppingCart()));
    connect(shoppingCart, SIGNAL(moveToSchoolStore()), this, SLOT(moveToSchoolStore()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete schoolModel;
    delete adminMenu;
    delete schoolStore;
    delete shoppingCart;
}

void MainWindow::populateWindow() {
    QSqlQuery query("SELECT * FROM college");
    while (query.next())
        if (collegeMap.at(query.value(1).toInt()).collegeName == "")
            collegeMap.insert({ query.value(1).toInt(), query.value(0).toString().toStdString(),
                                query.value(2).toString().toStdString(), query.value(3).toInt() });

    query.exec("SELECT SUM(X.TOTAL) FROM (SELECT undergrads as TOTAL FROM college) X;");
    if (query.next()) ui->undergrad_total_display->setText(QString::number(query.value(0).toInt()));

    ui->select_state->clear();
    ui->select_state->addItem("All States");
    query.exec("SELECT DISTINCT state FROM college ORDER BY state ASC");
    while (query.next()) ui->select_state->addItem(query.value(0).toString());

    schoolModel->setQuery("SELECT collegeName, state FROM college ORDER BY " + order);
    ui->school_list_tableView->setModel(schoolModel);
    ui->toggle_name_order_ascending->setVisible(false);
    ui->toggle_state_order_ascending->setVisible(false);
    ui->select_state->setCurrentText("All States");
}

void  MainWindow::schoolTableUpdate() {
    QString state = ui->select_state->currentText() == "All States" ? "" : "WHERE state=\"" +  ui->select_state->currentText() + "\"";
    schoolModel->setQuery("SELECT collegeName, state FROM college " + state + " ORDER BY " + order);
    ui->school_list_tableView->setModel(schoolModel);
}

void MainWindow::on_select_state_currentTextChanged(const QString &arg1) {
    schoolTableUpdate();
    collegeName = "";
}

void MainWindow::on_school_list_tableView_clicked(const QModelIndex &index) {
    QSqlQuery query("SELECT collegeNum FROM college WHERE collegeName=\"" +  index.siblingAtColumn(0).data().toString() + "\"");
    query.next();
    ui->college_name_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).collegeName));
    ui->state_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).state));
    ui->undergrad_label->setText(QString::number(collegeMap.at(query.value(0).toInt()).numsOfGrad));
    collegeName = index.siblingAtColumn(0).data().toString();
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
    Login* login = new Login(this);
    login->show();
}

void MainWindow::receiveMessage(const QString &msg) {
    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(1);
}

void MainWindow::returnToMainWindow() {
    ui->menuBar->setVisible(true);
    ui->main_stackedWidget->setCurrentIndex(0);
    populateWindow();
    schoolTableUpdate();
    collegeName = "";
}

void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}

void MainWindow::on_visit_store_button_clicked() {
    if (collegeName == "") return;

    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(2);
    emit updateSchoolStore(collegeName);
}

void MainWindow::moveToShoppingCart() {
    ui->main_stackedWidget->setCurrentIndex(3);
    emit updateShoppingCart();
}

void MainWindow::moveToSchoolStore() {
    ui->main_stackedWidget->setCurrentIndex(2);
    emit updateSchoolStore(collegeName);
}

void MainWindow::on_plan_route_button_clicked() {
    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(4);
}
