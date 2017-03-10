
#pragma once

#include "baseContent.h"

typedef CBaseContent* (*PFN_CREATE_CONTENT)(int nPageId, QWidget* parent);

class CContentMgrPrivate;
class CContentMgr
{
public:
    /**
     *      ��ȡ��̬����ָ��
     */
    static CContentMgr* GetInstance()
    {
        static CContentMgr s_Instance;
        return &s_Instance;
    }

    /*
     *  �������������������
     */
    void setParent(QWidget* parent);
    void setCreateContentFn(PFN_CREATE_CONTENT pfnCreateContent);


    /**
     *      ��ʾָ��ҳ��
     *
     *  \param[in]
     *      nPageID         ҳ��ID��ʹ�öԻ���ģ��ID
     *  \note
     *      ҳ������˳�Ӧ����һ����Ϣ
     *      �Ա�������ҳ����Ի�֪
     */
    CBaseContent* ShowPage(int nPageID);

private:
    CContentMgr();
    ~CContentMgr();

private:
    CContentMgrPrivate* m_data;
};
