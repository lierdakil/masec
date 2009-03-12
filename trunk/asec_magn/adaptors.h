/*
 * adaptors.h
 *
 *  Created on: 03.03.2009
 *      Author: kazei
 */

#ifndef ADAPTORS_H_
#define ADAPTORS_H_

#include "asec_magn.h"
#include <QtDBus>

class export_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.exports"))

        private:
            asec_magn *m;

public:
    export_adaptor(asec_magn *parent);

public slots:
    void set_field(double field);
};

class flow_adaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", QString("ru.pp.livid.asec.flow"))
        private:
            asec_magn *m;
public:
    flow_adaptor(asec_magn *parent);
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
            asec_magn *m;
public:
    help_adaptor(asec_magn *parent);
public slots:
    QString module_description();
    QStringList returned_values();
    QString set_field();
};

#endif /* ADAPTORS_H_ */
