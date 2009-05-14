#ifndef GPIBEXCEPTIONS_H
#define GPIBEXCEPTIONS_H

#include "gpibgenericexception.h"
#if defined(VISA)
#include "gpibvisaexception.h"
#else
#include "gpibbusexception.h"
#endif
#include "gpibidnexception.h"
#include "gpibinvalidreplyexception.h"
#include "gpibunxparamvalexception.h"

#endif // GPIBEXCEPTIONS_H
