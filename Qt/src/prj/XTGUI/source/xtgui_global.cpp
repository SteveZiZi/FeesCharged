#include "inc/xtgui_global.h"
#include "version.h"
#include <QString>

std::string xtgui_VersionInfo()
{
    std::string version = LIB_VERSION;
    version += " Built at ";
    version += __TIME__;
    version += " ";
    version += __DATE__;
    return version.c_str();
}


