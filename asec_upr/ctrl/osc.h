#ifndef CTRL_OSC_H_
#define CTRL_OSC_H_

#include <gpibctrl.h>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QDebug>

class oscctrl: public GPIBctrl
{
public:
    enum Mode {
        Auto,
        Normal
    };

    oscctrl(QString GPIBID);
    ~oscctrl();
    QByteArray readcurve();
    void wait(QString state);
    void setch1(double vol);
    void trig_mode(Mode mode);
    double getch1();
    double ymul();
};

#endif /*CTRL_OSC_H_*/
