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

    QString inputMoney() const {return QString::number(ui.lineEdit->text().toFloat(),'f',2);}            // ��ȡ��ǰ����

    void nextPosition();                                                // �л����¸�λ��
    void perPosition();                                                 // �л����ϸ�λ��
    void selectedContent();                                             // ѡ��ѡ�������
    void deleteContent();                                               // ɾ�����һ������
    void clearLastValue();                                              // �������

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
