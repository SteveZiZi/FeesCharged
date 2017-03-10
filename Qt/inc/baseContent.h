#pragma once

#include <QtGui/QWidget>


class CBaseContentPrivate;
class CBaseContent : public QWidget {
    Q_OBJECT
public:
    CBaseContent(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~CBaseContent();

    virtual void initUI();

    void setContentCaption(const QString &caption);
    void setContentIcon(QIcon *icon);

    bool setMenuCaption(int item, const QString &caption);

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
    bool setMenuContentPage(int item, int pageId);
    
    //更新菜单标题和图标
    void updateMenuInfo();
    // 设置按钮是否显示
    void setBtnVisiable(bool flag);

protected:
    void keyPressEvent(QKeyEvent *e);

    /*
     *  @Func:  OnF1Press ~ OnF4Press
     *  @Desc:  响应对应菜单按下的事件
     *  
     *  @Param[in]:
     *      NONE
     *  
     *  @Return:
     *      true:   菜单项处理完成，即使设置了菜单对应的切换页面也不会显示页面
     *      false:  设置了菜单对应的切换页面将会显示该页面
     */
    virtual bool OnF1Press() {return false;}
    virtual bool OnF2Press() {return false;}
    virtual bool OnF3Press() {return false;}
    virtual bool OnF4Press() {return false;}

protected slots:
    void menu1Pressed();
    void menu2Pressed();
    void menu3Pressed();
    void menu4Pressed();

private:
    void btnLayout();

protected:
    CBaseContentPrivate* m_data;
};

