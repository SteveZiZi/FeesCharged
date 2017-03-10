#pragma once

#include <QtGui/QPushButton>


class MPushButton : public QPushButton
{
    Q_OBJECT
public:
    MPushButton(QWidget* parent = 0);
    virtual ~MPushButton();

    enum PriorityDrawStatus{
        PRESSED_PRIORITY,               //优先绘制按下状态  默认
        FOCUSED_PRIORITY,               //优先绘制焦点状态  可以闪烁，闪烁过后进入正常状态
        SPLASHED_LOST_FOCUS_SET_PRESSED //优先绘制焦点，可以闪烁，闪烁过后压下状态取反
    };

    PriorityDrawStatus priorityDrawStatus() {return m_priorityDrawStatus;}
    void setPriorityDrawStatus(PriorityDrawStatus priority) {m_priorityDrawStatus = priority; emit repaint();}

    void SetPressMode(bool bPressed);
    bool pressMode() {return m_bPressed;}

public slots:
    void splash(void);

private slots:
    void finishSplash(void);

protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent* event);


private:
    bool m_bPressed;
    PriorityDrawStatus m_priorityDrawStatus; 
    bool m_bSplash;
    bool m_bSplashing;
};