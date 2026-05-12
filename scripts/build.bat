@echo off
REM ============================================================
REM  IAR 自动编译脚本 (通用版)
REM  用法: build.bat [项目路径] [配置名]
REM  示例: build.bat              (使用默认值编译)
REM        build.bat ..\other_prj Release
REM ============================================================

REM ---- 用户配置区 (换项目只改这里) ----
set "IAR_HOME=C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.3"
set "PROJECT_DIR=%~dp0..\auto_test\EWARM"
set "PROJECT_NAME=auto_test"
set "BUILD_CONFIG=auto_test"
REM ---- 配置区结束 ----

REM 命令行参数覆盖
if not "%~1"=="" set "PROJECT_DIR=%~1"
if not "%~2"=="" set "BUILD_CONFIG=%~2"

set "IARBUILD=%IAR_HOME%\common\bin\IarBuild.exe"
set "IELFTOOL=%IAR_HOME%\arm\bin\ielftool.exe"
set "EWP=%PROJECT_DIR%\%PROJECT_NAME%.ewp"
set "OUT_FILE=%PROJECT_DIR%\%BUILD_CONFIG%\Exe\%PROJECT_NAME%.out"
set "HEX_FILE=%PROJECT_DIR%\%BUILD_CONFIG%\Exe\%PROJECT_NAME%.hex"
set "BIN_FILE=%PROJECT_DIR%\%BUILD_CONFIG%\Exe\%PROJECT_NAME%.bin"

echo ============================================================
echo  IAR Build: %PROJECT_NAME% [%BUILD_CONFIG%]
echo ============================================================

REM 步骤1: 清理 + 编译
echo.
echo [1/3] Compiling...
"%IARBUILD%" "%EWP%" -build %BUILD_CONFIG% -log all
if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Build failed! Error code: %ERRORLEVEL%
    pause
    exit /b 1
)
echo [OK] Build succeeded: 0 Error

REM 步骤2: 生成 HEX (J-Link 下载用)
echo.
echo [2/3] Generating HEX...
"%IELFTOOL%" --ihex "%OUT_FILE%" "%HEX_FILE%"
if %ERRORLEVEL% neq 0 (
    echo [ERROR] HEX conversion failed!
    pause
    exit /b 1
)
echo [OK] %HEX_FILE%

REM 步骤3: 生成 BIN (可选)
echo.
echo [3/3] Generating BIN...
"%IELFTOOL%" --bin "%OUT_FILE%" "%BIN_FILE%"
if %ERRORLEVEL% neq 0 (
    echo [WARN] BIN conversion failed, skipping
) else (
    echo [OK] %BIN_FILE%
)

echo.
echo ============================================================
echo  Build complete! Output:
echo    HEX: %HEX_FILE%
echo    BIN: %BIN_FILE%
echo ============================================================
