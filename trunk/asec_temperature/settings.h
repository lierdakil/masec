#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtGui/QWidget>
#include <QDebug>
#include <QErrorMessage>
#include <QtDBus>
#include <QSettings>
#include "ui_settings.h"
#include "ctrl_temp.h"
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
    //QDBusInterface *iface;

private:
    Ui::Settings ui_settings;
	QTempTimeline test;
    
private slots:
	void accept();
	void on_btTest_clicked();
	void test_stateChange(QTimeLine::State newState);

};

#endif /*SETTINGS_H_*/
