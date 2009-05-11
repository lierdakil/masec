#ifndef ASEC_PLOTS_H
#define ASEC_PLOTS_H

#include <QtGui/QWidget>
#include <QtDBus>
#include "ui_asec_plots.h"

#include "QListWidgetItemPlot.h"
#include <QErrorMessage>

class asec_plots : public QWidget
{
    Q_OBJECT

public:
    asec_plots(QWidget *parent = 0);
    ~asec_plots();

protected:
	 void closeEvent(QCloseEvent *event);

private:
    Ui::asec_plotsClass ui;

public slots:
	void on_btAddPlot_clicked();
	void on_btDelPlot_clicked();
	void on_btEditPlot_clicked();
	void on_lwPlots_itemDoubleClicked(QListWidgetItem*);
};

#endif // ASEC_PLOTS_H
