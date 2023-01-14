#ifndef MODIFYCOURSE_H
#define MODIFYCOURSE_H

#include <QMainWindow>
#include "globel.h"
namespace Ui {
class ModifyCourse;
}

class ModifyCourse : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModifyCourse(QSqlDatabase db, QString username, int model, QWidget *parent = nullptr);
    ~ModifyCourse();

    void init();

signals:
    void update();
private slots:
    void on_Btn_Save_clicked();

    void on_Btn_Cancel_clicked();

private:
    Ui::ModifyCourse *ui;

    QSqlDatabase db;

    QString username;

    int Model;
};

#endif // MODIFYCOURSE_H
