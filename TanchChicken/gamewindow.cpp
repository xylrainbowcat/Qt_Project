#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(int idx, QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);

    setFixedSize(1280, 720);
    setWindowTitle("Tanch Chicken");
    setWindowIcon(QIcon(":/res/1.png"));

    //setWindowFlags(Qt::FramelessWindowHint);
    //showFullScreen();

    index = idx;
    headImage = QString("border-image: url(:/res/%1.png);").arg(index);
    bodyImage = "border-image: url(:/res/dan.png);";
    foodImage = "border-image: url(:/res/food.png)";



    score = 0, speed = 300, isAI = false;
    ui -> Score -> setText(QString::number(score));
    ui -> Speed -> setText(QString::number(speed));


    init();


    bgm = new QMediaPlayer;
    QAudioOutput *audio = new QAudioOutput;
    bgm -> setAudioOutput(audio);
    bgm -> setSource(QUrl("qrc:/res/bgm.wav"));
    bgm -> setLoops(-1);
    bgm -> play();


    gameOverImage = new QLabel(this);
    gameOverImage -> setGeometry(440, -400, 400, 400);
    gameOverImage -> setStyleSheet("border-image: url(:/res/gameover.png)");

    connect(&timer, &QTimer::timeout, this, [=]() {
        move();
    });

}

GameWindow::~GameWindow()
{
    delete ui;
}

// ============================================================================= 绘制背景图
void GameWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPixmap pix;

    // 抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    pix.load(":/res/background.jpg");
    painter.drawPixmap(0, 0, this -> width(), this -> height(), pix);

    pix.load(":/res/logo.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(40, 20, pix);
}

// ============================================================================= 初始化
void GameWindow::init() {
    for (int i = 0; i <= MAX_X; ++i) {
        for(int j = 0; j <= MAX_Y; ++j) {
            grid[i][j] = new Grid;

            if (i == 0 || i == MAX_X || j == 0 || j == MAX_Y) {
                grid[i][j] -> type = BORDER;
            }
            else grid[i][j] -> type = NORMAL;

            grid[i][j] -> label = new QLabel(this);
            grid[i][j] -> label -> setGeometry(LEVEL_MARGIN + i * BLOCK_SIZE, TOP_MARGIN + j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            grid[i][j] -> label -> lower();
            grid[i][j] -> x = i;
            grid[i][j] -> y = j;

        }

    }

    int len = 3;
    direction = 2;
    int sx = 3, sy = 3;

    for (int i = 0; i < len; ++i) {
        snake.append(grid[sx + i][sy]);
        grid[sx + i][sy] -> type = SNAKE;
        grid[sx + i][sy] -> label -> setStyleSheet(bodyImage);
    }

    hh = snake.at(snake.length() - 1);
    tt = snake.at(0);
    hh -> label -> setStyleSheet(headImage);

    generateFood();

    timer.start(speed);




    isOver = false;
    isChange = false;
}

void GameWindow::move() {
    Grid *front = grid[hh -> x + dx[direction]][hh -> y + dy[direction]];

    // 注意：这里可以跟着尾巴走
    if (front -> type == BORDER || (front -> type == SNAKE && front != tt) ) {
        gameOver();
        return;
    }

    if (front -> type == FOOD) {
        generateFood();

        speed = fmax(50, speed - ((++score + 1) % 2) * 10);
        ui -> Score -> setText(QString::number(score));
        ui -> Speed -> setText(QString::number(speed));
        timer.start(speed);
    }
    else {
        tt -> type = NORMAL;
        tt -> label -> setStyleSheet("");
        snake.removeFirst();
        tt = snake.at(0);
    }

    hh -> label -> setStyleSheet(bodyImage);
    hh = front;
    hh -> type = SNAKE;
    hh -> label -> setStyleSheet(headImage);
    snake.append(hh);

    isChange = false;

}

void GameWindow::generateFood() {
    srand((unsigned)time(0));

    do {
        foodx = rand() % (MAX_X - 1) + 1;
        foody = rand() % (MAX_Y - 1) + 1;
    } while (grid[foodx][foody] -> type == SNAKE || grid[foodx][foody] -> type == FOOD);

    grid[foodx][foody] -> type = FOOD;
    grid[foodx][foody] -> label -> setStyleSheet(foodImage);
}

void GameWindow::gameOver() {


    timer.stop();

    isOver = true;


    QPropertyAnimation *move = new QPropertyAnimation(gameOverImage, "geometry");
    move -> setDuration(1000);
    move -> setStartValue(QRect(gameOverImage -> x(), gameOverImage -> y(), gameOverImage -> width(), gameOverImage -> height()));
    move -> setEndValue(QRect(gameOverImage -> x(), gameOverImage -> y() + 630, gameOverImage -> width(), gameOverImage -> height()));
    move -> setEasingCurve(QEasingCurve::OutBounce);
    move -> start();



    bgm -> stop();
    QSoundEffect * gameOverBgm = new QSoundEffect(this);
    gameOverBgm -> setSource(QUrl::fromLocalFile(":/res/ngm.wav"));
    gameOverBgm -> play();


    saveHistory();



//    QMessageBox MBox;
//    MBox.setWindowTitle("游戏结束");
//    MBox.setText("用时：9:00   得分：78");
//    MBox.setIconPixmap(QPixmap(":/res/gameover.png").scaled(50, 50));
//    QPushButton *restartBut = MBox.addButton("重新开始", QMessageBox::ActionRole);
//    QPushButton *exitBut = MBox.addButton("退出", QMessageBox::ActionRole);

//    MBox.show();
//    MBox.exec();

//    if (MBox.clickedButton() == (QAbstractButton*)exitBut) {
//            //在 Qt Creator 的输出窗口中输出指定字符串
//            qDebug() << "用户点击了同意按钮";
//        }

//    else if (MBox.clickedButton() == (QAbstractButton*)restartBut) {
//            //在 Qt Creator 的输出窗口中输出指定字符串
//            qDebug() << "用户点击了同意按钮";
//        }
//    //QMessageBox::information(this,"Message:","游戏结束!",QMessageBox::Ok);

}


void GameWindow::keyPressEvent(QKeyEvent *event) {
    auto KEY = event -> key();
    if (isOver && KEY != Qt::Key_Q) return;

    switch (KEY) {
    case Qt::Key_Up:
        if (direction != 1 && direction != 3 && timer.isActive() && !isAI) {
            if (isChange == true) move();
            else isChange = true;
            direction = 1;
        }
        break;
    case Qt::Key_Down:
        if (direction != 1 && direction != 3 && timer.isActive() && !isAI) {
            if (isChange == true) move();
            else isChange = true;
            direction = 3;
        }
        break;
    case Qt::Key_Right:
        if (direction != 0 && direction != 2 && timer.isActive() && !isAI) {
            if (isChange == true) move();
            else isChange = true;
            direction = 2;
        }
        break;
    case Qt::Key_Left:
        if (direction != 0 && direction != 2 && timer.isActive() && !isAI) {
            if (isChange == true) move();
            else isChange = true;
            direction = 0;
        }
        break;
    case Qt::Key_Space:
        on_BtnStop_clicked();
        break;
    case Qt::Key_A:
        on_BtnAI_clicked();
        break;
    case Qt::Key_Q:
        on_BtnBack_clicked();
        break;
    default:
        break;
    }
}




void GameWindow::on_BtnBack_clicked() {
    disconnect(&timer, &QTimer::timeout, this, nullptr);
    bgm -> stop();
    emit back();
}


void GameWindow::on_BtnStop_clicked() {
    if (isOver) return;
    if (timer.isActive()) {
        timer.stop();
        bgm -> pause();
        ui -> BtnStop -> setStyleSheet("border-radius: 10px; border-width: 1; border-style: outset; border-image: url(:/res/start.png);");
    }
    else {
        timer.start(speed);
        bgm -> play();
        ui -> BtnStop -> setStyleSheet("border-radius: 10px; border-width: 1; border-style: outset; border-image: url(:/res/stop.png);");
    }
}


void GameWindow::saveHistory(){
    QDateTime time;
    QString time_str = time.currentDateTime().toString("yyyy-MM-dd HH:mm");

    QDir *DataFile = new QDir;
    bool exist = DataFile->exists("DataFile");

    if(!exist) DataFile->mkdir("DataFile"); // 新建文件夹

    QString filePath = "DataFile/datafile.txt";
    QFile file(filePath);
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream stream(&file);
    stream << "  " + list[index - 1] + "得分：" + QString::number(score) << "\t" << "时间：" + time_str << "\n";

    file.close();
}


void GameWindow::aiSnake() {
    if (score == 216) {
        gameOver();   //==========================================================================
        return;
    }


    if (bfs(hh, grid[foodx][foody])) {

        QList<Grid *> virtualSnake = snake;
        QStack<pair<int, int>> savePath = shortestPath;

        while (!shortestPath.empty()) {
            auto t = shortestPath.pop();

            virtualSnake.at(0) -> type = NORMAL;
            virtualSnake.removeFirst();
            virtualSnake.append(grid[t.fi][t.se]);
            virtualSnake.at(snake.length() - 1) -> type = SNAKE;

        }

        // 食物是虚拟蛇头
        // 如果能找到尾巴
        if (bfs(grid[foodx][foody], virtualSnake.at(0))) {
            for (auto i : virtualSnake) {
                i -> type = NORMAL;
            }
            for (auto i : snake) {
                i -> type = SNAKE;
            }

            if(savePath.empty()){
                for (int i = 0; i < 4; ++i) {
                    int nx = hh -> x + dx[i];
                    int ny = hh -> y + dy[i];
                    if (grid[nx][ny] -> type == FOOD) {
                        direction = i;
                        break;
                    }
                }
            }
            else {
                auto t = savePath.pop();
                for (int i = 0; i < 4; ++i) {
                    int nx = hh -> x + dx[i];
                    int ny = hh -> y + dy[i];
                    if (grid[nx][ny] == grid[t.fi][t.se]) {
                        direction = i;
                        break;
                    }
                }
            }
        }
        else {
            //========================================================================
            for (auto i : virtualSnake) {
                i -> type = NORMAL;
            }
            for (auto i : snake) {
                i -> type = SNAKE;
            }
            followTail();
        }

    }
    else followTail();

    move();
}

//void GameWindow::followFood() {

//}

void GameWindow::followTail() {

    tt -> type = NORMAL;
    tt = snake.at(1);

    srand((unsigned)time(0));       // 随机数，防止 fdist 相同时出现死循环
    int Rand = rand() % 4;

    int dist = -1, fdist = 0;
    for (int i = 0; i < 4; ++i) {
        int j = (Rand + i) % 4;
        int nx = hh -> x + dx[j];
        int ny = hh -> y + dy[j];
        if (nx > 0 && nx < MAX_X && ny > 0 && ny < MAX_Y && grid[nx][ny] -> type == NORMAL) {
            if (bfs(grid[nx][ny], tt) && shortestPath.size() >= dist) {
                if (shortestPath.size() == dist && abs(nx - foodx) + abs(ny - foody) <= fdist) continue;

                direction = j;
                dist = shortestPath.size();
                fdist = abs(nx - foodx) + abs(ny - foody);
            }
        }
    }

    tt = snake.at(0);
    tt -> type = SNAKE;

    // 找不到尾巴
    if (dist == -1) {
        qDebug() << 1;//=============================================================================================
        for (int i = 0; i < 4; ++i) {
            int nx = hh -> x + dx[i];
            int ny = hh -> y + dy[i];
            if (nx > 0 && nx < MAX_X && ny > 0 && ny < MAX_Y && grid[nx][ny] -> type == NORMAL) {
                direction = i;
                break;
            }
        }
    }
}


bool GameWindow::bfs(Grid *st, Grid *ed) {

    shortestPath.clear();

    //建立一个保存前一个坐标的地图数组
    struct Node {
        int prevx = 0;//保存前一个的坐标
        int prevy = 0;
        int dist = 0;//权值,表示路径长度
        bool visit = false;
    };

    Node Nodes[MAX_X + 1][MAX_Y + 1];

    pair<int, int> q[300];
    int qh = 0, qt = 0;

    q[qt] = {st -> x, st -> y};
    Nodes[st -> x][st -> y].visit = true;
    Nodes[st -> x][st -> y].prevx = st -> x;
    Nodes[st -> x][st -> y].prevy = st -> y;


    srand((unsigned)time(0));       // 随机数，防止死循环=================================================
    int Rand = rand() % 4;

    while (qh <= qt) {
        auto cur = q[qh++];
        bool findTail = false;
        for (int i = 0; i < 4; ++i) {
            int j = (Rand + i) % 4;
            int nx = cur.fi + dx[j];
            int ny = cur.se + dy[j];
            if (nx > 0 && nx < MAX_X && ny > 0 && ny < MAX_Y && (grid[nx][ny] -> type == NORMAL || grid[nx][ny] == ed) && !Nodes[nx][ny].visit) {

                Nodes[nx][ny].visit = true;

                Nodes[nx][ny].dist = Nodes[cur.fi][cur.se].dist + 1;
                Nodes[nx][ny].prevx = cur.fi;
                Nodes[nx][ny].prevy = cur.se;

                if (nx == ed -> x && ny == ed -> y) {
                    findTail = true;
                    break;
                }

                q[++qt] = {nx, ny};
            }
        }


        //若到达目的地，则回溯将路径储存起来，路径不包括蛇头和蛇尾
        if (findTail) {

            int x = Nodes[ed -> x][ed -> y].prevx;
            int y = Nodes[ed -> x][ed -> y].prevy;
            //qDebug() << x << ' ' << y << ' ' << st -> x << ' ' << st -> y;
            while (x != st -> x || y != st -> y) {
                shortestPath.push({x, y});
                int tx = x;
                x = Nodes[x][y].prevx;
                y = Nodes[tx][y].prevy;

            }

            return true;
        }
    }

    return false;
}


void GameWindow::on_BtnAI_clicked() {   //===================================================== 断开连接
    if (isOver || !timer.isActive()) return;

    if (!isAI) {
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        connect(&timer, &QTimer::timeout, this, [=]() {
            aiSnake();
        });
        ui -> BtnAI -> setStyleSheet("border-radius: 10px; border-width: 1; border-style: outset; border-image: url(:/res/AI2.png);");
        isAI = true;
    }
    else {
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        connect(&timer, &QTimer::timeout, this, [=]() {
            move();
        });
        ui -> BtnAI -> setStyleSheet("border-radius: 10px; border-width: 1; border-style: outset; border-image: url(:/res/AI.png);");
        isAI = false;
    }
}

