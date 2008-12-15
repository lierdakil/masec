#ifndef MEASURE_H_
#define MEASURE_H_

#include <QtGui/QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QtDBus>
#include <QList>
#include <QPair>

#include "ctrl/gen.h"
#include "ctrl/osc.h"
#include "ctrl/vol.h"
#include <stdlib.h>

class cmeasure : public QObject
{
	Q_OBJECT

private:
	double epsilon;
	double k, k2;
	void findresonance();
	QByteArray sweep();
	float getamplonf(float freq);
	float golden(float a, float b, float epsilon, bool max);
	genctrl* gen;
	volctrl* vol;
	oscctrl* osc;
	QString oscstr;
	double volts1;
	int sm1,sm2;

public:
	cmeasure(QString oscstr, QString genstr, QString volstr, double sf, double ff, double epsilon/*Hz*/, double volts1=0.1, int sm1=12, int sm2=24, QObject *parent=0);
	~cmeasure();
	double fsf;//First Run Start Frequency
	double fff;//First Run Stop Frequency
	double ssf;//Second Run Start Frequency
	double sff;//Second Run Stop Frequency
	double rf; //Resonance Frequency
	double ra; //Resonance Amplitude
	double af; //Antiresonance Frequency
	double aa; //Antiresonance Amplitude
	bool error;
	QString err_mesg;
	QList<QPair<double,double> > curve;

signals:
	void path(QList<qreal>,QPen pen);
	void path(QByteArray,QPen pen);
	void line(qreal x1, qreal y1,qreal x2, qreal y2, QPen pen);
};


#endif /*MEASURE_H_*/
