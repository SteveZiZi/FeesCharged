
#pragma once

#include "baseContent.h"

typedef CBaseContent* (*PFN_CREATE_CONTENT)(int nPageId, QWidget* parent);

class CContentMgrPrivate;
class CContentMgr
{
public:
    /**
     *      获取静态对象指针
     */
    static CContentMgr* GetInstance()
    {
        static CContentMgr s_Instance;
        return &s_Instance;
    }

    /*
     *  必须调用下面两个函数
     */
    void setParent(QWidget* parent);
    void setCreateContentFn(PFN_CREATE_CONTENT pfnCreateContent);


    /**
     *      显示指定页面
     *
     *  \param[in]
     *      nPageID         页面ID，使用对话框模板ID
     *  \note
     *      页面进入退出应该有一个信息
     *      以便新来的页面可以获知
     */
    CBaseContent* ShowPage(int nPageID);

private:
    CContentMgr();
    ~CContentMgr();

private:
    CContentMgrPrivate* m_data;
};
