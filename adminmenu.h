#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <QMainWindow>

namespace Ui {
class AdminMenu;
}

class AdminMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminMenu(QWidget *parent = nullptr);
    ~AdminMenu();

private:
    Ui::AdminMenu *ui;
};

#endif // ADMINMENU_H
