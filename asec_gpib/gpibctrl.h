#ifndef GPIBCTRL_H
#define GPIBCTRL_H

#include "asec_gpib_global.h"

#include <QtCore>
#ifdef VISA
#include <visa.h>
#elif LINUXGPIB
#include <gpib/ib.h>
#endif
#include "gpibexceptions.h"

class ASEC_GPIBSHARED_EXPORT GPIBctrl {
public:
    private:
#ifdef VISA
    ViSession defaultRM, did;
#elif LINUXGPIB
    int did;
#endif
    QMutex mutex;

public:
    GPIBctrl(QString GPIBID, QString IDN, int timeout=5000);
    ~GPIBctrl();

    void write(QString string);

    QByteArray readArray(int maxlength);
    QString readString();

    QString queryString(QString request, QString preamble=QString());
    float queryFloat(QString request, QString preamble=QString());
    int queryInt(QString request, QString preamble=QString());

    QStringList queryStringList(QString request, QChar separator, int count=0, QString preamble=QString());
    QList<float> queryFloatList(QString request, QChar separator, int count=0, QString preamble=QString());
    QList<int> queryIntList(QString request, QChar separator, int count=0, QString preamble=QString());
    QVariantList queryVariantList(QString request, QChar separator, QString fmt, QString preamble=QString());
};

#endif // GPIBCTRL_H
