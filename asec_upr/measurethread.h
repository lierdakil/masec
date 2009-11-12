/*
 * measurethread.h
 *
 *  Created on: 18.11.2008
 *      Author: kazei
 */

#ifndef MEASURETHREAD_H_
#define MEASURETHREAD_H_

#include <QtCore>
#include <QPen>
#include <QGraphicsScene>
#include <QErrorMessage>
#include "ctrl/gen.h"
#include "ctrl/vol.h"
#include "ctrl/osc.h"

class MeasureThread : public QThread
{
    Q_OBJECT

private:
    bool stop_scheldued;
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
    double fsf;//First Run Start Frequency
    double fff;//First Run Stop Frequency
    double ssf;//Second Run Start Frequency
    double sff;//Second Run Stop Frequency
    double rf; //Resonance Frequency
    double ra; //Resonance Amplitude
    double af; //Antiresonance Frequency
    double aa; //Antiresonance Amplitude
    //QList<QPair<double,double> > curve_forward, curve_reverse, curve_wide;
    QVector<qreal> curve_forward_x, curve_reverse_x, curve_wide_x,
    curve_forward_y, curve_reverse_y, curve_wide_y;
    double find_extremum(QByteArray dat, int start, int stop, int sm, bool max);

public:
    double startf;
    double stopf;
    QString filename;
    QString oscid;
    QString genid;
    QString mulid;
    double volts1;
    int sm1, sm2;
    double genvolpp;
    void run();
    void stop();

signals:
    /*void path(QList<qreal>,QPen pen);
    void path(QByteArray,QPen pen);
    void line(qreal x1, qreal y1,qreal x2, qreal y2, QPen pen);*/
    void path(QVector<qreal>,QVector<qreal>,QPen pen);
    void marker(qreal x, qreal y, QPen pen);
    void finished(QStringList data);
};

#endif /* MEASURETHREAD_H_ */
