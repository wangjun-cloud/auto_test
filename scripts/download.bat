@echo off
REM ============================================================
REM  J-Link 自动下载脚本 (通用版)
REM  用法: download.bat [HEX文件路径]
REM  示例: download.bat
REM        download.bat ..\other\Debug\Exe\firmware.hex
REM ============================================================

REM ---- 用户配置区 (换项目/芯片只改这里) ----
set "JLINK_HOME=C:\Program Files\SEGGER\JLink"
set "DEVICE=STM32F103RB"
set "INTERFACE=SWD"
set "SPEED=4000"
set "PROJECT_DIR=%~dp0.."
set "HEX_FILE=%PROJECT_DIR%Debug\Exe\auto_test.hex"
REM ---- 配置区结束 ----

REM 命令行参数覆盖
if not "%~1"=="" set "HEX_FILE=%~1"

set "JLINK=%JLINK_HOME%\JLink.exe"
set "CMD_FILE=%PROJECT_DIR%_jlink_cmd.tmp"

REM 检查 HEX 文件
if not exist "%HEX_FILE%" (
    echo [ERROR] HEX file not found: %HEX_FILE%
    echo Run build.bat first.
    pause
    exit /b 1
)

echo ============================================================
echo  J-Link Download: %DEVICE% via %INTERFACE%
echo  File: %HEX_FILE%
echo ============================================================

REM 生成临时命令文件
echo device %DEVICE%> "%CMD_FILE%"
echo si %INTERFACE%>> "%CMD_FILE%"
echo speed %SPEED%>> "%CMD_FILE%"
echo loadfile %HEX_FILE%>> "%CMD_FILE%"
echo r>> "%CMD_FILE%"
echo g>> "%CMD_FILE%"
echo exit>> "%CMD_FILE%"

REM 执行下载
"%JLINK%" -CommandFile "%CMD_FILE%"

REM 清理临时文件
del "%CMD_FILE%" 2>nul

echo.
echo ============================================================
echo  Download complete!
echo ============================================================
