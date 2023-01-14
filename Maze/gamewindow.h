#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QSoundEffect>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDir>
#include <QStack>
#include <QVector>
#include <QPushButton>
#include "heap.h"
#include "windows.h"

#define fi first
#define se second

enum Type
{
    WALL,
    ROAD
};

const int MAX_R = 27;
const int MAX_C = 48;
const int dr[4] = {0, -1, 0, 1};
const int dc[4] = {-1, 0, 1, 0};
const int ROADSIZE[3] = {32, 64, 96};
const int WALLSIZE[3] = {48, 96, 144};
const int CHARSIZE[3] = {44, 88, 132};
const int CHARR[3] = {20, 40, 60};
const int CHARC[3] = {12, 24, 36};
const int STEP[3] = {4, 8, 12};
const QString MODE[3] = {"困难", "中等", "简单"};

namespace Ui {
class GameWindow;
}

class Block {
public:
    QLabel *label;
    int type;
    int r;
    int c;
};

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(int x, QWidget *parent = nullptr);
    ~GameWindow();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void init();
    void generate(int row, int col);
    void move();
    void cover();
    void run();
    void turn();
    void pressSpace();
    void start();
    void aStar();
    void gameOver();
    void petMove();
    void saveHistory();
    int f(int r, int c);

signals:
    void back();

private:
    Ui::GameWindow *ui;

    double time_start;
    double time_end;
    double time_dur;
    QTimer timer;
    QTimer ai;
    QMediaPlayer *gameBgm;
    QLabel *stop;
    QPushButton *homeBtn;
    QPushButton *stopBtn;
    QPushButton *settingBtn;
    QStack<QPair<int, int>> path;
    QLabel *ladder;

    QString charImg;
    int charImgIdx;
    QString petImg;
    int petImgIdx;

    int row;
    int col;
    int size;
    int dirction;
    int petDir;
    int isLeft;
    int isPetLeft;
    int scene;
    bool isStop;
    Block *block[MAX_R + 2][MAX_C + 2];
    Block *character;
    Block *pet;
    int maze[MAX_R][MAX_C];
    int end;
    bool isAI;
    int dist;
};

#endif // GAMEWINDOW_H
