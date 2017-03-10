/*  @file
 *  @brief QApplication������أ�ͨ������ʵ��notify��ʵ�ֱ�����Զ��ر�
 *  @author MSZ
 *  @date 2016/03/10
 *  @version 0.1
 */
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class QTimer;
class Application : public QApplication
{
    Q_OBJECT
public:
    /*
        brief: Application��ʼ������
        param [in] int & argc, char ** argv
        param [out]None
        return bool: None
        author:MSZ
        date: 2016-03-10
    */
    Application(int & argc, char ** argv);
    /*
        brief: ����notify������ͨ�����MouseButtonPress�¼�����ʵ�ֶԱ���Ŀ���
        param [in] QObject *obj, QEvent *e
        param [out]None
        return bool: None
        author:MSZ
        date: 2016-03-10
    */
    bool notify(QObject *obj, QEvent *e);

signals:
    void userInActived();

public slots:
    void userActive();

private slots:
    void onTimeout();

private:
    int inactiveCnt;
    QTimer *inactiveTimer;
    int timeoutTimeS;
};

#endif // APPLICATION_H
