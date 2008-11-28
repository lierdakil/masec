#include "asec_plots.h"

asec_plots::asec_plots(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	QErrorMessage::qtHandler();
	QDBusConnection::sessionBus();
}

asec_plots::~asec_plots()
{

}

void asec_plots::on_btAddPlot_clicked()
{
	bool success;
	QListWidgetItemPlot *item =new QListWidgetItemPlot(ui.lwPlots, &success);
	if(!success)
		delete item;
}

void asec_plots::on_btDelPlot_clicked()
{
	delete ui.lwPlots->currentItem();
}

void asec_plots::on_btEditPlot_clicked()
{
	((QListWidgetItemPlot*)ui.lwPlots->currentItem())->EditItem();
}

void asec_plots::on_lwPlots_itemDoubleClicked(QListWidgetItem* item)
{
	((QListWidgetItemPlot*)item)->ShowWindow();
}
