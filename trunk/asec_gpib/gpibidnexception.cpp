#include "gpibidnexception.h"

GPIBIDNException::GPIBIDNException(QString ID, QString _IDN, QString IDN)
        /* _IDN = real device string
           IDN = expected device string */
{
    setMessage(QString::fromUtf8("Unexpected device IDN string at %1: '%2' (Expected '%3')").arg(ID,_IDN,IDN));
}
