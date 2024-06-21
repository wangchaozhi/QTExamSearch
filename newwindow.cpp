#include "newwindow.h"
#include "ui_newwindow.h"




NewWindow::NewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewWindow),
    networkManager(new QNetworkAccessManager(this)),
    model(new QStandardItemModel(this)),
    mysqlDbManager("localhost", 3306, "demo2", "root", "Wangchaozhi123@"),
    sqliteDbManager("test.db")

{
    ui->setupUi(this);
    // 设置窗口始终在最前
//       setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    // 禁止调整窗口大小
    setFixedSize(size());
    ui->tableView->setModel(model);
       model->setHorizontalHeaderLabels(QStringList() << "Question Content" << "Answer Content" << "Answer Code");
//    connect(ui->pushButton, &QPushButton::clicked, this, &NewWindow::on_pushButton_clicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &NewWindow::on_networkReplyFinished);
    // 设置列宽
        ui->tableView->setColumnWidth(0, 300); // 设置第一列（questionContent）的宽度为300像素
        ui->tableView->setColumnWidth(1, 200); // 设置第二列（answerContent）的宽度为200像素
        ui->tableView->setColumnWidth(2, 200); // 设置第三列（answerCode）的宽度为100像素
        // 隐藏行头
           ui->tableView->verticalHeader()->setVisible(true);
           // 设置音效文件路径
//              clickSound.setSource(QUrl::fromLocalFile("E:/RiderProjects/ExamSearchApp/ExamSearchApp/Assets/Audio/xm2727.wav"));
//              clickSound.setSource(QUrl::fromLocalFile("xm2727.wav"));
              clickSound.setSource(QUrl("qrc:/sounds/xm2727.wav")); // 使用资源 URL
              clickSound.setVolume(1.0); // 设置音量，范围从 0.0 到 1.0
//              initDatabase();  // 调用初始化数据库的函数
//              transferDataFromMySQLToSQLite(mysqlDbManager,sqliteDbManager);


}

NewWindow::~NewWindow()
{
    delete ui;
}

//void NewWindow::on_pushButton_clicked()
//{
//    // 播放点击音效
//    qDebug() << "Before playing sound";
//    clickSound.play();
//    qDebug() << "After playing sound";
//    QUrl url("http://192.168.3.42:8090/officeAutomation/test/search");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//    QJsonObject json;
//    json["query"] = ui->lineEdit->text();

//    networkManager->post(request, QJsonDocument(json).toJson());
//}
QString getMacAddress() {
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        // 检查接口是否处于活动状态且不是回环接口
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            // 返回第一个活动的非回环接口的 MAC 地址
            return interface.hardwareAddress();
        }
    }
    return QString();  // 如果没有找到合适的接口，返回空字符串
}

//void NewWindow::on_pushButton_clicked()
//{
//    // 播放点击音效
//    qDebug() << "Before playing sound";
//    clickSound.play();
//    qDebug() << "After playing sound";

//    // 获取输入框中的文本，并移除前后空白
//    QString query = ui->lineEdit->text().trimmed();
//    // 获取 MAC 地址
//    QString macAddress = getMacAddress();

//    // 只有当输入不为空时才发送请求
//    if (!query.isEmpty()) {
//        QUrl url("http://192.168.3.42:8090/officeAutomation/test/search");
//        QNetworkRequest request(url);
//        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

//        QJsonObject json;
//        json["query"] = query;  // 使用处理过的文本
//        json["mac"] = macAddress;  // 添加 MAC 地址
//        networkManager->post(request, QJsonDocument(json).toJson());
//    } else {
//        qDebug() << "Input is empty, not sending request.";
//    }
//}

void NewWindow::on_pushButton_clicked() {
    // Play the click sound effect
    qDebug() << "Before playing sound";
    clickSound.play();
    qDebug() << "After playing sound";

    // Retrieve the text from the input field and trim whitespace
    QString searchText = ui->lineEdit->text().trimmed();

    // Check if the input is not empty
    if (!searchText.isEmpty()) {

        if (sqliteDbManager.openDatabase()) {
             qDebug() << sqliteDbManager.countAnswers();
            QSqlQuery query = sqliteDbManager.search(searchText);

            model->clear(); // Clear existing items
            model->setHorizontalHeaderLabels(QStringList() << "Question Content" << "Answer Content" << "Answer Code"); // Reset headers after clear

            while (query.next()) {

                QString questionContent = query.value("question_content").toString();
                QString answerContent = query.value("answer_content").toString();
                QString answerCode = query.value("answer_code").toString();
                QStandardItem* itemQuestion = new QStandardItem(questionContent);
                QStandardItem* itemAnswer = new QStandardItem(answerContent);
                QStandardItem* itemCode = new QStandardItem(answerCode);

                itemAnswer->setTextAlignment(Qt::AlignLeft);
                itemCode->setTextAlignment(Qt::AlignCenter); // Center align the third column

                QList<QStandardItem*> items;
                items.append(itemQuestion);
                items.append(itemAnswer);
                items.append(itemCode);

                model->appendRow(items);
            }
            // 重新设置列宽
             ui->tableView->setColumnWidth(0, 300); // 设置第一列（questionContent）的宽度为300像素
             ui->tableView->setColumnWidth(1, 200); // 设置第二列（answerContent）的宽度为200像素
             ui->tableView->setColumnWidth(2, 200); // 设置第三列（answerCode）的宽度为100像素
        } else {
            QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        }
    } else {
        qDebug() << "Input is empty, not performing search.";
    }
}



void NewWindow::on_networkReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QJsonObject obj = doc.object();
            const QJsonArray dataArray = obj["data"].toArray();

            model->clear(); // 清除现有的项
            model->setHorizontalHeaderLabels(QStringList() << "Question Content" << "Answer Content" << "Answer Code"); // Reset headers after clear

            for (const QJsonValue &value : dataArray) {
                      QJsonObject obj = value.toObject();
                      QStandardItem *itemQuestion = new QStandardItem(obj["questionContent"].toString());
                      QStandardItem *itemAnswer = new QStandardItem(obj["answerContent"].toString());
                      QStandardItem *itemCode = new QStandardItem(obj["answerCode"].toString());

                      // 设置文本对齐方式
                      itemAnswer->setTextAlignment(Qt::AlignLeft);
                      itemCode->setTextAlignment(Qt::AlignCenter); // 居中对齐第三列

                      QList<QStandardItem *> items;
                      items.append(itemQuestion);
                      items.append(itemAnswer);
                      items.append(itemCode);

                      model->appendRow(items);
                  }
                // 重新设置列宽
                 ui->tableView->setColumnWidth(0, 300); // 设置第一列（questionContent）的宽度为300像素
                 ui->tableView->setColumnWidth(1, 200); // 设置第二列（answerContent）的宽度为200像素
                 ui->tableView->setColumnWidth(2, 200); // 设置第三列（answerCode）的宽度为100像素
        } else {
        QMessageBox::critical(this, "Network error", reply->errorString());
    }

    reply->deleteLater();
}

void NewWindow::initDatabase() {
//    SQLiteManager dbManager("test.db");
//       if (dbManager.openDatabase()) {
//           dbManager.createTable();
//           dbManager.insertData("Sample data");
//           QSqlQuery query = dbManager.search("Sample");
//           while (query.next()) {
//               QString info = query.value("info").toString();
//               qDebug() << info;
//           }
//       }


//       MySQLManager mysqlDbManager("localhost", 3306, "demo2", "root", "Wangchaozhi123@");
//          if (mysqlDbManager.openDatabase()) {
//              QSqlQuery query = mysqlDbManager.search("Example");

//          }

}
void NewWindow::transferDataFromMySQLToSQLite(MySQLManager& mysqlDbManager, SQLiteManager& sqliteDbManager) {
    if (!mysqlDbManager.openDatabase()) {
        qDebug() << "Failed to open MySQL database";
        return;
    }

    if (!sqliteDbManager.openDatabase()) {
        qDebug() << "Failed to open SQLite database";
        return;
    }

    // Assuming `search` in MySQLManager is modified to fetch all data without conditions
    QSqlQuery mysqlQuery = mysqlDbManager.allSearch();

    // Ensure the SQLite table exists
    sqliteDbManager.createTable();

    while (mysqlQuery.next()) {
        QString questionContent = mysqlQuery.value("question_content").toString();
        QString answerContent = mysqlQuery.value("answer_content").toString();
        QString answerCode = mysqlQuery.value("answer_code").toString();

        if (!sqliteDbManager.insertData(questionContent, answerContent, answerCode)) {
            qDebug() << "Failed to insert data into SQLite";
        }
    }
}
