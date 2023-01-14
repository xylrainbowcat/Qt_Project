#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QMainWindow>
#include "globel.h"
#include "modifyteacher.h"
#include "modifystudent.h"
#include "modifycourse.h"
#include "statics.h"
#include "changepassword.h"

//enum MANAGE_TABLE{
//    TEACHER_TABLE,
//    STUDENT_TABLE
//};

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit TeacherWindow(QSqlDatabase db, QString username, QWidget *parent = nullptr);
    ~TeacherWindow();

    void init_My();

    void init_Student();

    void init_Score();

    void init_Course();



    QString calculate_GPA(QString Sno);

signals:
    void back();

private slots:
    void on_Btn_Exit_clicked();

    void on_Btn_Modify_clicked();

    void on_Btn_My_clicked();

    void on_Btn_Student_clicked();

    void on_Btn_Score_clicked();

    void on_Btn_Dorm_clicked();

//    void on_Classno_currentTextChanged(const QString &arg1);

    void on_Btn_Search_clicked();

    void on_Btn_Save_clicked();

//    void on_Btn_Exchange_clicked();

//    void on_Boy_stateChanged(int arg1);

//    void on_Gril_stateChanged(int arg1);

    void on_Btn_Add_clicked();



    void on_Btn_stat_clicked();

    void on_Btn_Delete_clicked();

    void on_StudentTable_cellClicked(int row, int column);

    void on_Btn_Search_7_clicked();



    void on_student_radiobutton_toggled(bool checked);

    void on_teacher_radiobutton_toggled(bool checked);

    void TeacherManageRefresh();

    void StudentManageRefresh();

    void CourseManageRefresh();

    void on_CourseTable_cellClicked(int row, int column);

    void on_Btn_Search_2_clicked();

    void on_Btn_Modify_2_clicked();

    void on_Btn_Add_2_clicked();

    void on_Btn_Delete_2_clicked();

    void on_Cno_currentTextChanged(const QString &arg1);

    void on_Classno_2_currentTextChanged(const QString &arg1);

private:
    Ui::TeacherWindow *ui;

    QSqlDatabase db;

    QString username;

    int ts_ManageTable;


};

#endif // TEACHERWINDOW_H
