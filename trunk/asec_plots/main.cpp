#include "asec_plots.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    asec_plots w;
    w.show();

    return a.exec();
}
