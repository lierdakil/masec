#ifndef QPLOTPARAM_H
#define QPLOTPARAM_H

#include <QtGui/QDialog>
#include "ui_qplotparam.h"

#include <QtDBus>

class QPlotParam : public QDialog
{
    Q_OBJECT

public:
    QPlotParam(QWidget *parent = 0);
    ~QPlotParam();

    QString name();
    QString X();
    QString Y();
    QString X_service();
    QString Y_service();

public slots:
	void on_buttonBox_accepted();

private:
    Ui::QPlotParamClass ui;
};

#endif // QPLOTPARAM_H
