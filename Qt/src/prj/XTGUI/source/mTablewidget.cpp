#include "inc/control/mTableWidget.h"
#include "../encrypt/encrypt.h"

#include <QtGui/QTableWidget>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QHeaderView>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>

#include <QDebug>

MTableWidget::MTableWidget(QWidget *parent /* = 0 */)
:QTableWidget(parent)
, m_gridStyle(NULL)
, m_showLeftLine(false)
, m_showRightLine(false)
, m_showTopLine(false)
, m_showBottomLine(true)
, m_editMode(false)
{

}

MTableWidget::MTableWidget(int rows, int columns, QWidget *parent /* = 0 */)
:QTableWidget(rows, columns, parent)
, m_gridStyle(NULL)
, m_showLeftLine(false)
, m_showRightLine(false)
, m_showTopLine(false)
, m_showBottomLine(true)
, m_editMode(false)
{

}

MTableWidget::~MTableWidget()
{
    if (m_gridStyle)
        delete m_gridStyle;
}


void MTableWidget::setGridStyle(MGridStyle* gridLine)
{
    if (m_gridStyle && m_gridStyle != gridLine)
        delete m_gridStyle;

    m_gridStyle = gridLine;
}

MGridStyle* MTableWidget::gridStyle(void)
{
    return m_gridStyle;
}

void MTableWidget::paintEvent(QPaintEvent *event)
{
    QTableWidget::paintEvent(event);

    if (showGrid() || !m_gridStyle || !m_gridStyle->isEnable()) {
        return ;
    }

    const QHeaderView *verticalHeader = this->verticalHeader();
    const QHeaderView *horizontalHeader = this->horizontalHeader();

    // if there's nothing to do, clear the area and return
    if (horizontalHeader->count() == 0 || verticalHeader->count() == 0 || !itemDelegate())
        return;

    QPainter painter(viewport());
    const bool rightToLeft = isRightToLeft();

    //firstVisualRow is the visual index of the first visible row.  lastVisualRow is the visual index of the last visible Row.
    //same goes for ...VisualColumn
    int firstVisualRow = qMax(verticalHeader->visualIndexAt(0),0);
    int lastVisualRow = verticalHeader->visualIndexAt(verticalHeader->viewport()->height());
    if (lastVisualRow == -1)
        lastVisualRow = rowCount() - 1;

    int firstVisualColumn = horizontalHeader->visualIndexAt(0);
    int lastVisualColumn = horizontalHeader->visualIndexAt(horizontalHeader->viewport()->width());
    if (rightToLeft)
        qSwap(firstVisualColumn, lastVisualColumn);
    if (firstVisualColumn == -1)
        firstVisualColumn = 0;
    if (lastVisualColumn == -1)
        lastVisualColumn = horizontalHeader->count() - 1;

    const QPen& oldPen = painter.pen();

    //水平线
    int y = 0;
    int w = this->size().width();

    painter.setPen(QPen(m_gridStyle->gridColor(), 1)); 
    if (m_showTopLine) {
        painter.drawLine(0, y, w, y);
    }

    if (m_gridStyle->hasGridLineH() && m_gridStyle->linearGradientH()) {
        painter.setPen(QPen(QBrush(*m_gridStyle->linearGradientH()),1));
    }
    for(int row = firstVisualRow; row < lastVisualRow; row++) {
        y += this->rowHeight(row);
        if (m_gridStyle->hasGridLineH())
            painter.drawLine(0, y, w, y);
    }

    painter.setPen(QPen(m_gridStyle->gridColor(), 1)); 
    if (m_showBottomLine) {
        y += this->rowHeight(lastVisualRow);
        painter.drawLine(0, y, w, y);
    }

    //垂直线
    int x = 0;
    int h = this->size().height();

    if (m_showLeftLine) {
        painter.drawLine(x, 0, x, h);
    }

    if (m_gridStyle->hasGridLineV() && m_gridStyle->linearGradientV()) {
        painter.setPen(QPen(QBrush(*m_gridStyle->linearGradientV()),1));
    }
    for(int column = firstVisualColumn; column < lastVisualColumn; column++) {
        x += this->columnWidth(column);
        if (m_gridStyle->hasGridLineV())
            painter.drawLine(x, 0, x, h);
    }

    painter.setPen(QPen(m_gridStyle->gridColor(), 1)); 
    if (m_showRightLine) {
        x += this->columnWidth(lastVisualColumn);
        painter.drawLine(x, 0, x, h);
    }

    painter.setPen(oldPen);
}


void MTableWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    if (!verify_encrypt()) {
        return ;
    }

    if (m_editMode) {
        event->ignore();
    }
    else {
        switch(key) 
        {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            {
                QTableWidgetItem* item = this->item(currentRow(), currentColumn());
                if (item->flags()&Qt::ItemIsUserCheckable) {
                    if (item->checkState() == Qt::Checked)
                        item->setCheckState(Qt::Unchecked);
                    else
                        item->setCheckState(Qt::Checked);
                }
                else {
                    if (item->type() == QTableWidgetItem::UserType) {
                        item->setText(dynamic_cast<MTableWidgetItem*>(item)->text());
                    }
                    editItem(item);
                    m_editMode = true;
                }
            }
            event->ignore();
            break;
        case Qt::Key_Escape:
            event->ignore();
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            QTableWidget::keyPressEvent(event);
            break;
        default:
            event->ignore();
            break;
        }
    }
}


void MTableWidget::focusInEvent(QFocusEvent *event)
{
    if (m_editMode) {
        QTableWidgetItem* item = this->item(currentRow(), currentColumn());
        if (item && item->type() == QTableWidgetItem::UserType) {
            dynamic_cast<MTableWidgetItem*>(item)->setText(item->text());
        }
        m_editMode = false;
    }
    QTableWidget::focusInEvent(event);
}
