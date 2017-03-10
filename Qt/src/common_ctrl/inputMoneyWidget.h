#ifndef INPUTMONEYWIDGET_H
#define INPUTMONEYWIDGET_H

#include <QWidget>
#include "ui_inputMoneyWidget.h"

#define MAX_MONEY_VALUE     50000.00 

class QPushButton;
class QLineEdit;
class CInputMoneyWidget : public QWidget
{
    Q_OBJECT

public:
    CInputMoneyWidget(QWidget *parent = 0);
    ~CInputMoneyWidget();

    QString inputMoney() const {return QString::number(ui.lineEdit->text().toFloat(),'f',2);}            // 获取当前数据

    void nextPosition();                                                // 切换到下个位置
    void perPosition();                                                 // 切换到上个位置
    void selectedContent();                                             // 选中选择的内容
    void deleteContent();                                               // 删除最后一个数据
    void clearLastValue();                                              // 清空数据

private:
    void setLineEditRange();
    void collectBtn();
    void setBtnSelectMark(QPushButton *btn);
    void clearBtnSelectMark(QPushButton *btn);

private:
    Ui::CInputMoneyWidget ui;

    QList<QPushButton *>    m_lBtnList;
    int                     m_iCurrentPosition;
};

#endif // INPUTMONEYWIDGET_H
