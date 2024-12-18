#include "secondwindow.h"
#include "widget.h"

SecondWindow::SecondWindow(QWidget *parent,QSqlDatabase db)
    : QWidget{parent}
    , view(nullptr)
    , findButton(nullptr)
    , insertButton(nullptr)
    , second_db(db)
{
    this->setWindowTitle("数据库查询结果");
    view = new QTableView(this);

    findButton = new QPushButton("再次查询",this);

    insertButton = new QPushButton("返回插入",this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(findButton);
    buttonLayout->addWidget(insertButton);
    layout->addLayout(buttonLayout);

    connect(insertButton, &QPushButton::clicked, this, &SecondWindow::move);
    connect(findButton, &QPushButton::clicked, this, &SecondWindow::showNew);
}

SecondWindow::~SecondWindow(){
    delete view;
    delete findButton;
    delete insertButton;
}

void SecondWindow::showNow(){
    QSqlQuery query(second_db);
    bool success = query.prepare("select * from student");

    if (!success) {
        qDebug() << "Prepare failed:" << query.lastError().text();
        QMessageBox::critical(this, "查询提示", "Prepare failed:\n" + query.lastError().text());
        return;
    }

    // 执行查询
    if (!query.exec()) {
        qDebug() << "Execution failed:" << query.lastError().text();
        QMessageBox::critical(this, "查询提示", "查询失败:\n" + query.lastError().text());
        return;
    }

    // 使用 QSqlQueryModel 来显示查询结果
    QSqlQueryModel *model = new QSqlQueryModel(this); // 确保模型被正确释放
    model->setQuery(std::move(query));
    view->setModel(model);
    view->resizeColumnsToContents();
}

void SecondWindow::move(){
    emit moveToFirstView();
}

void SecondWindow::showNew(){
    showNow();
    QMessageBox::information(this, "再次查询提示", "已经重新查询");
}
