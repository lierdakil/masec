/********************************************************************************
** Form generated from reading ui file 'vib_temperature.ui'
**
** Created: Wed 30. Apr 19:30:53 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VIB_TEMPERATURE_H
#define UI_VIB_TEMPERATURE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_vib_temperatureClass
{
public:
    QVBoxLayout *vboxLayout;
    QGraphicsView *gvTemp;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *btSettings;

    void setupUi(QWidget *vib_temperatureClass)
    {
    if (vib_temperatureClass->objectName().isEmpty())
        vib_temperatureClass->setObjectName(QString::fromUtf8("vib_temperatureClass"));
    vib_temperatureClass->resize(400, 335);
    vboxLayout = new QVBoxLayout(vib_temperatureClass);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    gvTemp = new QGraphicsView(vib_temperatureClass);
    gvTemp->setObjectName(QString::fromUtf8("gvTemp"));

    vboxLayout->addWidget(gvTemp);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    btSettings = new QPushButton(vib_temperatureClass);
    btSettings->setObjectName(QString::fromUtf8("btSettings"));

    hboxLayout->addWidget(btSettings);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(vib_temperatureClass);

    QMetaObject::connectSlotsByName(vib_temperatureClass);
    } // setupUi

    void retranslateUi(QWidget *vib_temperatureClass)
    {
    vib_temperatureClass->setWindowTitle(QApplication::translate("vib_temperatureClass", "vib_temperature", 0, QApplication::UnicodeUTF8));
    btSettings->setText(QApplication::translate("vib_temperatureClass", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(vib_temperatureClass);
    } // retranslateUi

};

namespace Ui {
    class vib_temperatureClass: public Ui_vib_temperatureClass {};
} // namespace Ui

#endif // UI_VIB_TEMPERATURE_H
