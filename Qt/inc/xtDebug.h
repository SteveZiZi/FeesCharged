#pragma once

#include <QtGlobal>

#if (defined _DEBUG)
#undef qDebug
#define xtDebug(code, format) ((code)) ? ( qDebug format ) : 0
#else
#define xtDebug(code, format) 0
#endif

#undef qWarning
#define xtError(format) (qWarning format)

#define xtRelease(format) (qWarning format) 
