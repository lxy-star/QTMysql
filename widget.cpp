#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , secondWindow(nullptr)
{
    ui->setupUi(this);
    // 连接DB
    db = QSqlDatabase::addDatabase("QMYSQL"); // 加载MySQL驱动

    // 设置数据库参数
    db.setHostName("localhost");//如果是远程链接输入远程主机和端口
    db.setDatabaseName("mydatabase");
    db.setUserName("root");
    db.setPassword("123456");

    // 测试连接
    if (!db.open()) {
        qDebug() << "Error: connection with database failed:" << db.lastError().text();
        QMessageBox::critical(this, "连接提示", "Error: connection with database failed\n" + db.lastError().text());
    } else {
        qDebug() << "Database: connection ok";
        QMessageBox::information(this, "连接提示", "Database: connection ok");
    }
}

Widget::~Widget()
{
    delete ui;
    if(secondWindow){
        delete secondWindow;
    }
}

void Widget::on_insertButton_clicked()
{
    QString id = ui->idLineEdit->text().trimmed();
    QString name = ui->nameLineEdit->text().trimmed();
    QString brithStr = ui->brithLineEdit->text().trimmed();

    // 使用 QDate 验证和格式化日期
    QDate brithDate;
    bool isValidDate = false;

    // 尝试以 YYYY-MM-DD 格式解析日期
    if (brithStr.contains('-')) {
        brithDate = QDate::fromString(brithStr, "yyyy-MM-dd");
        isValidDate = brithDate.isValid();
    } else {
        // 如果没有连字符，尝试以 YYYYMMDD 格式解析日期
        if (brithStr.size() == 8) {
            brithDate = QDate(brithStr.left(4).toInt(), brithStr.mid(4, 2).toInt(), brithStr.right(2).toInt());
            isValidDate = brithDate.isValid();
        }
    }

    if (!isValidDate) {
        QMessageBox::critical(this, "插入提示", "无效的日期格式，请使用 YYYY-MM-DD 或 YYYYMMDD 格式。");
        return;
    }

    // 使用参数化查询防止SQL注入
    QSqlQuery query(db);
    bool success = query.prepare("INSERT INTO student (id, name, birth) VALUES (?, ?, ?)");
    if (!success) {
        qDebug() << "Prepare failed:" << query.lastError().text();
        QMessageBox::critical(this, "插入提示", "Prepare failed:\n" + query.lastError().text());
        return;
    }

    query.addBindValue(id.toInt()); // 假设 id 是整数类型
    query.addBindValue(name);
    query.addBindValue(brithDate.toString("yyyy-MM-dd")); // 确保日期格式为 YYYY-MM-DD

    if (query.exec()) {
        qDebug() << "Insertion successful";
        QMessageBox::information(this, "插入提示", "插入成功");
    } else {
        qDebug() << "Execution failed:" << query.lastError().text();
        QMessageBox::critical(this, "插入提示", "插入失败:\n" + query.lastError().text());
    }
}

void Widget::on_findButton_clicked() {
    if (!secondWindow) {
        secondWindow = new SecondWindow(nullptr, db);
        connect(secondWindow, &SecondWindow::moveToFirstView, this, &Widget::showNow);
    }
    secondWindow->showNow();
    secondWindow->show();
    this->hide();
}

void Widget::showNow() {
    if (secondWindow && secondWindow->isVisible()) {
        secondWindow->close();
    }
    this->show();
}


