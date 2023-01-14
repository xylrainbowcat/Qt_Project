#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStack>
#include <QQueue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void clickedNum();
    void clickedSign();
    bool isSign(QString sign);
    QQueue<QString> convert();
    void calculate();
    int priority(QString sign);
    void resizeEvent(QResizeEvent *event);
    void decToBinAndHex();

private slots:
    void on_Btn0_clicked();

    void on_Btn1_clicked();

    void on_Btn2_clicked();

    void on_Btn3_clicked();

    void on_Btn4_clicked();

    void on_Btn5_clicked();

    void on_Btn6_clicked();

    void on_Btn7_clicked();

    void on_Btn8_clicked();

    void on_Btn9_clicked();

    void on_BtnAdd_clicked();

    void on_BtnSub_clicked();

    void on_BtnMulti_clicked();

    void on_BtnDivi_clicked();

    void on_BtnBack_clicked();

    void on_BtnEqual_clicked();

    void on_BtnPoint_clicked();

    void on_BtnBracketLift_clicked();

    void on_BtnBracketRight_clicked();

    void on_Btn0_2_clicked();

    void on_Btn1_2_clicked();

    void on_Btn2_2_clicked();

    void on_Btn3_2_clicked();

    void on_Btn4_2_clicked();

    void on_Btn5_2_clicked();

    void on_Btn6_2_clicked();

    void on_Btn7_2_clicked();

    void on_Btn8_2_clicked();

    void on_Btn9_2_clicked();

    void on_BtnEqual_2_clicked();

    void on_BtnPoint_2_clicked();

    void on_BtnAdd_2_clicked();

    void on_BtnSub_2_clicked();

    void on_BtnMulti_2_clicked();

    void on_BtnDivi_2_clicked();

    void on_BtnBack_2_clicked();

    void on_BtnC_clicked();

    void on_BtnC_2_clicked();

    void on_BtnE_clicked();

    void on_BtnPi_clicked();

    void on_BtnNeg_clicked();

    void on_BtnNeg_2_clicked();

    void on_Btn1_3_clicked();

    void on_Btn2_3_clicked();

    void on_Btn3_3_clicked();

    void on_Btn4_3_clicked();

    void on_Btn5_3_clicked();

    void on_Btn6_3_clicked();

    void on_Btn7_3_clicked();

    void on_Btn8_3_clicked();

    void on_Btn9_3_clicked();

    void on_Btn0_3_clicked();

    void on_BtnEqual_3_clicked();

    void on_BtnAdd_3_clicked();

    void on_BtnSub_3_clicked();

    void on_BtnMulti_3_clicked();

    void on_BtnDivi_3_clicked();

    void on_BtnBack_3_clicked();

    void on_BtnC_3_clicked();

    void on_BtnBracketRight_2_clicked();

    void on_BtnBracketLift_2_clicked();


    void on_BtnAnd_clicked();

    void on_BtnOr_clicked();

    void on_BtnMod_2_clicked();

    void on_BtnMod_clicked();


    void on_BtnPower_clicked();

    void on_BtnMoveLeft_clicked();

    void on_BtnMoveRight_clicked();

    void on_BtnExp_clicked();

    void on_BtnAns_clicked();

    void on_BtnAns_2_clicked();

    void on_BtnSin_clicked();

    void on_BtnCos_clicked();

    void on_BtnAbs_clicked();

    void on_BtnNot_clicked();

    void on_BtnSqrt_2_clicked();

    void on_BtnSqrt_clicked();

    void on_BtnPower10_clicked();


    void on_BtnLg_clicked();

    void on_BtnLn_clicked();

    void on_BtnSquare_clicked();

    void on_BtnFraction_clicked();

    void on_BtnFact_clicked();

private:
    Ui::MainWindow *ui;
    QStringList historyE;
    QStringList historyR;
    QString ep;
    QString res;
    QString bin;
    QString hex;
    int index;
    bool done;
    bool toBeInput;
    bool error;
    bool decimalPoint;      //记录小数点是否已经输入，一个数里面不可以有重复的小数点，当前输入为整数时为false，为小数时为true
    int leftBracketNum;     // 记录左括号数量，右括号数量不得大于左括号数量
    QString list;
};
#endif // MAINWINDOW_H
