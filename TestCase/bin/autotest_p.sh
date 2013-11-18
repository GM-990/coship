#! /bin/bash

#------------���ù���·��----------------------
workpath=/home/huangkerong/trunk
#------------�����Ҫ����ģ������------------codec kernel pvr dtv mediaplayer protocol -
testmodel_list="kernel";
#------------����ƽ̨����----------------------
cd $workpath && \
. bin/setupenv --ia $workpath/bin/envfile_fortrunk/setenv_bcm740x_release
#------------�����������Ӳ����������޸�$workpath/bin/run_bcm740x_test----------------------
#------------����·���ж�--------------
if [ "$MMCP_HOME" = "" ]; then
	echo Not setup evn, exit!
	exit 0
fi
cd ${MMCP_HOME}/test
#------------���ò���ѡ�� mmcptest [����1][����2]--------------
#            ����1�� -u Ϊ����module�������Զ����԰汾
#            ����1�� -um Ϊ����module�������ֶ����԰汾
#            ����1�� -c Ϊ�����Զ����԰汾
#            ����1�� -cm Ϊ�����Զ����԰汾
#            ����1�� -r ִ���Զ�����

mmcptest_p -c "$testmodel_list" 

cd ${MMCP_HOME}
#reboot

#------- ���Ա����� $workpath/test/testdata-----------

   


 
