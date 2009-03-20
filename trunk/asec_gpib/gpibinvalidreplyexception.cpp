#include "gpibinvalidreplyexception.h"

GPIBInvalidReplyException::GPIBInvalidReplyException(QString request, QString reply, QString expected)
{
    setMessage(QString::fromUtf8("Unexpected reply to request '%1' received: '%2' (expected '%3')").arg(request,reply,expected));
}
