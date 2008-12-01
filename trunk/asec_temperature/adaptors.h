/*
 * adaptors.h
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include "asec_temperature.h"
#include <QtDBus>

class export_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.exports"))

private:
	vib_temperature *t;
	double setpoint;

public:
    export_adaptor(vib_temperature *parent);

public slots:
    void set_temp(double temp, double ramp, double timeout);
    void set_temp_step(double step, double ramp, double timeout);
};

class flow_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.flow"))
private:
	vib_temperature *t;
	double setpoint;
public:
	flow_adaptor(vib_temperature *parent);
public slots:
    void stop();
signals:
	void finished(QStringList data);
};

class help_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.help"))

private:
	vib_temperature *t;
public:
    help_adaptor(vib_temperature *parent);
public slots:
	//TODO: QString module_description();
	QStringList returned_values();
    QString set_temp();
    QString set_temp_step();
};

#endif /* ADAPTORS_H_ */
