#pragma once

#include <QtGui/QWidget>

//
//-----------------------------------------------------------------------------
//  CTopBarMgr
//
//  管理导航菜单的创建
//-----------------------------------------------------------------------------
//
class CTopBarMgrPrivate;
class CTopBarMgr : public QWidget
{
    Q_OBJECT

public:
    CTopBarMgr(QWidget* parent = 0, Qt::WindowFlags f = 0);
    ~CTopBarMgr();

public slots:
    void setMenuIcon(const QPixmap& pixmap);
    void setMenuCaption(const QString& caption);

private:
    CTopBarMgrPrivate* m_data;
};