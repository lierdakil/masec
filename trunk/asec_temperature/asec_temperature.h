#ifndef VIB_TEMPERATURE_H
#define VIB_TEMPERATURE_H

#include <QtGui/QWidget>
#include <QtDBus>
#include <QTimeLine>
#include <QtGlobal>
#include "ui_asec_temperature.h"
#include "settings.h"
#include "QTempTimeline.h"
#include "ctrl/temp.h"
#include <math.h>

class vib_temperature : public QWidget
{
    Q_OBJECT

public:
    QStringList result_var;
    vib_temperature(QWidget *parent = 0);
    ~vib_temperature();
    settings wset;
    Ui::vib_temperatureClass ui;
    QTempTimer temptl;
    QStringList result();

private slots:
    void on_btSettings_clicked();

public slots:
    void cont(float temp1, float temp2, float setp);
    QStringList set_temp(double temp, double ramp, double timeout);
};

class export_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.export"))

        private:
            vib_temperature *t;
    double setpoint;

public:
    export_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
    {
    }

public slots:
    QStringList set_temp(double temp, double ramp, double timeout)
    {
    	setpoint=temp;
    	return t->set_temp(setpoint,ramp,timeout);
    }

    QStringList step_temp(double step, double ramp, double timeout)
    {
    	setpoint+=step;
        return t->set_temp(setpoint,ramp,timeout);
    }

    void stop()
    {
        t->temptl.stop();
    }
};

class help_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.help"))

        private:
            vib_temperature *t;

public:
    help_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
    {

    }

public slots:
    QString set_temp()
    {
        return trUtf8("<p>Установить температуру. Устанавливается точка и ожидается установление температруры. </p> "
                  "<p><code>temp</code> - желаемое значение температуры, К</p> "
                  "<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
                  "при всех прочих значениях тока изменится скачком </p>"
                  "<p><code>timeout</code> - "
                  "максимальное время установлния температуры (в минутах). Если это время "
                  "будет превышено, выполнение программы прервется. На данный момент в таком случае "
                  "необходим перезапуск системы.</p>");
    }

    QString step_temp()
    {
        return trUtf8("<p>Изменить температуру на <b>step</b> градусов кельвина. Устанавливается точка "
                  "и ожидается установление температруры. </p> "
                  "<p><code>step</code> - шаг по температуре, К</p> "
                  "<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
                  "при всех прочих значениях тока изменится скачком </p>"
                  "<p><code>timeout</code> - максимальное время установлния температуры (в минутах). "
                  "Если это время будет превышено, выполнение программы прервется. На данный момент "
                  "в таком случае необходим перезапуск системы.</p>");
    }
};

#endif // VIB_TEMPERATURE_H
