#!/usr/bin/python
# -*- coding: cp936 -*-
#yezhenhong 904305
#----------------------edit record---------------------------------
#    20100402 added send inner outer mail server
#    20100406 inner mail server parameters changed,attchments need to change smaller;
#    20100406 change run_bcm740x_test_p for some modules stop on the way
#    20100415 add java test module
#    20100420 change skin parameters
#    20100715 add mail content and record test result
#    20100820 add zip mail content
# -----------------------------------------------------------------
import os
import time
import thread
import string
import sys
import threading
import shutil
import glob
import re
import myReplaceFile as fileEdit
import excelReport
import mailAgent
import RecordTest
import zipfile

#---------------------------global definition----------------------------------------
stbip=[]
svnupCopyFlag=[]
copyLibraryFlag=[]
makeCleanFlag=[]
makeFlag=[]

mountTrunkPath=[]
mountPortingPath=[]
mountCAPath=[]
srcIncludePath=[]
srcJarPath=[]
srcAPath=[]
bFixDateFlag=[]
srcPortingADate=[]
srcCaADate=[]
srcPortingAPath=[]
srcCaAPath=[]

mmcpPath =[]
mmcpEnvCmd= []
mmcpIncludePath=[]
mmcpJarPath=[]
mmcpAPath=[]
testModulePath=[]
testmodel_list=[]
argv_debugLevel=[]

hmcPath=[]
hmcIncludePath=[]
hmcJarPath=[]
hmcAPath=[]
hmcApplicationPath =[]
hmcProductPath = []
hmcBinExeDefineFile= []
hmcExecutepath=[]
hmcReportpath= []
hmcReportCasepath= []
hmcCommand = []
bAutoTestFlag = []


time_hour_minute = []
mailSubject =[]
mailSender=[]
mailReceiver =[]
mailServerAddress = []
mailHead = []
mailTail = []

maxDuration = 2*60*60
setFlagValue = []
attachments = []
testCmd = []


sendMailTmp= False
bMail = False
bReboot = False
timeReboot = 10
parameterName='parameter_test'
# 
#---------------------------function definition--------------------------------------------------
#temp,mmcpEnvCmd[0],mmcpIncludePath[0],mmcpJarPath[0],mmcpAPath[0], srcPortingAPath=[]

def setCmd():
    temp = '%s %s %s %s %s %s %s %s %s %s %s '%(mountTrunkPath[0],mountPortingPath[0], mountCAPath[0], srcIncludePath[0], srcJarPath[0],srcAPath[0],bFixDateFlag[0],srcPortingADate[0],srcCaADate[0], srcPortingAPath[0], srcCaAPath[0] ) 
    tempMmcp=' %s %s %s %s %s '%( temp,mmcpEnvCmd[0],mmcpIncludePath[0],mmcpJarPath[0], mmcpAPath[0]) 
    temphmc =' %s %s %s %s '%( temp,hmcIncludePath[0],hmcJarPath[0],hmcAPath[0])
    
    testmodel_list_split = testmodel_list[0].split()
    for tm in testmodel_list_split:
       cModuleCmd = '%s "%s %s %s %s %s %s %s %s %s" "%s" '%('./mmcp_test_p',stbip[0],svnupCopyFlag[0],copyLibraryFlag[0],makeCleanFlag[0],makeFlag[0], mmcpPath[0],argv_debugLevel[0],tempMmcp,testModulePath[0],tm)
       testCmd.append(cModuleCmd)

    javaModuldCmd = '%s "DtvmTestAdaper %s %s %s %s %s %s %s " '%('./hmctest_p',stbip[0],svnupCopyFlag[0],copyLibraryFlag[0],makeCleanFlag[0],makeFlag[0],mmcpPath[0],temphmc)
    javaModuldCmd += '" %s %s %s %s %s " "%s -DautoTest=%s" '%(hmcPath[0], hmcApplicationPath[0],hmcProductPath[0],hmcBinExeDefineFile[0],hmcExecutepath[0], hmcCommand[0],bAutoTestFlag[0] )
    testCmd.append(javaModuldCmd)
    
    javaModuldCmd = '%s  " DtvmTestCase %s %s %s %s %s %s %s " '%('./hmctest_p',stbip[0],svnupCopyFlag[0],copyLibraryFlag[0],makeCleanFlag[0],makeFlag[0],mmcpPath[0],temphmc)
    javaModuldCmd += '" %s %s %s %s %s " "%s -DautoTest=%s" '%(hmcPath[0], hmcApplicationPath[0],hmcProductPath[0],hmcBinExeDefineFile[0],hmcExecutepath[0], hmcCommand[0],bAutoTestFlag[0] )
    testCmd.append(javaModuldCmd)

def execute_javaAdapter():
    for cmd in testCmd:
       if cmd.find("DtvmTestAdaper") > 0:
          os.system(cmd) 
          break         

def execute_javaCase():
    for cmd in testCmd:
       if cmd.find("DtvmTestCase") > 0:
          os.system(cmd)
          break
          
def stb_report():    
    reportCmd = '%s %s %s %s %s '%('./autotest_p.sh -report ', mmcpPath[0],hmcPath[0],hmcReportpath[0],hmcReportCasepath[0])
    os.system(reportCmd)
    
def stb_reboot():    
    rebootCmd = '%s %s %s'%('./reboot',stbip[0],mmcpPath[0])
    os.system(rebootCmd)
    
def print_parameter():
    print '----------------------------------------------------------'
    print '             stbip = ', stbip[0]
    print '     svnupCopyFlag = ',svnupCopyFlag[0]
    print '   copyLibraryFlag = ',copyLibraryFlag[0]
    print '     makeCleanFlag = ',makeCleanFlag[0]
    print '          makeFlag = ',makeFlag[0]
    
    print '    mountTrunkPath = ', mountTrunkPath[0]
    print '  mountPortingPath = ', mountPortingPath[0]
    print '       mountCAPath = ', mountCAPath[0]
    print '    srcIncludePath = ', srcIncludePath[0]
    print '        srcJarPath = ', srcJarPath[0]
    print '          srcAPath = ', srcAPath[0]
    print '      bFixDateFlag = ', bFixDateFlag[0]
    print '   srcPortingADate = ', srcPortingADate[0]
    print '        srcCaADate = ', srcCaADate[0]
    print '   srcPortingAPath = ', srcPortingAPath[0]
    print '        srcCaAPath = ', srcCaAPath[0]
    
    print '          mmcpPath = ', mmcpPath[0]
    print '        mmcpEnvCmd = ', mmcpEnvCmd[0]
    print '   mmcpIncludePath = ', mmcpIncludePath[0]
    print '       mmcpJarPath = ', mmcpJarPath[0]
    print '         mmcpAPath = ', mmcpAPath[0]
    print '    testModulePath = ',testModulePath[0]
    print '    testmodel_list = ',testmodel_list[0]
    print '   argv_debugLevel = ',argv_debugLevel[0] 
    
    print '           hmcPath = ',hmcPath[0]
    print '    hmcIncludePath = ',hmcIncludePath[0]
    print '        hmcJarPath = ',hmcJarPath[0]
    print '          hmcAPath = ',hmcAPath[0]
    print 'hmcApplicationPath = ',hmcApplicationPath[0]
    print 'hmcBinExeDefineFile= ',hmcBinExeDefineFile[0]
    print '    hmcProductPath = ',hmcProductPath[0]
    print '    hmcExecutepath = ',hmcExecutepath[0]
    print '     hmcReportpath = ',hmcReportpath[0]
    print ' hmcReportCasepath = ',hmcReportCasepath[0]
    print '     bAutoTestFlag = ',bAutoTestFlag[0]
    print '        hmcCommand = ',hmcCommand[0]
    
    print '  time_hour_minute = ',time_hour_minute[0]
    print '       mailSubject = ',mailSubject[0]
    print '        mailSender = ',mailSender[0]
    print '      mailReceiver = ',mailReceiver[0]
    print ' mailServerAddress = ',mailServerAddress[0]
    print '----------------------------------------------------------'
    
def reset_value():
    print '-------reset value-------'
    if len(stbip) > 0:
       del stbip[0]
    if len(svnupCopyFlag) > 0:
       del svnupCopyFlag[0]
    if len(copyLibraryFlag) > 0:
       del copyLibraryFlag[0]
    if len(makeCleanFlag) > 0:
       del makeCleanFlag[0]
    if len(makeFlag) > 0:
       del makeFlag[0]     
       
    if len(mountTrunkPath) > 0:
       del mountTrunkPath[0]
    if len(mountPortingPath) > 0:
       del mountPortingPath[0]
    if len(mountCAPath) > 0:
       del mountCAPath[0]
    if len(srcIncludePath) > 0:
       del srcIncludePath[0]
    if len(srcJarPath) > 0:
       del srcJarPath[0]
    if len(srcAPath) > 0:
       del srcAPath[0]          
    if len(bFixDateFlag) > 0:
       del bFixDateFlag[0] 
    if len(srcPortingADate) > 0:
       del srcPortingADate[0] 
    if len(srcCaADate) > 0:
       del srcCaADate[0]    
    if len(srcPortingAPath) > 0:
       del srcPortingAPath[0] 
    if len(srcCaAPath) > 0:
       del srcCaAPath[0]    
       
    if len(mmcpPath) > 0:
       del mmcpPath[0]
    if len(mmcpEnvCmd) > 0:
       del mmcpEnvCmd[0]
    if len(mmcpIncludePath) > 0:
       del mmcpIncludePath[0]
    if len(mmcpJarPath) > 0:
       del mmcpJarPath[0]
    if len(mmcpAPath) > 0:
       del mmcpAPath[0] 
    if len(testModulePath) > 0:
       del testModulePath[0]
    if len(testmodel_list) > 0:
       del testmodel_list[0]
    if len(argv_debugLevel) > 0:
       del argv_debugLevel[0]
       
    if len(hmcPath) > 0:
       del hmcPath[0]
    if len(hmcIncludePath) > 0:
       del hmcIncludePath[0]
    if len(hmcJarPath) > 0:
       del hmcJarPath[0]
    if len(hmcAPath) > 0:
       del hmcAPath[0]
    if len(hmcApplicationPath) > 0:
       del hmcApplicationPath[0]
    if len(hmcProductPath) > 0:
       del hmcProductPath[0]
    if len(hmcBinExeDefineFile) > 0:
       del hmcBinExeDefineFile[0]   
    if len(hmcExecutepath) > 0:
       del hmcExecutepath[0]
    if len(hmcReportpath) > 0:
       del hmcReportpath[0]
    if len(hmcReportCasepath) > 0:
       del hmcReportCasepath[0]
    if len(bAutoTestFlag) > 0:
       del bAutoTestFlag[0]
    if len(hmcCommand) > 0:
       del hmcCommand[0]
       
    if len(time_hour_minute) > 0:
       del time_hour_minute[0]
    if len(mailSubject) > 0:
       del mailSubject[0]
    if len(mailSender) > 0:
       del mailSender[0]
    if len(mailReceiver) > 0:
       del mailReceiver[0]
    if len(mailServerAddress) > 0:  
       del mailServerAddress[0]
    

def writeConfig():
    if os.path.exists('parameter_test') == True:
       os.remove('parameter_test')
    fw = open(parameterName,'w')
    
    stuff = 'stbip=' +stbip[0] + '\n'
    stuff += 'svnupCopyFlag=' +svnupCopyFlag[0] + '\n'
    stuff += 'copyLibraryFlag=' +copyLibraryFlag[0] + '\n'
    stuff += 'makeCleanFlag=' +makeCleanFlag[0] + '\n'
    stuff += 'makeFlag=' +makeFlag[0] + '\n'

    stuff += 'mountTrunkPath=' +mountTrunkPath[0] + '\n'
    stuff += 'mountPortingPath=' +mountPortingPath[0] + '\n'
    stuff += 'mountCAPath=' +mountCAPath[0] + '\n'
    stuff += 'srcIncludePath=' +srcIncludePath[0] + '\n'
    stuff += 'srcJarPath=' +srcJarPath[0] + '\n'
    stuff += 'srcAPath=' +srcAPath[0] + '\n'
    stuff += 'bFixDateFlag=' +bFixDateFlag[0] + '\n'
    stuff += 'srcPortingADate=' +srcPortingADate[0] + '\n'
    stuff += 'srcCaADate=' +srcCaADate[0] + '\n'
    stuff += 'srcPortingAPath=' +srcPortingAPath[0] + '\n'
    stuff += 'srcCaAPath=' +srcCaAPath[0] + '\n'
    
    stuff += 'mmcpPath=' +mmcpPath[0] + '\n'
    stuff += 'mmcpEnvCmd=' +mmcpEnvCmd[0] + '\n'
    stuff += 'mmcpIncludePath=' +mmcpIncludePath[0] + '\n'
    stuff += 'mmcpJarPath=' +mmcpJarPath[0] + '\n'
    stuff += 'mmcpAPath=' +mmcpAPath[0] + '\n'
    stuff += 'testModulePath=' +testModulePath[0] + '\n'
    stuff += 'testmodel_list=' +testmodel_list[0] + '\n'
    stuff += 'argv_debugLevel=' +argv_debugLevel[0] + '\n'

    stuff += 'hmcPath=' +hmcPath[0] + '\n'
    stuff += 'hmcIncludePath=' +hmcIncludePath[0] + '\n'
    stuff += 'hmcJarPath=' +hmcJarPath[0] + '\n'
    stuff += 'hmcAPath=' +hmcAPath[0] + '\n'
    stuff += 'hmcApplicationPath=' +hmcApplicationPath[0] + '\n'
    stuff += 'hmcProductPath=' +hmcProductPath[0] + '\n'
    stuff += 'hmcBinExeDefineFile=' +hmcBinExeDefineFile[0] + '\n'
    stuff += 'hmcExecutepath=' +hmcExecutepath[0] + '\n'
    stuff += 'hmcReportpath=' +hmcReportpath[0] + '\n'
    stuff += 'hmcReportCasepath=' +hmcReportCasepath[0] + '\n'
    stuff += 'bAutoTestFlag=' +bAutoTestFlag[0] + '\n'
    stuff += 'hmcCommand=' +hmcCommand[0] + '\n'
    
    stuff += 'time_hour_minute=' + time_hour_minute[0] + '\n'
    stuff += 'mailSubject=' +mailSubject[0] + '\n'
    stuff += 'mailSender=' +mailSender[0] + '\n'
    stuff += 'mailReceiver='+mailReceiver[0] + '\n'
    stuff += 'mailServerAddress='+ mailServerAddress[0] +'\n'
    stuff += 'mailTail='+ mailHead[0]+'\n'
    stuff += 'mailTail='+ mailTail[0]+'\n'
    fw.write(stuff)
    fw.close()

def setFlag(value):
   if len(setFlagValue) > 0:
      del setFlagValue[0]
   setFlagValue.append(value)  
      
def readConfig():
    if os.path.exists('parameter_test') == False:
       shutil.copy( 'parameter_default', 'parameter_test' ) 
    reset_value()
    fr=open(parameterName,'r')
#    print '---readConfig'
    bMoreHeadLines = False
    bMoreTailLines = False
    while(True):
         line_split=[]
         line = fr.readline()
         print '---', line
         if len(line) == 0:
            break
         line_split = line[0:line.find("\n")].split("=")
         headIndex = line. find('[')
         tailIndex = line. find(']')
         if len(line_split) > 1:
            bMoreHeadLines = False
            bMoreTailLines = False
            if line_split[0] == 'stbip':
               stbip.append( line_split[1])
            if line_split[0] == 'svnupCopyFlag':
               svnupCopyFlag.append( line_split[1])
            if line_split[0] == 'copyLibraryFlag':
               copyLibraryFlag.append( line_split[1])
            if line_split[0] == 'makeCleanFlag':
               makeCleanFlag.append( line_split[1])
            if line_split[0] == 'makeFlag':
               makeFlag.append( line_split[1])
         
            if line_split[0] == 'mountTrunkPath':
               mountTrunkPath.append( line_split[1]) 
            if line_split[0] == 'mountPortingPath':
               mountPortingPath.append( line_split[1]) 
            if line_split[0] == 'mountCAPath':
               mountCAPath.append( line_split[1]) 
            if line_split[0] == 'srcIncludePath':
               srcIncludePath.append( line_split[1]) 
            if line_split[0] == 'srcJarPath':
               srcJarPath.append( line_split[1]) 
            if line_split[0] == 'srcAPath':
               srcAPath.append( line_split[1]) 
            if line_split[0] == 'bFixDateFlag':
               bFixDateFlag.append( line_split[1])
            if line_split[0] == 'srcPortingADate':
               srcPortingADate.append( line_split[1])
            if line_split[0] == 'srcCaADate':
               srcCaADate.append( line_split[1])   
            if line_split[0] == 'srcPortingAPath':
               srcPortingAPath.append( line_split[1])
            if line_split[0] == 'srcCaAPath':
               srcCaAPath.append( line_split[1])   
               
            if line_split[0] == 'mmcpPath':
               mmcpPath.append( line_split[1])              
            if line_split[0] == 'mmcpEnvCmd':
               mmcpEnvCmd.append( line_split[1])
            if line_split[0] == 'mmcpIncludePath':
               mmcpIncludePath.append( line_split[1])
            if line_split[0] == 'mmcpJarPath':
               mmcpJarPath.append( line_split[1])
            if line_split[0] == 'mmcpAPath':
               mmcpAPath.append( line_split[1])   
            if line_split[0] == 'testModulePath':
               testModulePath.append( line_split[1])
            if line_split[0] == 'testmodel_list':
               testmodel_list.append( line_split[1])
            if line_split[0] == 'argv_debugLevel':
               argv_debugLevel.append( line_split[1])
               
            if line_split[0] == 'hmcPath':
               hmcPath.append( line_split[1])
            if line_split[0] == 'hmcIncludePath':
               hmcIncludePath.append( line_split[1])
            if line_split[0] == 'hmcJarPath':
               hmcJarPath.append( line_split[1])
            if line_split[0] == 'hmcAPath':
               hmcAPath.append( line_split[1])   
            if line_split[0] == 'hmcApplicationPath':
               hmcApplicationPath.append( line_split[1])
            if line_split[0] == 'hmcProductPath':
               hmcProductPath.append( line_split[1])               
            if line_split[0] == 'hmcBinExeDefineFile':
               hmcBinExeDefineFile.append( line_split[1])              
            if line_split[0] == 'hmcExecutepath':
               hmcExecutepath.append( line_split[1])              
            if line_split[0] == 'hmcReportpath':
               hmcReportpath.append( line_split[1])               
            if line_split[0] == 'hmcReportCasepath':
               hmcReportCasepath.append( line_split[1])              
            if line_split[0] == 'bAutoTestFlag':
               bAutoTestFlag.append( line_split[1])  
                         
            if line_split[0] == 'hmcCommand':
               stringCmd = line[0:line.find("\n")]
               lenStrCmd = len(stringCmd)
               firstEq = 1 + stringCmd.find("=")
               hmcCommand.append(stringCmd[firstEq:lenStrCmd])
         
            if line_split[0] == 'time_hour_minute':
               time_hour_minute.append( line_split[1])
               
            if line_split[0] == 'mailSubject':
               mailSubject.append( line_split[1])
               
            if line_split[0] == 'mailSender':
               mailSender.append( line_split[1])
               
            if line_split[0] == 'mailReceiver':
               mailReceiver.append( line_split[1])
               
            if line_split[0] == 'mailServerAddress':
               mailServerAddress.append( line_split[1])

            if line_split[0] == 'mailHead':
                if tailIndex < 0:
                    mailHead.append(line[headIndex+1:line.find('\n')])
                    bMoreHeadLines = True
                else:
                    mailHead.append(line[headIndex+1:tailIndex])
            if line_split[0] == 'mailTail':
                if tailIndex < 0:
                    mailTail.append(line[headIndex+1:line.find('\n')])
                    bMoreTailLines = True
                else:
                    if headIndex < 0:
                        mailTail.append(line[headIndex+1:tailIndex])
         else:
             if bMoreHeadLines == True:
                if tailIndex < 0:
                     mailHead.append(line[0:line.find('\n')])
                else:
                    bMoreHeadLines = False
                    mailHead.append(line[0:tailIndex])   
             if bMoreTailLines == True:
                if tailIndex < 0:
                    mailTail.append(line[0:line.find('\n')])
                else:
                    mailTail.append(line[0:tailIndex])
                    bMoreTailLines = False
    fr.close()
    
def set_default():
    print '-------set default-------'
    reset_value()
    stbip.append('172.30.50.234')
    svnupCopyFlag.append('false')
    copyLibraryFlag.append('true')
    makeCleanFlag.append('true')
    makeFlag.append('true')

    mountTrunkPath.append('10.10.96.156:/home/pub/cc1_mmcp_all/mmcp_platform_release_bcm740x')
    mountPortingPath.append('10.10.96.156:/home/pub/cc2_cbb/porting_bcm')
    mountCAPath.append('10.10.96.156:/home/pub/cc2_cbb/trunk_ca')
    srcIncludePath.append('lib/include_release')
    srcJarPath.append('lib/lib/dtvmxjar')
    srcAPath.append('lib/lib/bcm740x_def')
    bFixDateFlag.append('true')
    srcPortingADate.append('20100525173431')
    srcCaADate.append('20100601153957')
    srcPortingAPath.append('lib/bcm740x')
    srcCaAPath.append('lib/bcm740x')

    mmcpPath.append('/home/wangqilin/trunk')
    mmcpEnvCmd.append('bin/envfile_fortrunk/setenv_bcm740x_release')
    mmcpIncludePath.append('include')
    mmcpJarPath.append('dtvmx/lib')
    mmcpAPath.append('lib/bcm740x')
    testModulePath.append('test')
    testmodel_list.append('codec kernel pvr dtv mediaplayer protocol ')
    argv_debugLevel.append('0')

    hmcPath.append('/home/wangqilin/HMC_V100R002_HD_tmp')
    hmcIncludePath.append('platform/include')
    hmcJarPath.append('application/javacode/ShareLib/lib')
    hmcAPath.append('platform/lib/bcm740x')
    hmcApplicationPath.append('application/javacode')
    hmcProductPath.append('product/N8606C_TF')
    hmcBinExeDefineFile.append('product/N8606C_TF/code/privateDefine.h')
    hmcExecutepath.append('platform/integration/hmc/javaresources/ROOTFS')
    hmcReportpath.append('platform/integration/hmc/javaresources/JFFS/flash/com_coship_app_dtvm_test_adaper_1.1')
    hmcReportCasepath.append('platform/integration/hmc/javaresources/JFFS/flash/com_coship_app_dtvm_testcase_1.0')
    bAutoTestFlag.append('true')
    hmcCommand.append('main.out ')

    time_hour_minute.append('19:02')
    mailSubject.append('bcm740x_autoTest_report')
    mailSender.append('robot_client_Jack@coship.com')
#    mailReceiver.append('yezhenhong@coship.com')
    mailReceiver.append('wangqilin@coship.com;yezhenhong@coship.com;wangdong@coship.com;tongfulei@coship.com;wangmin@coship.com;luojinhua@coship.com;zhengfen@coship.com;yanghuiyuan@coship.com;chenlin@coship.com;zhaodemin@coship.com;zhangcaihong@coship.com;zhangying@coship.com;wangwenbo@coship.com;zhangyahao@coship.com;xiejun@coship.com;chenrui@coship.com;wanhuiling@coship.com;tanguiyong@coship.com;zhuyaoguang@coship.com;yirui@coship.com')
    mailServerAddress.append('mailserver.coship.com:25')
    print_parameter()
    wiriteCheck = raw_input("please confirm save the parameter y/n(default: y):")
    if wiriteCheck =='n':
       readConfig()
    else:
       writeConfig()
def change_parameter():
           ipParameter = raw_input("please input stb ip(default: [172.30.50.234 ] :  ")
           if ipParameter != "":
              stbip[0] = ipParameter

           svnupCopyFlagInput = raw_input("please input svnupCopyFlag true/false(default :[ false ] :")
           if svnupCopyFlagInput != "":
              svnupCopyFlag[0] = svnupCopyFlagInput
              
           copyLibraryFlagInput = raw_input("please input copyLibraryFlag true/false(default:[ false ]:")
           if copyLibraryFlagInput != "":
              copyLibraryFlag[0] = copyLibraryFlagInput
              
           makeCleanFlagInput = raw_input("please input makeCleanFlag true/false(default:[ false ]:")
           if makeCleanFlagInput != "":
              makeCleanFlag[0] = makeCleanFlagInput
              
           makeFlagInput = raw_input("please input makeFlag true/false(default: [ false ] :")
           if makeFlagInput != "":
              makeFlag[0] = makeFlagInput  
    
           mountpathInput = raw_input("please input mountTrunkPath (default :[ 10.10.96.156:/home/pub/cc1_mmcp_all/mmcp_platform_release_bcm740x ] :")
           if mountpathInput != "":
              mountTrunkPath[0] = mountpathInput  
                     
           mountPortingPathInput = raw_input("please input mountTrunkPath (default :[ 10.10.96.156:/home/pub/cc2_cbb/porting_bcm ] :")
           if mountPortingPathInput != "":
              mountPortingPath[0] = mountPortingPathInput
              
           mountCAPathInput = raw_input("please input mountTrunkPath (default :[ 10.10.96.156:/home/pub/cc2_cbb/trunk_ca ] :")
           if mountCAPathInput != "":
              mountCAPath[0] = mountCAPathInput
           
           srcIncludePathInput = raw_input("please input srcIncludePath (default :[ lib/include_release ] :")
           if srcIncludePathInput != "":
              srcIncludePath[0] = srcIncludePathInput
           
           srcJarPathInput = raw_input("please input srcJarPath (default :[ lib/lib/dtvmxjar ] :")
           if srcJarPathInput != "":
              srcJarPath[0] = srcJarPathInput
              
           srcAPathInput = raw_input("please input srcAPath (default :[ lib/lib/bcm740x_def ] :")
           if srcAPathInput != "":
              srcAPath[0] = srcAPathInput 
              
           bFixDateFlagInput = raw_input("please input bFixDateFlag (default :[ true ] :")
           if bFixDateFlagInput != "":
              bFixDateFlag[0] = bFixDateFlagInput
           srcPortingAPathInput = raw_input("please input srcPortingADate (default :[ 20100525173431 ] :")
           if srcPortingADateInput != "":
              srcPortingADate[0] = srcPortingADateInput
           srcCaADateInput = raw_input("please input srcCaADate (default :[ 20100525173431 ] :")
           if srcCaADateInput != "":
              srcCaADate[0] = srcCaADateInput   
              
           srcPortingAPathInput = raw_input("please input srcPortingAPath (default :[ lib/bcm740x ] :")
           if srcPortingAPathInput != "":
              srcPortingAPath[0] = srcPortingAPathInput
           srcCaAPathInput = raw_input("please input srcCaAPath (default :[ lib/bcm740x ] :")
           if srcCaAPathInput != "":
              srcCaAPath[0] = srcCaAPathInput           
    
           pathCurrent=os.getcwd()
           print 'current work path: ',pathCurrent
           mmcpPath[0] = re.sub('/test','',pathCurrent,0) 
               
           mmcpEnvCmdInput = raw_input("please input mmcpEnvCmd (default: [bin/envfile_fortrunk/setenv_bcm740x_release ] :")
           if mmcpEnvCmdInput != "":
              mmcpEnvCmd[0] = '%s'%(mmcpEnvCmdInput )    
           
           mmcpIncludePathInput = raw_input("please input mmcpIncludePath (default: [include ] :")
           if mmcpIncludePathInput != "":
              mmcpIncludePath[0] = '%s'%(mmcpIncludePathInput ) 
           mmcpJarPathInput = raw_input("please input mmcpJarPath (default: [dtvmx/lib ] :")
           if mmcpJarPathInput != "":
              mmcpJarPath[0] = '%s'%(mmcpJarPathInput ) 
           mmcpAPathInput = raw_input("please input mmcpAPath (default: [lib/bcm740x ] :")
           if mmcpAPathInput != "":
              mmcpAPath[0] = '%s'%(mmcpAPathInput )  
           testModulePathName = raw_input("please input module path(default: [test ] :")
           if testModulePathName != "":
              testModulePath[0] = '"%s"'%(testModulePathName )                     
           moduleName = raw_input("please input module name(default: [codec kernel pvr dtv mediaplayer protocol ] :")
           if moduleName != "":
              testmodel_list[0] = '"%s"'%(moduleName )
           
           argv_debugLevelInput = raw_input("please input c modules debug level (default: [0 ] :")
           if argv_debugLevelInput != "":
              argv_debugLevel[0] = argv_debugLevelInput                                  
       
           hmcPathInput = raw_input("please input hmc path(default: [/home/yezhenhong/HMC_V100R002_HD_tmp ] :")
           if hmcPathInput != "":
              hmcPath[0] = hmcPathInput
           
           hmcIncludePathInput = raw_input("please input hmcIncludePath(default: [platform/include ] :")
           if hmcIncludePathInput != "":
              hmcIncludePath[0] = hmcIncludePathInput
           hmcJarPathInput = raw_input("please input hmcJarPath(default: [application/javacode/ShareLib/lib ] :")
           if hmcJarPathInput != "":
              hmcJarPath[0] = hmcJarPathInput
           hmcAPathInput = raw_input("please input hmcAPath(default: [platform/lib/bcm740x ] :")
           if hmcAPathInput != "":
              hmcAPath[0] = hmcAPathInput
              
           hmcApplicationPathInput = raw_input("please input hmc application path(default: [application/javacode ] :")
           if hmcApplicationPathInput != "":
              hmcApplicationPath[0] = hmcApplicationPathInput
              
           hmcProductPathInput = raw_input("please input hmc product path(default: [product/topway ] :")
           if hmcProductPathInput != "":
              hmcProductPath[0] = hmcProductPathInput
              
           hmcBinExeDefineFileInput = raw_input("please input hmc product path(default: [product/N8606C_TF/code/privateDefine.h ] :")
           if hmcBinExeDefineFileInput != "":
              hmcBinExeDefineFile[0] = hmcBinExeDefineFileInput  
  
           hmcExecutepathInput = raw_input("please input hmc execute path(default: [integration/hmc/javaresources/ROOTFS ] :")
           if hmcExecutepathInput != "":
              hmcExecutepath[0] = hmcExecutepathInput
          
           hmcReportpathInput = raw_input("please input DtvmTestAdper application path(default: [integration/hmc/javaresources/JFFS/flash/com_coship_app_dtvm_test_adaper_1.1 ] :")
           if hmcReportpathInput != "":
              hmcReportpath[0] =  hmcReportpathInput
          
           hmcReportCasepathInput = raw_input("please input DtvmTestCase application path(default: [integration/hmc/javaresources/JFFS/flash/com_coship_app_dtvm_test_case_1.0 ] :")
           if hmcReportCasepathInput != "":
              hmcReportCasepath[0] =  hmcReportCasepathInput
              
           bAutoTestFlagInput = raw_input("please input auto test java flag true/false(default: [ true] :")
           if bAutoTestFlagInput != "":
              bAutoTestFlag[0] = bAutoTestFlagInput
           
           hmcCommandInput = raw_input("please input hmc execute command (default: [ main.out] :")
           if hmcCommandInput != "":
              hmcCommand[0] = hmcCommandInput
              
                 
           timeSet = raw_input("please input trigger time (default is [21:35]):  ")
           if timeSet != "":
              time_hour_minute[0] = timeSet
                 
           mailSubjectInput = raw_input("please input mail subject (default: [ bcm740x_autoTest_report] :")
           if mailSubjectInput != "":
              mailSubject[0] = mailSubjectInput
              
           mailSenderInput = raw_input("please input mail sender (default: [ yezhenhong@coship.com ] :")
           if mailSenderInput != "":
              mailSender[0] = mailSenderInput
              
           mailReceiverInput = raw_input("please input mail receivers (default: [ yezhenhong@coship.com;] :")
           if mailReceiverInput != "":
              mailReceiver[0] = mailReceiverInput
              
           mailServerAddressInput = raw_input("please input mail server (default: [ mailserver.coship.com:25 ] :")
           if mailServerAddressInput != "":
              mailServerAddress[0] = mailServerAddressInput  
           print_parameter()
           wiriteCheck = raw_input("please confirm save the parameter y/n(default is y):")
           if wiriteCheck =='n':
              readConfig()
           else:
              writeConfig() 
              
def sleep( mytime):
    time.sleep( mytime )
    
def print_envirment():
        print_parameter()
        print '----------------------------------------------------------'
        print 'cmd description :'
        print '   [  c  ] change envirment parameters ; '
        print '   [  r  ] run thread by timer trigger(required stb owns fix ip address); '
        print '   [  rc ] run c module immediately ;    '
        print '   [ rjd ] run java dtvmTestAdaper module immediately ; '
        print '   [ rjt ] run java DtvmTestCase module immediately ; '
        print '   [  rm ] run mail service immediately ; '
        print '   [  rs ] reset default parameter ; '
        print '   [  re ] remove log and report file; '
        print '   [  rb ] reboot the set top box,but make sure the stb is in connected state';
        print '   [ "ctrl" + "c" ] terminate the thread!'
        print '   default value is [ r ]                '
        print 'warning: please setup ur own envirment for the first entry! '
        print '----------------------------------------------------------'
        print '----------------------------------------------------------'
def init_envirment():  
    tempCmd = '' 
    readConfig() 
    print_envirment()  
     
def reset_cmd():
   if len(setFlagValue) > 0:  
      del setFlagValue[0]
def check_cmd():
   cmdpool = ['', 'r', 'rs', 'c', 'cc', 'rt',  're', 'rc', 'rb', 'rjd', 'rjt',  'rm','maxcmd']
   if len(setFlagValue) == 0:
      while(1):
        tempCmd = raw_input("change input command (default: r):")  
        for cmd in cmdpool:
           if cmd == tempCmd:
               if tempCmd == '':
                  setFlag('r')
               else:
                  setFlag(tempCmd)     
               break
        print 'unknown input command!'
        if len(setFlagValue) > 0:
           break 
#   print 'current_cmd=',setFlagValue[0] 
        
def send_test_mail():
#     attachments=glob.glob('*.log') #too big, the maximum is 1.5Mbit in inner mail system
     attachments=[]
     print os.getcwd()
     reportfile= os.listdir('testdata')
     for f in reportfile:  
         if None != re.search('testReport_',f):  
            attachments.append('testdata/'+f)
     logFileName = 'test_record.txt'
     excelFileName = 'testdata/report.xls'
     excelReport.createLogFile(attachments, logFileName)
     RecordTest.recordResult(logFileName)
     result = excelReport.createExcel(logFileName, excelFileName)
     attachments.append(excelFileName)
     mailContent = mailAgent.createMailContent(mailHead, result, mailTail)
     mailAgent.sendMailToNotes(mailSubject,mailSender,mailReceiver, mailContent,attachments, mailServerAddress[0]) 
     
     compileSubject  = ['compile log']
     complilefiles = []
     fileNameZip = 'compile_log.zip'
     zf = zipfile.ZipFile(fileNameZip, "w", zipfile.zlib.DEFLATED)
     for f in reportfile:  
         if None != re.search('testcompile_',f):  
         #   complilefiles.append('testdata/'+f)
            zf.write('testdata/'+f)
     zf.close()
     complilefiles.append(fileNameZip)
     mailAgent.sendMailToNotes(compileSubject,mailSender,mailReceiver, "all compiling logs for u sir !! ",complilefiles, mailServerAddress[0]) 
     
def remove_test_report(dir, keyword):
   #'' == in current path
   if dir == '':
      logfile= glob.glob(keyword)
      for file in logfile:
          os.remove(file)
   else:
      reportfile= os.listdir(dir)
      for f in reportfile:  
         if None != re.search(keyword,f):
            os.remove('%s/%s'%(dir,f))
def remove_all_record():
    remove_test_report('','*.log')
    remove_test_report('testdata','testReport')
    remove_test_report('testdata','*.log')
    if os.path.exists('modelEndTmp') == True:
       shutil.rmtree('modelEndTmp')        
#--------------------thread----------------------------------
class autoTestThread(threading.Thread):
     def __init__(self):
         threading.Thread.__init__(self)
         self.mailTime = maxDuration  
         self._stop = threading.Event()
         self.count =0
   
     def stop (self):
         self._stop.set()
 
     def stopped (self):
         return self._stop.isSet()
         
     def run(self):
         testCmdCount=0
         sendMailTmp=False
         bMail = False
         while(1):
             if self.stopped():
                 break
             if self.count > 0:
                self.count -=1
#             print 'setFlagValue=',setFlagValue
             if self.count == 1:
                # send mail
                if bMail == True: 
                   if testCmdCount >= len(testCmd):
                      print "----------all models finish---------------"
                      bMail = False
                      send_test_mail()
                      if sendMailTmp == True:
                         sendMailTmp=False
                      if os.path.exists('modelEndTmp') == True:
                         shutil.rmtree('modelEndTmp')
                      testCmdCount = 0
 #                     reset_cmd()
                      continue
                   else:
                      print "----------one model timeout---------------cmdcount=",testCmdCount
                      print 'totalcmd=',len(testCmd) 
                      if os.path.exists('modelEndTmp') == True:
                         shutil.rmtree('modelEndTmp')
                      if setFlagValue[0] == "r":
                         stb_reboot()
                         stb_report()
                         sleep(5)
                      self.count = self.mailTime                     
                      os.system(testCmd[testCmdCount]) 
                      testCmdCount +=1
                      
             strTime = time.localtime()  
             #-------------------------------------------------------------------
             check_cmd()
             #-----------------------run time clock-------------------------------      
             if setFlagValue[0] == "r":
                timeTmp = time_hour_minute[0].split(':')
                if strTime[3] == string.atoi(timeTmp[0]):
                   if strTime[4] == string.atoi(timeTmp[1]):
                      if self.count == 0:
                         self.count = self.mailTime
                         bMail = True
                         remove_all_record()
                         os.system(testCmd[testCmdCount])
                         testCmdCount +=1
                         print "start test:"
              #-----------------------reset envirment------------------------------- 
             if setFlagValue[0] == "rs":
                set_default()
                reset_cmd()
                continue
               #-----------------------change envirement parameter------------------------------- 
             if setFlagValue[0] == "c":
                change_parameter()
                reset_cmd()  
                continue
              #-----------------------run test cmd 2------------------------------- 
             if setFlagValue[0] == "cc":
             #   scp /home/yezhenhong/trunk/test/readme /home/yezhenhong/trunk/test/testdata
                os.system('./autotest_p.sh -cc')
                reset_cmd()  
                continue
               #-----------------------run set current time------------------------------- 
             if setFlagValue[0] == "rt":
          #      del time_hour_minute[0]
          #      strTestTime = '%d:%d'%(strTime[3],strTime[4])
          #      time_hour_minute.append(strTestTime)
                reset_cmd()
                setFlagValue.append("r")
                self.count = self.mailTime
                bMail = True
                remove_all_record()
                os.system(testCmd[testCmdCount])
                testCmdCount +=1
                print "start test:"
                continue
              #-----------------------run remove all log and report files-------------------------------            
             if setFlagValue[0] == "re":
               # remove_all_record()
                stb_report()
                reset_cmd()
                continue
            
              #-----------------------run c module immediatelly-------------------------------            
             if setFlagValue[0] == "rc":
                if self.count == 0:
                   self.count = self.mailTime
                   os.system(testCmd[0])
              #-----------------------run reboot stb immediatelly-------------------------------         
             if setFlagValue[0] == "rb":
                stb_reboot()
                reset_cmd()
                continue
              #-----------------------run java module immediatelly-------------------------------           
             if setFlagValue[0] == "rjd":  
                if self.count == 0:
                   self.count = self.mailTime
                   execute_javaAdapter()   
             if setFlagValue[0] == "rjt":  
                if self.count == 0:
                   self.count = self.mailTime
                   execute_javaCase()
              #-----------------------run mail module immediatelly-------------------------------               
             if setFlagValue[0] == "rm":
              #  if self.count == 0:
                bMail = True
                self.count = self.mailTime
                sendMailTmp=True
                reset_cmd()
                setFlagValue.append('r')
              #-----------------------run win32 cmd immediatelly-------------------------------               
             if setFlagValue[0] == "rw":
  #              os.system('/10.10.98.199:/Coship-EW300-Simulator/EW300Simulator.exe')
                ftp=ftplib.FTP()
                ftp.connect("10.10.96.156",21)
                ftp.login("localhost.localdomain","localhost")
                reset_cmd()  
           #---------------------------------------------------------------------------------------
             if os.path.exists('modelEndTmp') == True:
                  shutil.rmtree('modelEndTmp')                
                  print "----------one model end---------------"
                  if setFlagValue[0] == "r":
                     stb_reboot()
                     stb_report()
                     sleep(5)
                     self.count = self.mailTime
                     if testCmdCount >= len(testCmd):
                         sendMailTmp = True
                     else:
                        stb_reboot() 
                        os.system(testCmd[testCmdCount]) 
                        testCmdCount +=1                   
                  else:
                     reset_cmd()
                     continue
            
             if sendMailTmp == True: 
                print "----------sendMailTmp flag---------------"
                send_test_mail()
                sendMailTmp=False
                bMail = False
                self.count = 0
             print 'count(s)=',self.count
             sys.stdout.flush()
             time.sleep(1)
#-------------------------main------------------------------------
def main(argv):
     init_envirment()
     setCmd()
     current = autoTestThread()
     current.setDaemon(True)
     current.start()
 
     try:
         while(1): pass
     except KeyboardInterrupt:
         print "Exiting program.."
         current.stop()
         print "Thread  finished." 
         sys.exit()
 
#----------------------------entrance------------------------------------------
if __name__ == "__main__":
     main(sys.argv[1:])

