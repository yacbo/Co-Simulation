/********************************************************************************
** Form generated from reading UI file 'sbssoftui.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SBSSOFTUI_H
#define UI_SBSSOFTUI_H

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

class Ui_SbsSoftUI
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_9;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QTableView *tableView;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_8;
    QTableView *tableView_2;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;

    void setupUi(QDialog *SbsSoftUI)
    {
        if (SbsSoftUI->objectName().isEmpty())
            SbsSoftUI->setObjectName(QStringLiteral("SbsSoftUI"));
        SbsSoftUI->resize(750, 450);
        QIcon icon;
        icon.addFile(QStringLiteral(":/power_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        SbsSoftUI->setWindowIcon(icon);
        SbsSoftUI->setAutoFillBackground(true);
        verticalLayout_2 = new QVBoxLayout(SbsSoftUI);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_3 = new QGroupBox(SbsSoftUI);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_3);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(groupBox_3);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_2->addWidget(lineEdit_2);


        horizontalLayout_5->addLayout(horizontalLayout_2);

        horizontalSpacer_6 = new QSpacerItem(25, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(groupBox_3);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);


        horizontalLayout_5->addLayout(horizontalLayout_3);

        horizontalSpacer_4 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        comboBox = new QComboBox(groupBox_3);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_4->addWidget(comboBox);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 3);

        horizontalLayout_5->addLayout(horizontalLayout_4);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(2, 1);
        horizontalLayout_5->setStretch(4, 1);

        horizontalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout->addWidget(groupBox_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        groupBox = new QGroupBox(SbsSoftUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableView = new QTableView(groupBox);
        tableView->setObjectName(QStringLiteral("tableView"));

        horizontalLayout->addWidget(tableView);


        horizontalLayout_9->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SbsSoftUI);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_2);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        tableView_2 = new QTableView(groupBox_2);
        tableView_2->setObjectName(QStringLiteral("tableView_2"));

        horizontalLayout_8->addWidget(tableView_2);


        horizontalLayout_9->addWidget(groupBox_2);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 4);

        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer);

        pushButton = new QPushButton(SbsSoftUI);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_7->addWidget(pushButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        pushButton_2 = new QPushButton(SbsSoftUI);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_7->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        horizontalLayout_7->setStretch(0, 2);
        horizontalLayout_7->setStretch(1, 3);
        horizontalLayout_7->setStretch(2, 2);
        horizontalLayout_7->setStretch(3, 3);
        horizontalLayout_7->setStretch(4, 2);

        verticalLayout->addLayout(horizontalLayout_7);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(SbsSoftUI);

        QMetaObject::connectSlotsByName(SbsSoftUI);
    } // setupUi

    void retranslateUi(QDialog *SbsSoftUI)
    {
        SbsSoftUI->setWindowTitle(QApplication::translate("SbsSoftUI", "\350\275\257\346\200\273\347\272\277\346\234\215\345\212\241\345\231\250", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("SbsSoftUI", "\351\205\215\347\275\256\344\277\241\346\201\257", Q_NULLPTR));
        label_2->setText(QApplication::translate("SbsSoftUI", "\346\234\254\346\234\272IP\357\274\232", Q_NULLPTR));
        label->setText(QApplication::translate("SbsSoftUI", "\351\200\232\344\277\241\347\253\257\345\217\243\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("SbsSoftUI", "\351\200\232\344\277\241\345\215\217\350\256\256\357\274\232", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("SbsSoftUI", "tcp", Q_NULLPTR)
         << QApplication::translate("SbsSoftUI", "udp", Q_NULLPTR)
        );
        groupBox->setTitle(QApplication::translate("SbsSoftUI", "\345\234\250\347\272\277\345\231\250\344\273\266", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("SbsSoftUI", "\350\277\233\347\250\213\344\277\241\346\201\257", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SbsSoftUI", "\345\220\257\345\212\250\346\234\215\345\212\241", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("SbsSoftUI", "\345\201\234\346\255\242\346\234\215\345\212\241", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SbsSoftUI: public Ui_SbsSoftUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SBSSOFTUI_H
