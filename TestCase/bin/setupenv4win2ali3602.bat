::coded by wuga,chenyajie 2009-6-12
::执行完本文件之后执行 source /bin/ali3602.tmp，即可像在linux下边那样选择编译平台，选择ali3602，然后即可编译，方法同其他平台一样。

@echo off

::与ali3329冲突，故先修改注册表
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

