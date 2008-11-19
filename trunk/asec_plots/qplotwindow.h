#ifndef QPLOTWINDOW_H
#define QPLOTWINDOW_H

#include <QtGui/QWidget>
#include "ui_qplotwindow.h"

#include "qplotparam.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class QPlotWindow : public QWidget
{
    Q_OBJECT

public:
    QPlotWindow(bool *success);
    ~QPlotWindow();

private:
    Ui::QPlotWindowClass ui;
    QVector<qreal> X_data;
    QVector<qreal> Y_data;
    QwtPlotCurve curve;
    QDBusInterface *X_iface;
    QDBusInterface *Y_iface;

public slots:
	void new_X(QStringList data);
	void new_Y(QStringList data);
	bool editparams();
};

#endif // QPLOTWINDOW_H
