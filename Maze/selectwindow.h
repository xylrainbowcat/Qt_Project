#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"

namespace Ui {
class SelectWindow;
}

class SelectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectWindow(QWidget *parent = nullptr);
    ~SelectWindow();

    void start(int x);

signals:
    void back();

private slots:
    void on_btn1_clicked();

    void on_btn2_clicked();

    void on_btn3_clicked();

private:
    Ui::SelectWindow *ui;

    GameWindow * game = NULL;
    QTimer timer;
    int idx;
};

#endif // SELECTWINDOW_H
