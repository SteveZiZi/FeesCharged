#pragma once

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class MThread : public QThread
{
    Q_OBJECT
public:
    MThread(QObject *parent = 0);
    ~MThread();

public slots:
    void exitThread() {
        m_mutex.lock();
        m_wait.wakeAll();
        m_mutex.unlock();
    }

public:
    /*
     *  @Return:
     *      false: TimeOut
     *      true:  exitThread
     */
    bool waitCondition(unsigned long time = ULONG_MAX) {
        QMutexLocker _lock(&m_mutex);
        return m_wait.wait(&m_mutex, time);
    }

protected:
    QWaitCondition m_wait;
    QMutex m_mutex;
};

