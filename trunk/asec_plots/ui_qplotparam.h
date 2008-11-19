/********************************************************************************
** Form generated from reading ui file 'qplotparam.ui'
**
** Created: Thu 20. Nov 00:09:08 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QPLOTPARAM_H
#define UI_QPLOTPARAM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QPlotParamClass
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *lbName;
    QLineEdit *edName;
    QLabel *lbX;
    QComboBox *cbX;
    QLabel *label_3;
    QComboBox *cbY;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *QPlotParamClass)
    {
    if (QPlotParamClass->objectName().isEmpty())
        QPlotParamClass->setObjectName(QString::fromUtf8("QPlotParamClass"));
    QPlotParamClass->resize(400, 123);
    verticalLayout = new QVBoxLayout(QPlotParamClass);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    lbName = new QLabel(QPlotParamClass);
    lbName->setObjectName(QString::fromUtf8("lbName"));

    gridLayout->addWidget(lbName, 0, 0, 1, 1);

    edName = new QLineEdit(QPlotParamClass);
    edName->setObjectName(QString::fromUtf8("edName"));

    gridLayout->addWidget(edName, 0, 1, 1, 1);

    lbX = new QLabel(QPlotParamClass);
    lbX->setObjectName(QString::fromUtf8("lbX"));

    gridLayout->addWidget(lbX, 1, 0, 1, 1);

    cbX = new QComboBox(QPlotParamClass);
    cbX->setObjectName(QString::fromUtf8("cbX"));

    gridLayout->addWidget(cbX, 1, 1, 1, 1);

    label_3 = new QLabel(QPlotParamClass);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    cbY = new QComboBox(QPlotParamClass);
    cbY->setObjectName(QString::fromUtf8("cbY"));

    gridLayout->addWidget(cbY, 2, 1, 1, 1);


    verticalLayout->addLayout(gridLayout);

    buttonBox = new QDialogButtonBox(QPlotParamClass);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    verticalLayout->addWidget(buttonBox);


    retranslateUi(QPlotParamClass);
    QObject::connect(buttonBox, SIGNAL(accepted()), QPlotParamClass, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), QPlotParamClass, SLOT(reject()));

    QMetaObject::connectSlotsByName(QPlotParamClass);
    } // setupUi

    void retranslateUi(QDialog *QPlotParamClass)
    {
    QPlotParamClass->setWindowTitle(QApplication::translate("QPlotParamClass", "Dialog", 0, QApplication::UnicodeUTF8));
    lbName->setText(QApplication::translate("QPlotParamClass", "Plot Name", 0, QApplication::UnicodeUTF8));
    lbX->setText(QApplication::translate("QPlotParamClass", "X variable", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("QPlotParamClass", "Y variable", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QPlotParamClass);
    } // retranslateUi

};

namespace Ui {
    class QPlotParamClass: public Ui_QPlotParamClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QPLOTPARAM_H
