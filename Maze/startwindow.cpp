#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StartWindow) {
    ui->setupUi(this);


    //setWindowFlags(Qt::FramelessWindowHint);
    //showFullScreen();
    setFixedSize(1536, 864);
    setWindowIcon(QIcon(":/res/icon.png"));

    ui -> startBtn -> move(503, 904);

    startBgm = new QMediaPlayer;
    QAudioOutput *audio = new QAudioOutput;
    startBgm -> setAudioOutput(audio);
    startBgm -> setSource(QUrl("qrc:/res/start_bgm.wav"));
    startBgm -> setLoops(-1);
    startBgm -> play();


    logo = new QLabel(this);
    logo -> setGeometry(40, -200, 564, 168);
    logo -> setStyleSheet("border-image: url(:/res/logo.png)");
    logo -> show();


    isStart = false;

    connect(&timer, &QTimer::timeout, this, [=]() {
        if (ui -> tips-> isHidden())
            ui -> tips -> show();
        else ui -> tips -> hide();
    });

    timer.start(500);
}


void StartWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPixmap pix;

    // 平滑转换算法抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    startImg = QString(":/res/start_%1.png").arg(isStart);
    pix.load(startImg);
    painter.drawPixmap(0, 0, this -> width(), this -> height(), pix);
}


void StartWindow::start() {
    if (!isStart) {
        ui -> tips -> lower();
        QPropertyAnimation *move1 = new QPropertyAnimation(logo, "geometry");
        QPropertyAnimation *move2 = new QPropertyAnimation(ui -> startBtn, "geometry");
        move1 -> setDuration(400);
        move2 -> setDuration(400);
        move1 -> setStartValue(QRect(logo -> x(), logo -> y(), logo -> width(), logo -> height()));
        move2 -> setStartValue(QRect(ui -> startBtn -> x(), ui -> startBtn -> y(), ui -> startBtn -> width(), ui -> startBtn -> height()));
        move1 -> setEndValue(QRect(logo -> x(), 60, logo -> width(), logo -> height()));
        move2 -> setEndValue(QRect(ui -> startBtn -> x(), 704, ui -> startBtn -> width(), ui -> startBtn -> height()));
        move1 -> setEasingCurve(QEasingCurve::OutQuint);
        move2 -> setEasingCurve(QEasingCurve::OutQuint);
        move1 -> start();
        move2 -> start();
    }
    else {
        QPropertyAnimation *move1 = new QPropertyAnimation(logo, "geometry");
        QPropertyAnimation *move2 = new QPropertyAnimation(ui -> startBtn, "geometry");
        move1 -> setDuration(400);
        move2 -> setDuration(400);
        move1 -> setStartValue(QRect(logo -> x(), logo -> y(), logo -> width(), logo -> height()));
        move2 -> setStartValue(QRect(ui -> startBtn -> x(), ui -> startBtn -> y(), ui -> startBtn -> width(), ui -> startBtn -> height()));
        move1 -> setEndValue(QRect(logo -> x(), -200, logo -> width(), logo -> height()));
        move2 -> setEndValue(QRect(ui -> startBtn -> x(), 904, ui -> startBtn -> width(), ui -> startBtn -> height()));
        move1 -> setEasingCurve(QEasingCurve::OutQuint);
        move2 -> setEasingCurve(QEasingCurve::OutQuint);
        move1 -> start();
        move2 -> start();
    }

    isStart = !isStart;
    update();
}

void StartWindow::keyPressEvent(QKeyEvent *event) {
    if(event->isAutoRepeat()) return;
    switch (event -> key()) {
    case Qt::Key_Space:
        start();
        break;
    default:
        break;
    }
}


void StartWindow::on_startBtn_clicked() {
    home = new HomeWindow();
    QTimer::singleShot(400, this, [=]() {   // 延迟 400 ms
        startBgm -> stop();
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        this -> close();
        home -> show();
    });
}




StartWindow::~StartWindow()
{
    delete ui;
}



