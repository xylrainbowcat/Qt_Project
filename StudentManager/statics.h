#ifndef STATICS_H
#define STATICS_H

#include <QWidget>
#include "globel.h"
namespace Ui {
class Statics;
}

class Statics : public QWidget
{
    Q_OBJECT

public:
    explicit Statics(QSqlDatabase db, QString course, QWidget *parent = nullptr);
    ~Statics();

    void avg();
    void count();
    void per();

private slots:
    void on_Btn_yes_clicked();

private:
    Ui::Statics *ui;

    QSqlDatabase database;

    QString course;
};

#endif // STATICS_H
