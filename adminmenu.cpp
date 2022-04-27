#include "adminmenu.h"
#include "ui_adminmenu.h"

AdminMenu::AdminMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::AdminMenu) {
    ui->setupUi(this);
    schoolModel = new QSqlQueryModel();
    souvenirModel = new QSqlQueryModel();
    schoolTableViewUpdate();
}

AdminMenu::~AdminMenu() {
    delete ui;
}

void AdminMenu::schoolTableViewUpdate() {
    schoolModel->setQuery("SELECT collegeName FROM college ORDER BY collgeName");
    ui->rest_tableView->setModel(schoolModel);
}

void AdminMenu::souvenirTableViewUpdate() {
    souvenirModel->setQuery("SELECT item, price FROM souvenir WHERE collegeName =\"" + collegeName + "\"");
    ui->menu_tableView->setModel(souvenirModel);
}

void AdminMenu::on_rest_tableView_clicked(const QModelIndex &index) {
        collegeName = index.siblingAtColumn(0).data().toString();
        souvenirModel->setQuery("SELECT item, price FROM souvenir WHERE collegeName =\"" + collegeName + "\"");
        ui->menu_tableView->setModel(souvenirModel);
        ui->lineEditItemName->setText("");
        ui->lineEditItemPrice->setText("");
        item = "";
}

