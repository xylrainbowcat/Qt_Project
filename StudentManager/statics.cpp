#include "statics.h"
#include "ui_statics.h"

Statics::Statics(QSqlDatabase db, QString Course, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Statics)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(360, 300);
    database = db;
    course = Course;

    avg();  //求平均分
    count();  //求不及格人数
    per(); //求挂科率


}

Statics::~Statics()
{
    delete ui;
}

void Statics::on_Btn_yes_clicked()
{
    this->close();
}

void Statics::avg(){
    QSqlQuery query(database);
    query.exec("select avg(score) "
               "from sc "
               "where score > 0 and cno = '" + course + "'");
    if(query.next()){
        ui->avg->setText(QString::number(query.value(0).toDouble(), 'f', 1));
    }
    else ui->avg->setText("/");
}

void Statics::count(){
    QSqlQuery query(database);
    query.exec("select count(sno) "
               "from sc "
               "where score > 0 and score < 60 and cno = '" + course + "'");
    if(query.next()){
        ui->count->setText(query.value(0).toString());
    }
    else ui->count->setText("/");
}

void Statics::per(){
    QSqlQuery query_fz(database);
    QSqlQuery query_fm(database);
    double per;
    query_fz.exec("select count(sno) "
                  "from sc "
                  "where score > 0 and score < 60 and cno = '" + course + "'");
    if(query_fz.isActive() && query_fz.next())per = query_fz.value(0).toDouble();
    else {
        ui->per->setText("/");
        qDebug()<<query_fz.lastError().text();
    }
    query_fm.exec("select count(sno) "
               "from sc "
               "where score > 0 and cno = '" + course + "'");
    if(query_fm.isActive() && query_fm.next() && query_fm.value(0).toInt() != 0)per /= query_fm.value(0).toDouble();
    else {
        per = -1;
        if(!query_fm.isActive())
            qDebug()<<query_fm.lastError().text();
    }
    ui->per->setText(per == -1 ? "/" : QString::number(per, 'f', 1) + "%");
}
