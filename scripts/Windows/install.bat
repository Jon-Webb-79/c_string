@echo off
:: ================================================================================
:: ================================================================================
:: - File:    install.bat
:: - Purpose: Install/Update c_string library files to system directories
::
:: Source Metadata
:: - Author:  Jonathan A. Webb
:: - Date:    January 11, 2025
:: - Version: 1.1
:: - Copyright: Copyright 2025, Jon Webb Inc.
:: ================================================================================
:: ================================================================================

:: Check for administrator privileges
NET SESSION >nul 2>&1
if %errorLevel% neq 0 (
   echo Please run as Administrator
   pause
   exit /b 1
)

:: Set installation directories
set "INCLUDE_DIR=%ProgramFiles(x86)%\c_string\include"
set "LIB_DIR=%ProgramFiles(x86)%\c_string\lib"
set "BACKUP_DIR=%TEMP%\c_string_backup"

:: Create directories if they don't exist
if not exist "%INCLUDE_DIR%" mkdir "%INCLUDE_DIR%"
if not exist "%LIB_DIR%" mkdir "%LIB_DIR%"
if not exist "%BACKUP_DIR%" mkdir "%BACKUP_DIR%"

:: Function to backup files (implemented as a label)
:backup_file
if exist "%~1" (
   echo Backing up existing file: %~1
   for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (
       set "mydate=%%c%%a%%b"
   )
   for /f "tokens=1-2 delims=: " %%a in ('time /t') do (
       set "mytime=%%a%%b"
   )
   copy "%~1" "%BACKUP_DIR%\%~2_%mydate%_%mytime%.bak" >nul
   if errorlevel 1 (
       echo Error creating backup
       pause
       exit /b 1
   )
   echo Backup created successfully
)
goto :eof

:: Check and backup existing header file
if exist "%INCLUDE_DIR%\c_string.h" (
   echo Updating existing header file...
   call :backup_file "%INCLUDE_DIR%\c_string.h" "c_string.h"
)

:: Install/Update header file
echo Installing/Updating header file...
copy /Y "..\..\c_string\c_string.h" "%INCLUDE_DIR%"
if errorlevel 1 (
   echo Error installing header file
   pause
   exit /b 1
)

:: Check and backup existing source file
if exist "%LIB_DIR%\c_string.c" (
   echo Updating existing source file...
   call :backup_file "%LIB_DIR%\c_string.c" "c_string.c"
)

:: Install/Update source file
echo Installing/Updating source file...
copy /Y "..\..\c_string\c_string.c" "%LIB_DIR%"
if errorlevel 1 (
   echo Error installing source file
   pause
   exit /b 1
)

:: Update system environment variables
echo Updating system environment variables...
setx C_INCLUDE_PATH "%INCLUDE_DIR%;%C_INCLUDE_PATH%" /M
setx LIBRARY_PATH "%LIB_DIR%;%LIBRARY_PATH%" /M

echo Installation/Update completed successfully
echo Backups (if any) are stored in: %BACKUP_DIR%
pause
:: ================================================================================
:: ================================================================================
:: eof
