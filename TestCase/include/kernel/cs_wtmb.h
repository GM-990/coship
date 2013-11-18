/*************************************************************************
* Copyright (c) 2006-2008, ShenZhen Coship Electronic Ltd Co.
* All rights reserved.

* File Name：cs_wtmb.c
*
* Summary： an efficient tranlsation of code from unicode to gb2312 

* * Current Version：1.0
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
//函数原型:cs_wtmb()
//输入参数:
//        lpWCStr 宽字符串(Unicode) 
//        nWCChar 宽字符个数
//        nMBByte 输出字符串的空间大小(BYTE)
//输出参数:
//        lpMBStr 多字节字符串(GB2312和ANSI混合编码)
//返回值: 
//        转换后的多字节字符串的实际长度
//
//功能 :  将Unicode编码的中文字符串转化为GB2312编码的字符串
//
//注意事项:
//       该函数仅可将Unicode编码的简体中文转换为GB2312编码,对其它
//    不同Unicode子集的或非Unicode编码的字符无法转换;
=========================================================== */
int cs_wtmb(char *lpMBStr, int nMBByte, char *lpWCStr, int nWCChar);

#ifdef __cplusplus
}
#endif

#endif
