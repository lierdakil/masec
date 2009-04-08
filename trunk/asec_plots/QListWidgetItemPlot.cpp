/*
 * QListWidgetItemPlot.cpp
 *
 *  Created on: 19.11.2008
 *      Author: �������������
 */

#include "QListWidgetItemPlot.h"

QListWidgetItemPlot::QListWidgetItemPlot(QListWidget *view, bool *success)
	: QListWidgetItem(view)
{
	window = new QPlotWindow(success);
	window->show();
	setText(window->windowTitle());
}

QListWidgetItemPlot::~QListWidgetItemPlot()
{
	delete window;
}

bool QListWidgetItemPlot::EditItem()
{
	return window->editparams();
}

bool QListWidgetItemPlot::ShowWindow()
{
	window->show();
	return true;
}
