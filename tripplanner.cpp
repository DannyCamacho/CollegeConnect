#include "tripplanner.h"
#include "ui_tripplanner.h"

tripplanner::tripplanner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tripplanner)
{
    ui->setupUi(this);
}

tripplanner::~tripplanner()
{
    delete ui;
}
