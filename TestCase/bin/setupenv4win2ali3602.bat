::coded by wuga,chenyajie 2009-6-12
::ִ���걾�ļ�֮��ִ�� source /bin/ali3602.tmp����������linux�±�����ѡ�����ƽ̨��ѡ��ali3602��Ȼ�󼴿ɱ��룬����ͬ����ƽ̨һ����

@echo off

::��ali3329��ͻ�������޸�ע���
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /f
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /f
reg delete "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /f

reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/" /v native /t reg_sz /d C:\Ali_3602\cygwin
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/bin" /v native /t reg_sz /d C:\Ali_3602\cygwin/bin
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /v flags /t reg_dword /d 2
reg add "hkcu\software\cygnus solutions\cygwin\mounts v2\/usr/lib" /v native /t reg_sz /d C:\Ali_3602\cygwin/lib

set path=%path%;C:\Ali_3602\cygwin;C:\Ali_3602\cygwin\bin

set CurPath=%cd%
set BashPath=C:/Ali_3602/cygwin/bin
set dest=%BashPath%/ali3602.tmp

C:
chdir C:\Ali_3602\cygwin

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

