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
#include "QTempTimeline.h"

class settings : public QDialog
{
    Q_OBJECT

public:
    settings(QWidget *parent = 0);
    ~settings();
    void read_settings();
    double getSettime(double temp);
    int getRange(double temp);
    double getMaxT();

private:
    Ui::Settings ui_settings;
    QPainterPath *pp_temp;
    QPainterPath *pp_setp;

private slots:
    void accept();
};

#endif /*SETTINGS_H_*/
