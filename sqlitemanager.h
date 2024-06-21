#ifndef SQLITEMANAGER_H
#define SQLITEMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMessageBox>


class SQLiteManager {
public:
    SQLiteManager(const QString& path);
    ~SQLiteManager();
    bool openDatabase();
    bool createTable();
    bool insertData(const QString& questionContent, const QString& answerContent, const QString& answerCode);
    QSqlQuery search(const QString& searchQuery);
    int countAnswers();
    void closeDatabase();

private:
    QSqlDatabase db;
    QString connectionName;  // 私有成员变量
};

#endif // SQLITEMANAGER_H
