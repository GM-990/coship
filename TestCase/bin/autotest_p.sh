#! /bin/bash

#------------设置工程路径----------------------
workpath=/home/huangkerong/trunk
#------------添加需要测试模块名称------------codec kernel pvr dtv mediaplayer protocol -
testmodel_list="kernel";
#------------设置平台环境----------------------
cd $workpath && \
. bin/setupenv --ia $workpath/bin/envfile_fortrunk/setenv_bcm740x_release
#------------设置网络连接参数，具体修改$workpath/bin/run_bcm740x_test----------------------
#------------编译路径判断--------------
if [ "$MMCP_HOME" = "" ]; then
	echo Not setup evn, exit!
	exit 0
fi
cd ${MMCP_HOME}/test
#------------设置测试选项 mmcptest [参数1][参数2]--------------
#            参数1： -u 为更新module，编译自动测试版本
#            参数1： -um 为更新module，编译手动测试版本
#            参数1： -c 为编译自动测试版本
#            参数1： -cm 为编译自动测试版本
#            参数1： -r 执行自动测试

mmcptest_p -c "$testmodel_list" 

cd ${MMCP_HOME}
#reboot

#------- 测试报告于 $workpath/test/testdata-----------

   


 
