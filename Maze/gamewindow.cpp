#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(int x, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    //setWindowFlags(Qt::FramelessWindowHint);
    //showFullScreen();
    setFixedSize(1536, 864);
    setWindowIcon(QIcon(":/res/icon.png"));
//    qDebug() << this -> height();
//    qDebug() << this -> width();


    gameBgm = new QMediaPlayer;
    QAudioOutput *audio = new QAudioOutput;
    gameBgm -> setAudioOutput(audio);



    stop = new QLabel(this);
    stop ->setGeometry(341, 222-800, 854, 420);
    stop -> setStyleSheet("border-image: url(:/res/stop.png); color: #ffffff;");
    stop -> setAlignment(Qt::AlignCenter);
    QFont ft;
    ft.setPointSize(20);
    stop -> setFont(ft);



    stop -> show();
    homeBtn = new QPushButton(this);
    homeBtn ->setGeometry(362, 501-800, 216, 120);
    homeBtn -> setStyleSheet("border-image: url(:/res/home_btn.png);");
    homeBtn -> setFocusPolicy(Qt::NoFocus);
    homeBtn -> show();
    stopBtn = new QPushButton(this);
    stopBtn ->setGeometry(598, 501-800, 340, 120);
    stopBtn -> setStyleSheet("border-image: url(:/res/stop_btn.png);");
    stopBtn -> setFocusPolicy(Qt::NoFocus);
    stopBtn -> show();
    settingBtn = new QPushButton(this);
    settingBtn ->setGeometry(958, 501-800, 216, 120);
    settingBtn -> setStyleSheet("border-image: url(:/res/setting_btn.png);");
    settingBtn -> setFocusPolicy(Qt::NoFocus);
    settingBtn -> show();

    connect(stopBtn, &QPushButton::clicked, this, &GameWindow::start);
    connect(homeBtn, &QPushButton::clicked, this, [=]() {
        gameOver();
    });

    size = x;
    scene = x%2;
    init();

    pet = new Block;
    pet -> label = new QLabel(this);
    petImg = "border-image: url(:/res/pet0_4_0.png);";
    pet -> label -> setStyleSheet(petImg);
    pet -> label -> hide();
    petImgIdx = 0;



    connect(&ai, &QTimer::timeout, this, [=]() {

        petImg = QString("border-image: url(:/res/pet0_%1_%2.png);").arg(petImgIdx).arg(isPetLeft);
        pet -> label -> setStyleSheet(petImg);
        petImgIdx = (petImgIdx + 1) % 4;

        int r = (pet -> r - (WALLSIZE[size] - CHARR[size])) / STEP[size];
        int c = (pet -> c - (ROADSIZE[size] - (!isPetLeft) * CHARC[size])) / STEP[size];

        if (r / 8 + 1 == row - 1 && c / 8 + 1 == end) {
            ai.stop();
            petImg = "border-image: url(:/res/pet0_4_0.png);";
            pet -> label -> setStyleSheet(petImg);
            return;
        }

        if (r % 8 == 0 && c % 8 == 0 && !path.isEmpty()) {
            auto t = path.pop();
            for (int i = 0; i < 4; ++i) {
                int nr = r / 8 + 1 + dr[i];
                int nc = c / 8 + 1 + dc[i];
                if (nr == t.fi && nc == t.se) {
                    petDir = i;
                    break;
                }
            }

            dist++;

        }

        pet -> r = pet -> r + dr[petDir] * STEP[size];
        pet -> c = pet -> c + dc[petDir] * STEP[size];


        pet -> label -> move(pet -> c, pet -> r);
        pet -> label -> raise();

        for (int i = 0; i < 3; ++i) {               // 下边覆盖
            int tr = (r + 6) / 8 + 2, tc = c / 8;
            if (block[tr][tc + i] -> type == WALL) {
                block[tr][tc + i] -> label -> raise();
                while (tr < row && block[++tr][tc + i] -> type == WALL) {
                    block[tr][tc + i] -> label -> raise();
                }
            }

        }

        if (dist >= 8 && abs(character -> r - pet -> r) + abs(character -> c - pet -> c) > 12 * STEP[size]) {
            ai.stop();
            petImg = "border-image: url(:/res/pet0_4_0.png);";
            pet -> label -> setStyleSheet(petImg);
            dist = 0;
            return;
        }
    });





}

void GameWindow::init() {

    srand((unsigned)time(0));

    //size = 0;
    //scene = 1;
    row = this -> height() / ROADSIZE[size] - 1;
    col = this -> width() / ROADSIZE[size] - 1;
    dirction = 2;
    petDir = 2;
    isLeft = 0;
    isPetLeft = 0;
    isStop = false;
    isAI = false;

    gameBgm -> setSource(QUrl(QString("qrc:/res/game_bgm_%1.wav").arg(scene)));
    gameBgm -> setLoops(-1);
    gameBgm -> play();

//    charImg[0] = "border-image: url(:/res/char1_1.png);";
//    charImg[1] = "border-image: url(:/res/char1_2.png);";
//    charImgIdx = 0;
//    connect(&timer, &QTimer::timeout, this, [=]() {
//        character -> label -> setStyleSheet(charImg[charImgIdx]);
//        charImgIdx = !charImgIdx;
//    });
//    timer.start(350);



    generate(row - 1, col - 1);

    for (int i = 0; i <= row; ++i) {
        for(int j = 0; j <= col; ++j) {
            block[i][j] = new Block;
            block[i][j] -> label = new QLabel(this);

            if (i == 0 || i == row || j == 0 || j == col) {
                QString wall = QString("border-image: url(:/res/wall%1_%2.png);").arg(scene).arg(rand() % 2);
                block[i][j] -> label -> setGeometry(j * ROADSIZE[size], i * ROADSIZE[size], ROADSIZE[size], WALLSIZE[size]);
                block[i][j] -> label -> setStyleSheet(wall);
                block[i][j] -> label -> raise();
                block[i][j] -> label -> show();
                block[i][j] -> type = WALL;
            }

            else if (maze[i - 1][j - 1] == ROAD) {

                QString road = QString("border-image: url(:/res/road%1_%2.png);").arg(scene).arg(rand() % 3);
                block[i][j] -> label -> setGeometry(j * ROADSIZE[size], i * ROADSIZE[size] + WALLSIZE[size] - ROADSIZE[size], ROADSIZE[size], ROADSIZE[size]);
                block[i][j] -> label -> setStyleSheet(road);
                block[i][j] -> label -> lower();
                block[i][j] -> label -> show();
                block[i][j] -> type = ROAD;
            }
            else {
                QString wall = QString("border-image: url(:/res/wall%1_2.png);").arg(scene);
                block[i][j] -> label -> setGeometry(j * ROADSIZE[size], i * ROADSIZE[size], ROADSIZE[size], WALLSIZE[size]);
                block[i][j] -> label -> setStyleSheet(wall);
                block[i][j] -> label -> raise();
                block[i][j] -> label -> show();
                block[i][j] -> type = WALL;
            }


            block[i][j] -> r = i;
            block[i][j] -> c = j;
        }

    }

    for (int i = col - 1; i > 0; --i) {
        if (block[row - 1][i] -> type == ROAD) {
            QString ladder_1 = "border-image: url(:/res/ladder_1.png);";
            QString ladder_2 = "border-image: url(:/res/ladder_2.png);";
            block[row][i] -> label -> setGeometry(i * ROADSIZE[size], row * ROADSIZE[size] + WALLSIZE[size] - ROADSIZE[size], ROADSIZE[size], WALLSIZE[size]);
            block[row][i] -> label -> setStyleSheet(ladder_1);
            block[row][i] -> label -> show();
            block[row][i] -> type = ROAD;

            ladder = new QLabel(this);
            ladder -> setGeometry(i * ROADSIZE[size], row * ROADSIZE[size] + WALLSIZE[size] - ROADSIZE[size], ROADSIZE[size], WALLSIZE[size]);
            ladder -> setStyleSheet(ladder_2);
            ladder -> show();
            ladder -> raise();

            end = i;
            break;

        }
    }

    character = new Block;
    character -> label = new QLabel(this);
    character -> label -> setGeometry(ROADSIZE[size] - CHARC[size], WALLSIZE[size] - CHARR[size], CHARSIZE[size], CHARSIZE[size]);
    charImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
    character -> label -> setStyleSheet(charImg);
    character -> label -> show();
    character -> r = WALLSIZE[size] - CHARR[size];
    character -> c = ROADSIZE[size] - CHARC[size];
    charImgIdx = 0;

    cover();

    time_start = (double) clock();
    time_dur = 0;
}


void GameWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    // 抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    painter.setBrush(QBrush(QColor(7, 25, 49)));
    painter.drawRect(this -> rect());

}

void GameWindow::move() {


    int nr = character -> r + dr[dirction] * STEP[size];
    int nc = character -> c + dc[dirction] * STEP[size];

    character -> r = nr;
    character -> c = nc;
    character -> label ->move(character -> c, character -> r);
    character -> label -> raise();


    cover();

}

void GameWindow::cover() {

    int r = (character -> r - (WALLSIZE[size] - CHARR[size])) / STEP[size];
    int c = (character -> c - (ROADSIZE[size] - (!isLeft) * CHARC[size])) / STEP[size];

    if ((r + 7) / 8 + 1 >= row) {
        if (size == 0)  saveHistory();                          // 结束时保存历史
        QTimer *win = new QTimer(this);
        connect(win, &QTimer::timeout, this, [=]() {
            int nr = character -> r + dr[dirction] * STEP[size];
            int nc = character -> c + dc[dirction] * STEP[size];

            if ((nr - (WALLSIZE[size] - CHARR[size])) / STEP[size] / 8 > row) {
                disconnect(win, &QTimer::timeout, this, nullptr);
                gameOver();
            }

            character -> r = nr;
            character -> c = nc;
            character -> label ->move(character -> c, character -> r);

            //ladder -> raise();
            //character -> label -> raise();
        });
        win -> start(200);
        ladder -> raise();
        return;
    }

    int tr = (r + 6) / 8 + 1, tc = c / 8;       // 左边覆盖
    if (block[tr][tc] -> type == WALL) {
        block[tr][tc] -> label -> raise();
    }
    while (tr < row && block[++tr][tc] -> type == WALL) {
        block[tr][tc] -> label -> raise();
    }

    tr = (r + 6) / 8 + 1, tc = (c + 7) / 8 + 2; // 右边覆盖
    if (block[tr][tc] -> type == WALL) {
        block[tr][tc] -> label -> raise();
    }
    while (tr < row && block[++tr][tc] -> type == WALL) {
        block[tr][tc] -> label -> raise();
    }


    for (int i = 0; i < 3; ++i) {               // 下边覆盖
        int tr = (r + 6) / 8 + 2, tc = c / 8;
        if (block[tr][tc + i] -> type == WALL) {
            block[tr][tc + i] -> label -> raise();
            while (tr < row && block[++tr][tc + i] -> type == WALL) {
                block[tr][tc + i] -> label -> raise();
            }
        }

    }
}

void GameWindow::run() {
    if (!timer.isActive()) {
        connect(&timer, &QTimer::timeout, this, [=]() {
            charImg = QString("border-image: url(:/res/char0_%1_%2.png);").arg(charImgIdx).arg(isLeft);
            character -> label -> setStyleSheet(charImg);
            charImgIdx = (charImgIdx + 1) % 4;
        });
        timer.start(200);
    }
}

void GameWindow::turn() {
    if (isLeft) {
        isLeft = 0;
        character -> c -= CHARC[size];
    }
    else {
        isLeft = 1;
        character -> c += CHARC[size];
    }

    charImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
    character -> label -> setStyleSheet(charImg);
    character -> label ->move(character -> c, character -> r);
    character -> label -> raise();

    cover();
}

void GameWindow::gameOver() {
    disconnect(&timer, &QTimer::timeout, this, nullptr);
    gameBgm -> stop();
    emit back();
}


void GameWindow::pressSpace() {
    if (!isStop) {

        time_end = (double) clock();
        stop->setText("模式：" + MODE[size] + "     " + QString("耗时：%1s").arg((time_end - time_start + time_dur) / 1000.0));
        time_dur += time_end - time_start;

        stop -> raise();
        homeBtn -> raise();
        stopBtn -> raise();
        settingBtn -> raise();

        QPropertyAnimation *move1 = new QPropertyAnimation(stop, "geometry");
        QPropertyAnimation *move2 = new QPropertyAnimation(homeBtn, "geometry");
        QPropertyAnimation *move3 = new QPropertyAnimation(stopBtn, "geometry");
        QPropertyAnimation *move4 = new QPropertyAnimation(settingBtn, "geometry");
        move1 -> setDuration(400);
        move2 -> setDuration(400);
        move3 -> setDuration(400);
        move4 -> setDuration(400);
        move1 -> setStartValue(QRect(stop -> x(), stop -> y(), stop -> width(), stop -> height()));
        move2 -> setStartValue(QRect(homeBtn -> x(), homeBtn -> y(), homeBtn -> width(), homeBtn -> height()));
        move3 -> setStartValue(QRect(stopBtn -> x(), stopBtn -> y(), stopBtn -> width(), stopBtn -> height()));
        move4 -> setStartValue(QRect(settingBtn -> x(), settingBtn -> y(), settingBtn -> width(), settingBtn -> height()));
        move1 -> setEndValue(QRect(stop -> x(), 222, stop -> width(), stop -> height()));
        move2 -> setEndValue(QRect(homeBtn -> x(), 501, homeBtn -> width(), homeBtn -> height()));
        move3 -> setEndValue(QRect(stopBtn -> x(), 501, stopBtn -> width(), stopBtn -> height()));
        move4 -> setEndValue(QRect(settingBtn -> x(), 501, settingBtn -> width(), settingBtn -> height()));
        move1 -> setEasingCurve(QEasingCurve::OutQuint);
        move2 -> setEasingCurve(QEasingCurve::OutQuint);
        move3 -> setEasingCurve(QEasingCurve::OutQuint);
        move4 -> setEasingCurve(QEasingCurve::OutQuint);
        move1 -> start();
        move2 -> start();
        move3 -> start();
        move4 -> start();

        isStop = true;
    }
    else {
        start();
    }
}

void GameWindow::start() {
    isStop = false;
    QPropertyAnimation *move1 = new QPropertyAnimation(stop, "geometry");
    QPropertyAnimation *move2 = new QPropertyAnimation(homeBtn, "geometry");
    QPropertyAnimation *move3 = new QPropertyAnimation(stopBtn, "geometry");
    QPropertyAnimation *move4 = new QPropertyAnimation(settingBtn, "geometry");
    move1 -> setDuration(400);
    move2 -> setDuration(400);
    move3 -> setDuration(400);
    move4 -> setDuration(400);
    move1 -> setStartValue(QRect(stop -> x(), stop -> y(), stop -> width(), stop -> height()));
    move2 -> setStartValue(QRect(homeBtn -> x(), homeBtn -> y(), homeBtn -> width(), homeBtn -> height()));
    move3 -> setStartValue(QRect(stopBtn -> x(), stopBtn -> y(), stopBtn -> width(), stopBtn -> height()));
    move4 -> setStartValue(QRect(settingBtn -> x(), settingBtn -> y(), settingBtn -> width(), settingBtn -> height()));
    move1 -> setEndValue(QRect(stop -> x(), 222-800, stop -> width(), stop -> height()));
    move2 -> setEndValue(QRect(homeBtn -> x(), 501-800, homeBtn -> width(), homeBtn -> height()));
    move3 -> setEndValue(QRect(stopBtn -> x(), 501-800, stopBtn -> width(), stopBtn -> height()));
    move4 -> setEndValue(QRect(settingBtn -> x(), 501-800, settingBtn -> width(), settingBtn -> height()));
    move1 -> setEasingCurve(QEasingCurve::OutQuint);
    move2 -> setEasingCurve(QEasingCurve::OutQuint);
    move3 -> setEasingCurve(QEasingCurve::OutQuint);
    move4 -> setEasingCurve(QEasingCurve::OutQuint);
    move1 -> start();
    move2 -> start();
    move3 -> start();
    move4 -> start();


    time_start = (double) clock();

}

void GameWindow::keyPressEvent(QKeyEvent *event) {
    auto KEY = event -> key();
    if (isStop && KEY != Qt::Key_Space) return;
    if (KEY == Qt::Key_Space && event -> isAutoRepeat()) return;

    int r = (character -> r - (WALLSIZE[size] - CHARR[size])) / STEP[size];
    int c = (character -> c - (ROADSIZE[size] - (!isLeft) * CHARC[size])) / STEP[size];

    if ((r + 7) / 8 + 1 >= row) return;

    if (abs(character -> r - pet -> r) + abs(character -> c - pet -> c) <= 12 * STEP[size] && !ai.isActive()) {
        petMove();
    }

    switch (KEY) {
    case Qt::Key_Up:
        run();

        if (block[(r + 7) / 8][(c + 4) / 8 + 1] -> type == ROAD) {
            while (c % 8 > 5) {
                dirction = 2;
                move();
                c++;
            }
            while (c % 8 > 0 && c % 8 < 3) {
                dirction = 0;
                move();
                c--;
            }
        }


        if (c % 8 == 0 && block[(r + 7) / 8][c / 8 + 1] -> type == ROAD) {
            dirction = 1;
            move();
        }

        break;
    case Qt::Key_Down:

        run();

        if (block[r / 8 + 2][(c + 4) / 8 + 1] -> type == ROAD) {
            while (c % 8 > 5) {
                dirction = 2;
                move();
                c++;
            }
            while (c % 8 > 0 && c % 8 < 3) {
                dirction = 0;
                move();
                c--;
            }
        }

        if (c % 8 == 0 && block[r / 8 + 2][c / 8 + 1] -> type == ROAD) {
            dirction = 3;
            move();
        }
        break;
    case Qt::Key_Right:
        if (isLeft) turn();

        run();

        if (block[(r + 4) / 8 + 1][c / 8 + 2] -> type == ROAD) {
            while (r % 8 > 5) {
                dirction = 3;
                move();
                r++;
            }
            while (r % 8 > 0 && r % 8 < 3) {
                dirction = 1;
                move();
                r--;
            }
        }

        if (r % 8 == 0 && block[r / 8 + 1][c / 8 + 2] -> type == ROAD) {
            dirction = 2;
            move();
        }
        break;
    case Qt::Key_Left:
        if (!isLeft) turn();

        run();

        if (block[(r + 4) / 8 + 1][(c + 7) / 8] -> type == ROAD) {
            while (r % 8 > 5) {
                dirction = 3;
                move();
                r++;
            }
            while (r % 8 > 0 && r % 8 < 3) {
                dirction = 1;
                move();
                r--;
            }
        }

        if (r % 8 == 0 && block[r / 8 + 1][(c + 7) / 8] -> type == ROAD) {
            dirction = 0;
            move();
        }
        break;
    case Qt::Key_Space:
        pressSpace();
        break;
    case Qt::Key_Q:
        if (!isAI) {
            aStar();

            if (!path.isEmpty()) {
                auto t = path.pop();
                pet -> r = (t.fi - 1) * ROADSIZE[size] + (WALLSIZE[size] - CHARR[size]);
                pet -> c = (t.se - 1) * ROADSIZE[size] + (ROADSIZE[size] - (!isPetLeft) * CHARC[size]);
                pet -> label -> setGeometry(pet -> c, pet -> r, CHARSIZE[size], CHARSIZE[size]);
                pet -> label -> show();
                pet -> label -> raise();

                dist = 0;

                for (int i = 0; i < 3; ++i) {               // 下边覆盖
                    int tr = (r + 6) / 8 + 2, tc = c / 8;
                    if (block[tr][tc + i] -> type == WALL) {
                        block[tr][tc + i] -> label -> raise();
                        while (tr < row && block[++tr][tc + i] -> type == WALL) {
                            block[tr][tc + i] -> label -> raise();
                        }
                    }

                }

                petMove();
            }

            isAI = true;
        }
        else {
            isAI = false;
            pet -> label -> hide();
            ai.stop();
            //disconnect(&ai, &QTimer::timeout, this, nullptr);
        }
        break;
    default:
        break;
    }
}

void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat())
        event->ignore();
    else if (timer.isActive() && ((character -> r - (WALLSIZE[size] - CHARR[size])) / STEP[size] + 7) / 8 + 1 < row) {
        timer.stop();
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        charImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
        character -> label -> setStyleSheet(charImg);
    }
}

void GameWindow::petMove() {


    ai.start(80);

}


int GameWindow::f(int r, int c) {
    return abs(row - r) + abs(end - c);
}

void GameWindow::aStar() {

    double run_time;
    _LARGE_INTEGER time_start;	//开始时间
    _LARGE_INTEGER time_over;	//结束时间
    double dqFreq;		//计时器频率
    LARGE_INTEGER ff;	//计时器频率
    QueryPerformanceFrequency(&ff);
    dqFreq=(double)ff.QuadPart;
    QueryPerformanceCounter(&time_start);	//计时开始

    //clock_t time_st = clock();

    int str = ((character -> r - (WALLSIZE[size] - CHARR[size])) / STEP[size]) / 8 + 1;
    int stc = ((character -> c - (ROADSIZE[size] - (!isLeft) * CHARC[size])) / STEP[size]) / 8 + 1;

    path.clear();

    struct Node {
        int prevr = 0;  //保存前一个的坐标
        int prevc = 0;
        int dist = 0x3f3f3f3f;
    };

    Node Nodes[MAX_R + 2][MAX_C + 2];



    Heap heap;
    heap.push({f(str, stc), {str, stc}});
    Nodes[str][stc].prevr = str;
    Nodes[str][stc].prevc = stc;
    Nodes[str][stc].dist = 0;

    while (heap.size()) {
        int r = heap.top().se.fi;
        int c = heap.top().se.se;
        heap.pop();

        if (r == row && c == end) break;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr > 0 && nr <= row && nc > 0 && nc < col && block[nr][nc] -> type == ROAD && Nodes[nr][nc].dist > Nodes[r][c].dist + 1) {

                Nodes[nr][nc].prevr = r;
                Nodes[nr][nc].prevc = c;
                Nodes[nr][nc].dist = Nodes[r][c].dist + 1;


                heap.push({Nodes[nr][nc].dist + f(nr, nc), {nr, nc}});
            }
        }
    }

    int r = Nodes[row][end].prevr;
    int c = Nodes[row][end].prevc;

    while (r != str || c != stc) {
        path.push({r, c});
        int tr = r;
        r = Nodes[r][c].prevr;
        c = Nodes[tr][c].prevc;
    }


//    QPair<int, int> q[MAX_R * MAX_C];
//    int hh = 0, tt = 0;

//    q[tt] = {str, stc};
//    Nodes[str][stc].prevr = str;
//    Nodes[str][stc].prevc = stc;

//    while (hh <= tt) {
//        auto t = q[hh++];
//        bool findPath = false;
//        for (int i = 0; i < 4; ++i) {
//            int nr = t.fi + dr[i];
//            int nc = t.se + dc[i];
//            if (nr > 0 && nr <= row && nc > 0 && nc < col && block[nr][nc] -> type == ROAD && Nodes[nr][nc].prevc == 0) {

//                Nodes[nr][nc].prevr = t.fi;
//                Nodes[nr][nc].prevc = t.se;

//                if (nr == row && nc == end) {
//                    findPath = true;
//                    break;
//                }

//                q[++tt] = {nr, nc};
//            }
//        }

//        if (findPath) {
//            int r = Nodes[row][end].prevr;
//            int c = Nodes[row][end].prevc;

//            while (r != str || c != stc) {
//                path.push({r, c});
//                int tr = r;
//                r = Nodes[r][c].prevr;
//                c = Nodes[tr][c].prevc;

//            }

//            //clock_t time_ed = clock();
//            //qDebug() << "BFS算法用时：" << QString::number((double) (time_ed - time_st) / CLK_TCK, 'f', 6) << "s";

//            QueryPerformanceCounter(&time_over);	//计时结束
//            run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq;
//            //乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
//            qDebug() << "BFS算法用时：" << run_time;
//            return;
//        }
//    }

    //clock_t time_ed = clock();
    //qDebug() << "A*算法用时：" << QString::number((double) (time_ed - time_st) / CLK_TCK, 'f', 6) << "s";

    QueryPerformanceCounter(&time_over);	//计时结束
    run_time=1000000*(time_over.QuadPart-time_start.QuadPart)/dqFreq;
    //乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
    qDebug() << "A*算法用时：" << run_time;
    //printf("\nrun_time：%fus\n",run_time);

}


GameWindow::~GameWindow() {
    delete ui;
}


void GameWindow::saveHistory() {
    time_end = (double) clock();
    time_dur += time_end - time_start;

    QDateTime time;
    QString time_str = time.currentDateTime().toString("yyyy-MM-dd");

    QDir *DataFile = new QDir;
    bool exist = DataFile->exists("DataFile");

    if(!exist) DataFile->mkdir("DataFile"); // 新建文件夹

    QString filePath = "DataFile/datafile.txt";
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream stream(&file);

    QString line = stream.readLine();
    if (line == "" || line.toDouble() > time_dur / 1000.0) {
        file.close();
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        stream << QString("%1\n").arg(time_dur / 1000.0) + time_str;
    }

    file.close();
}
