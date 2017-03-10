#include <QKeyEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include "inputMoneyWidget.h"

CInputMoneyWidget::CInputMoneyWidget(QWidget *parent)
    : QWidget(parent),
    m_iCurrentPosition(1)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);

    setLineEditRange();

    collectBtn();
}

CInputMoneyWidget::~CInputMoneyWidget()
{

}

void CInputMoneyWidget::nextPosition()
{
    clearBtnSelectMark(m_lBtnList[m_iCurrentPosition - 1]);

    m_iCurrentPosition++;
    if (m_iCurrentPosition >= m_lBtnList.size() + 1)
    {
        m_iCurrentPosition = 1;
    }

    setBtnSelectMark(m_lBtnList[m_iCurrentPosition - 1]);
}

void CInputMoneyWidget::perPosition()
{
    clearBtnSelectMark(m_lBtnList[m_iCurrentPosition - 1]);

    m_iCurrentPosition--;
    if(m_iCurrentPosition <= 0)
    {
        m_iCurrentPosition = 16;
    }

    setBtnSelectMark(m_lBtnList[m_iCurrentPosition - 1]);
}

void CInputMoneyWidget::selectedContent()
{
    if(ui.lineEdit->text().toFloat() <= MAX_MONEY_VALUE && ui.lineEdit->text().length() <=8)
    { 
        if(ui.lineEdit->text().contains("."))
        {
            if (m_lBtnList[m_iCurrentPosition - 1]->text().contains("."))
            {
                return;
            }
        }
        ui.lineEdit->setText(ui.lineEdit->text() + 
            m_lBtnList[m_iCurrentPosition - 1]->text());
    }
}

void CInputMoneyWidget::deleteContent()
{
    ui.lineEdit->setText(ui.lineEdit->text().left(
                         ui.lineEdit->text().length() - 1));
}

void CInputMoneyWidget::clearLastValue()
{
    ui.lineEdit->clear();
}

void CInputMoneyWidget::setLineEditRange()
{
    // 设置输入框的输入范围          似乎无效。。。
    QDoubleValidator *moneyReg = new QDoubleValidator(0.000,100000.000,2,this);
    moneyReg->setNotation(QDoubleValidator::StandardNotation);	// 
    ui.lineEdit->setValidator(moneyReg);
    ui.lineEdit->setMaxLength(8);
}

void CInputMoneyWidget::collectBtn()
{
    m_lBtnList = findChildren<QPushButton *>();
    Q_FOREACH(QPushButton* btn,m_lBtnList)
    {
        if (btn)
        {
            clearBtnSelectMark(btn);
        }
    }

    setBtnSelectMark(m_lBtnList[m_iCurrentPosition - 1]);
}

void CInputMoneyWidget::setBtnSelectMark(QPushButton *btn)
{
    btn->setStyleSheet("color: white; \
                        background-color: #a2bbd9; \
                        border: 1px; \
                        font-size: 12pt; \
                        border-radius:4px;");
}

void CInputMoneyWidget::clearBtnSelectMark(QPushButton *btn)
{
    btn->setStyleSheet("color: black; \
                       background-color: #cad7e6; \
                       border: 1px; \
                       font-size: 12pt; \
                       border-radius:4px;");
}