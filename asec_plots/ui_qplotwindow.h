/********************************************************************************
** Form generated from reading ui file 'qplotwindow.ui'
**
** Created: Thu 20. Nov 00:09:08 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QPLOTWINDOW_H
#define UI_QPLOTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_QPlotWindowClass
{
public:
    QVBoxLayout *verticalLayout;
    QwtPlot *qwtPlot;
    QPushButton *btHideTable;
    QTableWidget *twData;

    void setupUi(QWidget *QPlotWindowClass)
    {
    if (QPlotWindowClass->objectName().isEmpty())
        QPlotWindowClass->setObjectName(QString::fromUtf8("QPlotWindowClass"));
    QPlotWindowClass->resize(497, 346);
    verticalLayout = new QVBoxLayout(QPlotWindowClass);
    verticalLayout->setSpacing(6);
    verticalLayout->setMargin(11);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    qwtPlot = new QwtPlot(QPlotWindowClass);
    qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));

    verticalLayout->addWidget(qwtPlot);

    btHideTable = new QPushButton(QPlotWindowClass);
    btHideTable->setObjectName(QString::fromUtf8("btHideTable"));
    btHideTable->setCheckable(true);
    btHideTable->setChecked(true);

    verticalLayout->addWidget(btHideTable);

    twData = new QTableWidget(QPlotWindowClass);
    if (twData->columnCount() < 2)
        twData->setColumnCount(2);
    twData->setObjectName(QString::fromUtf8("twData"));
    twData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twData->setProperty("showDropIndicator", QVariant(false));
    twData->setDragDropOverwriteMode(false);
    twData->setColumnCount(2);

    verticalLayout->addWidget(twData);

    QWidget::setTabOrder(btHideTable, twData);

    retranslateUi(QPlotWindowClass);
    QObject::connect(btHideTable, SIGNAL(toggled(bool)), twData, SLOT(setVisible(bool)));

    QMetaObject::connectSlotsByName(QPlotWindowClass);
    } // setupUi

    void retranslateUi(QWidget *QPlotWindowClass)
    {
    QPlotWindowClass->setWindowTitle(QApplication::translate("QPlotWindowClass", "QPlotWindow", 0, QApplication::UnicodeUTF8));
    btHideTable->setText(QApplication::translate("QPlotWindowClass", "Table", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QPlotWindowClass);
    } // retranslateUi

};

namespace Ui {
    class QPlotWindowClass: public Ui_QPlotWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPLOTWINDOW_H
