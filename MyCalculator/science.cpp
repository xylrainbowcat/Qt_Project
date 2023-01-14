#include "mainwindow.h"
#include "ui_mainwindow.h"

// ========================== 科学模式 ========================== //

void MainWindow::on_BtnE_clicked() {
    if (done || error) {
        init();
        index = historyE.size();
    }

    if (toBeInput) {
        res = QString::number(M_E, 'f', 12);
        ui -> Result -> setText(res);
        decimalPoint = true;        // 小数点置为 true
        toBeInput = false;
    }
    else {
        res = QString::number(M_E, 'f', 12);
        ui -> Result -> setText(res);       // 科学模式专有的函数不用考虑程序员模式的情况
        decimalPoint = true;
    }
}


void MainWindow::on_BtnPi_clicked() {
    if (done || error) {
        init();
        index = historyE.size();
    }

    if (toBeInput) {
        res = QString::number(M_PI, 'f', 12);
        ui -> Result -> setText(res);
        decimalPoint = true;
        toBeInput = false;
    }
    else {
        res = QString::number(M_PI, 'f', 12);
        ui -> Result -> setText(res);
        decimalPoint = true;
    }
}

// exp 按钮实现逻辑
void MainWindow::on_BtnExp_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {        // 不改变状态
        return;
    }
    else {
        res = QString::number(exp(res.toDouble()));
        if (res == "inf") {
            ui -> Result -> setText("超出数据范围");
            error = true;
            return;
        }

        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}

// sin 按钮实现逻辑
void MainWindow::on_BtnSin_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        res = QString::number(sin(res.toDouble()));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}

void MainWindow::on_BtnCos_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        res = QString::number(cos(res.toDouble()));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}



void MainWindow::on_BtnAbs_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res.left(1) == "-") res = res.right(res.size() - 1);
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}


void MainWindow::on_BtnSqrt_2_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res.left(1) == "-") {
            ui -> Result -> setText("实数范围无法表示");
            error = true;
            return;
        }
        else res = QString::number(sqrt(res.toDouble()));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}


void MainWindow::on_BtnPower10_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        res = QString::number(pow(10, res.toDouble()));
        if (res == "inf") {
            ui -> Result -> setText("超出数据范围");
            error = true;
            return;
        }

        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}





void MainWindow::on_BtnLg_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res.left(1) == "-" || res == "0") {
            ui -> Result -> setText("真数应为正数");
            error = true;
            return;
        }
        res = QString::number(log10(res.toDouble()));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}


void MainWindow::on_BtnLn_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res.left(1) == "-" || res == "0") {
            ui -> Result -> setText("真数应为正数");
            error = true;
            return;
        }
        res = QString::number(log(res.toDouble()));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}


void MainWindow::on_BtnSquare_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        res = QString::number(pow(res.toDouble(), 2));
        if (res == "inf") {
            ui -> Result -> setText("超出数据范围");
            error = true;
            return;
        }

        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}


void MainWindow::on_BtnFraction_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res == "0") {
            ui -> Result -> setText("分母不能为零");
            error = true;
            return;
        }
        res = QString::number(pow(res.toDouble(), -1));
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}

void MainWindow::on_BtnFact_clicked() {
    if (error) {
        init();
        index = historyE.size();
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();

        toBeInput = false;
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (toBeInput) {
        return;
    }
    else {
        if (res.left(1) == "-" || res.indexOf(".") != -1) {
            ui -> Result -> setText("不支持该运算");
            error = true;
            return;
        }
        else if (res.toDouble() > 171) {
            ui -> Result -> setText("超出数据范围");
            error = true;
            return;
        }

        double ans = 1;
        for (double i = res.toDouble(); i != 1; --i) {
            ans *= i;
        }
        res = QString::number(ans);
        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
        ui -> Result -> setText(res);
    }
}

