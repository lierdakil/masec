/*
 * adaptors.h
 *
 *  Created on: 01.12.2008
 *      Author: kazei
 */

#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include <QtDBus>
#include "asec_upr.h"

class export_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.exports"))

        private:
            vibupraut *vua;

public:
    export_adaptor(vibupraut *v);
public slots:
    void mes_res(double startf, double stopf);
    void mes_res_file(double startf, double stopf, QString filename);
};

class flow_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.flow"))

        private:
            vibupraut *vua;

public:
    flow_adaptor(vibupraut *v);
public slots:
    void stop();
signals:
    void finished(QStringList data);
};

class help_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.help"))

        private:
            vibupraut *vua;

public:
    help_adaptor(vibupraut *v);

public slots:
    QString module_description();
    QStringList returned_values();
    QString mes_res();
    QString mes_res_file();
};

#endif /* ADAPTORS_H_ */
