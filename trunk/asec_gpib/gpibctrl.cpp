#include "gpibctrl.h"
#include "gpibexceptions.h"


GPIBctrl::GPIBctrl(QString GPIBID, QString IDN, int timeout)
{
    /*initialize GPIB control*/
    ViStatus err;
    err=viOpenDefaultRM(&defaultRM);
    if (err<VI_SUCCESS)
    {
        throw GenericException("There was an error opening default RM session");
    }

    err=viOpen(defaultRM, GPIBID.replace(",","::").append("::INSTR").toAscii().data(), VI_EXCLUSIVE_LOCK, 0, &did);//TODO: timeout?
    if (err<VI_SUCCESS)
    {
        VISAException e(err,defaultRM);
        viClose(defaultRM);
        throw e;
    }

    err=viClear(did);
    if (err<VI_SUCCESS)
    {
        VISAException e(err,did);
        viClose(did);
        viClose(defaultRM);
        throw e;
    }

    err=viSetAttribute(did, VI_ATTR_TMO_VALUE, timeout);
    if (err<VI_SUCCESS)
    {
        VISAException e(err,did);
        viClose(did);
        viClose(defaultRM);
        throw e;
    }

    try{
        write("*CLS");
    } catch (VISAException e) {
        viClose(did);
        viClose(defaultRM);
        throw e;//rethrow
    }

    QString _IDN;
    try {
        _IDN=query("*IDN?");
    } catch (VISAException e) {
        viClose(did);
        viClose(defaultRM);
        throw e;//rethrow
    }

    if(_IDN != IDN)
    {
        viClose(did);
        viClose(defaultRM);
        throw IDNException(GPIBID, _IDN, IDN);
    }
}

GPIBctrl::~GPIBctrl()
{
    /*free GPIB resources*/
    viClose(did);
    viClose(defaultRM);
}

void GPIBctrl::write(QString string)
{
    QMutexLocker m(&mutex);
    QString str=string.append("\n");
    msleep(SLEEP_TIME);
    ViStatus err=viWrite(did,ViBuf(str.toAscii().data()),str.length(), NULL);
    if(err<VI_SUCCESS)
        throw VISAException(err,did);
}

QString GPIBctrl::read()
{
    QMutexLocker m(&mutex);
    msleep(SLEEP_TIME);//If we read too fast, sometimes devices fail to answer in time

    char strres [256];
    unsigned long actual=0;
    ViStatus err=viRead(did,ViBuf(strres),256,&actual);

    if(err<VI_SUCCESS)
        throw VISAException(err,did);

    strres[actual]=0;

    QString res=QString::fromAscii(strres,actual-1);//actual-1 because we don't want terminator.
    return QString(res);
}

QString GPIBctrl::query(QString request)
{
    write(request);
    return read();
}

QByteArray GPIBctrl::read_array(int maxlength)
{
    QMutexLocker m(&mutex);
    QByteArray res;
    res.resize(maxlength);
    unsigned long len;
    msleep(SLEEP_TIME);
    ViStatus err = viRead(did,ViBuf(res.data()),maxlength,&len);
    if(err<VI_SUCCESS)
        throw VISAException(err,did);
    res.resize(len);
    return res;
}
