#ifndef MYSQLMANAGER_H
#define MYSQLMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMessageBox>

class MySQLManager {
public:
    MySQLManager(const QString& host, int port, const QString& databaseName, const QString& username, const QString& password);
    ~MySQLManager();
    bool openDatabase();
    bool createTable();
    bool insertData(const QString& data);
    QSqlQuery search(const QString& searchQuery);
    QSqlQuery allSearch();


private:
    QSqlDatabase db;
};

#endif // MYSQLMANAGER_H
