#include "adminmenu.h"
#include "ui_adminmenu.h"

/* ==== AdminMenu Constructor =======================================
    Constructor used to initialized necessary variables and update the
    window.
================================================================== */
AdminMenu::AdminMenu(QWidget *parent) : QMainWindow(parent), ui(new Ui::AdminMenu) {
    ui->setupUi(this);
    schoolModel = new QSqlQueryModel();
    souvenirModel = new QSqlQueryModel();
    schoolTableViewUpdate();
}

/* ==== AdminMenu Destructor ========================================
    Destructor used to delete heap allocated memory.
================================================================== */
AdminMenu::~AdminMenu() {
    delete ui;
    delete schoolModel;
    delete souvenirModel;
}

/* ==== AdminMenu schoolTableViewUpdate() ===========================
    void-returning method used to update the schoolModel based off
    the existing colleges.
================================================================== */
void AdminMenu::schoolTableViewUpdate() {
    schoolModel->setQuery("SELECT collegeName FROM college ORDER BY collegeName");
    ui->university_tableView->setModel(schoolModel);
}

/* ==== AdminMenu souvenirTableViewUpdate() =========================
    void-returning method used to update the souvenir table based off
    the selected college.
================================================================== */
void AdminMenu::souvenirTableViewUpdate() {
    souvenirModel->setQuery("SELECT item, price FROM souvenir WHERE collegeName =\"" + collegeName + "\"");
    ui->items_tableView->setModel(souvenirModel);
}

/* ==== AdminMenu on_university_tableView_clicked() =================
    void-returning method used to populate souvenir table when a
    college is selected, resets the souvenir lineEdits.
================================================================== */
void AdminMenu::on_university_tableView_clicked(const QModelIndex &index) {
            collegeName = index.siblingAtColumn(0).data().toString();
            souvenirModel->setQuery("SELECT item, price FROM souvenir WHERE collegeName =\"" + collegeName + "\"");
            ui->items_tableView->setModel(souvenirModel);
            ui->shop_item_lineEdit->setText("");
            ui->price_lineEdit->setText("");
            item = "";
}

/* ==== AdminMenu on_add_button_clicked() ===========================
    void-returning method used to add a new souvenir to the selected
    college. A warning popup is displayed if the input is not valid.
================================================================== */
void AdminMenu::on_add_button_clicked() {
    if (collegeName == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Addition","Please Select a University!");
        messageBox.setFixedSize(500,200);
        return;
    }

    item = ui->shop_item_lineEdit->text();
    QSqlQuery query("SELECT * FROM souvenir WHERE item =\"" + item + "\" AND collegeName =\"" + collegeName + "\";");

    if (query.next()) {
       QMessageBox messageBox;
       messageBox.critical(0,"Duplicate Souvenir","Souvenir already exists!");
       messageBox.setFixedSize(500,200);
       return;
    }

    if (item == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Addition","Please Input a Souvenir Name!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString price = ui->price_lineEdit->text();
    QRegularExpression re("^([0-9]{1,3},([0-9]{3},)*[0-9]{3}|[0-9]+)(.[0-9][0-9])?$");
    QRegularExpressionMatch match = re.match(price);
    if (!match.hasMatch()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Price","Please Input a Valid Price!");
        messageBox.setFixedSize(500,200);
        return;
    }

    query.exec("SELECT collegeNum from college WHERE collegeName =\"" + collegeName + "\";");
    query.next();
    query.exec("INSERT INTO souvenir (collegeName, collegeNum, item, price) VALUES (\"" + collegeName + "\", \"" + QString::number(query.value(0).toInt()) + "\", \"" + item + "\", \"" + price + "\");");
    item = "";
    souvenirTableViewUpdate();
}

/* ==== AdminMenu on_remove_button_clicked() ========================
    void-returning method used to remove an existing souvenir from the
    selected college. A warning popup is displayed if the input is not
    valid.
================================================================== */
void AdminMenu::on_remove_button_clicked() {
    if (item == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Deletion","Please Select a Souvenir!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QSqlQuery query("DELETE FROM souvenir WHERE item =\"" + item + "\" AND collegeName =\"" + collegeName + "\";");
    ui->shop_item_lineEdit->setText("");
    ui->price_lineEdit->setText("");
    item = "";
    souvenirTableViewUpdate();
}

/* ==== AdminMenu on_save_changes_button_clicked() ==================
    void-returning method used to edit an existing souvenir from the
    selected college. A warning popup is displayed if the input is not
    valid.
================================================================== */
void AdminMenu::on_save_changes_button_clicked() {
    if (collegeName == "") {
            QMessageBox messageBox;
            messageBox.critical(0,"Invalid Edit","Please Select a University!");
            messageBox.setFixedSize(500,200);
            return;
    }

    QString newItem = ui->shop_item_lineEdit->text();
    QString price = ui->price_lineEdit->text();

    if (newItem == "") {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Edit","Please Input an Souvenir Name!");
        messageBox.setFixedSize(500,200);
        return;
    }

    QSqlQuery query("UPDATE souvenir SET item =\"TEMPORARYITEMNAME\", price =\"" + price + "\" WHERE collegeName =\"" + collegeName + "\" AND item = \"" + item + "\";");
    query.exec("SELECT * FROM souvenir WHERE item =\"" + item + "\" AND collegeName =\"" + collegeName + "\";");

    if (query.next()) {
       QMessageBox messageBox;
       messageBox.critical(0,"Duplicate Souvenir","Souvenir already exists!");
       messageBox.setFixedSize(500,200);
       query.exec("UPDATE souvenir SET item =\"" + item + "\", price =\"" + price + "\" WHERE collegeName =\"" + collegeName + "\" AND item = \"TEMPORARYITEMNAME\";");
       return;
    }

    QRegularExpression re("^([0-9]{1,3},([0-9]{3},)*[0-9]{3}|[0-9]+)(.[0-9][0-9])?$");
    QRegularExpressionMatch match = re.match(price);

    if (!match.hasMatch()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Invalid Price","Please Input a Valid Price!");
        messageBox.setFixedSize(500,200);
        return;
    }
    query.exec("UPDATE souvenir SET item =\"" + newItem + "\", price =\"" + price + "\" WHERE collegeName =\"" + collegeName + "\" AND item = \"TEMPORARYITEMNAME\";");
    item = "";
    souvenirTableViewUpdate();
}

/* ==== AdminMenu on_items_tableView_clicked() ======================
    void-returning method used to populate the souvenir lineEdits
    when a souvenir is selected.
================================================================== */
void AdminMenu::on_items_tableView_clicked(const QModelIndex &index) {
    item = index.siblingAtColumn(0).data().toString();
    ui->shop_item_lineEdit->setText(index.siblingAtColumn(0).data().toString());
    ui->price_lineEdit->setText(index.siblingAtColumn(1).data().toString());
}

/* ==== AdminMenu on_actionLogout_triggered() =======================
    void-returning method used to move to the main window.
================================================================== */
void AdminMenu::on_actionLogout_triggered(){
    emit adminLogout();
}

/* ==== AdminMenu on_actionImport_University_triggered() ============
    void-returning method used to import new universities using a
    selected file. A file selection window popup is displayed and
    the selected path is passed to the Database's importCollege
    method.
================================================================== */
void AdminMenu::on_actionImport_University_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Select University Import File");
    fileName = filename.toStdString();
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot import file: " + file.errorString());
        return;
    }
    database.importColleges(fileName);
    schoolTableViewUpdate();
}

/* ==== AdminMenu on_actionImport_Souveners_triggered() =============
    void-returning method used to import new souvenirs using a
    selected file. A file selection window popup is displayed and
    the selected path is passed to the Database's importSouvenir
    method.
================================================================== */
void AdminMenu::on_actionImport_Souveners_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Select Souvenir Import File");
    fileName = filename.toStdString();
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot import file: " + file.errorString());
        return;
    }
    database.importSouvenirs(fileName);
    schoolTableViewUpdate();
}
