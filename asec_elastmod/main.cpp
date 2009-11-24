#include <QtCore>
#include <QtGui>
#include <iostream>
#include <exception>
#include "defines.h"

int doCalculate(QTextStream *in, QTextStream *out, QString prefixf, QString prefixV)
{
    double Mq=-1, Ms=-1, Fr=-1, ls=-1, hs=-1, ws=-1, rho=-1;
    QString line;
    do
    {
        line=in->readLine().trimmed();
        if(line.startsWith('#'))
        {
            getparam(Mq,gm);
            getparam(Ms,gm);
            getparam(Fr,hz);
            getparam(ls,cm);
            getparam(hs,cm);
            getparam(ws,cm);
            getparam(rho,gm/cm3);
            *out<<line<<"\n";
        }
    } while (line.startsWith('#'));

    checkparam(Mq,gm);
    checkparam(Ms,gm);
    checkparam(Fr,Hz);
    checkparam(ls,cm);
    checkparam(hs,cm);
    checkparam(ws,cm);
    //checkparam(rho,gm/cm3);
    //TODO: Notify user that he can ignore rho
    if(rho<=0)
        rho=Ms/(ls*hs*ws);

    QStringList header=line.split('\t');
    header<<"fs, Hz";
    header<<"E, dn,cm2";
    header<<"1/Q";
    *out<<header.join("\t")<<"\n";
    while(true)
    {
        line=in->readLine().trimmed();
        if(line.isEmpty())
            break;

        if(line.isEmpty())
            continue;
        QStringList data=line.split('\t');
        int fri=header.indexOf(prefixf+"Resonance freq, Hz"),
        Vri=header.indexOf(prefixV+"Resonance ampl, V"),
        fai=header.indexOf(prefixf+"Antiresonance freq, Hz"),
        Vai=header.indexOf(prefixV+"Antiresonance ampl, V");
        if(data.count()<= fri ||
           data.count()<= fai ||
           data.count()<= Vri ||
           data.count()<= Vai)
            continue;

        double A=1+Mq/Ms;
        double B=Mq*Fr/Ms;
        double fr=data.at(fri).toDouble();
        double fa=data.at(fai).toDouble();
        double Vr=data.at(Vri).toDouble();
        double Va=data.at(Vai).toDouble();
        double fs=(fr+(fa-fr)*Va/(Va+Vr))*A-B;
        double E=4*rho*ls*ls*fs*fs;
        double Q1=2*(fa-fr)*A*sqrt(Vr*Va)/(fs*(Vr+Va));
        data_append(fs);
        data_append(E);
        data_append(Q1);
        *out<<data.join("\t")<<"\n";
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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

    QFile in;
    QFile out;
    QString prefixf,prefixV;

    if (consoleonly)
    {
        if(arguments.length()>=1)
            prefixf=arguments[0]+" ";
        if(arguments.length()>=2)
            prefixV=arguments[1]+" ";
        in.open(0,QIODevice::ReadOnly);
        out.open(1,QIODevice::WriteOnly);
    } else {
        in.setFileName(
                QFileDialog::getOpenFileName(
                        0,
                        "ASCII table with rough experimental data",
                        "","ASCII Table (*.txt)"
                        )
                );
        in.open(QIODevice::ReadOnly | QIODevice::Text);
        out.setFileName(
                QFileDialog::getSaveFileName(
                        0,
                        "ASCII table to save to",
                        "","ASCII Table (*.txt)"
                        )
                );
        out.open(QIODevice::WriteOnly | QIODevice::Text);
        prefixf=QInputDialog::getText(0,"Frequency column prefix", "Enter frequency column name prefix (usually Simplex) to use as calculation basis");
        prefixV=QInputDialog::getText(0,"Amplitude column prefix", "Enter amplitude column name prefix (usually Simplex) to use as calculation basis");
    }
    QTextStream instream(&in);
    QTextStream outstream(&out);
    doCalculate(&instream,&outstream,prefixf,prefixV);
    in.close();
    out.close();
    return 0;
}
