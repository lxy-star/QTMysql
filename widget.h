#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

#include "secondwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_insertButton_clicked();
    void on_findButton_clicked();
    void showNow();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    SecondWindow *secondWindow;
signals:
    void moveToSecondView();
};
#endif // WIDGET_H
