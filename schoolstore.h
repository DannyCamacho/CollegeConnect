#ifndef SCHOOLSTORE_H
#define SCHOOLSTORE_H

#include <QMainWindow>
#include <QSqlQueryModel>

namespace Ui { class SchoolStore; }
class SchoolStore : public QMainWindow {
    Q_OBJECT

public:
    explicit SchoolStore(QWidget *parent = nullptr);
    ~SchoolStore();
    void schoolStoreTableViewUpdate();

private slots:
    void on_initial_list_pushButton_clicked();

private:
    Ui::SchoolStore *ui;
    QSqlQueryModel* schoolStoreModel;

};

#endif // SCHOOLSTORE_H
