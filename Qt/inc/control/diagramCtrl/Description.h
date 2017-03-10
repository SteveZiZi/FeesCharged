#pragma once

#include "decorate.h"

#include <QtGui/QRgb>
#include <QtGui/QFont>

namespace MDIAGRAM
{
    class CDescription : public CDecorator
    {
    public: 
        CDescription(quint32 id, CComponent* pComponent = NULL);
        virtual ~CDescription();

        //override
        virtual void Draw(QPainter* painter, const QRect& rect);

        void SetLabelColor(QRgb clr);

        void SetDescriptionPos(const QPoint &pt);
        void SetDescriptionColor(QRgb clr);
        void SetDescriptionFont(const QFont& font);
        void SetDescription(const QString& description);
        void EnabelShowLabel(bool bShow) {m_bShowLabel = bShow;}

    private:
        QPoint      m_pos;
        QRgb        m_labelClr;
        QRgb        m_descriptionClr;
        QFont       m_font; 
        QString     m_strDescription;
        bool        m_bShowLabel;
    };

    inline void CDescription::SetDescriptionPos(const QPoint &pt)
    {
        m_pos = pt;
    }

    inline void CDescription::SetLabelColor(QRgb clr)
    {
        m_labelClr = clr;
    }

    inline void CDescription::SetDescriptionColor(QRgb clr)
    {
        m_descriptionClr = clr;
    }
    inline void CDescription::SetDescriptionFont(const QFont& font)
    {
        m_font = font;
    }

    inline void CDescription::SetDescription(const QString& description)
    {
        m_strDescription = description;
    }


};