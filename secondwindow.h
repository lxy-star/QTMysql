#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class SecondWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SecondWindow(QWidget *parent = nullptr,QSqlDatabase db = QSqlDatabase());
    ~SecondWindow() override;
    void showNow();

private slots:
    void move();
    void showNew();

signals:
    void moveToFirstView();

private:
    QTableView* view;
    QPushButton* findButton;
    QPushButton* insertButton;
    QSqlDatabase second_db;
};

#endif // SECONDWINDOW_H
