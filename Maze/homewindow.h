#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include "selectwindow.h"

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void init();
    void move();
    void run();
    void turn();

private slots:
    void on_Btn_Exit_clicked();

private:
    Ui::HomeWindow *ui;

    SelectWindow * select = NULL;
    QMediaPlayer *homeBgm;
    QTimer timer;

    QPushButton *role = NULL;
    QString roleImg;
    int roleImgIdx;
    int isLeft;
    int dirction;
    QLabel *tips;

    bool tipsShow;
    bool certShow;
    bool stopShow;
};

#endif // HOMEWINDOW_H
