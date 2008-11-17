#ifndef REPLYINTERFACE_H
#define REPLYINTERFACE_H

#include <QtCore>
#include <QtDBus>

class ReplyInterface
{

private:
	QStringList data;
public:
	ReplyInterface()
	{
	}

    ~ReplyInterface()
    {
    	QDBusInterface iface("ru.pp.livid.asec","/","ru.pp.livid.asec.reply");
    	iface.call(QDBus::NoBlock, "reply", data);
    }

    void operator<<(QString string)
    {
    	data<<string;
    }
};

#endif
