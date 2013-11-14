/**@defgroup UDI2PPPOE  PPPOE ��ģ����Ҫ������PPPOE��صĽӿڣ�����pppoe�ĳ�ʼ����������ֹͣ��
@brief PPPOEģ���Ӧͷ�ļ�<udi2_pppoe.h>��������PPPOE�ĳ�ʼ�������ӡ��Ͽ��Ƚӿ� ��
pppoe��Ҫ�ṩͨ�����ţ���ȡip��ַ���ͻ�ʹ��pppoe��ʽ����������ʵ���������ơ������Ʒѵȹ���

��������в���pppoe��ʽ���в�����������Ҫ�õ��������ģ�������в���������pppoeʹ�÷ǳ��򵥣���Ӧ��
���ɵ�ʱ��ֻ��Ҫ�ȵ���CSUDIPPPOEInit����ʼ��pppoeģ�飬CSUDIPPPOEInit��Ҫ����һЩ��ʼ���Ĳ�����������
������CSUDIPPPOEInitPara_S�ṹ�����ж��壬������һ�γɹ���ȡip��ַ���ȡ�ķ��ʼ�������mac��ַ���Ựid������������
���״ν���pppoe���ӵ�ʱ����Դ��գ����������ӵ�ʱ�򣬾������ϴ����ӳɹ�֮�󱣴��macֵ��sessionidֵ
��ʼ��pppoeģ��֮�󣬵���CSUDIPPPOEConnect�Ϳ��Խ��в������ӣ���������Ҫ�����û��������룬���ųɹ��ͻ�������з���һ��
IP��ַ�������оͿ��������IP��ַ���������ˡ�

��ÿ�β��ųɹ�֮���û�Ӧ�ð�AC��mac��ַ�ͻỰid������flash�У�AC��mac��ַ���Իص�����ʽ֪ͨ���û����û����յ�CSUDIPPPOEMsgType_E
�ж����PPPOE_STATE_RUNNING��Ϣ����ʾ��ȡ����ip��ַ��Ȼ���û����Դӻص������Ĳ����б���AC��mac��ַ�ͱ��ε�sessionid��flash�У��´ο���
ֱ�Ӵ�flash�ж�����ֵ�����ʼ��ģ�飬�����ͻ�ӿ��Ժ󲦺ŵ��ٶ�

����֮���״̬����ͨ��CSUDIPPPOEGetState����ȡ����CSUDIPPPOEState_E�ж�����Ǽ���״̬����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.2 2010/10/13 ��һ���ۺ�������޸ĵİ汾
@version 2.0.1 2009/07/24 ����
@{
*/

#ifndef _UDI2_PPPOE_H_
#define _UDI2_PPPOE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifndef CSUDI_PPPOE_IP4_LEN
#define CSUDI_PPPOE_IP4_LEN  16
#endif


#ifndef CSUDI_PPPOE_MAC_LEN
#define CSUDI_PPPOE_MAC_LEN	6	
#endif

/**@brief PPPOE ��ش������ֵ*/
enum 
{
	CSUDIPPPOE_ERROR_BAD_PARAMETER = CSUDI_PPPOE_ERROR_BASE,    ///<  �Ƿ�����,���������������ƽ̨�ӿڶ���Ĳ�����Χ �������涨��ͨ�����ظô����;                       
    CSUDIPPPOE_ERROR_NOT_SUPPORTED,   ///< ��֧�ֲ���
	CSUDIPPPOE_ERROR_UNKNOWN_ERROR	, 	///< δ֪ԭ��ʧ��(ʼ��Ϊ1)  
	CSUDIPPPOE_ERROR_TIMEOUT	, 	///<  ������ʱʱ�����ƴ���  
	CSUDIPPPOE_ERROR_FATAL  , 	///< ��������,ͨ�����ڵ��ò���ϵͳAPI��CSIPƽ̨OsPorting API����ʧ��ʱ����Ҫ���ϲ㷵�ظô����
	CSUDIPPPOE_ERROR_EXIST	,	///< ��Դ���ļ��Ѿ����ڻ�ģ���Ѿ�����,ͨ���ǵ���ģ�� ��ʼ������(CSUDIPPPOE_XXX_Init)����Դ��������	 (CSUDIPPPOE_XXX_CreateYYY)ʧ��ʱ����   
	CSUDIPPPOE_ERROR_NOTEXIST	,	///< ��Դ���ļ������ڻ�ģ����ж��,ͨ���ǵ���ģ�鷴��ʼ������(CSUDIPPPOE_XXX_Cleanup)����Դ���ٺ���(CSUDIPPPOE_XXX_DestoryYYY)ʧ��ʱ����
	CSUDIPPPOE_ERROR_AUTHFAILURE,       ///< ��֤ʧ��
	CSUDIPPPOE_ERROR_OTHER,              ///< ��������
	CSUDIPPPOE_ERROR_CALLBACK_ALREADY_ADDED,      ///<�ص��Ѿ����
    CSUDIPPPOE_ERROR_CALLBACK_NOT_FOUND      ///<�ص�����������
};


/**@brief ������Ϣ*/
typedef struct
{
    char acACMac[CSUDI_PPPOE_MAC_LEN];              ///<  MAC of AC ������ {0x00,0x0e,0x09,0x11,0x22,0x33} ,ÿ�β��ųɹ�֮�󣬽�AC��mac��ַ������flash�У��Ժ󿪻����Ŵ�flash�ж�ȡ��ֵ�������ʼ��������
    unsigned short usSessionId;        ///<  PPPoE �Ựid��ÿ�λ�ȡ��ip��ַ֮�󣬽��ǴεĻỰid������flash�У��Ժ��ٿ������žʹ�flash�ж�ȡ�Ựid�������ʼ��ģ�飬�ӿ첦���ٶ�
    int nStackSize;                   ///<  pppoe�̶߳�ջ��С. 
    int nPriority;                    ///<  pppoe�߳����ȼ�
}CSUDIPPPOEInitPara_S;


/**@brief pppoe��ȡ���ĵ�ַ�����Ϣ*/
typedef struct               ///<�ýṹ���ɻص������׳�
{
	int nState;                        ///< pppoe state ��״̬����ȡ�õ�ֵΪCSUDIPPPOEState_E�����е�ֵ
	int nPID;                          ///< pppd ���̵�pidֵ 
	char acLocalIP[CSUDI_PPPOE_IP4_LEN];  	///< ��ȡ����ip��ַ, a string ended with '\0', as "255.255.255.255" 		
	char acRemoteIP [CSUDI_PPPOE_IP4_LEN];     ///< ��������ip��ַ, a string ended with '\0',as "255.255.255.255" 
	char acMask[CSUDI_PPPOE_IP4_LEN];          ///<��ȡ������������
	char acGateWay[CSUDI_PPPOE_IP4_LEN];        ///<��ȡ�������� 
	char acDNS1[CSUDI_PPPOE_IP4_LEN];           ///<��ȡ���ĵ�һ��dns
	char acDNS2[CSUDI_PPPOE_IP4_LEN];           ///<��ȡ���ĵڶ���dns
	char acACMac[CSUDI_PPPOE_MAC_LEN];         ///< MAC of AC "FFFFFFFFFFFF"����Ҫ���浽flash�У��ڳ�ʼ�������д����ֵ
	unsigned short usSessionId;                 ///<  pppoe���εĻỰid ����Ҫ���浽flash�У��ڳ�ʼ�������д����ֵ
} CSUDIPPPOECallBackPara_S;


/**@brief pppoe�ļ���״̬*/
typedef enum 
{
	EM_UDIPPPOE_PHASE_DEAD = 0, ///<  link is dead 
	EM_UDIPPPOE_ESTABLISH,      ///< ���ڽ������� 
	EM_UDIPPPOE_AUTHENTICATE,    ///<  ������֤ 
	EM_UDIPPPOE_NETWORK,        ///< ���ڻ�ȡip��ַ 
	EM_UDIPPPOE_RUNNING,        ///<  ��ȡ��ip��ַ
	EM_UDIPPPOE_AUTH_FAILED ,        ///<  ��֤ʧ��
	EM_UDIPPPOE_SESSIONID_CHANGED,     ///<  session ID �����ı� 
	EM_UDIPPPOE_SERVER_NORESPONSE,   ///<  ����������Ӧ 
	EM_UDIPPPOE_TIMEOUT ,             ///<  ��ʱ 
	EM_UDIPPPOE_CLEAR_TO_INIT,   ///<���״̬��������״̬
	EM_UDIPPPOE_TERMINATEBYPEER               ///<PPPOE�ͻ��˱��߳�
} CSUDIPPPOEState_E;


///<��Ӷ�ص�������������CSUDIPPPOEAddCallback���������������͵�callback,nStateΪCSUDIPPPOEState_E���ͣ�pParaΪCSUDIPPPOECallBackPara_S*����
typedef void  (*CSUDIPPPOECallback_F)(CSUDIPPPOEState_E nState, void *pPara, void *pvUserData);


/**
@brief ��ʼ��pppoeģ��

@param[in] psInitPara ��ʼ��ʱ��Ҫ����Ĳ���,ΪCSUDIPPPOEInitPara_S����,��ջ��С�����ȼ���0�Ļ����ǵ�ģ���ڲ���ȡĬ��ֵ
@return CSUDI_SUCCESS��ʾ��ʼ���ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  ACMAC�ͻỰid��flash�ж���(ÿ�����ӳɹ���Ҫ��������ֵ������flash��)����һ��ACMAC��ȫFF,sessionid��0
*/
CSUDI_Error_Code  CSUDIPPPOEInit(CSUDIPPPOEInitPara_S *psInitPara);


/**
@brief ����pppoeģ��

@param[in] nFlag kill��pppd���������õķ�ʽ��gentle����rude��ʽ,0Ϊgentle��ʽ��1Ϊrude��ʽ��0��ʾɱ���ý��̼��ɣ�1��ʾɱ���ý��̼����е��ӽ���
@return CSUDI_SUCCESS��ʾ���ٳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note �ú������������е�pppoe����Դ�����Ҫ�������ӣ���Ҫ���³�ʼ�����ٽ�������
*/
CSUDI_Error_Code  CSUDIPPPOECleanup (int nFlag);


/**
@brief ��������

@param[in] pcUserName �û���
@param[in] pcPasswd ����
@param[in] pcServiceName pppoe��������
@param[in] pcEthName �����������ƣ���"eth0"
@param[in] nTimeout ���ӳ�ʱʱ�䣬��λ:��
@return CSUDI_SUCCESS��ʾ���ӳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ���ӳɹ�֮�󣬻�ȡ����ip��ַ��������������ء�dns����Ϣ���Իص�����ʽ֪ͨ���û�
*/
CSUDI_Error_Code  CSUDIPPPOEConnect(const char *pcUserName, const char *pcPasswd, char *pcServiceName, char *pcEthName, int nTimeout);


/**
@brief �Ͽ�����

@param[in] nTimeout �Ͽ����ӵĳ�ʱʱ�䣬��λ:��
@return CSUDI_SUCCESS��ʾ�Ͽ����ӳɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ��Ҫ�Ͽ�pppoe���ӣ����Ե��øú������Ժ��ٽ������Ӿ�ֱ�ӵ���CSUDIPPPOEConnect
*/
CSUDI_Error_Code  CSUDIPPPOEDisconnect(int nTimeout );


/**
@brief ��ȡpppoe��������

@param[out] pcSevicename ���������ȡ��pppoe��������
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetServiceName( char *pcSevicename);


/**
@brief ��ȡpppoe״̬

@param[out] pnState ���������ȡ��pppoe��״̬,״̬��ȡֵΪCSUDIPPPOEState_E�ж���ļ���ֵ
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetState(int *pnState);


/**
@brief ��ȡAC��MAC��ַ

@param[out] pcMAC ���������ȡ��AC��MAC��ַ
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ac��mac��ַ�ĳ���Ϊ6���ֽڣ������buffer�Ŀռ�Ҫ����6���ֽ�
*/
CSUDI_Error_Code  CSUDIPPPOEGetACMacAddr(char *pcMAC);


/**
@brief ��ȡpppoe�Ựid

@param[out] pusSessionID ���������ȡpppoe�ĻỰid
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note ÿ�����ӳɹ��������һ���Ựid������ûỰid���´����ӵ�ʱ��ֱ�Ӵ��룬��ӿ������ٶ�
*/
CSUDI_Error_Code  CSUDIPPPOEGetSessionID(unsigned short *pusSessionID);


/**
@brief ��ȡpppoe�汾��

@param[out] pcVer ���������ȡpppoe�İ汾��
@return CSUDI_SUCCESS��ʾ��ȡ�ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
*/
CSUDI_Error_Code  CSUDIPPPOEGetVersion( char *pcVer);


/**
@brief ���һ��pppoe�ص�

@param[in] pvUserData �����û�������Ϣ,����Ϊ�ص�������dwUserData��������
@param[in] fnCallback Ҫ��ӵĻص�����pppoe״̬�����仯��ʱ�����øú���
@return CSUDI_SUCCESS��ʾ��ӻص��ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  ������Ӷ���ص� ,Ҫ�� pfCallback��dwUserData����ͬ��ʱ��Ų�����ӽ�ȥ���������
@note pppoeģ��֧��ע����С8���ص�����
*/
CSUDI_Error_Code CSUDIPPPOEAddCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData);


/**
@brief �Ƴ�һ���ص�

@param[in] pvUserData �û�������Ϣ
@param[in] fnCallback Ҫ�Ƴ��Ļص�
@return CSUDI_SUCCESS��ʾ�Ƴ��ص��ɹ���������ʾʧ�ܣ�ʧ�����ͼ�������ֵ
@note  �����ں�pfCallback��dwUserData����ͬ��ʱ��Ż��Ƴ�,����������ƥ��ʱ���Ƴ�
*/
CSUDI_Error_Code CSUDIPPPOERemoveCallback(CSUDIPPPOECallback_F fnCallback,void *pvUserData);

#ifdef __cplusplus
}
#endif


#endif







