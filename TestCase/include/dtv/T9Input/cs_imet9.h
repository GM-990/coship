/*************************************************************************
* CoSpellright (c) 2004,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name：CS_InputMethod.h  （T9 release）  
* File Identify：reference to Configuration Manager
* Summary： A chinese character typing methord 
* 
* Current Version：1.0
* Author(s)：
* 
/---------------------------------------------------------------------------------------------------------/
revision             author            reason             date
1.0                 jl.Huang          Original       September 8,2004
/--------------------------------------------------------------------------------------------------------/
************************************************************************** */

#ifndef _CS_IMET9_H_
#define _CS_IMET9_H_

#ifdef __cplusplus
extern "C" {
#endif

/*----------------- MICRO for controlling complier (start)----------- *
 * if you want to compile this file on PC platform, please define the *
 * CSSTRSAFE_PLATFORM 0, otherwise(under LSI,ST) define				  *
 * CSSTRSAFE_PLATFORM 1 or 2.											  *
 *----------------- MICRO for controlling complier (start)------------*/
#define CSIMET9_PLATFORM 1    //select PC Platform

//
#if (CSIMET9_PLATFORM == 0)
#define CSIME_EXPORT __declspec(dllexport)
typedef unsigned int  size_t;
//
#elif (CSIMET9_PLATFORM == 1)
#define CSIME_EXPORT
//
#elif(CSIMET9_PLATFORM == 2)
#define CSIME_EXPORT
#endif

#ifdef __cplusplus
#define _CSIME_EXTERN_C    extern "C"
#else
#define _CSIME_EXTERN_C    extern
#endif

#define CSIMET9API  _CSIME_EXTERN_C CSIME_EXPORT

/*==========================================================
//   Spell 是拼音的简称   MB 为模板的简称    Character为汉字的简称   //
//   Symbol  符号的简称                                        // 
===========================================================*/          
//拼音模板一般有GB编码和其他扩充编码两种类型(分别为1和2) 
//选择模板并获得汉字模板的地址   
//成功则返回1，失败返回0
//备注：扩充映射模板需完善扩展的模板
//      nFlag可选其他值作为扩展标志
CSIMET9API int CreateIMET9(int nFlag);
CSIMET9API int DeleteIMET9();


//函数功能：输入数字并返回对应的拼音的候选项首地址和拼音个数
//输入：整型数字    返回个数的指针  
//输出：成功（有候选项）返回其句柄  失败（没有）返回0   
//      返回拼音候选项的个数和首地址
//备注：返回的句柄是候选项中第一个拼音的首地址
CSIMET9API int GetSpellCount(int nInputDigit,int* pnSpellCount);


//函数功能：根据获得的拼音候选项得到对应的拼音字符
//输入：候选项的句柄    拼音地址索引(注意根据候选项个数限定)   
//      保存选出拼音的指针       buffer大小
//输出：成功返回1   否则返回0      参数返回选出拼音
CSIMET9API int GetSpell(int nSpellHandle,int nIndex,char* pcSpellBuffer,int nSize);


//函数功能：根据获得的拼音字符得到对应的汉字候选项
//输入：选出的拼音的句柄     指向汉字候选项的个数的指针
//输出：成功返回值   汉字候选项的句柄    返回候选项个数
CSIMET9API int GetCharacterCount(char* pcSpellBuffer,int* pnCharacterCount);


//函数功能：根据获得的汉字的候选项得到要找的汉字
//输入：汉字候选项的句柄   汉字索引(注意根据汉字返回的候选项个数进行限定)
//      选出的汉字的指针             buffer大小	
//输出：成功则返回1   否则返回0
CSIMET9API int GetCharacter(int nCharacterHandle,int nIndex,char* pcCharacterBuffer,int nSize);


//函数功能：输入数字并返回对应的符号的候选项首地址
//输入：整型数字(0,1)    指向符号的个数的指针    标志位（1为符号，0为字母）
//输出：成功（有候选项）返回值  失败（没有）返回0   
//      返回符号候选项的个数和首地址
//备注：返回的首地址是候选项中第一个符号的首地址
//      函数会根据你选取的模板类型选择符号的编码类型
CSIMET9API int GetSymbolCount(int nInputDigit,int* pnSymbolCount,int nFlag);



//函数功能：根据获得的符号候选项得到对应的符号
//输入：符号候选项的首地址    符号地址索引(注意根据候选项个数限定)   
//         保存选出符号的BUFFER    buffer大小
//输出：成功返回1   否则返回0  同时返回选出拼音的首地址
CSIMET9API int GetSymbol(int nSymbolHandle,int nIndex,char* pcSymbolBuffer,int nSize);

#ifdef __cplusplus
}

#endif

#endif
