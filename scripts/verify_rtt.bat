@echo off
REM ============================================================
REM  RTT 验收测试脚本 (通用版)
REM  用法: verify_rtt.bat [捕获时长秒] [RTT控制块地址]
REM  示例: verify_rtt.bat           (默认捕获10秒, 自动搜索RTT)
REM        verify_rtt.bat 30        (捕获30秒)
REM        verify_rtt.bat 10 0x20000400  (指定RTT地址)
REM
REM  输出: rtt_output.txt (RTT日志, 每次覆盖)
REM ============================================================

REM ---- 用户配置区 ----
set "JLINK_HOME=C:\Program Files\SEGGER\JLink"
set "DEVICE=STM32F103RB"
set "INTERFACE=SWD"
set "SPEED=4000"
set "PROJECT_DIR=%~dp0..\auto_test\EWARM"
set "RTT_LOG=%~dp0..\rtt_output.txt"
set "CAPTURE_TIME=10"
set "RTT_ADDR="
REM ---- 配置区结束 ----

REM 命令行参数覆盖
if not "%~1"=="" set "CAPTURE_TIME=%~1"
if not "%~2"=="" set "RTT_ADDR=-RTTAddr %~2"

set "JLINK=%JLINK_HOME%\JLink.exe"
set "RTT_LOGGER=%JLINK_HOME%\JLinkRTTLogger.exe"

echo ============================================================
echo  RTT Verification: %DEVICE%
echo  Capture: %CAPTURE_TIME% seconds
echo  Output:  %RTT_LOG%
echo ============================================================
echo.

REM 方式1: 使用 JLinkRTTLogger (需要RTT控制块)
echo [INFO] Starting RTT capture for %CAPTURE_TIME% seconds...
echo [INFO] If no output, check if RTT control block is initialized.

start /B "" "%RTT_LOGGER%" -Device %DEVICE% -If %INTERFACE% -Speed %SPEED% %RTT_ADDR% "%RTT_LOG%" >nul 2>&1

REM 等待捕获
timeout /t %CAPTURE_TIME% /nobreak >nul

REM 停止 RTT Logger
taskkill /F /IM JLinkRTTLogger.exe >nul 2>&1

REM 显示结果
echo.
echo ============================================================
echo  RTT Output:
echo ============================================================
if exist "%RTT_LOG%" (
    type "%RTT_LOG%"
    echo.
    echo ============================================================
    echo  Capture saved to: %RTT_LOG%
    echo ============================================================
) else (
    echo [WARN] No RTT output captured.
    echo.
    echo Possible reasons:
    echo   1. RTT control block not found - try specifying address:
    echo      verify_rtt.bat 10 0x20000400
    echo.
    echo   2. Program not running - try download.bat first
    echo.
    echo   3. HSE crystal not connected - program stuck in clock init
    echo.
    echo To find RTT address, check map file:
    echo   grep "_RTT" Debug\List\auto_test.map
)
