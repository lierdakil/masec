#ifndef MEASURE_H_
#define MEASURE_H_

#include <QtGui/QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QtDBus>
#include <QList>
#include <QPair>

//TODO: ѕереписать под Qt все ctrl_, использу€ ћ»Ќ»ћ”ћ возможностей sicl.
#include "ctrl/gen.h"
#include "ctrl/osc.h"
#include "ctrl/vol.h"
#include <stdlib.h>

class cmeasure
{
public:
	cmeasure(QString oscstr, QString genstr, QString volstr, float sf, float ff, float epsilon=1/*Hz*/,QGraphicsScene *scene=0);
	~cmeasure();
	double fsf;//First Run Start Frequency
	double fff;//First Run Stop Frequency
	double ssf;//Second Run Start Frequency
	double sff;//Second Run Stop Frequency
	double rf; //Resonance Frequency
	double ra; //Resonance Amplitude
	double af; //Antiresonance Frequency
	double aa; //Antiresonance Amplitude
	QList<QPair<double,double> > curve; //TODO: Make it more like it should be
private:
	double epsilon;
	QGraphicsScene *scene;
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
