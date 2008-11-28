#ifndef VIBUPRAUT_H
#define VIBUPRAUT_H

#include <QtCore/QStringList>
#include <QtDBus>
#include <QtGui/QWidget>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QtCore>
#include <QErrorMessage>
#include "ui_asec_upr.h"
#include "measurethread.h"

class vibupraut : public QWidget
{
	Q_OBJECT

public:
	vibupraut(QWidget *parent = 0);
	~vibupraut();

private:
	Ui::vibuprautClass ui;
	qreal current_x;
	qreal current_y;

public:
	void measure(double startf, double stopf, QString filename);
	MeasureThread thread;

public slots:
	void path(QList<qreal>,QPen pen);
	void path(QByteArray,QPen pen);
	void line(qreal x1, qreal y1,qreal x2, qreal y2, QPen pen);
signals:
	void finished(QStringList data);
};

class export_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.exports"))

private:
	vibupraut *vua;

public:
	export_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
	{

	}

public slots:

	void mes_res(double startf, double stopf)
	{
		vua->measure(startf, stopf, QString());
	}

	void mes_res_file(double startf, double stopf, QString filename)
	{
		vua->measure(startf, stopf, filename);
	}
};

class flow_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.flow"))

private:
	vibupraut *vua;

public:
	flow_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
	{
		setAutoRelaySignals(true);
		//or
		//connect(v,SIGNAL(finished(QStringList)),this,SIGNAL(finished(QStringList)));
	}

public slots:
	void stop()
	{
		vua->thread.quit();
	}
signals:
	void finished(QStringList data);
};

class help_adaptor : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.help"))

private:
	vibupraut *vua;

public:
	help_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
	{

	}

public slots:

//TODO: QString module_description()

	QStringList returned_values()
	{
		QStringList data;
		data<<QString("First run start freq, Hz");
		data<<QString("First run stop freq, Hz");
		data<<QString("Second run start freq, Hz");
		data<<QString("Second run stop freq, Hz");
		data<<QString("Resonance freq, Hz");
		data<<QString("Resonance ampl, V");
		data<<QString("Antiresonance freq, Hz");
		data<<QString("Antiresonance ampl, V");
		return data;
	}

	QString mes_res()
	{
		return trUtf8(
				"<p>Получить данные о резонансе и антирезонансе. </p>"
				"<p><code>startf</code> - частота начала пробега в герцах</p>"
				"<p><code>stopf</code> - частота конца пробега в герцах</p>"
		);
	}

	QString mes_res_file()
	{
		return trUtf8(
				"<p>Получить данные о резонансе и антирезонансе. </p>"
				"<p><code>startf</code> - частота начала пробега в герцах</p>"
				"<p><code>stopf</code> - частота конца пробега в герцах</p>"
				"<p><code>filename</code> - имя файла для сохранения вида"
				"резонансной кривой (если пустое - не сохранять)</p>"
		);
	}
};

#endif // VIBUPRAUT_H
