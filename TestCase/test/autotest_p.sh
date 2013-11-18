#! /bin/bash
#yezhenhong 904305
#----------------------edit record---------------------------------


echo in autotest_p parameter 1 is "$1";
echo in autotest_p parameter 2 is "$2";

testCmd=$1
#----------------------报告调度命令----------------------------------------------------
if [ "${testCmd}" == "-report" ]; then 
   mmcpPath=$2
   hmcPath=$3
   hmcReportpath=$4
   hmcReportCasepath=$5
   echo ${hmcPath}/${hmcReportpath}
   if [ -f "${hmcPath}/${hmcReportpath}/report/report.html" ];then
      mv ${hmcPath}/${hmcReportpath}/report/report.html $mmcpPath/test/testdata/testReport_bcm740x_DtvmTestAdaper_$(date "+%Y%m%d_%H%M")_java.html
   else
      echo fail to find DtvmTestAdaper report
   fi
   if [ -d "${hmcPath}/${hmcReportpath}" ];then
      echo remove DtvmTestAdaper
      rm ${hmcPath}/${hmcReportpath} -Rf 
   fi
   if [ -f "${hmcPath}/${hmcReportCasepath}/report/report.html" ];then
      mv ${hmcPath}/${hmcReportCasepath}/report/report.html $mmcpPath/test/testdata/testReport_bcm740x_DtvmTestCase_$(date "+%Y%m%d_%H%M")_java.html
   else
      echo fail to find DtvmTestCase report
   fi
   if [ -d "${hmcPath}/${hmcReportCasepath}" ];then
      echo remove DtvmTestCase
      rm ${hmcPath}/${hmcReportCasepath} -Rf 
   fi
fi





   


 
