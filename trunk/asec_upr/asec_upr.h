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
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
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
        void path(QVector<qreal>,QVector<qreal>,QPen pen);
        void marker(qreal x, qreal y, QPen pen);
        void on_btRun_clicked();
        void onfinished(QStringList data);
signals:
        void finished(QStringList data);
};

#endif // VIBUPRAUT_H
