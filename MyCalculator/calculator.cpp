#include "mainwindow.h"
#include "ui_mainwindow.h"

// =================== 核心代码一：中缀转后缀 =================== //
QQueue<QString> MainWindow::convert() {
    QStringList  infixExpression = ep.split(" ");
    QQueue<QString> suffixExpression;   // 用于存放后缀表达式的队列
    QStack<QString> stack;        // 符号栈
    for (QString unit : infixExpression)
        if (unit[0].isNumber() || (unit[0] == '-' && unit != "-"))
            suffixExpression.push_back(unit);       // 数字直接入队列
        else if (unit == "(")
            stack.push(unit);       // 左括号直接入栈
        else if (unit == ")") {
            while (stack.top() != "(")          // 右括号将栈中符号弹出，直到左括号
                suffixExpression.push_back(stack.pop());

            stack.pop();    // 弹出左括号
        }
        else {
            // 运算符将栈中元素弹出，直到左括号或更低级运算符，再入栈
            while (!stack.empty() && stack.top() != "(" && priority(unit) <= priority(stack.top()))
                suffixExpression.push_back(stack.pop());

            stack.push(unit);
        }

    while (!stack.empty())      // 弹出除左括号外的所有符号
        if (stack.top() != "(") suffixExpression.push_back(stack.pop());
        else stack.pop();

    return suffixExpression;

    QPushButton *p = new QPushButton(this);
    p -> setGeometry(0, 0, 0, 0);
}

// =================== 核心代码二：后缀的计算 =================== //
void MainWindow::calculate() {
    QQueue<QString> suffixExpression = convert();
    QStack<double> stack;       // 数字栈

    while (!suffixExpression.empty()) {
        QString unit = suffixExpression.front();
        if (unit == "^") {
            double a = stack.pop();     // 栈的储存是相反的
            double b = stack.pop();
            if (b < 0 && a != int(a)) {     // 1. 底数为负，指数为小数
                ui -> Result -> setText("实数范围无法表示");
                error = true;
                return;
            }
            else if (b == 0 && a == 0) {    // 2. 底数指数都为 0
                ui -> Result -> setText("结果未定义");
                error = true;
                return;
            }
            else if (b == 0 && a < 0) {     // 3. 底数为 0，指数为负
                ui -> Result -> setText("被除数不能为零");
                error = true;
                return;
            }

            stack.push(pow(b, a));
        }
        else if (unit == "*") {
            stack.push(stack.pop() * stack.pop());
        }
        else if (unit == "/") {
            double a = stack.pop();
            double b = stack.pop();
            if (a == 0) {
                if (b == 0)
                    ui -> Result -> setText("结果未定义");
                else
                    ui -> Result -> setText("被除数不能为零");

                error = true;
                return;
            }

            stack.push(b / a);
        }
        else if (unit == "%") {
            double a = stack.pop();
            double b = stack.pop();
            if (a == 0) stack.push(b);
            else stack.push(b - int(b / a) * a);
        }
        else if (unit == "+") {
            stack.push(stack.pop() + stack.pop());
        }
        else if (unit == "-") {
            double a = stack.pop();
            double b = stack.pop();
            stack.push(b - a);
        }
        else if (unit == ">>") {
            int a = stack.pop();    // 程序员模式不能输入小数
            int b = stack.pop();
            stack.push(b >> a);
        }
        else if (unit == "<<") {
            int a = stack.pop();    // 程序员模式不能输入小数
            int b = stack.pop();
            stack.push(b << a);
        }
        else if (unit == "&") {
            int a = stack.pop();    // 程序员模式不能输入小数
            int b = stack.pop();
            stack.push(b & a);
        }
        else if (unit == "|") {
            int a = stack.pop();    // 程序员模式不能输入小数
            int b = stack.pop();
            stack.push(b | a);
        }
        else stack.push(unit.toDouble());
        suffixExpression.pop_front();
    }

    // 显示运算结果，将浮点数转为字符串
    res = QString::number(stack.pop());

    if (res == "inf") {
        ui -> Result -> setText("超出数据范围");
        error = true;
        return;
    }

    decToBinAndHex();
    ui -> Result -> setText(res);
    ui -> ResultBin -> setText(bin);
    ui -> ResultHex -> setText(hex);

    // 状态还原
    toBeInput = true;       // 调用函数会使 toBeInput 变为 1
    decimalPoint = false;   // 这里不可以令 done = true
}


// 优先级判断
int MainWindow::priority(QString sign) {
    if (sign == "^") return 4;
    else if (sign == "*" || sign == "/" || sign == "%") return 3;
    else if (sign == "+" || sign == "-") return 2;
    else if (sign == ">>" || sign == "<<") return 1;
    else if (sign == "&" || sign == "|") return 0;
}
