#!/usr/bin/python
# -*- coding: cp936 -*-
#yezhenhong 904305
#description:just send mail through coship mail server
import glob
import time
import smtplib
import email
import ftplib
from email.MIMEBase import MIMEBase
from email.MIMEText import MIMEText
from email.MIMEMultipart import MIMEMultipart

def createMailContent(head, stuff, tail):
    contentStuff = ''
    for part in head:
        contentStuff += '\t %s <br>'%(part)
#    for i in range(len(stuff)/5):
    contentStuff += '-----------------------------------------------------------<br>'
    i = 0
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp %s &nbsp&nbsp %s &nbsp&nbsp %s <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 1
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 2
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 3
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 4
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 5
    contentStuff += '\t %s  %s &nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 6
    contentStuff += '\t %s &nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    i = 7
    contentStuff += '\t %s &nbsp&nbsp&nbsp&nbsp %s &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d &nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp %d <br>'%(stuff[i*5], stuff[i*5+1], stuff[i*5+2], stuff[i*5+3], stuff[i*5+4])
    
    contentStuff += '-----------------------------------------------------------<br>'
    for part in tail:
        contentStuff += '\t %s <br>'%(part)     
    
    mailStuff = '''<a> %s <br> </a>'''%(contentStuff)
    return mailStuff
    
def sendMailToNotes(subject, fromUser, toUser, content, attachFiles, serverAddress):
         print 'start send mail'
         #  attachment handle
         msg=MIMEMultipart()
         msg['From'] = '%s'%(fromUser[0])
         mailSubject =  '%s'%(subject[0])
         msg['Subject'] = mailSubject
         msg['Reply-To'] = '%s'%(fromUser[0])
         msg['Date'] = time.ctime(time.time())
         msg['X-Priority'] =  '''3'''
         msg['X-MSMail-Priority'] =  '''Normal'''
         msg['X-Mailer'] =  '''Microsoft Outlook Express 6.00.2900.2180'''
         msg['X-MimeOLE'] =  '''Produced By Microsoft MimeOLE V6.00.2900.2180'''
         for part in attachFiles:
             filePath = part
             blineWrite = False
             bLineCount = 0 
             if part.find('\\') > 0:
                pathSplit = part.split('\\')
                name = pathSplit[len(pathSplit)-1]
                print 'name=',name
                disposition = 'attachment; filename="%s"'%(name)
             else:
                disposition = 'attachment; filename="%s"'%(part)
             print disposition
             att = MIMEText(open(filePath, 'rb').read(), 'base64', 'gb2312')
             att["Content-Type"]='application/octet-stream'
             att["Content-Disposition"] = disposition
             msg.attach(att)
         body=email.MIMEText.MIMEText(content,_subtype='html',_charset='gb2312')
         msg.attach(body)
         s = smtplib.SMTP()
     #    s.set_debuglevel(1)
         mailServer = '%s'%(serverAddress)
         print 'mailServer=',mailServer
         s.connect(mailServer)
         print "toUser=",toUser
         toUserSplit= toUser[0].split(';')
         for dest in toUserSplit:
            s.sendmail(msg['From'],  '%s'%(dest), msg.as_string())
         s.close()
         print 'end send mail' 
         return True

