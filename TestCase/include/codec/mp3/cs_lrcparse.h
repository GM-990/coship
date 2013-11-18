
/******************************************************************************
FileName:  cs_lrcparse.h
Copyright (c) 2009， Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:        
description: LRC歌词文件解析
	1、支持线程安全操作
	2、支持非法句柄判断
modify:
Version     Date            Author          Description
--------    ----------    --------          ------------

*******************************************************************************/

#ifndef _CSLRC_PARSE_H_
#define _CSLRC_PARSE_H_
#ifdef __cplusplus
extern "C"{
#endif



/*!
	描述返回值
*/
typedef enum _CSLrc_RetCode_E
{
	EM_CSLRC_SUCCESS,
	EM_CSLRC_FAILED,              //  一般失败
	EM_CSLRC_PARM_ERR,       //输入参数有误
	EM_CSLRC_FILE_ERR,         //文件路径有误
	EM_CSLRC_NO_MEM,          //内存不足
	EM_CSLRC_PARSE_FAILED,   //解析lrc歌词失败
	EM_CSLRC_INVALID_HANDLE   // 非法的句柄
}CSLrc_RetCode_E;

/*!
	行信息	
*/
typedef struct  _CSLrcLineInfo_S
{
	int      m_nLineNumber;      //行号
 	char  	*m_pcBuf;                 //行的内容，以'\0'结束
 	int      m_nBufSize;            //pcBuf分配的大小
	int      m_nStartTime;        // 开始时间，以ms为单位
	int      m_nEndTime;          //结束时间，以ms为单位，注意最后一句歌词没有结束时间，该成员无效
}CSLrcLineInfo_S;

/*!
      创建歌词解析器
@param[in]   pcFileName 输入LRC歌词文件
@param[out]  phHandle  解析成功用于存放用于后续操作的句柄
\return     成功返回EM_CSLRC_SUCCESS，其它为失败
\note  pnHandle可用于后续操作
*/
CSLrc_RetCode_E  CSLrcCreateFromFile(char *pcFileName, int *pnHandle);

/*!
      创建歌词解析器
@param[in]   pcBuf     输入存有LRC的Buf
@param[in]   nBufSize  pcBuf的大小
@param[out]  phHandle  解析成功用于存放用于后续操作的句柄
\return     成功返回EM_CSLRC_SUCCESS，其它为失败
\note  pnHandle可用于后续操作
*/
CSLrc_RetCode_E  CSLrcCreateFromBuf(char *pcBuf, int  nBufSize, int *pnHandle);

/*!
      获得歌词的总行数
@param[in]   nHandle    有效的句柄，由CSLrcCreateFromFile()得到
@param[out]  pnLineCount   存放行数
\return  成功返回EM_CSLRC_SUCCESS，其它为失败
\note  可以根据行号调用CSLrcGetLineInfoByIndex()获得一行的完整信息
*/
CSLrc_RetCode_E  CSLrcGetLineCount(int nHandle, int *pnLineCount);

/*!
	获取一行的完整信息
@param[in]   nHandle  有效的句柄
@param[in]   nIndex   要获取的行，从0开始，如果总行数是10，则有效的行号是0到9
@param[out]  psLineInfo   行的完整信息，记录了行号、行的内容、起始时间和结束时间
\return   成功返回EM_CSLRC_SUCCESS，其它为失败
\note     结构体psLineInfo的成员pcBuf需要由外部分配内存，nBufSize表示分配了多大内存,大小以大于256字节为宜
          如内存分配不够，则只返回部分  
\note     循环调用本函数可以获取到整首歌词的内容
\note     最后一行没有结束时间戮，用户可自由处理，比如设置个默认时间戮
*/
CSLrc_RetCode_E CSLrcGetLineInfoByIndex(int nHandle, int nIndex, CSLrcLineInfo_S *psLineInfo);

/*!
      获得某时间点对应的完整行信息
@param[in]    nHandle      句柄
@param[in]    nMillSec     输入时间，以ms为单位
@param[out]   psLineInfo   行的完整信息，记录了行号、行的内容、起始时间、结束时间、当前百分比
\return   成功返回EM_CSLRC_SUCCESS，其它为失败
\note  如果时间太小（如为负）找不到对应的歌词，则返回第一句歌词
       如果时间太大找不到对应的歌词，则返回最后一句歌词
	  太小或太大的百分数都是零
\note  结构体psLineInfo的成员pcBuf需要由外部分配内存，nBufSize表示分配了多大内存,大小以大于256字节为宜
       如内存分配不够，则只返回部分  
\note  本函数通常用在动态显示当前行,psLineInfo的成员nLineNumber可用于显示整首歌曲行进度
\note  最后一行没有结束时间戮，用户可自由处理，比如设置个默认时间戮
*/

CSLrc_RetCode_E CSLrcGetLineInfoByTime(int nHandle, int nMillSec, CSLrcLineInfo_S *psLineInfo);

/*!
      释放句柄
@param[in]  nHandle  要销毁的句柄
\return     成功返回EM_CSLRC_SUCCESS，其它为失败
*/
CSLrc_RetCode_E CSLrcDestory(int nHandle);

#ifdef __cplusplus
}
#endif
#endif
