#ifndef REPLYINTERFACE_H
#define REPLYINTERFACE_H

#include <QtCore>
#include <QtDBus>

class ReplyInterface
{

public:
	QStringList data;

	ReplyInterface()
	{
	}

    ~ReplyInterface()
    {
    	QDBusInterface iface("ru.pp.livid.asec","/","ru.pp.livid.asec.reply");
    	iface.call(QDBus::NoBlock, "reply_call", data);
    }
};

#endif
