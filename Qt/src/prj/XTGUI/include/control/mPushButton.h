#pragma once

#include <QtGui/QPushButton>


class MPushButton : public QPushButton
{
    Q_OBJECT
public:
    MPushButton(QWidget* parent = 0);
    virtual ~MPushButton();

    enum PriorityDrawStatus{
        PRESSED_PRIORITY,               //���Ȼ��ư���״̬  Ĭ��
        FOCUSED_PRIORITY,               //���Ȼ��ƽ���״̬  ������˸����˸�����������״̬
        SPLASHED_LOST_FOCUS_SET_PRESSED //���Ȼ��ƽ��㣬������˸����˸����ѹ��״̬ȡ��
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