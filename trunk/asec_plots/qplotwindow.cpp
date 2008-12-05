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

		connect(X_iface,SIGNAL(finished(QStringList)),SLOT(new_X(QStringList)));
		connect(Y_iface,SIGNAL(finished(QStringList)),SLOT(new_Y(QStringList)));

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
//	ui.twData->item(lenx-1,0)->setText(QString("%1").arg(X));
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
		double val = data.at(xi).split(":").at(1).toDouble();//TODO: error handler?

		//if it's second X in a row
		if(X_data.count()==Y_data.count()+1)
		{
			//clone last Y
			if(Y_data.count()>0)
				addY(Y_data.last());
			else
				addY(0);
			//draw plot
			updateCurve();
		}

		//add new data
		addX(val);


		//if both values are in place, draw plot
		if(X_data.count()==Y_data.count())
			updateCurve();
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
			if(X_data.count()>0)
				addX(X_data.last());
			else
				addX(0);
			//draw plot
			updateCurve();
		}

		//add new data
		addY(val);

		//if both values are in place, draw plot
		if(X_data.count()==Y_data.count())
			updateCurve();
	}
}
