# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

rootdir=../../..
PROJECT=CostCtrlTerminal

INCLUDEPATH += $${GeneratedFileDir} \
               $${rootdir} \
               . 

DESTDIR = $${rootdir}/bin
LIBS += -L$${rootdir}/lib \
        -L$${rootdir}/bin
        

TEMPLATE = app
QT += core gui network xml
CONFIG += debug_and_release
DEFINES += QT_DLL QT_XML_LIB QT_NETWORK_LIB

GeneratedFileDir=$${rootdir}/build/intermediate/$${PROJECT}
DEPENDPATH += .
UI_DIR  += $${GeneratedFileDir}
RCC_DIR += $${GeneratedFileDir}


CONFIG(debug, debug|release) {
    TARGET = $${PROJECT}_d
    MOC_DIR += $${GeneratedFileDir}/debug
    INCLUDEPATH += $${GeneratedFileDir}/Debug
    OBJECTS_DIR += $${GeneratedFileDir}/Debug
    LIBS += -ldataManager_d \
            -lXTGUI_d
} else {
    DEFINES += QT_NO_DEBUGs
    TARGET = $${PROJECT}
    INCLUDEPATH += $${GeneratedFileDir}/Release
    MOC_DIR += $${GeneratedFileDir}/Release
    OBJECTS_DIR += $${GeneratedFileDir}/Release
    LIBS += -ldataManager\
            -lXTGUI
}

include(CostCtrlTerminal.pri)
