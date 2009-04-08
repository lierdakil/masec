/*
 * qfuncinitbulder.cpp
 *
 *  Created on: 15.11.2008
 *      Author: Администратор
 */

#include "qfuncinitbuilder.h"

QFuncInitBuilder::QFuncInitBuilder(QString service) : QMethodFetcher(service, "ru.pp.livid.asec.exports")
{
    code="%4.%1 = function(%3) { call(\"%1\", \"%2\"%5%3); }; ";
    object=service;
    object.replace("ru.pp.livid.asec.","");
    parse();
}

QFuncInitBuilder::~QFuncInitBuilder()
{

}


void QFuncInitBuilder::new_method(QString name, int argcount)
{
    method_name=name;
    method_args="";
    method_args_count=argcount;
    method_type="";
    if(argcount==0)
        init+=code.arg(method_name,service,"",object,"");
}

void QFuncInitBuilder::new_arg(QString name, QString type, QString direction)
{
    method_args_count--;
    if(direction=="out" && name=="")
        method_type=type;
    else
    {
        method_args+=name;
        if(method_args_count>0)
            method_args+=",";
        else
            init+=code.arg(method_name,service,method_args,object,",");
    }
}
