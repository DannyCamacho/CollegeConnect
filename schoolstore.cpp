#include "schoolstore.h"
#include "ui_schoolStore.h"

SchoolStore::SchoolStore(QWidget *parent) : QMainWindow(parent), ui(new Ui::SchoolStore) {
    ui->setupUi(this);
}

SchoolStore::~SchoolStore() {
    delete ui;
}
