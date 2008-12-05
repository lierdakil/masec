#include "asec_control.h"

#include <QtGui>
#include <QApplication>

// TODO: Continue button if module failure occurs

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vib_control w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}
