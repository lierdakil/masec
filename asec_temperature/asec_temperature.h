#ifndef VIB_TEMPERATURE_H
#define VIB_TEMPERATURE_H

#include <QtGui/QWidget>
#include <QtDBus>
#include <QTimeLine>
#include <QtGlobal>
#include "ui_vib_temperature.h"
#include "settings.h"
#include "QTempTimeline.h"
#include <math.h>

class vib_temperature : public QWidget
{
    Q_OBJECT

public:
    vib_temperature(QWidget *parent = 0);
    ~vib_temperature();
    settings wset;
    Ui::vib_temperatureClass ui;
    QTempTimeline temptl;

private slots:
    void on_btSettings_clicked();
    
public slots:
    void cont(float temp1, float temp2, float setp);
};

#define SERVICE "ru.pp.livid.vib.temp"
#define OBJECT "/Main"

class vt_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.iface"))
	
private:
	vib_temperature *t;
	
public:
	vt_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
	{
    	QDBusInterface iface("ru.pp.livid.vib.con","/Main","ru.pp.livid.iface");
//		QStringList list;
//		list<<"set_temp double,double,int";
//		list<<"step_temp double,double,int";
//    	iface.call("announce",SERVICE,OBJECT,"ru.pp.livid.iface",list);
//    	iface.call("reg_func",SERVICE,OBJECT,"ru.pp.livid.iface","do_measure","startf,stopf,filename");
    	iface.call("reg_func",SERVICE,OBJECT,"ru.pp.livid.iface","set_temp","temp,ramp,timeout",
    			QString::fromUtf8("<p><code>void set_temp(double temp, double ramp, double timeout)</code></p>"
    					"<p>Установить температуру. Устанавливается точка и ожидается установление температруры. </p> "
    					"<p><code>temp</code> - желаемое значение температуры, К</p> "
    					"<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
    					"при всех прочих значениях тока изменится скачком </p>"
    					"<p><code>timeout</code> - "
    					"максимальное время установлния температуры (в минутах). Если это время "
    					"будет превышено, выполнение программы прервется. На данный момент в таком случае "
    					"необходим перезапуск системы.</p>"));
    	iface.call("reg_func",SERVICE,OBJECT,"ru.pp.livid.iface","step_temp","step,ramp,timeout",
    			QString::fromUtf8("<p><code>void step_temp(double step, double ramp, double timeout)</code></p> "
    					"<p>Изменить температуру на <b>step</b> градусов кельвина. Устанавливается точка "
    					"и ожидается установление температруры. </p> "
    					"<p><code>step</code> - шаг по температуре, К</p> "
    					"<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
    					"при всех прочих значениях тока изменится скачком </p>"
    					"<p><code>timeout</code> - максимальное время установлния температуры (в минутах). "
    					"Если это время будет превышено, выполнение программы прервется. На данный момент "
    					"в таком случае необходим перезапуск системы.</p>"));
    	iface.call("reg_stop",SERVICE,OBJECT,"ru.pp.livid.iface");
	}

public slots:
	void set_temp(double temp, double ramp, double timeout)
	{
		if (!t->wset.tempid.isEmpty())
		{
			tempctrl *tempc=new tempctrl((char*)t->wset.tempid.toAscii().data());
		
			tempc->ramp(ramp);
			t->temptl.start(tempc, temp, (int)ceil(timeout*60000), t->wset.getSettime(temp), t->ui.gvTemp);
		}
	}
	
	void step_temp(double step, double ramp, double timeout)
	{
		if (t->wset.tempid.isEmpty())
		{
			tempctrl *tempc=new tempctrl((char*)t->wset.tempid.toAscii().data());

			tempc->ramp(ramp);
			double temp=tempc->getsetp()+step;
			t->temptl.start(tempc, temp, (int)ceil(timeout*60000), t->wset.getSettime(temp), t->ui.gvTemp);
			//t->waittemp(tempc, temp,timeout);
		}
	}
	
	void stop()
	{
		t->temptl.stop();
	}
};

#endif // VIB_TEMPERATURE_H
