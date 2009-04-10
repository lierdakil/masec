/*
 * adaptors.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#include "adaptors.h"

export_adaptor::export_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
{

}

void export_adaptor::set_temp(double temp, double ramp, double timeout)
{
    t->set_temp_zone(temp,ramp,timeout);
}

flow_adaptor::flow_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
{
    setAutoRelaySignals(true);
}

void flow_adaptor::stop()
{
    t->temptl.stop();
}

help_adaptor::help_adaptor(vib_temperature *parent) : QDBusAbstractAdaptor(parent), t(parent)
{

}

QStringList help_adaptor::returned_values()
{
    QStringList data;
    data<<QString("Setpoint,K");
    //data<<QString("Temperature 1,K");
    //data<<QString("Temperature 2,K");
    data<<QString("Ramp,K/min");
    data<<QString("Timeout,min");
    data<<QString("Stabilization time,min");
    return data;
}

QString help_adaptor::module_description()
{
    return trUtf8("Модуль управления ПИД-регулятором температуры LakeShore 331");
}

QString help_adaptor::set_temp()
{
    QString desc=trUtf8("<p>Установить температуру. Устанавливается точка и ожидается установление температруры. </p> "
                        "<p><code>temp</code> - желаемое значение температуры, К</p> "
                        "<p><code>ramp</code> - скорость изменения точки К/мин, минимум 0.1, максимум 100, "
                        "при всех прочих значениях тока изменится скачком </p>"
                        "<p><code>timeout</code> - "
                        "максимальное время установлния температуры (в минутах). Если это время "
                        "будет превышено, выполнение программы прервется.</p>");
    QString retvals=returned_values().join("<br/>");
    return trUtf8("%1 <p>Возвращаемые значения:<br/>%2</p>").arg(desc,retvals);
}
