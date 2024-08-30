#include "MyCAD.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyCAD w;
    w.resize(1124, 800);
    w.show();
    return a.exec();
}
