#include <QtCore>
#include <QtGui>
#include <iostream>

#define getparam(p,u) \
{ \
  QRegExp r(QString("%1\\s*=\\s*([0-9eE.]+)\\s*([A-z]+)").arg(#p));\
  if(r.indexIn(line)>=0) if(r.capturedTexts().at(2).toLower()==#u) p=r.capturedTexts().at(1).toDouble(); }
#define checkparam(p,u) \
if(p<0) p=QInputDialog::getDouble(0, \
                                  "asec_elastmod", \
                                  QString("Please enter value for %1 in %2")\
                                  .arg(#p).arg(#u),\
                                  0,-2147483647,2147483647,10); \
        std::cout<<"# "#p" = "<<p<<" "#u"\n"
#define data_append(var) data<<QString::number(var,'f')

int doCalculate(QFile *f, QString prefixf, QString prefixV)
{
    double Mq=-1, Ms=-1, Fr=-1, ls=-1, hs=-1, ws=-1, rho=-1;
    QString line;
    do
    {
        line=QString::fromLocal8Bit(f->readLine()).trimmed();
        if(line.startsWith('#'))
        {
            getparam(Mq,gm);
            getparam(Ms,gm);
            getparam(Fr,hz);
            getparam(ls,cm);
            getparam(hs,cm);
            getparam(ws,cm);
            getparam(rho,gm/cm3);
            //std::cout<<line.toLocal8Bit().data();
        }
    } while (line.startsWith('#'));

    checkparam(Mq,gm);
    checkparam(Ms,gm);
    checkparam(Fr,Hz);
    checkparam(ls,cm);
    checkparam(hs,cm);
    checkparam(ws,cm);
    checkparam(rho,gm/cm3);
    //TODO: Notify user that he can ignore rho
    if(rho<=0)
        rho=Ms/(ls*hs*ws);

    QStringList header=line.split('\t');
    header<<"fs, Hz";
    header<<"E, dn,cm2";
    header<<"1/Q";
    std::cout<<header.join("\t").append("\n").toLocal8Bit().data();
    QByteArray buf;
    while(true)
    {
        buf=f->readLine();
        if(buf.isEmpty())
            break;
        line=QString::fromLocal8Bit(buf).trimmed();

        if(line.isEmpty())
            continue;
        QStringList data=line.split('\t');
        if(data.count()<=header.indexOf(prefixf+"Resonance freq, Hz") ||
           data.count()<= header.indexOf(prefixV+"Resonance ampl, V") ||
           data.count()<= header.indexOf(prefixf+"Antiresonance freq, Hz") ||
           data.count()<= header.indexOf(prefixV+"Antiresonance ampl, V"))
            continue;
        double A=1+Mq/Ms;
        double B=Mq*Fr/Ms;
        double fr=data.at(header.indexOf(prefixf+"Resonance freq, Hz")).toDouble();
        double fa=data.at(header.indexOf(prefixf+"Antiresonance freq, Hz")).toDouble();
        double Vr=data.at(header.indexOf(prefixV+"Resonance ampl, V")).toDouble();
        double Va=data.at(header.indexOf(prefixV+"Antiresonance ampl, V")).toDouble();
        double fs=(fr+(fa-fr)*Va/(Va+Vr))*A-B;
        double E=4*rho*ls*ls*fs*fs;
        double Q1=2*(fa-fr)*A*sqrt(Vr*Va)/(fs*(Vr+Va));
        data_append(fs);
        data_append(E);
        data_append(Q1);
        std::cout<<data.join("\t").append("\n").toLocal8Bit().data();
    }
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f;
    QString prefixf,prefixV;
    QString parameters;
    QStringList arguments;
    bool consoleonly=false;
    //TODO: add GUI, use files instead of STDIN in gui mode

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
            switch(parameters.at(i).toAscii()){
            case 'c':
                consoleonly=true;
                break;
            default:
                std::cerr<<"Usage:\n"
                        <<"\t"<<argv[0]<<" [-c]\n"
                        <<"\tSTDIN: Ascii table with experimental data\n"
                        <<"\tSTDOUT: Ascii table with appended calculated data\n"
                        <<"\tc - console only, no gui, MANDATORY for stdin/stdout to work\n";
                return 1;
                break;
            }
        }
    }

    if (consoleonly)
    {
        if(argc>1)
            prefixf=QString::fromLocal8Bit(argv[1])+" ";
        if(argc>2)
            prefixV=QString::fromLocal8Bit(argv[2])+" ";
        if(f.open(0,QIODevice::ReadOnly))
        {
            doCalculate(&f,prefixf,prefixV);
            f.close();
            return 0;
        } else {
            return 2;
        }
    }
}
