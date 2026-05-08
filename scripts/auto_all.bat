@echo off
REM ============================================================
REM  一键构建-下载-验收 脚本
REM  用法: auto_all.bat [捕获秒数]
REM  流程: 编译 → 转HEX → 下载 → 复位运行 → RTT捕获 → 显示结果
REM ============================================================

echo ============================================================
echo  AUTO BUILD ^+ DOWNLOAD ^+ VERIFY
echo ============================================================
echo.

REM 步骤1: 编译
echo [1/3] Building...
call "%~dp0build.bat"
if %ERRORLEVEL% neq 0 (
    echo [ABORT] Build failed.
    exit /b 1
)
echo.

REM 步骤2: 下载
echo [2/3] Downloading...
call "%~dp0download.bat"
if %ERRORLEVEL% neq 0 (
    echo [ABORT] Download failed.
    exit /b 1
)
echo.

REM 步骤3: 等待芯片启动后验收
echo [3/3] Waiting 2s for chip boot, then capturing RTT...
timeout /t 2 /nobreak >nul
call "%~dp0verify_rtt.bat" %1
