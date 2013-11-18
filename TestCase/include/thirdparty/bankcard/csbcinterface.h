/******************************************************************************
文件名称：CSBCINTERFACE.H
版权所有(c) 2010， 深圳市同洲电子股份有限公司。

当前版本：Ver 1.0    
作    者：胡远峰 (huyuanfeng@coship.com)

内容摘要：
		文件功能说明

修改记录：
版本号		日期		作者			修改内容
--------	----------	----------		------------
1.0			2010-8-12	胡远峰	创建
*******************************************************************************/

#ifndef CSBCINTERFACE_H
#define CSBCINTERFACE_H

#ifdef _cplusplus
extern "C"{
#endif
/******************************************************************************
 *                                 头文件引用                                 *
 ******************************************************************************/
#include "udiplus_typedef.h"
/******************************************************************************
 *                                 宏、定义、结构体                           *
 ******************************************************************************/
typedef enum _CSBCRetCode
{
	EM_BC_FAILURE = -1,
	EM_BC_SUCCESS,
	EM_BC_CARD_OUT,
	EM_BC_INCORRECT_CARD,
	
	EM_BC_RET_MAX
}CSBCRetCode_EM;
/******************************************************************************
 *                                 函数声明                                   *
 ******************************************************************************/

/******************************************************************************
函数名称:	CSBCInit

功能描述:
		银联卡通信模块初始化

输入参数:
		无

输出参数:
		无

返 回 值:
		EM_BC_SUCCESS:初始化成功
		EM_BC_FAILURE:初始化失败

其他说明:
*******************************************************************************/
INT CSBCInit(void);

/******************************************************************************
函数名称:	CSBCGetSerialNumber

功能描述:
		获取银联卡序列号

输入参数:
		pnSNLen: 获取序列号的数组的最大长度

输出参数:
		pucSerialNumber: 存储序列号的数组
		
		pnSNLen:序列号的实际长度
返 回 值:
		EM_BC_SUCCESS:获取序列号成功
		EM_BC_FAILURE:获取序列号失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCGetSerialNumber(BYTE * pucSerialNumber,INT *pnSNLen);
/******************************************************************************
函数名称:	CSBCGetATR

功能描述:
		获取银联卡ATR信息

输入参数:
		pnATRLen: 获取ATR信息的数组的最大长度

输出参数:
		pucATRData: 存储ATR信息的数组
		
		pnATRLen:ATR信息的实际长度
返 回 值:
		EM_BC_SUCCESS:获取ATR信息成功
		EM_BC_FAILURE:获取ATR信息失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCGetATR(BYTE * pucATRData,INT *pnATRLen);
/******************************************************************************
函数名称:	CSBCEncryptPinCode

功能描述:
		对银联卡Pin码进行加密处理

输入参数:
		pucClearPinCode:加密前的Pin码，必须为16进制ASCII码格式
		
		nPinCodeLen:加密前的Pin码长度
输出参数:
		无

返 回 值:
		EM_BC_SUCCESS:Pin码加密成功
		EM_BC_FAILURE:Pin码加密失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCEncryptPinCode(BYTE * pucClearPinCode,INT nPinCodeLen);

/******************************************************************************
函数名称:	CSBCGetEncryptedPinCode

功能描述:
		获取加密后的PIN码

输入参数:		
		nPinCodeLen:加密后的Pin码长度
输出参数:
		pucEncryptedPinCode:加密后的Pin码,为16进制ASCII码格式

返 回 值:
		EM_BC_SUCCESS:获取加密后的PIN码成功
		EM_BC_FAILURE:获取加密后的PIN码失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCGetEncryptedPinCode(BYTE * pucEncryptedPinCode,INT nPinCodeLen);

/******************************************************************************
函数名称:	CSBCEncryptBusinessData

功能描述:
		对银联卡交易数据进行加密处理

输入参数:
		pucClearData:加密前的交易数据，必须为16进制ASCII码格式
		
		nDataLen:加密前的交易数据长度（单位：BYTE）
输出参数:
		无

返 回 值:
		EM_BC_SUCCESS:交易数据加密成功
		EM_BC_FAILURE:交易数据加密失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCEncryptBusinessData(BYTE * pucClearData,INT nDataLen);

/******************************************************************************
函数名称:	CSBCGetEncryptedBusinessData

功能描述:
		获取加密后的交易数据
		
输入参数:		
		nDataLen:加密后的交易数据长度
输出参数:
		pucEncryptedData:加密后的交易数据,为16进制ASCII码格式

返 回 值:
		EM_BC_SUCCESS:获取加密交易数据成功
		EM_BC_FAILURE:获取加密交易数据失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCGetEncryptedBusinessData(BYTE * pucEncryptedData,INT nDataLen);

/******************************************************************************
函数名称:	CSBCDecryptBusinessData

功能描述:
		对银联卡交易数据进行解密处理

输入参数:
		pucEncryptedData:解密前的交易数据，必须为16进制ASCII码格式
		
		nDataLen:解密前的交易数据长度
输出参数:
		无

返 回 值:
		EM_BC_SUCCESS:交易数据解密成功
		EM_BC_FAILURE:交易数据解密失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCDecryptBusinessData(BYTE * pucEncryptedData,INT nDataLen);
/******************************************************************************
函数名称:	CSBCGetDecryptedBusinessData

功能描述:
		获取解密后的交易数据

输入参数:		
		nDataLen:解密后的交易数据长度
输出参数:
		pucDecryptedData:解密后的交易数据,为16进制ASCII码格式

返 回 值:
		EM_BC_SUCCESS:获取解密交易数据成功
		EM_BC_FAILURE:获取解密交易数据失败
		EM_BC_CARDOUT:未插卡
		EM_BC_INCORRECT_CARD:插入的卡不正确

其他说明:
*******************************************************************************/
INT CSBCGetDecryptedBusinessData(BYTE * pucDecryptedData,INT nDataLen);

#ifdef  __cplusplus
}
#endif

#endif  /* CSBCINTERFACE_H */

/* 文件结束 */

