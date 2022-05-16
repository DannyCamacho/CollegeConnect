#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QMainWindow>
#include "database.h"

namespace Ui { class GraphViewer; }
class GraphViewer : public QMainWindow {
    Q_OBJECT

public:
    explicit GraphViewer(QWidget *parent = nullptr);
    ~GraphViewer();

signals:
     void moveToTripPlanner();

private slots:
    void populateWindow();
    void tableViewUpdate();
    void on_choose_graph_dropdown_currentTextChanged(const QString &arg1);
    void on_choose_college_dropdown_currentTextChanged(const QString &arg1);
    void on_trip_planner_pushButton_clicked();

private:
    Ui::GraphViewer *ui;
    QSqlQueryModel* path;
    QSqlQueryModel* discoverEdges;
    QSqlQueryModel* backEdges;
    AdjacencyMatrix adjMatrix;
};

#endif // GRAPHVIEWER_H
