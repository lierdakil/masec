#ifndef GPIBVISAEXCEPTION_H
#define GPIBVISAEXCEPTION_H

#include "asec_gpib_global.h"

#include "gpibgenericexception.h"
#include <gpib/ib.h>

class ASEC_GPIBSHARED_EXPORT GPIBBusException : public GPIBGenericException
{
public:
    GPIBBusException(int err);
};

#endif // GPIBVISAEXCEPTION_H
