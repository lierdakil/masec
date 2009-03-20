#include "gpibvisaexception.h"

GPIBVISAException::GPIBVISAException(ViStatus status, ViSession session)
{
    char desc[1024];
    viStatusDesc(session,status,desc);
    setMessage(QString::fromUtf8("There was an error #%1 with message '%2'").arg(status).arg(QString::fromLocal8Bit(desc)));
}
