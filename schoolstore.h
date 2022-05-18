#ifndef SCHOOLSTORE_H
#define SCHOOLSTORE_H

#include <QMainWindow>
#include <QSqlQueryModel>

namespace Ui { class SchoolStore; }

/*! \class SchoolStore
 *  \brief Handles purchasing of souvenirs
 *  This class handles the adding and removing of souvenirs of the selected college.
 */
class SchoolStore : public QMainWindow {
    Q_OBJECT

public:

    /*! SchoolStore Constructor
     *
     *  Constructor used to initialize SQLQueryModels schoolStoreModel and update the schoolStore tableview.
     *  @param QWidget parent
     */
    explicit SchoolStore(QWidget *parent = nullptr);

    /*! SchoolStore Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~SchoolStore();

    /*! @fn void schoolStoreTableViewUpdate()
     *
     *  Void-returning method used to update the school_store tableview.
     */
    void schoolStoreTableViewUpdate();

public slots:

    /*! @fn void getCollegeName(const QString &collegeName)
     *
     *  Void-returning method used to catch signal emitted from MainWindow, used to update the collegeName used to determine which school souvenirs to display.
     *  @param const QString &collegeName
     */
    void getCollegeName(const QString &collegeName);

private slots:

    /*! @fn void on_initial_list_pushButton_clicked()
     *
     *  Proceed back to initial list: hides SchoolStore UI and heads back to MainWindow.
     *  @see mainwindow.h
     */
    void on_initial_list_pushButton_clicked();

    /*! @fn void on_school_store_tableView_clicked(const QModelIndex &index)
     *
     *  Void-returning method that updates souvenirItem and souvenirPrice when a menu item is selected from school_store tableview. index is the selected row of the tableview, with 0 being the name of the item and 1 being the price.
     *  @param const QModelIndex &index
     */
    void on_school_store_tableView_clicked(const QModelIndex &index);

    /*! @fn void on_add_to_cart_pushButton_clicked()
     *
     *  Void-returning that adds selected souvenir item and quantity to cart database. When an item is not selected, the method is returned, the quantity is determined by the spinBox. If the resulting quantity is greater than 100, the method is returned, else the souvenir item with the quantity, price, and college name is added to the cart or updated if an entry already existed.
     */
    void on_add_to_cart_pushButton_clicked();

    /*! @fn void on_shopping_cart_pushButton_clicked()
     *
     *  Void-returning Method used to catch signal emitted from MainWindow, used to update the collegeName used to determine which school souvenirs to display.
     */
    void on_shopping_cart_pushButton_clicked();

    /*! @fn void updateQuantity()
     *
     *  Void-returning method used to send signal to MainWindow to switch to shopping cart. Resets souvenirItem to avoid unintentional adding of items when returning to school store.
     */
    void updateQuantity();

    /*! @fn void on_plan_route_button_clicked()
     *
     *  Method used update quantity of items within the cart.
     */
    void on_plan_route_button_clicked();

signals:

    /*! @fn void adminLogout()
     *
     *  Signal used to notify MainWindow of widget change, returns user to MainWindow.
     *  @see mainwindow.h
     */
    void leaveSchoolStore();

    /*! @fn void adminLogout()
     *
     *  Signal used to notify MainWindow of widget change, moves user to ShoppingCart.
     *  @see shoppingcart.h
     */
    void moveToShoppingCart();

    /*! @fn void adminLogout()
     *
     *  Signal used to notify MainWindow of widget change, moves user to TripPlanner.
     *  @see tripplanner.h
     */
    void moveToTripPlanner();

private:

    /*! @var Ui::SchoolStore *ui
     *
     *  SchoolStore used to display and manage SchoolStore interactions.
     */
    Ui::SchoolStore *ui;

    /*! @var QSqlQueryModel* schoolStoreModel
     *
     *  QSqlQueryModel used to display the school store of the selected college.
     */
    QSqlQueryModel* schoolStoreModel;

    /*! @var QString collegeName
     *
     *  QString used to store the name of the selected college.
     */
    QString collegeName;

    /*! @var QString souvenirItem
     *
     *  QString used to store the name of the selected souvenir item.
     */
    QString souvenirItem;

    /*! @var QString souvenirPrice
     *
     *  QString used to store the price of the selected souvenir item.
     */
    QString souvenirPrice;
};

#endif // SCHOOLSTORE_H
