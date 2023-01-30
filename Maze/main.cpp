#include "startwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString filePath = "DataFile/scale.txt";
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString scale = stream.readLine();

    qputenv("QT_SCALE_FACTOR", scale.toUtf8());
    QApplication a(argc, argv);
    StartWindow w;
    //SelectWindow w;
    w.show();

    return a.exec();
}
