#ifndef TRIPPLANNER_H
#define TRIPPLANNER_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"

namespace Ui { class TripPlanner; }
class TripPlanner : public QMainWindow {
    Q_OBJECT

public:
    explicit TripPlanner(QWidget *parent = nullptr);
    ~TripPlanner();

signals:
    void moveToMainWindow();
    void moveToGraphViewer();
    void moveToSchoolStore(QString name);
    void moveToShoppingCart();

private slots:
    void populateWindow();
    void updateQuantity();
    void updateTrip();
    void calculateTrip(int start);
    void tableViewUpdate();
    void on_available_routes_tableView_clicked(const QModelIndex &index);
    void on_selected_route_tableView_clicked(const QModelIndex &index);
    void on_add_pushButton_clicked();
    void on_remove_pushButton_clicked();
    void on_starting_location_dropdown_currentTextChanged(const QString &arg1);
    void on_irvine_pushButton_clicked();
    void on_arizona_pushButton_clicked();
    void on_michigan_pushButton_clicked();
    void on_view_auto_select_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_school_store_pushButton_clicked();
    void on_shopping_cart_pushButton_clicked();
    void on_return_home_pushButton_clicked();
    void dijkstra(int size);
    int minKey(std::vector<int> key, std::vector<bool> set, int size);

private:
    Ui::TripPlanner *ui;
    QSqlQueryModel* availableModel;
    QSqlQueryModel* selectedModel;
    std::vector<int> order;
    QString name;
    bool isSelected[20];
    double d[20][20];
    double optimal[20][20];
    int start;
    int spinBoxMax;
};

#endif // TRIPPLANNER_H
