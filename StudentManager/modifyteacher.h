#ifndef MODIFYTEACHER_H
#define MODIFYTEACHER_H

#include <QMainWindow>
#include "globel.h"

namespace Ui {
class ModifyTeacher;
}

class ModifyTeacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModifyTeacher(QSqlDatabase db, QString username, int model, QWidget *parent = nullptr);
    ~ModifyTeacher();

    void init();

signals:
    void update();

private slots:
    void on_Btn_Cancel_clicked();

    void on_Btn_Save_clicked();

private:
    Ui::ModifyTeacher *ui;

    QSqlDatabase db;

    QString username;

    QButtonGroup admin;

    int Model;
};

#endif // MODIFYTEACHER_H
