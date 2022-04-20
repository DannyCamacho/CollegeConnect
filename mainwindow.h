#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "map.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void populateWindow();
    void schoolTableUpdate();
private slots:
    void on_select_state_currentTextChanged(const QString &arg1);
    void on_school_list_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel* schoolModel;
    QSqlQueryModel* schoolDetailModel;
    Database database;
    QString order;
    DoubleHashMap collegeMap;
};
#endif // MAINWINDOW_H
