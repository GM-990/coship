/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_fs.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"


#include "udi2_fs.h"
/**
@brief 添加设备通知回调函数

详见RDI模块CSUDIRDIAddCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] fnCallback 回调函数,当设备状态发生变化时调用该函数
@param[in] pvUserData 用户数据,将作为回调函数的pUserData参数返回
@return 成功添加返回CSUDI_SUCCESS,失败返回错误代码值
@note 添加回调前产生的设备变化信息,不会补充通知.即若开机时这些设备就存在,调用本函数时不会得到通知.
@note fnNotify和pUserData一起唯一确定一个回调函数，当两次注册它们完全一样时，第二次将返回CSUDIFS_ERROR_ALREADY_ADDED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIAddCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 删除设备通知回调函数

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] fnCallback 回调函数,之前注册的函数
@param[in] pvUserData 用户数据,必须与注册时的一致
@return 成功删除返回CSUDI_SUCCESS,失败返回错误代码值
@note fnNotify和pUserData一起唯一确定一个回调函数

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveCallback(CSUDIRDICallback_F fnCallback,const void * pvUserData)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 安全移除可移动存储设备

详见RDI模块CSUDIRDIRemoveCallback_F接口定义，仅用于处理“STORAGE”、“VOLUME”类型的可移动设备
@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 只能删除可移动设备，试图删除其它设备类型则返回CSUDIFS_ERROR_FEATURE_NOT_SUPPORTED，通过设备的设备Id可判断设备类型
@note 删除设备后不可再通过该设备Id获取设备信息
@note 用户调用该接口会触发EM_UDIRDI_PLUGOUT_SAFE类型的EM_UDIRDI_EVENT_PLUGOUT消息

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSRDIRemoveDev(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取所有设备的Id

包括可移动的和固定的存储设备和分卷设备
@param[out] pdwIds 用于存储所有存在的设备的Id
@param[in] nMaxIdCnt pdwIds数组的大小,即最多能返回多少设备ID,一般来说有一个分卷的U盘会表示为2个设备,该值传入32基本能满足需求
@param[out] pnActIdCnt 实际的设备ID数，该值表示有多少设备，与拷贝到pdwIds中的个数相同
@return 成功返回CSUDI_SUCCESS，失败返回错误代码值
@note 该接口仅返回中间件需关注的设备,如U盘,移动硬盘,需在中间件中访问的cramfs设备等,其数量一般来说少于实际的设备数.
故UDI可能需要根据项目进行一些配置工作。
@note 为保持兼容，所有非可移动设备的设备号仍从0开始索引
@note 允许pdwIds为CSUDI_CSUDI_NULL，这种情况下表示应用仅关注有多少个存储设备，但不关心它们具体的Id

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetAllDeviceId(CSUDI_UINT32 * pdwIds,int nMaxIdCnt, int * pnActIdCnt)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 根据设备ID获取设备信息

@param[in] dwDeviceId 目标设备ID,其值来源于CSUDIFSGetAllDeviceId或回调函数
@param[out] psDeviceInfo 指向返回设备信息内存的指针
@return 成功获取返回CSUDI_SUCCESS，失败则返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetDeviceInfo(CSUDI_UINT32 dwDeviceId,CSUDIFSDeviceInfo_S * psDeviceInfo)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 使设备进入待机状态

@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 仅支持存储设备即可,分卷设备返回失败
@note 已经在待机状态下调用本接口仍返回CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSStandby(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}
/**
@brief 唤醒设备

@param[in] dwDeviceId 目标设备ID
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 仅支持存储设备即可,分卷设备返回失败
@note 已经在唤醒状态下调用本接口仍返回CSUDI_SUCCESS

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSWakeup(CSUDI_UINT32 dwDeviceId)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 格式化设备

@param[in] dwDeviceId 目标设备ID
@param[in] eType 拟格式化成的文件系统格式
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note 传入分卷设备将格式化整个分卷
@note 传入存储设备将整个设备格式化为一个分卷,并产生原分卷(如果存在)PLUGOUT,新分卷创建的回调

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSFormat(CSUDI_UINT32 dwDeviceId,CSUDIFSPartitionType_E eType)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取路径编码方式

@param[out] pnCodePage 编码字节顺序 | 编码方式,详见以上宏定义
通过该函数确定与UDIFS交互的路径的编码方式
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
		But it is just a stub function.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIFSGetCodePage(int * pnCodePage)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 打开文件

@param[in] pcFileName 文件名
@param[in] pcMode 打开方式,详见C99标准
@return 文件句柄,失败返回CSUDI_NULL
@note 本函数定义类似C99标准fopen

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpen(const char * pcFileName, const char * pcMode)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 读文件

@param[in] hFile 文件句柄,CSUDIFS_Open的返回值
@param[in] pcBuf 指向存放读入数据的内存
@param[in] uCount 需读入的字节数
@return 实际读到的字节数,它可能少于uCount
@note uCount为0时返回0,文件状态不发生任何变化
@note 返回值<0表示出错或者文件到结尾了

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRead(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 写文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] pcBuf 指向存放写入数据的内存
@param[in] uCount 需读入的字节数
@return 实际写入的字节数,它可能少于uCount
@note uCount为0时返回0,文件状态不发生任何变化

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSWrite(CSUDI_HANDLE hFile,char * pcBuf, unsigned int uCount)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 关闭文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSClose(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief seek文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] lOffset 移动偏移
@param[in] uOrigin 移动起始位置,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSSeek(CSUDI_HANDLE hFile, long lOffset, unsigned int uOrigin)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief seek文件64位版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] i64Offset 移动偏移
@param[in] uOrigin 移动起始位置,CSUDIFS_SEEK_SET,CSUDIFS_SEEK_CUR,CSUDIFS_SEEK_END
@return 成功返回0,其它情况返回-1
@note 即使文件系统不支持超过2G的文件也必须实现

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLseek(CSUDI_HANDLE hFile,CSUDI_INT64 i64Offset,unsigned int uOrigin)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取文件当前位置

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回文件当前距开始处的偏移,其它情况返回-1
@note 当前偏移大于long能表示的最大范围时(32位机上为2G),返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
long CSUDIFSTell(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取文件当前位置64位版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回文件当前距开始处的偏移,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_INT64 CSUDIFSLtell(CSUDI_HANDLE hFile)
{
	CSUDI_INT64 value = 0;
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return value;
}

/**
@brief flush文件

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFlush(CSUDI_HANDLE hFile)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 删除文件

@param[in] pcFileName 文件名
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRemove(const char * pcFileName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 重命名文件

@param[in] pcOldPath 旧文件名
@param[in] pcNewPath 新文件名
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹有效
@note 该接口可以改变文件的目录,但不会创建新目录,即若pNewPath指定的目录不存在则返回失败
@note 如果pNewPath指向的文件已存在,结果是不确定的,情慎用该特性.

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRename(const char * pcOldPath, const char * pcNewPath)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 截断文件

将一个文件截断为一定长度
@param[in] pcPathName 文件名
@param[in] ulLength 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSTruncate(const char * pcPathName, unsigned long ulLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 截断文件描述符版

将一个文件截断为一定长度
@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] ulLength 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFTruncate(CSUDI_HANDLE hFile, unsigned long ulLength)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 截断文件64位版

将一个文件截断为一定长度
@param[in] pcPathName 文件名
@param[in] u64Length 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLTruncate(const char * pcPathName,CSUDI_UINT64 u64Length)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 截断文件描述符64位版

将一个文件截断为一定长度
@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[in] u64Length 新文件长度
@return 成功返回0,其它情况返回-1
@note 如果新长度小于原文件长度,则多余的部分丢弃;如果新长度大于原文件长度,则新增部分用0填充
@note 该接口对文件夹无效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSLFTruncate(CSUDI_HANDLE hFile,CSUDI_UINT64 u64Length)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取文件属性

@param[in] pcFileName 文件名
@param[out] psFileStat 输出文件属性
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹有效
@note psFileStat->m_lMode 值必须正确。只可能是CSUDIFS_ISREG、CSUDIFS_ISDIR，不应该为其他值

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStat(const char * pcFileName, CSUDIFSFileStat_S * psFileStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取文件属性文件描述符版

@param[in] hFile 文件句柄,CSUDIFSOpen的返回值
@param[out] psFileStat 输出文件属性
@return 成功返回0,其它情况返回-1
@note 该接口对文件夹无效
@note psFileStat->m_lMode 值必须正确,即为CSUDIFS_ISREG。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstat(CSUDI_HANDLE hFile, CSUDIFSFileStat_S * psFileStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 创建文件夹

@param[in] pcPathName 文件夹名
@param[in] nReserved 保留字段,必须传0,否则须返回失败
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSMkdir(const char * pcPathName, int nReserved)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 删除文件夹

@param[in] pcPathName 文件夹名
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSRmdir(const char * pcPathName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 打开文件夹

@param[in] pcPathName 文件夹名
@return 成功返回文件夹句柄,失败返回CSUDI_NULL

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_HANDLE CSUDIFSOpenDir(const char * pcPathName)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 关闭文件夹

@param[in] hDir 文件夹句柄,CSUDIFSOpendir的返回值
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSCloseDir(CSUDI_HANDLE hDir)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 读取文件夹

一次读取该文件夹下的一个节点的信息,依次调用可获取该文件夹下所有文件信息
@param[in] hDir 文件夹句柄,CSUDIFSOpendir的返回值
@return 返回一个文件夹内容节点指针,没有更多节点时返回CSUDI_NULL
@note
- 该接口仅列出其子文件和文件夹，不递归
- 子文件夹包括"."和".."两个目录

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDIFSDirent_S *CSUDIFSReadDir(CSUDI_HANDLE hDir)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 获取分卷信息

获取pcPathName所在分卷的分卷信息,如磁盘大小,空闲空间等
@param[in] pcPathName 分卷路径,所需获得信息的分卷的路径
@param[out] psFsStat 存储分卷信息内存指针
@return 成功返回0,其它情况返回-1
@note 该接口对文件、文件夹均有效

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSStatfs(const char * pcPathName, CSUDIFSStatFs_S * psFsStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

/**
@brief 通过文件句柄获取分卷信息

获取hFile对应的文件所在分卷的分卷信息,如磁盘大小,空闲空间等
@param[in] hFile 文件句柄,即去获取该文件所在的分卷的信息
@param[out] psFsStat 存储分卷信息内存指针
@return 成功返回0,其它情况返回-1

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
int CSUDIFSFstatfs(CSUDI_HANDLE hFile, CSUDIFSStatFs_S * psFsStat)
{
	UDIDRV_LOGI("%s %s begin\n", __FUNCTION__, UDIDRV_IMPLEMENTED);

	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s (Retcode =%d)end\n", __FUNCTION__, Retcode);    
	return Retcode;
}

