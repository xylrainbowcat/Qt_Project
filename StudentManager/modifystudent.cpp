#include "modifystudent.h"
#include "ui_modifystudent.h"

ModifyStudent::ModifyStudent(QSqlDatabase db, QString username, int model, QWidget *parent) : QMainWindow(parent), ui(new Ui::ModifyStudent) {
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(385, 420);

    this->db = db;

    sex.addButton(ui->sex1, 0);
    sex.addButton(ui->sex2, 1);
    ui->sex1->setChecked(true);
    Model = model;
    if(Model == MODIFY || Model == CHECK){
        this->username = username;
        init();
    }
}

ModifyStudent::~ModifyStudent() {
    delete ui;
}


void ModifyStudent::init() {
    QSqlQuery query(db);
    query.exec("select sno, sname, sex, grade, major, classno "
               "from student "
               "where sno = '" + username + "'");

    query.next();
    ui->Sno->setText(query.value(0).toString());
    ui->Sno->setReadOnly(true);
    ui->Sname->setText(query.value(1).toString());
    query.value(2).toString() == "男"?ui->sex1->setChecked(true):ui->sex2->setChecked(true);
    ui->Grade->setText(query.value(3).toString());
    ui->Major->setText(query.value(4).toString());
    ui->Classno->setText(query.value(5).toString());
    if(Model == CHECK){
        ui->Sname->setReadOnly(true);
        ui->sex1->setEnabled(false);
        ui->sex2->setEnabled(false);
        ui->Grade->setReadOnly(true);
        ui->Major->setReadOnly(true);
        ui->Classno->setReadOnly(true);
        ui->Btn_Save->setText("确定");
    }
}

void ModifyStudent::on_Btn_Cancel_clicked() {
    this->close();
}


void ModifyStudent::on_Btn_Save_clicked() {
    QSqlQuery query(db);

    switch(Model){
    case MODIFY:
    {
        bool k = query.exec("update student "
                             "set sname = '" + ui->Sname->text() + "', "
                                 "sex = '" + (sex.checkedId() == 0?"男":"女") + "', "
                                 "grade = '" + ui->Grade->text() + "', "
                                 "major = '" + ui->Major->text() + "', "
                                 "classno = '" + ui->Classno->text() + "' "
                                 "where sno = '" + username + "'");
        QMessageBox box;
        if (k) {
            box.setIcon(QMessageBox::Information);
            box.setWindowTitle("师生管理");
            box.setText("修改成功");
            box.exec();
            this->close();
            emit update();
        }
        else {
            box.setIcon(QMessageBox::Critical);
            box.setWindowTitle("师生管理");
            box.setText("修改失败，请检查数据正确性！");
            box.exec();
            qDebug()<<query.lastError().text();
        }
    }
        break;
    case ADD:
    {
        if(ui->Sno->text() == ""){
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("新增无效");
            box.exec();
            return ;
        }
        QSqlQuery t = db.exec("insert into student "
                            "(sno, sname, sex, grade, major, classno, password) "
                            "values ("
                            " '" + ui->Sno->text() + "', "
                            " '" + ui->Sname->text() + "', "
                            " '" + (sex.checkedId() == 0?"男":"女") + "', "
                            " '" + ui->Grade->text() + "', "
                            " '" + ui->Major->text() + "', "
                            " '" + ui->Classno->text() + "', "
                            " '123456'); ");    //默认密码123456
        QMessageBox box;
        if (t.isActive()) {
            box.setIcon(QMessageBox::Information);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("新增成功");
            box.exec();
            this->close();
            emit update();
        }
        else {
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("新增失败，请检查数据正确性！");
            box.exec();
            qDebug()<<t.lastError().text();
        }

    }
        break;
    case CHECK:
    {
        this->close();
    }
    }
}

