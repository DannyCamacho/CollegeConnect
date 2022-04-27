#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QAbstractProxyModel>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class Database {
public:
    Database();
    void populateColleges(std::string fileName);
    void populateSouvenirs(std::string fileName);
    void importColleges(std::string fileName);
    void importSouvenirs(std::string fileName);
private:
    void connect();
    void init();
    static bool isInitialized;
};

class Horizontal_proxy_model : public QAbstractProxyModel {
public:
  Horizontal_proxy_model(QObject * parent = 0);
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // DATABASE_H
