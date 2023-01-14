#include "mainwindow.h"
#include "ui_mainwindow.h"


// ========================= 程序员模式 ========================= //
void MainWindow::decToBinAndHex() {
    if (ui->StackedWidget->currentIndex() != 2) {
        return;     // 只有在程序员模式下才会执行，部分情况下会改变 res
    }


    long long int dec1 = (long long int)(res.toDouble());
    if (dec1 != res.toDouble()) res = QString::number(dec1);      // 防止除法出现小数
    if (dec1 == -9223372036854775808) {
        bin = "OUT OF RANGE";
        hex = "OUT OF RANGE";
        res = "超出数据范围";
        error = true;
        return;
    }
    else if (dec1 == 0) {
        bin = "0";
        hex = "0";
        return;
    }

    int k = 0;
    if (dec1 < 0) {
        dec1 = -dec1;
        k = 1;
    }

    long long int dec2 = dec1;
    bin.clear();
    hex.clear();
    while (dec1) {
        bin = list[dec1 % 2] + bin;
        dec1 /= 2;
    }
    while (dec2) {
        hex = list[dec2 % 16] + hex;
        dec2 /= 16;
    }

    if (k) {
        bin = "-" + bin;
        hex = "-" + hex;
    }
}


void MainWindow::on_BtnNot_clicked() {
    long long int dec = 0, d = 1;
    int t = 0, idx = 0, h = 1;
    hex.clear();

    for (int i = bin.size() - 1; i >= 0; --i) {
        bin[i] = bin[i] == '1' ? '0' : '1';

        dec += bin[i] == '1' ? d : 0;
        d *= 2;

        t += bin[i] == '1' ? h : 0;
        h *= 2;
        if (++idx % 4 == 0) {
            h = 1;
            hex = list[t] + hex;
            t = 0;
        }
    }
    if (t) hex = list[t] + hex;

    res = QString::number(dec);
    ui->Result->setText(res);
    ui->ResultBin->setText(bin);
    ui->ResultHex->setText(hex);

//    long long int dec1 = (long long int)(res.toDouble());

//    if (done) {
//        index = historyE.size();

//        ep = "";
//        ui -> Expression -> setText(ep);
//        done = false;
//        toBeInput = true;
//        decimalPoint = false;
//        leftBracketNum = 0;

//        toBeInput = false;
//    }

//    if (res.right(1) == ".") res = res.left(res.size() - 1);

//    if (toBeInput) {
//        return;
//    }
//    else {
//        if (res.left(1) == "-") res = res.right(res.size() - 1);
//        // 更新小数点状态
//        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
//            decimalPoint = true;
//        }
//        ui -> Result -> setText(res);
//    }
}
