#include <QtCore>
#include <QtGui>
#include <iostream>

#define getparam(p) \
{ \
  QRegExp r(QString("%1\\s*=\\s*([0-9eE.]+)").arg(#p));\
  if(r.indexIn(line)>=0) p=r.capturedTexts().at(1).toDouble(); }
#define checkparam(p,u) \
if(p<0) p=QInputDialog::getDouble(0, \
                                  "asec_elastmod", \
                                  QString("Please enter value for %1 in %2")\
                                  .arg(#p).arg(#u),\
                                  0,-2147483647,2147483647,10); \
        std::cout<<"# "#p" = "<<p<<" "#u"\n"
#define data_append(var) QString::number(var,'f'); data<<buf

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f;
    QString prefixf,prefixV;
    if(argc>1)
        prefixf=QString::fromLocal8Bit(argv[1])+" ";
    if(argc>2)
        prefixV=QString::fromLocal8Bit(argv[2])+" ";
    if(f.open(0,QIODevice::ReadOnly))
    {
        double Mq=-1, Ms=-1, Fr=-1, ls=-1, hs=-1, ws=-1;
        QString line;
        do
        {
            line=QString::fromLocal8Bit(f.readLine()).trimmed();
            if(line.startsWith('#'))
            {
                getparam(Mq);
                getparam(Ms);
                getparam(Fr);
                getparam(ls);
                getparam(hs);
                getparam(ws);
                //std::cout<<line.toLocal8Bit().data();
            }
        } while (line.startsWith('#'));

        checkparam(Mq,gm);
        checkparam(Ms,gm);
        checkparam(Fr,Hz);
        checkparam(ls,cm);
        checkparam(hs,cm);
        checkparam(ws,cm);

        QStringList header=line.split('\t');
        header<<"fs, Hz";
        header<<"E, dn,cm2";
        header<<"1/Q";
        std::cout<<header.join("\t").append("\n").toLocal8Bit().data();
        QByteArray buf;
        while(true)
        {
            buf=f.readLine();
            if(buf.isEmpty())
                break;
            line=QString::fromLocal8Bit(buf).trimmed();

            if(line.isEmpty())
                continue;
            QStringList data=line.split('\t');
            if(data.count()<4)
                continue;
            double A=1+Mq/Ms;
            double B=Mq*Fr/Ms;
            double fr=data.at(header.indexOf(prefixf+"Resonance freq, Hz")).toDouble();
            double fa=data.at(header.indexOf(prefixf+"Antiresonance freq, Hz")).toDouble();
            double Vr=data.at(header.indexOf(prefixV+"Resonance ampl, V")).toDouble();
            double Va=data.at(header.indexOf(prefixV+"Antiresonance ampl, V")).toDouble();
            double fs=(fr+(fa-fr)*Va/(Va+Vr))*A-B;
            double E=4*Ms/(hs*ws)*ls*fs*fs;
            double Q1=2*(fa-fr)*A*sqrt(Vr*Va)/(fs*(Vr+Va));
            data_append(fs);
            data_append(E);
            data_append(Q1);
            std::cout<<data.join("\t").append("\n").toLocal8Bit().data();
        }
        f.close();
        return 0;
    } else {
        return 2;
    }
}
