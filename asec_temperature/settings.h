#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtGui/QWidget>
#include <QDebug>
#include <QErrorMessage>
#include <QtDBus>
#include <QSettings>
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

private:
    Ui::Settings ui_settings;
	QTempTimer test;

private slots:
	void accept();
	void on_btTest_clicked();
};

#endif /*SETTINGS_H_*/
