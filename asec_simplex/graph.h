#ifndef GRAPH_H
#define GRAPH_H

#include <QtGui/QDialog>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_symbol.h>
#include <qwt_plot_picker.h>
#include "functions.h"

namespace Ui {
    class Graph;
}

class Graph : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Graph)
public:
    explicit Graph(QVector<qreal> X_exp, QVector<qreal> Y_exp,
                   QVector<qreal> X_f, QVector<qreal> Y_f,
                   double Rm, double Lm, double Cm ,double U,
                   double C0, double R0, double fa, double Va,
                   double fr, double Vr, bool useRm, bool useLm,
                   bool useCm, bool useU, bool useC0, bool useR0,
                   bool usesimplex);
    double Rm();
    double Lm();
    double Cm();
    double U();
    double C0();
    double R0();
    bool useRm();
    bool useLm();
    bool useCm();
    bool useU();
    bool useC0();
    bool useR0();
    double Vr;
    double Va;
    double fr;
    double fa;
    QwtPlotPicker* qwtPlotPicker;

    virtual ~Graph();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Graph *m_ui;
    QwtPlotCurve exp;
    QwtPlotCurve fn;
    QwtPlotMarker R;
    QwtPlotMarker A;
    double startf,endf;
    bool setres;//setres=true - setting resonance, false -- antiresonance

private slots:
    void sb_valueChanged(double );
    void sb_Lm_valueChanged(double );
    void plotSelected(QwtDoublePoint);
};

#endif // GRAPH_H
