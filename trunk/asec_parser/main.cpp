#include <QtCore>
#include <QtGui>
#include <QFile>
#include <iostream>
#include <mainwindow.h>
using namespace std;

int len(char* str)
{
    int i=0;
    while(str[i])
        i++;
    return i;
}

int main(int argc, char *argv[])
{
    bool usegui=false;
    bool stripcomments=false;
    QString parameters;
    QStringList arguments;
    for(int i=1; i<argc; ++i)
    {
        if(argv[i][0]=='-')
            parameters+=QString::fromLocal8Bit(argv[i]+1);
        else
            arguments<<QString::fromLocal8Bit(argv[i]);
    }
    if (!parameters.isEmpty())
    {
        for(int i=0; i<parameters.length(); ++i)
        {
            if(parameters[i]=='g')
                usegui=true;
            else if (parameters[i]=='s')
                stripcomments=true;
            else
                goto usage;
        }
    }

    if(arguments.count()==1 && !usegui)
    {
        QFile f(arguments[0]);
        if (f.open(QIODevice::ReadOnly))
        {
            QString line;
            QStringList header;

            //build header
            while(!f.atEnd())
            {
                line = QString::fromUtf8(f.readLine());
                while (line.startsWith('#')){
                    line = QString::fromUtf8(f.readLine());
                }
                QStringList dataline=line.split(";");
                foreach(QString entry, dataline)
                {
                    QString name=entry.split(":").at(0);
                    if(!header.contains(name))
                        header<<name;
                }
            }

            f.seek(0);
            do{
                line = QString::fromUtf8(f.readLine());
                if(!stripcomments && line.startsWith('#'))
                    cout<<line.toLocal8Bit().data();
            }while (line.startsWith('#'));

            //output header
            foreach(QString name, header)
                cout<<name.toLocal8Bit().data()<<"\t";
            cout<<"\n";

            while(true)
            {
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
                if(f.atEnd())
                    break;
                else
                    line = QString::fromUtf8(f.readLine());
            }
        } else {
            cout<<"Could not open file ";
            cout<<(arguments.at(0).toLocal8Bit().data());
            cout<<":";
            cout<<f.errorString().toLocal8Bit().data();
        }
        return 0;
    } else if (arguments.count()==1 && usegui) {
        //show gui
        QApplication a(argc,argv);
        MainWindow w(arguments[0]);
        w.show();
        a.exec();
        return 0;
    }
    usage:
    cout<<"Usage: asec_parser [-gs] \"filename.red\"";
    return 1;
}
