#pragma once

#include <QtGui/QTableWidget>

/****************************************************************************
**
** file: mTableWidget.h
**
** description:
**     Custom QTbaleWidget's Grid Line.
**
** change logs:
**      2015/11/21:   yangyuchao   create file
**
****************************************************************************/


class MGridStyle
{
public:
    MGridStyle(bool enabled = false, 
               bool hasGradLineH = false,
               bool hasGradLineV = false, 
               QLinearGradient*linearGradientH=NULL, 
               QLinearGradient*linearGradientV=NULL) 
        : m_enbaled(enabled)
        , m_hasGradLineH(hasGradLineH)
        , m_hasGradLineV(hasGradLineV)
        , m_linearGradientH(linearGradientH)
        , m_linearGradientV(linearGradientV)
        , m_gridColor(Qt::red)
    {

    }

    MGridStyle(QColor gridColor,
               bool hasGradLineH = false,
               bool hasGradLineV = false) 
        : m_enbaled(true)
        , m_hasGradLineH(hasGradLineH)
        , m_hasGradLineV(hasGradLineV)
        , m_linearGradientH(NULL)
        , m_linearGradientV(NULL)
        , m_gridColor(gridColor)
    {

    }

    virtual ~MGridStyle()
    {
        if (m_linearGradientH)
            delete m_linearGradientH;
        if (m_linearGradientV)
            delete m_linearGradientV;
    }

    bool isEnable() {return m_enbaled;}
    void setEnable(bool enabled) {m_enbaled = enabled;}

    QLinearGradient* linearGradientH() {return m_linearGradientH;}
    void setLinearGradientH(QLinearGradient* linearGradient) {
        if (m_linearGradientH && m_linearGradientH!=linearGradient)
            delete m_linearGradientH;
        m_linearGradientH = linearGradient;
    }
    QLinearGradient* linearGradientV() {return m_linearGradientV;}
    void setLinearGradientV(QLinearGradient* linearGradient) {
        if (m_linearGradientV && m_linearGradientH!=linearGradient)
            delete m_linearGradientV;
        m_linearGradientV = linearGradient;
    }

    void setHasGridLineH(bool hasGradLineH) { m_hasGradLineH = hasGradLineH; }
    bool hasGridLineH() {return m_hasGradLineH;}
    void setHasGridLineV(bool hasGradLineV) { m_hasGradLineV = hasGradLineV;  }
    bool hasGridLineV() {return m_hasGradLineV;}

    /*
     *  使用了QLinearGradient后 m_gridColor会被隐藏
     */
    void setGridColor(QColor clr) {
        m_gridColor = clr;
    }
    QColor gridColor() {return m_gridColor;}

private:
    bool m_enbaled;
    bool m_hasGradLineH;
    bool m_hasGradLineV;
    QLinearGradient* m_linearGradientH;
    QLinearGradient* m_linearGradientV;
    QColor           m_gridColor;
};


/*
 *  包含单位的 MTableWidgetItem
 */
class MTableWidgetItem : public QTableWidgetItem
{
public:

    MTableWidgetItem(const QString& unit)
        : QTableWidgetItem(QTableWidgetItem::UserType)
    {
        setData(Qt::UserRole, QVariant(unit));
    }
    explicit MTableWidgetItem(const QString &text, const QString& unit)
        : QTableWidgetItem(text, QTableWidgetItem::UserType)
    {
        setData(Qt::UserRole, QVariant(unit));
    }
    virtual ~MTableWidgetItem() {}

    inline QString text() const
    { return QTableWidgetItem::text().remove(data(Qt::UserRole).toString()); }
    inline void setText(const QString &text) {
        if (-1 != text.indexOf(data(Qt::UserRole).toString())) {
            QTableWidgetItem::setText(text);
        }
        else {
            QTableWidgetItem::setText(text + data(Qt::UserRole).toString());
        }
    }

    inline QString unit() const {
        return data(Qt::UserRole).toString();
    }
    inline void setUnit(const QString &unit) {
        setData(Qt::UserRole, QVariant(unit));
        QTableWidgetItem::setText(text() + unit);
    }
};

class MTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    MTableWidget(QWidget *parent = 0);
    MTableWidget(int rows, int columns, QWidget *parent = 0);
    virtual ~MTableWidget();

    /*
     *  想要MGridStyle生效  必须设置QTableWidget::setShowGrid(false)
     */
    void setGridStyle(MGridStyle* gridLine);
    MGridStyle* gridStyle(void);

    void setShowLeftRightLine(bool showLeftLine, bool showRightLine) {
        m_showLeftLine = showLeftLine;
        m_showRightLine = showRightLine;
    }
    void setShowTopBottomLine(bool showTopLine, bool showBottomLine) {
        m_showTopLine = showTopLine;
        m_showBottomLine = showBottomLine;
    }

protected:
    virtual void paintEvent(QPaintEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void focusInEvent(QFocusEvent *event);


private:
    MGridStyle* m_gridStyle;
    bool m_showLeftLine;
    bool m_showRightLine;
    bool m_showTopLine;
    bool m_showBottomLine;

    bool m_editMode;

};
