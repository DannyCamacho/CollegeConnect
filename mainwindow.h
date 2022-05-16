#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "map.h"
#include "adminmenu.h"
#include "login.h"
#include "schoolstore.h"
#include "shoppingcart.h"
#include "tripplanner.h"
#include "graphviewer.h"

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

public slots:
    void receiveMessage(const QString &msg);
    void moveToShoppingCart();
    void moveToSchoolStore();
    void moveToGraphViewer();
    void moveToTripPlanner();
    void moveToSchoolStoreFromTrip(const QString name);

private slots:
    void on_select_state_currentTextChanged(const QString &arg1);
    void on_school_list_tableView_clicked(const QModelIndex &index);
    void on_toggle_name_order_ascending_clicked();
    void on_toggle_name_order_descending_clicked();
    void on_toggle_state_order_ascending_clicked();
    void on_toggle_state_order_descending_clicked();
    void on_actionLogin_triggered();
    void returnToMainWindow();
    void on_actionQuit_triggered();
    void on_visit_store_button_clicked();
    void on_plan_route_button_clicked();

signals:
    void updateSchoolStore(QString collegeName);
    void updateShoppingCart();
    void updateTripPlanner();
    void updateGraphViewer();
    void updateTripPlannerQuantity();

private:
    Ui::MainWindow *ui;
    AdminMenu* adminMenu;
    SchoolStore* schoolStore;
    ShoppingCart* shoppingCart;
    TripPlanner* tripPlanner;
    GraphViewer* graphViewer;
    QSqlQueryModel* schoolModel;
    Database database;
    QString collegeName;
    QString order;
    DoubleHashMap collegeMap;
};
#endif // MAINWINDOW_H
