#include "inc/baseContent.h"
#include <QCoreApplication>
#include <QKeyEvent>

#include "inc/control/mPushButton.h"
#include "src/main/common.h"
#include "inc/customEvent.h"
#include "inc/contentMgr.h"

#include "src/main/utils/xt_utility.h"

class CBaseContentPrivate
{
public:
    CBaseContentPrivate();

    enum {MAX_BTN = 4};

private:    
    MPushButton* m_btn[MAX_BTN];
    QString m_menuCaption;
    QIcon* m_menuIcon;

    int m_menuPageId[MAX_BTN];

    friend class CBaseContent;
};

CBaseContentPrivate::CBaseContentPrivate()
 : m_menuCaption("")
 , m_menuIcon(0)
{
    memset(m_menuPageId, 0xFF, sizeof(m_menuPageId));
}

CBaseContent::CBaseContent(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
: QWidget(parent, f)
, m_data(new CBaseContentPrivate())
{

}

CBaseContent::~CBaseContent()
{
    delete m_data;
}

void CBaseContent::initUI()
{
    this->setGeometry(CONTENT_POS_L, CONTENT_POS_T, CONTENT_SIZE_W+NAVI_MENU_SIZE_W, CONTENT_SIZE_H);
    btnLayout();
    this->setFocus();
}

void CBaseContent::setContentCaption(const QString &caption)
{
    m_data->m_menuCaption = caption;
    updateMenuInfo();
}

void CBaseContent::setContentIcon(QIcon *icon)
{
    m_data->m_menuIcon = icon;
    updateMenuInfo();
}

bool CBaseContent::setMenuCaption(int item, const QString &caption)
{
    if (item < CBaseContentPrivate::MAX_BTN) {
        m_data->m_btn[item]->setText(caption);
        return true;
    }

    return false;
}
/*
 *  @Func:  setMenuContentPage
 *  @Desc:  设置菜单项对应显示的页面
 *  
 *  @Param[in]:
 *      item:   菜单序号：0~3
 *      pageId: content关联的页面ID号, -1：无效ID号，在菜单有效情况下，不会切换页面
 *  
 *  @Return:
 *      true:   设置成功
 *      false:  设置失败，可能是item越界了
 */
bool CBaseContent::setMenuContentPage(int item, int pageId)
{
    if (item < CBaseContentPrivate::MAX_BTN) {
        m_data->m_menuPageId[item] = pageId;
        return true;
    }

    return false;
}


void CBaseContent::updateMenuInfo()
{
    QCoreApplication::postEvent(this->parent(), new CTopBarEvent(m_data->m_menuCaption, m_data->m_menuIcon));
}

void CBaseContent::setBtnVisiable(bool flag)
{
    for (int i = 0;i < CBaseContentPrivate::MAX_BTN;i++)
    {
        m_data->m_btn[i]->setVisible(flag);
    }
}

void CBaseContent::keyPressEvent(QKeyEvent *e)
{
    int item = -1;
    bool rc = true;
    switch(e->key()) {
    case Qt::Key_F1:
        m_data->m_btn[0]->splash();
        item = 0;
        rc = OnF1Press();
        e->accept();
        break;
    case Qt::Key_F2:
        m_data->m_btn[1]->splash();
        e->accept();
        item = 1;
        rc = OnF2Press();
        break;
    case Qt::Key_F3:
        m_data->m_btn[2]->splash();
        e->accept();
        item = 2;
        rc = OnF3Press();
        break;
    case Qt::Key_F4:
        m_data->m_btn[3]->splash();
        e->accept();
        item = 3;
        rc = OnF4Press();
        break;
    default:
        QWidget::keyPressEvent(e);
    }

    if (!rc) {
        if(m_data->m_menuPageId[item] != -1) {
            xt_msleep(100);
            CContentMgr::GetInstance()->ShowPage(m_data->m_menuPageId[item]);
        }
    }
}


void CBaseContent::menu1Pressed()
{
    if(!OnF1Press() && m_data->m_menuPageId[0] != -1) {
        CContentMgr::GetInstance()->ShowPage(m_data->m_menuPageId[0]);
    }
}
void CBaseContent::menu2Pressed()
{
    if(!OnF2Press() && m_data->m_menuPageId[1] != -1) {
        CContentMgr::GetInstance()->ShowPage(m_data->m_menuPageId[1]);
    }
}
void CBaseContent::menu3Pressed()
{
    if(!OnF3Press() && m_data->m_menuPageId[2] != -1) {
        CContentMgr::GetInstance()->ShowPage(m_data->m_menuPageId[2]);
    }
}
void CBaseContent::menu4Pressed()
{
    if(!OnF4Press() && m_data->m_menuPageId[3] != -1) {
        CContentMgr::GetInstance()->ShowPage(m_data->m_menuPageId[3]);
    }
}

void CBaseContent::btnLayout()
{
    for(int i = 0; i < CBaseContentPrivate::MAX_BTN; i++) {
        m_data->m_btn[i] = new MPushButton(this);
        m_data->m_btn[i]->setObjectName("btnNone");
        m_data->m_btn[i]->setPriorityDrawStatus(MPushButton::FOCUSED_PRIORITY);
    }

    for(int i = 0; i < CBaseContentPrivate::MAX_BTN; i++) {
        m_data->m_btn[i]->resize(NAVI_MENU_SIZE_W, NAVI_MENU_ITEM_H);
        QPoint pos = this->mapFromParent(QPoint(NAVI_MENU_POS_L, NAVI_MENU_POS_T+NAVI_MENU_ITEM_H*i+i));
        m_data->m_btn[i]->move(pos);
    }

    connect(m_data->m_btn[0], SIGNAL(clicked()), this, SLOT(menu1Pressed()));
    connect(m_data->m_btn[1], SIGNAL(clicked()), this, SLOT(menu2Pressed()));
    connect(m_data->m_btn[2], SIGNAL(clicked()), this, SLOT(menu3Pressed()));
    connect(m_data->m_btn[3], SIGNAL(clicked()), this, SLOT(menu4Pressed()));
}