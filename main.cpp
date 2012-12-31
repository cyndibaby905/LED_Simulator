#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setMinimumSize(600,200);
    w.resize(600,200);
    w.show();
    return a.exec();
}
