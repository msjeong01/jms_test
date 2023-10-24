#include "qt_teleop.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qt_teleop w;
    w.show();
    return a.exec();
}
