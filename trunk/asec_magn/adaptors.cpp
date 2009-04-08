/*
 * adaptors.cpp
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */
#include "adaptors.h"

export_adaptor::export_adaptor(asec_magn *parent) : QDBusAbstractAdaptor(parent), m(parent)
{

}

void export_adaptor::set_field(double field)
{
	m->set_field(field);
}

flow_adaptor::flow_adaptor(asec_magn *parent) : QDBusAbstractAdaptor(parent), m(parent)
{
	setAutoRelaySignals(true);
}

void flow_adaptor::stop()
{
	m->magn_timer.stop();
}

help_adaptor::help_adaptor(asec_magn *parent) : QDBusAbstractAdaptor(parent), m(parent)
{

}

QStringList help_adaptor::returned_values()
{
	QStringList data;
	data<<QString("Requested Field, kG");
	data<<QString("Current Field,K");
	data<<QString("Stabilization time,min");
	return data;
}

QString help_adaptor::module_description()
{
    return trUtf8("Модуль управления источником питания для сверхпроводящего \n"
                  "соленоида LakeShore Model 625");
}

QString help_adaptor::set_field()
{
	QString desc=trUtf8("<p>Установить магнитное плое. Задается уставка и ожидается установление поля. </p> "
			"<p><code>field</code> - желаемое значение поля, кГс или Тл, в зависимости от настроек прибора</p> "
	);
	QString retvals=returned_values().join("<br/>");
	return trUtf8("%1 <p>Возвращаемые значения:<br/>%2</p>").arg(desc,retvals);
}
