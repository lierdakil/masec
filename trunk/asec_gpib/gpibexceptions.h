#ifndef GPIBEXCEPTIONS_H
#define GPIBEXCEPTIONS_H

#include "gpibgenericexception.h"
#ifdef VISA
#include "gpibvisaexception.h"
#elif LINUXGPIB
#include "gpibbusexception.h"
#endif
#include "gpibidnexception.h"
#include "gpibinvalidreplyexception.h"
#include "gpibunxparamvalexception.h"

#endif // GPIBEXCEPTIONS_H
