/**@defgroup SMC SMARTCARD 定义了智能卡相关接口
@brief 智能模块对应头文件<udi2_smc.h>，包括智能卡相关的操作接口 。

智能卡模块应该叫做智能卡槽模块，模块所有接口、设备对应的，都是指终端上的智能卡插槽而非智能卡。

卡槽的数量是指真正可用的且可能要插入智能卡的卡槽。如果主芯片支持两个卡槽，但系统只用了一个，那么此时应该算只有一个智能卡卡槽。

实际项目中，我们发现不同CA的智能卡在性能上，存在较大差异。因此UDI可能需要面向不同的CA进行不同的性能设置并进行调试。UDI的测试只会使用最终产品所选用的智能卡进行，并保证智能卡驱动能与此类卡正常通信。比如最终的产品为一款Irdeto CA的终端，则所有智能卡相关的测试均会使用Irdeto智能卡进行，无需关注该部分移植层是否可正常与其它CA卡正常通信。

@note  因为NDS CA使用ICAM模块与智能卡通讯，所以如果为NDS产品，则需遵循NDS规范使用芯片厂商提供的ICAM模块，而无需实现本模块。

@version 2.0.1 2009/8/18 
@{
*/

#ifndef _UDI2_SMC_H_
#define _UDI2_SMC_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**@brief 智能卡相关错误代码值*/
enum
{
	CSUDISMC_ERROR_BAD_PARAMETER = CSUDI_SMC_ERROR_BASE,		///< 错误参数
	CSUDISMC_ERROR_ERROR_PARITY,	///< 错误的校验值
	CSUDISMC_ERROR_ERROR_ANSWER,	///< 应答错误,例如应答的数据校验失败
	CSUDISMC_ERROR_TIMEOUT,			///< 超时错误
	
	CSUDISMC_ERROR_OUT ,			///< 智能卡拔出状态或错误      								 
	CSUDISMC_ERROR_IN,				///< 智能卡插入状态或错误
	CSUDISMC_ERROR_CARD_BUSY,		///< 智能卡繁忙状态或错误
	CSUDISMC_ERROR_NOT_LOCK_OWNER,	///< 智能卡锁定状态，不允许执行其它指令
	CSUDISMC_ERROR_READY,			///< 智能卡准备好

	CSUDISMC_ERROR_MUTE,			///< 无效卡，例如反插卡,插入名片，插入饭卡等不能识别的卡片
	CSUDISMC_ERROR_ERROR_CARD,		///< 错误卡，例如插入非本CA系统的或磨损严重的卡片，但可以识别出是CA卡
	CSUDISMC_ERROR_INVALID_PROTOCOL,///< 非法协议
	CSUDISMC_ERROR_NOT_RESET,		///< 智能卡没有重置
	CSUDISMC_ERROR_FAILURE,			///< 智能卡通用错误
	CSUDISMC_ERROR_NOT_SUPPORT		///< 智能卡不支持该命令
};

/**@brief 智能卡协议*/
typedef enum 
{
    	EM_UDISMC_PROTOCOL_UNKNOWN,		    ///< 未知协议 
	EM_UDISMC_PROTOCOL_T0,				 
	EM_UDISMC_PROTOCOL_T1,				 
	EM_UDISMC_PROTOCOL_T14,			    
	EM_UDISMC_PROTOCOL_MAX				///< 上限  
}CSUDISMCProtocol_E;

/**@brief 智能卡标准*/
typedef enum 
{
	EM_UDISMC_STANDARD_ISO,			///< ISO标准，支持T0，T1协议
	EM_UDISMC_STANDARD_EMV2000,		///< EMV2000标准 
	EM_UDISMC_STANDARD_IRDETO,		///< 爱迪德标准 
	EM_UDISMC_STANDARD_MAX			///< 上限 
}CSUDISMCStandard_E;

/**@brief 智能卡校验位信息*/
typedef enum
{
	EM_UDISMC_8BITS_NO_PARITY,     ///< 8位无奇偶校验
	EM_UDISMC_8BITS_ODD_PARITY,    ///< 8位奇校验
	EM_UDISMC_8BITS_EVEN_PARITY,   ///< 8位偶校验
	EM_UDISMC_PARITY_MAX                ///<上限
}CSUDISMCDataBits_E;

/**@brief 停止位类型枚举定义*/
typedef enum
{
	EM_UDISMC_STOP_0,    ///< 无停止位
	EM_UDISMC_STOP_1,    ///< 1个bit的停止位
	EM_UDISMC_STOP_2,    ///< 2个bit的停止位
	EM_UDISMC_STOP_3,     ///< 3个bit的停止位
	EM_UDISMC_STOP_MAX ///<上限
}CSUDISMCStopBits_E;

/**@brief 智能卡属性信息*/
typedef struct
{
	int    m_nETU;                      ///< ETU:372,625...
	int    m_nBaudRate;                 ///< 波特率，例如：9600,115200
	int    m_nFrequency;                ///< 时钟频率:3570KHZ/4500KHZ
	CSUDISMCStandard_E  m_eStandard;    ///< 智能卡标准，初始化设置完后，后面的操作通过get获取此字段
	CSUDISMCProtocol_E  m_eProtocol;    ///< 智能卡传输协议:T0/T1/T14
	CSUDISMCStopBits_E  m_eStopBit;     ///< 停止位
	CSUDISMCDataBits_E  m_eCheckBit;    ///< 校验位:奇校验/偶校验/无校验	
}CSUDISMCParam_S;


/**
@brief 智能卡回调函数

@param[in] nCardIndex 智能卡卡号
@param[in] eStatus 智能卡状态，见智能卡相关错误代码值的定义
*/
typedef void (*CSUDISMCCallback)(int nCardIndex, CSUDI_Error_Code eStatus);

/**
@brief 获取系统中可用的智能卡卡槽个数

@param[out] pnSMCCount 系统中可用的智能卡卡槽个数
@note 如果主芯片支持两个卡槽，但系统只用了一个，那么此接口返回1
@return 成功获取则返回CSUDI_SUCCESS；失败则返回错误代码值，且pnSMCCount的内容未定义
*/
CSUDI_Error_Code CSUDISMCGetCount(int * pnSMCCount);

/**
@brief 打开智能卡

@param[in] nCardIndex 智能卡索引号，从0开始   
@param[in] fnSMCCallback 回调函数，详见CSUDISMCCallback定义
@return 如果成功返回CSUDI_SUCCESS；如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCOpen(int nCardIndex, CSUDISMCCallback fnSMCCallback);

/**
@brief 关闭智能卡

@param[in] nCardIndex 智能卡索引号，从0开始。   
@return  如果成功返回CSUDI_SUCCESS；如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCClose(int nCardIndex);

/**
@brief 重置智能卡

@param[in] nCardIndex 智能卡索引号，从0开始。 
@param[in] bColdRst 冷热复位标志，TRUE时冷复位，FALSE时热复位
@param[out] pucAtr 重置智能卡的应答数据
@param[out] pnAtrLength pATr的长度
@return 如果成功返回CSUDI_SUCCESS；如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCReset(int nCardIndex, unsigned char * pucAtr, int * pnAtrLength, CSUDI_BOOL bColdRst);


/**
@brief 设置智能卡参数信息

@param[in] nCardIndex 智能卡索引号   
@param[in] psSMCParams  智能卡参数信息
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 如果智能卡不存在，则返回CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCSetParams(int nCardIndex,const CSUDISMCParam_S * psSMCParams);


/**
@brief 获取智能卡参数信息

@param[in] nCardIndex 智能卡索引号   
@param[out] psSMCParams  智能卡参数信息
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值，且psSMCParams的内容未定义
@note 如果智能卡不存在，则返回CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCGetParams(int nCardIndex,CSUDISMCParam_S * psSMCParams);

/**
@brief 设备和智能卡之间的数据传输

@param[in] nCardIndex 智能卡索引号   
@param[in] pucWriteData 写入智能卡的数据
@param[in] nNumberToWrite 写入数据长度
@param[out] pcResponseData 读取数据的存储位置
@param[out] pnNumberRead 读取数据的长度
@param[out] pucStatusWord 驱动提供的状态字
@return  如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCDataExchange(
									int             nCardIndex, 
									unsigned char  *pucWriteData,
	                                int             nNumberToWrite,
	                                unsigned char  *pcResponseData,
	                                int            *pnNumberRead,
	                                unsigned char  *pucStatusWord);

/**
@brief  向智能卡写数据

@param[in] nCardIndex 智能卡索引号   
@param[in] pucWriteBuf 准备写数据的存储区
@param[in] nNumberToWrite 准备写数据的长度
@param[out] pnNumberWrite 已写的数据长度
@param[in]  nTimeout 超时时间
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCSend(
					 int             nCardIndex,
				     unsigned char  *pucWriteBuf,
				     int             nNumberToWrite,
				     int            *pnNumberWrite,
				     int             nTimeout);

/**
@brief 从智能卡读数据

@param[in] nCardIndex 智能卡索引号   
@param[in] pucReadBuf 读取数据存储区
@param[in] nNumberToRead 读取空间内存长度
@param[out] pnNumberRead 已读取的数据长度
@param[in] nTimeout 超时时间
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
*/
CSUDI_Error_Code CSUDISMCReceive(
						int             nCardIndex,
				        unsigned char  *pucReadBuf,
				        int             nNumberToRead,
				        int            *pnNumberRead,
				        int             nTimeout);


/**
@brief 设置智能卡协商模式

@param[in] nCardIndex 智能卡索引号   
@param[in] pucWriteData 一组FI,DI因子数据
@param[in] nNumberToWriteLen 发送数据的字节个数
@note
- case  ((pucWriteData[0] & 0x10) != 0)    PPS1 valid
- case  ((pucWriteData[0] & 0x20) != 0)    PPS2 valid
- case  ((pucWriteData[0] & 0x40) != 0)    PPS3 valid
@param[out] pucResponseData 设置完协商模式后驱动回应数据
@param[out] pnResponseDataLen 回应数据的字节个数
@return 如果成功返回CSUDI_SUCCESS；如果失败返回错误代码值
@note 如果传入非法协议模式，则返回CSUDISMC_ERROR_INVALID_PROTOCOL
*/
CSUDI_Error_Code CSUDISMCSetPPS(
						 int             nCardIndex,
	                    unsigned char  *pucWriteData,
                           int             nNumberToWriteLen,
	                    unsigned char  *pucResponseData,
                           int            *pnResponseDataLen);


/**
@brief  检测智能卡

@param[in] nCardIndex 智能卡索引号   
@return 返回当前卡状态
@note 如果智能卡不存在，则返回CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDetect(int nCardIndex);

/**
@brief  激活智能卡

@param[in] nCardIndex 智能卡索引号   
@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 如果智能卡不存在，则返回CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCActive(int nCardIndex);

/**
@brief  去激活

@param[in] nCardIndex 智能卡索引号   

@return 如果成功返回CSUDI_SUCCESS;如果失败返回错误代码值
@note 如果智能卡不存在，则返回CSUDISMC_ERROR_OUT
*/
CSUDI_Error_Code CSUDISMCDeactive(int nCardIndex);


#ifdef __cplusplus
}
#endif
/**@}*/
#endif

