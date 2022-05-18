#include "mainwindow.h"
#include "ui_mainwindow.h"

/* ==== MainWindow Constructor ======================================
    Constructor used to initialized necessary variables, populate the
    stackWidget, and connect the widget change buttons to the main
    window.
================================================================== */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), collegeMap(20) {
    ui->setupUi(this);
    order = "collegeName ASC";
    schoolModel = new QSqlQueryModel;
    populateWindow();
    adminMenu = new AdminMenu(this);
    schoolStore = new SchoolStore(this);
    shoppingCart = new ShoppingCart(this);
    tripPlanner = new TripPlanner(this);
    graphViewer = new GraphViewer(this);
    ui->main_stackedWidget->insertWidget(1, adminMenu);
    ui->main_stackedWidget->insertWidget(2, schoolStore);
    ui->main_stackedWidget->insertWidget(3, shoppingCart);
    ui->main_stackedWidget->insertWidget(4, tripPlanner);
    ui->main_stackedWidget->insertWidget(5, graphViewer);
    connect(adminMenu, SIGNAL(adminLogout()), this, SLOT(returnToMainWindow()));
    connect(schoolStore, SIGNAL(leaveSchoolStore()), this, SLOT(returnToMainWindow()));
    connect(schoolStore, SIGNAL(moveToShoppingCart()), this, SLOT(moveToShoppingCart()));
    connect(schoolStore, SIGNAL(moveToTripPlanner()), this, SLOT(moveToTripPlanner()));
    connect(shoppingCart, SIGNAL(moveToSchoolStore()), this, SLOT(moveToSchoolStore()));
    connect(shoppingCart, SIGNAL(moveToTripPlanner()), this, SLOT(moveToTripPlanner()));
    connect(tripPlanner, SIGNAL(moveToGraphViewer()), this, SLOT(moveToGraphViewer()));
    connect(tripPlanner, SIGNAL(moveToMainWindow()), this, SLOT(returnToMainWindow()));
    connect(graphViewer, SIGNAL(moveToTripPlanner()), this, SLOT(moveToTripPlanner()));
}

/* ==== MainWindow Destructor =======================================
    Destructor used to delete heap allocated memory.
================================================================== */
MainWindow::~MainWindow() {
    delete ui;
    delete schoolModel;
    delete adminMenu;
    delete schoolStore;
    delete shoppingCart;
    delete tripPlanner;
    delete graphViewer;
}

/* ==== MainWindow populateWindow() =================================
    void-returning method used to populate the window, initialize the
    custom map, and populate the state dropdown.
================================================================== */
void MainWindow::populateWindow() {
    double distFromSaddleback[20] = { 0 };
    int i = 0;
    QSqlQuery query("SELECT distance FROM edge WHERE collegeName=\"Saddleback College\";");
    while (query.next()) { if (i == 4) ++i; distFromSaddleback[i++] = query.value(0).toDouble(); }

    query.exec("SELECT * FROM college");
    while (query.next())
        if (collegeMap.at(query.value(1).toInt()).collegeName == "")
            collegeMap.insert({ query.value(1).toInt(), query.value(0).toString().toStdString(),
                                query.value(2).toString().toStdString(), query.value(3).toInt(),
                                distFromSaddleback[query.value(1).toInt()] });

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

/* ==== MainWindow schoolTableUpdate() ==============================
    void-returning method used to update the table based off the
    selected state option and ordering.
================================================================== */
void  MainWindow::schoolTableUpdate() {
    QString state = ui->select_state->currentText() == "All States" ? "" : "WHERE state=\"" +  ui->select_state->currentText() + "\"";
    schoolModel->setQuery("SELECT collegeName, state FROM college " + state + " ORDER BY " + order);
    ui->school_list_tableView->setModel(schoolModel);
}

/* ==== MainWindow on_select_state_currentTextChanged() =============
    void-returning method used to update the table and set collegeName
    to null.
================================================================== */
void MainWindow::on_select_state_currentTextChanged(const QString &arg1) {
    schoolTableUpdate();
    collegeName = "";
}

/* ==== MainWindow on_school_list_tableView_clicked() ===============
    void-returning method used to populate the information sidebar
    based off the selected college.
================================================================== */
void MainWindow::on_school_list_tableView_clicked(const QModelIndex &index) {
    QSqlQuery query("SELECT collegeNum FROM college WHERE collegeName=\"" +  index.siblingAtColumn(0).data().toString() + "\"");
    query.next();
    ui->college_name_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).collegeName));
    ui->state_label->setText(QString::fromStdString(collegeMap.at(query.value(0).toInt()).state));
    ui->undergrad_label->setText(QString::number(collegeMap.at(query.value(0).toInt()).numsOfGrad) + " Undergraduates");
    ui->distance_label->setText(QString::number(collegeMap.at(query.value(0).toInt()).distFromSaddleback) + " Miles from Saddleback College");
    collegeName = index.siblingAtColumn(0).data().toString();
}

/* ==== MainWindow on_toggle_name_order_ascending_clicked() =========
    void-returning method used to order the window by collegeName in
    ascending order.
================================================================== */
void MainWindow::on_toggle_name_order_ascending_clicked() {
    order = "collegeName ASC";
    ui->toggle_name_order_ascending->setVisible(false);
    ui->toggle_name_order_descending->setVisible(true);
    schoolTableUpdate();
}

/* ==== MainWindow on_toggle_name_order_descending_clicked() ========
    void-returning method used to order the window by collegeName in
    descending order.
================================================================== */
void MainWindow::on_toggle_name_order_descending_clicked() {
    order = "collegeName DESC";
    ui->toggle_name_order_descending->setVisible(false);
    ui->toggle_name_order_ascending->setVisible(true);
    schoolTableUpdate();
}

/* ==== MainWindow on_toggle_state_order_ascending_clicked() ========
    void-returning method used to order the window by state in
    ascending order.
================================================================== */
void MainWindow::on_toggle_state_order_ascending_clicked() {
    order = "state ASC";
    ui->toggle_state_order_ascending->setVisible(false);
    ui->toggle_state_order_descending->setVisible(true);
    schoolTableUpdate();
}

/* ==== MainWindow on_toggle_state_order_descending_clicked() =======
    void-returning method used to order the window by state in
    descending order.
================================================================== */
void MainWindow::on_toggle_state_order_descending_clicked() {
    order = "state DESC";
    ui->toggle_state_order_descending->setVisible(false);
    ui->toggle_state_order_ascending->setVisible(true);
    schoolTableUpdate();
}

/* ==== MainWindow on_actionLogin_triggered() =======================
    void-returning method used to access the login window popup.
================================================================== */
void MainWindow::on_actionLogin_triggered() {
    Login* login = new Login(this);
    login->show();
}

/* ==== MainWindow receiveMessage() =================================
    void-returning method used to move to the admin menu on successful
    login.
================================================================== */
void MainWindow::receiveMessage(const QString &msg) {
    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(1);
}

/* ==== MainWindow returnToMainWindow() =============================
    void-returning method used by other widgets in the QStackedWidget
    to move to the MainWindow.
================================================================== */
void MainWindow::returnToMainWindow() {
    ui->menuBar->setVisible(true);
    ui->main_stackedWidget->setCurrentIndex(0);
    populateWindow();
    schoolTableUpdate();
    collegeName = "";
}

/* ==== MainWindow on_actionQuit_triggered() ========================
    void-returning method used to close the program.
================================================================== */
void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();
}

/* ==== MainWindow on_visit_store_button_clicked() ==================
    void-returning method used to move to the school store of the
    selected school. If no school is selected, the method is returned.
================================================================== */
void MainWindow::on_visit_store_button_clicked() {
    if (collegeName == "") return;

    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(2);
    emit updateSchoolStore(collegeName);
}

/* ==== MainWindow moveToShoppingCart() =============================
    void-returning method used to move to the shopping cart.
================================================================== */
void MainWindow::moveToShoppingCart() {
    if (collegeName == "") collegeName = "Saddleback College";
    ui->main_stackedWidget->setCurrentIndex(3);
    emit updateShoppingCart();
}

/* ==== MainWindow moveToSchoolStore() ==============================
    void-returning method used to move to the school store from a
    separate widget.
================================================================== */
void MainWindow::moveToSchoolStore() {
    ui->main_stackedWidget->setCurrentIndex(2);
    emit updateSchoolStore(collegeName);
}

/* ==== MainWindow moveToTripPlanner() ==============================
    void-returning method used to move to the trip planner.
================================================================== */
void MainWindow::moveToTripPlanner() {
    ui->main_stackedWidget->setCurrentIndex(4);
}

/* ==== MainWindow on_plan_route_button_clicked() ===================
    void-returning method used to move to the trip planner from
    MainWindow. Reinitializes the distances for changes.
================================================================== */
void MainWindow::on_plan_route_button_clicked() {
    ui->menuBar->setVisible(false);
    ui->main_stackedWidget->setCurrentIndex(4);
    emit updateTripPlanner();
}

/* ==== MainWindow moveToGraphViewer() ==============================
    void-returning method used to move to the graph viewer from a
    separate widget.
================================================================== */
void MainWindow::moveToGraphViewer() {
    ui->main_stackedWidget->setCurrentIndex(5);
    emit updateGraphViewer();
}

/* ==== MainWindow moveToSchoolStoreFromTrip() ======================
    void-returning method used to move to the school store from the
    trip planner using the school selected within the trip planner.
================================================================== */
void MainWindow::moveToSchoolStoreFromTrip(const QString name) {
    collegeName = name;
    moveToSchoolStore();
}
