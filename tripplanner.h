#ifndef TRIPPLANNER_H
#define TRIPPLANNER_H

#include <QMainWindow>
#include <QMessageBox>
#include "database.h"

namespace Ui { class TripPlanner; }

/*! \class TripPlanner
 *  \brief Handles planning of user selected trips
 *  This class handles the planning of user-selected trips. User can either choose a custom route or a Dijkstra-based efficient route.
 */
class TripPlanner : public QMainWindow {
    Q_OBJECT

public:

    /*! TripPlanner Constructor
     *
     *  Constructor used to initialized necessary variables, populate the route dropdown, and connect the widget change buttons to the main window.
     *  @param QWidget parent
     */
    explicit TripPlanner(QWidget *parent = nullptr);

    /*! TripPlanner Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~TripPlanner();

signals:

    /*! @fn void moveToMainWindow()
     *
     *  Signal used to notify MainWindow of widget change, returns user to MainWindow.
     *  @see mainwindow.h
     */
    void moveToMainWindow();

    /*! @fn void moveToGraphViewer()
     *
     *  Signal used to notify MainWindow of widget change, moves user to GraphViewer.
     *  @see graphviewer.h
     */
    void moveToGraphViewer();

    /*! @fn void moveToSchoolStore(QString name)
     *
     *  Signal used to notify MainWindow of widget change, moves user to SchoolStore. School store is determined by current school selected.
     *  @param QString name
     *  @see schoolstore.h
     */
    void moveToSchoolStore(QString name);

private slots:

    /*! @fn void populateWindow()
     *
     *  void-returning method used to populate the window, uses the main college table to determine the college dropdown and uses the edge table to populate the distance array.
     */
    void populateWindow();

    /*! @fn void updateTrip()
     *
     *  void-returning method used to prepare the model based off the selected institutions.
     */
    void updateTrip();

    /*! @fn void calculateTrip(int start)
     *
     *  Void-returning method used to catch signal emitted from MainWindow, used to update the collegeName used to determine which school souvenirs to display.
     *  @param int start
     */
    void calculateTrip(int start);

    /*! @fn void tableViewUpdate()
     *
     *  void-returning method used to update the tables based off the selected route.
     */
    void tableViewUpdate();

    /*! @fn void on_available_routes_tableView_clicked(const QModelIndex &index)
     *
     *  void-returning method used to update 'name' based off the institution selected on the available table.
     *  @param const QModelIndex &index
     */
    void on_available_routes_tableView_clicked(const QModelIndex &index);

    /*! @fn void on_selected_route_tableView_clicked(const QModelIndex &index)
     *
     *  void-returning method used to update 'name' based off the institution selected on the route table.
     *  @param const QModelIndex &index
     */
    void on_selected_route_tableView_clicked(const QModelIndex &index);

    /*! @fn void on_add_pushButton_clicked()
     *
     *  void-returning method used to add an institution to the route table.
     */
    void on_add_pushButton_clicked();

    /*! @fn void on_remove_pushButton_clicked()
     *
     *  void-returning method used to remove an institution from the route table.
     */
    void on_remove_pushButton_clicked();

    /*! @fn void on_starting_location_dropdown_currentTextChanged(const QString &arg1)
     *
     *  void-returning method used to set the starting college to the selected institution from the dropdown.
     *  @param const QString &arg1
     */
    void on_starting_location_dropdown_currentTextChanged(const QString &arg1);

    /*! @fn void on_irvine_pushButton_clicked()
     *
     *  void-returning method used to set the starting college to University of California, Irvine (UCI).
     */
    void on_irvine_pushButton_clicked();

    /*! @fn void on_arizona_pushButton_clicked()
     *
     *  void-returning method used to set the starting college to Arizona State University.
     */
    void on_arizona_pushButton_clicked();

    /*! @fn void on_michigan_pushButton_clicked()
     *
     *  void-returning method used to set the starting college to University of Michigan.
     */
    void on_michigan_pushButton_clicked();

    /*! @fn void on_view_auto_select_pushButton_clicked()
     *
     *  void-returning method used to select the closest colleges based on the spinBox.
     */
    void on_view_auto_select_pushButton_clicked();

    /*! @fn void on_pushButton_2_clicked()
     *
     *  void-returning method used to move to the graph viewer.
     *  @see graphviewer.h
     */
    void on_pushButton_2_clicked();

    /*! @fn void on_school_store_pushButton_clicked()
     *
     *  void-returning method used to move to the school store when a school is selected. If no school is selected the method is returned.
     *  @see schoolstore.h
     */
    void on_school_store_pushButton_clicked();

    /*! @fn void on_return_home_pushButton_clicked()
     *
     *  void-returning method used to return to the MainWindow.
     *  @see mainwindow.h
     */
    void on_return_home_pushButton_clicked();

    /*! @fn void dijkstra(int size)
     *
     *  void-returning method used to run the adjacency matrix through the Dijkstra algo. The distance and path for each city is determined and the totals are displayed to screen.
     *  @param int size
     */
    void dijkstra(int size);

    /*! @fn int minKey(std::vector<double> key, std::vector<bool> set, int size)
     *
     *  int-returning method used by both the dijkstra and mst algorithms respectively. Used to determine the next minimum index.
     *  @param std::vector<double> key
     *  @param std::vector<bool> set, int size
     */
    int minKey(std::vector<double> key, std::vector<bool> set, int size);

    /*! @fn void on_starting_location_dropdown_2_currentTextChanged(const QString &arg1)
     *
     *  void-returning method called when the route dropdown is changed, disables the closest college route frame when Custom Route is selected.
     *  @param const QString &arg1
     */
    void on_starting_location_dropdown_2_currentTextChanged(const QString &arg1);

private:

    /*! @var Ui::TripPlanner *ui
     *
     *  TripPlanner used to display and manage TripPlanner interactions.
     */
    Ui::TripPlanner *ui;

    /*! @var QSqlQueryModel* availableModel
     *
     *  QSqlQueryModel used to display the available colleges for the route.
     */
    QSqlQueryModel* availableModel;

    /*! @var QSqlQueryModel* selectedModel
     *
     *  QSqlQueryModel used to display the selected colleges for the route.
     */
    QSqlQueryModel* selectedModel;

    /*! @var std::vector<int> order
     *
     *  Vector used to store the order of the efficient route.
     */
    std::vector<int> order;

    /*! @var QString name
     *
     *  QString used to store the name of the selected college.
     */
    QString name;

    /*! @var bool isSelected[20]
     *
     *  2D array used store the bool status for each college using the efficient route. 1 is visited, 0 is unvisited.
     */
    bool isSelected[20];

    /*! @var double d[20][20]
     *
     *  2D array used store the initial distances for each college.
     */
    double d[20][20];

    /*! @var double optimal[20][20]
     *
     *  2D array used store the Dijkstra distances for each college.
     */
    double optimal[20][20];

    /*! @var int start
     *
     *  int used to set the starting college for the route.
     */
    int start;

    /*! @var int spinBoxMax
     *
     *  int used to set the maximum spinBox value.
     */
    int spinBoxMax;
};

#endif // TRIPPLANNER_H
