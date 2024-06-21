#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include "newwindow.h" // 确保包含 NewWindow 类的头文件
#include <QMouseEvent>  // 包含 QMouseEvent 的定义
#include <QPainter>



namespace Ui {
class MainWindow;
}
//extern QString imagePath;  // 声明外部全局变量

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();  // 如果你使用自动生成的槽

    void on_pushButton_2_clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;   // 重写鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;    // 重写鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override; // 重写鼠标释放事件
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
    bool m_dragging;
    QPoint m_dragPosition;
    QSettings settings;
    QString imagePath; // 成员变量声明
    QString imagePathRc; // 成员变量声明
    NewWindow *newWindow;  // 此处声明 newWindow
    const QString validUsername = "admin";
    const QString validPassword = "123456";
    void loadSettings();  // 声明加载设置的函数
    void saveSettings();  // 声明保存设置的函数

};

#endif // MAINWINDOW_H
