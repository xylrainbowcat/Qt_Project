#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//              ."" '<  `.___\_<|>_/___.'  >'"".                  //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//      ========`-.____`-.___\_____/___.-`____.-'========         //
//                           `=---='                              //
//      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//             佛祖保佑      永无BUG      永不修改                 //
////////////////////////////////////////////////////////////////////

/* 待完善：
 *
 *
*/

// =========================== 主函数 =========================== //
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    resize(450, 680);
    setWindowTitle("巨硬计算器");
    setWindowIcon(QIcon(":/res/logo.png"));

    historyE.clear();
    historyR.clear();

    init();
    list = "0123456789ABCDEF";

    index = historyE.size();

    ui->StackedWidget->setCurrentIndex(0);


    // 透明效果
    ui -> Expression -> setStyleSheet("background : transparent; border-width : 0; border-style : outset; color: #616161");
    ui -> Result -> setStyleSheet("background : transparent; border-width : 0; border-style : outset");
    ui -> ResultBin -> setStyleSheet("background : transparent; border-width : 0; border-style : outset");
    ui -> ResultHex -> setStyleSheet("background : transparent; border-width : 0; border-style : outset");
    ui -> AllHistory -> setStyleSheet("background : transparent; border-width : 0; border-style : outset");

    // 隐藏侧边栏
    ui -> AllHistory -> setVisible(0);
    ui -> AllHistory -> clear();        // 初始化
    ui -> AllHistory -> setAlignment(Qt::AlignRight);   // 右对齐


    // 切换到标准模式
    connect(ui->ActionNormal, &QAction::triggered, [=]() {
        // 从程序员模式切换回来需要还原历史记录
        if (ui -> StackedWidget -> currentIndex() == 2) {
            ui -> AllHistory -> clear();
            ui -> AllHistory -> setAlignment(Qt::AlignRight);

            for (int i = 0; i < index; ++i) {
                ui -> AllHistory -> setTextColor("#616161"); // 设置下一行颜色和字号
                ui -> AllHistory -> setFontPointSize(14);
                ui -> AllHistory -> append(historyE[i]);
                ui -> AllHistory -> setFontPointSize(18);
                ui -> AllHistory -> setTextColor(Qt::black);
                ui -> AllHistory -> append(historyR[i] + "\n");
            }
        }

        // 切换模式时会初始化界面
        if (ui -> StackedWidget -> currentIndex() != 0) init();
        ui->StackedWidget->setCurrentIndex(0);
        index = historyE.size();
    });

    // 切换到科学模式
    connect(ui->ActionScience, &QAction::triggered, [=]() {
        if (ui -> StackedWidget -> currentIndex() == 2) {
            ui -> AllHistory -> clear();
            ui -> AllHistory -> setAlignment(Qt::AlignRight);

            // 还原历史
            for (int i = 0; i < index; ++i) {
                ui -> AllHistory -> setTextColor("#616161"); // 设置下一行颜色和字号
                ui -> AllHistory -> setFontPointSize(14);
                ui -> AllHistory -> append(historyE[i]);
                ui -> AllHistory -> setFontPointSize(18);
                ui -> AllHistory -> setTextColor(Qt::black);
                ui -> AllHistory -> append(historyR[i] + "\n");
            }
        }

        if (ui -> StackedWidget -> currentIndex() != 1) init();
        ui->StackedWidget->setCurrentIndex(1);
        index = historyE.size();
    });

    // 切换到程序员模式
    connect(ui->ActionProgrammer, &QAction::triggered, [=]() {
        ui -> AllHistory -> clear();
        ui -> AllHistory -> setAlignment(Qt::AlignLeft);
        ui -> AllHistory -> setTextColor("#616161");
        ui -> AllHistory -> setFontPointSize(14);
        ui -> AllHistory -> append("程序员模式下不支持历史功能");

        if (ui -> StackedWidget -> currentIndex() != 2) init();
        ui->StackedWidget->setCurrentIndex(2);
        index = historyE.size();
    });

    // 上一条历史记录
    connect(ui -> ActionPrev, &QAction::triggered, [=]() {
        if (ui -> StackedWidget -> currentIndex() == 2) return;     // 程序员模式下不允许调用历史记录
        if (index > 0) {
            --index;
            init();                         // 调用完历史记录后为 toBeInput 状态
            ep = historyE[index];
            res = historyR[index];
            ui -> Expression -> setText(ep);
            ui -> Result -> setText(res);
            done = true;
        }
    });

    // 下一条历史记录
    connect(ui -> ActionNext, &QAction::triggered, [=]() {
        if (ui -> StackedWidget -> currentIndex() == 2) return;
        if (index < historyE.size() - 1) {
            ++index;
            init();
            ep = historyE[index];
            res = historyR[index];
            ui -> Expression -> setText(ep);
            ui -> Result -> setText(res);
            done = true;
        }
    });

    // 情况历史记录
    connect(ui -> ActionClear, &QAction::triggered, [=]() {
        if (ui -> StackedWidget -> currentIndex() == 2) return;
        historyE.clear();
        historyR.clear();
        index = historyE.size();
        ui -> AllHistory -> setText("");        // 初始化
        ui -> AllHistory -> setAlignment(Qt::AlignRight);   // 右对齐
    });
}

// 实时监测窗口大小，并隐藏/显示侧边栏
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (this -> width() > 800) ui -> AllHistory -> setVisible(1);
    else ui -> AllHistory -> setVisible(0);
}


MainWindow::~MainWindow() {
    delete ui;
}


// =========================== 初始化 =========================== //
void MainWindow::init() {
    // 输入框初始化
    ep = "";
    res = "0";
    bin = "0";
    hex = "0";
    ui -> Expression -> setText(ep);
    ui -> Result -> setText(res);
    ui -> ResultBin -> setText(bin);
    ui -> ResultHex -> setText(hex);
    // index = historyE.size();     不能加，再调用上一条历史时会用到
    // 状态初始化
    error = false;
    done = false;
    toBeInput = true;
    decimalPoint = false;
    leftBracketNum = 0;
}


// ========================= 运算符判断 ========================= //
bool MainWindow::isSign(QString sign) {
    if (sign == "+" || sign == "-" || sign == "*" || sign == "/" || sign == "^" || sign == "%" || sign == "&" || sign == "|" || sign == ">>" || sign == "<<" || sign == ">" || sign == "<")
        return true;
    return false;
}

// ========================== 数字按钮 ========================== //
void MainWindow::clickedNum() {
    if (done || error) {     // done 和 error 状态
        init();
        index = historyE.size();    // 开启一条新的历史记录
    }

    QString num = ((QPushButton*)sender()) -> text();   // 获取发送点击信号的按钮的 text 值

    if (toBeInput) {        // toBeInput 状态
        res = num;
        toBeInput = false;
    }
    else {                  // normal 状态
        if (res == "0") res = num;
        else if (res.size() <= 16) res += num;  // 最多输入 16 位数
    }

    decToBinAndHex();       // 程序员模式下转换为二进制和十六进制
    ui -> Result -> setText(res);
    ui -> ResultBin -> setText(bin);
    ui -> ResultHex -> setText(hex);
}

// ========================== 符号按钮 ========================== //
void MainWindow::clickedSign() {
    if (error) {            // error 状态直接初始化屏幕
        init();
        index = historyE.size();
    }
    else if (done) {        // done 状态调用上次结果
        if (ui -> StackedWidget -> currentIndex() == 2) {
            QString s = ui->Result->text();
            init();
            res = s;
        }
        else {
            init();
            res = historyR[index];
            qDebug() << 2;                  // =========== bug: 程序员模式无历史 (已修复)
            index = historyE.size();
        }
    }

    QString sign = ((QPushButton*)sender()) -> text(); // 获取发送点击信号的按钮的 text 值
    if (sign == "x^y") sign = "^";          // 特殊符号的处理
    else if (sign == "MOD") sign = "%";
    else if (sign == "AND") sign = "&";
    else if (sign == "OR") sign = "|";

    if (res.right(1) == ".") res = res.left(res.size() - 1);    // 去掉要调用的数多余小数点

    if (toBeInput) {        // toBeInput 状态
        if (ep == "")
            ep = res;
        else if (isSign(ep.right(1)))       // isSign 函数判断前一个元素是否也为运算符
            if (ep.right(1) == "<" || ep.right(1) == ">") ep = ep.left(ep.size() - 3);
            else ep = ep.left(ep.size() - 2);    // 若是，则替换（特判 << 和 >>）
        else if (ep.right(1) == "(")
            ep += " " + res;

        convert();          // 实时显示已输入表达式的结果
        calculate();        // 运算后在函数中会将状态置为 toBeInput，同时将 decimalPoint 置为 false
        ep += " " + sign;
    }
    else {                  // normal 状态
        if (ep == "")
            ep = res;
        else if (isSign(ep.right(1)) || ep.right(1) == "(")
            ep += " " + res;
        else if (ep.right(1) == ")")
            ep += " * " + res;

        convert();
        calculate();
        ep += " " + sign;
    }

    ui-> Expression -> setText(ep);     // 更新表达式输出
}


// =========================== 左括号 =========================== //
void MainWindow::on_BtnBracketLift_clicked() {
    if (done || error) {        // done 和 error 状态
        init();
        index = historyE.size();
    }

    if (res.right(1) == ".") res = res.left(res.size() - 1);    // 去掉要调用的数多余小数点

    if (toBeInput) {            // toBeInput 状态
        if (ep == "")
            ep = "(";
        else if (isSign(ep.right(1)) || ep.right(1) == "(")
            ep += " (";
        else if (ep.right(1) == ")")
            ep += " * (";
    }
    else {                      // normal 状态
        if (ep == "")
            ep = res;
        else if (isSign(ep.right(1)) || ep.right(1) == "(")
            ep += " " + res;
        else if (ep.right(1) == ")")
            ep += " * " + res;

        convert();
        calculate();
        ep += " * (";
    }

    ui-> Expression -> setText(ep);
    ++leftBracketNum;           // 未匹配左括号数量加一
}

// =========================== 右括号 =========================== //
void MainWindow::on_BtnBracketRight_clicked() {
    if (done || error) {        // done 和 error 状态
        init();
        index = historyE.size();
    }

    if (leftBracketNum > 0) {       // 只有在 leftBracketNum > 0 时才会执行
        if (res.right(1) == ".") res = res.left(res.size() - 1);    // 去掉要调用的数多余小数点

        if (isSign(ep.right(1)) || ep.right(1) == "(") {
            ep += " " + res;
            convert();
            calculate();
        }
        else if (!toBeInput && ep.right(1) == ")") {
            ep += " * " + res;
            convert();
            calculate();
        }

        ep += " )";
        ui -> Expression -> setText(ep);
        --leftBracketNum;       // 未匹配左括号数量减一
    }
}

// =========================== 小数点 =========================== //
void MainWindow::on_BtnPoint_clicked() {
    if (done || error) {
        init();
        index = historyE.size();
    }

    if (toBeInput) {
        res = "0.";
        ui -> Result -> setText(res);
        decimalPoint = true;
        toBeInput = false;
    }
    else {
        if (!decimalPoint) {    //防止一个数字里面有两个小数点
            res += ".";
            ui -> Result -> setText(res);
            decimalPoint = true;
        }
    }
}

// =========================== 退格键 =========================== //
void MainWindow::on_BtnBack_clicked() {
    if (done || error) {
        init();
        index = historyE.size();
        return;
    }

    if (toBeInput && res != "0") {
        res = "0";
        ui -> Result -> setText(res);
        ui -> ResultBin -> setText("0");
        ui -> ResultHex -> setText("0");
    }
    else {
        if (res.size() > 1) {      // 字符数量大于 1，直接删除
            QString del = res.right(1);   // 获取要删除的字符
            if (del == ".")
                decimalPoint = false;   // 若删除小数点，更新小数点状态

            res = res.left(res.size() - 1);

            // 单独为正负号按钮设置的判断
            if (res == "-") res = "0";


            decToBinAndHex();
            ui -> Result -> setText(res);
            ui -> ResultBin -> setText(bin);
            ui -> ResultHex -> setText(hex);
        }
        else if (res != "0") {
            res = "0";
            ui -> Result -> setText(res);  // 字符数量等于 1，将其变为 0
            ui -> ResultBin -> setText("0");
            ui -> ResultHex -> setText("0");
            toBeInput = true;
        }
//        else  {
//            int k = 1;
//            if (ep.right(1) == ")") ++leftBracketNum;   // 括号数还原
//            if (ep.right(1) == "(") --leftBracketNum;       // ============ 未实现

//            for (int i = ep.size() - 3; i >= 0; --i)
//                if ( (ep[i] < '0' || ep[i] > '9' ) && ep[i] != '.' && ep[i] != ' ') {
//                    ui -> Expression -> setText(ep.left(i + 1));
//                    k = 0;
//                    break;
//                }
//            if (k) {
//                init();
//            }
//        }
    }
}



// ============================ 等号 ============================ //
void MainWindow::on_BtnEqual_clicked() {
    if (done || error) return;

    if (res.right(1) == ".") res = res.left(res.size() - 1);

    if (ep == "")
        ep = res;
    else if (isSign(ep.right(1)) || ep.right(1) == "(")
        ep += " " + res;
    else if (!toBeInput)
        ep += " * " + res;



    while (leftBracketNum--) ep += " )";
    //qDebug() << leftBracketNum;               // 此时 leftBracketNum 为 -1，但是不用做处理

    convert();
    calculate();
    ep += " =";
    ui -> Expression -> setText(ep);

    if (!error && ui -> StackedWidget -> currentIndex() != 2) {   // 程序员模式下不允许保存历史
        historyE << ep;
        historyR << res;
        index = historyE.size() - 1;    // 此时处于当前一条历史记录的位置

        // 更新历史记录窗口
        ui -> AllHistory -> setTextColor("#616161"); // 设置下一行颜色和字号
        ui -> AllHistory -> setFontPointSize(14);
        ui -> AllHistory -> append(ep);
        ui -> AllHistory -> setFontPointSize(18);
        ui -> AllHistory -> setTextColor(Qt::black);
        ui -> AllHistory -> append(res + "\n");
    }

    done = true;    // 更新 done 状态
}

// =========================== 归零键 =========================== //
void MainWindow::on_BtnC_clicked() { 
    if (done || error) {
        init();
        index = historyE.size();
        return;
    }

    if (res != "0") {
        res = "0";
        ui -> Result -> setText(res);
        ui -> ResultBin -> setText(res);
        ui -> ResultHex -> setText(res);
        toBeInput = true;
        decimalPoint = false;
    }
    else init();
}



// =========================== 正负号 =========================== //
void MainWindow::on_BtnNeg_clicked() {
    if (error) {
        init();
        index = historyE.size();
        return;
    }
    else if (done) {
        init();
        res = historyR[index];
        index = historyE.size();
    }

    if (res.left(1) == "-") res = res.right(res.size() - 1);
    else if (res != "0") res = "-" + res;

    ui -> Result -> setText(res);     // 若为 toBeInput 变为 !toBeInput 状态
    toBeInput = false;
}

// ========================== ans 键 ========================== //
void MainWindow::on_BtnAns_clicked() {
    if (done || error) {
        init();
        index = historyE.size();
    }

    if (index != 0) {       // 有历史记录时才会执行
        if (toBeInput) {
            res = historyR[index - 1];
            ui -> Result -> setText(res);
            toBeInput = false;
        }
        else {
            res = historyR[index - 1];
            ui -> Result -> setText(res);
        }

        // 更新小数点状态
        if (res.indexOf(".") != -1 || res.indexOf("e") != -1) {
            decimalPoint = true;
        }
    }
}




