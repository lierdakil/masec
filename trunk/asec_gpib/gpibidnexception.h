#ifndef GPIBIDNEXCEPTION_H
#define GPIBIDNEXCEPTION_H

#include "gpibgenericexception.h"

class ASEC_GPIBSHARED_EXPORT GPIBIDNException : public GPIBGenericException
{
public:
    GPIBIDNException(QString ID, QString _IDN, QString IDN);
};

#endif // GPIBIDNEXCEPTION_H
