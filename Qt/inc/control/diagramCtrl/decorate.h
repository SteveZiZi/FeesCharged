#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QRect>

#include "diagramCommon.h"

class QPainter;

namespace MDIAGRAM
{
    class CComponent
    {
    public: 
        CComponent(quint32 id) : m_id(id) {}
        virtual ~CComponent() { }

        virtual void Draw(QPainter* painter, const QRect& rect) = 0;

        quint32 GetId() { return m_id; }
        bool IsEquare(quint32 id) {return (m_id == id);}

    protected:
        const quint32  m_id;
    };

    class CNullComponent : public CComponent {
    public:
        CNullComponent() : CComponent(NULL_COMPONENT_ID) {}
        virtual ~CNullComponent() {}

        //override
        void Draw(QPainter* painter, const QRect& rect) {}
    };

    /*
     *  CDecorator
     *  
     *  @Note:
     *      如果子类需要访问 m_pComponent， 请调用GetComponent()接口获得
     */
    class CDecorator : public CComponent
    {
    public:
        CDecorator(quint32 id, CComponent* component) : CComponent(id)  {
            if (component) {
                m_pComponent = component;
            }
            else {
                m_pComponent = new CNullComponent;
            }
        }

        virtual ~CDecorator() {
            if (m_pComponent) {
                delete m_pComponent;
                m_pComponent = NULL;
            }
        }

        virtual void Draw(QPainter* painter, const QRect& rect) = 0;
        CComponent* GetComponent() {return m_pComponent;}

        /*
         *  通过 ID 号可以查找到指定的CComponent
         */
        quint32 GetId() { return m_id; }
        bool IsEquare(quint32 id) {
            if (m_id == id)
                return true;
            else {
                return m_pComponent->IsEquare(id);
            }
        }

    private:
        CComponent* m_pComponent;
    };
};
