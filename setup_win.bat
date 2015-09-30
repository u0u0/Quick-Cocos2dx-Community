@echo off
set QUICK_V3_ROOT=%~dp0
set SETX=setx

echo.
echo config:
echo.
echo QUICK_V3_ROOT = "%QUICK_V3_ROOT%"
echo.

%SETX% QUICK_V3_ROOT %QUICK_V3_ROOT%

echo.

del /f /q %userprofile%\Desktop\player3.lnk
%QUICK_V3_ROOT%quick\bin\win32\shortcut.exe %QUICK_V3_ROOT%quick\player\win32\player3.exe /d player3 /ld player3.lnk

pause
