#include "modifyteacher.h"
#include "ui_modifyteacher.h"

ModifyTeacher::ModifyTeacher(QSqlDatabase db, QString username, int model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModifyTeacher)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(420, 500);
    Model = model;
    this->db = db;
    admin.addButton(ui->admin_1, 1);
    admin.addButton(ui->admin_2, 0);
    ui->admin_2->setChecked(true);

    ui->class_table->setRowCount(50);
    ui->class_table->setColumnCount(1);
    ui->class_table->setColumnWidth(0,150);
    ui->class_table->setStyle(QStyleFactory::create("Fusion"));
    ui->class_table->setHorizontalHeaderItem(0, new QTableWidgetItem("班级"));
    ui->course_table->setRowCount(50);
    ui->course_table->setColumnCount(1);
    ui->course_table->setColumnWidth(0,150);
    ui->course_table->setStyle(QStyleFactory::create("Fusion"));
    ui->course_table->setHorizontalHeaderItem(0, new QTableWidgetItem("课程"));
    if(Model == MODIFY || Model == CHECK){
        this->username = username;
        init();
    }
}

ModifyTeacher::~ModifyTeacher()
{
    delete ui;
}

void ModifyTeacher::init(){
    QSqlQuery query(db);
    query.exec("select tno, tname, admin "
               "from teacher "
               "where tno = '" + username + "'");

    query.next();
    ui->Tno->setText(query.value(0).toString());
    ui->Tno->setReadOnly(true);
    ui->Tname->setText(query.value(1).toString());
    if(query.value(2).toInt() == 1){
        ui->admin_1->setChecked(true);
    }
    else {
        ui->admin_2->setChecked(true);
    }
    QSqlQuery query_2(db);
    query_2.exec("select classno "
                 "from class "
                 "where tno = '" + username + "'");
    int row = 0;
    while(query_2.next()){
        ui->class_table->insertRow(row);
        ui->class_table->setItem(row, 0, new QTableWidgetItem(query_2.value(0).toString()));
        ui->class_table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    }
    QSqlQuery query_3(db);
    query_3.exec("select cno "
                 "from course "
                 "where tno = '" + username + "'");
    int row2 = 0;
    while(query_3.next()){
        ui->course_table->insertRow(row2);
        ui->course_table->setItem(row2, 0, new QTableWidgetItem(query_3.value(0).toString()));
        ui->course_table->item(row2, 0)->setTextAlignment(Qt::AlignCenter);
    }

    if(Model == CHECK){

        ui->Tname->setReadOnly(true);
        ui->admin_1->setEnabled(false);
        ui->admin_2->setEnabled(false);
        ui->class_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->course_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->Btn_Save->setText("确定");
    }

}


void ModifyTeacher::on_Btn_Cancel_clicked()
{
    this->close();
}

void ModifyTeacher::on_Btn_Save_clicked()
{
    switch (Model) {
    case MODIFY:
    {

        /*修改逻辑：只能带领目前没有教师带领的班级\只能任教目前没有教师授课的课程*/
        bool ifwork = true;
        bool ifconflict = false;  //如果选了已有老师带领的班级\已有授课的课程，造成冲突
        if(ui->Tno->text() == "")ifconflict = true;
        QSqlQuery query(db);
        query.exec("update teacher "
                   "set tname = '" + ui->Tname->text() + "', "
                       "admin = " + QString::number(admin.checkedId()) + " "
                       "where tno = '" + username + "'");
        if(!query.isActive()){
            qDebug()<<query.lastError().text();
            ifwork = false;
        }
        if(ifwork){
            db.exec("update class "
                    "set tno = '' "
                    "where tno = '" + username + "' ");
            int row = 0;
            while(ui->class_table->item(row, 0)){
                if( ui->class_table->item(row, 0)->text() == ""){
                    row++;
                    continue;
                }
                QSqlQuery q(db);
                q.exec("update class "
                       "set tno = '" + username + "' "
                       "where tno = '' and classno = '" + ui->class_table->item(row, 0)->text() + "'");
                row++;
                if(!q.isActive()){
                    qDebug()<<q.lastError().text();
                    ifwork = false;
                    break;
                }
                if(q.numRowsAffected() == 0){
                    ifconflict = true;
                }
            }
            if(ifwork){
                db.exec("update course "
                        "set tno = '' "
                        "where tno = '" + username + "' ");
                int row = 0;
                while(ui->course_table->item(row, 0)){
                    if( ui->course_table->item(row, 0)->text() == ""){
                        row++;
                        continue;
                    }
                    QSqlQuery q(db);
                    q.exec("update course "
                           "set tno = '" + username + "' "
                           "where tno = '' and cno = '" + ui->course_table->item(row, 0)->text() + "'");
                    row++;
                    if(!q.isActive()){
                        qDebug()<<q.lastError().text();
                        ifwork = false;
                        break;
                    }
                    if(q.numRowsAffected() == 0){
                        ifconflict = true;
                    }
                }
            }
        }
        if(ifwork){
            if(ifconflict){
                QMessageBox box;
                box.setIcon(QMessageBox::Warning);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("师生管理");
                box.setText("修改无效");
                box.exec();
            }
            else {
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("师生管理");
                box.setText("修改成功");
                box.exec();
                this->close();
                emit update();
            }
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("修改失败，请注意数据正确性");
            box.exec();
        }

    }
        break;
    case ADD:
    {
        /*新增逻辑：只能带领目前没有教师带领的班级\只能任教目前没有教师授课的课程*/
        bool ifwork = true;
        bool ifconflict = false;
        if(ui->Tno->text() == ""){
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("新增无效");
            box.exec();
            return ;
        }
        QSqlQuery query(db);
        query.exec("insert into teacher(tno, tname, admin, password) "
                   "values ("
                   "'" + ui->Tno->text() + "', "
                   "'" + ui->Tname->text() + "', "
                   "" + QString::number(admin.checkedId()) + ", "
                   "'123456')");
        if(!query.isActive()){
            qDebug()<<query.lastError().text();
            ifwork = false;
        }
        if(ifwork){
            int row = 0;
            while(ui->class_table->item(row, 0)){
                if( ui->class_table->item(row, 0)->text() == ""){
                    row++;
                    continue;
                }
                QSqlQuery q(db);
                q.exec("update class "
                       "set tno = '" + ui->Tno->text() + "' "
                       "where tno = '' and classno = '" + ui->class_table->item(row, 0)->text() + "'");
                row++;
                if(!q.isActive()){
                    qDebug()<<q.lastError().text();
                    ifwork = false;
                    break;
                }
                if(q.numRowsAffected() == 0){
                    ifconflict = true;
                }
            }
            if(ifwork){
                int row = 0;
                while(ui->course_table->item(row, 0)){
                    if( ui->course_table->item(row, 0)->text() == ""){
                        row++;
                        continue;
                    }
                    QSqlQuery q(db);
                    q.exec("update course "
                           "set tno = '" + ui->Tno->text() + "' "
                           "where tno = '' and cno = '" + ui->course_table->item(row, 0)->text() + "'");
                    row++;
                    if(!q.isActive()){
                        qDebug()<<q.lastError().text();
                        ifwork = false;
                        break;
                    }
                    if(q.numRowsAffected() == 0){
                        ifconflict = true;
                    }
                }
            }
        }
        if(ifwork){
            if(ifconflict){
                QMessageBox box;
                box.setIcon(QMessageBox::Warning);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("师生管理");
                box.setText("新增无效");
                box.exec();
            }
            else {
                QMessageBox box;
                box.setIcon(QMessageBox::Information);
                box.setWindowIcon(QIcon(":/res/logo.png"));
                box.setWindowTitle("师生管理");
                box.setText("新增成功");
                box.exec();
                this->close();
                emit update();
            }
        }
        else {
            QMessageBox box;
            box.setIcon(QMessageBox::Critical);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("师生管理");
            box.setText("新增失败, 请注意数据正确性");
            box.exec();
        }
    }
        break;
    case CHECK:
    {
        this->close();
    }
    }
}
