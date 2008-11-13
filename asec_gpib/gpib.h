#ifndef CTRL_GPIB_H_
#define CTRL_GPIB_H_

#include "sicl.h"
#include <QString>
#include <QMutex>
#include <QMutexLocker>

//TODO: We need mutex in each function, maybe better in GPIBctrl, by GPIB ID.

class GPIBctrl
{
private:
	int did;
	QMutex mutex;

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
		iprintf(did,string.append("\n").toAscii().data());
	}

	QString read()
	{
		QMutexLocker m(&mutex);
		char reply[256];
		iscanf(did,"%s",reply);
		return QString(reply);
	}

	QByteArray read_array(int *length)
	{
		QMutexLocker m(&mutex);
		char *res = new char[*length];
		long unsigned int len;
		iread(did,(char*)res,*length,0,&len);
		*length=len;
		QByteArray res2(res);
		delete[] res;
		return res2;
	}

	QString query(QString request)
	{
		write(request);
		return read();
	}
};

#endif /*CTRL_GPIB_H_*/
