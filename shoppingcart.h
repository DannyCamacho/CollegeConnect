#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include <QMainWindow>

namespace Ui {
class ShoppingCart;
}

class ShoppingCart : public QMainWindow
{
    Q_OBJECT
public:
    explicit ShoppingCart(QWidget *parent = nullptr);
    ~ShoppingCart();

private:
    Ui::ShoppingCart *ui;
};

#endif // SHOPPINGCART_H
