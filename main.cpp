#include <QApplication>
#include "mainwindow.h"

/* ==== CollegeConnect main() =======================================
    Function ran at startup, declare, initialize, and show the MainWindow
    widget used throughout the programs run.
================================================================== */
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
