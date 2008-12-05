#include "asec_plots.h"

#include <QtGui>
#include <QApplication>

// TODO: Segfault if two plots are built, with both modules running
// see bugged_plot.fcs

// TODO: forbid empty plot_window title

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    asec_plots w;
    w.show();

    return a.exec();
}
