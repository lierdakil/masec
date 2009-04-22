#ifndef GRAPH_H
#define GRAPH_H

#include <QtGui/QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace Ui {
    class Graph;
}

class Graph : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Graph)
public:
    explicit Graph(QVector<qreal> X_exp, QVector<qreal> Y_exp,
                   QVector<qreal> X_f, QVector<qreal> Y_f,
                   double Rm, double Lm, double Cm ,double U,
                   double C0, double R0);
    virtual ~Graph();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Graph *m_ui;
    QwtPlotCurve exp;
    QwtPlotCurve fn;
    double startf,endf;

private slots:
    void sb_valueChanged(double );
};

#endif // GRAPH_H
