#include "mainwindow.h"
#include "database.h"
#include "map.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;


    DoubleHashMap collegeMap(11);
    Pair currentCollege;

    QSqlQuery query;
    query.exec("SELECT * from college");
    if (query.exec())
    {
        while (query.next())
        {
            currentCollege.collegeName = query.value(0).toString().toStdString();
            currentCollege.key = query.value(1).toInt();
            currentCollege.state = query.value(2).toString().toStdString();
            currentCollege.numsOfGrad = query.value(3).toInt();
            collegeMap.insert(currentCollege);
        }
    }
    collegeMap.printMap();


    w.show();
    return a.exec();
}
