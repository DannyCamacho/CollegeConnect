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
    void moveToShoppingCart();

private slots:
    void populateWindow();
    void updateQuantity();

private:
    Ui::TripPlanner *ui;
    QSqlQueryModel* availableModel;
    QSqlQueryModel* selectedModel;
    std::vector<int> order;
    QString name;
    bool isAvailable[20];
    double d[20][20];
    int start;
};

#endif // TRIPPLANNER_H
