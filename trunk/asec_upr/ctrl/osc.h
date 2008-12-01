#ifndef CTRL_OSC_H_
#define CTRL_OSC_H_

#include <gpib.h>
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QDebug>

class oscctrl: public GPIBctrl
{
public:
	oscctrl(QString GPIBID);
	~oscctrl();
	QByteArray readcurve();
	void wait(QString state);
	void setch1(double vol);
	double getch1();
};

#endif /*CTRL_OSC_H_*/
