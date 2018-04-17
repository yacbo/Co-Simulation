/********************************************************************************
** Form generated from reading UI file 'midsoftui.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIDSOFTUI_H
#define UI_MIDSOFTUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MidSoftUI
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_8;
    QComboBox *comboBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *lineEdit_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QComboBox *comboBox;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_8;
    QTableView *tableView;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *registerButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *unregisterButton;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QDialog *MidSoftUI)
    {
        if (MidSoftUI->objectName().isEmpty())
            MidSoftUI->setObjectName(QStringLiteral("MidSoftUI"));
        MidSoftUI->resize(835, 435);
        QIcon icon;
        icon.addFile(QStringLiteral(":/power_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        MidSoftUI->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(MidSoftUI);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        groupBox_3 = new QGroupBox(MidSoftUI);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_2 = new QGroupBox(groupBox_3);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        groupBox_2->setFont(font);
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        QFont font1;
        font1.setFamily(QStringLiteral("Agency FB"));
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        label_8->setFont(font1);

        horizontalLayout->addWidget(label_8);

        comboBox_2 = new QComboBox(groupBox_2);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setStyleSheet(QStringLiteral("QComboBox{min-height:23;}"));

        horizontalLayout->addWidget(comboBox_2);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        label_4->setFont(font2);

        horizontalLayout_3->addWidget(label_4);

        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setStyleSheet(QStringLiteral("QLineEdit{min-height:23;}"));

        horizontalLayout_3->addWidget(lineEdit_3);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font2);

        horizontalLayout_4->addWidget(label_5);

        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setStyleSheet(QStringLiteral("QLineEdit{min-height:23;}"));

        horizontalLayout_4->addWidget(lineEdit_4);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 2);

        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox = new QGroupBox(groupBox_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font2);

        horizontalLayout_5->addWidget(label_2);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setStyleSheet(QStringLiteral("QLineEdit{min-height:23;}"));

        horizontalLayout_5->addWidget(lineEdit);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 2);

        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font2);

        horizontalLayout_6->addWidget(label_3);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setStyleSheet(QStringLiteral("QLineEdit{min-height:23;}"));

        horizontalLayout_6->addWidget(lineEdit_2);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 2);

        verticalLayout_3->addLayout(horizontalLayout_6);


        verticalLayout_2->addWidget(groupBox);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font1);

        horizontalLayout_7->addWidget(label_7);

        comboBox = new QComboBox(groupBox_3);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setStyleSheet(QStringLiteral("QComboBox{min-height:23;}"));

        horizontalLayout_7->addWidget(comboBox);

        horizontalLayout_7->setStretch(0, 1);
        horizontalLayout_7->setStretch(1, 2);

        verticalLayout_2->addLayout(horizontalLayout_7);

        verticalLayout_2->setStretch(0, 3);
        verticalLayout_2->setStretch(1, 2);
        verticalLayout_2->setStretch(2, 1);

        horizontalLayout_9->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(MidSoftUI);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_4);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        tableView = new QTableView(groupBox_4);
        tableView->setObjectName(QStringLiteral("tableView"));

        horizontalLayout_8->addWidget(tableView);


        horizontalLayout_9->addWidget(groupBox_4);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 2);

        verticalLayout_4->addLayout(horizontalLayout_9);

        verticalSpacer = new QSpacerItem(1, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);

        registerButton = new QPushButton(MidSoftUI);
        registerButton->setObjectName(QStringLiteral("registerButton"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setWeight(50);
        registerButton->setFont(font3);

        horizontalLayout_10->addWidget(registerButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_2);

        unregisterButton = new QPushButton(MidSoftUI);
        unregisterButton->setObjectName(QStringLiteral("unregisterButton"));
        unregisterButton->setFont(font3);

        horizontalLayout_10->addWidget(unregisterButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_4);

        horizontalLayout_10->setStretch(0, 1);
        horizontalLayout_10->setStretch(1, 2);
        horizontalLayout_10->setStretch(2, 1);
        horizontalLayout_10->setStretch(3, 2);
        horizontalLayout_10->setStretch(4, 1);

        verticalLayout_4->addLayout(horizontalLayout_10);


        retranslateUi(MidSoftUI);

        QMetaObject::connectSlotsByName(MidSoftUI);
    } // setupUi

    void retranslateUi(QDialog *MidSoftUI)
    {
        MidSoftUI->setWindowTitle(QApplication::translate("MidSoftUI", "MidSoftUI", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MidSoftUI", "\345\231\250\344\273\266\345\217\212\345\234\260\345\235\200\351\205\215\347\275\256", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MidSoftUI", "Simulation Software(\344\273\277\347\234\237\350\275\257\344\273\266)", Q_NULLPTR));
        label_8->setText(QApplication::translate("MidSoftUI", "\345\231\250\344\273\266\347\261\273\345\236\213\357\274\232          ", Q_NULLPTR));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MidSoftUI", "\347\224\265\345\212\233\350\275\257\344\273\266", Q_NULLPTR)
         << QApplication::translate("MidSoftUI", "\351\200\232\344\277\241\350\275\257\344\273\266", Q_NULLPTR)
         << QApplication::translate("MidSoftUI", "\347\224\265\345\212\233\345\272\224\347\224\250", Q_NULLPTR)
        );
        label_4->setText(QApplication::translate("MidSoftUI", "\344\270\255\351\227\264\344\273\266IP\357\274\232  ", Q_NULLPTR));
        label_5->setText(QApplication::translate("MidSoftUI", "\344\270\255\351\227\264\344\273\266Port\357\274\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MidSoftUI", "SBS(\350\275\257\346\200\273\347\272\277)", Q_NULLPTR));
        label_2->setText(QApplication::translate("MidSoftUI", "  IP\357\274\232 ", Q_NULLPTR));
        label_3->setText(QApplication::translate("MidSoftUI", "Port\357\274\232 ", Q_NULLPTR));
        label_7->setText(QApplication::translate("MidSoftUI", "\351\200\232\344\277\241\346\226\271\345\274\217\357\274\232", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MidSoftUI", "tcp", Q_NULLPTR)
         << QApplication::translate("MidSoftUI", "udp", Q_NULLPTR)
        );
        groupBox_4->setTitle(QApplication::translate("MidSoftUI", "\350\277\233\347\250\213\344\277\241\346\201\257", Q_NULLPTR));
        registerButton->setText(QApplication::translate("MidSoftUI", "\346\263\250\345\206\214", Q_NULLPTR));
        unregisterButton->setText(QApplication::translate("MidSoftUI", "\346\263\250\351\224\200", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MidSoftUI: public Ui_MidSoftUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIDSOFTUI_H
