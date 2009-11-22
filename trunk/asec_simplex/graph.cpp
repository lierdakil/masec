#include "graph.h"
#include "ui_graph.h"

Graph::Graph(QVector<qreal> X_exp, QVector<qreal> Y_exp,
             QVector<qreal> X_f, QVector<qreal> Y_f,
             double Rm, double Lm, double Cm ,double U,
             double C0, double R0,
             double fa, double Va,
             double fr, double Vr,
             bool useRm, bool useLm,
             bool useCm, bool useU,
             bool useC0, bool useR0,
             bool usesimplex) :
QDialog(0),
m_ui(new Ui::Graph)
{
    this->Va=Va;
    this->Vr=Vr;
    this->fa=fa;
    this->fr=fr;
    m_ui->setupUi(this);
    m_ui->cbFixRm->setChecked(useRm);
    m_ui->cbFixLm->setChecked(useLm);
    m_ui->cbFixCm->setChecked(useCm);
    m_ui->cbFixU->setChecked(useU);
    m_ui->cbFixC0->setChecked(useC0);
    m_ui->cbFixR0->setChecked(useR0);
    exp.setData(X_exp,Y_exp);
    if(usesimplex) fn.setData(X_f,Y_f);
    exp.attach(m_ui->qwtPlot);
    fn.attach(m_ui->qwtPlot);
    fn.setPen(QPen(Qt::red));
    exp.setPen(QPen(Qt::darkBlue));
    exp.setStyle(QwtPlotCurve::Dots);
    R.setLinePen(Qt::SolidLine);
    R.setLineStyle(QwtPlotMarker::Cross);
    R.setLabel(QwtText(QString("f_r = %1, V_r = %2").arg(fr).arg(Vr),QwtText::AutoText));
    R.setLabelAlignment(Qt::AlignRight | Qt::AlignBottom);
    R.attach(m_ui->qwtPlot);
    A.setLinePen(Qt::SolidLine);
    A.setLineStyle(QwtPlotMarker::Cross);
    A.setLabel(QwtText(QString("f_a = %1, V_a = %2").arg(fa).arg(Va),QwtText::AutoText));
    A.setLabelAlignment(Qt::AlignLeft | Qt::AlignTop);
    A.attach(m_ui->qwtPlot);

    if(usesimplex)
    {
        R.setValue(fr,Vr);
        A.setValue(fa,Va);
    }

    qwtPlotPicker=new QwtPlotPicker(QwtPlot::xBottom,QwtPlot::yLeft,
                                    QwtPlotPicker::PointSelection,
                                    QwtPlotPicker::CrossRubberBand,
                                    QwtPlotPicker::AlwaysOn,
                                    m_ui->qwtPlot->canvas());
    setres=true;

    connect(m_ui->sbLm,SIGNAL(valueChanged(double)),SLOT(sb_Lm_valueChanged(double)));

    m_ui->sbRm->setValue(fabs(Rm)*1e-3);
    m_ui->sbLm->setValue(fabs(Lm));
    m_ui->sbCm->setValue(fabs(Cm)*1e19);
    m_ui->sbU->setValue(fabs(U));
    m_ui->sbC0->setValue(fabs(C0)*1e14);
    m_ui->sbR0->setValue(fabs(R0));
    startf=X_f.first();
    endf=X_f.last();

    m_ui->qwtPlot->setBackgroundRole(QPalette::Light);
    m_ui->qwtPlot->replot();

    connect(m_ui->sbRm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbCm,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbU,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbC0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(m_ui->sbR0,SIGNAL(valueChanged(double)),SLOT(sb_valueChanged(double)));
    connect(qwtPlotPicker,SIGNAL(selected(QwtDoublePoint)),SLOT(plotSelected(QwtDoublePoint)));
}

Graph::~Graph()
{
    delete m_ui;
    delete qwtPlotPicker;
}

double Graph::Rm()
{
    return m_ui->sbRm->value()*1e3;
}

double Graph::Lm()
{
    return m_ui->sbLm->value();
}

double Graph::Cm()
{
    return m_ui->sbCm->value()*1e-19;
}

double Graph::U()
{
    return m_ui->sbU->value();
}

double Graph::C0()
{
    return m_ui->sbC0->value()*1e-14;
}

double Graph::R0()
{
    return m_ui->sbR0->value();
}

bool Graph::useRm()
{
    return m_ui->cbFixRm->isChecked();
}
bool Graph::useLm()
{
    return m_ui->cbFixLm->isChecked();
}
bool Graph::useCm()
{
    return m_ui->cbFixCm->isChecked();
}
bool Graph::useU()
{
    return m_ui->cbFixU->isChecked();
}
bool Graph::useC0()
{
    return m_ui->cbFixC0->isChecked();
}
bool Graph::useR0()
{
    return m_ui->cbFixR0->isChecked();
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

    Vr = If((const gsl_vector*)NULL, &func_params, startf);
    Va = Vr;
    fr = startf;
    fa = startf;

    QVector<qreal> X_f,Y_f;
    for(double f=startf; f<endf; ++f)
    {
        double I=If((const gsl_vector*)NULL, &func_params, f);
        X_f.push_back(f);
        Y_f.push_back(I);
        if(I>Vr)
        {
            Vr=I;
            fr=f;
        }
        if(I<Va)
        {
            Va=I;
            fa=f;
        }
    }
    fn.setData(X_f,Y_f);
    fn.setVisible(true);
    R.setValue(fr,Vr);
    R.setLabel(QwtText(QString("f_r = %1, V_r = %2").arg(fr).arg(Vr),QwtText::AutoText));
    A.setValue(fa,Va);
    A.setLabel(QwtText(QString("f_a = %1, V_a = %2").arg(fa).arg(Va),QwtText::AutoText));
    m_ui->qwtPlot->update();
    m_ui->qwtPlot->replot();
}

void Graph::plotSelected(QwtDoublePoint p)
{
    fn.setVisible(false);
    if(setres)
    {
        fr=p.x();
        Vr=p.y();
        R.setValue(fr,Vr);
        R.setLabel(QwtText(QString("f_r = %1, V_r = %2").arg(fr).arg(Vr),QwtText::AutoText));
    } else {
        fa=p.x();
        Va=p.y();
        A.setValue(fa,Va);
        A.setLabel(QwtText(QString("f_a = %1, V_a = %2").arg(fa).arg(Va),QwtText::AutoText));
    }
    setres=!setres;
    m_ui->qwtPlot->replot();
}

