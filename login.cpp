#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

/* ==== Login::Constructor ==========================================
    Constructor used to connect to the existing MainWindow UI.
================================================================== */
Login::Login(QWidget *parent) : QDialog(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    connect(this, SIGNAL(sendMessage(QString)), parent, SLOT(receiveMessage(QString)));
}

/* ==== Login::Destructor ===========================================
    Destructor used to delete Login UI.
================================================================== */
Login::~Login() {
    delete ui;
}

/* ==== Login::on_pushButtonLogin_clicked ===========================
    Login Button: Checks that both username and password match the
    predetermined QStrings, if both match then the MainWindow UI is
    notified and the Login UI is deleted. If the input strings do
    not match, the a warning popup is shown to the user.
================================================================== */
void Login::on_pushButtonLogin_clicked() {
    QString username = ui->lineEditUsername->text();
    QString password = ui->lineEditPassword->text();

    if (username == "admin" && password == "password") {
        emit sendMessage("Admin Logged in");
        hide();
        delete ui;
    } else QMessageBox::warning(this, "Login", "Incorrect username or password");
}

/* ==== Login::on_pushButtonCancel_clicked ==========================
    Cancel Button: hides and deletes the Login UI.
================================================================== */
void Login::on_pushButtonCancel_clicked() {
    hide();
    delete ui;
}

/* ==== Login::on_lineEditPassword_returnPressed ====================
    Return key pressed on password line edit: calls the pushButton
    clicked method.
================================================================== */
void Login::on_lineEditPassword_returnPressed() {
    on_pushButtonLogin_clicked();
}
