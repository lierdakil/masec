#include "qplotwindow.h"

QPlotWindow::QPlotWindow(bool *success)
    : QWidget()
{
	ui.setupUi(this);
	X_iface=0;
	Y_iface=0;
	*success=editparams();
}

QPlotWindow::~QPlotWindow()
{

}

bool QPlotWindow::editparams()
{
	QPlotParam params;
	if(params.exec()==QDialog::Accepted)
	{
		ui.qwtPlot->setAxisTitle(QwtPlot::xBottom,params.X());
		ui.qwtPlot->setAxisTitle(QwtPlot::yLeft,params.Y());
		setWindowTitle(params.name());
		if(X_iface!=0)
			delete X_iface;
		if(Y_iface!=0)
			delete Y_iface;
		X_iface=new QDBusInterface(params.X_service(),"/","ru.pp.livid.asec.flow");
		Y_iface=new QDBusInterface(params.Y_service(),"/","ru.pp.livid.asec.flow");

		connect(X_iface,SIGNAL(finished(QStringList)),SLOT(new_X(QStringList)));
		connect(Y_iface,SIGNAL(finished(QStringList)),SLOT(new_Y(QStringList)));

		return true;
	} else
		return false;
}

void QPlotWindow::new_X(QStringList data)
{
	int xi = data.indexOf(QRegExp("^"+ui.qwtPlot->axisTitle(QwtPlot::xBottom).text()+":(.*)"));

	//Append new data
	if(xi>=0)
	{
		double val = data.at(xi).split(":").at(1).toDouble();//TODO: error handler?

		//if it's second X in a row
		if(X_data.count()==Y_data.count()+1)
		{
			//clone last Y
			Y_data.append(Y_data.last());
			//draw plot
			curve.setData(X_data,Y_data);
		}

		//add new data
		X_data.append(val);

		//if both values are in place, draw plot
		if(X_data.count()==Y_data.count())
			curve.setData(X_data,Y_data);
	}
}

void QPlotWindow::new_Y(QStringList data)
{
	int yi = data.indexOf(QRegExp("^"+ui.qwtPlot->axisTitle(QwtPlot::yLeft).text()+":(.*)"));

	if(yi>=0)
	{
		double val = data.at(yi).split(":").at(1).toDouble();//TODO: error handler?
		//if it's second Y in a row
		if(Y_data.count()==X_data.count()+1)
		{
			//clone last X
			X_data.append(X_data.last());
			//draw plot
			curve.setData(X_data,Y_data);
		}

		//add new data
		Y_data.append(val);

		//if both values are in place, draw plot
		if(X_data.count()==Y_data.count())
			curve.setData(X_data,Y_data);
	}
}
