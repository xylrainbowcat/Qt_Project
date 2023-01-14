#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "teacherwindow.h"
#include "studentwindow.h"

#include "reset.h"


QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();


private slots:
    void on_Btn_Login_clicked();

    void on_Btn_Reset_clicked();

    void on_Btn_Exit_clicked();

    void on_Btn_Pass_clicked();

private:
    Ui::LoginWindow *ui;

    QSqlDatabase db;
};

#endif // LOGINWINDOW_H
