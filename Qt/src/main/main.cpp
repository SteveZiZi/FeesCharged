#include "costctrlterminal.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QWSServer>
#include <QFile>

#include "inc/xtDebug.h"

#include "application.h"
#include "src/main/utils/publicNotice.h"
#include "src/main/utils/keyInput.h"
#include "src/main/common.h"

#if (defined _DEBUG) && (defined WIN32)
#include "vld.h"
#endif


QString getStyleSheetFromFile(QString fileName)
{
    QFile file(fileName);

    QString styleSheet("");
    if (file.open(QFile::ReadOnly))  {
        styleSheet = QLatin1String(file.readAll());
        file.close();
    }

    return styleSheet;
}


int main(int argc, char *argv[])
{
#if JUST_FOR_DEMO
    QApplication a(argc, argv);
#else
    Application a(argc, argv);
#endif

    xtDebug(1, ("ddd"));

    qApp->setStyleSheet(getStyleSheetFromFile(":/CostCtrlTerminal/CostCtrlTerminal.qss"));

    //text codec
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    //a.setFont(QFont("Î¢ÈíÑÅºÚ"));
#if JUST_FOR_DEMO

#else
    //splash
    QObject::connect(&a, SIGNAL(userInActived()), SplashDlg::getInstance(), SLOT(splash()));
    QObject::connect(SplashDlg::getInstance(), SIGNAL(exitSplash()), &a, SLOT(userActive()));
#endif

    //hide cursor
#ifndef WIN32
    QWSServer::setCursorVisible(false);
#endif

    CostCtrlTerminal w;
#ifndef WIN32
    CKeyInputThread* inputThread = new CKeyInputThread(&w);
    inputThread->start(QThread::HighPriority);
#endif
    w.show();
#if JUST_FOR_DEMO
    CPublicNotice::getInstance()->splash();
#endif
    return a.exec();
}
