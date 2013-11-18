/**@defgroup  
@brief 
1、本模块定义的ip地址采用形如"192.168.1.1"的ASII码字符串方式表示，定义最大长度为16个字节(最后一个字节为\0)
2、本模块定义的ip地址范围采用形如"192.168.1.1~255"的字符串方式表示，定义最大长度为20(最后一个字节为\0)
2、本模块使用的MAC地址采用形如{0x00,0x0e,0x09,0x11,0x12,0xe1}的无符号BYTE数组方式表示，定义的长度是6个字节

@version 2.0.1 2011/11/14 
@{
*/

#ifndef _CS_APCLTR_CLIENT_H_
#define _CS_APCLTR_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"

#define CSAPCTRLC_IP_LEN (16)				///<IP地址最大长度,形如"192.168.1.1"
#define CSAPCTRLC_IP_RANGE_LEN (24)			///<Ip地址范围，形如"192.168.1.2~254"
#define CSAPCTRLC_MAC_LEN (6)				///<MAC地址长度，形如{0x00,0x0e,0x09,0x11,0x12,0xe1}
#define CSAPCTRLC_URL_LEN (256)				///<URL长度

/**
 * 路由规则类型定义
 * 支持按位与操作
 * 末4位为过滤规则类型0x1 ~ 0x8
 * 末4 ~ 8位为转换规则类型0x10 ~ 0x80
 * 末8~12位为QOS规则类型 0x100 ~ 0x800
 * 末12~16位为静态路由规则0x1000 ~ 0x8000
 * */
typedef enum{
	EM_APCTRLC_RULETYPE_FILTER = 0x1,
	EM_APCTRLC_RULETYPE_URL_FILTER = 0x2,
	EM_APCTRLC_RULETYPE_NAT_IPPORT = 0x10,
	EM_APCTRLC_RULETYPE_NAT_PORT_RANGE = 0x20,
	EM_APCTRLC_RULETYPE_NAT_APP = 0x40,
	EM_APCTRLC_RULETYPE_QOS_SEPARATELY = 0x100,
	EM_APCTRLC_RULETYPE_QOS_TOTAL = 0x200,
	EM_APCTRLC_RULETYPE_STATIC_ROUTE = 0x1000
}CSAPCTRLCRuleType_E;

typedef enum{
	EM_APCTRLC__PROTOCOL_TYPE_ALL = 0x1,
	EM_APCTRLC__PROTOCOL_TYPE_TCP = 0x2,
	EM_APCTRLC__PROTOCOL_TYPE_UDP = 0x3
}CSAPCTRLCProType_E;



/**
 * 基于IP的过滤规则
 * 符合规则条件的内部本地（inside local简称IL）主机的访问将被路由器丢弃或者允许
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN]; ///< 源ip地址,空字符串""表示不关心源Ip地址
	unsigned char aucILMac[CSAPCTRLC_MAC_LEN];	///<源mac地址，全为0，表示不关心源mac地址。
	char aucDesinationIp[CSAPCTRLC_IP_LEN];///< 目标ip地址,空支付串""表示不关心目标地址
	CSAPCTRLCProType_E eApplyProtocolType;
	unsigned int nILPortLowerLimit;   ///<源端口上限，当eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALL时，端口范围无效
	unsigned int nILPortUpperLimit;   ///<源端口下限，当eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALL时，端口范围无效
	unsigned int nDesPortLowerLimit;  ///<目的端口上限，当eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALL时，端口范围无效
	unsigned int nDesPortUpperLimit; ///<目的端口下限，当eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALL时，端口范围无效
	CSUDI_BOOL bDrop;//指定过滤规则，true表示符合过滤条件的访问是被丢弃，false表示符合过滤条件的访问被接受
}CSAPCTRLCRuleFilter_S;

/**
 * 基于url的过滤规则
 * 所有到指定目标url的访问将被丢弃
 * */
typedef struct{
	char szDesinationUrl[CSAPCTRLC_URL_LEN];
	unsigned int nDesinationPort; 
}CSAPCTRLCRuleFilterUrl_S;

/**
 * 基于IP的端口地址转换规则
 * NAT： Network Address Translation 网络地址转换规则
 * IL：inside local 内部本地主机（子网主机，不可被外网访问的）
 * IG：inside global 内部全局主机（可被外网访问的）
 * 将指定IP的本地内部主机的指定端口号转换映射到内部全局主机的指定端口号
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN];
	unsigned int nILPort;
	unsigned int nIGPort;
	CSAPCTRLCProType_E eApplyProtocolType;
}CSAPCTRLCRuleNatIpPort_S;

/**
 * 基于IP的端口段映射地址转换规则
 * NAT： Network Address Translation 网络地址转换规则
 * IL：inside local 内部本地主机（子网主机，不可被外网访问的）
 * IG：inside global 内部全局主机（可被外网访问的）
 * 将指定IP的本地内部主机的端口段直接被映射到内部全局主机的对应端口(端口不转换)
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN];
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLCProType_E eApplyProtocolType;
}CSAPCTRLCRuleNatPortRange_S;


/**
 * 特殊应用程序地址转换规则
 * NAT：network address Translation 网络地址转换规则
 * IL:inside local  内部本地主机
 * OG：outside gloabl 外部全局主机，一般为服务器。
 * 某些程序需要多条连接，如Internet游戏，视频会议，网络电话等。由于防火墙的存在，这些程序无法在简单的NAT 路由下工作。
 * 特殊应用程序地址转换规则就是解决该问题的，在特殊应用程序地址转换规则下：
 * 当内部本地主机向外部全局服务器的触发端口发出连接请求时，其内部本地主机的所有开放端口将被打开，允许外部全局服务器通过这些开放端口与内部本地主机建立连接请求；
 * */
typedef struct{
	unsigned int nOGTriggerPort;
	CSAPCTRLCProType_E eTriggerProtocolType;
	unsigned int  nILOpenPortLowerLimit;
	unsigned int  nILOpenPortUpperLimit;
	CSAPCTRLCProType_E eOpenProtocolType;
}CSAPCTRLCRuleNatApp_S;

/**
 * 独立的Qos规则
 * 控制符合条件的内部本地主机的上/下行带宽
 * 条件定义为：符合Ip条件范围，且符合端口条件范围，且符合适用协议的访问。
 * @note:
 * 		Qos规则只有在CSAPCTRLCEnableQos(1)时才会生效
 * */
 typedef struct{
	char aucILIpLowerLimit[CSAPCTRLC_IP_RANGE_LEN]; ///< 本地地址范围，形如"192.168.1.1~254"
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLCProType_E eApplyProtocolType;
	unsigned int nUpBandwidthLowerLimit;
	unsigned int nUpBandwidthUpperLimit;
	unsigned int nDownBandwidthLowerLimit;
	unsigned int nDownBandwidthUpperLimit;
}CSAPCTRLCRuleQosSeparately_S;

/**
 * 全局QOS规则
 * @note:
 * 		Qos规则只有在CSAPCTRLCEnableQos(1)时才会生效
 * */
 typedef struct{
	unsigned int nUpBandwidth;
	unsigned int nDownBandwidth;
}CSAPCTRLCRuleQosTotal_S;

/**
 * 静态路由
 * 指定从所有内部本地主机到指定目标外部全局（outside Global简称OG）地址范围的网关。
 * 由aucDestinationOGIp 指定适用于静态路由的外部全局地址
 * 由aucGatewayIp 指定静态路由通过的网关
 * @note
 *     静态路由规则只有在CSAPCTRLCEnableStaticRoute(1)时才会生效
 * */
typedef struct{
	char aucDestinationOGIp[CSAPCTRLC_IP_RANGE_LEN]; ///<若指定目标主机，则字符串格式为"192.168.1.5"；若指定目标网络，则字符串格式为"192.168.1.0/24"
	char aucGatewayIp[CSAPCTRLC_IP_LEN];//网关ip
}CSAPCTRLCRuleStaticRoute_S;

/**
 * 路由规则
*/
typedef struct{
	CSUDI_BOOL bEnable; ///<路由规则是否生效
	CSAPCTRLCRuleType_E eRuleType;///<路由规则类型
	union{
		CSAPCTRLCRuleFilter_S stIpFilter; 		///<基于ip和mac的过滤规则，eRuleType==EM_APCTRLC_RULETYPE_FILTER时有效
		CSAPCTRLCRuleFilterUrl_S stURlFilter;	///<基于url的过滤规则，eRuleType==EM_APCTRLC_RULETYPE_URL_FILTER时有效
		CSAPCTRLCRuleNatIpPort_S stNatIpPort; ///<NAT单端口转换规则，eRuleType==EM_APCTRLC_RULETYPE_NAT_IPPORT时有效
		CSAPCTRLCRuleNatPortRange_S stNatRange;///<NAT端口段转换规则,eRuleType==EM_APCTRLC_RULETYPE_NAT_PORT_RANGE时有效
		CSAPCTRLCRuleNatApp_S stNatApp;		///<特殊应用程序转换规则.eRuleType==EM_APCTRLC_RULETYPE_NAT_APP时有效
		CSAPCTRLCRuleQosSeparately_S stQos;	///<主机独立QOS规则，eRuleType==EM_APCTRLC_RULETYPE_QOS_SEPARATELY时有效
		CSAPCTRLCRuleQosTotal_S stQosTotal;	///<全局QOS规则,eRuleType==EM_APCTRLC_RULETYPE_QOS_TOTAL时有效
		CSAPCTRLCRuleStaticRoute_S stStaticRoute;///<静态路由规则,eRuleType==EM_APCTRLC_RULETYPE_STATIC_ROUTE时有效
	}uRule;
}CSAPCTRLCRule_S;




typedef enum{
	EM_APCTRLC_MODE_SINGL_CPE = 0x1, ///<工作模式：单CPE模式
	EM_APCTRLC_MODE_WIFI_SUBNET = 0x2, ///<工作模式：无线子网模式
	EM_APCTRLC_MODE_WIFI_BRIDGE = 0x3///<工作模式：无线桥接模式
}CSAPCTRLCMode_E;


typedef struct{
	CSUDI_BOOL bIsFilterEnable;
	CSUDI_BOOL bFilterDefaultDrop;
	CSUDI_BOOL bIsNatEnable;
	CSUDI_BOOL bIsQosEnable;
	CSUDI_BOOL bIsStaticRouteEnable;
}CSAPCTRLCRuleStatus_S;


/**
@brief 初始化
*/
CSUDI_Error_Code CSAPCTRLCInit();

/**
@brief 恢复出厂设置
*/
CSUDI_Error_Code CSAPCTRLCRecoverSetting();

/**
@brief 设置工作模式
@param[in] eMode 工作模式
@return 
@note 设置工作模式后，需要重启才能生效
*/
CSUDI_Error_Code CSAPCTRLCSetMode(CSAPCTRLCMode_E eMode);

/**
@brief 获取工作模式
*/
CSUDI_Error_Code CSAPCTRLCGetMode(CSAPCTRLCMode_E* peMode);

/**
@brief 获取正在运行的工作模式
*/
CSUDI_Error_Code CSAPCTRLCGetWorkingMode(CSAPCTRLCMode_E* peMode);

/**
@brief 获取规则
@param[in] nRuleTypeMask 需获取的规则掩码，由CSAPCTRLCRuleType_E中定义的各个枚举值与运算得到
@param[out] phOutRuleHandleArray 输出规则句柄数组
@param[in/out] pnRuleCount 输入数组大小，输出实际规则个数
*/
CSUDI_Error_Code CSAPCTRLCGetRules(int nRuleTypeMask,CSUDI_HANDLE* phOutRuleHandleArray,int* pnRuleCount);

/**
@brief 获取规则信息
@param[in] hRuleHandle 规则句柄，通过CSAPCTRLCGetRules接口获取得到
@param[out] pstOutRule 输出规则的信息
*/
CSUDI_Error_Code CSAPCTRLCGetRuleInfo(CSUDI_HANDLE hRuleHandle,CSAPCTRLCRule_S* pstOutRule);

/**
@brief 修改规则信息
@param[in] hRuleHandle 规则句柄，通过CSAPCTRLCGetRules接口获取得到
@param[in] pstRule 需修改后的规则信息
*/
CSUDI_Error_Code CSAPCTRLCModifyRule(CSUDI_HANDLE hRuleHandle,const CSAPCTRLCRule_S* pstRule);


/**
@brief 添加规则
@param[in] pstRule 规则信息
@param[out] phOutRuleHandle 输出规则句柄
*/
CSUDI_Error_Code CSAPCTRLCAddRule(const CSAPCTRLCRule_S* pstRule,CSUDI_HANDLE* phOutRuleHandle);

/**
@brief 删除规则规则
@param[in] hRuleHandle 规则句柄，通过CSAPCTRLCGetRules接口获取得到
*/
CSUDI_Error_Code CSAPCTRLCDelRule(CSUDI_HANDLE hRuleHandle);

/**
@brief 打开/关闭所有静态路由规则
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableStaticRoute(CSUDI_BOOL bEnable);

/**
@brief 打开/关闭所有QOS规则
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableQos(CSUDI_BOOL bEnable);

/**
@brief 打开/关闭所有过滤规则
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableFilter(CSUDI_BOOL bEnable);

/**
@brief 设置不符合过滤条件的访问被丢弃或者被允许
@param[in] bDrop true表示不符合过滤条件的访问被丢弃，false表示不符合过滤条件的访问被接受
*/
CSUDI_Error_Code CSAPCTRLCFilterDefaultDrop(CSUDI_BOOL bDrop);

/**
@brief 打开/关闭所有NAT规则
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableNat(CSUDI_BOOL bEnable);

/**
@brief 获取路由规则状态
*/
CSUDI_Error_Code CSAPCTRLCGetRuleStatus(CSAPCTRLCRuleStatus_S* pstOutStatus);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
