/*
 * QListWidgetItemPlot.h
 *
 *  Created on: 19.11.2008
 *      Author: �������������
 */

#ifndef QLISTWIDGETITEMPLOT_H_
#define QLISTWIDGETITEMPLOT_H_

#include <QListWidget>
#include "qplotwindow.h"

class QListWidgetItemPlot : public QListWidgetItem
{
private:
	QPlotWindow *window;

public:
	QListWidgetItemPlot(QListWidget *view, bool *success);
	~QListWidgetItemPlot();
	bool EditItem();
	bool ShowWindow();
};

#endif /* QLISTWIDGETITEMPLOT_H_ */
