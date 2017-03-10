#pragma once

#include "mThread.h"


#ifndef WIN32
class CKeyInputThread : public MThread
{
public: 
    explicit CKeyInputThread(QObject *parent = 0);
    ~CKeyInputThread();

protected:
    virtual void run();

private:
    bool devInit();
    void devDeinit();

    void _postKeyEvent(quint16 code, quint32 value);
private:
    int m_fd;
};
#endif