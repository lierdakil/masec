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

public:
	cmeasure(QString oscstr, QString genstr, QString volstr, double sf, double ff, double epsilon=1/*Hz*/,QGraphicsView *view=0);
	~cmeasure();
	double fsf;//First Run Start Frequency
	double fff;//First Run Stop Frequency
	double ssf;//Second Run Start Frequency
	double sff;//Second Run Stop Frequency
	double rf; //Resonance Frequency
	double ra; //Resonance Amplitude
	double af; //Antiresonance Frequency
	double aa; //Antiresonance Amplitude
	QList<QPair<double,double> > curve;
private:
	double epsilon;
	QGraphicsView *view;
	double k, k2;
	void findresonance();
	QByteArray sweep();
	float getamplonf(float freq);
	float golden(float a, float b, float epsilon, bool max);
	genctrl* gen;
	volctrl* vol;
	oscctrl* osc;
	QString oscstr;
};


#endif /*MEASURE_H_*/
