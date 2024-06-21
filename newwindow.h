#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QMessageBox>
#include <QTableView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QSoundEffect>
#include <QNetworkInterface>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "mysqlmanager.h"
#include "sqlitemanager.h"



namespace Ui {
class NewWindow;
}

class NewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewWindow(QWidget *parent = nullptr);
    ~NewWindow();

private slots:
    void on_pushButton_clicked();
    void on_networkReplyFinished(QNetworkReply *reply);  // Add parameter

private:
    Ui::NewWindow *ui;
    QNetworkAccessManager *networkManager;
    QStandardItemModel *model; // Model to hold and display JSON data
    MySQLManager mysqlDbManager;
    SQLiteManager sqliteDbManager;
    QSoundEffect clickSound;
    void initDatabase();
    void transferDataFromMySQLToSQLite(MySQLManager& mysqlDbManager, SQLiteManager& sqliteDbManager);

};

#endif // NEWWINDOW_H
