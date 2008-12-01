#ifndef CTRL_GPIB_H_
#define CTRL_GPIB_H_

#include "sicl.h"
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "sleep.h"
#define SLEEP_TIME 20

class GPIBctrl
{
private:
	int did;
	QMutex mutex;//TODO: o_O When I make it static pointer, compilation fails
	//TODO: Make mutex at least process-wise... somehow...

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
		msleep(SLEEP_TIME);
		iwrite(did,str.toAscii().data(),str.length(),1,NULL);
	}

	QString read()
	{
		QMutexLocker m(&mutex);
		QByteArray r;
		r.resize(256);
		long unsigned int actual=0;
		msleep(SLEEP_TIME);
		while(actual<=1)//there's always at least \n
		//TODO: timeout
			iread(did,r.data(),256,NULL,&actual);
		r.resize(actual-1);
		return QString(r);
	}

	QString query(QString request)
	{
		write(request);
		return read();
	}

	QByteArray read_array(int maxlength)
	{
		QMutexLocker m(&mutex);
		QByteArray res;
		res.resize(maxlength);
		long unsigned int len;
		msleep(SLEEP_TIME);
		iread(did,res.data(),maxlength,0,&len);
		res.resize(len);
		return res;
	}
};

#endif /*CTRL_GPIB_H_*/
