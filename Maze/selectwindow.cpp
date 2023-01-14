#include "selectwindow.h"
#include "ui_selectwindow.h"

SelectWindow::SelectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SelectWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    showFullScreen();
    setWindowIcon(QIcon(":/res/icon.png"));

    idx = 0;
    connect(&timer, &QTimer::timeout, this, [=]() {
        ui->Gate->setStyleSheet(QString("border-image: url(:/res/gate_%1.png)").arg(idx));
        idx = (idx + 1) % 8;
    });

    timer.start(100);
}

SelectWindow::~SelectWindow()
{
    delete ui;
}

void SelectWindow::start(int x) {
    game = new GameWindow(x);
    QTimer::singleShot(400, this, [=]() {   // 延迟 400 ms
        timer.stop();
        disconnect(&timer, &QTimer::timeout, this, nullptr);
        this -> hide();
        game -> show();
    });

    connect(game, &GameWindow::back, this, [=]() {
        game -> close();
        emit back();
    });

}

void SelectWindow::on_btn1_clicked() {
    start(2);
}


void SelectWindow::on_btn2_clicked() {
    start(1);
}


void SelectWindow::on_btn3_clicked() {
    start(0);
}

