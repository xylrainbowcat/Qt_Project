#include "studentwindow.h"
#include "ui_studentwindow.h"

StudentWindow::StudentWindow(QSqlDatabase db, QString username, QWidget *parent) : QMainWindow(parent), ui(new Ui::StudentWindow) {
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

    ui->CourseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    // 表格随窗口调整大小
    ui->ScoreTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Time->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->Time->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->comboBox->view()->window()->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint|Qt::NoDropShadowWindowHint);
    ui->comboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);         // 下拉框圆角

    QSqlQuery t = db.exec("select grade from student where sno = '" + username + "'");    //设定学期
    t.next();
    currentTime = t.value(0).toString();

    for(int i = 8; i >= 0; i--){                                   //设定下拉框为不高于自己的学期
        if(ui->Time->itemText(i) != currentTime){
            ui->Time->removeItem(i);
            ui->comboBox->removeItem(i);
        }
        else break;
    }

    ui->Body->setCurrentIndex(0);                                                       // 初始化第一个窗口和成员变量
    this->db = db;
    this->username = username;

    init_My();                                                                          // 调用初始化函数
    init_Course();
    init_Score();
    init_SelectCourse();

}

StudentWindow::~StudentWindow() {
    delete ui;
}

// ======================================================================================= 初始化基本信息界面
void StudentWindow::init_My() {
    QSqlQuery query(db);
    query.exec("select sno, sname, sex, grade, major, classno "
               "from student "
               "where sno ='" + username + "'");

    query.next();
    ui->Sno->setText(query.value(0).toString());
    ui->Sname->setText(query.value(1).toString());
    ui->Sex->setText(query.value(2).toString());
    ui->Age->setText(query.value(3).toString());
    ui->Major->setText(query.value(4).toString());
    ui->Classno->setText(query.value(5).toString());

}

// ======================================================================================= 初始化课程查询界面
void StudentWindow::init_Course() {
    QSqlQuery query(db);
    query.exec("select sc.cno, cname, tname, grade, period, credit, assess, property "
               "from course, sc, teacher "
               "where sc.cno = course.cno and course.tno = teacher.tno and sno = '" + username + "'");

    QStringList headerlist;
    headerlist<<"课程号"<<"课程名"<<"任课老师"<<"开课学期"<<"学时"<<"学分"<<"考核方式"<<"课程属性";
    ui->CourseTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->CourseTable->columnCount(); ++i){
        ui->CourseTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));
    }
    ui->CourseTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->CourseTable->horizontalHeader()->setVisible(true);

    int row = 0;
    while (query.next()) {
        ui->CourseTable->insertRow(row);
        for (int i = 0; i < ui->CourseTable->columnCount(); ++i) {
            ui->CourseTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->CourseTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

// ======================================================================================= 初始化成绩查询界面
void StudentWindow::init_Score() {
    QSqlQuery query(db);
    query.exec("select sc.cno, cname, grade, score, period, credit, assess, property "
               "from course, sc "
               "where sc.cno = course.cno and sno = '" + username + "'");

    QStringList headerlist;
    headerlist<<"课程号"<<"课程名"<<"开课年级"<<"成绩"<<"学时"<<"学分"<<"考核方式"<<"课程属性";
    ui->ScoreTable->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->ScoreTable->columnCount(); ++i){
        ui->ScoreTable->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));
    }
    ui->ScoreTable->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->ScoreTable->horizontalHeader()->setVisible(true);

    int row = 0;
    double GPA = 0, credit = 0;
    while (query.next()) {
        ui->ScoreTable->insertRow(row);
        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }

        GPA += std::max(query.value(3).toDouble() - 50, 0.0) / 10 * query.value(5).toDouble();
        credit += query.value(5).toDouble();
    }

    if (credit)
        ui->GPA->setText(QString::number(GPA / credit, 'f', 3));
    else ui->GPA->setText("0.000");
}

// ======================================================================================= 搜索按钮
void StudentWindow::on_Btn_Search_clicked() {
    QString time = ui->Time->currentText();
    QString cname = ui->Cname->text();

    if (time == " 全部") time = "大";
    if (cname == "") cname = "%";

    QSqlQuery query(db);
    query.exec("select sc.cno, cname, tname, grade, period, credit, assess, property "
               "from course, sc, teacher "
               "where sc.cno = course.cno and course.tno = teacher.tno and sno = '" + username + "' "
                     "and grade like '%" + time + "%' and cname like '%" + cname + "%'");

    ui->CourseTable->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->CourseTable->clearContents();
    ui->CourseTable->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->CourseTable->insertRow(row);
        for (int i = 0; i < ui->CourseTable->columnCount(); ++i) {
            ui->CourseTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->CourseTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
    }

    ui->CourseTable->setSortingEnabled (true);
}

// ======================================================================================= 选择学期下拉框
void StudentWindow::on_comboBox_currentTextChanged(const QString &arg1) {
    qDebug() << arg1;

    QString time = arg1, property = "%";
    if (time == " 全部") time = "%";
    if (ui->Compulsory->isChecked()) property = "必修";

    QSqlQuery query(db);
    query.exec("select sc.cno, cname, grade, score, period, credit, assess, property "
               "from course, sc "
               "where sc.cno = course.cno and sno = '" + username + "' and grade like '" + time + "' and property like '" + property + "'");

    ui->ScoreTable->clearContents();
    ui->ScoreTable->setRowCount(0);
    int row = 0;
    double GPA = 0, credit = 0;
    while (query.next()) {
        ui->ScoreTable->insertRow(row);
        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }

        GPA += std::max(query.value(3).toDouble() - 50, 0.0) / 10 * query.value(5).toDouble();
        credit += query.value(5).toDouble();
    }

    if (credit)
        ui->GPA->setText(QString::number(GPA / credit, 'f', 3));
    else ui->GPA->setText("0.000");
}

// ======================================================================================= 只看必修
void StudentWindow::on_Compulsory_stateChanged(int arg1) {
    qDebug() << arg1;

    QString time = ui->comboBox->currentText(), property = arg1 ? "必修" : "%";
    if (time == " 全部") time = "%";

    QSqlQuery query(db);
    query.exec("select sc.cno, cname, grade, score, period, credit, assess, property "
               "from course, sc "
               "where sc.cno = course.cno and sno = '" + username + "' and grade like '" + time + "' and property like '" + property + "'");


    ui->ScoreTable->clearContents();
    ui->ScoreTable->setRowCount(0);
    int row = 0;
    double GPA = 0, credit = 0;
    while (query.next()) {
        ui->ScoreTable->insertRow(row);
        for (int i = 0; i < ui->ScoreTable->columnCount(); ++i) {
            ui->ScoreTable->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }

        GPA += std::max(query.value(3).toDouble() - 50, 0.0) / 10 * query.value(5).toDouble();
        credit += query.value(5).toDouble();
    }

    if (credit)
        ui->GPA->setText(QString::number(GPA / credit, 'f', 3));
    else ui->GPA->setText("0.000");
}


void StudentWindow::on_Btn_Exit_clicked() {
    this->close();
    emit back();
}


void StudentWindow::on_Btn_My_clicked() {
    if (ui->Body->currentIndex() != 0)
        ui->Body->setCurrentIndex(0);
}


void StudentWindow::on_Btn_Course_clicked() {
    if (ui->Body->currentIndex() != 1)
        ui->Body->setCurrentIndex(1);
}


void StudentWindow::on_Btn_Score_clicked() {
    if (ui->Body->currentIndex() != 2)
        ui->Body->setCurrentIndex(2);
}


void StudentWindow::on_Btn_Fee_clicked() {
    if (ui->Body->currentIndex() != 3)
        ui->Body->setCurrentIndex(3);
}


//    QAction *searchAction = new QAction(ui->Search);
//    searchAction->setIcon(QIcon(":/res/search.png"));
//    ui->Search->addAction(searchAction, QLineEdit::LeadingPosition);

// ======================================================================================= 初始化选课中心界面
void StudentWindow::init_SelectCourse(){


    QStringList headerlist;
    headerlist<<"课程号"<<"课程名"<<"任课老师"<<"开课学期"<<"学时"<<"学分"<<"考核方式"<<"课程属性";
    ui->ScoreTable_3->setColumnCount(headerlist.size());
    ui->ScoreTable_4->setColumnCount(headerlist.size());
    for (int i = 0; i < ui->ScoreTable_3->columnCount(); ++i){
        ui->ScoreTable_3->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));
        ui->ScoreTable_4->setHorizontalHeaderItem(i, new QTableWidgetItem(headerlist[i]));
    }
    ui->ScoreTable_3->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->ScoreTable_4->horizontalHeader()->setStyleSheet("QHeaderView::section{font: Bold 13pt '微软雅黑'}");
    ui->ScoreTable_3->horizontalHeader()->setVisible(true);
    ui->ScoreTable_4->horizontalHeader()->setVisible(true);
    ui->ScoreTable_3->setColumnCount(headerlist.size());
    ui->ScoreTable_4->setColumnCount(headerlist.size());

    SelectCourseRefresh();
    SelectedCourseRefresh();
}

// ======================================================================================= 查找
void StudentWindow::on_Btn_SearchCourse_clicked()
{
    QString cur_course = ui->Cname_2->text();
    if(cur_course == ""){
        SelectCourseRefresh();
        return ;
    }

    QSqlQuery query(db);
    bool k = query.exec("select cno, cname, tname, grade, period, credit, assess, property "
                          "from course, teacher "
                          "where course.tno = teacher.tno and "
                          "grade = '" + currentTime + "' and "
                          "cno like '%" + cur_course + "%' and "
                          "cno <>all( select cno "
                                      "from sc "
                                      "where sno = '" + username + "' )");
    if(k){
        ui->ScoreTable_3->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
        ui->ScoreTable_3->clearContents();
        ui->ScoreTable_3->setRowCount(0);


        int row = 0;
        while(query.next()){
            if(ui->ScoreTable_3->rowCount() <= row)ui->ScoreTable_3->insertRow(row);
            for(int i = 0; i < ui->ScoreTable_3->columnCount(); i++){
                ui->ScoreTable_3->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
                ui->ScoreTable_3->item(row, i)->setTextAlignment(Qt::AlignCenter);
            }
            row++;
        }

        ui->ScoreTable_3->setSortingEnabled (true);
    }
}

// ======================================================================================= 选课
void StudentWindow::on_Btn_SelectCourse_clicked()
{
    QString cur_course = ui->Cname_2->text();
    if(cur_course == ""){
        SelectCourseRefresh();
        return ;
    }
    QSqlQuery query(db);
    bool k = query.exec("select cno, cname, tname, grade, period, credit, assess, property "
                          "from course, teacher "
                          "where course.tno = teacher.tno and "
                          "grade = '" + currentTime + "' and "
                          "cno = '" + cur_course + "'");
    if(k && query.next()){

        QMessageBox box;
        box.setIcon(QMessageBox::Question);
        box.setWindowTitle("选课中心");
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setText("确定选择课程：" + cur_course + " 吗？");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(box.exec() == QMessageBox::Yes){
            QSqlQuery t = db.exec("insert into "
                                  "sc (sno, cno, score)"
                                  "values ( "
                                  "'" + username + "', "
                                  "'" + query.value(0).toString() + "', "
                                  " 0 )"   //默认分数0
                                  );
            QMessageBox box_2;
            if(t.isActive()){
                box_2.setIcon(QMessageBox::Information);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("选课中心");
                box_2.setText("选课成功");
                box_2.exec();
                SelectCourseRefresh();
                SelectedCourseRefresh();
            }
            else {
                box_2.setIcon(QMessageBox::Critical);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("选课中心");
                box_2.setText("选课失败");
                box_2.exec();
                qDebug()<<t.lastError().text();
            }
        }
    }
    else {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("选课中心");
        box.setText("未找到该课程");
        box.exec();
    }

}

void StudentWindow::on_Btn_deleteCourse_clicked()
{
    int currow;
    if(ui->ScoreTable_4->currentRow() < 0)
        return;
    else currow = ui->ScoreTable_4->currentRow();
    auto current_item = ui->ScoreTable_4->item(currow, 0);
    if(current_item == nullptr)return ;
    QString cur_course = current_item->text();
    QMessageBox box;
    box.setIcon(QMessageBox::Question);
    box.setWindowTitle("选课中心");
    box.setWindowIcon(QIcon(":/res/logo.png"));
    box.setText("确定删除课程：" + cur_course + " 吗？");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(box.exec() == QMessageBox::Yes){
        QSqlQuery query(db);
        query.exec("delete from sc "
                   "where sno = '" + username + "' and "
                         "cno = ( select cno "
                                    "from course "
                                    "where grade = '" + currentTime + "' and "
                                           "cno = '" + cur_course + "' )");
        qDebug()<<currentTime<<cur_course;
        QMessageBox box_2;
        if(query.isActive()){
            if(query.numRowsAffected() == 0){
                box_2.setIcon(QMessageBox::Warning);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("选课中心");
                box_2.setText("无法删除已选课程");
                box_2.exec();
                SelectCourseRefresh();
                SelectedCourseRefresh();
            }
            else {
                box_2.setIcon(QMessageBox::Information);
                box_2.setWindowIcon(QIcon(":/res/logo.png"));
                box_2.setWindowTitle("选课中心");
                box_2.setText("删除成功");
                box_2.exec();
                SelectCourseRefresh();
                SelectedCourseRefresh();
            }
        }
        else {
            box_2.setIcon(QMessageBox::Critical);
            box_2.setWindowIcon(QIcon(":/res/logo.png"));
            box_2.setWindowTitle("选课中心");
            box_2.setText("删除失败");
            box_2.exec();
            qDebug()<<query.lastError().text();
        }
    }

}

// ======================================================================================= 刷新已选表
void StudentWindow::SelectedCourseRefresh(){
    QSqlQuery query(db);
    query.exec("select course.cno, cname, tname, grade, period, credit, assess, property "
               "from course, teacher, sc "
               "where sc.cno = course.cno and "
                     "course.grade = '" + currentTime + "' and "
                     "sc.sno = '" + username + "' and "
                     "course.tno = teacher.tno"
                        );
    qDebug()<<query.lastError().text();
    ui->ScoreTable_4->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->ScoreTable_4->clearContents();
    ui->ScoreTable_4->setRowCount(0);

    int row = 0;
    while(query.next()){
        if(ui->ScoreTable_4->rowCount() <= row)ui->ScoreTable_4->insertRow(row);
        for(int i = 0; i < ui->ScoreTable_4->columnCount(); i++){
            ui->ScoreTable_4->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable_4->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
        row++;
    }

    ui->ScoreTable_4->setSortingEnabled (true);
}

// ======================================================================================= 刷新选课表
void StudentWindow::SelectCourseRefresh(){
    QSqlQuery query(db);
    query.exec("select cno, cname, tname, grade, period, credit, assess, property "
               "from course, teacher "
               "where course.tno = teacher.tno and "
                           "grade = '" + currentTime + "' and "
                           "cno <>all( select cno "
                                       "from sc "
                                       "where sno = '" + username + "' )");
    ui->ScoreTable_3->setSortingEnabled (false);                                        // 解决关于排序后查询错乱的问题
    ui->ScoreTable_3->clearContents();
    ui->ScoreTable_3->setRowCount(0);

    int row = 0;
    while(query.next()){
        if(ui->ScoreTable_3->rowCount() <= row)ui->ScoreTable_3->insertRow(row);
        for(int i = 0; i < ui->ScoreTable_3->columnCount(); i++){
            ui->ScoreTable_3->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            ui->ScoreTable_3->item(row, i)->setTextAlignment(Qt::AlignCenter);
        }
        row++;
    }

    ui->ScoreTable_3->setSortingEnabled (true);
}

void StudentWindow::on_ScoreTable_3_cellClicked(int row, int column)
{
    column = 0;
    if(ui->ScoreTable_3->item(row, column) != nullptr)
        ui->Cname_2->setText(ui->ScoreTable_3->item(row, column)->text());
}

void StudentWindow::on_Btn_Search_7_clicked()
{
    changePassword * password = new changePassword(db, username, STUDENT);
    password->setWindowModality(Qt::ApplicationModal);
    password->show();
}

void StudentWindow::on_Time_currentTextChanged(const QString &arg1)
{
    ui->Cname->setText("");
    on_Btn_Search_clicked();
}

