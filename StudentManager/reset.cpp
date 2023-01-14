#include "reset.h"
#include "ui_reset.h"

Reset::Reset(QString name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reset)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint);    // 隐藏标题栏
    setFixedSize(300, 180);

    this->name = name;
}

Reset::~Reset()
{
    delete ui;
}

void Reset::on_Btn_No_clicked()
{
    this->close();
}


void Reset::on_Btn_Yes_clicked()
{
    if (ui->Name->text() == name) {
        this->close();
        emit reset();
    }
    else {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setWindowIcon(QIcon(":/res/logo.png"));
        box.setWindowTitle("警告");
        box.setText("验证失败");
        box.exec();
    }

}

