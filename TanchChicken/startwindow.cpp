#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StartWindow) {
    ui->setupUi(this);

    setFixedSize(1280, 720);
    setWindowTitle("Tanch Chicken");
    setWindowIcon(QIcon(":/res/1.png"));

    isNoEnergy = false;
    noEnergyLabel = new QLabel(this);
    noEnergyLabel -> setGeometry(480, -100, 200, 50);
    noEnergyLabel -> setText("没有香翅捞饭啦 ~");
    noEnergyLabel -> setFont(QFont("Microsoft YaHei", 14, 75));
    noEnergyLabel -> setStyleSheet("background-color: rgba(161, 161, 161, 100); border-radius: 10px; border-width: 0; border-style: outset;");
    noEnergyLabel -> setAlignment(Qt::AlignCenter);

    isPay = false;
    pay = new QLabel(this);
    pay -> setGeometry(440, 720, 400, 400);
    pay -> setStyleSheet("border-image: url(:/res/pay.png)");

    chickenNum = 8;
    idx = 0;
    ui -> stackedWidget -> setCurrentIndex(idx);



    QDateTime time;
    QString time_str = time.currentDateTime().toString("yyyy-MM-dd");

    QDir *DataFile = new QDir;
    bool exist = DataFile->exists("DataFile");
    if(!exist) DataFile->mkdir("DataFile"); // 新建文件夹


    QString filePath = "DataFile/data.txt";
    QFile datafile(filePath);
    datafile.open(QIODevice::ReadOnly);
    QTextStream stream(&datafile);
    QString line = stream.readLine();
    bool k = false;
    if (line != time_str) {
        k = true;
    }
    else {
        line = stream.readLine();
        energy = line.toInt();
        ui -> Energy -> setText(QString::number(energy));
    }
    datafile.close();

    if (k) {
        datafile.open(QIODevice::WriteOnly);
        QTextStream stream(&datafile);
        energy = 10;
        ui -> Energy -> setText(QString::number(energy));
        stream << time_str << "\n" << QString::number(energy);
        datafile.close();
    }


    filePath = "DataFile/datafile.txt";
    QFile historyfile(filePath);
    historyfile.open(QIODevice::ReadOnly);
    QByteArray arr = historyfile.readAll();
    ui -> History -> setText(arr);
    historyfile.close();
}

StartWindow::~StartWindow()
{
    delete ui;
}


void StartWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QPixmap pix;

    // 平滑转换算法抗锯齿
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    pix.load(":/res/background.jpg");
    painter.drawPixmap(0, 0, this -> width(), this -> height(), pix);

    pix.load(":/res/logo.png");
    pix = pix.scaled(pix.width() * 0.5, pix.height() * 0.5);
    painter.drawPixmap(40, 20, pix);
}

void StartWindow::on_BtnRight_clicked() {
    if (idx == 6) ui -> BtnStart -> setText("敬请期待");
    else if (idx == 7) ui -> BtnStart -> setText("开始");

    idx = (idx + 1) % chickenNum;
    ui -> stackedWidget -> setCurrentIndex(idx);
}


void StartWindow::on_BtnLeft_clicked() {
    if (idx == 0) ui -> BtnStart -> setText("敬请期待");
    else if (idx == 7) ui -> BtnStart -> setText("开始");

    idx = (idx + chickenNum - 1) % chickenNum;
    ui -> stackedWidget -> setCurrentIndex(idx);
}

void StartWindow::on_BtnStart_clicked() {
    if (idx == 7) return;
    if (energy <= 0) {
        if (!isNoEnergy) noEnergy();
        return;
    }

    energy--;
    game = new GameWindow(ui -> stackedWidget -> currentIndex() + 1);

    QTimer::singleShot(400, this, [=]() {   // 延迟 400 ms
        this -> hide();
        game -> show();
    });

    connect(game, &GameWindow::back, this, [=]() {
        QDir *DataFile = new QDir;
        bool exist = DataFile->exists("DataFile");
        if(!exist) DataFile->mkdir("DataFile");


        QString filePath = "DataFile/data.txt";
        QFile datafile(filePath);
        if (energy == 9) datafile.resize(datafile.size() - 2);
        else datafile.resize(datafile.size() - 1);
        datafile.open(QIODevice::Append);
        QTextStream stream(&datafile);
        stream << QString::number(energy);
        datafile.close();


        filePath = "DataFile/datafile.txt";
        QFile historyfile(filePath);
        historyfile.open(QIODevice::ReadOnly);
        QByteArray arr = historyfile.readAll();
        ui -> History -> setText(arr);
        historyfile.close();


        game -> close();
        ui -> Energy -> setText(QString::number(energy));
        this -> show();
    });
}


void StartWindow::keyPressEvent(QKeyEvent *event) {
    switch (event -> key()) {
    case Qt::Key_Right:
        on_BtnRight_clicked();
        break;
    case Qt::Key_Left:
        on_BtnLeft_clicked();
        break;
    case Qt::Key_Space:
        on_BtnStart_clicked();
        break;
    default:
        break;
    }
}


void StartWindow::noEnergy() {

    isNoEnergy = true;
    QPropertyAnimation *move = new QPropertyAnimation(noEnergyLabel, "geometry");
    move -> setDuration(500);
    move -> setStartValue(QRect(noEnergyLabel -> x(), noEnergyLabel -> y(), noEnergyLabel -> width(), noEnergyLabel -> height()));
    move -> setEndValue(QRect(noEnergyLabel -> x(), noEnergyLabel -> y() + 130, noEnergyLabel -> width(), noEnergyLabel -> height()));
    move -> setEasingCurve(QEasingCurve::OutExpo);
    move -> start();

    QTimer::singleShot(1500, this, [=]() {
        QPropertyAnimation *move = new QPropertyAnimation(noEnergyLabel, "geometry");
        move -> setDuration(500);
        move -> setStartValue(QRect(noEnergyLabel -> x(), noEnergyLabel -> y(), noEnergyLabel -> width(), noEnergyLabel -> height()));
        move -> setEndValue(QRect(noEnergyLabel -> x(), noEnergyLabel -> y() - 130, noEnergyLabel -> width(), noEnergyLabel -> height()));
        move -> setEasingCurve(QEasingCurve::InExpo);
        move -> start();
    });

    QTimer::singleShot(2100, this, [=]() {
        isNoEnergy = false;
    });
}

void StartWindow::on_BtnBuy_clicked() {
    if (pay -> y() != 720 && pay -> y() != 200) return;

    if (!isPay) {
        QPropertyAnimation *move = new QPropertyAnimation(pay, "geometry");
        move -> setDuration(600);
        move -> setStartValue(QRect(pay -> x(), pay -> y(), pay -> width(), pay -> height()));
        move -> setEndValue(QRect(pay -> x(), pay -> y() - 520, pay -> width(), pay -> height()));
        move -> setEasingCurve(QEasingCurve::OutExpo);
        move -> start();
        isPay = true;

    }
    else {
        QPropertyAnimation *move = new QPropertyAnimation(pay, "geometry");
        move -> setDuration(600);
        move -> setStartValue(QRect(pay -> x(), pay -> y(), pay -> width(), pay -> height()));
        move -> setEndValue(QRect(pay -> x(), pay -> y() + 520, pay -> width(), pay -> height()));
        move -> setEasingCurve(QEasingCurve::InExpo);
        move -> start();

        QString filePath = "DataFile/data.txt";
        QFile datafile(filePath);
        if (energy == 10) datafile.resize(datafile.size() - 2);
        else datafile.resize(datafile.size() - 1);
        datafile.open(QIODevice::Append);
        QTextStream stream(&datafile);
        stream << "10";
        datafile.close();

        isPay = false;
        energy = 10;
        ui -> Energy -> setText(QString::number(energy));
    }
}

