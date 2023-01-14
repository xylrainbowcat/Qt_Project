#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

#include "gamewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    void paintEvent(QPaintEvent *);     // 背景图
    void keyPressEvent(QKeyEvent *event);

    void noEnergy();

private slots:
    void on_BtnRight_clicked();

    void on_BtnLeft_clicked();

    void on_BtnStart_clicked();

    void on_BtnBuy_clicked();

private:
    GameWindow * game = NULL;

    int chickenNum;
    int idx;
    int energy;
    QLabel *noEnergyLabel;
    bool isNoEnergy;
    QLabel * pay;
    bool isPay;

    Ui::StartWindow *ui;
};
#endif // STARTWINDOW_H
