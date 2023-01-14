#ifndef RESET_H
#define RESET_H

#include <QWidget>
#include "globel.h"

namespace Ui {
class Reset;
}

class Reset : public QWidget
{
    Q_OBJECT

public:
    explicit Reset(QString name, QWidget *parent = nullptr);
    ~Reset();

signals:
    void reset();

private slots:
    void on_Btn_No_clicked();

    void on_Btn_Yes_clicked();

private:
    Ui::Reset *ui;

    QString name;
};

#endif // RESET_H
