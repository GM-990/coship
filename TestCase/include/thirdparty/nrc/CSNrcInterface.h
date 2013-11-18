#ifndef _CSNRCINTERFACE_H
#define _CSNRCINTERFACE_H

#include "mmcp_typedef.h"
#include "udi2_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif

#define CSNRC_INFOLEN 32

/**@brief STB状态枚举*/
typedef enum{
	EM_NRC_STATUS_DVB,
	EM_NRC_STATUS_BROWSE,
	EM_NRC_STATUS_VOD,
	EM_NRC_STATUS_INVALID
 }CSNrcStatus_E;

typedef struct{
	char m_acVersion[CSNRC_INFOLEN];		///<版本信息，包括IPT版本号、NRC版本号、IPT协议版本号
	char m_acRf[CSNRC_INFOLEN];			///<上行信息，包括RF频点、RF地址
	char m_acLinkstatus[CSNRC_INFOLEN];	///<通信状态，连接/未连接、连接时间
	char m_acRssi[CSNRC_INFOLEN];		///<窄带电平，上行RSSI、下行RSSI
	char m_acSignalQuality[CSNRC_INFOLEN];	///<信号品质，RF错误包个数、RF丢包率
	char m_acTsInfo[CSNRC_INFOLEN];		///<下行信息，TS主频点的频率、符号率、调制方式
	char m_acIpgIp[CSNRC_INFOLEN];		///<局端地址，IPG IP
	char m_acErrorInfo[CSNRC_INFOLEN];		///<诊断结果，
}CSNrcStatusInfo_S;

typedef struct{
	char m_acTsInfo[CSNRC_INFOLEN];			///<下行频点：TS主频点的频率、符号率和调制方式
	char m_acTunerStatus[CSNRC_INFOLEN];		///<锁频状态：TS主频点的锁频状态
	char m_acSignalLevel[CSNRC_INFOLEN];		///<信号强度：TUNER的信号强度
	char m_acIPQAMIp[CSNRC_INFOLEN];		///<转发配置：IPQAM的IP地址、域
	char m_acRfLink[CSNRC_INFOLEN];			///<RF  测试：RF测试的总包数、丢包数、延时时间、质量评估
	char m_acTsLink[CSNRC_INFOLEN];			///<TS  测试：TS测试的总包数、丢包数、延时时间，质量评估
	char m_acUartInfo[CSNRC_INFOLEN];			///<串口收发：串口发送字节数、接收字节数
	char m_acErrorInfo[CSNRC_INFOLEN];		///<诊断结果
}CSNrcTestResult_S;

/**
@brief 通知NRC库， STB业务切换

@param[in] eMode STB将切换至何种状态,取值参见CSNrcStatus_E
@return 成功TRUE，失败返回FALSE
*/
BOOL CSNrcSwitch(CSNrcStatus_E eMode);

/**
@brief 设置NRC点播频点

@param[in] sDeliver VOD点播频点
@return 成功返回TRUE，失败返回FALSE
*/
BOOL CSNrcSetVodDeliver(CSUDITunerCableDeliver_S sDeliver);

/**
@brief 获取当前NRC状态，调用此接口前需要调用CSNrcSwitch切换至EM_NRC_STATUS_BROWSE状态

@param[out] psStatusInfo NRC状态信息
@return 成功返回TRUE，失败返回FALSE
@note 此接口用于获取静态的当前NRC状态
*/
BOOL CSNrcGetStatusInfo(CSNrcStatusInfo_S* psStatusInfo);

/**
@brief NRC状态测试

@param[out] psTestInfo NRC测试结果信息
@return 成功返回TRUE，失败返回FALSE
@note 此接口需要被多次调用以获取动态的信息，调用此接口前需要调用CSNrcSwitch切换至EM_NRC_STATUS_BROWSE状态
*/
BOOL CSNrcLinkTest(CSNrcTestResult_S* psTestInfo);

#ifdef __cplusplus
}
#endif

#endif

