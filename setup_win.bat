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

set shortPathLnk=%userprofile%\Desktop\player3.lnk
if exist %shortPathLnk% del /f /q %shortPathLnk%
set shortPathUrl=%userprofile%\Desktop\player3.url
if exist %shortPathUrl% del /f /q %shortPathUrl%
%QUICK_V3_ROOT%quick\bin\win32\shortcut.exe %QUICK_V3_ROOT%quick\player\win32\player3.exe /d player3 /ld player3.lnk

pause
