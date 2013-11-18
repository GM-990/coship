
//cs_condanalyzer.h
#ifndef _CS_COND_ANALYZER_H
#define _CS_COND_ANALYZER_H

#ifdef  __cplusplus
extern "C" {
#endif



#include "mmcp_typedef.h"


//运算结果
typedef enum _CSCondResult_e
{
	CS_COND_RESULT_INVALID = 0,	//非法值
	CS_COND_RESULT_TRUE,			//TRUE
	CS_COND_RESULT_FALSE			//FALSE
}CSCondResult_E;



/**
@brief 扫描Cond词法，获取对应的词法分析句柄

@param[in] pcCond  Cond词法字符串
@param[in] nLen  Cond字符串长度
@return 成功返回Cond词法对应的词法分析句柄，失败返回NULL
*/
HCSHANDLE CSCondScan(const char *pcCond, int nLen);


/**
@brief 根据扫描到的词法分析句柄和附加数据计算分析结果

@param[in] hCond  词法分析句柄
@param[in] pvData 基本运算单元附加数据
@return 运算结果
*/
CSCondResult_E CSCondAnalyze(HCSHANDLE hCond, void *pvData);


/**
@brief 释放词法分析句柄

@param[in] hCond  词法分析句柄
@return 成功返回TRUE，失败返回FALSE
*/
BOOL CSCondRelease(HCSHANDLE hCond);


/**
@brief 基本运算单元运算回调函数

@param[in] pcBase  基本运算单元字符串
@param[in] nLen  基本运算单元字符串长度
@param[in] pvData  基本运算单元附加数据
@return 运算结果
*/
typedef CSCondResult_E (*CSCondBaseOperator_Fn)(const char *pcBase, int nLen, void *pvData);


/**
@brief 注册基本运算单元运算回调函数

@param[in] hCond  词法分析句柄
@param[in] fnBaseOperator  回调函数
@return 成功返回TRUE，失败则返回FALSE
*/
BOOL CSCondRegistBaseOperator(HCSHANDLE hCond, CSCondBaseOperator_Fn fnBaseOperator);

#ifdef  __cplusplus
}
#endif

#endif

