#ifndef CTRL_GPIB_H_
#define CTRL_GPIB_H_

#include "sicl.h"
#include <QString>
#include <QMutex>
#include <QMutexLocker>

class GPIBctrl
{
private:
	int did;
	QMutex mutex;//TODO: Better to be GPIBID-wise, than instance-wise.

public:
	GPIBctrl(QString GPIBID)
	{
		/*initialize GPIB control*/
		ionerror(I_ERROR_NO_EXIT);

		did = iopen(GPIBID.toAscii().data());
		itimeout(did,600000);
	}

	~GPIBctrl()
	{
		/*free GPIB resources*/
		iclose(did);
	}

	void write(QString string)
	{
		QMutexLocker m(&mutex);
                //iprintf(did,string.append("\n").toAscii().data());
                QString str=string.append("\n");
                iwrite(did,str.toAscii().data(),str.length(),1,NULL);
	}

	QString read()
	{
		QMutexLocker m(&mutex);
		QByteArray r;
		r.resize(256);
                //iscanf(did,"%S",r.data());
                long unsigned int actual=0;
                iread(did,r.data(),256,NULL,&actual);
                r.resize(actual-1);
		QString reply(r); //TODO: Better return QString(r)?
                //reply.remove("\n");//last character is \n
		return reply;
	}

	QString query(QString request)
	{
		write(request);//TODO: ipromptf?
		return read();
	}

	QByteArray read_array(int maxlength)
	{
		QMutexLocker m(&mutex);
		QByteArray res;
		res.resize(maxlength);
		long unsigned int len;
		iread(did,res.data(),maxlength,0,&len);
		res.resize(len);
		return res;
	}
};

#endif /*CTRL_GPIB_H_*/
