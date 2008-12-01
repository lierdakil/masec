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

#endif // VIBUPRAUT_H
