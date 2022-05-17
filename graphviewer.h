#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QMainWindow>
#include "database.h"

namespace Ui { class GraphViewer; }

/*! \class GraphViewer
 *  \brief Handles displaying graphs
 *  This class handles the creation and modification of the graphs using the AdjacencyMatrix.
 */
class GraphViewer : public QMainWindow {
    Q_OBJECT

public:

    /*! GraphViewer Constructor
     *
     *  Constructor used to initialized necessary variables and connect the widget change buttons to the MainMindow.
     *  @param QWidget parent
     */
    explicit GraphViewer(QWidget *parent = nullptr);

    /*! GraphViewer Destructor
     *
     *  Destructor used to delete heap allocated memory.
     */
    ~GraphViewer();

signals:

    /*! @fn void moveToTripPlanner()
     *
     *  Signal used to notify MainWindow of user leaving GraphViewer, returns user to TripPlanner.
     *  @see mainwindow.h
     *  @see tripplanner.h
     */
     void moveToTripPlanner();

private slots:

     /*! @fn void populateWindow()
      *
      *  void-returning method used to populate the window and initialize the distances for the ajacencyMatrix.
      */
    void populateWindow();

    /*! @fn void tableViewUpdate()
     *
     *  void-returning method used to populate the window and initialize the distances for the ajacencyMatrix.
     */
    void tableViewUpdate();

    /*! @fn void on_choose_graph_dropdown_currentTextChanged()
     *
     *  void-returning method used to update the tableview based off the selected graph and college.
     *  @param const QString &arg1
     */
    void on_choose_graph_dropdown_currentTextChanged(const QString &arg1);

    /*! @fn void on_choose_college_dropdown_currentTextChanged()
     *
     *  void-returning method used to update the graph when a new college is chosen in the college dropdown.
     *  @param const QString &arg1
     */
    void on_choose_college_dropdown_currentTextChanged(const QString &arg1);

    /*! @fn void on_trip_planner_pushButton_clicked()
     *
     *  void-returning method used to move to the trip planner.
     */
    void on_trip_planner_pushButton_clicked();

private:

    /*! @var Ui::GraphViewer *ui
     *
     *  GraphViewer used to display and manage GraphViewer interactions.
     */
    Ui::GraphViewer *ui;

    /*! @var QSqlQueryModel* path
     *
     *  QSqlQueryModel used to display the path used for the selected graph.
     */
    QSqlQueryModel* path;

    /*! @var QSqlQueryModel* discoverEdges
     *
     *  QSqlQueryModel used to display the discover edges for the selected graph.
     */
    QSqlQueryModel* discoverEdges;

    /*! @var QSqlQueryModel* backEdges
     *
     *  QSqlQueryModel used to display the back edges for the selected graph.
     */
    QSqlQueryModel* backEdges;

    /*! @var AdjacencyMatrix adjMatrix
     *
     *  AdjacencyMatrix used to store distances and implement the selected graph.
     */
    AdjacencyMatrix adjMatrix;
};

#endif // GRAPHVIEWER_H
