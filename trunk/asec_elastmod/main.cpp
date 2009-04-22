#include <QtCore>
#include <QtGui>

#define getparam(p) { QRegExp r=match(#p); if(r.indexIn(line)>=0) p=r.capturedTexts().at(1).toDouble(); }
#define checkparam(p) if(p<0) p=QInputDialog::getDouble(0,"asec_elastmod",QString("Please enter value for %1").arg(#p))
#define data_append(var) buf.setNum(var); data<<buf

QRegExp match(QString name)
{
    return QRegExp(QString("%1\\s*=\\s*([0-9eE.]+)").arg(name));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc!=3)
        return 1;
    QFile f(argv[1]);
    QFile f2(argv[2]);
    if(f.open(QIODevice::ReadOnly) && f2.open(QIODevice::WriteOnly))
    {
        double Mq=-1, Ms=-1, Fr=-1, ls=-1, hs=-1, ws=-1;
        QString line;
        do
        {
            line=QString::fromLocal8Bit(f.readLine());
            if(line.startsWith('#'))
            {
                getparam(Mq);
                getparam(Ms);
                getparam(Fr);
                getparam(ls);
                getparam(hs);
                getparam(ws);
                f2.write(line.toLocal8Bit());
            }
        } while (line.startsWith('#'));

        checkparam(Mq);
        checkparam(Ms);
        checkparam(Fr);
        checkparam(ls);
        checkparam(hs);
        checkparam(ws);

        line.remove("\n");
        QStringList header=line.split('\t');
        header<<"fs, Hz";
        header<<"E, dn,cm2";
        header<<"1/Q";
        f2.write(header.join("\t").append("\n").toLocal8Bit());
        while(!f.atEnd())
        {
            line=QString::fromLocal8Bit(f.readLine());
            line.remove("\n");
            QStringList data=line.split('\t');
            double A=1+Mq/Ms;
            double B=Mq*Fr/Ms;
            double fr=data.at(header.indexOf("Resonance freq, Hz")).toDouble();
            double fa=data.at(header.indexOf("Antiresonance freq, Hz")).toDouble();
            double Vr=data.at(header.indexOf("Resonance ampl, V")).toDouble();
            double Va=data.at(header.indexOf("Antiresonance ampl, V")).toDouble();
            double fs=(fr+(fa-fr)*Va/(Va+Vr))*A-B;
            double E=4*Ms/(hs*ws)*ls*fs;
            double Q1=2*(fa-fr)*A*sqrt(Vr*Va)/(fs*(Vr+Va));
            QString buf;
            data_append(fs);
            data_append(E);
            data_append(Q1);
            f2.write(data.join("\t").append("\n").toLocal8Bit());
        }
        return 0;
    } else {
        return 2;
    }
}
