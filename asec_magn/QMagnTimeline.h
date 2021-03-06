/*
 * QMagnTimeline.h
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */

#ifndef QMAGNTIMELINE_H_
#define QMAGNTIMELINE_H_

#include <QTimer>
#include <QtDBus>
#include <QtCore>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtAlgorithms>
#include <QGraphicsView>
#include <math.h>
#include "ctrl/magn.h"

class QMagnTimeline : public QObject
{
    Q_OBJECT

public:
    QMagnTimeline();
    ~QMagnTimeline();

private:
    double time,drawtime;//minutes
    int startclock;
    bool is_running;
    bool stop_requested;
    magnctrl *magn;

    void wait(double sec, const char* member);
    void raiseError(QString message);

public slots:
    void start(float field);//start field setting
    void draw_field();//emit newpoint in separate event thread
    void check_quench();//check if magnet quenched
    void rampdone();//first we wait until ramp is done
    void checkcurr();//now we check every TIMESTEP while current is at requested level
    //if it is, return field_set
    void stop();//stop
    void ID_changed();

signals:
    void field_set(float reqfield, float field, float settime/*minutes*/);//field reached requested value
    void stopped();//stopped by user
    void quench();//achtung! quench detected!
    void error(QString message);//An ordianry error
    void newpoint(float time, float field);//used to draw points
};

#endif /* QMAGNTIMELINE_H_ */
