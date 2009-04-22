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

    connect(m_ui->sbLm,SIGNAL(valueChanged(double)),SLOT(sb_Lm_valueChanged(double)));

    m_ui->sbRm->setValue(Rm*1e-3);
    m_ui->sbLm->setValue(Lm);
    m_ui->sbCm->setValue(Cm*1e19);
    m_ui->sbU->setValue(U);
    m_ui->sbC0->setValue(C0*1e14);
    m_ui->sbR0->setValue(R0);
    startf=X_f.first();
    endf=X_f.last();

    connect(m_ui->sbRm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbCm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbU,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbC0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbR0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
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

void Graph::sb_Lm_valueChanged(double val)
{
    static double oldval;
    m_ui->sbCm->setValue(m_ui->sbCm->value()*oldval/val);
    oldval=val;
}

void Graph::sb_valueChanged(double val)
{
    param_struct func_params;

    func_params.data=0;
    func_params.f_min=startf;
    func_params.f_max=endf;
    func_params.RmU=0;
    func_params.LmU=0;
    func_params.CmU=0;
    func_params.C0U=0;
    func_params.UU=0;
    func_params.R0U=0;

    func_params.Rm=m_ui->sbRm->value()*1e3;
    func_params.Lm=m_ui->sbLm->value();
    func_params.Cm=m_ui->sbCm->value()*1e-19;
    func_params.C0=m_ui->sbC0->value()*1e-14;
    func_params.U=m_ui->sbU->value();
    func_params.R0=m_ui->sbR0->value();

    QVector<qreal> X_f,Y_f;
    for(double f=startf; f<endf; ++f)
    {
        func_params.f=f;
        double I=If((const gsl_vector*)NULL, &func_params);
        X_f.push_back(f);
        Y_f.push_back(I);
    }
    fn.setData(X_f,Y_f);
    m_ui->qwtPlot->update();
    m_ui->qwtPlot->replot();
}

