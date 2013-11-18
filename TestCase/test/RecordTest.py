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

dateName = '日期'
stateName = '状态'
moduleState = ['未测','已测','未完测']
moduleTotalName = ['测试总数','通过总数']
subModuleMarkName = ['用例个数','通过个数']

def GetFormateTime():
    strLocalTime = time.localtime(time.time())
    strTime =str(strLocalTime.tm_year)+'/'+str(strLocalTime.tm_mon)+'/'+str(strLocalTime.tm_mday)+' '+str(strLocalTime.tm_hour)+':'+str(strLocalTime.tm_min)
 #   print '--time---',strTime
    return strTime
class stUnit:
    def __init__(self, name, subModule):
        self.name = name
        self.numModules = len(subModule)
        self.lineLen = len(subModule)*2 + 4
        self.subModule = subModule
        self.count = 0
        self.value=[]
    def createNew(self):
        self.value.append(GetFormateTime())
        self.value.append(moduleState[1])
    def createNewBlank(self):
        self.value.append(GetFormateTime())
        self.value.append(moduleState[2])
        self.value.append(0)
        self.value.append(0)
    def noPassAll(self):
        self.value.append(GetFormateTime())
        self.value.append(moduleState[0])
        self.value.append(0)
        self.value.append(0)
        for i in range(self.numModules):
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

def readExcelRecord(fileName):
    if os.path.exists(fileName) == True:
        xl = xlrd.open_workbook(fileName)
        for i in range(len(gRecord)):
            print gRecord[i].name,gRecord[i].numModules
            sheet = xl.sheet_by_name(gRecord[i].name)
            if sheet != None:
                for line in xrange(2,sheet.nrows):
                    for j in range(gRecord[i].lineLen):                  
               #         print '---line----',line, sheet.cell_value(line, j)
                        gRecord[i].value.append(sheet.cell_value(line, j))
                gRecord[i].count = sheet.nrows - 2
          #  print '----total----',gRecord[i].count

def readTestLog(fileName):
    if os.path.exists(fileName) == True:
        print fileName
        checkPointLog = ['Module:', 'Test:', 'Success:']
        fr=open(fileName,'r')
        bModule = False
        logBuf = []
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
                        logBuf.append(caseHead[0: caseHead.find(',')])
                    checkPassIndex = lineNoEnd.find(checkPointLog[2])
                    if checkPassIndex > 0:
                        passHead = lineNoEnd[checkPassIndex+len(checkPointLog[2]):lenLineNoEnd]
                        logBuf.append(passHead[0: passHead.find(',')])
        fr.close()
#        print '------log buf-----',logBuf
        
        #copy into records
        if moduleCount > 0:
            for i in range(len(gRecord)):
                bMainModule = False
                bSubModule = False
          #      print '----beforce--',gRecord[i].name,gRecord[i].value
                for j in range(moduleCount):
                    if cmp(logBuf[j*3], gRecord[i].name) == 0:
          #              print '------main----', gRecord[i].name
                        gRecord[i].createNew()
                        gRecord[i].value.append(string.atoi(logBuf[j*3+1]))
                        gRecord[i].value.append(string.atoi(logBuf[j*3+2]))
                        bMainModule = True
                
                if bMainModule == False:
                    bHalfModule = False
                    for j in range(gRecord[i].numModules):                  
                        for k in range(moduleCount):
                            if cmp(logBuf[k*3], gRecord[i].subModule[j]) == 0:
                                bHalfModule = True
                    if bHalfModule == False:
             #           print '---None--',gRecord[i].name
                        gRecord[i].noPassAll()
                    else:
              #          print '---half--',gRecord[i].name
                        gRecord[i].createNewBlank()
                        for j in range(gRecord[i].numModules):
                            bSubModule = False
                            for k in range(moduleCount):  
                                if cmp(logBuf[k*3], gRecord[i].subModule[j]) == 0:
                                    bSubModule = True
                                    gRecord[i].value.append( string.atoi(logBuf[k*3+1]))
                                    gRecord[i].value.append(string.atoi(logBuf[k*3+2]))
                            if bSubModule == False:
                                gRecord[i].noPassOne()
                else:
                    for j in range(gRecord[i].numModules):
                        bSubModule = False
                        for k in range(moduleCount):
                            if cmp(logBuf[k*3], gRecord[i].subModule[j]) == 0:
                                bSubModule = True
                                gRecord[i].value.append( string.atoi(logBuf[k*3+1]))
                                gRecord[i].value.append( string.atoi(logBuf[k*3+2]))
                        if bSubModule == False:
                            gRecord[i].noPassOne()
        #        print '----------record----------'               
         #       print gRecord[i].name,gRecord[i].value
                gRecord[i].count += 1
def writeExcelRecord(fileName):
    print fileName
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
    for i in range(len(gRecord)):
        ws = wb.add_sheet(gRecord[i].name, True)
        #write title
        ws.write_merge(0, 1, 0, 0, dateName, styleBlack)
        ws.write_merge(0, 1, 1, 1, stateName, styleBlack)
        ws.write_merge(0, 1, 2, 2, moduleTotalName[0], styleBlack)
        ws.write_merge(0, 1, 3, 3, moduleTotalName[1], styleBlack)
        for  j in range(gRecord[i].numModules):
            ws.write_merge(0, 0, 4+j*2, 4+j*2+1, gRecord[i].subModule[j], styleBlack)
            ws.write_merge(1, 1, 4+j*2, 4+j*2, subModuleMarkName[0], styleBlack)
            ws.write_merge(1, 1, 4+j*2+1, 4+j*2+1, subModuleMarkName[1], styleBlack)
        #write data
 #       print '----record',gRecord[i].name, gRecord[i].count, gRecord[i].value
        for  j in xrange(0,gRecord[i].count):
            for  k in range(gRecord[i].lineLen):
                ws.write_merge(2+j, 2+j, k, k, gRecord[i].value[j*gRecord[i].lineLen +k], styleBlack)
    #save excel file
    wb.save(fileName)     
# main loop

def recordResult(logFile):
	readExcelRecord('testRecord.xls')
	readTestLog(logFile)
	writeExcelRecord('testRecord.xls')

