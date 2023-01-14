#include "modifycourse.h"
#include "ui_modifycourse.h"

ModifyCourse::ModifyCourse(QSqlDatabase db, QString username, int model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModifyCourse)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(700, 360);

    ui->Grade->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->Grade->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    this->db = db;
    Model = model;

    if(Model == MODIFY || Model == CHECK){
        this->username = username;
        init();
    }
}

ModifyCourse::~ModifyCourse()
{
    delete ui;
}

void ModifyCourse::init(){
    QSqlQuery query(db);
    query.exec("select cno, cname, tno, grade, period, credit, assess, property "
               "from course "
               "where cno = '" + username + "'");

    query.next();
    ui->Cno->setText(query.value(0).toString());
    ui->Cno->setReadOnly(true);
    ui->Cname->setText(query.value(1).toString());
    ui->Tno->setText(query.value(2).toString());
    ui->Grade->setCurrentText(query.value(3).toString());
    ui->Period->setText(query.value(4).toString());
    ui->Credit->setText(query.value(5).toString());
    ui->Assess->setText(query.value(6).toString());
    ui->Property->setText(query.value(7).toString());
    if(Model == CHECK){

        ui->Cname->setReadOnly(true);
        ui->Tno->setReadOnly(true);
        ui->Grade->setEnabled(false);
        ui->Period->setReadOnly(true);
        ui->Credit->setReadOnly(true);
        ui->Assess->setReadOnly(true);
        ui->Property->setReadOnly(true);
        ui->Btn_Save->setText("确定");
    }
}



void ModifyCourse::on_Btn_Save_clicked()
{
    QSqlQuery query(db);
    query.exec("select tno from teacher where tno = '" + ui->Tno->text() + "'");

    if (!query.next()) {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setWindowTitle("课程管理");
        box.setText("修改失败，请输入有效的教师编号！");
        box.exec();
        qDebug()<<query.lastError().text();
        return;
    }
    switch (Model) {
    case MODIFY:
    {
        QSqlQuery query(db);
        query.exec("update course set "
                   "cname = '" + ui->Cname->text() + "', "
                   "tno = '" + ui->Tno->text() + "', "
                   "grade = '" + ui->Grade->currentText().simplified() + "', "
                   "period = '" + ui->Period->text() + "', "
                   "credit = '" + ui->Credit->text() + "', "
                   "assess = '" + ui->Assess->text() + "', "
                   "property = '" + ui->Property->text() + "' "
                   "where cno = '" + username + "'");
        QMessageBox box;
        if(query.isActive()){
            box.setIcon(QMessageBox::Information);
            box.setWindowTitle("课程管理");
            box.setText("修改成功");
            box.exec();
            this->close();
            emit update();
        }
        else {
            box.setIcon(QMessageBox::Critical);
            box.setWindowTitle("课程管理");
            box.setText("修改失败，请检查数据正确性！");
            box.exec();
            qDebug()<<query.lastError().text();
        }
    }
        break;
    case ADD:
    {
        if(ui->Cno->text().trimmed() == "")return ;
        QSqlQuery query(db);
        query.exec("insert into course(cno, cname, tno, grade, period, credit, assess, property) "
                   "values ( "
                   " '" + ui->Cno->text() + "', "
                   " '" + ui->Cname->text() + "', "
                   " '" + ui->Tno->text() + "', "
                   " '" + ui->Grade->currentText().simplified() + "', "
                   " '" + ui->Period->text() + "', "
                   " '" + ui->Credit->text() + "', "
                   " '" + ui->Assess->text() + "', "
                   " '" + ui->Property->text() + "')");
        QMessageBox box;
        if(query.isActive()){
            box.setIcon(QMessageBox::Information);
            box.setWindowTitle("课程管理");
            box.setText("新增成功");
            box.exec();
            this->close();
            emit update();
        }
        else {
            box.setIcon(QMessageBox::Critical);
            box.setWindowTitle("课程管理");
            box.setText("新增失败，请检查数据正确性！");
            box.exec();
            qDebug()<<query.lastError().text();
        }
    }
        break;
    case CHECK:
    {
        this->close();
    }
    }
}

void ModifyCourse::on_Btn_Cancel_clicked()
{
    this->close();
}
