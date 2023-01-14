#include "homewindow.h"
#include "ui_homewindow.h"

HomeWindow::HomeWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::HomeWindow) {
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    setWindowIcon(QIcon(":/res/icon.png"));

    homeBgm = new QMediaPlayer;
    QAudioOutput *audio = new QAudioOutput;
    homeBgm -> setAudioOutput(audio);
    homeBgm -> setSource(QUrl("qrc:/res/home_bgm.wav"));
    homeBgm -> setLoops(-1);


    role = new QPushButton(this);
    role -> setFocusPolicy(Qt::NoFocus);
    role -> show();
    init();

    tips = new QLabel(this);
    tips->setText("按“回车键”查看通关证书");
    tips->setStyleSheet("color:#ffffff;");
    tips->setGeometry(960, -50, 200, 30);
    QFont ft;
    ft.setPointSize(12);
    tips -> setFont(ft);
    tips->show();
    tipsShow = false;

    ui->certificate->setGeometry(396, 136 - 1000, 744, 592);
    certShow = false;

    ui->Stop->setGeometry(341, 222 - 1000, 854, 420);
    stopShow = false;
}

void HomeWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPixmap pix;

    // 平滑转换算法抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    QString homeImg = QString(":/res/home.png");
    pix.load(homeImg);
    painter.drawPixmap(0, 0, this -> width(), this -> height(), pix);
}

void HomeWindow::init() {
    isLeft = 0;
    roleImgIdx = 0;

    homeBgm -> play();

    role -> setGeometry(256, 256, 88, 88);
    roleImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
    role -> setStyleSheet(roleImg);


    QDir *DataFile = new QDir;
    bool exist = DataFile->exists("DataFile");

    if(!exist) DataFile->mkdir("DataFile"); // 新建文件夹

    QString filePath = "DataFile/datafile.txt";
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString line = stream.readLine();
    if (line == "") {
        ui->date->setText("xxxx-xx-xx");
        ui->time->setText("暂无通关记录");
    }
    else {
        ui->time->setText(line + "s");
        line = stream.readLine();
        ui->date->setText(line);
    }
}

void HomeWindow::move() {
    int nr = role -> y() + dr[dirction] * STEP[1];
    int nc = role -> x() + dc[dirction] * STEP[1];


    role ->move(nc, nr);
    role -> raise();


    if (role->x() >= 960 && role->x() <= 1048 && role->y() == 176) {
        if (!tipsShow) {
            QPropertyAnimation *move = new QPropertyAnimation(tips, "geometry");
            move -> setDuration(400);
            move -> setStartValue(QRect(tips -> x(), tips -> y(), tips -> width(), tips -> height()));
            move -> setEndValue(QRect(tips -> x(), 20, tips -> width(), tips -> height()));
            move -> setEasingCurve(QEasingCurve::OutQuint);
            move -> start();

            tipsShow = true;
        }
    }
    else if (tipsShow) {
        QPropertyAnimation *move = new QPropertyAnimation(tips, "geometry");
        move -> setDuration(400);
        move -> setStartValue(QRect(tips -> x(), tips -> y(), tips -> width(), tips -> height()));
        move -> setEndValue(QRect(tips -> x(), -50, tips -> width(), tips -> height()));
        move -> setEasingCurve(QEasingCurve::OutQuint);
        move -> start();

        tipsShow = false;
    }
}

void HomeWindow::run() {
    if (!timer.isActive()) {
        connect(&timer, &QTimer::timeout, this, [=]() {
            roleImg = QString("border-image: url(:/res/char0_%1_%2.png);").arg(roleImgIdx).arg(isLeft);
            role -> setStyleSheet(roleImg);
            roleImgIdx = (roleImgIdx + 1) % 4;
        });
        timer.start(200);
    }
}

void HomeWindow::turn() {
    if (isLeft) {
        isLeft = 0;
        role -> move(role -> x() - CHARC[1], role -> y());
    }
    else {
        isLeft = 1;
        role -> move(role -> x() + CHARC[1], role -> y());
    }

    roleImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
    role -> setStyleSheet(roleImg);
    role -> raise();
}


void HomeWindow::keyPressEvent(QKeyEvent *event) {
    if (stopShow && event->key() != Qt::Key_Space) return;
    if (certShow) {
        QPropertyAnimation *m = new QPropertyAnimation(ui->certificate, "geometry");
        m -> setDuration(400);
        m -> setStartValue(QRect(ui->certificate -> x(), ui->certificate -> y(), ui->certificate -> width(), ui->certificate -> height()));
        m -> setEndValue(QRect(ui->certificate -> x(), 136 - 1000, ui->certificate -> width(), ui->certificate -> height()));
        m -> setEasingCurve(QEasingCurve::OutQuint);
        m -> start();

        role->raise();
        if (event->key() != Qt::Key_Return && event->key() != Qt::Key_Enter) certShow = false;
    }


    switch (event -> key()) {
    case Qt::Key_Up:
        if (role -> y() == 176 && role -> x() >= 624 && role -> x() <= 776) {
            select = new SelectWindow();
            QTimer::singleShot(400, this, [=]() {   // 延迟 400 ms
                timer.stop();
                disconnect(&timer, &QTimer::timeout, this, nullptr);
                homeBgm -> stop();
                this -> hide();
                select -> show();
            });

            connect(select, &SelectWindow::back, this, [=]() {
                select -> close();
                this -> show();
                init();
            });
        }
        run();
        if (role -> y() > 176) {
            dirction = 1;
            move();
        }
        break;
    case Qt::Key_Down:
        run();
        if (role -> y() < 760) {
            dirction = 3;
            move();
        }
        break;
    case Qt::Key_Right:
        if (isLeft) turn();
        run();
        if (role -> x() < 1440) {
            dirction = 2;
            move();
        }
        break;
    case Qt::Key_Left:
        if (!isLeft) turn();
        run();
        if (role -> x() > 8) {
            dirction = 0;
            move();
        }
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (tipsShow && !certShow) {
            ui->certificate->raise();
            role->lower();
            QPropertyAnimation *move = new QPropertyAnimation(ui->certificate, "geometry");
            move -> setDuration(400);
            move -> setStartValue(QRect(ui->certificate -> x(), ui->certificate -> y(), ui->certificate -> width(), ui->certificate -> height()));
            move -> setEndValue(QRect(ui->certificate -> x(), 136, ui->certificate -> width(), ui->certificate -> height()));
            move -> setEasingCurve(QEasingCurve::OutQuint);
            move -> start();

            certShow = true;
        }
        else certShow = false;
        break;
    case Qt::Key_Space:
        if (!stopShow) {
            ui->Stop->raise();
            role->lower();
            QPropertyAnimation *move = new QPropertyAnimation(ui->Stop, "geometry");
            move -> setDuration(400);
            move -> setStartValue(QRect(ui->Stop -> x(), ui->Stop -> y(), ui->Stop -> width(), ui->Stop -> height()));
            move -> setEndValue(QRect(ui->Stop -> x(), 222, ui->Stop -> width(), ui->Stop -> height()));
            move -> setEasingCurve(QEasingCurve::OutQuint);
            move -> start();

            stopShow = true;
        }
        else {
            QPropertyAnimation *m = new QPropertyAnimation(ui->Stop, "geometry");
            m -> setDuration(400);
            m -> setStartValue(QRect(ui->Stop -> x(), ui->Stop -> y(), ui->Stop -> width(), ui->Stop -> height()));
            m -> setEndValue(QRect(ui->Stop -> x(), 222 - 1000, ui->Stop -> width(), ui->Stop -> height()));
            m -> setEasingCurve(QEasingCurve::OutQuint);
            m -> start();

            role->raise();
            stopShow = false;
        }
        break;
    case Qt::Key_Q:
        qDebug() << role -> x();
        break;
    default:
        break;
    }
}

void HomeWindow::keyReleaseEvent(QKeyEvent *event) {
    if(event->isAutoRepeat())
        event->ignore();
    else if (timer.isActive()) {
        timer.stop();
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        roleImg = QString("border-image: url(:/res/char0_4_%1.png);").arg(isLeft);
        role -> setStyleSheet(roleImg);
    }
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_Btn_Exit_clicked() {
    this->close();
}

