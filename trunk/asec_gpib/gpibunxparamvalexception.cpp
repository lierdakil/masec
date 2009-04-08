#include "gpibunxparamvalexception.h"

GPIBUnxParamValException::GPIBUnxParamValException(QString request, QString value, QString expected)
{
    setMessage(QString::fromUtf8("Unexpected parameter value in %1: '%2' (expected '%3')").arg(request,value,expected));
}
