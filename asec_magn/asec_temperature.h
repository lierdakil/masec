#ifndef VIB_TEMPERATURE_H
#define VIB_TEMPERATURE_H

#include <QtGui/QWidget>
#include <QtDBus>
#include <QtGlobal>
#include <QGraphicsScene>
#include <QErrorMessage>
#include <math.h>
#include "ui_asec_temperature.h"

#include "ctrl/temp.h"
#include "settings.h"
#include "QTempTimeline.h"

class vib_temperature : public QWidget
{
    Q_OBJECT

public:
    QStringList result_var;
    bool can_return;
    vib_temperature(QWidget *parent = 0);
    ~vib_temperature();
    settings wset;
    Ui::vib_temperatureClass ui;
    QTempTimer temptl;
    QStringList result();
private:
	QTimer fix_timer;

private slots:
    void on_btSettings_clicked();

public slots:
    void temp_set();
    void timedout();
    void stopped();
    void fix_range();
    void newpoint(float time, float temp, float setpoint);
    void set_temp(double temp, double ramp, double timeout);
signals:
	void finished(QStringList data);
};

#endif // VIB_TEMPERATURE_H
