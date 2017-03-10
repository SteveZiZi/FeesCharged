#!/bin/bash

PRJ_DIR=../../src/prj

cd ${PRJ_DIR}/XTGUI
/mcuzone/work/qt481/_install/bin/qmake XTGUI.pro
if [ "$1" == "debug" ];then
    make debug
elif [ "$1" == "clean" ];then
    make clean
else 
    make release
fi

cd -
cd ${PRJ_DIR}/dataManager
/mcuzone/work/qt481/_install/bin/qmake dataManager.pro
if [ "$1" == "debug" ];then
    make debug
elif [ "$1" == "clean" ];then
    make clean
else 
    make release
fi

cd -
cd "$PRJ_DIR"/CostCtrlTerminal
/mcuzone/work/qt481/_install/bin/qmake CostCtrlTerminal.pro
if [ "$1" == "debug" ];then
    make debug
elif [ "$1" == "clean" ];then
    make clean
else 
    make release
fi
