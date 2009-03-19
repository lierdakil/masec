#ifndef CTRL_GPIB_H_
#define CTRL_GPIB_H_

#include "visa.h"
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>
#include "sleep.h"
#define SLEEP_TIME 20

class GenericException
{
private:
    QString m;
public:
    GenericException()
    {
    }

    GenericException(QString message)
    {
        m=message;
    }

    ~GenericException()
    {
    }

    void setMessage(QString message)
    {
        m=message;
    }

    QString Report()
    {
        qDebug()<<m;
        return m;
    }
};

class VISAException : public GenericException
{
public:
    VISAException(ViStatus status, ViSession session)
    {
        char desc[1024];
        viStatusDesc(session,status,desc);
        setMessage(QString::fromUtf8("There was an error #%1 with message '%2'").arg(status).arg(QString::fromLocal8Bit(desc)));
    }

    ~VISAException()
    {
    }
};

class IDNException : public GenericException
{
public:
    IDNException(QString ID, QString _IDN, QString IDN)
            /* _IDN = real device string
           IDN = expected device string */
    {
        setMessage(QString::fromUtf8("Unexpected device IDN string at %1: '%2' (Expected '%3')").arg(ID,_IDN,IDN));
    }

    ~IDNException()
    {
    }
};

class GPIBctrl
{
private:
    ViSession defaultRM, did;
    QMutex mutex;//TODO: o_O When I make it static pointer, compilation fails
    //TODO: Make mutex at least process-wise... somehow...

public:
    GPIBctrl(QString GPIBID, QString IDN, int timeout=5000)
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

    ~GPIBctrl()
    {
        /*free GPIB resources*/
        viClose(did);
        viClose(defaultRM);
    }

    void write(QString string)
    {
        QMutexLocker m(&mutex);
        QString str=string.append("\n");
        msleep(SLEEP_TIME);
        ViStatus err=viWrite(did,ViBuf(str.toAscii().data()),str.length(), NULL);
        if(err<VI_SUCCESS)
            throw VISAException(err,did);
    }

    QString read()
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

    QString query(QString request)
    {
        read();
        write(request);
        return read();
    }

    QByteArray read_array(int maxlength)
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
};

#endif /*CTRL_GPIB_H_*/
