#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include "globel.h"
#include "changepassword.h"
namespace Ui {
class StudentWindow;
}

class StudentWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StudentWindow(QSqlDatabase db, QString username, QWidget *parent = nullptr);
    ~StudentWindow();

    void init_My();

    void init_Course();

    void init_Score();

    void init_SelectCourse();

    void SelectCourseRefresh();

    void SelectedCourseRefresh();

signals:
    void back();

private slots:
    void on_Btn_My_clicked();

    void on_Btn_Course_clicked();

    void on_Btn_Score_clicked();

    void on_Btn_Fee_clicked();

    void on_Btn_Search_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_Compulsory_stateChanged(int arg1);

    void on_Btn_Exit_clicked();

    void on_Btn_SearchCourse_clicked();

    void on_Btn_SelectCourse_clicked();

    void on_Btn_deleteCourse_clicked();

    void on_ScoreTable_3_cellClicked(int row, int column);

    void on_Btn_Search_7_clicked();


    void on_Time_currentTextChanged(const QString &arg1);

private:
    Ui::StudentWindow *ui;

    QSqlDatabase db;

    QString username;

    QString currentTime;
};

#endif // STUDENTWINDOW_H
