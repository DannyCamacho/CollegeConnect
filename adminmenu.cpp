#include "adminmenu.h"
#include "ui_adminmenu.h"

AdminMenu::AdminMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminMenu)
{
    ui->setupUi(this);
}

AdminMenu::~AdminMenu()
{
    delete ui;
}
