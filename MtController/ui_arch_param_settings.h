/********************************************************************************
** Form generated from reading UI file 'arch_param_settings.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARCH_PARAM_SETTINGS_H
#define UI_ARCH_PARAM_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_arch_param_settings
{
public:
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QLabel *label_15;
    QLineEdit *lineEdit_8;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_16;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_17;
    QComboBox *comboBox_2;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_18;
    QComboBox *comboBox_3;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *arch_param_settings)
    {
        if (arch_param_settings->objectName().isEmpty())
            arch_param_settings->setObjectName(QStringLiteral("arch_param_settings"));
        arch_param_settings->resize(479, 118);
        verticalLayout_5 = new QVBoxLayout(arch_param_settings);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        label_15 = new QLabel(arch_param_settings);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMinimumSize(QSize(20, 20));
        label_15->setMaximumSize(QSize(20, 20));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/offLine.png")));
        label_15->setScaledContents(true);

        horizontalLayout_4->addWidget(label_15);

        lineEdit_8 = new QLineEdit(arch_param_settings);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        lineEdit_8->setMinimumSize(QSize(110, 20));
        lineEdit_8->setMaximumSize(QSize(110, 20));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(11);
        lineEdit_8->setFont(font);
        lineEdit_8->setFocusPolicy(Qt::NoFocus);
        lineEdit_8->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_4->addWidget(lineEdit_8);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(arch_param_settings);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setWordWrap(false);

        verticalLayout->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_16 = new QLabel(arch_param_settings);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setMinimumSize(QSize(20, 20));
        label_16->setMaximumSize(QSize(20, 20));
        label_16->setPixmap(QPixmap(QString::fromUtf8(":/offLine.png")));
        label_16->setScaledContents(true);

        horizontalLayout->addWidget(label_16);

        comboBox = new QComboBox(arch_param_settings);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(110, 20));
        comboBox->setMaximumSize(QSize(110, 20));

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_5->addLayout(verticalLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_4 = new QLabel(arch_param_settings);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_17 = new QLabel(arch_param_settings);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setMinimumSize(QSize(20, 20));
        label_17->setMaximumSize(QSize(20, 20));
        label_17->setPixmap(QPixmap(QString::fromUtf8(":/offLine.png")));
        label_17->setScaledContents(true);

        horizontalLayout_2->addWidget(label_17);

        comboBox_2 = new QComboBox(arch_param_settings);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setMinimumSize(QSize(110, 20));
        comboBox_2->setMaximumSize(QSize(110, 20));

        horizontalLayout_2->addWidget(comboBox_2);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_5->addLayout(verticalLayout_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_11 = new QLabel(arch_param_settings);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_3->addWidget(label_11);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_18 = new QLabel(arch_param_settings);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setMinimumSize(QSize(20, 20));
        label_18->setMaximumSize(QSize(20, 20));
        label_18->setPixmap(QPixmap(QString::fromUtf8(":/offLine.png")));
        label_18->setScaledContents(true);

        horizontalLayout_3->addWidget(label_18);

        comboBox_3 = new QComboBox(arch_param_settings);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        comboBox_3->setMinimumSize(QSize(110, 20));
        comboBox_3->setMaximumSize(QSize(110, 20));

        horizontalLayout_3->addWidget(comboBox_3);


        verticalLayout_3->addLayout(horizontalLayout_3);


        horizontalLayout_5->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_5);


        verticalLayout_5->addLayout(verticalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);


        retranslateUi(arch_param_settings);

        QMetaObject::connectSlotsByName(arch_param_settings);
    } // setupUi

    void retranslateUi(QWidget *arch_param_settings)
    {
        arch_param_settings->setWindowTitle(QApplication::translate("arch_param_settings", "Form", Q_NULLPTR));
        label_15->setText(QString());
        lineEdit_8->setText(QApplication::translate("arch_param_settings", "          \346\216\247\345\210\266\345\217\260", Q_NULLPTR));
        label_3->setText(QApplication::translate("arch_param_settings", "    \347\224\265\345\212\233\344\273\277\347\234\237\350\275\257\344\273\266", Q_NULLPTR));
        label_16->setText(QString());
        label_4->setText(QApplication::translate("arch_param_settings", "    \351\200\232\344\277\241\344\273\277\347\234\237\350\275\257\344\273\266", Q_NULLPTR));
        label_17->setText(QString());
        label_11->setText(QApplication::translate("arch_param_settings", "    \347\224\265\345\212\233\345\272\224\347\224\250", Q_NULLPTR));
        label_18->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class arch_param_settings: public Ui_arch_param_settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARCH_PARAM_SETTINGS_H
