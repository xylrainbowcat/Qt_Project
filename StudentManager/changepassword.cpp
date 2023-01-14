#include "changepassword.h"
#include "ui_changepassword.h"

changePassword::changePassword(QSqlDatabase db, QString Username, int UserType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changePassword)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(470, 270);
    data = db;
    username = Username;
    userType = UserType;

    ui->oldline->setEchoMode(QLineEdit::Password);   // 小黑点代替输入
    ui->newline->setEchoMode(QLineEdit::Password);   // 小黑点代替输入
    ui->confirmline->setEchoMode(QLineEdit::Password);   // 小黑点代替输入

    switch (userType) {
    case TEACHER:
    {
        ui->Btn_Confirm->setStyleSheet(
                    "QPushButton {"
                    "border: 1 solid #8c8c8c;"
                    "border-radius: 5px;"
                    "background : #107C41;"
                    "color: #ffffff;"
                    "}"
                    "QPushButton:hover {"
                        "background-color: #0F703B;"
                    "}"
                    "QPushButton:pressed {"
                        "background-color: #107C41;"
                        "border: 1 solid #000000;"
                    "}"
                    );
        ui->Btn_Cancel->setStyleSheet(
                    "QPushButton {"
                        "border-radius: 5px;"
                        "background : #ffffff;"
                        "color: #262626;"
                        "border: 1 solid #8c8c8c;"
                    "}"

                    "QPushButton:hover {"
                       " background-color: #E9F5EE;"
                    "}"

                    "QPushButton:pressed {"
                        "background-color: #ffffff;"
                        "border: 1 solid #000000;"
                    "}"
                    );

    }
        break;
    case STUDENT:
    {
        ui->Btn_Confirm->setStyleSheet(
                    "QPushButton {"
                    "border: 1 solid #8c8c8c;"
                    "border-radius: 5px;"
                    "background : #185ABD;"
                    "color: #ffffff;"
                    "}"
                    "QPushButton:hover {"
                        "background-color: #1651AA;"
                    "}"
                    "QPushButton:pressed {"
                        "background-color: #185ABD;"
                        "border: 1 solid #000000;"
                    "}"
                    );
        ui->Btn_Cancel->setStyleSheet(
                    "QPushButton {"
                        "border-radius: 5px;"
                        "background : #ffffff;"
                        "color: #262626;"
                        "border: 1 solid #8c8c8c;"
                    "}"

                    "QPushButton:hover {"
                       " background-color: #d9def5;"
                    "}"

                    "QPushButton:pressed {"
                        "background-color: #ffffff;"
                        "border: 1 solid #000000;"
                    "}"
                    );

    }
    }
}

changePassword::~changePassword()
{
    delete ui;
}

void changePassword::on_Btn_Confirm_clicked()
{
    if (ui->oldline->text() ==ui->newline->text()) {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("新密码不能和旧密码相同");
        box.exec();
        return;
    }

    QSqlQuery query(data);
    switch (userType) {
    case TEACHER:
    {
        query.exec("select password "
                   "from teacher "
                   "where tno = '" + username + "'");
    }
        break;
    case STUDENT:
    {
        query.exec("select password "
                   "from student "
                   "where sno = '" + username + "'");
    }
    }
    if(query.isActive() && query.next() && query.value(0).toString() == ui->oldline->text()){
        QString new_1 = ui->newline->text();
        QString new_2 = ui->confirmline->text();
        if(new_1 != new_2){
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("请确认密码");
            box.exec();
        }
        else {
            QSqlQuery query_new(data);
            switch (userType) {
            case TEACHER:
            {
                query_new.exec("update teacher "
                               "set password = '" + new_1 + "' "
                               "where tno = '" + username + "' ");
            }
                break;
            case STUDENT:
            {
                query_new.exec("update student "
                               "set password = '" + new_1 + "' "
                               "where sno = '" + username + "' ");
            }

            }
            if(query_new.isActive()){
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("密码更改");
                box.setText("修改成功");
                box.exec();
                this->close();
            }
            else{
                QMessageBox box;
                box.setIcon(QMessageBox::Critical);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("警告");
                box.setText("修改失败");
                box.exec();
                qDebug()<<query_new.lastError().text();
            }
        }
    }
    else {
        if(!query.isActive()){
            qDebug()<<query.lastError().text();
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("密码错误");
            box.exec();
        }
    }
}

void changePassword::on_Btn_Cancel_clicked()
{
        this->close();
}
