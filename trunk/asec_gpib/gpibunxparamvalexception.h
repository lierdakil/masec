#ifndef GPIBUNXPARAMVALEXCEPTION_H
#define GPIBUNXPARAMVALEXCEPTION_H

#include "asec_gpib_global.h"
#include "gpibgenericexception.h"
#include <QString>

class ASEC_GPIBSHARED_EXPORT GPIBUnxParamValException : public GPIBGenericException
{
public:
    GPIBUnxParamValException(QString request, QString value, QString expected);
};

#endif // GPIBUNXPARAMVALEXCEPTION_H
