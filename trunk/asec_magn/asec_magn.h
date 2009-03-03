#ifndef ASEC_MAGN_H
#define ASEC_MAGN_H

#include <QtGui/QWidget>
#include "ui_asec_magn.h"

class asec_magn : public QWidget
{
    Q_OBJECT

public:
    asec_magn(QWidget *parent = 0);
    ~asec_magn();

private:
    Ui::asec_magnClass ui;
};

#endif // ASEC_MAGN_H
