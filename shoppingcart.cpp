#include "shoppingcart.h"
#include "ui_shoppingcart.h"
#include "mainwindow.h"

ShoppingCart::ShoppingCart(QWidget *parent) : QMainWindow(parent), ui(new Ui::ShoppingCart) {
    ui->setupUi(this);
    cartModel = new QSqlQueryModel;
    totModel = new QSqlQueryModel;
    cartTableViewUpdate();
    connect(parent, SIGNAL(updateShoppingCart()), this, SLOT(updateShoppingCart()));
}

ShoppingCart::~ShoppingCart() {
    delete ui;
    delete cartModel;
    delete totModel;
}

void ShoppingCart::cartTableViewUpdate() {
    cartModel->setQuery("SELECT collegeName, souvenirItem, souvenirPrice, quantity FROM cart ORDER BY collegeName");
    ui->shopping_cart_TableView->setModel(cartModel);
    totTableViewUpdate();
    calculateTotal();
}

void ShoppingCart::totTableViewUpdate() {
    totModel->setQuery("SELECT collegeName, '$' || printf(\"%.2f\", SUM(TRIM(souvenirPrice, '$') * quantity)) FROM cart GROUP BY collegeName");
    ui->total_TableView->setModel(totModel);
}

void ShoppingCart::calculateTotal() {
    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT TRIM(souvenirPrice, '$') * quantity as TOTAL FROM cart) X;");
    if (query.next()) ui->total_amount->setText(QString::number(query.value(0).toDouble(), 'f', 2));
    if (ui->total_amount->text() == "") ui->total_amount->setText("0.00");
}

void ShoppingCart::on_shopping_cart_TableView_clicked(const QModelIndex &index) {
    collegeName = index.siblingAtColumn(0).data().toString();
    item = index.siblingAtColumn(1).data().toString();
}

void ShoppingCart::on_empty_cart_pushButton_clicked() {
    QSqlQuery query("DROP TABLE cart;");
    item = collegeName = "";
    ui->total_amount->setText("0.00");
    query.exec("CREATE TABLE cart (collegeName TEXT, collegeNum INTEGER, souvenirItem TEXT, souvenirPrice INTEGER, quantity INTEGER);");
    cartTableViewUpdate();
}

void ShoppingCart::on_remove_item_pushButton_clicked() {
    if (item == "" || collegeName == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Deletion","Please Select a Souvenir!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QSqlQuery query("DELETE FROM cart WHERE souvenirItem =\"" + item + "\" AND collegeName =\"" + collegeName + "\";");
    item = collegeName = "";
    cartTableViewUpdate();
}

void ShoppingCart::on_update_pushButton_clicked() {
    if (item == "" || collegeName == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Update Quantity","Please Select a Souvenir!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QSqlQuery query("UPDATE cart SET quantity =\"" + ui->spinBox->text() + "\" WHERE collegeName =\"" + collegeName + "\" AND souvenirItem = \"" + item + "\";");
    if (ui->spinBox->text().toInt() == 0) on_remove_item_pushButton_clicked();
    else { item = collegeName = ""; cartTableViewUpdate(); }
}

void ShoppingCart::on_pushButton_6_clicked() {
    emit moveToSchoolStore();
}

void ShoppingCart::updateShoppingCart() {
    cartTableViewUpdate();
}

void ShoppingCart::on_route_pushButton_clicked() {
    emit moveToTripPlanner();
}

