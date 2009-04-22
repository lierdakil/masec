#include "graph.h"
#include "ui_graph.h"

Graph::Graph(QVector<qreal> X_exp, QVector<qreal> Y_exp,
             QVector<qreal> X_f, QVector<qreal> Y_f,
             double Rm, double Lm, double Cm ,double U,
                   double C0, double R0) :
    QWidget(0),
    m_ui(new Ui::Graph)
{
    m_ui->setupUi(this);
    exp.setData(X_exp,Y_exp);
    fn.setData(X_f,Y_f);
    exp.attach(m_ui->qwtPlot);
    fn.attach(m_ui->qwtPlot);
    fn.setPen(QPen(Qt::red));
    m_ui->qwtPlot->update();

    m_ui->sbRm->setValue(Rm);
    m_ui->sbLm->setValue(Lm);
    m_ui->sbCm->setValue(Cm*1e15);
    m_ui->sbU->setValue(U);
    m_ui->sbC0->setValue(C0*1e12);
    m_ui->sbR0->setValue(R0);
    startf=X_f.first();
    endf=X_f.last();

//    connect(m_ui->sbRm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
//    connect(m_ui->sbLm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
//    connect(m_ui->sbCm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
//    connect(m_ui->sbU,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
//    connect(m_ui->sbC0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
//    connect(m_ui->sbR0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
}

Graph::~Graph()
{
    delete m_ui;
}

void Graph::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Graph::sb_valueChanged(double val)
{
    double Rm=m_ui->sbRm->value();
    double Lm=m_ui->sbLm->value();
    double Cm=m_ui->sbCm->value()*pow(10,-17);
    double U=m_ui->sbU->value();
    double C0=m_ui->sbC0->value()*pow(10,-15);
    double R0=m_ui->sbR0->value();

    QVector<qreal> X_f,Y_f;
    for(double f=startf; f<endf; f++)
    {
        double w=2*3.1415926535*f;
        qreal If=U/sqrt((((pow(Cm,2)*pow(w,4)*pow(Rm,2)+pow(Cm,2)*pow(w,6)*pow(Lm,2)-2*Cm*pow(w,4)*Lm+pow(w,2))*pow(C0,2)+(2*Cm*pow(w,2)-2*pow(Cm,2)*pow(w,4)*Lm)*C0+pow(Cm,2)*pow(w,2))*pow(R0,2)+2*pow(Cm,2)*pow(w,2)*Rm*R0+pow(Cm,2)*pow(w,2)*pow(Rm,2)+pow(Cm,2)*pow(w,4)*pow(Lm,2)-2*Cm*pow(w,2)*Lm+1)/((pow(Cm,2)*pow(w,4)*pow(Rm,2)+pow(Cm,2)*pow(w,6)*pow(Lm,2)-2*Cm*pow(w,4)*Lm+pow(w,2))*pow(C0,2)+(2*Cm*pow(w,2)-2*pow(Cm,2)*pow(w,4)*Lm)*C0+pow(Cm,2)*pow(w,2)));
        X_f.push_back(f);
        Y_f.push_back(If);
    }
    fn.setData(X_f,Y_f);
    m_ui->qwtPlot->update();
    m_ui->qwtPlot->replot();
}

