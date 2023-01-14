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
using namespace std;

#define fi first
#define se second

enum Type
{
    NORMAL,   //普通类型，背景
    BORDER,   //边界
    SNAKE,    //蛇节点
    FOOD,     //食物
};

const int MAX_X = 23;
const int MAX_Y = 11;
const int BLOCK_SIZE = 50;
const int LEVEL_MARGIN = 40;
const int TOP_MARGIN = 110;
const int BOTTOM_MARGIN = 10;

namespace Ui {
class GameWindow;
}

class Grid {
public:
    QLabel *label;
    int type;
    int x;
    int y;
};

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(int idx, QWidget *parent = nullptr);
    ~GameWindow();

    void paintEvent(QPaintEvent *);     // 背景图
    void init();
    void move();
    void keyPressEvent(QKeyEvent *event);
    void gameOver();
    void generateFood();
    void saveHistory();


    void aiSnake();
    void followTail();
    bool bfs(Grid *, Grid *);

signals:
    void back();




private slots:

    void on_BtnBack_clicked();

    void on_BtnStop_clicked();

    void on_BtnAI_clicked();

private:
    Ui::GameWindow *ui;

    QString list[7] = {" 小黑鸡  ", " 奥特鸡  ", "鸡卡巴卡 ", "电玩小鸡 ", " 懒鸡鸡  ", "鸡国队长 ", "  柯鸡   "};
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, -1, 0, 1};
    int direction;

    Grid *grid[MAX_X + 1][MAX_Y + 1];
    QList<Grid *> snake;
    Grid *hh;
    Grid *tt;

    QTimer timer;
    QMediaPlayer *bgm;
    int index;
    QString headImage;
    QString bodyImage;
    QString foodImage;
    bool isOver;
    bool isChange;
    int score;
    int speed;
    bool isAI;


    int foodx;
    int foody;
    QStack<pair<int, int>> shortestPath;

    QLabel * gameOverImage;

};

#endif // GAMEWINDOW_H
