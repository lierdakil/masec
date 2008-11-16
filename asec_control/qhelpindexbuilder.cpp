/*
 * qhelpindexbuilder.cpp
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#include "qhelpindexbuilder.h"

QHelpIndexBuilder::QHelpIndexBuilder(QString service) : QMethodFetcher(service, "ru.pp.livid.asec.exports")
{
	object=service;
	object.replace("ru.pp.livid.asec.","");
	parse();
}

QHelpIndexBuilder::~QHelpIndexBuilder()
{

}

void QHelpIndexBuilder::new_method(QString name, int argcount)
{
	method=object+"."+name+"(";
	method_args=argcount;
	method_type="";
	if(argcount==0)
		index<<method+")";
}

void QHelpIndexBuilder::new_arg(QString name, QString type, QString direction)
{
	method_args--;
	if(direction=="out" && name=="")
		method_type=type;
	else
	{
		method+=type+" "+name;
		if(method_args>0)
			method+=",";
		else
		{
			method+=")";
			index<<method;
		}
	}
}
