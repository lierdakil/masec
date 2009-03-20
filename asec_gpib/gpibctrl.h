#ifndef GPIBCTRL_H
#define GPIBCTRL_H

#include "asec_gpib_global.h"

#include <QtCore>
#include <visa.h>
#include "gpibexceptions.h"

class ASEC_GPIBSHARED_EXPORT GPIBctrl {
public:
    private:
    ViSession defaultRM, did;
    QMutex mutex;//TODO: o_O When I make it static pointer, compilation fails
    //TODO: Make mutex at least process-wise... somehow...

public:
    GPIBctrl(QString GPIBID, QString IDN, int timeout=5000);
    ~GPIBctrl();

    void write(QString string);
    QString read();
    QString query(QString request);

    QByteArray read_array(int maxlength);
};

#endif // GPIBCTRL_H
