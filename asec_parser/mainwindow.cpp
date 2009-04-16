#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QString filename, QWidget *parent) :
        QWidget(parent),
        m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly))
    {
        QString line;
        QStringList header;
        QStringList comment;

        //build header
        int rowCount=0;
        while(!f.atEnd())
        {
            line = QString::fromUtf8(f.readLine());
            while (line.startsWith('#')){
                line = QString::fromUtf8(f.readLine());
            }
            ++rowCount;
            QStringList dataline=line.split(";");
            foreach(QString entry, dataline)
            {
                QString name=entry.split(":").at(0);
                if(!header.contains(name))
                    header<<name;
            }
        }

        //output header
        m_ui->twData->setColumnCount(header.count());
        m_ui->twData->setRowCount(rowCount);
        for(int i=0; i<header.count(); ++i)
            m_ui->twData->setHorizontalHeaderItem(i, new QTableWidgetItem(header.at(i)));

        f.seek(0);
        int curRow=0;
        while(!f.atEnd())
        {
            line = QString::fromUtf8(f.readLine());
            while (line.startsWith('#')){
                comment<<line;
                line = QString::fromUtf8(f.readLine());
            }
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

            for(int i=0;i<row.count();++i)
                m_ui->twData->setItem(curRow,i, new QTableWidgetItem(row[i]));
            curRow++;
        }
        m_ui->teComment->setPlainText(comment.join(""));
        f.close();
    }
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
