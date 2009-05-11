#include "gpibctrl.h"
#include "sleep.h"
#define SLEEP_TIME 20

#ifdef VISA
GPIBctrl::GPIBctrl(QString GPIBID, QString IDN, int timeout)
{
    /*initialize GPIB control*/
    ViStatus err;
    err=viOpenDefaultRM(&defaultRM);
    if (err<VI_SUCCESS)
    {
        throw GPIBGenericException("There was an error opening default RM session");
    }

    err=viOpen(defaultRM, GPIBID.replace(",","::").append("::INSTR").toAscii().data(), VI_EXCLUSIVE_LOCK, 0, &did);
    if (err<VI_SUCCESS)
    {
        GPIBVISAException e(err,defaultRM);
        viClose(defaultRM);
        throw e;
    }

    err=viClear(did);
    if (err<VI_SUCCESS)
    {
        GPIBVISAException e(err,did);
        viClose(did);
        viClose(defaultRM);
        throw e;
    }

    err=viSetAttribute(did, VI_ATTR_TMO_VALUE, timeout);
    if (err<VI_SUCCESS)
    {
        GPIBVISAException e(err,did);
        viClose(did);
        viClose(defaultRM);
        throw e;
    }

    try{
        write("*CLS");
    } catch (GPIBVISAException e) {
        viClose(did);
        viClose(defaultRM);
        throw e;//rethrow
    }

    QString _IDN;
    try {
        _IDN=queryString("*IDN?");
    } catch (GPIBVISAException e) {
        viClose(did);
        viClose(defaultRM);
        throw e;//rethrow
    }

    if(_IDN != IDN)
    {
        viClose(did);
        viClose(defaultRM);
        throw GPIBIDNException(GPIBID, _IDN, IDN);
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
        throw GPIBVISAException(err,did);
}

QByteArray GPIBctrl::readArray(int maxlength)
{
    QMutexLocker m(&mutex);
    msleep(SLEEP_TIME);//If we read too fast, sometimes devices fail to answer in time

    QByteArray res;
    res.resize(maxlength);

    ViUInt32 len;
    ViStatus err = viRead(did,ViBuf(res.data()),maxlength,&len);

    if(err<VI_SUCCESS)
        throw GPIBVISAException(err,did);

    res.resize(len);
    return res;
}

#elif LINUXGPIB
GPIBctrl::GPIBctrl(QString GPIBID, QString IDN, int timeout)
{
}
GPIBctrl::~GPIBctrl()
{
}
void GPIBctrl::write(QString string)
{
}
QByteArray GPIBctrl::readArray(int maxlength)
{
}
#endif

//platform-independant functions follow

QString GPIBctrl::readString()
{
    QString res;
    QByteArray buf;
    do{
        buf = readArray(256);
        res.append(buf);
    } while(buf.count()==256 && buf[buf.length()-1]!='\n');

    //We do not want terminator
    res.chop(1);

    return res;
}

QString GPIBctrl::queryString(QString request, QString preamble)
{
    write(request);
    QString reply=readString();
    if(!preamble.isEmpty())
    {
        if(!reply.startsWith(preamble))
            throw GPIBInvalidReplyException(request,reply,QString("Preamble %1").arg(preamble));

        reply.remove(0,preamble.length());
        reply=reply.trimmed();
    }
    return reply;
}

float GPIBctrl::queryFloat(QString request, QString preamble)
{
    QString reply = queryString(request, preamble);
    bool ok;
    float res = reply.toFloat(&ok);
    if(!ok)
        throw GPIBInvalidReplyException(request,reply,"Float");
    return res;
}

int GPIBctrl::queryInt(QString request, QString preamble)
{
    QString reply = queryString(request, preamble);
    bool ok;
    int res = reply.toInt(&ok);
    if(!ok)
        throw GPIBInvalidReplyException(request,reply,"Int");
    return res;
}

QStringList GPIBctrl::queryStringList(QString request, QChar separator, int count, QString preamble)
{
    QString reply = queryString(request, preamble);
    QStringList list = reply.split(separator);

    if(count>0)
        if(count!=list.count())
            throw GPIBInvalidReplyException(request,reply,QString("List of %2 items with \"%1\" separator").arg(separator).arg(count));

    return list;
}


QList<float> GPIBctrl::queryFloatList(QString request, QChar separator, int count, QString preamble)
{
    QStringList list = queryStringList(request, separator, count, preamble);
    QList<float> res;

    foreach(QString sitem, list)
    {
        bool ok;
        float fitem = sitem.toFloat(&ok);
        if(!ok)
            throw GPIBInvalidReplyException(request,list.join(separator),QString("List of float with \"%1\" separator").arg(separator));
        res<<fitem;
    }

    return res;
}

QList<int> GPIBctrl::queryIntList(QString request, QChar separator, int count, QString preamble)
{
    QStringList list = queryStringList(request, separator, count, preamble);
    QList<int> res;

    foreach(QString sitem, list)
    {
        bool ok;
        int iitem = sitem.toInt(&ok);
        if(!ok)
            throw GPIBInvalidReplyException(request,list.join(separator),QString("List of float with \"%1\" separator").arg(separator));
        res<<iitem;
    }

    return res;
}

QVariantList GPIBctrl::queryVariantList(QString request, QChar separator, QString fmt, QString preamble)
        //fmt allowed: int,float,str
{
    QStringList fmtlist = fmt.split(separator);
    QStringList list = queryStringList(request, separator, fmtlist.count(), preamble);
    QVariantList res;

    for(int i=0; i<list.count(); ++i)
    {
        if(fmtlist.at(i)=="int") {
            bool ok;
            int item = list.at(i).toInt(&ok);
            if(!ok)
                throw GPIBInvalidReplyException(request,list.join(separator),QString("Variant list of '%1' format").arg(fmt));
            res<<item;
        } else if(fmtlist.at(i)=="float") {
            bool ok;
            float item = list.at(i).toFloat(&ok);
            if(!ok)
                throw GPIBInvalidReplyException(request,list.join(separator),QString("Variant list of '%1' format").arg(fmt));
            res<<item;
        } else if(fmtlist.at(i)=="str") {
            res<<list.at(i);
        } else
            throw GPIBGenericException(QString("Unrecognized format in queryVariantList: '%1'").arg(fmtlist.at(i)));
    }

    return res;
}
