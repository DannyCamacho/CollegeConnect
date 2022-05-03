#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include <QMainWindow>
#include "database.h"

namespace Ui { class ShoppingCart; }
class ShoppingCart : public QMainWindow {
    Q_OBJECT

public:
    explicit ShoppingCart(QWidget *parent = nullptr);
    ~ShoppingCart();

private slots:
    void cartTableViewUpdate();
    void totTableViewUpdate();
    void calculateTotal();
    void on_shopping_cart_TableView_clicked(const QModelIndex &index);
    void on_empty_cart_pushButton_clicked();
    void on_remove_item_pushButton_clicked();
    void on_update_pushButton_clicked();
    void on_pushButton_6_clicked();
    void updateShoppingCart();

signals:
    void moveToSchoolStore();

private:
    Ui::ShoppingCart *ui;
    QSqlQueryModel* cartModel;
    QSqlQueryModel* totModel;
    QString collegeName;
    QString item;
};

#endif // SHOPPINGCART_H
