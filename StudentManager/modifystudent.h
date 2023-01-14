#ifndef MODIFYSTUDENT_H
#define MODIFYSTUDENT_H

#include <QMainWindow>
#include "globel.h"


namespace Ui {
class ModifyStudent;
}

class ModifyStudent : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModifyStudent(QSqlDatabase db, QString username, int model, QWidget *parent = nullptr);
    ~ModifyStudent();

    void init();


signals:
    void update();

private slots:
    void on_Btn_Cancel_clicked();

    void on_Btn_Save_clicked();

private:
    Ui::ModifyStudent *ui;

    QSqlDatabase db;

    QString username; //要修改的学号

    int Model; //0修改; 1新增

    QButtonGroup sex;
};

#endif // MODIFYSTUDENT_H
