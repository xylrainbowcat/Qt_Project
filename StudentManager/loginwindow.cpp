#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/res/logo.png"));                                             // 设置窗口样式
    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(630, 420);

    ui->Login_Password->setEchoMode(QLineEdit::Password);   // 小黑点代替输入
    ui->Login_Username->setPlaceholderText("请输入用户名");
    ui->Login_Password->setPlaceholderText("请输入密码");
    ui->Mode_Student->setChecked(true);        // 设置默认选项

    //ui->Login_Username->setText("202241803629");
    //ui->Login_Password->setText("812814");

    if (QSqlDatabase::contains("qt_sql_default_connection"))                    // 连接数据库
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
//          db = QSqlDatabase::addDatabase("QODBC");

    db.setHostName("localhost");
    db.setDatabaseName("studentmanager");
    db.setUserName(USERNAME);
//    db.setPassword(PASSWORD);
    db.setPassword(PASSWORD_WWY);

    if (!db.open())
        qDebug() << "Failed to connect to root mysql admin";
    else qDebug() << "open";
}

LoginWindow::~LoginWindow() {
    delete ui;
}


// =============================================================================== 登陆按钮
void LoginWindow::on_Btn_Login_clicked() {
    QString username = ui->Login_Username->text();
    QString password = ui->Login_Password->text();

    qDebug() << "用户名：" << username << "密码:" << password;

    if (ui->Mode_Teacher->isChecked()) {                                        // 登陆教师端
        QSqlQuery query(db);
        query.exec("select password "
                   "from teacher "
                   "where tno = '" + username + "'");

        if (query.next() && password == query.value(0).toString()) {
                this->hide();
                TeacherWindow *teacher = new TeacherWindow(db, username);
                teacher->show();

                connect(teacher, &TeacherWindow::back, this, [=]() {
                    this->show();
                });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("用户名或密码错误");
            box.exec();
        }
    }

    else {                                                                      // 登陆学生端
        QSqlQuery query(db);
        query.exec("select password "
                   "from student "
                   "where sno = '" + username + "'");

        if (query.next() && password == query.value(0).toString()) {
                this->hide();
                StudentWindow *student = new StudentWindow(db, username);
                student->show();

                connect(student, &StudentWindow::back, this, [=]() {
                    this->show();
                });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("用户名或密码错误");
            box.exec();
        }

    }
}

// =============================================================================== 重置按钮
void LoginWindow::on_Btn_Reset_clicked() {
    ui->Login_Username->clear();
    ui->Login_Password->clear();
}

// =============================================================================== 退出按钮
void LoginWindow::on_Btn_Exit_clicked() {
    this->close();
}

// =============================================================================== 忘记密码
void LoginWindow::on_Btn_Pass_clicked() {
    QString username = ui->Login_Username->text();
    if (username == "") {
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("请输入用户名");
        box.exec();
        return;
    }

    if (ui->Mode_Teacher->isChecked()) {
        QSqlQuery query(db);
        query.exec("select tname from teacher where tno = '" + username + "'");
        if (query.next()) {
            Reset *reset = new Reset(query.value(0).toString());
            reset->setWindowModality(Qt::ApplicationModal);
            reset->show();

            connect(reset, &Reset::reset, this, [=]() {
                QSqlQuery query(db);
                query.exec("update teacher set password = '" + username + "' where tno = '" + username + "'");
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("警告");
                box.setText("密码已重置为您的职工号");
                box.exec();
            });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("请输入正确的用户名");
            box.exec();
        }

    }
    else {
        QSqlQuery query(db);
        query.exec("select sname from student where sno = '" + username + "'");
        if (query.next()) {
            Reset *reset = new Reset(query.value(0).toString());
            reset->setWindowModality(Qt::ApplicationModal);
            reset->show();

            connect(reset, &Reset::reset, this, [=]() {
                QSqlQuery query(db);
                query.exec("update student set password = '" + username + "' where sno = '" + username + "'");
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("警告");
                box.setText("密码已重置为您的学号");
                box.exec();
            });
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("请输入正确的用户名");
            box.exec();
        }

    }
}

