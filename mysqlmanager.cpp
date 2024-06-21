#include "mysqlmanager.h"

MySQLManager::MySQLManager(const QString& host, int port, const QString& databaseName, const QString& username, const QString& password) {
    db = QSqlDatabase::addDatabase("QMYSQL","mysql");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);
}

MySQLManager::~MySQLManager() {
    if (db.isOpen()) {
        db.close();
    }
}

bool MySQLManager::openDatabase() {
    if (!db.open()) {
        qDebug() << "Error: failed to open database." << db.lastError().text();
        return false;
    }
    qDebug() << "Database " << db.connectionName() << " opened successfully.";
    return true;
}


bool MySQLManager::createTable() {
    QSqlDatabase specificDb = QSqlDatabase::database("mysql");  // 获取特定的数据库连接
    QSqlQuery query(specificDb);  // 使用特定的连接创建查询
    bool success = query.exec("CREATE TABLE IF NOT EXISTS data (id INT AUTO_INCREMENT PRIMARY KEY, info VARCHAR(255))");
    if (!success) {
        qDebug() << "Failed to create table:" << query.lastError();
    }
    return success;
}

bool MySQLManager::insertData(const QString& data) {
    QSqlDatabase specificDb = QSqlDatabase::database("mysql");
    QSqlQuery query(specificDb);
    query.prepare("INSERT INTO data (info) VALUES (:info)");
    query.bindValue(":info", data);
    if (!query.exec()) {
        qDebug() << "Error inserting data:" << query.lastError();
        return false;
    }
    return true;
}

QSqlQuery MySQLManager::search(const QString& searchQuery) {
    QSqlDatabase specificDb = QSqlDatabase::database("mysql");
    QSqlQuery query(specificDb);
    query.prepare("SELECT * FROM t_answer WHERE question_content LIKE :search");
    query.bindValue(":search", "%" + searchQuery + "%");
    if (!query.exec()) {
        qDebug() << "Error searching in database:" << query.lastError();
    }
    return query;
}
QSqlQuery MySQLManager::allSearch() {
    QSqlDatabase specificDb = QSqlDatabase::database("mysql");
    QSqlQuery query(specificDb);
    query.prepare("SELECT question_content, answer_content, answer_code FROM t_answer");
    if (!query.exec()) {
        qDebug() << "Error fetching data:" << query.lastError();
    }
    return query;
}


