#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtGui/QWidget>
#include <QDebug>
#include <QErrorMessage>
#include <QtDBus>
#include <QSettings>
#include <QGraphicsScene>
#include <QPainterPath>
#include "ui_settings.h"
#include "ctrl/temp.h"
#include "QTempTimeline.h"

class settings : public QDialog
{
    Q_OBJECT

public:
    settings(QWidget *parent = 0);
    ~settings();
    void read_settings();
    double getSettime(double temp);
    QString tempid;

public slots:
	void finished();
	void newpoint(float time, float temp, float setpoint);

private:
    Ui::Settings ui_settings;
	QTempTimer test;
	QPainterPath *pp_temp;
	QPainterPath *pp_setp;

private slots:
	void accept();
	void on_btTest_clicked();
	void on_btStopTest_clicked();
};

#endif /*SETTINGS_H_*/
