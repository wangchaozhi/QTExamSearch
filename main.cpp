#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 设置插件路径
     QCoreApplication::addLibraryPath("E:/Qt/Qt5.12.12/5.12.12/mingw73_64/plugins");
    MainWindow w;
    w.show();
     qDebug() << "Available drivers:" << QSqlDatabase::drivers();
    return a.exec();
}
