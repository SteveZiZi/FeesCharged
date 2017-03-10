#include "inc/contentMgr.h"
#include "src/main/common.h"
#include "../encrypt/encrypt.h"


class CContentMgrPrivate 
{
public:
    CContentMgrPrivate() : m_parent(0), m_curPageId(-1), m_curContent(0), m_pfnCreateContent(NULL) {}


private:
    QWidget        *m_parent;

    int            m_curPageId;
    CBaseContent   *m_curContent;

    PFN_CREATE_CONTENT m_pfnCreateContent;

    friend class CContentMgr;
};

CContentMgr::CContentMgr()
: m_data(new CContentMgrPrivate())
{

}
CContentMgr::~CContentMgr()
{
    delete m_data;
}




void CContentMgr::setParent(QWidget* parent)
{
    m_data->m_parent = parent;
}

void CContentMgr::setCreateContentFn(PFN_CREATE_CONTENT pfnCreateContent)
{
    m_data->m_pfnCreateContent = pfnCreateContent;
}

/**
 *      ��ʾָ��ҳ��
 *
 *  \param[in]
 *      nPageID         ҳ��ID��ʹ�öԻ���ģ��ID
 *  \note
 *      ҳ������˳�Ӧ����һ����Ϣ
 *      �Ա�������ҳ����Ի�֪
 */
CBaseContent* CContentMgr::ShowPage(int nPageID)
{
    if (!verify_encrypt()) {
        return new CBaseContent();
    }
    if (m_data->m_pfnCreateContent) {
        if (nPageID != m_data->m_curPageId) {
            CBaseContent *content = m_data->m_pfnCreateContent(nPageID, m_data->m_parent);
            m_data->m_curPageId = nPageID;
            if (m_data->m_curContent) {
                m_data->m_curContent->deleteLater();
            }
            m_data->m_curContent = content;
            m_data->m_curContent->show();
        }
    }
    else {
        m_data->m_curContent = NULL;
    }

    return m_data->m_curContent;
}