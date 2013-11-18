@echo on

cd ..
set MMCP_HOME=%cd%
set MMCP_ENVIRONMENT=windows


set MMCP_PLATFORM=win32
set TARGET_OS=win32
set TARGET_ARCH=win32
set TARGET_CPU=win32
set PLATFORM=win32
set CHIP_PROVIDER=win32
set PLATFORM_JAVAVM=win32

rem set MMCP_PLATFORM=st7100
rem set TARGET_OS=st40os21
rem set TARGET_ARCH=st40
rem set TARGET_CPU=c2
rem set PLATFORM=st7100
rem set CHIP_PROVIDER=st
rem set PLATFORM_JAVAVM=st40os21-c2

rem set ST40ROOT=C:\STM\ST40R4.0.2
rem set PATH=%PATH%;%ST40ROOT%;%ST40ROOT%\bin;C:\Stm\multicom-3.1.1\bin\winnt\ia32;%STB7100ROOT%\bin;

set MMCP_COMPILING_MODE=printmode
set MMCP_MIDDLEWARE=dtvm
set MMCP_KERNEL_FS_ENABLE=1
set MMCP_KERNEL_FS_CONFIG="-DMMCP_FS_SUPPORT_CFS "
set MMCP_KERNEL_FS_JFFS_ENABLE=0
rem MMCP_KERNEL_FS_CONFIG+="-DMMCP_FS_SUPPORT_JFFS "
set MMCP_KERNEL_FS_ROM_ENABLE=0
rem MMCP_KERNEL_FS_CONFIG+="-DMMCP_FS_SUPPORT_ROM "
set MMCP_KERNEL_FS_FAT_ENABLE=0
rem MMCP_KERNEL_FS_CONFIG+="-DMMCP_FS_SUPPORT_FAT "
set MMCP_KERNEL_FS_E2FS_ENABLE=0
rem MMCP_KERNEL_FS_CONFIG+="-DMMCP_FS_SUPPORT_E2FS "
set MMCP_KERNEL_FS_OC_ENABLE=0
rem MMCP_KERNEL_FS_CONFIG+="-DMMCP_FS_SUPPORT_OC "

set MMCP_SECURITY=FALSE
set MMCP_GUITOOL=FALSE
set MMCP_PVR=FALSE
set MMCP_SECURITY=FALSE

@echo off
if "%InitPath%"=="" set Initpath=%Path%
set path=%InitPath%
@echo Clean old path successfully!
call %VSCOMNTOOLS%\vsvars32.bat
@echo set Win32 Successfully!

rem call %MMCP_HOME%\porting\%CHIP_PROVIDER%\%PLATFORM%\setenv.bat
rem call %MMCP_HOME%\integration\stb\integration.bat

cmd

