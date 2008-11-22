#ifndef VIB_TEMPERATURE_H
#define VIB_TEMPERATURE_H

#include <QtGui/QWidget>
#include <QtDBus>
#include <QtGlobal>
#include <QGraphicsScene>
#include <QErrorMessage>
#include <math.h>
#include "ui_asec_temperature.h"

#include "ctrl/temp.h"
#include "settings.h"
#include "QTempTimeline.h"

class vib_temperature : public QWidget
{
    Q_OBJECT

public:
    QStringList result_var;
    bool can_return;
    vib_temperature(QWidget *parent = 0);
    ~vib_temperature();
    settings wset;
    Ui::vib_temperatureClass ui;
    QTempTimer temptl;
    QStringList result();

private slots:
    void on_btSettings_clicked();

public slots:
    void temp_set();
    void timedout();
    void stopped();
    void newpoint(float time, float temp, float setpoint);
    void set_temp(double temp, double ramp, double timeout);
signals:
	void finished(QStringList data);
};

class export_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.exports"))

        private:
            vib_temperature *t;
    double setpoint;

public:
    export_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
    {
    }

public slots:
    void set_temp(double temp, double ramp, double timeout)
    {
    	setpoint=temp;
    	t->set_temp(setpoint,ramp,timeout);
    }

    void set_temp_step(double step, double ramp, double timeout)
    {
    	setpoint+=step;
        t->set_temp(setpoint,ramp,timeout);
    }
};

class flow_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.flow"))

private:
	vib_temperature *t;
	double setpoint;

public:
	flow_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
    {
		setAutoRelaySignals(true);
    }

public slots:
    void stop()
    {
        t->temptl.stop();
    }
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
    help_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
    {

    }

public slots:
	//TODO: QString module_description()

	QStringList returned_values()
	{
		QStringList data;
		data<<QString("Setpoint,K");
		data<<QString("Temperature 1,K");
		data<<QString("Temperature 2,K");
		data<<QString("Ramp,K/min");
		data<<QString("Timeout,min");
		data<<QString("Stabilization time,min");
		return data;
	}

    QString set_temp()
    {
        return trUtf8("<p>Установить температуру. Устанавливается точка и ожидается установление температруры. </p> "
                  "<p><code>temp</code> - желаемое значение температуры, К</p> "
                  "<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
                  "при всех прочих значениях тока изменится скачком </p>"
                  "<p><code>timeout</code> - "
                  "максимальное время установлния температуры (в минутах). Если это время "
                  "будет превышено, выполнение программы прервется.</p>");
    }

    QString set_temp_step()
    {
        return trUtf8("<p>Изменить температуру на <b>step</b> градусов кельвина. Устанавливается точка "
                  "и ожидается установление температруры. </p> "
                  "<p><code>step</code> - шаг по температуре, К</p> "
                  "<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
                  "при всех прочих значениях тока изменится скачком </p>"
                  "<p><code>timeout</code> - максимальное время установлния температуры (в минутах). "
                  "Если это время будет превышено, выполнение программы прервется.</p>");
    }
};

#endif // VIB_TEMPERATURE_H
