#ifndef GPIBGENERICEXCEPTION_H
#define GPIBGENERICEXCEPTION_H

#include "asec_gpib_global.h"

#include <QString>

class ASEC_GPIBSHARED_EXPORT GPIBGenericException
{
private:
    QString m;
protected:
    void setMessage(QString message);
public:
    GPIBGenericException();
    GPIBGenericException(QString message);
    QString report();
    //~GenericException();//does not do anything, so might as well not exist
};

#endif // GPIBGENERICEXCEPTION_H
