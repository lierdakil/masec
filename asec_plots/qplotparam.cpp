#include "qplotparam.h"

QPlotParam::QPlotParam(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	QStringList list = QDBusConnection::sessionBus().interface()->registeredServiceNames().value().filter(QRegExp("^ru.pp.livid.asec.(.+)"));
	foreach(QString service, list)
	{
		QDBusInterface iface(service,"/","ru.pp.livid.asec.help");
		QDBusReply<QStringList> rv = iface.call("returned_values");
		//TODO: Error handlers
		foreach(QString value, rv.value())
		{
			ui.cbX->addItem(value,service);
			ui.cbY->addItem(value,service);
		}
	}
}

QPlotParam::~QPlotParam()
{

}

QString QPlotParam::name()
{
	return ui.edName->text();
}

QString QPlotParam::X()
{
	return ui.cbX->currentText();
}

QString QPlotParam::Y()
{
	return ui.cbY->currentText();
}

QString QPlotParam::X_service()
{
	return ui.cbX->itemData(ui.cbX->currentIndex()).toString();
}

QString QPlotParam::Y_service()
{
	return ui.cbX->itemData(ui.cbX->currentIndex()).toString();
}
