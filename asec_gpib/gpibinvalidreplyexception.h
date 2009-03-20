#ifndef GPIBINVALIDREPLYEXCEPTION_H
#define GPIBINVALIDREPLYEXCEPTION_H

#include "asec_gpib_global.h"

#include "gpibgenericexception.h"
#include <QString>

class ASEC_GPIBSHARED_EXPORT GPIBInvalidReplyException : public GPIBGenericException
{
public:
    GPIBInvalidReplyException(QString request, QString reply, QString expected);
};

#endif // GPIBINVALIDREPLYEXCEPTION_H
