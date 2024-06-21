#include "sqlitemanager.h"

SQLiteManager::SQLiteManager(const QString& path)  : connectionName("SQLITE"){
    db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    db.setDatabaseName(path);
}

SQLiteManager::~SQLiteManager() {
    if (db.isOpen()) {
           db.close();
       }
}

bool SQLiteManager::openDatabase() {
    if (!db.open()) {
        qDebug() << "Error: failed to open database." << db.lastError().text();
        return false;
    }
    return true;
}

bool SQLiteManager::createTable() {
    QSqlDatabase specificDb = QSqlDatabase::database(connectionName);
    QSqlQuery query(specificDb);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS t_answer ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "question_content VARCHAR(800), "
        "answer_content VARCHAR(500), "
        "answer_code VARCHAR(2)"
        ")"
    );
    if (!success) {
        qDebug() << "Failed to create table:" << query.lastError();
    }
    return success;
}


bool SQLiteManager::insertData(const QString& questionContent, const QString& answerContent, const QString& answerCode) {
    QSqlDatabase specificDb = QSqlDatabase::database(connectionName);
    QSqlQuery query(specificDb);
    query.prepare("INSERT INTO t_answer (question_content, answer_content, answer_code) VALUES (:questionContent, :answerContent, :answerCode)");
    query.bindValue(":questionContent", questionContent);
    query.bindValue(":answerContent", answerContent);
    query.bindValue(":answerCode", answerCode);
    if (!query.exec()) {
        qDebug() << "Error inserting data:" << query.lastError();
        return false;
    }
    return true;
}


QSqlQuery SQLiteManager::search(const QString& searchQuery) {
    QSqlDatabase specificDb = QSqlDatabase::database(connectionName);
    QSqlQuery query(specificDb);
    // Correctly include the placeholder within the LIKE clause
    query.prepare("SELECT id, question_content, answer_content, answer_code FROM t_answer WHERE question_content LIKE :search");
    query.bindValue(":search", "%" + searchQuery + "%");  // Bind the value with wildcards for fuzzy search
    if (!query.exec()) {
        qDebug() << "Error searching in database:" << query.lastError();
    }
    return query;
}
int SQLiteManager::countAnswers() {
    QSqlDatabase specificDb = QSqlDatabase::database(connectionName);
    QSqlQuery query(specificDb);
    query.prepare("SELECT COUNT(*) FROM t_answer");
    if (!query.exec()) {
        qDebug() << "Error counting entries in database:" << query.lastError();
        return -1;  // Return -1 or another error indicator
    }
    if (query.next()) {
        return query.value(0).toInt();  // Return the count of rows
    }
    return 0;  // Return 0 if no rows found (should not happen with COUNT(*))
}



void SQLiteManager::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        QSqlDatabase::removeDatabase(connectionName);  // 移除数据库连接
        qDebug() << "Database connection " << connectionName << " closed and removed successfully.";
    }
}
