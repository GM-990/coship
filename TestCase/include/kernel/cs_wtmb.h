/*************************************************************************
* Copyright (c) 2006-2008, ShenZhen Coship Electronic Ltd Co.
* All rights reserved.

* File Name��cs_wtmb.c
*
* Summary�� an efficient tranlsation of code from unicode to gb2312 

* * Current Version��1.0
/--------------------------------------------------------------------------/
revision             author            reason             date
1.0                  zhang shm         Original           Feb,2006
/--------------------------------------------------------------------------/
***************************************************************************/
#ifndef _CS_WTMB_H_
#define _CS_WTMB_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================
//����ԭ��:cs_wtmb()
//�������:
//        lpWCStr ���ַ���(Unicode) 
//        nWCChar ���ַ�����
//        nMBByte ����ַ����Ŀռ��С(BYTE)
//�������:
//        lpMBStr ���ֽ��ַ���(GB2312��ANSI��ϱ���)
//����ֵ: 
//        ת����Ķ��ֽ��ַ�����ʵ�ʳ���
//
//���� :  ��Unicode����������ַ���ת��ΪGB2312������ַ���
//
//ע������:
//       �ú������ɽ�Unicode����ļ�������ת��ΪGB2312����,������
//    ��ͬUnicode�Ӽ��Ļ��Unicode������ַ��޷�ת��;
=========================================================== */
int cs_wtmb(char *lpMBStr, int nMBByte, char *lpWCStr, int nWCChar);

#ifdef __cplusplus
}
#endif

#endif
