#include "gpibgenericexception.h"

GPIBGenericException::GPIBGenericException()
{
}

GPIBGenericException::GPIBGenericException(QString message)
{
    setMessage(message);
}

void GPIBGenericException::setMessage(QString message)
{
    m=message;
}

QString GPIBGenericException::report()
{
    qDebug()<<m;
    return m;
}
