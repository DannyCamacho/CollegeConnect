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
    connect(parent, SIGNAL(updateSchoolStore(QString)), this, SLOT(getCollegeName(QString)));
}


/* ==== SchoolStore::Destructor ====================================
    Destructor used to delete SQLQueryModels schoolStoreModel and
    and the MainWindow UI.
================================================================== */
SchoolStore::~SchoolStore() {
    delete ui;
    delete schoolStoreModel;
}


/* ==== SchoolStore::schoolStoreTableViewUpdate =====================
    Updates the school_store tableview.
================================================================== */
void SchoolStore::schoolStoreTableViewUpdate() {
    schoolStoreModel->setQuery("SELECT item, price FROM souvenir WHERE collegeName =\"" + collegeName + "\"");
    ui->school_store_tableView->setModel(schoolStoreModel);
}


/* ==== SchoolStore::on_initial_list_pushButton_clicked =============
    Proceed back to initial list: hides and deletes SchoolStore UI and
    heads back to main window.
================================================================== */
void SchoolStore::on_initial_list_pushButton_clicked() {
    emit leaveSchoolStore();
}


/* ==== SchoolStore::on_school_store_tableView_clicked ==============
    Updates souvenirItem and souvenirPrice when a menu item is selected from
    school_store tableview. index is the selected row of the tableview, with
    0 being the name of the item and 1 being the price.
================================================================== */
void SchoolStore::on_school_store_tableView_clicked(const QModelIndex &index) {
    souvenirItem = index.siblingAtColumn(0).data().toString();
    souvenirPrice = index.siblingAtColumn(1).data().toString();
}


/* ==== SchoolStore::on_add_to_cart_pushButton_clicked ==============
    Add_To_cart Button: Adds selected souvenir item and quantity to
    cart database. When an item is not selected, the method is returned,
    the quantity is determined by the spinBox. If the resulting quantity
    is greater than 100, the method is returned, else the souvenir item
    with the quantity, price, and college name is added to the cart
    or updated if an entry already existed.
================================================================== */
void SchoolStore::on_add_to_cart_pushButton_clicked() {
    if (souvenirItem == "") return;

    int quantity = ui->quantity_spinBox->text().toInt();

    QString collegeName;
    QSqlQuery query("SELECT collegeName FROM souvenir WHERE item=\"" + souvenirItem + "\" AND price =\"" + souvenirPrice + "\"");
    if (query.next()) collegeName = query.value(0).toString();

    QString stringQuery = "INSERT INTO cart (collegeName, souvenirItem, souvenirPrice, quantity) VALUES (\"" + collegeName + "\", \"" + souvenirItem + "\", \"" + souvenirPrice + "\", \"" + QString::number(quantity) + "\");";
    query.exec("SELECT quantity FROM cart where collegeName =\"" + collegeName + "\" AND souvenirItem =\"" + souvenirItem + "\";");

     if (query.next()) {
         quantity += query.value(0).toInt();
         if (quantity > 100) return;
         stringQuery = "UPDATE cart SET collegeName =\"" + collegeName + "\",souvenirItem =\"" + souvenirItem + "\", souvenirPrice =\"" + souvenirPrice + "\", quantity =\"" + QString::number(quantity) + "\" WHERE collegeName =\"" + collegeName + "\" AND souvenirItem =\"" + souvenirItem + "\";";
     }

     query.exec(stringQuery);
     query.exec("SELECT SUM(X.TOTAL) FROM (SELECT quantity as TOTAL FROM cart) X;");
     if (query.next()) ui->cart_quantity_display->setText(query.value(0).toString());
}


/* ==== SchoolStore::getCollegeName =================================
    Method used to catch signal emitted from MainWindow, used to
    update the collegeName used to determine which school souvenirs
    to display.
================================================================== */
void SchoolStore::getCollegeName(const QString &collegeName) {
    this->collegeName = collegeName;
    schoolStoreTableViewUpdate();
}


/* ==== SchoolStore::on_shopping_cart_pushButton_clicked ============
    Method used to send signal to MainWindow to switch to shopping
    cart. Resets souvenirItem to avoid unintentional adding of items
    when returning to school store.
================================================================== */
void SchoolStore::on_shopping_cart_pushButton_clicked() {
    souvenirItem = "";
    emit moveToShoppingCart();
}
