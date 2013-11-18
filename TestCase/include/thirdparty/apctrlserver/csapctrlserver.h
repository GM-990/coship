/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

@version 2.0.1 2011/11/14 
@{
*/

#ifndef _CS_APCLTR_SERVER_H_
#define _CS_APCLTR_SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"

#define CSAPCTRLS_IP_LEN (16)				///<IP��ַ��󳤶�,����"192.168.1.1"
#define CSAPCTRLS_IP_RANGE_LEN (24)			///<Ip��ַ��Χ������"192.168.1.2~254"
#define CSAPCTRLS_MAC_LEN (6)				///<MAC��ַ���ȣ�����{0x00,0x0e,0x09,0x11,0x12,0xe1}
#define CSAPCTRLS_URL_LEN (256)				///<URL����

/**
 * ·�ɹ������Ͷ���
 * ֧�ְ�λ�����
 * ĩ4λΪ���˹�������0x1 ~ 0x8
 * ĩ4 ~ 8λΪת����������0x10 ~ 0x80
 * ĩ8~12λΪQOS�������� 0x100 ~ 0x800
 * ĩ12~16λΪ��̬·�ɹ���0x1000 ~ 0x8000
 * */
typedef enum{
	EM_APCTRLS_RULETYPE_FILTER = 0x1,
	EM_APCTRLS_RULETYPE_URL_FILTER = 0x2,
	EM_APCTRLS_RULETYPE_NAT_IPPORT = 0x10,
	EM_APCTRLS_RULETYPE_NAT_PORT_RANGE = 0x20,
	EM_APCTRLS_RULETYPE_NAT_APP = 0x40,
	EM_APCTRLS_RULETYPE_QOS_SEPARATELY = 0x100,
	EM_APCTRLS_RULETYPE_QOS_TOTAL = 0x200,
	EM_APCTRLS_RULETYPE_STATIC_ROUTE = 0x1000
}CSAPCTRLSRuleType_E;

typedef enum{
	EM_APCTRLS__PROTOCOL_TYPE_ALL = 0x1,
	EM_APCTRLS__PROTOCOL_TYPE_TCP = 0x2,
	EM_APCTRLS__PROTOCOL_TYPE_UDP = 0x3
}CSAPCTRLSProType_E;



/**
 * ����IP�Ĺ��˹���
 * ���Ϲ����������ڲ����أ�inside local���IL�������ķ��ʽ���·����������������
 * */
typedef struct{
	char aucILIp[CSAPCTRLS_IP_LEN]; ///< Դip��ַ,���ַ���""��ʾ������ԴIp��ַ
	unsigned char aucILMac[CSAPCTRLS_MAC_LEN];	///<Դmac��ַ��ȫΪ0����ʾ������Դmac��ַ��
	char aucDesinationIp[CSAPCTRLS_IP_LEN];///< Ŀ��ip��ַ,��֧����""��ʾ������Ŀ���ַ
	CSAPCTRLSProType_E eApplyProtocolType;
	unsigned int nILPortLowerLimit;   ///<Դ�˿����ޣ���eApplyProtocolType==EM_APCTRLS__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nILPortUpperLimit;   ///<Դ�˿����ޣ���eApplyProtocolType==EM_APCTRLS__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nDesPortLowerLimit;  ///<Ŀ�Ķ˿����ޣ���eApplyProtocolType==EM_APCTRLS__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nDesPortUpperLimit; ///<Ŀ�Ķ˿����ޣ���eApplyProtocolType==EM_APCTRLS__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	CSUDI_BOOL bDrop;//ָ�����˹���true��ʾ���Ϲ��������ķ����Ǳ�������false��ʾ���Ϲ��������ķ��ʱ�����
}CSAPCTRLSRuleFilter_S;

/**
 * ����url�Ĺ��˹���
 * ���е�ָ��Ŀ��url�ķ��ʽ�������
 * */
typedef struct{
	char szDesinationUrl[CSAPCTRLS_URL_LEN];
	unsigned int nDesinationPort; 
}CSAPCTRLSRuleFilterUrl_S;

/**
 * ����IP�ĵ��˿�ӳ���ַת������
 * NAT�� Network Address Translation �����ַת������
 * IL��inside local �ڲ������������������������ɱ��������ʵģ�
 * IG��inside global �ڲ�ȫ���������ɱ��������ʵģ�
 * ��ָ��IP�ı����ڲ�������ָ���˿ں�ת��ӳ�䵽�ڲ�ȫ��������ָ���˿ں�
 * */
typedef struct{
	char aucILIp[CSAPCTRLS_IP_LEN];
	unsigned int nILPort;
	unsigned int nIGPort;
	CSAPCTRLSProType_E eApplyProtocolType;
}CSAPCTRLSRuleNatIpPort_S;

/**
 * ����IP�Ķ˿ڶ�ӳ���ַת������
 * NAT�� Network Address Translation �����ַת������
 * IL��inside local �ڲ������������������������ɱ��������ʵģ�
 * IG��inside global �ڲ�ȫ���������ɱ��������ʵģ�
 * ��ָ��IP�ı����ڲ������Ķ˿ڶ�ֱ�ӱ�ӳ�䵽�ڲ�ȫ�������Ķ�Ӧ�˿�(�˿ڲ�ת��)
 * */
typedef struct{
	char aucILIp[CSAPCTRLS_IP_LEN];
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLSProType_E eApplyProtocolType;
}CSAPCTRLSRuleNatPortRange_S;


/**
 * ����Ӧ�ó����ַת������
 * NAT��network address Translation �����ַת������
 * IL:inside local  �ڲ���������
 * OG��outside gloabl �ⲿȫ��������һ��Ϊ��������
 * ĳЩ������Ҫ�������ӣ���Internet��Ϸ����Ƶ���飬����绰�ȡ����ڷ���ǽ�Ĵ��ڣ���Щ�����޷��ڼ򵥵�NAT ·���¹�����
 * ����Ӧ�ó����ַת��������ǽ��������ģ�������Ӧ�ó����ַת�������£�
 * ���ڲ������������ⲿȫ�ַ������Ĵ����˿ڷ�����������ʱ�����ڲ��������������п��Ŷ˿ڽ����򿪣������ⲿȫ�ַ�����ͨ����Щ���Ŷ˿����ڲ���������������������
 * */
typedef struct{
	unsigned int nOGTriggerPort;
	CSAPCTRLSProType_E eTriggerProtocolType;
	unsigned int  nILOpenPortLowerLimit;
	unsigned int  nILOpenPortUpperLimit;
	CSAPCTRLSProType_E eOpenProtocolType;
}CSAPCTRLSRuleNatApp_S;

/**
 * ������Qos����
 * ���Ʒ����������ڲ�������������/���д���
 * ��������Ϊ������Ip������Χ���ҷ��϶˿�������Χ���ҷ�������Э��ķ��ʡ�
 * @note:
 * 		Qos����ֻ����CSAPCTRLSEnableQos(1)ʱ�Ż���Ч
 * */
 typedef struct{
	char aucILIpLowerLimit[CSAPCTRLS_IP_RANGE_LEN]; ///< ���ص�ַ��Χ������"192.168.1.1~254"
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLSProType_E eApplyProtocolType;
	unsigned int nUpBandwidthLowerLimit;
	unsigned int nUpBandwidthUpperLimit;
	unsigned int nDownBandwidthLowerLimit;
	unsigned int nDownBandwidthUpperLimit;
}CSAPCTRLSRuleQosSeparately_S;

/**
 * ȫ��QOS����
 * @note:
 * 		Qos����ֻ����CSAPCTRLSEnableQos(1)ʱ�Ż���Ч
 * */
 typedef struct{
	unsigned int nUpBandwidth;
	unsigned int nDownBandwidth;
}CSAPCTRLSRuleQosTotal_S;

/**
 * ��̬·��
 * ָ���������ڲ�����������ָ��Ŀ���ⲿȫ�֣�outside Global���OG����ַ��Χ�����ء�
 * ��aucDestinationOGIp ָ�������ھ�̬·�ɵ��ⲿȫ�ֵ�ַ
 * ��aucGatewayIp ָ����̬·��ͨ��������
 * @note
 *     ��̬·�ɹ���ֻ����CSAPCTRLSEnableStaticRoute(1)ʱ�Ż���Ч
 * */
typedef struct{
	char aucDestinationOGIp[CSAPCTRLS_IP_RANGE_LEN]; ///<��ָ��Ŀ�����������ַ�����ʽΪ"192.168.1.5"����ָ��Ŀ�����磬���ַ�����ʽΪ"192.168.1.0/24"
	char aucGatewayIp[CSAPCTRLS_IP_LEN];//����ip
}CSAPCTRLSRuleStaticRoute_S;

/**
 * ·�ɹ���
*/
typedef struct{
	CSUDI_BOOL bEnable; ///<·�ɹ����Ƿ���Ч
	CSAPCTRLSRuleType_E eRuleType;///<·�ɹ�������
	union{
		CSAPCTRLSRuleFilter_S stIpFilter; 		///<����ip��mac�Ĺ��˹���eRuleType==EM_APCTRLS_RULETYPE_FILTERʱ��Ч
		CSAPCTRLSRuleFilterUrl_S stURlFilter;	///<����url�Ĺ��˹���eRuleType==EM_APCTRLS_RULETYPE_URL_FILTERʱ��Ч
		CSAPCTRLSRuleNatIpPort_S stNatIpPort; ///<NAT���˿�ת������eRuleType==EM_APCTRLS_RULETYPE_NAT_IPPORTʱ��Ч
		CSAPCTRLSRuleNatPortRange_S stNatRange;///<NAT�˿ڶ�ת������,eRuleType==EM_APCTRLS_RULETYPE_NAT_PORT_RANGEʱ��Ч
		CSAPCTRLSRuleNatApp_S stNatApp;		///<����Ӧ�ó���ת������.eRuleType==EM_APCTRLS_RULETYPE_NAT_APPʱ��Ч
		CSAPCTRLSRuleQosSeparately_S stQos;	///<��������QOS����eRuleType==EM_APCTRLS_RULETYPE_QOS_SEPARATELYʱ��Ч
		CSAPCTRLSRuleQosTotal_S stQosTotal;	///<ȫ��QOS����,eRuleType==EM_APCTRLS_RULETYPE_QOS_TOTALʱ��Ч
		CSAPCTRLSRuleStaticRoute_S stStaticRoute;///<��̬·�ɹ���,eRuleType==EM_APCTRLS_RULETYPE_STATIC_ROUTEʱ��Ч
	}uRule;
}CSAPCTRLSRule_S;


typedef enum{
	EM_APCTRLS_MODE_SINGL_CPE = 0x1, ///<����ģʽ����CPEģʽ
	EM_APCTRLS_MODE_WIFI_SUBNET = 0x2, ///<����ģʽ����������ģʽ
	EM_APCTRLS_MODE_WIFI_BRIDGE = 0x3,///<����ģʽ�������Ž�ģʽ
}CSAPCTRLSMode_E;


typedef struct{
	CSUDI_BOOL bIsFilterEnable;
	CSUDI_BOOL bFilterDefaultDrop;
	CSUDI_BOOL bIsNatEnable;
	CSUDI_BOOL bIsQosEnable;
	CSUDI_BOOL bIsStaticRouteEnable;
}CSAPCTRLSRuleStatus_S;


/**
@brief ��ʼ��
*/
CSUDI_Error_Code CSAPCTRLSInit(const char * pcPath);

/**
@brief �ָ���������
*/
CSUDI_Error_Code CSAPCTRLSRecoverSetting();

/**
@brief ���ù���ģʽ
@param[in] eMode ����ģʽ
@return 
@note ���ù���ģʽ����Ҫ����������Ч
*/
CSUDI_Error_Code CSAPCTRLSSetMode(CSAPCTRLSMode_E eMode);

/**
@brief ��ȡ����ģʽ
*/
CSUDI_Error_Code CSAPCTRLSGetMode(CSAPCTRLSMode_E* peMode);

/**
@brief ��ȡ�������еĹ���ģʽ
*/
CSUDI_Error_Code CSAPCTRLSGetWorkingMode(CSAPCTRLSMode_E* peMode);

/**
@brief ��ȡ����
@param[in] nRuleTypeMask ���ȡ�Ĺ������룬��CSAPCTRLSRuleType_E�ж���ĸ���ö��ֵ������õ�
@param[out] phOutRuleHandleArray �������������
@param[in/out] pnRuleCount ���������С�����ʵ�ʹ������
*/
CSUDI_Error_Code CSAPCTRLSGetRules(int nRuleTypeMask,CSUDI_HANDLE* phOutRuleHandleArray,int* pnRuleCount);

/**
@brief ��ȡ������Ϣ
@param[in] hRuleHandle ��������ͨ��CSAPCTRLSGetRules�ӿڻ�ȡ�õ�
@param[out] pstOutRule ����������Ϣ
*/
CSUDI_Error_Code CSAPCTRLSGetRuleInfo(CSUDI_HANDLE hRuleHandle,CSAPCTRLSRule_S* pstOutRule);

/**
@brief �޸Ĺ�����Ϣ
@param[in] hRuleHandle ��������ͨ��CSAPCTRLSGetRules�ӿڻ�ȡ�õ�
@param[in] pstRule ���޸ĺ�Ĺ�����Ϣ
*/
CSUDI_Error_Code CSAPCTRLSModifyRule(CSUDI_HANDLE hRuleHandle,const CSAPCTRLSRule_S* pstRule);


/**
@brief ��ӹ���
@param[in] pstRule ������Ϣ
@param[out] phOutRuleHandle ���������
*/
CSUDI_Error_Code CSAPCTRLSAddRule(const CSAPCTRLSRule_S* pstRule,CSUDI_HANDLE* phOutRuleHandle);

/**
@brief ɾ���������
@param[in] hRuleHandle ��������ͨ��CSAPCTRLSGetRules�ӿڻ�ȡ�õ�
*/
CSUDI_Error_Code CSAPCTRLSDelRule(CSUDI_HANDLE hRuleHandle);

/**
@brief ��/�ر����о�̬·�ɹ���
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLSEnableStaticRoute(CSUDI_BOOL bEnable);

/**
@brief ��/�ر�����QOS����
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLSEnableQos(CSUDI_BOOL bEnable);

/**
@brief ��/�ر����й��˹���
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLSEnableFilter(CSUDI_BOOL bEnable);

/**
@brief ���ò����Ϲ��������ķ��ʱ��������߱�����
@param[in] bDrop true��ʾ�����Ϲ��������ķ��ʱ�������false��ʾ�����Ϲ��������ķ��ʱ�����
*/
CSUDI_Error_Code CSAPCTRLSFilterDefaultDrop(CSUDI_BOOL bDrop);

/**
@brief ��/�ر�����NAT����
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLSEnableNat(CSUDI_BOOL bEnable);

/**
@brief ��ȡ·�ɹ���״̬
*/
CSUDI_Error_Code CSAPCTRLSGetRuleStatus(CSAPCTRLSRuleStatus_S* pstOutStatus);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
