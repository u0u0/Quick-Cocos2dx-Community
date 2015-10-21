@echo off
set DIR=%~dp0
%DIR%win32\php.exe "%DIR%lib\encrypt_res.php" %*
