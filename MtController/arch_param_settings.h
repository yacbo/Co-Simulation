#ifndef ARCH_PARAM_SETTINGS_H
#define ARCH_PARAM_SETTINGS_H

#include <QWidget>

namespace Ui {
class arch_param_settings;
}

class arch_param_settings : public QWidget
{
    Q_OBJECT

public:
    explicit arch_param_settings(QWidget *parent = 0);
    ~arch_param_settings();

private:
    Ui::arch_param_settings *ui;

public:
    void init();
    void setComboBoxClear();  //清空所有comboBox内容
    void setComboBoxCurrentText(QString devName);  //comboBox设值
    void setComboBoxCurrentTextClear(QString devName);  //comboBox设值为空
    void setSbsSoftOnLine();  //SBS在线设置.
    void setSbsSoftOffLine();  //SBS离线设置.
    void setElecSoftOnLine();  //电力在线设置.
    void setElecSoftOffLine();  //电力离线设置.
    void setCommSoftOnLine();   //通信在线设置.
    void setCommSoftOffLine();   //通信离线设置.
    void setElecAppSoftOnLine();//电力应用在线设置.
    void setElecAppSoftOffLine(); //电力应用离线设置.

private:
    void paintEvent(QPaintEvent* pEvent);
};

#endif // ARCH_PARAM_SETTINGS_H
