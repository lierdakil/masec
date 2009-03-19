#include "settings.h"

settings::settings(QWidget *parent)
    : QDialog(parent)
{
    QErrorMessage::qtHandler();
    ui_settings.setupUi(this);

    read_settings();
}

settings::~settings()
{

}

double settings::getSettime(double temp)
{
    for (int n=0;n<10;n++)
    {
        double ct = this->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
        if (temp<=ct)
            return this->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->value();
    }
    //qFatal("getSettime failed!");
    return 0;
}

int settings::getRange(double temp)
{
    for (int n=0;n<10;n++)
    {
        double ct = this->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
        if (temp<=ct)
            return this->findChild<QComboBox*>(QString("sbRange_%1").arg(n))->currentIndex();
    }
    //qFatal("getSettime failed!");
    return 0;
}

double settings::getMaxT()
{
    double maxt=0;
    for (int n=0;n<10;n++)
    {
        double ct = this->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
        if (maxt<ct)
            maxt=ct;
    }
    return maxt;
}

void settings::read_settings()
{
    QSettings f("settings.ini",QSettings::IniFormat);
    QString tempid = f.value("GPIB/tempid","").toString();

    ui_settings.edTempId->setText(tempid);

    try {
        if (!tempid.isEmpty())
        {
            tempctrl temp(tempid);
            qApp->setProperty("tempid",tempid);

            for (int n=0;n<=9;n++)
            {
                float Temp, P, I, D, mout;
                int range;
                temp.readzone(n+1,&Temp,&P,&I,&D,&mout,&range);
                float time=f.value(QString("TempCtrl/Time_%1").arg(n),0).toDouble();
                this->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->setValue(Temp);
                this->findChild<QDoubleSpinBox*>(QString("sbP_%1").arg(n))->setValue(P);
                this->findChild<QDoubleSpinBox*>(QString("sbI_%1").arg(n))->setValue(I);
                this->findChild<QDoubleSpinBox*>(QString("sbD_%1").arg(n))->setValue(D);
                this->findChild<QDoubleSpinBox*>(QString("sbMout_%1").arg(n))->setValue(mout);
                this->findChild<QComboBox*>(QString("cbRange_%1").arg(n))->setCurrentIndex(range);
                this->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->setValue(time);
            }
        }
    } catch (GenericException e) {
        qDebug()<<e.Report();
        return;
    }
}

void settings::accept()
{
    QString tempid = ui_settings.edTempId->text();
    QSettings f("settings.ini",QSettings::IniFormat);
    f.setValue("GPIB/tempid", tempid);
    qApp->setProperty("tempid",tempid);

    if (!tempid.isEmpty() && ui_settings.gbZones->isEnabled())
    {
        QSettings f("settings.ini",QSettings::IniFormat);

        tempctrl temp(qApp->property("tempid").toString());

        for (int n=0;n<=9;n++)
        {
            float t=this->findChild<QDoubleSpinBox*>(QString("sbTemp_%1").arg(n))->value();
            float p=this->findChild<QDoubleSpinBox*>(QString("sbP_%1").arg(n))->value();
            float i=this->findChild<QDoubleSpinBox*>(QString("sbI_%1").arg(n))->value();
            float d=this->findChild<QDoubleSpinBox*>(QString("sbD_%1").arg(n))->value();
            float mout=this->findChild<QDoubleSpinBox*>(QString("sbMout_%1").arg(n))->value();
            int range=this->findChild<QComboBox*>(QString("cbRange_%1").arg(n))->currentIndex();
            float time=this->findChild<QDoubleSpinBox*>(QString("sbTime_%1").arg(n))->value();
            temp.setzone(n+1,t,p,i,d,mout,range);
            f.setValue(QString("TempCtrl/Time_%1").arg(n),time);
        }
        temp.rst();
    }

    QDialog::accept();
}
