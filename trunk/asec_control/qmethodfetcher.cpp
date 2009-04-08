/*
 * qmethodfetcher.cpp
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#include "qmethodfetcher.h"

QMethodFetcher::QMethodFetcher(QString service, QString interface): service(service), interface(interface)
{

}

void QMethodFetcher::parse()
{
	QDBusInterface iface(service,"/","org.freedesktop.DBus.Introspectable");
	if (!iface.isValid())
	{
		QDBusError err(iface.lastError());
		emit error(trUtf8("Cannot introspect object %1 at %2:\n  %3 (%4)\n").arg("/").arg(
				service).arg(err.name()).arg(err.message()));
	}

	QDBusReply<QString> xml = iface.call("Introspect");

	if (!xml.isValid()) {
		QDBusError err(xml.error());
		if (err.isValid()) {
			emit error(trUtf8("Call to object %1 at %2:\n  %3 (%4) failed\n").arg(
					"/").arg(service).arg(err.name()).arg(err.message()));
		} else {
			emit error(trUtf8("Invalid XML received from object %1 at %2\n").arg(
					"/").arg(service));
		}
	}

	QDomDocument doc;
	doc.setContent(xml);

	QDomElement node = doc.documentElement();//path = /, <node>
	QDomNodeList ifaces = node.elementsByTagName("interface");
	for(unsigned int i=0;i<ifaces.length();++i)
		if(ifaces.item(i).isElement())
			if(ifaces.item(i).toElement().attribute("name")==interface)
			{
				QDomNodeList methods=ifaces.item(i).toElement().elementsByTagName("method");
				for(unsigned int j=0;j<methods.length();++j)
					if(methods.item(j).isElement())
					{
						QDomElement method = methods.item(j).toElement();
						QDomNodeList args = method.elementsByTagName("arg");
						new_method(method.attribute("name"),args.count());
						for(unsigned int k=0;k<args.length();++k)
							if(args.item(k).isElement())
							{
								QDomElement arg=args.item(k).toElement();
								new_arg(
										arg.attribute("name"),
										arg.attribute("type"),
										arg.attribute("direction")
								);
							}
					}
			}
}

QMethodFetcher::~QMethodFetcher()
{

}
