#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include "database.h"

namespace Ui { class AdminMenu; }
class AdminMenu : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminMenu(QWidget *parent = nullptr);
    ~AdminMenu();
    void schoolTableViewUpdate();
    void souvenirTableViewUpdate();

private slots:

    void on_university_tableView_clicked(const QModelIndex &index);

private:
    Ui::AdminMenu *ui;
    QSqlQueryModel* schoolModel;
    QSqlQueryModel* souvenirModel;
    Database database;
    QString collegeName;
    QString item;
};

#endif // ADMINMENU_H
