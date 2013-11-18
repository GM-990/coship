#!/usr/bin/python
# -*- coding: cp936 -*-
#yzhenhong 904305
import os,re,subprocess
import shutil
import glob
import xlrd
import xlwt
import xlwt
import time
import string 
from datetime import datetime

dateName = '日期: '
module = '模块'
stateName = '状态'
moduleState = ['未测','已测','未完测']
moduleTotalName = ['测试总数','通过总数','通过率%']

def GetFormateTime():
    strLocalTime = time.localtime(time.time())
    strTime =str(strLocalTime.tm_year)+'/'+str(strLocalTime.tm_mon)+'/'+str(strLocalTime.tm_mday)+' '+str(strLocalTime.tm_hour)+':'+str(strLocalTime.tm_min)
 #   print '--time---',strTime
    return strTime
class stUnit:
    def __init__(self, name, subModule):
        self.name = name
        self.numModules = len(subModule)
        self.lineLen = 5
        self.subModule = subModule
        self.count = 0
        self.value=[]
    def createNew(self):
    #    self.value.append(GetFormateTime())
        self.value.append(self.name)
        self.value.append(moduleState[1])
    def createNewBlank(self):
     #   self.value.append(GetFormateTime())
        self.value.append(self.name)
        self.value.append(moduleState[2])
        self.value.append(0)
        self.value.append(0)
        self.value.append(0)
    def noPassAll(self):
     #   self.value.append(GetFormateTime())
        self.value.append(self.name)
        self.value.append(moduleState[0])
        self.value.append(0)
        self.value.append(0)
        self.value.append(0)
    def noPassOne(self):
        self.value.append(0)
        self.value.append(0)
dtvSubModule = ['section', 'epg', 'search', 'softdemux', 'datamanager', 'sicache','workermanager','si', 'nvod']
dtvUnit = stUnit('dtv', dtvSubModule)
codecSubModule = ['mp3']
codecUnit = stUnit('codec', codecSubModule)
kernelSubModule = ['cfs', 'event', 'oc']
kernelUnit = stUnit('kernel', kernelSubModule)
pvrSubModule = ['pvrfile', 'pvrstream', 'pvrplay', 'pvrrec', 'pvrmgr', 'pvrconfig']
pvrUnit = stUnit('pvr', pvrSubModule)
mediaplayerSubModule = ['selection', 'player']
mediaplayerUnit = stUnit('mediaplayer', mediaplayerSubModule)
protocolSubModule = ['ntp','http','tftp','socket','ipcfg']
protocolUnit = stUnit('protocol', protocolSubModule)
dtvmxSubModule = ['testCase']
dtvmxUnit = stUnit('dtvmx', dtvmxSubModule)

gRecord = [dtvUnit,codecUnit,kernelUnit,pvrUnit,mediaplayerUnit,protocolUnit,dtvmxUnit ]

def readTestLog(fileName):
    if os.path.exists(fileName) == True:
        print fileName
        checkPointLog = ['Module:', 'Test:', 'Success:','Success Rate:']
        fr=open(fileName,'r')
        bModule = False
        logBuf = []
        testResult = []
        moduleCount = 0
        while(True): 
            line = fr.readline()
            if len(line) == 0:
               break
            lineNoEnd = line[0:line.find("\n")]
            lenLineNoEnd = len(lineNoEnd)
            if lenLineNoEnd > 1:
                if bModule == False:
                    checkModuleIndex = lineNoEnd.find(checkPointLog[0])
                    if checkModuleIndex >= 0:
                        bModule = True
                        leftBrickIndex = lineNoEnd.find('[')
                        rightBrickIndex = lineNoEnd.find(']')
                        moduleName = string.lower(lineNoEnd[leftBrickIndex+1:rightBrickIndex])
         #               print 'moduleName: ',moduleName
                        logBuf.append(moduleName)
                else:
                    moduleCount += 1
                    bModule = False
                    checkCaseIndex = lineNoEnd.find(checkPointLog[1])
                    if checkCaseIndex > 0:
                        caseHead = lineNoEnd[checkCaseIndex+len(checkPointLog[1]):lenLineNoEnd]
                        logBuf.append(string.atoi(caseHead[0: caseHead.find(',')]))
                    checkPassIndex = lineNoEnd.find(checkPointLog[2])
                    if checkPassIndex > 0:
                        passHead = lineNoEnd[checkPassIndex+len(checkPointLog[2]):lenLineNoEnd]
                        logBuf.append(string.atoi(passHead[0: passHead.find(',')]))
                    checkRateIndex = lineNoEnd.find(checkPointLog[3])
                    if checkRateIndex > 0:
                        rateHead = lineNoEnd[checkRateIndex+len(checkPointLog[3]):lenLineNoEnd]
                        logBuf.append(string.atoi(rateHead[0: rateHead.find('%')]))
        fr.close()
        #copy into records
     #   if moduleCount > 0:
        for i in range(len(gRecord)):
                bMainModule = False
                bSubModule = False
    #            print '----beforce--',gRecord[i].name,gRecord[i].value
                for j in range(moduleCount):
                    if cmp(logBuf[j*4], gRecord[i].name) == 0:
                        print '------main----', gRecord[i].name
                        gRecord[i].createNew()
                        gRecord[i].value.append(logBuf[j*4+1])
                        gRecord[i].value.append(logBuf[j*4+2])
                        gRecord[i].value.append(logBuf[j*4+3])
                        bMainModule = True
                if bMainModule == False:
                    gRecord[i].noPassAll()
                gRecord[i].count += 1
  #              print '------------', gRecord[i].value
                for k in range(len(gRecord[i].value)):
                	testResult.append(gRecord[i].value[k])
                
        return testResult
    return None
def writeExcelReport(fileName):
    print fileName
    dataResult = []
    font = xlwt.Font()
    font.name = 'fontBlack'
    font.colour_index = 0
    font.bold = True
    #font0.overstrick = True
    #font0.height *= 2
    styleBlack = xlwt.XFStyle()
    styleBlack.font = font
    wb = xlwt.Workbook(encoding='cp936')
    #     ws.insert_bitmap('python.bmp', 8, 8)
    ws = wb.add_sheet('bcm740x', True)
    #write title
    ws.write_merge(0, 0, 0, 4, dateName + GetFormateTime(), styleBlack)
    ws.write_merge(1, 1, 0, 0, module, styleBlack)
    ws.write_merge(1, 1, 1, 1, stateName, styleBlack)
    ws.write_merge(1, 1, 2, 2, moduleTotalName[0], styleBlack)
    ws.write_merge(1, 1, 3, 3, moduleTotalName[1], styleBlack)
    ws.write_merge(1, 1, 4, 4, moduleTotalName[2], styleBlack)
    dataResult.append(module)
    dataResult.append(stateName)
    dataResult.append(moduleTotalName[0])
    dataResult.append(moduleTotalName[1])
    dataResult.append(moduleTotalName[2])
    #write data
    
    for i in range(len(gRecord)):
            for  k in range(gRecord[i].lineLen):
                 ws.write_merge(2+i, 2+i, k, k, gRecord[i].value[k], styleBlack)
                 dataResult.append(gRecord[i].value[k])
    #save excel file
    wb.save(fileName)
 #   print 'dataResult',dataResult
    return dataResult
def createLogFile(attachFile, logFile):
    print 'start send mail'
    #  attachment handle
    reportStuff = ''
    for iFile in attachFile:
        if os.path.exists(iFile) == True:
             filePath = iFile
             blineWrite = False
             bLineCount = 0
             fr = open(iFile,'r')
             while True:
                  line = fr.readline()
                  if len(line) == 0: 
                     break
                  if bLineCount == 2:
                     bLineCount = 0
                     reportStuff += line
                     blineWrite = False
                  if blineWrite == False:
                     if line.find('Sum of Module') != -1:
                        reportStuff += line
                        blineWrite = True
                     if line.find('执行用例数') != -1:
                        reportStuff += line
                  if blineWrite == True:
                     bLineCount += 1 
             fr.close()
    if os.path.exists(logFile) == True:
        print logFile
        os.remove(logFile)
    fw=open(logFile,'w')
    fw.write(reportStuff)
    fw.close()

def createExcel(logFile, xlsFile):
    if os.path.exists(xlsFile) == True:
        print xlsFile
        os.remove(xlsFile)
    readTestLog(logFile)
    dataResult = writeExcelReport(xlsFile)
    print dataResult
    return dataResult

