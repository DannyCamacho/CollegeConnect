#include "graphviewer.h"
#include "ui_graphviewer.h"

GraphViewer::GraphViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::GraphViewer) {
    ui->setupUi(this);
    //AdjacencyList adjList(20);
    //AdjacencyMatrix adjMatrix(20);
}

GraphViewer::~GraphViewer() {
    delete ui;
}
