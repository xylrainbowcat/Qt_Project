#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(QSqlDatabase db, QString username, QWidget *parent) : QMainWindow(parent), ui(new Ui::TeacherWindow) {
    ui->setupUi(this);

    setWindowIcon(QIcon(":/res/logo.png"));                                             // 设置窗口样式
    setWindowTitle("江苏科技大学学生管理系统");
    this->setStyleSheet("background-color: rgb(255, 255, 255);");
    resize(1170, 672);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);     // 侧边栏阴影效果
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(QColor(38, 78, 119, 127));
    shadow_effect->setBlurRadius(22);
    ui->Sidebar->setGraphicsEffect(shadow_effect);


    ui->Body->setCurrentIndex(0);                                                       // 初始化第一个窗口和成员变量
    this->db = db;
    this->username = username;

    //默认普通模式，隐藏管理员相关权限
    ui->Btn_Student->hide();
    ui->Btn_Dorm->hide();

    ui->StudentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);   // 表格随窗口调整大小
    ui->ScoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->CourseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    ui->Classno->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
//    ui->Classno->view()->window()->setAttribute(Qt::WA_TranslucentBackground);          // 下拉框圆角
    ui->Classno_2->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->Classno_2->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->Cno->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->Cno->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
//    ui->Bednum->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
//    ui->Bednum->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
//    ui->Bednum_2->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
//    ui->Bednum_2->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
//    ui->Dorm->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
//    ui->Dorm->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
//    ui->Dorm_2->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
//    ui->Dorm_2->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    QSqlQuery query(db);                                                                // 初始化下拉框选项
    query.exec("select classno from class where tno = '" + username + "'");
    while (query.next()) {
//        ui->Classno->addItem(query.value(0).toString());
        ui->Classno_2->addItem(query.value(0).toString());
    }

    query.exec("select cno from course where tno = '" + username + "'");
    while (query.next()) {
        ui->Cno->addItem(query.value(0).toString());
    }

    init_My();                                                                          // 调用初始化函数
    init_Score();
    QSqlQuery query_manager(db);
    query_manager.exec("select admin "
                       "from teacher "
                       "where tno = '" + username + "'");
    if(query_manager.next() && query_manager.value(0).toInt() == 1){
        ui->Btn_Student->show();
        ui->Btn_Dorm->show();
        init_Student();
        init_Course();
    }
    else {
        qDebug()<<query_manager.lastError().text();
    }

}

TeacherWindow::~TeacherWindow() {
    delete ui;
}

// ======================================================================================= 初始化基本信息界面
void TeacherWindow::init_My() {
    QSqlQuery query(db);
    query.exec("select tno, tname from teacher where tno = '" + username + "'");

    query.next();
    ui->Tno->setText(query.value(0).toString());
    ui->Tname->setText(query.value(1).toString());
    ui->Class->clear();
    ui->Cname->clear();
    query.exec("select major, classno from class where tno = '" + username + "'");
    if (query.next()) ui->Class->insertPlainText(query.value(0).toString() + "(" + query.value(1).toString() + ")");
    while (query.next()) {
        ui->Class->insertPlainText("、" + query.value(0).toString() + "(" + query.value(1).toString() + ")");
    }

    query.exec("select cname, cno from course where tno = '" + username + "'");
    if (query.next()) ui->Cname->insertPlainText(query.value(0).toString() + "(" + query.value(1).toString() + ")");
    while (query.next()) {
        ui->Cname->insertPlainText("、" + query.value(0).toString() + "(" + query.value(1).toString() + ")");
    }
}

// ======================================================================================= 初始化师生管理界面
void TeacherWindow::init_Student() {
    ui->student_radiobutton->setChecked(true);
    ui->Search->setPlaceholderText("请输入学号");
    ui->Search->clear();
    StudentManageRefresh();
}

// ======================================================================================= 初始化成绩录入界面
void TeacherWindow::init_Score() {

    QStringList headerlist;
    headerlist<<"学号"<<"姓名"<<"专业"<<"课程名"<<"成绩";
    ui->ScoreTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->ScoreTable->columnCount(); ++i){
        ui->ScoreTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));

    }
    ui->ScoreTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->ScoreTable->horizontalHeader()->setVisible(true);

//    QString cno = ui->Cno->currentText();

//    QSqlQuery query(db);
//    query.exec("select distinct student.sno, sname, major, cname, score, course.grade "
//               "from student, sc, course "
//               "where student.sno = SC.sno and course.cno = SC.cno and SC.cno = '" + cno + "'");
//    int row = 0;
//    ui->ScoreTable->clearContents();
//    ui->ScoreTable->setRowCount(0);
//    while (query.next()) {
//        ui->ScoreTable->insertRow(row);
//        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
//            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
//            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
//            if (i != 4) ui->ScoreTable->item(row, i)->setFlags(Qt::ItemIsEnabled);        // 设置成绩列为可修改
//            if(query.value(5).toString() != currentTime && ui->ScoreTable->item(row, 4))                            //本学期的课程才可而修改
//                ui->ScoreTable->item(row, 4)->setFlags(Qt::ItemIsEnabled);
//        }
//        row++;
//    }

}

// ======================================================================================= 初始化宿舍管理界面
//void TeacherWindow::init_Dorm() {
//    QSqlQuery query(db);
//    query.exec("select distinct dormitory "
//               "from student, class "
//               "where student.classno = class.classno and tno = '" + username + "'");

//    QStringList headerlist;
//    headerlist<<"宿舍号"<<"1号床"<<"2号床"<<"3号床"<<"4号床";
//    ui->DormTable->setColumnCount(headerlist.size());
//    for (int i = 0; i < ui->DormTable->columnCount(); ++i){
//        ui->DormTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));

//    }
//    ui->DormTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
//    ui->DormTable->horizontalHeader()->setVisible(true);

//    int row = 0;
//    while (query.next()) {
//        ui->DormTable->insertRow(row);
//        ui->DormTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
//        ui->DormTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
//        ui->Dorm->addItem(query.value(0).toString());
//        ui->Dorm_2->addItem(query.value(0).toString());

//        QSqlQuery q(db);
//        q.exec("select sname, bednum from student where dormitory = '" + query.value(0).toString() +  "'");

//        while (q.next()) {
//            if (0 < q.value(1).toInt() && q.value(1).toInt() < 5) {
//                ui->DormTable->setItem(row, q.value(1).toInt(), new QTableWidgetItem(q.value(0).toString()));
//                ui->DormTable->item(row, q.value(1).toInt())->setTextAlignment(Qt::AlignCenter);
//            }
//        }
//        row++;
//    }
//}

// ======================================================================================= 初始化课程管理界面
void TeacherWindow::init_Course() {

    ui->Search_2->setPlaceholderText("请输入课程号");
    ui->Search_2->clear();
    CourseManageRefresh();
}

// ======================================================================================= 计算绩点
QString TeacherWindow::calculate_GPA(QString Sno) {
    QSqlQuery query(db);
    query.exec("select score, credit "
               "from course, sc "
               "where sc.cno = course.cno and sno = '" + Sno + "'");

    double GPA = 0, credit = 0;
    while (query.next()) {
        GPA += std::max(query.value(0).toDouble() - 50, 0.0) / 10 * query.value(1).toDouble();
        credit += query.value(1).toDouble();
    }

    if (credit)
        return QString::number(GPA / credit, 'f', 3);

    return "0.000";
}

// ======================================================================================= 修改按钮
void TeacherWindow::on_Btn_Modify_clicked() {
    QSqlQuery query(db);
    if(ui->Search->text() == "")return ;
    if(ui->student_radiobutton->isChecked()){
        query.exec("select sno "
                   "from student "
                   "where sno like '%" + ui->Search->text() + "%'");
        if(query.next()){
            ModifyStudent *modify = new ModifyStudent(db, query.value(0).toString(), MODIFY);
            modify->setWindowModality(Qt::ApplicationModal);
            modify->show();

            connect(modify, &ModifyStudent::update, this, [=]() {                           // 更新表格
                StudentManageRefresh();
            });

        }
        else{
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("未找到该学生");
            box.exec();
        }
    }
    else {
        query.exec("select tno "
                   "from teacher "
                   "where tno like '%" + ui->Search->text() + "%'");
        if(query.next()){
            ModifyTeacher *modify = new ModifyTeacher(db, query.value(0).toString(), MODIFY);
            modify->setWindowModality(Qt::ApplicationModal);
            modify->show();

            connect(modify, &ModifyTeacher::update, this, [=]() {                           // 更新表格
                TeacherManageRefresh();
            });
        }
        else{
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("未找到该教师");
            box.exec();
        }
    }

}

// ======================================================================================= 成绩录入选择班级下拉框
void TeacherWindow::on_Classno_2_currentTextChanged(const QString &arg1) {
    qDebug() << arg1;

    QString classno = arg1 == "全部" ? "%" : arg1;
    QString cno = ui->Cno->currentText() == "全部" ? "%" : ui->Cno->currentText();
    if(cno == "%" && classno == "%"){
        ui->ScoreTable->clearContents();
        ui->ScoreTable->setRowCount(0);
        return ;
    }
    QSqlQuery query(db);
    query.exec("select distinct student.sno, sname, major, cname, score, course.grade, course.cno, student.grade "
               "from student, sc, class, course "
               "where student.sno = sc.sno and "
               "course.cno = sc.cno and "
               "student.classno like '" + classno + "' and "
               "course.cno like '" +cno + "'");
    qDebug()<<query.lastError().text();
    ui->ScoreTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->ScoreTable->clearContents();
    ui->ScoreTable->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->ScoreTable->insertRow(row);
        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
            if (i != 4) ui->ScoreTable->item(row, i)->setFlags(Qt::ItemIsEnabled);        // 设置成绩列为可修改
            if(query.value(5).toString() != query.value(7).toString() && ui->ScoreTable->item(row, 4))  //本学期的课程才可而修改
                ui->ScoreTable->item(row, 4)->setFlags(Qt::ItemIsEnabled);
            if(cno == "%"){                                                                       //只有自己的课可以修改
                QString curcno = query.value(6).toString();
                QSqlQuery t = db.exec("select cno "
                                 "from course "
                                 "where tno = '" + username + "' and cno = '" + curcno + "'");
                if(!t.next()&& ui->ScoreTable->item(row, 4))
                    ui->ScoreTable->item(row, 4)->setFlags(Qt::ItemIsEnabled);
            }
        }
        row++;
    }
    ui->ScoreTable->setSortingEnabled (true);
}

// ======================================================================================= 成绩录入选择课程下拉框
void TeacherWindow::on_Cno_currentTextChanged(const QString &arg1)
{
    qDebug() << arg1;

    QString cno = arg1 == "全部" ? "%" : arg1;
    QString classno = ui->Classno_2->currentText() == "全部" ? "%" : ui->Classno_2->currentText();
    if(cno == "%" && classno == "%"){
        ui->ScoreTable->clearContents();
        ui->ScoreTable->setRowCount(0);
        return ;
    }
    QSqlQuery query(db);
    query.exec("select distinct student.sno, sname, major, cname, score, course.grade, course.cno, student.grade "
               "from student, sc, class, course "
               "where student.sno = sc.sno and "
               "course.cno = sc.cno and "
               "student.classno like '" + classno + "' and "
               "course.cno like '" + cno + "'");

    ui->ScoreTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    int row = 0;
    ui->ScoreTable->clearContents();
    ui->ScoreTable->setRowCount(0);
    while (query.next()) {
        ui->ScoreTable->insertRow(row);
        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
            if (i != 4) ui->ScoreTable->item(row, i)->setFlags(Qt::ItemIsEnabled);        // 设置成绩列为可修改
            if(query.value(5).toString() != query.value(7).toString() && ui->ScoreTable->item(row, 4)){ //只有本学期的课才能修改
                ui->ScoreTable->item(row, 4)->setFlags(Qt::ItemIsEnabled);
            }
            if(cno == "%"){                                                                       //只有自己的课可以修改
                QString curcno = query.value(6).toString();
                qDebug()<<"##"<<curcno;
                QSqlQuery t = db.exec("select cno "
                                 "from course "
                                 "where tno = '" + username + "' and cno = '" + curcno + "'");
                if(!t.next()&& ui->ScoreTable->item(row, 4))
                    ui->ScoreTable->item(row, 4)->setFlags(Qt::ItemIsEnabled);
            }

        }
        row++;
    }
    ui->StudentTable->setSortingEnabled (true);

}

// ======================================================================================= 查询按钮
void TeacherWindow::on_Btn_Search_clicked() {
    QSqlQuery query(db);
    if(ui->Search->text() == ""){
        ui->student_radiobutton->isChecked()?StudentManageRefresh():TeacherManageRefresh();
        return ;
    }
    if(ui->student_radiobutton->isChecked()){
        query.exec("select sno "
                   "from student "
                   "where sno like '%" + ui->Search->text() + "%'");
        if(query.next()){
            ModifyStudent * modify = new ModifyStudent(db, query.value(0).toString(), CHECK);
            modify->setWindowModality(Qt::ApplicationModal);
            modify->show();

            connect(modify, &ModifyStudent::update, this, [=]() {                           // 更新表格
                StudentManageRefresh();
            });
        }
        else{
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("未找到该学生");
            box.exec();
        }
    }
    else {
        query.exec("select tno "
                   "from teacher "
                   "where tno like '%" + ui->Search->text() + "%'");
        if(query.next()){
            ModifyTeacher * modify = new ModifyTeacher(db, query.value(0).toString(), CHECK);
            modify->setWindowModality(Qt::ApplicationModal);
            modify->show();

            connect(modify, &ModifyTeacher::update, this, [=]() {                           // 更新表格
                TeacherManageRefresh();
            });
        }
        else{
            QMessageBox box;
            box.setIcon(QMessageBox::Warning);
            box.setWindowIcon(QIcon(":/res/logo.png"));
            box.setWindowTitle("警告");
            box.setText("未找到该教师");
            box.exec();
        }
    }


//    ui->StudentTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
//    ui->StudentTable->clearContents();                                                  // 清空表
//    ui->StudentTable->setRowCount(0);
//    int row = 0;
//    while (query.next()) {
//        ui->StudentTable->insertRow(row);
//        for (int i = 0; i < ui->StudentTable->columnCount(); ++i) {
//                if(i == 2 && ui->teacher_radiobutton->isChecked()){
//                    ui->StudentTable->setItem(row, i, new QTableWidgetItem(query.value(i).toInt()==1?"是":"否"));
//                }
//                else
//                ui->StudentTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
//                ui->StudentTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
//        }
//    }
//    ui->StudentTable->setSortingEnabled (true);


}

// ======================================================================================= 保存按钮
void TeacherWindow::on_Btn_Save_clicked() {
    QSqlQuery query(db);
    bool k = true;
    for (int i = 0; i < ui->ScoreTable->rowCount(); ++i) {
        query.exec("update SC set score = '" + ui->ScoreTable->item(i, 4)->text() + "' "
                        "where sno = '" + ui->ScoreTable->item(i, 0)->text() + "'");
        if(!query.isActive())k =false;
    }
    QMessageBox box;
    if(k){
        box.setIcon(QMessageBox::Information);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("保存");
        box.setText("保存成功");
    }
    else {
        box.setIcon(QMessageBox::Critical);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("保存失败");
        qDebug()<<query.lastError().text();
    }
    box.exec();
}

// ======================================================================================= 交换宿舍按钮
//void TeacherWindow::on_Btn_Exchange_clicked() {
//    QString dorm = ui->Dorm->currentText();
//    QString bednum = ui->Bednum->currentText();
//    QString dorm_2 = ui->Dorm_2->currentText();
//    QString bednum_2 = ui->Bednum_2->currentText();

//    QString sex1 = ui->Boy->isChecked() ? "无": "男";
//    QString sex2 = ui->Gril->isChecked() ? "无": "女";

//    QSqlQuery query(db);
//    query.exec("update student set dormitory = '0-A000', bednum = 0 "
//               "where dormitory = '" + dorm + "' and bednum = '" + bednum + "'");
//    query.exec("update student set dormitory = '" + dorm + "', bednum = '" + bednum + "' "
//               "where dormitory = '" + dorm_2 + "' and bednum = '" + bednum_2 + "'");
//    query.exec("update student set dormitory = '" + dorm_2 + "', bednum = '" + bednum_2 + "' "
//               "where dormitory = '0-A000' and bednum = 0");

//    query.exec("select distinct dormitory "
//               "from student, class "
//               "where student.classno = class.classno and tno = '" + username + "' and sex <> '" + sex1 + "' and sex <> '" + sex2 + "'");

//    ui->DormTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
//    ui->DormTable->clearContents();                                                  // 清空表
//    ui->DormTable->setRowCount(0);
//    int row = 0;
//    while (query.next()) {
//        ui->DormTable->insertRow(row);
//        ui->DormTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
//        ui->DormTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
//        QSqlQuery q(db);
//        q.exec("select sname, bednum from student where dormitory = '" + query.value(0).toString() +  "'");

//        while (q.next()) {
//            if (0 < q.value(1).toInt() && q.value(1).toInt() < 5) {
//                ui->DormTable->setItem(row, q.value(1).toInt(), new QTableWidgetItem(q.value(0).toString()));
//                ui->DormTable->item(row, q.value(1).toInt())->setTextAlignment(Qt::AlignCenter);
//            }
//        }
//    }
//    ui->DormTable->setSortingEnabled (true);
//}

// ======================================================================================= 筛选 - 男
//void TeacherWindow::on_Boy_stateChanged(int arg1) {
//    QString sex1 = arg1 ? "无": "男";
//    QString sex2 = ui->Gril->isChecked() ? "无": "女";

//    QSqlQuery query(db);
//    query.exec("select distinct dormitory "
//               "from student, class "
//               "where student.classno = class.classno and tno = '" + username + "' and sex <> '" + sex1 + "' and sex <> '" + sex2 + "'");

//    ui->DormTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
//    ui->DormTable->clearContents();                                                  // 清空表
//    ui->DormTable->setRowCount(0);
//    int row = 0;
//    while (query.next()) {
//        ui->DormTable->insertRow(row);
//        ui->DormTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
//        ui->DormTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
//        QSqlQuery q(db);
//        q.exec("select sname, bednum from student where dormitory = '" + query.value(0).toString() +  "'");

//        while (q.next()) {
//            if (0 < q.value(1).toInt() && q.value(1).toInt() < 5) {
//                ui->DormTable->setItem(row, q.value(1).toInt(), new QTableWidgetItem(q.value(0).toString()));
//                ui->DormTable->item(row, q.value(1).toInt())->setTextAlignment(Qt::AlignCenter);
//            }
//        }
//    }
//    ui->DormTable->setSortingEnabled (true);
//}

// ======================================================================================= 筛选 - 女
//void TeacherWindow::on_Gril_stateChanged(int arg1) {
//    QString sex1 = ui->Boy->isChecked() ? "无": "男";
//    QString sex2 = arg1 ? "无": "女";

//    QSqlQuery query(db);
//    query.exec("select distinct dormitory "
//               "from student, class "
//               "where student.classno = class.classno and tno = '" + username + "' and sex <> '" + sex1 + "' and sex <> '" + sex2 + "'");

//    ui->DormTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
//    ui->DormTable->clearContents();                                                  // 清空表
//    ui->DormTable->setRowCount(0);
//    int row = 0;
//    while (query.next()) {
//        ui->DormTable->insertRow(row);
//        ui->DormTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
//        ui->DormTable->item(row, 0)->setTextAlignment(Qt::AlignCenter);
//        QSqlQuery q(db);
//        q.exec("select sname, bednum from student where dormitory = '" + query.value(0).toString() +  "'");

//        while (q.next()) {
//            if (0 < q.value(1).toInt() && q.value(1).toInt() < 5) {
//                ui->DormTable->setItem(row, q.value(1).toInt(), new QTableWidgetItem(q.value(0).toString()));
//                ui->DormTable->item(row, q.value(1).toInt())->setTextAlignment(Qt::AlignCenter);
//            }
//        }
//    }
//    ui->DormTable->setSortingEnabled (true);
//}


void TeacherWindow::on_Btn_My_clicked() {
    if (ui->Body->currentIndex() != 0)
        ui->Body->setCurrentIndex(0);
    init_My();
}

void TeacherWindow::on_Btn_Student_clicked() {
    if (ui->Body->currentIndex() != 1)
        ui->Body->setCurrentIndex(1);
    init_Student();
}

void TeacherWindow::on_Btn_Score_clicked() {
    if (ui->Body->currentIndex() != 2)
        ui->Body->setCurrentIndex(2);
    init_Score();
}

void TeacherWindow::on_Btn_Dorm_clicked() {
    if (ui->Body->currentIndex() != 3)
        ui->Body->setCurrentIndex(3);
    init_Course();
}

void TeacherWindow::on_Btn_Exit_clicked() {
    this->close();
    emit back();
}

// ======================================================================================= 新增按钮
void TeacherWindow::on_Btn_Add_clicked()
{
    if(ui->student_radiobutton->isChecked()){
        ModifyStudent * modify = new ModifyStudent(db, "", ADD);
        modify->setWindowModality(Qt::ApplicationModal);
        modify->show();
        connect(modify, &ModifyStudent::update, this, [=]() {                           // 更新表格
            StudentManageRefresh();
        });
    }
    else {
        ModifyTeacher * modify = new ModifyTeacher(db, "", ADD);
        modify->setWindowModality(Qt::ApplicationModal);
        modify->show();
        connect(modify, &ModifyTeacher::update, this, [=]() {                           // 更新表格
            TeacherManageRefresh();
        });
    }


//    ModifyStudent *modify = new ModifyStudent(db, "", ADD);
//    modify->setWindowModality(Qt::ApplicationModal);
//    modify->show();


}


// ======================================================================================= 统计按钮
void TeacherWindow::on_Btn_stat_clicked()
{
    QString cno = ui->Cno->currentText();
    Statics *statics = new Statics(db, cno);
    statics->setWindowModality(Qt::ApplicationModal);
    statics->show();
}

// ======================================================================================= 删除按钮
void TeacherWindow::on_Btn_Delete_clicked()
{
    QSqlQuery query(db);
    if(ui->student_radiobutton->isChecked()){
        query.exec("select sno "
                   "from student "
                   "where sno = '" + ui->Search->text() + "'");
    }
    else {
        query.exec("select tno "
                   "from teacher "
                   "where tno = '" + ui->Search->text() + "'");
    }

    QMessageBox box;
    if (query.next()) {
        box.setIcon(QMessageBox::Question);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("确定删除该记录吗？");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(box.exec() == QMessageBox::Yes){



            QSqlQuery t(db);
            if(ui->student_radiobutton->isChecked()){
                t.exec("delete "
                       "from student "
                       "where sno = '" + query.value(0).toString() + "' ");
                t.exec("delete "
                       "from sc "
                       "where sno = '" + query.value(0).toString() + "'");   //同步删除sc表记录
            }
            else {
                t.exec("delete "
                       "from teacher "
                       "where tno = '" + query.value(0).toString() + "' ");
                t.exec("update class "
                       "set tno = '' where tno = '" + query.value(0).toString() + "' ");
                t.exec("update course "
                       "set tno = '' where tno = '" + query.value(0).toString() + "' ");   //同步删除class和course表记录

            }
            QMessageBox box_2;
            if(t.isActive()){
                box_2.setIcon(QMessageBox::Information);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("师生管理");
                box_2.setText("记录已删除");
                box_2.exec();
                db.commit();
//                on_Classno_currentTextChanged(ui->Classno->currentText());
            }
            else{
                box_2.setIcon(QMessageBox::Critical);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("警告");
                box_2.setText("删除失败");
                box_2.exec();
                qDebug()<<t.lastError().text();
            }

        }
    }
    else {
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("无效的记录");
        box.exec();
    }
    if(ui->student_radiobutton->isChecked()){
        StudentManageRefresh();
    }
    else {
        TeacherManageRefresh();
    }
}

void TeacherWindow::on_StudentTable_cellClicked(int row, int column)
{
    column = 0;
    if(ui->StudentTable->item(row, column) != nullptr)
        ui->Search->setText(ui->StudentTable->item(row, column)->text());
}

void TeacherWindow::on_CourseTable_cellClicked(int row, int column)
{
    column = 0;
    if(ui->CourseTable->item(row, column) != nullptr)
        ui->Search_2->setText(ui->CourseTable->item(row, column)->text());
}

// ======================================================================================= 修改密码
void TeacherWindow::on_Btn_Search_7_clicked()
{
    changePassword * password = new changePassword(db, username, TEACHER);
    password->setWindowModality(Qt::ApplicationModal);
    password->show();
}


// ======================================================================================= 刷新教师管理表
void TeacherWindow::TeacherManageRefresh(){
//    ui->Search->setPlaceholderText("请输入教师职工号");

    QSqlQuery query(db);
    query.exec("select tno, tname, admin "
               "from teacher ");

    QStringList headerlist;   //设置表头
    headerlist<<"教师工号"<<"姓名"<<"是否为管理员";
    ui->StudentTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->StudentTable->columnCount(); ++i){
        ui->StudentTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));

    }
    ui->StudentTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->StudentTable->horizontalHeader()->setVisible(true);

    db.exec("delete from teacher where tno = ''");

    ui->StudentTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->StudentTable->clearContents();
    ui->StudentTable->setRowCount(0);
    int row = 0;
    ui->StudentTable->setRowCount(0);
    while (query.next()) {
        ui->StudentTable->insertRow(row);
        for (int i = 0; i < ui->StudentTable->columnCount(); ++i) {
            if(i == 2){
                ui->StudentTable->setItem(row, i, new QTableWidgetItem(query.value(i).toInt()==1?"是":"否"));
            }
            else
            ui->StudentTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->StudentTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
        row++;
    }

    ui->StudentTable->setSortingEnabled (true);
}
// ======================================================================================= 刷新学生管理表
void TeacherWindow::StudentManageRefresh(){


    QSqlQuery query(db);
    query.exec("select * "
               "from student ");

    QStringList headerlist;   //设置表头
    headerlist<<"学号"<<"姓名"<<"性别"<<"年级"<<"专业"<<"班级号";
    ui->StudentTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->StudentTable->columnCount(); ++i){
        ui->StudentTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));

    }
    ui->StudentTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->StudentTable->horizontalHeader()->setVisible(true);

    ui->StudentTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->StudentTable->clearContents();
    ui->StudentTable->setRowCount(0);

    int row = 0;
    ui->StudentTable->setRowCount(0);
    while (query.next()) {
        ui->StudentTable->insertRow(row);
        for (int i = 0; i < ui->StudentTable->columnCount(); ++i) {
                ui->StudentTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
                ui->StudentTable->item(row, i)->setTextAlignment(Qt::AlignCenter);

        }
        row++;
    }

    ui->StudentTable->setSortingEnabled (true);
}

// ======================================================================================= 刷新课程管理表
void TeacherWindow::CourseManageRefresh(){
    QSqlQuery query(db);
    query.exec("select * "
               "from course ");

    QStringList headerlist;   //设置表头
    headerlist<<"课程号"<<"课程名"<<"教师职工号"<<"开课年级"<<"学时"<<"学分"<<"考查方式"<<"属性";
    ui->CourseTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->CourseTable->columnCount(); ++i){
        ui->CourseTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));

    }
    ui->CourseTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->CourseTable->horizontalHeader()->setVisible(true);


    ui->CourseTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->CourseTable->clearContents();
    ui->CourseTable->setRowCount(0);

    int row = 0;
    ui->CourseTable->setRowCount(0);
    while (query.next()) {
        ui->CourseTable->insertRow(row);
        for (int i = 0; i < ui->CourseTable->columnCount(); ++i) {
                ui->CourseTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
                ui->CourseTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
        row++;
    }

    ui->CourseTable->setSortingEnabled (true);
}



void TeacherWindow::on_student_radiobutton_toggled(bool)
{
    ui->Search->clear();
    ui->Search->setPlaceholderText("请输入学号");
    StudentManageRefresh();
}

void TeacherWindow::on_teacher_radiobutton_toggled(bool)
{
    ui->Search->clear();
    ui->Search->setPlaceholderText("输入职工号");
    TeacherManageRefresh();
}


// ======================================================================================= 课程管理查询
void TeacherWindow::on_Btn_Search_2_clicked()
{
    if(ui->Search_2->text() == ""){
        CourseManageRefresh();
        return ;
    }
    QSqlQuery query(db);
    query.exec("select cno "
            "from course "
            "where cno like '%" + ui->Search_2->text() + "%'");
    if(query.next()){
        ModifyCourse * modify = new ModifyCourse(db, query.value(0).toString(), CHECK);
        modify->setWindowModality(Qt::ApplicationModal);
        modify->show();

        connect(modify, &ModifyCourse::update, this, [=]() {                           // 更新表格
           CourseManageRefresh();
        });
    }
    else{
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("未找到该课程");
        box.exec();
    }
}
// ======================================================================================= 课程管理修改
void TeacherWindow::on_Btn_Modify_2_clicked()
{
    if(ui->Search_2->text() == "")return ;
    QSqlQuery query(db);
    query.exec("select cno "
            "from course "
            "where cno like '%" + ui->Search_2->text() + "%'");
    if(query.next()){
        ModifyCourse * modify = new ModifyCourse(db, query.value(0).toString(), MODIFY);
        modify->setWindowModality(Qt::ApplicationModal);
        modify->show();

        connect(modify, &ModifyCourse::update, this, [=]() {                           // 更新表格
           CourseManageRefresh();
        });
    }
    else{
        QMessageBox box;
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("未找到该课程");
        box.exec();
    }
}
// ======================================================================================= 课程管理新增
void TeacherWindow::on_Btn_Add_2_clicked()
{
    ModifyCourse * modify = new ModifyCourse(db, "", ADD);
    modify->setWindowModality(Qt::ApplicationModal);
    modify->show();

    connect(modify, &ModifyCourse::update, this, [=]() {                           // 更新表格
       CourseManageRefresh();
    });
}
// ======================================================================================= 课程管理删除
void TeacherWindow::on_Btn_Delete_2_clicked()
{
    QSqlQuery query(db);
    query.exec("select cno "
            "from course "
            "where cno like '" + ui->Search_2->text() + "'");

    QMessageBox box;
    if (query.next()) {
        box.setIcon(QMessageBox::Question);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("确定删除该记录吗？");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(box.exec() == QMessageBox::Yes){
            QSqlQuery t(db);
            t.exec("delete from course "
                   "where cno = '" + query.value(0).toString() + "'");
            t.exec("delete from sc "
                   "where cno = '" + query.value(0).toString() + "'");
            QMessageBox box_2;
            if(t.isActive()){
                box_2.setIcon(QMessageBox::Information);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("课程管理");
                box_2.setText("记录已删除");
                box_2.exec();
                db.commit();
//                on_Classno_currentTextChanged(ui->Classno->currentText());
            }
            else{
                box_2.setIcon(QMessageBox::Critical);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("警告");
                box_2.setText("删除失败");
                box_2.exec();
                qDebug()<<t.lastError().text();
            }
        }
    }
    else {
        box.setIcon(QMessageBox::Warning);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("无效的记录");
        box.exec();
    }
    CourseManageRefresh();
}


