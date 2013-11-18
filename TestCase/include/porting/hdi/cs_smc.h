/**@defgroup CSSMC [CSSMC]Smartcard drvier
@brief 提供智能卡统一接口

@version 0.4 2009/02/26 增加了获取智能卡时钟频率接口
@version 0.3 2009/01/06 增加了一些功能接口
@version 0.2 2008/11/5  第一次细化版本
@version 0.1 2008/10/23 初始版本
@{
*/

#ifndef __CS_SMC_H
#define __CS_SMC_H

#include "cs_typedef.h"
#include "CS_OS.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief 智能卡设备总数*/
#define CS_SMC_DEVICE_COUNT 	(2)       /* 1 or 2  Total Smart Card Device Number */


/**@brief 智能卡协议*/
typedef enum _SmcProtocol
{
	CS_SMC_PROTOCOL_UNKNOWN,		/**< 未知协议*/
	CS_SMC_PROTOCOL_T0,				/**< 待补充*/
	CS_SMC_PROTOCOL_T1,				/**< 待补充*/
	CS_SMC_PROTOCOL_T14,			/**< 待补充*/

	CS_SMC_PROTOCOL_MAX				/**<  上限 */
}E_SmcPrococol;

/**@brief 智能卡标准*/
typedef enum _SmcStandard
{
	CS_SMC_STANDARD_NDS,			/**< NDS标准*/
	CS_SMC_STANDARD_ISO,			/**< ISO标准*/
	CS_SMC_STANDARD_EMV2000,		/**< EMV2000标准*/
	CS_SMC_STANDARD_IRDETO,			/**< 爱迪德标准*/

	CS_SMC_STANDARD_MAX				/**<  上限 */
}E_SmcStandard;

/**@brief CA类型*/
typedef enum _CaType
{
	SMC_UNKNOWN_CA = 0,
	SMC_CONAX_CA,  
	SMC_VIACCESS_CA , 
	SMC_TONGFANG_CA ,
	SMC_DVN_CA , 
	SMC_CRYPTOWORKS_CA , 
	SMC_MEDIAGUARD_CA , 
	SMC_SUMAVISION_CA ,
	SMC_ST_CA,
	SMC_NAGRAVISON_CA , 
	SMC_IRDETO_CA, 
	SMC_COMVENIENT_CA,
	SMC_ABV_CA

}E_SMC_CaType;

/**@brief 错误返回值*/
typedef enum _DrvReturnType
{
	CS_SMC_FAILURE = -1,
	CS_SMC_SUCCESS,
	CS_SMC_TIMEOUT,
	CS_SMC_ERROR_PARITY,
	CS_SMC_NO_ANSWER,
	
	CS_SMC_OUT,
	CS_SMC_IN,	
	CS_SMC_BUSY,
	CS_SMC_NOT_LOCK_OWNER,
	CS_SMC_READY,

	CS_SMC_MUTE,
	CS_SMC_ABORT,
	CS_SMC_INVALID,
	CS_SMC_INVALID_PROTOCOL,
	CS_SMC_NOT_RESET,
	CS_SMC_NOT_SUPPORT,

	CS_SMC_MAX	
}E_DrvReturnType;

/**
@brief 智能卡回调函数

由CA厂商提供，在智能卡插入或者拔出的时候由Coship智能卡API调用
@param[in] dCardIndex 智能卡卡号
@param[in] bStatus 智能卡状态
*/
typedef void (*T_CSSMCTaskCallback)(DWORD dwCardNumber, INT nStatus);


/**
@brief 智能卡初始化

*在系统启动的时候调用，并且只调用一次
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;
*/
INT CSSMCInit(void);

/**
@brief 打开智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[in] CallBack 回调函数
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;如果卡号无效返回CS_SMC_INVALID_INDEX；
*/
INT CSSMCOpen(DWORD dCardIndex, T_CSSMCTaskCallback CallBack);

/**
@brief 关闭智能卡

@param[in] dCardIndex 智能卡卡槽号
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;如果卡号无效返回CS_SMC_INVALID_INDEX；
*/
INT CSSMCClose(DWORD dCardIndex);

/**
@brief 设备和智能卡之间的数据传输

@param[in] dCardIndex 智能卡卡槽号
@param[in] WriteData 写入智能卡的数据
@param[in] NumberToWrite 写入数据长度
@param[out] ResponseData 读取数据的存储位置
@param[out] NumberRead 读取数据的长度
@param[out] StatusWord 驱动提供的状态字
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;如果没有卡返回CS_SMC_NO_CARD；如果智能卡繁忙则返回CS_SMC_CARD_BUSY；如果超时返回CS_SMC_TIMEOUT;如果卡号无效返回CS_SMC_INVALID_INDEX；如果传输过程中出现错误返回CS_SMC_ERROR_PARITY
*/
INT CSSMCDataExchange
(
	DWORD  dCardIndex,
	BYTE  *WriteData,
	DWORD  NumberToWrite,
	BYTE  *ResponseData,
	DWORD *NumberRead,
	BYTE  *StatusWord
);

/**
@brief 设备和智能卡之间的数据传输

@param[in] dCardIndex 智能卡卡槽号
@param[in] WriteData 写入智能卡的数据
@param[out] ResponseData 从智能卡中读取的数据
@note:
- case  ((WriteData[0] & 0x10) != 0)    WriteData[1] valid
- case  ((WriteData[0] & 0x20) != 0)    WriteData[2] valid    
- case  ((WriteData[0] & 0x40) != 0)    WriteData[3] valid
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;如果没有卡返回CS_SMC_NO_CARD；如果智能卡繁忙则返回CS_SMC_CARD_BUSY；如果超时返回CS_SMC_TIMEOUT;如果卡号无效返回CS_SMC_INVALID_INDEX
*/
INT CSSMCSetProtocol
(
	DWORD  dCardIndex,
	BYTE  *WriteData,
	BYTE  *ResponseData
);

/**
@brief 智能卡电源关闭

@param[in] dCardIndex 智能卡卡槽号
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;
*/
INT CSSMCPowerDown(DWORD dCardIndex);

/**
@brief 重置智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[out] pATR 重置智能卡的应答信号
@param[out] pAtrLength pATR的长度
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE;如果没有卡返回CS_SMC_NO_CARD；如果智能卡繁忙则返回CS_SMC_CARD_BUSY
*/
INT CSSMCResetSmartCard
(
	DWORD dCardIndex,
	BYTE* pATR,
	DWORD* pAtrLength
);

/**
@brief 检测智能卡

@param[in] dCardIndex 智能卡卡槽号
@return 如果成功返回CS_SMC_SUCCESS;如果没有卡返回CS_SMC_NO_CARD；如果无效的卡号则返回CS_SMC_INVALID_INDEX
*/
INT CSSMCDetectSmartCard(DWORD dCardIndex);

/**
@brief 获取智能卡历史字节数据

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] historyData 存储历史字节数据空间
@param[in/out] pSize 返回历史字节个数
@return 如果成功返回CS_SMC_SUCCESS;如果没有卡返回CS_SMC_NO_CARD；如果无效的卡号则返回CS_SMC_INVALID_INDEX
*/
INT CSSMCGetHistorical(DWORD  dCardIndex, unsigned char* historyData ,DWORD* pSize);

/**
@brief 设置智能卡的波特率

@param[in] dCardIndex 智能卡卡槽号
@param[in] BaudRate 波特率
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；
*/
INT CSSMCSetBaudRate(DWORD dCardIndex, DWORD BaudRate);

/**
@brief 向智能卡写数据

@param[in] dCardIndex 智能卡卡槽号
@param[in] pWriteBuf 准备写数据的存储区
@param[in] NumberToWrite 准备写数据的长度
@param[out] pNumberWrite 已写的数据长度
@param[in] Timeout 超时时间
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；
*/
INT CSSMCSend(DWORD dCardIndex,
							BYTE  *pWriteBuf,
							DWORD NumberToWrite,
							DWORD *pNumberWrite,
							DWORD  Timeout);

/**
@brief 从智能卡读数据

@param[in] dCardIndex 智能卡卡槽号
@param[in] pReadBuf 读取数据存储区
@param[in] NumberToRead 读取空间内存长度
@param[out] pNumberRead 已读取的数据长度
@param[in] Timeout 超时时间
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；
*/
INT CSSMCReceive(DWORD dCardIndex,
								BYTE  *pReadBuf,
								DWORD NumberToRead,
								DWORD *pNumberRead,
								DWORD  Timeout);

/**
@brief 获取智能卡协议和波特率

@param[in] dCardIndex 智能卡卡槽号
@param[in] protocol 协议类型
@param[in] rate 最大波特率
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；如果未检测到智能卡返回CS_SMC_NO_CARD
*/
INT CSSMCGetSCCapability(DWORD dCardIndex, DWORD *protocol, DWORD* rate);

/**
@brief 获取智能卡波特率

@param[in] dCardIndex 智能卡卡槽号
@param[in] protocol 协议类型
@param[in] rate 最大波特率
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；如果未检测到智能卡返回CS_SMC_NO_CARD
*/
INT CSSMCGetSCProtocol(DWORD dCardIndex, DWORD *protocol, DWORD *rate);

/**
@brief 智能卡挂起

@param[in] dCardIndex 智能卡卡槽号
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；如果未检测到智能卡返回CS_SMC_NO_CARD
*/
INT CSSMCAbortSmartCard(DWORD dCardIndex);

/**
@brief 锁定智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] pnCardStatus 智能卡状态
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；如果未检测到智能卡返回CS_SMC_NO_CARD
*/
INT CSSMCLockSmartCard(DWORD dCardIndex, INT *pnCardStatus);

/**
@brief 唤醒智能卡

@param[in] dCardIndex 智能卡卡号
@param[in/out] pnCardStatus 智能卡状态
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE；如果未检测到智能卡返回CS_SMC_NO_CARD
*/
INT CSSMCUnlockSmartCard(DWORD dCardIndex, INT *pnCardStatus);

/**
@brief 配置智能卡标准及协议类型

@param[in] dCardIndex 智能卡卡槽号
@param[in] eProtocol  协议T0/T1/T14
@param[in] eStandard  智能卡通讯标准
@param[in] dwUserData 预留
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE
*/
INT CSSMCConfig(DWORD dCardIndex,E_SmcPrococol eProtocol,E_SmcStandard eStandard,DWORD dwUserData);

/**
@brief 激活智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] pnCardStatus  智能卡状态
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE
*/
INT CSSMCActiveSmartCard(DWORD dCardIndex, INT* pnCardStatus);

/**
@brief 检测智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] pnCardStatus  智能卡状态
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE
*/
INT CSSMCDeactiveSmartCard(DWORD dCardIndex, INT* pnCardStatus);

/**
@brief 复位智能卡

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] pATR  存储智能卡复位应答数据
@param[in/out] pAtrLength 复位应答数据长度
@param[in] bColdReset 复位方式(热复位还是冷复位)
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE
*/
INT CSSMCResetSmartCardEx
(
	DWORD dCardIndex,
	BYTE* pATR,
	DWORD* pAtrLength,
	BOOL bColdReset
);

/**
@brief 获取智能卡时钟频率

@param[in] dCardIndex 智能卡卡槽号
@param[in/out] dwclockfrequency  存储获取到的时钟频率
@return 如果成功返回CS_SMC_SUCCESS;如果失败返回CS_SMC_FAILURE
*/
INT CSSMCGetSCClockFrequency(DWORD dCardIndex, DWORD *dwclockfrequency);

/**
@brief 设置当前机器用的是正向卡板还是反向卡板

@param[in] bIsReverseDir TURE:为反向卡板，FALSE：为正向卡板
@                        默认情况为正向卡板
@return None
*/
void CSSMCSetCardReaderResetDir(BOOL bIsReverseDir);
#ifdef __cplusplus
}
#endif
#endif

