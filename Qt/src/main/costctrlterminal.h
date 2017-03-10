#ifndef COSTCTRLTERMINAL_H
#define COSTCTRLTERMINAL_H

#include <QtGui/QDialog>

#include "inc/dataTypes.h"

class QWidget;
class CBottomBarMgr;
class CTopBarMgr;
class CContentMgr;
class CCardStateManager;
class QTimer;
class CostCtrlTerminal : public QDialog
{
    Q_OBJECT

public:
    CostCtrlTerminal(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~CostCtrlTerminal();

public slots:
    void cardStatusChanged(CCT::CARD_TYPE cardType, CCT::CARD_STATUS status);

protected:
    bool event(QEvent *);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void onCardStateTimer();

private:
    void localConnect();

private:
    CCT::CARD_TYPE  m_currentCardType;
    CBottomBarMgr  *m_bottomBar;
    CTopBarMgr     *m_topBar;
    CContentMgr    *m_contentMgr;
    CCardStateManager *m_pCardStateMgr;                                 // ¿¨×´Ì¬¹ÜÀíÆ÷

    QTimer          *m_pCardStateTimer;
};

#endif // COSTCTRLTERMINAL_H
