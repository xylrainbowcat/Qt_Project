#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QWidget>
#include "globel.h"

enum USER_TYPE{
    TEACHER,
    STUDENT
};


namespace Ui {
class changePassword;
}

class changePassword : public QWidget
{
    Q_OBJECT

public:
    explicit changePassword(QSqlDatabase db, QString username, int UserType,  QWidget *parent = nullptr);
    ~changePassword();

private slots:
    void on_Btn_Confirm_clicked();

    void on_Btn_Cancel_clicked();

private:
    Ui::changePassword *ui;

    QSqlDatabase data;

    QString username;

    int userType;

    int interfaceType;
};

#endif // CHANGEPASSWORD_H
