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

/*! \class MainWindow
 *  \brief Handles flow of program
 *  This class is the main window of the program, handles the flow of the program using a QStackedWidget.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    /*! MainWindow Constructor
     *
     *  Constructor used to initialized necessary variables, populate the QStackedWidget, and connect the widget change buttons to the MainWindow.
     *  @param QWidget parent
     */
    MainWindow(QWidget *parent = nullptr);

    /*! MainWindow Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~MainWindow();

    /*! @fn void populateWindow()
     *
     *  void-returning method used to populate the window, initialize the custom map, and populate the state dropdown.
     */
    void populateWindow();

    /*! @fn void schoolTableUpdate()
     *
     *  void-returning method used to update the table based off the selected state option and ordering.
     *  @param const QString &arg1
     */
    void schoolTableUpdate();

public slots:

    /*! @fn void receiveMessage(const QString &msg)
     *
     *  void-returning method used to move to the admin menu on successful login.
     *  @param const QString &msg
     */
    void receiveMessage(const QString &msg);

    /*! @fn void moveToShoppingCart()
     *
     *  void-returning method used to move to the shopping cart.
     */
    void moveToShoppingCart();

    /*! @fn void moveToSchoolStore()
     *
     *  void-returning method used to move to the school store from a separate widget.
     */
    void moveToSchoolStore();

    /*! @fn void moveToGraphViewer()
     *
     *  void-returning method used to move to the graph viewer from a separate widget.
     */
    void moveToGraphViewer();

    /*! @fn void moveToSchoolStoreFromTrip(const QString name);
     *
     *  void-returning method used to move to the school store from the trip planner using the school selected within the trip planner.
     *  @param const QString name
     */
    void moveToSchoolStoreFromTrip(const QString name);

private slots:

    /*! @fn void on_select_state_currentTextChanged(const QString &arg1);
     *
     *  void-returning method used to update the table and set collegeName to null.
     *  @param const QString &arg1
     */
    void on_select_state_currentTextChanged(const QString &arg1);

    /*! @fn void on_school_list_tableView_clicked(const QModelIndex &index);
     *
     *  void-returning method used to populate the information sidebar based off the selected college.
     *  @param const QModelIndex &index
     */
    void on_school_list_tableView_clicked(const QModelIndex &index);

    /*! @fn void on_toggle_name_order_ascending_clicked()
     *
     *  void-returning method used to order the window by collegeName in ascending order.
     */
    void on_toggle_name_order_ascending_clicked();

    /*! @fn void on_toggle_name_order_descending_clicked()
     *
     *  void-returning method used to order the window by collegeName in descending order.
     */
    void on_toggle_name_order_descending_clicked();

    /*! @fn void on_choose_graph_dropdown_currentTextChanged()
     *
     *  void-returning method used to order the window by state in ascending order.
     */
    void on_toggle_state_order_ascending_clicked();

    /*! @fn void on_choose_graph_dropdown_currentTextChanged()
     *
     *  void-returning method used to order the window by state in descending order.
     */
    void on_toggle_state_order_descending_clicked();

    /*! @fn void on_actionLogin_triggered()
     *
     *  void-returning method used to access the login window popup.
     */
    void on_actionLogin_triggered();

    /*! @fn void returnToMainWindow()
     *
     *  void-returning method used by other widgets in the QStackedWidget to move to the MainWindow.
     */
    void returnToMainWindow();

    /*! @fn void on_actionQuit_triggered()
     *
     *  void-returning method used to close the program.
     */
    void on_actionQuit_triggered();


    /*! @fn void moveToTripPlanner()
     *
     *  void-returning method used to move to the trip planner.
     */
    void moveToTripPlanner();

    /*! @fn void on_visit_store_button_clicked()
     *
     *  void-returning method used to move to the school store of the selected school. If no school is selected, the method is returned.
     */
    void on_visit_store_button_clicked();

    /*! @fn void on_plan_route_button_clicked()
     *
     *  void-returning method used to move to the trip planner from MainWindow. Reinitializes the distances for changes.
     */
    void on_plan_route_button_clicked();

signals:

    /*! @fn void updateSchoolStore(QString collegeName)
     *
     *  Signal used to update the SchoolStore using the currently selected college.
     *  @see schoolstore.h
     *  @param QString collegeName
     */
    void updateSchoolStore(QString collegeName);

    /*! @fn void updateShoppingCart()
     *
     *  Signal used to update the ShoppingCart model.
     *  @see shoppingcart.h
     */
    void updateShoppingCart();

    /*! @fn void updateTripPlanner()
     *
     *  Signal used to update the TripPlanner model and distances.
     *  @see tripplanner.h
     */
    void updateTripPlanner();

    /*! @fn void updateGraphViewer()
     *
     *  Signal used to update the GraphViewer model and distances.
     *  @see graphviewer.h
     */
    void updateGraphViewer();

private:

    /*! @var Ui::MainWindow *ui
     *
     *  MainWindow used to display and manage MainWindow interactions.
     */
    Ui::MainWindow *ui;

    /*! @var AdminMenu* adminMenu
     *
     *  AdminMenu used to add widget to MainWindow QStackedWidget.
     */
    AdminMenu* adminMenu;

    /*! @var SchoolStore* schoolStore
     *
     *  SchoolStore used to add widget to MainWindow QStackedWidget.
     */
    SchoolStore* schoolStore;

    /*! @var ShoppingCart* shoppingCart
     *
     *  ShoppingCart used to add widget to MainWindow QStackedWidget.
     */
    ShoppingCart* shoppingCart;

    /*! @var TripPlanner* tripPlanner
     *
     *  TripPlanner used to add widget to MainWindow QStackedWidget.
     */
    TripPlanner* tripPlanner;

    /*! @var GraphViewer* graphViewer
     *
     *  GraphViewer used to add widget to MainWindow QStackedWidget.
     */
    GraphViewer* graphViewer;

    /*! @var QSqlQueryModel* schoolModel
     *
     *  QSqlQueryModel used to display the list of colleges and the associated state.
     */
    QSqlQueryModel* schoolModel;

    /*! @var Database database
     *
     *  Database instance used to access and modify the database.
     */
    Database database;

    /*! @var QString collegeName
     *
     *  QString used to hold the name of the selected college.
     */
    QString collegeName;

    /*! @var QString order
     *
     *  QString used to hold the sorting order (ascending/descending) of the colleges/states.
     */
    QString order;

    /*! @var DoubleHashMap collegeMap
     *
     *  DoubleHashMap is a custom map used to hold the collegeName, state, numOfUndergrads, and distFromSaddleback. These variables are mapped to the collegeNum.
     */
    DoubleHashMap collegeMap;
};
#endif // MAINWINDOW_H
