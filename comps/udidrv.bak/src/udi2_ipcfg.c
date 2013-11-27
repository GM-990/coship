/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_ipcfg.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project.
		 Because there is no EEPROM device in the STB system.
 *-------------------------------------------------------------------------------
 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "udi2_ipcfg.h"
 /**
@brief ע�����������Ļص�����

@param[in] fnEtherCallback �ص�����ָ�룬���CSUDIEtherCallback_F����
@param[in] pvUserData �û�������Զ���ָ�룬����֪ͨ�������ظ��û���������Ϣ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �ص�����������û�����Ψһȷ��һ���ص�������������ע��������ȫһ��ʱ���ڶ��ν�����CSUDIIPCFG_ERROR_ALREADY_ADDED
@note ����֧��ע��Ļص���������Ϊ32���������
*/
CSUDI_Error_Code CSUDIIPCFGAddEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


 /**
@brief ɾ�����������Ļص�����

@param[in] fnEtherCallback �ص�����ָ�룬���CSUDIEtherCallback_F����
@param[in] pvUserData ͬCSUDIIPCFGAddEtherCallback��pvUserData
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGDelEtherCallback(CSUDIEtherCallback_F fnEtherCallback,void * pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �����������֧�����ߺ�����������pppoe���������pppoe���ܣ�����Ӧ����ʵ�ʵ�����������ppp0
@param[out] pnCount ��ȡ����������Ŀ
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/ 
CSUDI_Error_Code CSUDIIPCFGGetEtherCount(int* pnCount)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡ�����豸����֧�����ߺ�����������pppoe��

@param[in] nEthIndex �����豸�����ţ��� 0��1 ������
@param[in] nLen pcEthName��ָ�ռ�ĳ��ȣ��������0С�ڵ���16
@param[out] pcEthName ��ȡ���������豸�������û�����ռ䣬���ȱ������0С�ڵ���16
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherName(int nEthIndex,char * pcEthName,int nLen )
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �������оƬ����Ϣ�����������̺����͵ȡ�

@param[in] pcEthName ������������:eth0
@param[out] psChipInfo ��CSUDIEtherInfo_S
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherChipInfo(char * pcEthName, CSUDIEtherInfo_S * psChipInfo)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ͨ���豸��������ȡ���������ַ��֧�����ߺ�����������

@param[in] pcEthName ������������:eth0
@param[out] pucMacAddress ָ��һ��MAC��ַ���ַ�������, ��ʽΪ{0x00, 0x0E, 0xDB, 0x11, 0x22, 0x33}��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note 1.udi2û���ƶ�����mac��ַ�Ľӿڣ���Ϊ����mac��ַ��Ӧ����udi2���ĵ����飬��Ӧ����ϵͳ������ʱ��ִ�еĶ�����
@note ���仰˵��mac��ַ�ڵ��øýӿ�֮ǰ���Ѿ�ȷ���������ȡ��mac��ַ��Ҳ������ϵͳ����ʱ���ý��ı���һ�¡�
*/
CSUDI_Error_Code CSUDIIPCFGGetMacByName(char * pcEthName,unsigned char * pucMacAddress)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡ���ߺ���������������״̬��

@param[in] pcEthName ָ��һ�������������ַ�������"eth0"
@param[out] pushLinkState ����״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӣ�
@note 2��pushLinkState & CSUDI_ETHMASK_100BASE_10BASE:Ϊ00����100Mbps��Ϊ01����10Mbps��Ϊ10��������54Mbps��Ϊ11����δ֪���ʣ�
@note 3��pushLinkState & CSUDI_ETHMASK_FULL_HALF: Ϊ0����ȫ˫����Ϊ1�����˫����
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӡ�
*/
CSUDI_Error_Code CSUDIIPCFGGetEtherStatus(char * pcEthName, unsigned short * pushLinkState)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ���������ڴ��ڵ�����AP����Ϣ��ֻ��������������

@param[in] pcEthName ��������������
@param[in] psWlanApInfo �洢AP��Ϣ�Ľṹ������
@param[in] pnWlanApInfoNum psWlanApInfo�ṹ������ĸ���
@param[out] psWlanApInfo �洢ʵ�ʻ�ȡ����AP��Ϣ�Ľṹ������
@param[out] pnWlanApInfoNum ʵ�ʻ�ȡ����AP��Ϣ����Ч���� 	
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSearchAP(char * pcEthName,CSUDIWLanApInfo_S * psWlanApInfo, int * pnWlanApInfoNum)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��AP�������Ӳ��ҽ�����֤��ֻ��������������

@param[in] psWLanConf �������������ò������ݽṹ�������������������ƣ���֤��ESSID�ַ�������֤����Կ���ַ����ͳ��ȣ���֤�����ͣ���Կ����������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGConnectAP(CSUDIWLanConfig_S * psWLanConf)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief �Ͽ���AP�����ӣ�ֻ��������������

@param[in] pcEthName ��������������
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGDisconnectAP(char * pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡAP�źŵ�������ǿ�ȡ�

@param[out] pnWlanApSigQulity AP�źŵ�����
@param[out] pnWlanApSigStrength AP�źŵ�ǿ��
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note �˽ӿڽ���������������ȡ��Ϣ��Ӧ����CSUDIIPCFGSearchAP ��ʱ�򱣴���Ϣ
*/
CSUDI_Error_Code CSUDIIPCFGGetAPSignalQulityStrength(int * pnWlanApSigQulity,int * pnWlanApSigStrength)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ����DNS�ĵ�ַ��

@param[in] nDNSIndex DNS�������ţ��� 0��1 �����֧��4��
@param[in] pcDNS DNS��IP��ַ����"192.168.99.5"�ĵ��ʮ����IP��ַ)
@param[in] nLen DNS��IP��ַ�ַ������ȣ���С��16 
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSetDNSServer(int nDNSIndex,char * pcDNS,int nLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**  
@brief ���DNS�ĵ�ַ��

@param[in] nDNSIndex DNS�������ţ��� 0��1�����֧��4��
@param[in] nLen DNS��IP��ַ�ַ������ȣ���С��16 
@param[out] pcDNS DNS��IP��ַ(��"192.168.99.5"�ĵ��ʮ����IP��ַ����pcDNS�Ŀռ����û�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetDNSServer(int nDNSIndex,char * pcDNS,int nLen)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����������IP��ַ��������������ص�ַ��֧�����ߺ�����������

@param[in] pcEthName �����豸������:"eth0"����ΪCSUDI_NULL���򷵻ش���
@param[in] pcIp IP��ַ�ַ�������"192.168.1.1" ����ΪCSUDI_NULL���򷵻ش���    
@param[in] pcMask ���������ַ�������"255.255.255.0"������ΪCSUDI_NULL
@param[in] pcGateWay ���ص�ַ�ַ�������"192.168.1.254"������ΪCSUDI_NULL
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGSetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡ������IP��ַ��������������ص�ַ��֧�����ߺ�����������

@param[in] pcEthName �����豸��
@param[out] pcIp ��ȡ����IP��ַ�����ȱ��벻С��16
@param[out] pcMask ��ȡ����mask��ַ�����ȱ��벻С��16
@param[out] pcGateWay ��ȡ����gateway��ַ�����ȱ��벻С��16
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note: 
         ������������ֻҪ��һ����Ϊ�գ��ͻ�ȡ��Ӧ��ֵ���ɹ�����CSUDI_SUCCESS 
         ���pcIp==pcMask==pcGateWay==CSUDI_NULL���᷵��CSUDIIPCFG_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDIIPCFGGetIpInfo(char * pcEthName,char * pcIp,char * pcMask,char * pcGateWay)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ��

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[in] pcDhcpBuf  Ӧ�ò�����/��̫��������
@param[in] nBufLen buffer�ĳ���
@param[in] nFlag �Ƿ���Ҫ�˺������udp/ip/ethernetͷ nFlag =1,��Ҫ��ӣ�nFlag =0,ֱ�ӷ���
@return �ɹ����� 0,ʧ�ܷ��� -1
*/
int  CSUDIRawSendPacket(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��Ӧ�ò㴫�����İ��������������������㷢�ͳ�ȥ,�����û����õĲ���eRawSendType�������͵İ��ǵ��������ǹ㲥����

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[in] pcDhcpBuf  Ӧ�ò�����/��̫��������
@param[in] nBufLen buffer�ĳ���
@param[in] nFlag �Ƿ���Ҫ�˺������udp/ip/ethernetͷ nFlag =1,��Ҫ��ӣ�nFlag =0,ֱ�ӷ���
@param[in] eRawSendType ���巢�Ͱ�ʱ�ǵ��������ǹ㲥
@return �ɹ����� 0,ʧ�ܷ��� -1
*/
int  CSUDIRawSendPacketByMode(char *pszEthName, int  *fd, const char *pcDhcpBuf, int nBufLen, int nFlag, CSUDIRawSendType_E eRawSendMode)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief ���մ����������������İ�������Ӧ�ò�

@param[in] pszEthName �����豸����
@param[in] fd socket��������(*fd==-1��ʱ���ڴ˺����ڴ���socket,������sosket�޹�)
@param[out] pchBuf  ��Ҫ���յ�����
@param[in,out] pnBufLen buffer�ĳ���.�������ʱΪpchBuf�Ļ��������ȣ��������ΪpchBuf����Ч���ݳ���
@param[in] nFlag  �Ƿ���Ҫ�˺���ȥ��udp/ip/ethernetͷ  nFlag =1,��Ҫȥ����nFlag =0,ֱ�ӽ�����һ��
@return �ɹ����� ʵ���յ����ֽ���,ʧ�ܷ��� -1
*/
int CSUDIRawRecvPacket (char *pszEthName, int  *fd, const char *pchBuf, int* pnBufLen, int nFlag)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��ȡ����ͳ����Ϣ
@param[in] pcName �����豸����
@param[out] psNetStatus �洢ʵ�ʻ�ȡ��������ͳ����Ϣ�Ľṹ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGGetStatInfo(char *pcName,CSUDIEtherStat_S *psNetStatus)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��������
@param[in] pcEthName �����豸����
@param[out] ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGNICUp(char *pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}


/**
@brief ��������
@param[in] pcEthName �����豸����
@param[out] ��
@return �ɹ�����CSUDI_SUCCESS ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIIPCFGNICDown(char *pcEthName)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/*-----------------���½ӿڶ���ɲ����������ؽӿ�---------------------*/
/**
@brief ��ӿɲ������֪ͨ�ص�����

���RDIģ��CSUDIRDIAddCallback_F�ӿڶ��壬�����ڴ������������͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,���豸״̬�����仯ʱ���øú���
@param[in] pvUserData �û�����,����Ϊ�ص�������pvUserData��������
@return �ɹ���ӷ���CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ.
@note ��ƽ̨����Ҫ֧���κοɲ���������ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
@note ��CSUDIRDICallback_F�ص��У��ɲ�������豸��EM_UDIRDI_EVENT_READY��Ϣ�У�pEventDataΪethname (���������ַ���)�������ڱ�ͷ�ļ����涨���ʹ���������Ľӿ�
*/
CSUDI_Error_Code CSUDIIPCFGRdiAddCallback(CSUDIRDICallback_F fnCallback, void* pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ɾ���ɲ������֪ͨ�ص�����

���RDIģ��CSUDIRDIRemoveCallback_F�ӿڶ��壬�����ڴ������������͵Ŀ��ƶ��豸
@param[in] fnCallback �ص�����,֮ǰע��ĺ���
@param[in] pvUserData �û�����,������ע��ʱ��һ��
@return �ɹ�ɾ������CSUDI_SUCCESS,ʧ�ܷ��ش������ֵ
@note fnCallback��pUserDataһ��Ψһȷ��һ���ص�����
@note ��ƽ̨����Ҫ֧���κοɲ���������ýӿڷ���CSUDITUNER_ERROR_FEATURE_NOT_SUPPORTED
*/
CSUDI_Error_Code CSUDIIPCFGRdiRemoveCallback(CSUDIRDICallback_F fnCallback, void* pvUserData)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

