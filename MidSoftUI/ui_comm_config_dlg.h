/********************************************************************************
** Form generated from reading UI file 'comm_config_dlg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMM_CONFIG_DLG_H
#define UI_COMM_CONFIG_DLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_comm_config_dlg
{
public:
    QToolButton *toolButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *comboBox;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *lineEdit_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *lineEdit_5;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *comm_config_dlg)
    {
        if (comm_config_dlg->objectName().isEmpty())
            comm_config_dlg->setObjectName(QStringLiteral("comm_config_dlg"));
        comm_config_dlg->resize(360, 325);
        QIcon icon;
        icon.addFile(QStringLiteral(":/power_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        comm_config_dlg->setWindowIcon(icon);
        toolButton = new QToolButton(comm_config_dlg);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(560, 320, 37, 18));
        layoutWidget = new QWidget(comm_config_dlg);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(27, 12, 291, 301));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral(""));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout_4->addWidget(label_4);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_4->addWidget(lineEdit_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout_3->addWidget(label_3);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_3->addWidget(comboBox);

        horizontalLayout_3->setStretch(0, 7);
        horizontalLayout_3->setStretch(1, 11);

        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setAutoFillBackground(false);
        groupBox_2->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout_5->addWidget(label_5);

        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        horizontalLayout_5->addWidget(lineEdit_4);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setStyleSheet(QStringLiteral("QLabel{border:0px}"));

        horizontalLayout_6->addWidget(label_6);

        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));

        horizontalLayout_6->addWidget(lineEdit_5);


        verticalLayout_2->addLayout(horizontalLayout_6);


        verticalLayout_3->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStyleSheet(QStringLiteral(""));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(comm_config_dlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), comm_config_dlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), comm_config_dlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(comm_config_dlg);
    } // setupUi

    void retranslateUi(QDialog *comm_config_dlg)
    {
        comm_config_dlg->setWindowTitle(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\350\275\257\344\273\266\350\277\236\346\216\245\351\205\215\347\275\256", Q_NULLPTR));
        toolButton->setText(QApplication::translate("comm_config_dlg", "...", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\350\275\257\344\273\266\347\275\221\347\273\234\351\205\215\347\275\256", Q_NULLPTR));
        label->setText(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\350\275\257\344\273\266IP\357\274\232     ", Q_NULLPTR));
        label_2->setText(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\351\205\215\347\275\256Port\357\274\232   ", Q_NULLPTR));
        label_4->setText(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\344\270\232\345\212\241Port\357\274\232   ", Q_NULLPTR));
        label_3->setText(QApplication::translate("comm_config_dlg", "\351\200\232\344\277\241\346\226\271\345\274\217\357\274\232", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("comm_config_dlg", "tcp", Q_NULLPTR)
         << QApplication::translate("comm_config_dlg", "udp", Q_NULLPTR)
        );
        groupBox_2->setTitle(QApplication::translate("comm_config_dlg", "\350\277\234\347\250\213\344\270\273\346\234\272\347\275\221\347\273\234\351\205\215\347\275\256", Q_NULLPTR));
        label_5->setText(QApplication::translate("comm_config_dlg", "\350\277\234\347\250\213\344\270\273\346\234\272IP\357\274\232  ", Q_NULLPTR));
        label_6->setText(QApplication::translate("comm_config_dlg", "\350\277\234\347\250\213\344\270\273\346\234\272Port\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class comm_config_dlg: public Ui_comm_config_dlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMM_CONFIG_DLG_H
