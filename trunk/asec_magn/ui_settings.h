/********************************************************************************
** Form generated from reading ui file 'settings.ui'
**
** Created: Mon 1. Dec 17:12:56 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabWidget;
    QWidget *tbTempCtrl;
    QVBoxLayout *vboxLayout1;
    QGroupBox *gbGPIB;
    QHBoxLayout *hboxLayout;
    QLabel *lbTempId;
    QLineEdit *edTempId;
    QSpacerItem *spacerItem;
    QGroupBox *gbZones;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QDoubleSpinBox *sbTemp_0;
    QDoubleSpinBox *sbP_0;
    QDoubleSpinBox *sbI_0;
    QDoubleSpinBox *sbD_0;
    QDoubleSpinBox *sbMout_0;
    QComboBox *cbRange_0;
    QDoubleSpinBox *sbTime_0;
    QDoubleSpinBox *sbTemp_1;
    QDoubleSpinBox *sbP_1;
    QDoubleSpinBox *sbI_1;
    QDoubleSpinBox *sbD_1;
    QDoubleSpinBox *sbMout_1;
    QComboBox *cbRange_1;
    QDoubleSpinBox *sbTime_1;
    QDoubleSpinBox *sbTemp_2;
    QDoubleSpinBox *sbP_2;
    QDoubleSpinBox *sbI_2;
    QDoubleSpinBox *sbD_2;
    QDoubleSpinBox *sbMout_2;
    QComboBox *cbRange_2;
    QDoubleSpinBox *sbTime_2;
    QDoubleSpinBox *sbTemp_3;
    QDoubleSpinBox *sbP_3;
    QDoubleSpinBox *sbI_3;
    QDoubleSpinBox *sbD_3;
    QDoubleSpinBox *sbMout_3;
    QComboBox *cbRange_3;
    QDoubleSpinBox *sbTime_3;
    QDoubleSpinBox *sbTemp_4;
    QDoubleSpinBox *sbP_4;
    QDoubleSpinBox *sbI_4;
    QDoubleSpinBox *sbD_4;
    QDoubleSpinBox *sbMout_4;
    QComboBox *cbRange_4;
    QDoubleSpinBox *sbTime_4;
    QDoubleSpinBox *sbTemp_5;
    QDoubleSpinBox *sbP_5;
    QDoubleSpinBox *sbI_5;
    QDoubleSpinBox *sbD_5;
    QDoubleSpinBox *sbMout_5;
    QComboBox *cbRange_5;
    QDoubleSpinBox *sbTime_5;
    QDoubleSpinBox *sbTemp_6;
    QDoubleSpinBox *sbP_6;
    QDoubleSpinBox *sbI_6;
    QDoubleSpinBox *sbD_6;
    QDoubleSpinBox *sbMout_6;
    QComboBox *cbRange_6;
    QDoubleSpinBox *sbTime_6;
    QDoubleSpinBox *sbTemp_7;
    QDoubleSpinBox *sbP_7;
    QDoubleSpinBox *sbI_7;
    QDoubleSpinBox *sbD_7;
    QDoubleSpinBox *sbMout_7;
    QComboBox *cbRange_7;
    QDoubleSpinBox *sbTime_7;
    QDoubleSpinBox *sbTemp_8;
    QDoubleSpinBox *sbP_8;
    QDoubleSpinBox *sbI_8;
    QDoubleSpinBox *sbD_8;
    QDoubleSpinBox *sbMout_8;
    QComboBox *cbRange_8;
    QDoubleSpinBox *sbTime_8;
    QDoubleSpinBox *sbTemp_9;
    QDoubleSpinBox *sbP_9;
    QDoubleSpinBox *sbI_9;
    QDoubleSpinBox *sbD_9;
    QDoubleSpinBox *sbMout_9;
    QComboBox *cbRange_9;
    QDoubleSpinBox *sbTime_9;
    QSpacerItem *spacerItem1;
    QWidget *tbTempTest;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel *lbT_test;
    QLCDNumber *nlT_test;
    QGraphicsView *gvGraph;
    QHBoxLayout *hboxLayout2;
    QLabel *lbNewT_test;
    QDoubleSpinBox *sbNewT_test;
    QHBoxLayout *hboxLayout3;
    QLabel *lbP_test;
    QDoubleSpinBox *sbP_test;
    QHBoxLayout *hboxLayout4;
    QLabel *lbI_test;
    QDoubleSpinBox *sbI_test;
    QHBoxLayout *hboxLayout5;
    QLabel *lbD_test;
    QDoubleSpinBox *sbD_test;
    QHBoxLayout *hboxLayout6;
    QLabel *lbMout_test;
    QDoubleSpinBox *sbMout_test;
    QHBoxLayout *hboxLayout7;
    QLabel *lbRange_test;
    QComboBox *cbRange_test;
    QHBoxLayout *hboxLayout8;
    QLabel *lbt_test;
    QDoubleSpinBox *sbt_test;
    QHBoxLayout *hboxLayout9;
    QLabel *lbRamp;
    QDoubleSpinBox *sbRamp;
    QPushButton *btTest;
    QPushButton *btStopTest;
    QSpacerItem *spacerItem2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Settings)
    {
    if (Settings->objectName().isEmpty())
        Settings->setObjectName(QString::fromUtf8("Settings"));
    Settings->setWindowModality(Qt::ApplicationModal);
    Settings->resize(684, 472);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(Settings->sizePolicy().hasHeightForWidth());
    Settings->setSizePolicy(sizePolicy);
    Settings->setSizeGripEnabled(false);
    Settings->setModal(true);
    vboxLayout = new QVBoxLayout(Settings);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    tabWidget = new QTabWidget(Settings);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setTabShape(QTabWidget::Rounded);
    tbTempCtrl = new QWidget();
    tbTempCtrl->setObjectName(QString::fromUtf8("tbTempCtrl"));
    vboxLayout1 = new QVBoxLayout(tbTempCtrl);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    gbGPIB = new QGroupBox(tbTempCtrl);
    gbGPIB->setObjectName(QString::fromUtf8("gbGPIB"));
    hboxLayout = new QHBoxLayout(gbGPIB);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    lbTempId = new QLabel(gbGPIB);
    lbTempId->setObjectName(QString::fromUtf8("lbTempId"));

    hboxLayout->addWidget(lbTempId);

    edTempId = new QLineEdit(gbGPIB);
    edTempId->setObjectName(QString::fromUtf8("edTempId"));

    hboxLayout->addWidget(edTempId);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);


    vboxLayout1->addWidget(gbGPIB);

    gbZones = new QGroupBox(tbTempCtrl);
    gbZones->setObjectName(QString::fromUtf8("gbZones"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(gbZones->sizePolicy().hasHeightForWidth());
    gbZones->setSizePolicy(sizePolicy1);
    gridLayout = new QGridLayout(gbZones);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(gbZones);
    label->setObjectName(QString::fromUtf8("label"));
    label->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(gbZones);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setScaledContents(false);
    label_2->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_2, 0, 1, 1, 1);

    label_3 = new QLabel(gbZones);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    label_3->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_3, 0, 2, 1, 1);

    label_4 = new QLabel(gbZones);
    label_4->setObjectName(QString::fromUtf8("label_4"));
    label_4->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_4, 0, 3, 1, 1);

    label_5 = new QLabel(gbZones);
    label_5->setObjectName(QString::fromUtf8("label_5"));
    label_5->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_5, 0, 4, 1, 1);

    label_6 = new QLabel(gbZones);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setAlignment(Qt::AlignCenter);

    gridLayout->addWidget(label_6, 0, 5, 1, 1);

    label_7 = new QLabel(gbZones);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 0, 6, 1, 1);

    sbTemp_0 = new QDoubleSpinBox(gbZones);
    sbTemp_0->setObjectName(QString::fromUtf8("sbTemp_0"));
    sbTemp_0->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_0, 1, 0, 1, 1);

    sbP_0 = new QDoubleSpinBox(gbZones);
    sbP_0->setObjectName(QString::fromUtf8("sbP_0"));
    sbP_0->setMinimum(0.1);
    sbP_0->setMaximum(1000);

    gridLayout->addWidget(sbP_0, 1, 1, 1, 1);

    sbI_0 = new QDoubleSpinBox(gbZones);
    sbI_0->setObjectName(QString::fromUtf8("sbI_0"));
    sbI_0->setMinimum(0.1);
    sbI_0->setMaximum(1000);

    gridLayout->addWidget(sbI_0, 1, 2, 1, 1);

    sbD_0 = new QDoubleSpinBox(gbZones);
    sbD_0->setObjectName(QString::fromUtf8("sbD_0"));
    sbD_0->setMaximum(200);

    gridLayout->addWidget(sbD_0, 1, 3, 1, 1);

    sbMout_0 = new QDoubleSpinBox(gbZones);
    sbMout_0->setObjectName(QString::fromUtf8("sbMout_0"));
    sbMout_0->setMaximum(100);

    gridLayout->addWidget(sbMout_0, 1, 4, 1, 1);

    cbRange_0 = new QComboBox(gbZones);
    cbRange_0->setObjectName(QString::fromUtf8("cbRange_0"));
    cbRange_0->setEnabled(true);

    gridLayout->addWidget(cbRange_0, 1, 5, 1, 1);

    sbTime_0 = new QDoubleSpinBox(gbZones);
    sbTime_0->setObjectName(QString::fromUtf8("sbTime_0"));

    gridLayout->addWidget(sbTime_0, 1, 6, 1, 1);

    sbTemp_1 = new QDoubleSpinBox(gbZones);
    sbTemp_1->setObjectName(QString::fromUtf8("sbTemp_1"));
    sbTemp_1->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_1, 2, 0, 1, 1);

    sbP_1 = new QDoubleSpinBox(gbZones);
    sbP_1->setObjectName(QString::fromUtf8("sbP_1"));
    sbP_1->setMinimum(0.1);
    sbP_1->setMaximum(1000);

    gridLayout->addWidget(sbP_1, 2, 1, 1, 1);

    sbI_1 = new QDoubleSpinBox(gbZones);
    sbI_1->setObjectName(QString::fromUtf8("sbI_1"));
    sbI_1->setMinimum(0.1);
    sbI_1->setMaximum(1000);

    gridLayout->addWidget(sbI_1, 2, 2, 1, 1);

    sbD_1 = new QDoubleSpinBox(gbZones);
    sbD_1->setObjectName(QString::fromUtf8("sbD_1"));
    sbD_1->setMaximum(200);

    gridLayout->addWidget(sbD_1, 2, 3, 1, 1);

    sbMout_1 = new QDoubleSpinBox(gbZones);
    sbMout_1->setObjectName(QString::fromUtf8("sbMout_1"));
    sbMout_1->setMaximum(100);

    gridLayout->addWidget(sbMout_1, 2, 4, 1, 1);

    cbRange_1 = new QComboBox(gbZones);
    cbRange_1->setObjectName(QString::fromUtf8("cbRange_1"));
    cbRange_1->setEnabled(true);

    gridLayout->addWidget(cbRange_1, 2, 5, 1, 1);

    sbTime_1 = new QDoubleSpinBox(gbZones);
    sbTime_1->setObjectName(QString::fromUtf8("sbTime_1"));

    gridLayout->addWidget(sbTime_1, 2, 6, 1, 1);

    sbTemp_2 = new QDoubleSpinBox(gbZones);
    sbTemp_2->setObjectName(QString::fromUtf8("sbTemp_2"));
    sbTemp_2->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_2, 3, 0, 1, 1);

    sbP_2 = new QDoubleSpinBox(gbZones);
    sbP_2->setObjectName(QString::fromUtf8("sbP_2"));
    sbP_2->setMinimum(0.1);
    sbP_2->setMaximum(1000);

    gridLayout->addWidget(sbP_2, 3, 1, 1, 1);

    sbI_2 = new QDoubleSpinBox(gbZones);
    sbI_2->setObjectName(QString::fromUtf8("sbI_2"));
    sbI_2->setMinimum(0.1);
    sbI_2->setMaximum(1000);

    gridLayout->addWidget(sbI_2, 3, 2, 1, 1);

    sbD_2 = new QDoubleSpinBox(gbZones);
    sbD_2->setObjectName(QString::fromUtf8("sbD_2"));
    sbD_2->setMaximum(200);

    gridLayout->addWidget(sbD_2, 3, 3, 1, 1);

    sbMout_2 = new QDoubleSpinBox(gbZones);
    sbMout_2->setObjectName(QString::fromUtf8("sbMout_2"));
    sbMout_2->setMaximum(100);

    gridLayout->addWidget(sbMout_2, 3, 4, 1, 1);

    cbRange_2 = new QComboBox(gbZones);
    cbRange_2->setObjectName(QString::fromUtf8("cbRange_2"));
    cbRange_2->setEnabled(true);

    gridLayout->addWidget(cbRange_2, 3, 5, 1, 1);

    sbTime_2 = new QDoubleSpinBox(gbZones);
    sbTime_2->setObjectName(QString::fromUtf8("sbTime_2"));

    gridLayout->addWidget(sbTime_2, 3, 6, 1, 1);

    sbTemp_3 = new QDoubleSpinBox(gbZones);
    sbTemp_3->setObjectName(QString::fromUtf8("sbTemp_3"));
    sbTemp_3->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_3, 4, 0, 1, 1);

    sbP_3 = new QDoubleSpinBox(gbZones);
    sbP_3->setObjectName(QString::fromUtf8("sbP_3"));
    sbP_3->setMinimum(0.1);
    sbP_3->setMaximum(1000);

    gridLayout->addWidget(sbP_3, 4, 1, 1, 1);

    sbI_3 = new QDoubleSpinBox(gbZones);
    sbI_3->setObjectName(QString::fromUtf8("sbI_3"));
    sbI_3->setMinimum(0.1);
    sbI_3->setMaximum(1000);

    gridLayout->addWidget(sbI_3, 4, 2, 1, 1);

    sbD_3 = new QDoubleSpinBox(gbZones);
    sbD_3->setObjectName(QString::fromUtf8("sbD_3"));
    sbD_3->setMaximum(200);

    gridLayout->addWidget(sbD_3, 4, 3, 1, 1);

    sbMout_3 = new QDoubleSpinBox(gbZones);
    sbMout_3->setObjectName(QString::fromUtf8("sbMout_3"));
    sbMout_3->setMaximum(100);

    gridLayout->addWidget(sbMout_3, 4, 4, 1, 1);

    cbRange_3 = new QComboBox(gbZones);
    cbRange_3->setObjectName(QString::fromUtf8("cbRange_3"));
    cbRange_3->setEnabled(true);

    gridLayout->addWidget(cbRange_3, 4, 5, 1, 1);

    sbTime_3 = new QDoubleSpinBox(gbZones);
    sbTime_3->setObjectName(QString::fromUtf8("sbTime_3"));

    gridLayout->addWidget(sbTime_3, 4, 6, 1, 1);

    sbTemp_4 = new QDoubleSpinBox(gbZones);
    sbTemp_4->setObjectName(QString::fromUtf8("sbTemp_4"));
    sbTemp_4->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_4, 5, 0, 1, 1);

    sbP_4 = new QDoubleSpinBox(gbZones);
    sbP_4->setObjectName(QString::fromUtf8("sbP_4"));
    sbP_4->setMinimum(0.1);
    sbP_4->setMaximum(1000);

    gridLayout->addWidget(sbP_4, 5, 1, 1, 1);

    sbI_4 = new QDoubleSpinBox(gbZones);
    sbI_4->setObjectName(QString::fromUtf8("sbI_4"));
    sbI_4->setMinimum(0.1);
    sbI_4->setMaximum(1000);

    gridLayout->addWidget(sbI_4, 5, 2, 1, 1);

    sbD_4 = new QDoubleSpinBox(gbZones);
    sbD_4->setObjectName(QString::fromUtf8("sbD_4"));
    sbD_4->setMaximum(200);

    gridLayout->addWidget(sbD_4, 5, 3, 1, 1);

    sbMout_4 = new QDoubleSpinBox(gbZones);
    sbMout_4->setObjectName(QString::fromUtf8("sbMout_4"));
    sbMout_4->setMaximum(100);

    gridLayout->addWidget(sbMout_4, 5, 4, 1, 1);

    cbRange_4 = new QComboBox(gbZones);
    cbRange_4->setObjectName(QString::fromUtf8("cbRange_4"));
    cbRange_4->setEnabled(true);

    gridLayout->addWidget(cbRange_4, 5, 5, 1, 1);

    sbTime_4 = new QDoubleSpinBox(gbZones);
    sbTime_4->setObjectName(QString::fromUtf8("sbTime_4"));

    gridLayout->addWidget(sbTime_4, 5, 6, 1, 1);

    sbTemp_5 = new QDoubleSpinBox(gbZones);
    sbTemp_5->setObjectName(QString::fromUtf8("sbTemp_5"));
    sbTemp_5->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_5, 6, 0, 1, 1);

    sbP_5 = new QDoubleSpinBox(gbZones);
    sbP_5->setObjectName(QString::fromUtf8("sbP_5"));
    sbP_5->setMinimum(0.1);
    sbP_5->setMaximum(1000);

    gridLayout->addWidget(sbP_5, 6, 1, 1, 1);

    sbI_5 = new QDoubleSpinBox(gbZones);
    sbI_5->setObjectName(QString::fromUtf8("sbI_5"));
    sbI_5->setMinimum(0.1);
    sbI_5->setMaximum(1000);

    gridLayout->addWidget(sbI_5, 6, 2, 1, 1);

    sbD_5 = new QDoubleSpinBox(gbZones);
    sbD_5->setObjectName(QString::fromUtf8("sbD_5"));
    sbD_5->setMaximum(200);

    gridLayout->addWidget(sbD_5, 6, 3, 1, 1);

    sbMout_5 = new QDoubleSpinBox(gbZones);
    sbMout_5->setObjectName(QString::fromUtf8("sbMout_5"));
    sbMout_5->setMaximum(100);

    gridLayout->addWidget(sbMout_5, 6, 4, 1, 1);

    cbRange_5 = new QComboBox(gbZones);
    cbRange_5->setObjectName(QString::fromUtf8("cbRange_5"));
    cbRange_5->setEnabled(true);

    gridLayout->addWidget(cbRange_5, 6, 5, 1, 1);

    sbTime_5 = new QDoubleSpinBox(gbZones);
    sbTime_5->setObjectName(QString::fromUtf8("sbTime_5"));

    gridLayout->addWidget(sbTime_5, 6, 6, 1, 1);

    sbTemp_6 = new QDoubleSpinBox(gbZones);
    sbTemp_6->setObjectName(QString::fromUtf8("sbTemp_6"));
    sbTemp_6->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_6, 7, 0, 1, 1);

    sbP_6 = new QDoubleSpinBox(gbZones);
    sbP_6->setObjectName(QString::fromUtf8("sbP_6"));
    sbP_6->setMinimum(0.1);
    sbP_6->setMaximum(1000);

    gridLayout->addWidget(sbP_6, 7, 1, 1, 1);

    sbI_6 = new QDoubleSpinBox(gbZones);
    sbI_6->setObjectName(QString::fromUtf8("sbI_6"));
    sbI_6->setMinimum(0.1);
    sbI_6->setMaximum(1000);

    gridLayout->addWidget(sbI_6, 7, 2, 1, 1);

    sbD_6 = new QDoubleSpinBox(gbZones);
    sbD_6->setObjectName(QString::fromUtf8("sbD_6"));
    sbD_6->setMaximum(200);

    gridLayout->addWidget(sbD_6, 7, 3, 1, 1);

    sbMout_6 = new QDoubleSpinBox(gbZones);
    sbMout_6->setObjectName(QString::fromUtf8("sbMout_6"));
    sbMout_6->setMaximum(100);

    gridLayout->addWidget(sbMout_6, 7, 4, 1, 1);

    cbRange_6 = new QComboBox(gbZones);
    cbRange_6->setObjectName(QString::fromUtf8("cbRange_6"));
    cbRange_6->setEnabled(true);

    gridLayout->addWidget(cbRange_6, 7, 5, 1, 1);

    sbTime_6 = new QDoubleSpinBox(gbZones);
    sbTime_6->setObjectName(QString::fromUtf8("sbTime_6"));

    gridLayout->addWidget(sbTime_6, 7, 6, 1, 1);

    sbTemp_7 = new QDoubleSpinBox(gbZones);
    sbTemp_7->setObjectName(QString::fromUtf8("sbTemp_7"));
    sbTemp_7->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_7, 8, 0, 1, 1);

    sbP_7 = new QDoubleSpinBox(gbZones);
    sbP_7->setObjectName(QString::fromUtf8("sbP_7"));
    sbP_7->setMinimum(0.1);
    sbP_7->setMaximum(1000);

    gridLayout->addWidget(sbP_7, 8, 1, 1, 1);

    sbI_7 = new QDoubleSpinBox(gbZones);
    sbI_7->setObjectName(QString::fromUtf8("sbI_7"));
    sbI_7->setMinimum(0.1);
    sbI_7->setMaximum(1000);

    gridLayout->addWidget(sbI_7, 8, 2, 1, 1);

    sbD_7 = new QDoubleSpinBox(gbZones);
    sbD_7->setObjectName(QString::fromUtf8("sbD_7"));
    sbD_7->setMaximum(200);

    gridLayout->addWidget(sbD_7, 8, 3, 1, 1);

    sbMout_7 = new QDoubleSpinBox(gbZones);
    sbMout_7->setObjectName(QString::fromUtf8("sbMout_7"));
    sbMout_7->setMaximum(100);

    gridLayout->addWidget(sbMout_7, 8, 4, 1, 1);

    cbRange_7 = new QComboBox(gbZones);
    cbRange_7->setObjectName(QString::fromUtf8("cbRange_7"));
    cbRange_7->setEnabled(true);

    gridLayout->addWidget(cbRange_7, 8, 5, 1, 1);

    sbTime_7 = new QDoubleSpinBox(gbZones);
    sbTime_7->setObjectName(QString::fromUtf8("sbTime_7"));

    gridLayout->addWidget(sbTime_7, 8, 6, 1, 1);

    sbTemp_8 = new QDoubleSpinBox(gbZones);
    sbTemp_8->setObjectName(QString::fromUtf8("sbTemp_8"));
    sbTemp_8->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_8, 9, 0, 1, 1);

    sbP_8 = new QDoubleSpinBox(gbZones);
    sbP_8->setObjectName(QString::fromUtf8("sbP_8"));
    sbP_8->setMinimum(0.1);
    sbP_8->setMaximum(1000);

    gridLayout->addWidget(sbP_8, 9, 1, 1, 1);

    sbI_8 = new QDoubleSpinBox(gbZones);
    sbI_8->setObjectName(QString::fromUtf8("sbI_8"));
    sbI_8->setMinimum(0.1);
    sbI_8->setMaximum(1000);

    gridLayout->addWidget(sbI_8, 9, 2, 1, 1);

    sbD_8 = new QDoubleSpinBox(gbZones);
    sbD_8->setObjectName(QString::fromUtf8("sbD_8"));
    sbD_8->setMaximum(200);

    gridLayout->addWidget(sbD_8, 9, 3, 1, 1);

    sbMout_8 = new QDoubleSpinBox(gbZones);
    sbMout_8->setObjectName(QString::fromUtf8("sbMout_8"));
    sbMout_8->setMaximum(100);

    gridLayout->addWidget(sbMout_8, 9, 4, 1, 1);

    cbRange_8 = new QComboBox(gbZones);
    cbRange_8->setObjectName(QString::fromUtf8("cbRange_8"));
    cbRange_8->setEnabled(true);

    gridLayout->addWidget(cbRange_8, 9, 5, 1, 1);

    sbTime_8 = new QDoubleSpinBox(gbZones);
    sbTime_8->setObjectName(QString::fromUtf8("sbTime_8"));

    gridLayout->addWidget(sbTime_8, 9, 6, 1, 1);

    sbTemp_9 = new QDoubleSpinBox(gbZones);
    sbTemp_9->setObjectName(QString::fromUtf8("sbTemp_9"));
    sbTemp_9->setMaximum(1e+10);

    gridLayout->addWidget(sbTemp_9, 10, 0, 1, 1);

    sbP_9 = new QDoubleSpinBox(gbZones);
    sbP_9->setObjectName(QString::fromUtf8("sbP_9"));
    sbP_9->setMinimum(0.1);
    sbP_9->setMaximum(1000);

    gridLayout->addWidget(sbP_9, 10, 1, 1, 1);

    sbI_9 = new QDoubleSpinBox(gbZones);
    sbI_9->setObjectName(QString::fromUtf8("sbI_9"));
    sbI_9->setMinimum(0.1);
    sbI_9->setMaximum(1000);

    gridLayout->addWidget(sbI_9, 10, 2, 1, 1);

    sbD_9 = new QDoubleSpinBox(gbZones);
    sbD_9->setObjectName(QString::fromUtf8("sbD_9"));
    sbD_9->setMaximum(200);

    gridLayout->addWidget(sbD_9, 10, 3, 1, 1);

    sbMout_9 = new QDoubleSpinBox(gbZones);
    sbMout_9->setObjectName(QString::fromUtf8("sbMout_9"));
    sbMout_9->setMaximum(100);

    gridLayout->addWidget(sbMout_9, 10, 4, 1, 1);

    cbRange_9 = new QComboBox(gbZones);
    cbRange_9->setObjectName(QString::fromUtf8("cbRange_9"));
    cbRange_9->setEnabled(true);

    gridLayout->addWidget(cbRange_9, 10, 5, 1, 1);

    sbTime_9 = new QDoubleSpinBox(gbZones);
    sbTime_9->setObjectName(QString::fromUtf8("sbTime_9"));

    gridLayout->addWidget(sbTime_9, 10, 6, 1, 1);


    vboxLayout1->addWidget(gbZones);

    spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem1);

    tabWidget->addTab(tbTempCtrl, QString());
    tbTempTest = new QWidget();
    tbTempTest->setObjectName(QString::fromUtf8("tbTempTest"));
    gridLayout1 = new QGridLayout(tbTempTest);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    lbT_test = new QLabel(tbTempTest);
    lbT_test->setObjectName(QString::fromUtf8("lbT_test"));

    hboxLayout1->addWidget(lbT_test);

    nlT_test = new QLCDNumber(tbTempTest);
    nlT_test->setObjectName(QString::fromUtf8("nlT_test"));
    QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(nlT_test->sizePolicy().hasHeightForWidth());
    nlT_test->setSizePolicy(sizePolicy2);
    nlT_test->setFrameShape(QFrame::NoFrame);
    nlT_test->setFrameShadow(QFrame::Plain);
    nlT_test->setMidLineWidth(1);
    nlT_test->setSmallDecimalPoint(true);
    nlT_test->setSegmentStyle(QLCDNumber::Flat);

    hboxLayout1->addWidget(nlT_test);


    gridLayout1->addLayout(hboxLayout1, 0, 0, 1, 1);

    gvGraph = new QGraphicsView(tbTempTest);
    gvGraph->setObjectName(QString::fromUtf8("gvGraph"));

    gridLayout1->addWidget(gvGraph, 0, 1, 12, 1);

    hboxLayout2 = new QHBoxLayout();
    hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
    lbNewT_test = new QLabel(tbTempTest);
    lbNewT_test->setObjectName(QString::fromUtf8("lbNewT_test"));

    hboxLayout2->addWidget(lbNewT_test);

    sbNewT_test = new QDoubleSpinBox(tbTempTest);
    sbNewT_test->setObjectName(QString::fromUtf8("sbNewT_test"));
    sbNewT_test->setMaximum(100000);

    hboxLayout2->addWidget(sbNewT_test);


    gridLayout1->addLayout(hboxLayout2, 1, 0, 1, 1);

    hboxLayout3 = new QHBoxLayout();
    hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
    lbP_test = new QLabel(tbTempTest);
    lbP_test->setObjectName(QString::fromUtf8("lbP_test"));
    lbP_test->setAlignment(Qt::AlignCenter);

    hboxLayout3->addWidget(lbP_test);

    sbP_test = new QDoubleSpinBox(tbTempTest);
    sbP_test->setObjectName(QString::fromUtf8("sbP_test"));
    sbP_test->setMinimum(0.1);
    sbP_test->setMaximum(1000);

    hboxLayout3->addWidget(sbP_test);


    gridLayout1->addLayout(hboxLayout3, 2, 0, 1, 1);

    hboxLayout4 = new QHBoxLayout();
    hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
    lbI_test = new QLabel(tbTempTest);
    lbI_test->setObjectName(QString::fromUtf8("lbI_test"));
    lbI_test->setAlignment(Qt::AlignCenter);

    hboxLayout4->addWidget(lbI_test);

    sbI_test = new QDoubleSpinBox(tbTempTest);
    sbI_test->setObjectName(QString::fromUtf8("sbI_test"));
    sbI_test->setMinimum(0.1);
    sbI_test->setMaximum(1000);

    hboxLayout4->addWidget(sbI_test);


    gridLayout1->addLayout(hboxLayout4, 3, 0, 1, 1);

    hboxLayout5 = new QHBoxLayout();
    hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
    lbD_test = new QLabel(tbTempTest);
    lbD_test->setObjectName(QString::fromUtf8("lbD_test"));
    sizePolicy.setHeightForWidth(lbD_test->sizePolicy().hasHeightForWidth());
    lbD_test->setSizePolicy(sizePolicy);
    lbD_test->setAlignment(Qt::AlignCenter);

    hboxLayout5->addWidget(lbD_test);

    sbD_test = new QDoubleSpinBox(tbTempTest);
    sbD_test->setObjectName(QString::fromUtf8("sbD_test"));
    sbD_test->setMaximum(200);

    hboxLayout5->addWidget(sbD_test);


    gridLayout1->addLayout(hboxLayout5, 4, 0, 1, 1);

    hboxLayout6 = new QHBoxLayout();
    hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
    lbMout_test = new QLabel(tbTempTest);
    lbMout_test->setObjectName(QString::fromUtf8("lbMout_test"));
    sizePolicy.setHeightForWidth(lbMout_test->sizePolicy().hasHeightForWidth());
    lbMout_test->setSizePolicy(sizePolicy);
    lbMout_test->setAlignment(Qt::AlignCenter);

    hboxLayout6->addWidget(lbMout_test);

    sbMout_test = new QDoubleSpinBox(tbTempTest);
    sbMout_test->setObjectName(QString::fromUtf8("sbMout_test"));
    sbMout_test->setMaximum(100);

    hboxLayout6->addWidget(sbMout_test);


    gridLayout1->addLayout(hboxLayout6, 5, 0, 1, 1);

    hboxLayout7 = new QHBoxLayout();
    hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
    lbRange_test = new QLabel(tbTempTest);
    lbRange_test->setObjectName(QString::fromUtf8("lbRange_test"));
    lbRange_test->setAlignment(Qt::AlignCenter);

    hboxLayout7->addWidget(lbRange_test);

    cbRange_test = new QComboBox(tbTempTest);
    cbRange_test->setObjectName(QString::fromUtf8("cbRange_test"));
    cbRange_test->setEnabled(true);

    hboxLayout7->addWidget(cbRange_test);


    gridLayout1->addLayout(hboxLayout7, 6, 0, 1, 1);

    hboxLayout8 = new QHBoxLayout();
    hboxLayout8->setObjectName(QString::fromUtf8("hboxLayout8"));
    lbt_test = new QLabel(tbTempTest);
    lbt_test->setObjectName(QString::fromUtf8("lbt_test"));

    hboxLayout8->addWidget(lbt_test);

    sbt_test = new QDoubleSpinBox(tbTempTest);
    sbt_test->setObjectName(QString::fromUtf8("sbt_test"));
    sbt_test->setMinimum(0.01);
    sbt_test->setMaximum(3600);
    sbt_test->setValue(5);

    hboxLayout8->addWidget(sbt_test);


    gridLayout1->addLayout(hboxLayout8, 7, 0, 1, 1);

    hboxLayout9 = new QHBoxLayout();
    hboxLayout9->setObjectName(QString::fromUtf8("hboxLayout9"));
    lbRamp = new QLabel(tbTempTest);
    lbRamp->setObjectName(QString::fromUtf8("lbRamp"));

    hboxLayout9->addWidget(lbRamp);

    sbRamp = new QDoubleSpinBox(tbTempTest);
    sbRamp->setObjectName(QString::fromUtf8("sbRamp"));
    sbRamp->setMaximum(100);

    hboxLayout9->addWidget(sbRamp);


    gridLayout1->addLayout(hboxLayout9, 8, 0, 1, 1);

    btTest = new QPushButton(tbTempTest);
    btTest->setObjectName(QString::fromUtf8("btTest"));

    gridLayout1->addWidget(btTest, 9, 0, 1, 1);

    btStopTest = new QPushButton(tbTempTest);
    btStopTest->setObjectName(QString::fromUtf8("btStopTest"));
    btStopTest->setEnabled(false);

    gridLayout1->addWidget(btStopTest, 10, 0, 1, 1);

    spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem2, 11, 0, 1, 1);

    tabWidget->addTab(tbTempTest, QString());

    vboxLayout->addWidget(tabWidget);

    buttonBox = new QDialogButtonBox(Settings);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    vboxLayout->addWidget(buttonBox);


    retranslateUi(Settings);
    QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

    tabWidget->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
    Settings->setWindowTitle(QApplication::translate("Settings", "Settings", 0, QApplication::UnicodeUTF8));
    gbGPIB->setTitle(QApplication::translate("Settings", "GPIB", 0, QApplication::UnicodeUTF8));
    lbTempId->setText(QApplication::translate("Settings", "GPIB \320\242\320\265\321\200\320\274\320\276\320\272\320\276\320\275\321\202\321\200\320\276\320\273\320\273\320\265\321\200\320\260", 0, QApplication::UnicodeUTF8));
    gbZones->setTitle(QApplication::translate("Settings", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\260 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262 \321\202\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\320\275\321\213\321\205 \320\267\320\276\320\275", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label->setToolTip(QApplication::translate("Settings", "\320\234\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \321\202\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\320\260 \320\262 \320\267\320\276\320\275\320\265. \320\234\320\270\320\275\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \321\201\320\276\320\276\321\202\320\262\320\265\321\202\321\201\321\202\320\262\320\265\320\275\320\275\320\276 - \321\215\321\202\320\276 \320\274\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\260\321\217 \320\277\321\200\320\265\320\264\321\213\320\264\321\203\321\211\320\265\320\271 \320\267\320\276\320\275\321\213. \320\234\320\270\320\275\320\270\320\274\321\203\320\274 \320\277\320\265\321\200\320\262\320\276\320\271 \320\267\320\276\320\275\321\213 - 0 \320\232", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label->setText(QApplication::translate("Settings", "\320\234\320\260\320\272\321\201. \321\202\320\265\320\274\320\277.", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_2->setToolTip(QApplication::translate("Settings", "\320\237\321\200\320\276\320\277\320\276\321\200\321\206\320\270\320\276\320\275\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 \320\262 \321\203\320\277\321\200\320\260\320\262\320\273\321\217\321\216\321\211\320\265\320\274 \321\203\321\200\320\260\320\262\320\275\320\265\320\275\320\270\320\270. \320\222\320\236\320\267\320\274\320\276\320\266\320\275\321\213\320\265 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217 - \320\276\321\202 0.1 \320\264\320\276 1000", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_2->setText(QApplication::translate("Settings", "P", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_3->setToolTip(QApplication::translate("Settings", "\320\230\320\275\321\202\320\265\320\263\321\200\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200. \320\236\321\202 0.1 \320\264\320\276 1000", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_3->setText(QApplication::translate("Settings", "I", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_4->setToolTip(QApplication::translate("Settings", "\320\224\320\270\321\204\321\204\320\265\321\200\320\265\320\275\321\206\320\270\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200. \320\236\321\202 0 \320\264\320\276 200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_4->setText(QApplication::translate("Settings", "D", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_5->setToolTip(QApplication::translate("Settings", "\320\237\320\276\320\267\320\262\320\276\320\273\321\217\320\265\321\202 \320\262\321\200\321\203\321\207\320\275\321\203\321\216 \320\267\320\260\320\264\320\260\321\202\321\214 \320\277\320\276\321\201\321\202\320\276\321\217\320\275\320\275\321\203\321\216 \320\274\320\276\321\211\320\275\320\276\321\201\321\202\321\214 \320\275\320\260\320\263\321\200\320\265\320\262\320\260\321\202\320\265\320\273\321\217 \320\262 \320\264\320\260\320\275\320\275\320\276\320\271 \320\267\320\276\320\275\320\265. \320\236\321\202 0 \320\264\320\276 100%", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_5->setText(QApplication::translate("Settings", "\320\240\321\203\321\207\320\275.", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_6->setToolTip(QApplication::translate("Settings", "\320\224\320\270\320\260\320\277\320\260\320\267\320\276\320\275 \320\274\320\276\321\211\320\275\320\276\321\201\321\202\320\270 \320\275\320\260\320\263\321\200\320\265\320\262\320\260\321\202\320\265\320\273\321\217.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_6->setText(QApplication::translate("Settings", "\320\234\320\276\321\211\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    label_7->setToolTip(QApplication::translate("Settings", "\320\230\320\275\321\202\320\265\321\200\320\262\320\260\320\273 \320\262\321\200\320\265\320\274\320\265\320\275\320\270 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270 \321\202\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\321\213 \320\277\320\276\321\201\320\273\320\265 \320\267\320\260\320\262\320\265\321\200\321\210\320\265\320\275\320\270\321\217 \320\277\321\200\320\276\321\202\321\217\320\266\320\272\320\270 \321\203\321\201\321\202\320\260\320\262\320\272\320\270 \320\262 \320\274\320\270\320\275\321\203\321\202\320\260\321\205", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_7->setText(QApplication::translate("Settings", "\320\222\321\200\320\265\320\274\321\217 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270", 0, QApplication::UnicodeUTF8));
    cbRange_0->clear();
    cbRange_0->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_1->clear();
    cbRange_1->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_2->clear();
    cbRange_2->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_3->clear();
    cbRange_3->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_4->clear();
    cbRange_4->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_5->clear();
    cbRange_5->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_6->clear();
    cbRange_6->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_7->clear();
    cbRange_7->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_8->clear();
    cbRange_8->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    cbRange_9->clear();
    cbRange_9->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    tabWidget->setTabText(tabWidget->indexOf(tbTempCtrl), QApplication::translate("Settings", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \321\202\320\265\321\200\320\274\320\276\320\272\320\276\320\275\321\202\321\200.", 0, QApplication::UnicodeUTF8));
    lbT_test->setText(QApplication::translate("Settings", "\320\242\320\265\320\272\321\203\321\211\320\260\321\217 \320\242.", 0, QApplication::UnicodeUTF8));
    lbNewT_test->setText(QApplication::translate("Settings", "\320\235\320\276\320\262\320\260\321\217 \320\242.", 0, QApplication::UnicodeUTF8));
    lbP_test->setText(QApplication::translate("Settings", "P", 0, QApplication::UnicodeUTF8));
    lbI_test->setText(QApplication::translate("Settings", "I", 0, QApplication::UnicodeUTF8));
    lbD_test->setText(QApplication::translate("Settings", "D", 0, QApplication::UnicodeUTF8));
    lbMout_test->setText(QApplication::translate("Settings", "\320\240\321\203\321\207\320\275.", 0, QApplication::UnicodeUTF8));
    lbRange_test->setText(QApplication::translate("Settings", "\320\234\320\276\321\211\320\275.", 0, QApplication::UnicodeUTF8));
    cbRange_test->clear();
    cbRange_test->insertItems(0, QStringList()
     << QApplication::translate("Settings", "\320\222\321\213\320\272\320\273.", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\235\320\270\320\267\320\272. (0.5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\241\321\200\320\265\320\264. (5 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("Settings", "\320\222\321\213\321\201. (50 \320\222\321\202)", 0, QApplication::UnicodeUTF8)
    );
    lbt_test->setText(QApplication::translate("Settings", "\320\222\321\200\320\265\320\274\321\217 (\320\274\320\270\320\275)", 0, QApplication::UnicodeUTF8));
    lbRamp->setText(QApplication::translate("Settings", "\320\237\321\200\320\276\321\202\321\217\320\266\320\272\320\260", 0, QApplication::UnicodeUTF8));
    btTest->setText(QApplication::translate("Settings", "\320\242\320\265\321\201\321\202", 0, QApplication::UnicodeUTF8));
    btStopTest->setText(QApplication::translate("Settings", "\320\241\321\202\320\276\320\277", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tbTempTest), QApplication::translate("Settings", "\320\242\320\265\321\201\321\202 \321\202\320\265\321\200\320\274\320\276\320\272\320\276\320\275\321\202\321\200.", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(Settings);
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
