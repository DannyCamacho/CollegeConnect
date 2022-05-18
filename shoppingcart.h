#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include <QMainWindow>
#include "database.h"

namespace Ui { class ShoppingCart; }

/*! \class ShoppingCart
 *  \brief Handles the shopping cart
 *  This class handles the modifying and removing of souvenirs within the shopping cart.
 */
class ShoppingCart : public QMainWindow {
    Q_OBJECT

public:

    /*! ShoppingCart Constructor
     *
     *  Constructor used to initialized necessary variables, update the cart model, and connect the widget change buttons to the MainWindow.
     *  @see mainwindow.h
     *  @param QWidget parent
     */
    explicit ShoppingCart(QWidget *parent = nullptr);

    /*! ShoppingCart Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~ShoppingCart();

private slots:

    /*! @fn void cartTableViewUpdate()
     *
     *  void-returning method used to update the cart when changes are made.
     */
    void cartTableViewUpdate();

    /*! @fn void totTableViewUpdate()
     *
     *  void-returning method used to calculate the total for each college and display result as the totModel table.
     */
    void totTableViewUpdate();

    /*! @fn void calculateTotal()
     *
     *  void-returning method used to calculate the total for all items within the cart table.
     */
    void calculateTotal();

    /*! @fn void on_shopping_cart_TableView_clicked()
     *
     *  void-returning method used to update the collegeName and item variables when an item is selected from the cart table.
     *  @param const QModelIndex &index
     */
    void on_shopping_cart_TableView_clicked(const QModelIndex &index);

    /*! @fn void on_empty_cart_pushButton_clicked()
     *
     *  void-returning method used to remove all items from the cart.
     */
    void on_empty_cart_pushButton_clicked();

    /*! @fn void on_remove_item_pushButton_clicked()
     *
     *  void-returning method used to remove a selected item. If no item is selected, a warning message pop-up is displayed.
     */
    void on_remove_item_pushButton_clicked();

    /*! @fn void on_update_pushButton_clicked()
     *
     *  void-returning method used to update the quantity of a selected item. If no item is selected, a warning message pop-up is displayed.
     */
    void on_update_pushButton_clicked();

    /*! @fn void on_pushButton_6_clicked()
     *
     *  void-returning method used to move to the school store.
     */
    void on_pushButton_6_clicked();

    /*! @fn void updateShoppingCart()
     *
     *  void-returning method used to update the cart when moving to the cart from another widget.
     */
    void updateShoppingCart();

    /*! @fn void on_route_pushButton_clicked()
     *
     *  void-returning method used to move to the TripPlanner.
     *  @see tripplanner.h
     */
    void on_route_pushButton_clicked();

signals:

    /*! @fn void moveToSchoolStore()
     *
     *  Signal used to notify MainWindow of widget change, moves user to SchoolStore.
     *  @see schoolstore.h
     */
    void moveToSchoolStore();

    /*! @fn void moveToTripPlanner()
     *
     *  Signal used to notify MainWindow of widget change, moves user to TripPlanner.
     *  @see tripplanner.h
     */
    void moveToTripPlanner();

private:

    /*! @var Ui::ShoppingCart *ui
     *
     *  ShoppingCart used to display and manage ShoppingCart interactions.
     */
    Ui::ShoppingCart *ui;

    /*! @var QSqlQueryModel* cartModel
     *
     *  QSqlQueryModel used to display the souvenirs within the cart along with costs.
     */
    QSqlQueryModel* cartModel;

    /*! @var QSqlQueryModel* totModel
     *
     *  QSqlQueryModel used to display the total cost of souvenirs per university.
     */
    QSqlQueryModel* totModel;

    /*! @var QString collegeName
     *
     *  QString used to store the name of the selected college.
     */
    QString collegeName;

    /*! @var QString item
     *
     *  QString used to store the name of the selected souvenir item.
     */
    QString item;
};

#endif // SHOPPINGCART_H
