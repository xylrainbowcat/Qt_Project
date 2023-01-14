#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include "homewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void start();

private slots:
    void on_startBtn_clicked();

private:
    Ui::StartWindow *ui;

    HomeWindow * home = NULL;

    QMediaPlayer *startBgm;
    QString startImg;
    QLabel * logo;
    bool isStart;

    QTimer timer;
};
#endif // STARTWINDOW_H
