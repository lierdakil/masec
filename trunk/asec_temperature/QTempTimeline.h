#ifndef QTEMPTIMELINE_H_
#define QTEMPTIMELINE_H_

#include <QTimeLine>
#include <QtDBus>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtAlgorithms>
#include <QGraphicsView>
#include "ctrl/temp.h"
#include <math.h>

class QTempTimeline: public QTimeLine
{
	Q_OBJECT
	
private:
	tempctrl *temp;
	float temp1,temp2,setp, h, setp1, setp2;
	int t0,dt;
	bool first;
	double waittime;
	double waiting;
	QGraphicsScene *scene;
	
public slots:
	void vC(qreal value);
	void finish();
	void start(tempctrl *temp, float newtemp, int timeout, double settime, QGraphicsView *view);
	
signals:
	void reached(float temp1,float temp2,float setp);
	
public:
	QTempTimeline();
	~QTempTimeline();
};

#endif /*QTEMPTIMELINE_H_*/
