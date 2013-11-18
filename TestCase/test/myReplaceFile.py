#!/usr/bin/python
#yezhenhong 904305
#----------------------edit record---------------------------------
#    20100402 added send inner outer mail server
#    20100406 inner mail server parameters changed,attchments need to change smaller;
#    20100406 change run_bcm740x_test_p for some modules stop on the way
# ----------------------------------------------------------
import os
import time
import string 
import shutil
import re

def replace_file_stuff_define(fileName,name,value):
        if fileName == "":
           return -1
        if name == "":
           return -1
        src = fileName+ 'tmp'
        shutil.copy(fileName,src)
        fw = open(fileName,'wb')
        fr = open(src,'r')
        name += '='
        while True:
              line = fr.readline()
              if len(line) == 0: 
                 break
              if line.find(name) != -1:   
                 bufLine = name + value +'\t\n'
                 fw.write(bufLine)
#                 print 'line=',line,"result:",bufLine  
              else:
                 fw.write(line)
        fr.close()
        os.remove(src)
        fw.close()
        return 0

def mask_file_stuff_define(fileName,name,mask,lines):
        strName = '%s'%(name)
        strMask = '%s'%(mask)
  #      print '---mask---',fileName, strName, mask,lines
        if fileName == "":
           return -1
        if name == "":
           return -1
        src = fileName+ 'tmp'
        shutil.copy(fileName,src)
        fw = open(fileName,'wb')
        fr = open(src,'r')
        name += '='
        count = 0
        maskFlag = False
      #  print '---2---',fileName
        while True:
              line = fr.readline()
              if len(line) == 0: 
                 break
              indexMask = -1
              indexMask = string.upper(line).find(string.upper(strMask))
              if string.upper(line).find(string.upper(strName)) >= 0:
 #                print '----mask line=',line
                 count += 1
                 if count < lines:
                    maskFlag = True
                 if indexMask >= 0:
                     bufLine = line
                 else:
                     bufLine = strMask +line
#                 print "result:",bufLine
                 fw.write(bufLine)  
              else:
                if maskFlag == True:
                    count += 1
                    if indexMask >= 0:
                        bufLine = line
                    else:
                        bufLine = strMask +line
                    fw.write(bufLine)
                    if count >= lines:
                       count = 0
                       maskFlag = False
                else:
                    fw.write(line)
        fr.close()
        os.remove(src)
        fw.close()
        return 0
def unmask_file_stuff(fileName,name,mask,lines):
        strName = '%s'%(name)
        strMask = '%s'%(mask)
   #     print '---unmask---',fileName, strName, strMask,lines
        if fileName == "":
           return -1
        if name == "":
           return -1
        src = fileName+ 'tmp'
        shutil.copy(fileName,src)
        fw = open(fileName,'wb')
        fr = open(src,'r')
        name += '='
        count = 0
        maskFlag = False
        lenMask = len(strMask)
#        print '---2---',fileName
        while True:
              line = fr.readline()
              if len(line) == 0: 
                 break
              index = string.upper(line).find(string.upper(strName))
              indexMask = string.upper(line).find(string.upper(strMask))
              if indexMask >= 0:
                 indexMask += lenMask
                 for i in xrange(20):
                     if string.upper(line[(indexMask):len(line)]).find(string.upper(strMask)) >= 0:
                        indexMask += lenMask
              if index >= 0:
    #             print '---unmask-line=',indexMask, line
                 count += 1
                 if count < lines:
                    maskFlag = True
                 if indexMask >= 0:
                    bufLine = line[indexMask:len(line)]
                 else:
                    bufLine = line
  #               print "result:",bufLine
                 fw.write(bufLine)  
              else:
                if maskFlag == True:
                    count += 1
                    if indexMask >= 0:
                       bufLine = line[indexMask:len(line)]
                    else:
                       bufLine = line 
                    fw.write(bufLine)
                    if count >= lines:
                       count = 0
                       maskFlag = False
                else:
                    fw.write(line)
        fr.close()
        os.remove(src)
        fw.close()
        return 0
def replace_file_stuff(fileName,name,value):
        if fileName == "":
           return -1
        if name == "":
           return -1
        src = fileName+ 'tmp'
        shutil.copy(fileName,src)
        fw = open(fileName,'wb')
        fr = open(src,'r')
        while True:
              line = fr.readline()
              if len(line) == 0: 
                 break
              if line.find(name) != -1:   
                 bufLine = sub = re.sub(name,value,line,0);
                 fw.write(bufLine)
 #                print 'line=',line,"result:",bufLine  
              else:
                 fw.write(line)
        fr.close()
        os.remove(src)
        fw.close()
        return 0

#test = MyReplaceFile()
#test.replace_file_stuff_define('test.sh', 'hmcpath','hello')
#print "write end"   

