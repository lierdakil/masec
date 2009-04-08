#include "qplotwindow.h"

QPlotWindow::QPlotWindow(bool *success)
    : QWidget()
{
    ui.setupUi(this);
    X_iface=0;
    Y_iface=0;
    *success=editparams();
    curve.attach(ui.qwtPlot);
}

QPlotWindow::~QPlotWindow()
{
    disconnect(X_iface,SIGNAL(finished(QStringList)),this,SLOT(new_X(QStringList)));
    disconnect(Y_iface,SIGNAL(finished(QStringList)),this,SLOT(new_Y(QStringList)));
    delete X_iface;
    delete Y_iface;
}

bool QPlotWindow::editparams()
{
    QPlotParam params;
    if(params.exec()==QDialog::Accepted)
    {
        ui.qwtPlot->setAxisTitle(QwtPlot::xBottom,params.X());
        ui.qwtPlot->setAxisTitle(QwtPlot::yLeft,params.Y());
        QStringList labels;
        labels<<params.X();
        labels<<params.Y();
        ui.twData->setHorizontalHeaderLabels(labels);
        setWindowTitle(params.name());
        if(X_iface!=0)
        {
            disconnect(X_iface,SIGNAL(finished(QStringList)),this,SLOT(new_X(QStringList)));
            delete X_iface;
        }
        if(Y_iface!=0)
        {
            disconnect(Y_iface,SIGNAL(finished(QStringList)),this,SLOT(new_Y(QStringList)));
            delete Y_iface;
        }
        X_iface=new QDBusInterface(params.X_service(),"/","ru.pp.livid.asec.flow");
        Y_iface=new QDBusInterface(params.Y_service(),"/","ru.pp.livid.asec.flow");

        //Reverse order, because event stack is popped in reverse order
        connect(Y_iface,SIGNAL(finished(QStringList)),SLOT(new_Y(QStringList)));
        connect(X_iface,SIGNAL(finished(QStringList)),SLOT(new_X(QStringList)));

        return true;
    } else
        return false;
}

void QPlotWindow::addX(qreal X)
{
    X_data.append(X);
    int lenx=X_data.count();
    int leny=Y_data.count();
    int len=lenx>leny?lenx:leny;
    ui.twData->setRowCount(len);
    QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(X));
    ui.twData->setItem(lenx-1,0,newItem);
}

void QPlotWindow::addY(qreal Y)
{
    Y_data.append(Y);
    int lenx=X_data.count();
    int leny=Y_data.count();
    int len=lenx>leny?lenx:leny;
    ui.twData->setRowCount(len);
    QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(Y));
    ui.twData->setItem(leny-1,1,newItem);
}

void QPlotWindow::updateCurve()
{
    curve.setData(X_data,Y_data);
    ui.qwtPlot->replot();
}

void QPlotWindow::new_X(QStringList data)
{
    int xi = data.indexOf(QRegExp("^"+ui.qwtPlot->axisTitle(QwtPlot::xBottom).text()+":(.*)"));

    //Append new data
    if(xi>=0)
    {
        bool ok=true;
        double val = data.at(xi).split(":").at(1).toDouble(&ok);
        if(ok)
            addX(val);
        //TODO: tell about error?
    }
}

void QPlotWindow::new_Y(QStringList data)
{
    int yi = data.indexOf(QRegExp("^"+ui.qwtPlot->axisTitle(QwtPlot::yLeft).text()+":(.*)"));

    if(yi>=0)
    {
        bool ok;
        double val = data.at(yi).split(":").at(1).toDouble(&ok);
        if(ok)
        {
            //if it's second Y in a row
            if(Y_data.count()==X_data.count())
            {
                //clone last X
                if(X_data.count()>0)
                    addX(X_data.last());
                else
                    addX(0);
            }

            //add new data
            addY(val);
            updateCurve();
        } //TODO: tell about error?
    }
}

void QPlotWindow::on_btClear_clicked()
{
    X_data.clear();
    Y_data.clear();
    ui.twData->setRowCount(0);
    updateCurve();
}
