#ifndef TRIPPLANNER_H
#define TRIPPLANNER_H

#include <QMainWindow>

namespace Ui {
class tripplanner;
}

class tripplanner : public QMainWindow
{
    Q_OBJECT

public:
    explicit tripplanner(QWidget *parent = nullptr);
    ~tripplanner();

private:
    Ui::tripplanner *ui;
};

#endif // TRIPPLANNER_H
