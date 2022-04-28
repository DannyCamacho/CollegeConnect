#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
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
    void on_add_button_clicked();
    void on_remove_button_clicked();
    void on_save_changes_button_clicked();
    void on_items_tableView_clicked(const QModelIndex &index);
    void on_actionLogout_triggered();

private:
    Ui::AdminMenu *ui;
    QSqlQueryModel* schoolModel;
    QSqlQueryModel* souvenirModel;
    Database database;
    std::string fileName;
    QString collegeName;
    QString item;
};

#endif // ADMINMENU_H
