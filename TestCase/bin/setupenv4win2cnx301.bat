::coded by wuga,chenyajie 2009-5-8
::ִ�б��ļ���ֱ�ӵ���Ӧ��Ŀ¼��make���ɣ����ܽ���ѡ��ֻ�ܱ���release�汾�Ŀ⡣

@echo off

cd ..
set MMCP_HOME=%cd%
set MMCP_ENVIRONMENT=windows

set MMCP_PLATFORM=cnx301
set TARGET_OS=conexant
set TARGET_ARCH=arm
set TARGET_CPU=ARM946E-S
set PLATFORM=conexant301
set CHIP_PROVIDER=conexant

set CNXROOT=C:\Program Files\ARM\ADSv1_2
set PATH=%PATH%;%CNXROOT%;%CNXROOT%\bin;%MMCP_HOME%\bin;

set MMCP_COMPILING_MODE=release
set MMCP_MIDDLEWARE=dtv
set MMCP_SECURITY=FALSE
set MMCP_GUITOOL=FALSE
set MMCP_PVR=FALSE
set MMCP_SECURITY=FALSE

cmd
