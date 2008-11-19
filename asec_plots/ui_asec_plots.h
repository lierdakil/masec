/********************************************************************************
** Form generated from reading ui file 'asec_plots.ui'
**
** Created: Thu 20. Nov 00:09:08 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASEC_PLOTS_H
#define UI_ASEC_PLOTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_asec_plotsClass
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *lwPlots;
    QHBoxLayout *horizontalLayout;
    QPushButton *btAddPlot;
    QPushButton *btDelPlot;
    QPushButton *btEditPlot;

    void setupUi(QWidget *asec_plotsClass)
    {
    if (asec_plotsClass->objectName().isEmpty())
        asec_plotsClass->setObjectName(QString::fromUtf8("asec_plotsClass"));
    asec_plotsClass->resize(400, 300);
    verticalLayout = new QVBoxLayout(asec_plotsClass);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    lwPlots = new QListWidget(asec_plotsClass);
    lwPlots->setObjectName(QString::fromUtf8("lwPlots"));

    verticalLayout->addWidget(lwPlots);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(6);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    btAddPlot = new QPushButton(asec_plotsClass);
    btAddPlot->setObjectName(QString::fromUtf8("btAddPlot"));

    horizontalLayout->addWidget(btAddPlot);

    btDelPlot = new QPushButton(asec_plotsClass);
    btDelPlot->setObjectName(QString::fromUtf8("btDelPlot"));

    horizontalLayout->addWidget(btDelPlot);

    btEditPlot = new QPushButton(asec_plotsClass);
    btEditPlot->setObjectName(QString::fromUtf8("btEditPlot"));

    horizontalLayout->addWidget(btEditPlot);


    verticalLayout->addLayout(horizontalLayout);


    retranslateUi(asec_plotsClass);

    QMetaObject::connectSlotsByName(asec_plotsClass);
    } // setupUi

    void retranslateUi(QWidget *asec_plotsClass)
    {
    asec_plotsClass->setWindowTitle(QApplication::translate("asec_plotsClass", "asec_plots", 0, QApplication::UnicodeUTF8));
    btAddPlot->setText(QApplication::translate("asec_plotsClass", "Add Plot", 0, QApplication::UnicodeUTF8));
    btDelPlot->setText(QApplication::translate("asec_plotsClass", "Delete Plot", 0, QApplication::UnicodeUTF8));
    btEditPlot->setText(QApplication::translate("asec_plotsClass", "Edit Plot", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(asec_plotsClass);
    } // retranslateUi

};

namespace Ui {
    class asec_plotsClass: public Ui_asec_plotsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASEC_PLOTS_H
