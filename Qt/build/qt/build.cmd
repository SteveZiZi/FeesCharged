@echo off

echo build IEC101

%~d0
cd %~dp0

set QT_DISK=D:
set PRJ_DIR=..\..\src\prj
set QT_VERSION=4.8.1
set QT_DIR=%QT_DISK%\Qt\%QT_VERSION%\bin

%QT_DISK%
cd %QT_DIR%
call qtvars.bat vsvars

%~d0
cd %~dp0
cd %PRJ_DIR%\XTGUI
qmake XTGUI.pro
if "%1"=="debug" (
    nmake debug
) else (
    nmake release
)


%~d0
cd %~dp0
cd %PRJ_DIR%\dataManager
qmake dataManager.pro
if "%1"=="debug" (
    nmake debug
) else (
    nmake release
)

%~d0
cd %~dp0
cd %PRJ_DIR%\CostCtrlTerminal
qmake CostCtrlTerminal.pro
if "%1"=="debug" (
    nmake debug
) else (
    nmake release
)