/*
 * adaptors.cpp
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */
#include "adaptors.h"

export_adaptor::export_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
{

}


void export_adaptor::mes_res(double startf, double stopf)
{
    vua->measure(startf, stopf, QString());
}

void export_adaptor::mes_res_file(double startf, double stopf, QString filename)
{
    vua->measure(startf, stopf, filename);
}

flow_adaptor::flow_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
{
    setAutoRelaySignals(true);
    //or
    //connect(v,SIGNAL(finished(QStringList)),this,SIGNAL(finished(QStringList)));
}

void flow_adaptor::stop()
{
    vua->thread.stop();
}


help_adaptor::help_adaptor(vibupraut *v) : QDBusAbstractAdaptor(v), vua(v)
{

}

QStringList help_adaptor::returned_values()
{
    QStringList data;
    data<<QString("Generator amplitude, VPP");
    data<<QString("First run start freq, Hz");
    data<<QString("First run stop freq, Hz");
    data<<QString("Second run start freq, Hz");
    data<<QString("Second run stop freq, Hz");
    data<<QString("Resonance freq, Hz");
    data<<QString("Resonance ampl, V");
    data<<QString("Antiresonance freq, Hz");
    data<<QString("Antiresonance ampl, V");
    return data;
}

QString help_adaptor::module_description()
{
    return trUtf8("Модуль управления составным резонатором на базе\n"
                  "генератора переменного сигнала Agilent 33120A,\n"
                  "цифрового мультиметра Agilent 34401A\n"
                  "и осциллографа Tektronix TDS 2012\n");
}

#define mes_res_desc "<p>Получить данные о резонансе и антирезонансе. </p> \
        <p><code>startf</code> - частота начала пробега в герцах</p> \
        <p><code>stopf</code> - частота конца пробега в герцах</p>"

        QString help_adaptor::mes_res()
{
    QString desc=trUtf8(mes_res_desc);
    QString retvals=returned_values().join("<br/>");
    return trUtf8("%1 <p>Возвращаемые значения:<br/>%2</p>").arg(desc,retvals);
}

QString help_adaptor::mes_res_file()
{
    QString desc=trUtf8(
            mes_res_desc
            "<p><code>filename</code> - имя файла для сохранения вида"
            "резонансной кривой (если пустое - не сохранять).</p>"
            "<p>Слеши в имени файла должны быть прямыми.</p>"
            );
    QString retvals=returned_values().join("<br/>");
    return trUtf8("%1 <p>Возвращаемые значения:<br/>%2</p>").arg(desc,retvals);
}
