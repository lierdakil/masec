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
#include "ui_asec_upr.h"
#include "measure.h"

class MeasureThread : public QThread
{
public:
	double startf;
	double stopf;
	QString filename;
	QGraphicsScene* scene;
	QString oscid;
	QString genid;
	QString mulid;
	QStringList result;
	void run();
};

class vibupraut : public QWidget
{
    Q_OBJECT

public:
    vibupraut(QWidget *parent = 0);
    ~vibupraut();

private:
    Ui::vibuprautClass ui;

public:
	QStringList measure(double startf, double stopf, QString filename);
	MeasureThread thread;
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
	QStringList do_measure(double startf, double stopf, QString filename)
	{
		return vua->measure(startf, stopf, filename);//TODO: change this behaviour?
	}

	void stop()
	{
		vua->thread.quit();
	}
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

	QString do_measure()
	{
		return QString::fromUtf8(
				"<p>ѕолучить данные о резонансе и антирезонансе. </p>"
				"<p><code>startf</code> - частота начала пробега в герцах</p>"
				"<p><code>stopf</code> - частота конца пробега в герцах</p>"
				"<p><code>filename</code> - им€ файла дл€ сохранени€ вида"
				"резонансной кривой (если пустое - не сохран€ть)</p>"
				);
	}
};

#endif // VIBUPRAUT_H
/* TODO: Ќадо разобратьс€, что работает в результате изменений, что не работает,
 * и что со всем этим делать...
 * ≈стественно, что, даже скомпилив это дома € не могу провер€ть работоспособность.
 */
