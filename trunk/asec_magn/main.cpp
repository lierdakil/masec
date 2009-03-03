#include "asec_magn.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    asec_magn w;
    w.show();
    return a.exec();
}
