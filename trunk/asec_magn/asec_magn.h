#ifndef ASEC_MAGN_H
#define ASEC_MAGN_H

#include <QtGui/QWidget>
#include "ui_asec_magn.h"
#include "QMagnTimeline.h"
#include <QErrorMessage>

class asec_magn : public QWidget
{
    Q_OBJECT

public:
    asec_magn(QWidget *parent = 0);
    ~asec_magn();

public:
    QMagnTimeline magn_timer;

public slots:
    void set_field(float field); //method to set field

    void field_set(float reqfield, float field, float settime/*minutes*/);//field reached requested value
    //void stopped();//stopped by user
    void quench();//achtung! quench detected!
    void error(QString message);
    void newpoint(float time, float field);//used to draw points
    void on_edGPID_returnPressed();
    void on_btSetField_clicked();

signals:
    void finished(QStringList data);
    void critical(QString module, QString message);

private:
    Ui::asec_magnClass ui;
};

#include "adaptors.h"

#endif // ASEC_MAGN_H
