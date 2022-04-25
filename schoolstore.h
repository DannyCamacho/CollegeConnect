#ifndef SCHOOLSTORE_H
#define SCHOOLSTORE_H

#include <QMainWindow>

namespace Ui {
class SchoolStore;
}

class SchoolStore : public QMainWindow
{
    Q_OBJECT
public:
    explicit SchoolStore(QWidget *parent = nullptr);
    ~SchoolStore();

private:
    Ui::SchoolStore *ui;
};

#endif // SCHOOLSTORE_H
