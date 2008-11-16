#ifndef QTEMPTIMELINE_H_
#define QTEMPTIMELINE_H_

#include <QTimer>
#include <QtDBus>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtAlgorithms>
#include <QGraphicsView>
#include "ctrl/temp.h"
#include <math.h>

#define TIMESTEP 0.001

class QTempTimer : public QObject
{
	Q_OBJECT

private:
	tempctrl *temp;
	double dt; //minutes
	double time,drawtime;//minutes
	void wait(double min, const char *member);

public:
	//"input"
	float setp;
	float ramp;
	float timeout;
	double settime;
	//return vals
	float temp1,temp2;

public slots:
	bool stable();//we use this function to check if temperature is stable at the moment
	void start(QString tempid, float nsetp, float nramp, float ntimeout, float nsettime);//start temperature setting
	void draw_temp();//emit newpoint in separate event thread
	void rampdone();//first we wait until ramp is done
	void step1();//now we check every TIMESTEP while temperature stabilizes - main check cycle, should include emit timedout() here
	void step2();//then we wait $settime and check if it really stabilized
	void stop();//make it think it timed out

signals:
	void finished();//temperature stabilized at given setpoint
	void timedout();//temperature could not stabilize in $timeout minutes
	void newpoint(float time, float temp, float setpoint);

public:
	QTempTimer();
	~QTempTimer();
};

#endif /*QTEMPTIMELINE_H_*/
