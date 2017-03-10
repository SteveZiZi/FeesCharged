
@set path=%path%;D:\Python27


@echo === qt build ===
@if "%1"=="debug" (
    cd ./qt
    @call build.cmd debug
) else (
    cd ./qt
    @call build.cmd release
)
cd %~dp0

pause