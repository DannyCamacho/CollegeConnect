#include "schoolstore.h"
#include "ui_schoolStore.h"
#include "mainwindow.h"


/* ==== SchoolStore::Constructor ===================================
    Constructor used to initialize SQLQueryModels schoolStoreModel and
    and update the schoolStore tableview.
================================================================== */
SchoolStore::SchoolStore(QWidget *parent) : QMainWindow(parent), ui(new Ui::SchoolStore) {
    ui->setupUi(this);
    schoolStoreModel = new QSqlQueryModel;
    schoolStoreTableViewUpdate();
}


/* ==== SchoolStore::Destructor ====================================
    Destructor used to delete SQLQueryModels schoolStoreModel and
    and the MainWindow UI.
================================================================== */
SchoolStore::~SchoolStore() {
    delete ui;
    delete schoolStoreModel;
}


/* ==== SchoolStore::schoolStoreTableViewUpdate ===========================
    Updates the school_store tableview.
================================================================== */
void SchoolStore::schoolStoreTableViewUpdate() {
    schoolStoreModel->setQuery("SELECT * FROM schoolStore");
    ui->school_store_tableView->setModel(schoolStoreModel);
}


/* ==== SchoolStore::on_initial_list_pushButton_clicked =======================
    Proceed back to initial list: hides and deletes SchoolStore UI and
    heads back to main window.
================================================================== */
void SchoolStore::on_initial_list_pushButton_clicked()
{
    //QSqlQuery query("DROP TABLE schoolStore");
    hide();
    delete ui;
    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();
}

