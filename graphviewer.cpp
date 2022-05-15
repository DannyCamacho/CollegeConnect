#include "graphviewer.h"
#include "ui_graphviewer.h"

GraphViewer::GraphViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::GraphViewer), adjMatrix(20) {
    ui->setupUi(this);
    path = new QSqlQueryModel;
    discoverEdges = new QSqlQueryModel;
    backEdges = new QSqlQueryModel;
    connect(parent, SIGNAL(updateTripPlanner()), this, SLOT(populateWindow()));
}

GraphViewer::~GraphViewer() {
    delete ui;
    delete path;
    delete discoverEdges;
    delete backEdges;
}

void GraphViewer::populateWindow() {
    ui->choose_graph_dropdown->clear();
    ui->choose_graph_dropdown->addItem("BFS");
    ui->choose_graph_dropdown->addItem("DFS");
    ui->choose_graph_dropdown->addItem("Dijkstra");
    ui->choose_graph_dropdown->addItem("MST");

    int size = 0;
    ui->choose_college_dropdown->clear();
    QSqlQuery query("SELECT collegeName FROM college");
    while (query.next()) {
        ++size;
        ui->choose_college_dropdown->addItem(query.value(0).toString());
    }
    adjMatrix.setSize(size);
    query.exec("SELECT * FROM edge");
    while (query.next()) {
        adjMatrix.add(query.value(0).toString().toStdString(), query.value(1).toString().toStdString(), query.value(2).toDouble());
    }
}

void GraphViewer::on_choose_graph_dropdown_currentTextChanged(const QString &arg1) {
    QSqlQuery query("DROP TABLE path;");
    query.exec("DROP TABLE discoveryEdges;");
    query.exec("DROP TABLE backEdges;");
    query.exec("CREATE TABLE path(collegeName TEXT, routeOrder INTEGER UNIQUE PRIMARY KEY, distToNext INTEGER);");
    query.exec("CREATE TABLE discoveryEdges(collegeName TEXT, endingCollege TEXT, distance INTEGER);");
    query.exec("CREATE TABLE backEdges(collegeName TEXT, endingCollege TEXT, distance INTEGER);");

    if (arg1 == "MST") {
        ui->choose_college_dropdown->setVisible(0);
        adjMatrix.mst();
    } else {
        ui->choose_college_dropdown->setVisible(1);
        ui->choose_college_dropdown->setCurrentText("Select College");
    }
    tableViewUpdate();
}

void GraphViewer::on_choose_college_dropdown_currentTextChanged(const QString &arg1) {
    QSqlQuery query("DROP TABLE path;");
    query.exec("DROP TABLE discoveryEdges;");
    query.exec("DROP TABLE backEdges;");
    query.exec("CREATE TABLE path(collegeName TEXT, routeOrder INTEGER UNIQUE PRIMARY KEY, distToNext INTEGER);");
    query.exec("CREATE TABLE discoveryEdges(collegeName TEXT, endingCollege TEXT, distance INTEGER);");
    query.exec("CREATE TABLE backEdges(collegeName TEXT, endingCollege TEXT, distance INTEGER);");

    int start = adjMatrix.getStartingIndex(arg1.toStdString());
    if (ui->choose_graph_dropdown->currentText() == "BFS") {
        adjMatrix.BFS(start);
    } else if (ui->choose_graph_dropdown->currentText() == "DFS") {
        adjMatrix.DFS(start);
    } else {
        adjMatrix.dijkstra(start);
    }
   tableViewUpdate();
}

void GraphViewer::tableViewUpdate() {
    path->setQuery("SELECT collegeName, distToNext FROM path ORDER BY routeOrder");
    ui->graph_result_tableView->setModel(path);
    backEdges->setQuery("INSERT INTO backEdges(collegeName, endingCollege, distance) SELECT (collegeName, endingCollege, distance) FROM edges;");
    //backEdges->setQuery("DELETE FROM backEdges WHERE EXISTS (SELECT collegeName, endingCollege, distance FROM discoveryEdges WHERE backEdges.collegeName = discoveryEdges.collegeName, backEdges.endingCollege = discoveryEdges.endingCollege, backEdges.distance = discoveryEdges.distance);");
    //backEdges->setQuery("DELETE FROM backEdges WHERE EXISTS (SELECT collegeName, endingCollege, distance FROM discoveryEdges WHERE backEdges.collegeName = discoveryEdges.endingCollege, backEdges.endingCollege = discoveryEdges.collegeName, backEdges.distance = discoveryEdges.distance);");
    backEdges->setQuery("SELECT collegeName, endingCollege, distance FROM backEdges");
    ui->back_edges_tableView->setModel(backEdges);
    discoverEdges->setQuery("SELECT collegeName, endingCollege, distance FROM discoverEdges");
    ui->discovery_edges_tableView->setModel(discoverEdges);

    QSqlQuery query("SELECT SUM(X.TOTAL) FROM (SELECT distToNext as TOTAL FROM path) X;");
    if (query.next()) ui->distance_label->setText(QString::number(query.value(0).toDouble(), 'f', 2));
    if (ui->distance_label->text() == "") ui->distance_label->setText("0.00");
}
