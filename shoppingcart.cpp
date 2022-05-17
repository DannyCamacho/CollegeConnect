#include "shoppingcart.h"
#include "ui_shoppingcart.h"
#include "mainwindow.h"

/* ==== ShoppingCart Constructor ====================================
    Constructor used to initialized necessary variables, update the
    cart model, and connect the widget change buttons to the main
    window.
================================================================== */
ShoppingCart::ShoppingCart(QWidget *parent) : QMainWindow(parent), ui(new Ui::ShoppingCart) {
    ui->setupUi(this);
    cartModel = new QSqlQueryModel;
    totModel = new QSqlQueryModel;
    cartTableViewUpdate();
    connect(parent, SIGNAL(updateShoppingCart()), this, SLOT(updateShoppingCart()));
}

/* ==== ShoppingCart Destructor =====================================
    Destructor used to delete heap allocated memory.
================================================================== */
ShoppingCart::~ShoppingCart() {
    delete ui;
    delete cartModel;
    delete totModel;
}

/* ==== ShoppingCart cartTableViewUpdate() ==========================
    void-returning method used to update the cart when changes are made.
================================================================== */
void ShoppingCart::cartTableViewUpdate() {
    cartModel->setQuery("SELECT collegeName, souvenirItem, souvenirPrice, quantity FROM cart ORDER BY collegeName");
    ui->shopping_cart_TableView->setModel(cartModel);
    totTableViewUpdate();
    calculateTotal();
}

/* ==== ShoppingCart totTableViewUpdate() ===========================
    void-returning method used to calculate the total for each college
    and display result as the totModel table.
================================================================== */
void ShoppingCart::totTableViewUpdate() {
    totModel->setQuery("SELECT collegeName, '$' || printf(\"%.2f\", SUM(TRIM(souvenirPrice, '$') * quantity)) FROM cart GROUP BY collegeName");
    ui->total_TableView->setModel(totModel);
}

/* ==== ShoppingCart calculateTotal() ===============================
    void-returning method used to calculate the total for all items
    within the cart table.
================================================================== */
void ShoppingCart::calculateTotal() {
    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT TRIM(souvenirPrice, '$') * quantity as TOTAL FROM cart) X;");
    if (query.next()) ui->total_amount->setText(QString::number(query.value(0).toDouble(), 'f', 2));
    if (ui->total_amount->text() == "") ui->total_amount->setText("0.00");
}

/* ==== ShoppingCart on_shopping_cart_TableView_clicked() ===========
    void-returning method used to update the collegeName and item
    variables when an item is selected from the cart table.
================================================================== */
void ShoppingCart::on_shopping_cart_TableView_clicked(const QModelIndex &index) {
    collegeName = index.siblingAtColumn(0).data().toString();
    item = index.siblingAtColumn(1).data().toString();
}

/* ==== ShoppingCart on_empty_cart_pushButton_clicked() =============
    void-returning method used to remove all items from the cart.
================================================================== */
void ShoppingCart::on_empty_cart_pushButton_clicked() {
    QSqlQuery query("DROP TABLE cart;");
    item = collegeName = "";
    ui->total_amount->setText("0.00");
    query.exec("CREATE TABLE cart (collegeName TEXT, collegeNum INTEGER, souvenirItem TEXT, souvenirPrice INTEGER, quantity INTEGER);");
    cartTableViewUpdate();
}

/* ==== ShoppingCart on_remove_item_pushButton_clicked() ============
    void-returning method used to remove a selected item. If no item is
    selected, a warning message pop-up is displayed.
================================================================== */
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

/* ==== ShoppingCart on_update_pushButton_clicked() =================
    void-returning method used to update the quantity of a selected item.
    If no item is selected, a warning message pop-up is displayed.
================================================================== */
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

/* ==== ShoppingCart on_pushButton_6_clicked() ======================
    void-returning method used to move to the school store.
================================================================== */
void ShoppingCart::on_pushButton_6_clicked() {
    emit moveToSchoolStore();
}

/* ==== ShoppingCart updateShoppingCart() ===========================
    void-returning method used to update the cart when moving to the
    cart from another widget.
================================================================== */
void ShoppingCart::updateShoppingCart() {
    cartTableViewUpdate();
}

/* ==== ShoppingCart on_route_pushButton_clicked() ==================
    void-returning method used to move to the trip planner.
================================================================== */
void ShoppingCart::on_route_pushButton_clicked() {
    emit moveToTripPlanner();
}
