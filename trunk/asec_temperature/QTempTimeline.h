#ifndef QTEMPTIMELINE_H_
#define QTEMPTIMELINE_H_

#include <QTimer>
#include <QtDBus>
#include <QtCore>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtAlgorithms>
#include <QGraphicsView>
#include <math.h>
#include <gpibexceptions.h>
#include "ctrl/temp.h"

class QTempTimer : public QObject
{
	Q_OBJECT

private:
	double dt; //minutes
	double time,drawtime;//minutes
	void wait(double min, const char *member);
        tempctrl *temp;
        bool is_stopped;
        bool stop_requested;

private slots:
        void start(float nsetp, float nramp, float ntimeout, float nsettime);//start temperature setting
        void raiseError(QString message); //stops timeline and raises error via error(QString) signal
        bool stable();//we use this function to check if temperature is stable at the moment

public:
	//"input"
	float setp;
	float ramp;
	float timeout;
	double settime;
	//return vals
	int startclock;
	float temp1,temp2;

public slots:
        void start_zone(float nsetp, float nramp, float ntimeout, float nsettime);
        void start_manual(float nsetp, float nramp, float ntimeout, float nsettime, float P, float I, float D, int range, double mout);
	void draw_temp();//emit newpoint in separate event thread
	void rampdone();//first we wait until ramp is done
	void step1();//now we check every TIMESTEP while temperature stabilizes - main check cycle, should include emit timedout() here
	void step2();//then we wait $settime and check if it really stabilized
	void stop();//stop

signals:
	void temp_set();//temperature stabilized at given setpoint
	void stopped();//stopped by user
        void error(QString message);//error occured
	void newpoint(float time, float temp, float setpoint);

public:
	QTempTimer();
	~QTempTimer();
};

#endif /*QTEMPTIMELINE_H_*/
