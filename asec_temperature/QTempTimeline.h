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
    //void wait(double min, const char *member);
    tempctrl *temp;
    bool is_stopped;
    float fstartclock;
    float ramptime;//minutes
    float testtime;//minutes
    int step_num;

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
    QVector<float> temps;
    //QVector<float> times;

public slots:
    void start_zone(float nsetp, float nramp, float ntimeout, float nsettime);
    void start_manual(float nsetp, float nramp, float ntimeout, float nsettime, float P, float I, float D, int range, double mout);
    void step();
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
