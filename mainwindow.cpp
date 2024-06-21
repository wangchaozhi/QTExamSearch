#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
     m_dragging(false),
    settings("YourOrganization", "YourApp"),
    newWindow(nullptr)  // 初始化指针为 nullptr
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);  // Make the window background transparent
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);  // Ensure the window is frameless

     menuBar()->setVisible(false);
     statusBar()->setVisible(false);
    // 设置窗口为无边框样式
//       setWindowFlags(Qt::FramelessWindowHint);
       // 禁止调整窗口大小
       setFixedSize(size());

       // 根据操作系统设置 imagePath
          #ifdef Q_OS_WIN
          imagePath = "C:/Users/86131/Pictures/home2.png";
          #elif defined(Q_OS_LINUX)
          imagePath = "/home/1.jpg";
          #endif
           imagePathRc=":/images/home2.png";
       // 设置样式表
//       this->setStyleSheet("QMainWindow {"
//                           "border-radius: 15px;"
//                           "}");
    loadSettings();  // 加载设置
    // connect 调用已移除，使用 Qt Designer 自动生成的槽处理点击事件
}

MainWindow::~MainWindow()
{
    delete ui;  // 清理 UI
    delete newWindow;  // 清理 NewWindow
}

void MainWindow::saveSettings()
{
    if (ui->checkBox->isChecked()) {
        settings.setValue("username", ui->lineEdit->text());
        settings.setValue("password", ui->lineEdit_2->text());
        settings.setValue("rememberPassword", true);
    } else {
        settings.remove("username");
        settings.remove("password");
        settings.setValue("rememberPassword", false);
    }
}

void MainWindow::loadSettings()
{
    if (settings.value("rememberPassword", false).toBool()) {
        ui->lineEdit->setText(settings.value("username").toString());
        ui->lineEdit_2->setText(settings.value("password").toString());
        ui->checkBox->setChecked(true);
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 只有在用户点击窗口的客户区时才开始拖动
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 如果正在拖动，则更新窗口位置
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 停止拖动
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}
//void MainWindow::paintEvent(QPaintEvent *event) {
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    // 使用 QPainterPath 绘制倒角矩形
//    QPainterPath path;
//    path.addRoundedRect(rect(), 25, 25);  // 15 是倒角的半径
//    painter.fillPath(path, Qt::white);  // 以白色填充（或选择其他颜色）

////    // 背景图绘制
//    QPixmap bg(imagePathRc);
//    painter.drawPixmap(rect(), bg);

//    QMainWindow::paintEvent(event);  // 调用基类的 paintEvent
//}
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), 25, 25);  // 25 是倒角的半径
    painter.fillPath(path, Qt::transparent);  // 以白色填充

    QPixmap bg(imagePathRc);
    painter.setClipPath(path);  // 确保背景图也应用倒角
    painter.drawPixmap(rect(), bg);

    QMainWindow::paintEvent(event);  // 调用基类的 paintEvent
}




void MainWindow::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();  // 从用户名输入框获取文本
    QString password = ui->lineEdit_2->text();  // 从密码输入框获取文本

    if (username == validUsername && password == validPassword) {
        if (!newWindow)  // 如果 newWindow 未创建，则创建它
            newWindow = new NewWindow(this);

        saveSettings();  // 保存设置
        newWindow->show();  // 显示新窗口
        this->hide();  // 隐藏当前窗口
    } else {
        // 如果用户名或密码不正确，显示错误消息
        QMessageBox::warning(this, "登录失败", "用户名或密码错误！");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

