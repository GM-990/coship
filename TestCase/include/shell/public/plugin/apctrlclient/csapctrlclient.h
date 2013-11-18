/**@defgroup  
@brief 
1����ģ�鶨���ip��ַ��������"192.168.1.1"��ASII���ַ�����ʽ��ʾ��������󳤶�Ϊ16���ֽ�(���һ���ֽ�Ϊ\0)
2����ģ�鶨���ip��ַ��Χ��������"192.168.1.1~255"���ַ�����ʽ��ʾ��������󳤶�Ϊ20(���һ���ֽ�Ϊ\0)
2����ģ��ʹ�õ�MAC��ַ��������{0x00,0x0e,0x09,0x11,0x12,0xe1}���޷���BYTE���鷽ʽ��ʾ������ĳ�����6���ֽ�

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

#define CSAPCTRLC_IP_LEN (16)				///<IP��ַ��󳤶�,����"192.168.1.1"
#define CSAPCTRLC_IP_RANGE_LEN (24)			///<Ip��ַ��Χ������"192.168.1.2~254"
#define CSAPCTRLC_MAC_LEN (6)				///<MAC��ַ���ȣ�����{0x00,0x0e,0x09,0x11,0x12,0xe1}
#define CSAPCTRLC_URL_LEN (256)				///<URL����

/**
 * ·�ɹ������Ͷ���
 * ֧�ְ�λ�����
 * ĩ4λΪ���˹�������0x1 ~ 0x8
 * ĩ4 ~ 8λΪת����������0x10 ~ 0x80
 * ĩ8~12λΪQOS�������� 0x100 ~ 0x800
 * ĩ12~16λΪ��̬·�ɹ���0x1000 ~ 0x8000
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
 * ����IP�Ĺ��˹���
 * ���Ϲ����������ڲ����أ�inside local���IL�������ķ��ʽ���·����������������
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN]; ///< Դip��ַ,���ַ���""��ʾ������ԴIp��ַ
	unsigned char aucILMac[CSAPCTRLC_MAC_LEN];	///<Դmac��ַ��ȫΪ0����ʾ������Դmac��ַ��
	char aucDesinationIp[CSAPCTRLC_IP_LEN];///< Ŀ��ip��ַ,��֧����""��ʾ������Ŀ���ַ
	CSAPCTRLCProType_E eApplyProtocolType;
	unsigned int nILPortLowerLimit;   ///<Դ�˿����ޣ���eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nILPortUpperLimit;   ///<Դ�˿����ޣ���eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nDesPortLowerLimit;  ///<Ŀ�Ķ˿����ޣ���eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	unsigned int nDesPortUpperLimit; ///<Ŀ�Ķ˿����ޣ���eApplyProtocolType==EM_APCTRLC__PROTOCOL_TYPE_ALLʱ���˿ڷ�Χ��Ч
	CSUDI_BOOL bDrop;//ָ�����˹���true��ʾ���Ϲ��������ķ����Ǳ�������false��ʾ���Ϲ��������ķ��ʱ�����
}CSAPCTRLCRuleFilter_S;

/**
 * ����url�Ĺ��˹���
 * ���е�ָ��Ŀ��url�ķ��ʽ�������
 * */
typedef struct{
	char szDesinationUrl[CSAPCTRLC_URL_LEN];
	unsigned int nDesinationPort; 
}CSAPCTRLCRuleFilterUrl_S;

/**
 * ����IP�Ķ˿ڵ�ַת������
 * NAT�� Network Address Translation �����ַת������
 * IL��inside local �ڲ������������������������ɱ��������ʵģ�
 * IG��inside global �ڲ�ȫ���������ɱ��������ʵģ�
 * ��ָ��IP�ı����ڲ�������ָ���˿ں�ת��ӳ�䵽�ڲ�ȫ��������ָ���˿ں�
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN];
	unsigned int nILPort;
	unsigned int nIGPort;
	CSAPCTRLCProType_E eApplyProtocolType;
}CSAPCTRLCRuleNatIpPort_S;

/**
 * ����IP�Ķ˿ڶ�ӳ���ַת������
 * NAT�� Network Address Translation �����ַת������
 * IL��inside local �ڲ������������������������ɱ��������ʵģ�
 * IG��inside global �ڲ�ȫ���������ɱ��������ʵģ�
 * ��ָ��IP�ı����ڲ������Ķ˿ڶ�ֱ�ӱ�ӳ�䵽�ڲ�ȫ�������Ķ�Ӧ�˿�(�˿ڲ�ת��)
 * */
typedef struct{
	char aucILIp[CSAPCTRLC_IP_LEN];
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLCProType_E eApplyProtocolType;
}CSAPCTRLCRuleNatPortRange_S;


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
	CSAPCTRLCProType_E eTriggerProtocolType;
	unsigned int  nILOpenPortLowerLimit;
	unsigned int  nILOpenPortUpperLimit;
	CSAPCTRLCProType_E eOpenProtocolType;
}CSAPCTRLCRuleNatApp_S;

/**
 * ������Qos����
 * ���Ʒ����������ڲ�������������/���д���
 * ��������Ϊ������Ip������Χ���ҷ��϶˿�������Χ���ҷ�������Э��ķ��ʡ�
 * @note:
 * 		Qos����ֻ����CSAPCTRLCEnableQos(1)ʱ�Ż���Ч
 * */
 typedef struct{
	char aucILIpLowerLimit[CSAPCTRLC_IP_RANGE_LEN]; ///< ���ص�ַ��Χ������"192.168.1.1~254"
	unsigned int nILPortLowerLimit;
	unsigned int nILPortUpperLimit;
	CSAPCTRLCProType_E eApplyProtocolType;
	unsigned int nUpBandwidthLowerLimit;
	unsigned int nUpBandwidthUpperLimit;
	unsigned int nDownBandwidthLowerLimit;
	unsigned int nDownBandwidthUpperLimit;
}CSAPCTRLCRuleQosSeparately_S;

/**
 * ȫ��QOS����
 * @note:
 * 		Qos����ֻ����CSAPCTRLCEnableQos(1)ʱ�Ż���Ч
 * */
 typedef struct{
	unsigned int nUpBandwidth;
	unsigned int nDownBandwidth;
}CSAPCTRLCRuleQosTotal_S;

/**
 * ��̬·��
 * ָ���������ڲ�����������ָ��Ŀ���ⲿȫ�֣�outside Global���OG����ַ��Χ�����ء�
 * ��aucDestinationOGIp ָ�������ھ�̬·�ɵ��ⲿȫ�ֵ�ַ
 * ��aucGatewayIp ָ����̬·��ͨ��������
 * @note
 *     ��̬·�ɹ���ֻ����CSAPCTRLCEnableStaticRoute(1)ʱ�Ż���Ч
 * */
typedef struct{
	char aucDestinationOGIp[CSAPCTRLC_IP_RANGE_LEN]; ///<��ָ��Ŀ�����������ַ�����ʽΪ"192.168.1.5"����ָ��Ŀ�����磬���ַ�����ʽΪ"192.168.1.0/24"
	char aucGatewayIp[CSAPCTRLC_IP_LEN];//����ip
}CSAPCTRLCRuleStaticRoute_S;

/**
 * ·�ɹ���
*/
typedef struct{
	CSUDI_BOOL bEnable; ///<·�ɹ����Ƿ���Ч
	CSAPCTRLCRuleType_E eRuleType;///<·�ɹ�������
	union{
		CSAPCTRLCRuleFilter_S stIpFilter; 		///<����ip��mac�Ĺ��˹���eRuleType==EM_APCTRLC_RULETYPE_FILTERʱ��Ч
		CSAPCTRLCRuleFilterUrl_S stURlFilter;	///<����url�Ĺ��˹���eRuleType==EM_APCTRLC_RULETYPE_URL_FILTERʱ��Ч
		CSAPCTRLCRuleNatIpPort_S stNatIpPort; ///<NAT���˿�ת������eRuleType==EM_APCTRLC_RULETYPE_NAT_IPPORTʱ��Ч
		CSAPCTRLCRuleNatPortRange_S stNatRange;///<NAT�˿ڶ�ת������,eRuleType==EM_APCTRLC_RULETYPE_NAT_PORT_RANGEʱ��Ч
		CSAPCTRLCRuleNatApp_S stNatApp;		///<����Ӧ�ó���ת������.eRuleType==EM_APCTRLC_RULETYPE_NAT_APPʱ��Ч
		CSAPCTRLCRuleQosSeparately_S stQos;	///<��������QOS����eRuleType==EM_APCTRLC_RULETYPE_QOS_SEPARATELYʱ��Ч
		CSAPCTRLCRuleQosTotal_S stQosTotal;	///<ȫ��QOS����,eRuleType==EM_APCTRLC_RULETYPE_QOS_TOTALʱ��Ч
		CSAPCTRLCRuleStaticRoute_S stStaticRoute;///<��̬·�ɹ���,eRuleType==EM_APCTRLC_RULETYPE_STATIC_ROUTEʱ��Ч
	}uRule;
}CSAPCTRLCRule_S;




typedef enum{
	EM_APCTRLC_MODE_SINGL_CPE = 0x1, ///<����ģʽ����CPEģʽ
	EM_APCTRLC_MODE_WIFI_SUBNET = 0x2, ///<����ģʽ����������ģʽ
	EM_APCTRLC_MODE_WIFI_BRIDGE = 0x3///<����ģʽ�������Ž�ģʽ
}CSAPCTRLCMode_E;


typedef struct{
	CSUDI_BOOL bIsFilterEnable;
	CSUDI_BOOL bFilterDefaultDrop;
	CSUDI_BOOL bIsNatEnable;
	CSUDI_BOOL bIsQosEnable;
	CSUDI_BOOL bIsStaticRouteEnable;
}CSAPCTRLCRuleStatus_S;


/**
@brief ��ʼ��
*/
CSUDI_Error_Code CSAPCTRLCInit();

/**
@brief �ָ���������
*/
CSUDI_Error_Code CSAPCTRLCRecoverSetting();

/**
@brief ���ù���ģʽ
@param[in] eMode ����ģʽ
@return 
@note ���ù���ģʽ����Ҫ����������Ч
*/
CSUDI_Error_Code CSAPCTRLCSetMode(CSAPCTRLCMode_E eMode);

/**
@brief ��ȡ����ģʽ
*/
CSUDI_Error_Code CSAPCTRLCGetMode(CSAPCTRLCMode_E* peMode);

/**
@brief ��ȡ�������еĹ���ģʽ
*/
CSUDI_Error_Code CSAPCTRLCGetWorkingMode(CSAPCTRLCMode_E* peMode);

/**
@brief ��ȡ����
@param[in] nRuleTypeMask ���ȡ�Ĺ������룬��CSAPCTRLCRuleType_E�ж���ĸ���ö��ֵ������õ�
@param[out] phOutRuleHandleArray �������������
@param[in/out] pnRuleCount ���������С�����ʵ�ʹ������
*/
CSUDI_Error_Code CSAPCTRLCGetRules(int nRuleTypeMask,CSUDI_HANDLE* phOutRuleHandleArray,int* pnRuleCount);

/**
@brief ��ȡ������Ϣ
@param[in] hRuleHandle ��������ͨ��CSAPCTRLCGetRules�ӿڻ�ȡ�õ�
@param[out] pstOutRule ����������Ϣ
*/
CSUDI_Error_Code CSAPCTRLCGetRuleInfo(CSUDI_HANDLE hRuleHandle,CSAPCTRLCRule_S* pstOutRule);

/**
@brief �޸Ĺ�����Ϣ
@param[in] hRuleHandle ��������ͨ��CSAPCTRLCGetRules�ӿڻ�ȡ�õ�
@param[in] pstRule ���޸ĺ�Ĺ�����Ϣ
*/
CSUDI_Error_Code CSAPCTRLCModifyRule(CSUDI_HANDLE hRuleHandle,const CSAPCTRLCRule_S* pstRule);


/**
@brief ��ӹ���
@param[in] pstRule ������Ϣ
@param[out] phOutRuleHandle ���������
*/
CSUDI_Error_Code CSAPCTRLCAddRule(const CSAPCTRLCRule_S* pstRule,CSUDI_HANDLE* phOutRuleHandle);

/**
@brief ɾ���������
@param[in] hRuleHandle ��������ͨ��CSAPCTRLCGetRules�ӿڻ�ȡ�õ�
*/
CSUDI_Error_Code CSAPCTRLCDelRule(CSUDI_HANDLE hRuleHandle);

/**
@brief ��/�ر����о�̬·�ɹ���
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableStaticRoute(CSUDI_BOOL bEnable);

/**
@brief ��/�ر�����QOS����
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableQos(CSUDI_BOOL bEnable);

/**
@brief ��/�ر����й��˹���
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableFilter(CSUDI_BOOL bEnable);

/**
@brief ���ò����Ϲ��������ķ��ʱ��������߱�����
@param[in] bDrop true��ʾ�����Ϲ��������ķ��ʱ�������false��ʾ�����Ϲ��������ķ��ʱ�����
*/
CSUDI_Error_Code CSAPCTRLCFilterDefaultDrop(CSUDI_BOOL bDrop);

/**
@brief ��/�ر�����NAT����
@param[in] bEnable
*/
CSUDI_Error_Code CSAPCTRLCEnableNat(CSUDI_BOOL bEnable);

/**
@brief ��ȡ·�ɹ���״̬
*/
CSUDI_Error_Code CSAPCTRLCGetRuleStatus(CSAPCTRLCRuleStatus_S* pstOutStatus);

#ifdef __cplusplus
}
#endif

/** @} */

#endif
