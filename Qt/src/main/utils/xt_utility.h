
#pragma once

#include <QtCore/QTimer>
#include <QtCore/QEventLoop>
/*
 *  @Func:          xt_msleep
 *  @Desc:          ms delay 
 *  
 *  @Param[in]:     
 *          ms:     millisecond
 */
inline void xt_msleep(int ms)
{
    QEventLoop eventloop;
    QTimer::singleShot(ms, &eventloop, SLOT(quit()));
    eventloop.exec();
}