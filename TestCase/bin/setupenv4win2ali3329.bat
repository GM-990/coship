::coded by wuga,chenyajie 2009-5-6
::ִ���걾�ļ�֮��ִ�� source /bin/ali3329.tmp����������linux�±�����ѡ�����ƽ̨��ѡ��ali3329��Ȼ�󼴿ɱ��룬����ͬ����ƽ̨һ����

@echo off

::��ali3602��ͻ�������޸�ע���
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /f
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /f
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /f

reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /v native /t reg_sz /d C:\Ali_SDK\cygwin
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /v native /t reg_sz /d C:\Ali_SDK\cygwin/bin
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /v native /t reg_sz /d C:\Ali_SDK\cygwin/lib

set path=%path%;C:\Ali_SDK\cygwin;C:\Ali_SDK\cygwin\bin
set CurPath=%cd%
set BashPath=C:/Ali_SDK/cygwin/bin
set dest=%BashPath%/ali3329.tmp

C:
chdir C:\Ali_SDK\cygwin

echo cd %CurPath%>%dest%
setlocal enabledelayedexpansion
for /f "tokens=*" %%i in (%dest%) do ( 
set var=%%i 
set "var=!var:\=/!" 
set "var=!var:/bin=!"
echo !var! > %dest% 
)
echo . bin/setupenv --im >>%dest%

bash --login -i

