#include <QtCore>
#include <QFile>
#include <iostream>
using namespace std;

// TODO: make a bat file

int main(int argc, char *argv[])
{
	QFile f(argv[1]);
	if (f.open(QIODevice::ReadOnly))
	{
		QString line = QString::fromUtf8(f.readLine());
		QStringList header;

		//copy comment
		if(line.contains("/*"))
			while(!line.contains("*/"))
			{
				cout<<line.toLocal8Bit().data();
				line = QString::fromUtf8(f.readLine());
			}
		cout<<" * \n";
		cout<<" * File was parsed with asec_parser,\n * hence it's a tab-delimited ascii-table now\n";
		cout<<line.toLocal8Bit().data();

		//build header
		unsigned long int data_start=f.pos();
		while(!f.atEnd())
		{
			line = QString::fromUtf8(f.readLine());
			QStringList dataline=line.split(";");
			foreach(QString entry, dataline)
			{
				QString name=entry.split(":").at(0);
				if(!header.contains(name))
					header<<name;
			}
		}

		//output header
		foreach(QString name, header)
			cout<<name.toLocal8Bit().data()<<"\t";
		cout<<"\n";

		f.seek(data_start);
		while(!f.atEnd())
		{
			line = QString::fromUtf8(f.readLine());
			QStringList dataline=line.split(";");
			QVector<QString> row;
			row.fill("",header.count());
			foreach(QString entry, dataline)
			{
				QString name=entry.split(":").at(0);
				QString value=entry.split(":").at(1);
				int id=header.indexOf(QRegExp(name));
				row[id]=value;
			}
			row.last().remove("\n");
			foreach(QString cell, row)
				cout<<cell.toLocal8Bit().data()<<"\t";
			cout<<"\n";
		}
	} else {
		cout<<"Usage: asec_parser \"filename.red\"";
	}
	return 0;
}
