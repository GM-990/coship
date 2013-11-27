/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        udi2_ota.c
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

#include "udi2_ota.h"

/**
@brief ����wifi����
@param[out] pbWifiUpdate: CSUDI_TRUE:��ʾ��Ҫ����wifi������CSUDI_FALSE:��ʾ����Ҫ����wifi����
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
@note:ֻ�е��ô˽ӿں��м�����ܽ��յ�wifi������ϢCSUDIOTAWIFIMsg_E
@note:ʹ�ó������м����������ô˽ӿڣ�ͨ��pbWifiUpdate�ж�wifi�Ƿ���Ҫ������
     ����Ҫ������������̨���߳�ʵ����wifiģ�齻��,����wifi����������ļ���
     ���ڸ��¹����н����������Ϣͨ��ע��Ļص������׸��м����
*/
CSUDI_Error_Code CSUDIOTAWIFIStart(CSUDI_BOOL *pbWifiUpdate)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ע��wifi�����ص�����
@param[in] fnWifiCallback: �ص�����ָ�룬�����CSUDIOTAWIFICakllback_F
@param[in] pvUserData: �û������Զ���ָ�룬����ΪNULL
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
@note:
*/
CSUDI_Error_Code CSUDIOTAWIFIAddCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ɾ��wifi�����ص�����
@param[in] fnWifiCallback: �ص�����ָ�룬�����CSUDIOTAWIFICakllback_F
@param[in] pvUserData: �û������Զ���ָ�룬����ΪNULL
@return �ɹ�����CSUDI_SUCCESS��ʧ��CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDIOTAWIFIDelCallback(CSUDIOTAWIFICakllback_F fnWifiCallback, void *pvUserData)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief ��ȡOTA��Ϣ�ӿ�

@param[in] eType ��Ϣ���ͣ����CSUDIOTAInfoType_E
@param[out] value ָ��洢��ȡ��Ϣ���ڴ��ַ
@param[in] length valueֵ�ĳ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note MMCP�������Ӫ��ѡ���OTA��ʽ�����ñ��ӿڻ�ȡ�ض����͵�OTA��Ϣ��

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE��Ӧ����������int��
	//valueΪһ��ָ��int��ָ�룬�䳤��Ӧ��Ϊsizeof(int)
	//�Ѷ�Ӧֵ�洢��valueָ���buffer��
	*value = (int)100;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//�Ѷ�Ӧ�����ݴ洢��valueָ���buffer��
	memcpy(value,s_aBCAData,sizeof(s_aBCAData));
}
@endcode
*/
CSUDI_Error_Code CSUDIOTAGetInfo(CSUDIOTAInfoType_E eType, void* value, int length)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief ����OTA��Ϣ�ӿ�

@param[in] eType ��Ϣ���ͣ����CSUDIOTAInfoType_E
@param[in] value ָ��洢��ȡ��Ϣ���ڴ��ַ
@param[in] length valueֵ�ĳ���
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note MMCP�������Ӫ��ѡ���OTA��ʽ�����ñ��ӿ������ض����͵�OTA��Ϣ������Ϣ��ʱЧ���ɾ���ѡ�������

@code
//�ο�ʵ��(δ���ǻ�����������쳣)
//ע��:�������������value��ʾ�����ͼ��������в�ͬ���밴�ο�����ʵ��/ʹ�ü��ɡ�
if(eType == EM_UDIOTA_NOR_MANUFACTURER_CODE)
{
	//EM_UDIOTA_NOR_MANUFACTURER_CODE��Ӧ����������int��
	//valueΪһ��ָ��int��ָ�룬�䳤��Ӧ��Ϊsizeof(int)
	//��valueָ���buffer�л�ȡ��ֵ
	int manu_code = *(int*)value;
}
else if(eType == EM_UDIOTA_NDS_BCA)
{
	//EM_UDIOTA_NDS_BCA��Ӧ����������CSUDI_UINT8����
	//valueһ��ָ��CSUDI_UINT8���飬�䳤��Ϊlength
	//��valueָ���buffer�л�ȡ��Ӧ����
	memcpy(s_aBCAData,value,length);
}
@endcode
*/
CSUDI_Error_Code CSUDIOTASetInfo(CSUDIOTAInfoType_E eType, void* value, int length)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**@brief ����OTA��Ϣ.

��ͨOTAģʽ��ͨ���ýӿ�֪ͨLoader��Ҫ����OTA��������Ҫ��Ϣ�洢��֮��MMCP����ʱ�����նˣ��´�����ʱֱ�ӽ���Loader��
@param[in] pUrl ������Ϣ������������λ�ã���Ƶ��,IP��ַ�ȡ�
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note NDS OTAģʽʱ�ýӿ���Ч��MMCP���������NDS���ݽṹ��FDCA�ȵ��޸ģ����ڱ�Ҫʱ������Booter�����NDS����������ʱ�ж���Щ�ṹ
@note Ŀǰ֧�ֵ�URL�У�
- dvb://networkid.tsid.serviceid����dvb://1000.100.10��
- dvbc://qamfreq.SymbolRate.Modulation����dvbc://363000.6875.64
- http://ipurl/����http://www.coship.com/otafile.bin��
- tftp://ipurl/����tftp://ftp.coship.com/otafile.bin��
- wifi://upgrade?ipurl/, ��wifi://upgrade?http://www.downloadWifi.com/wifi.bin?ver=4.1.0.20&upgradeType=1
���������������ݾ�Ϊ10���ơ�
@note URL���滹���ܸ���һЩ��ѡ��������������dvbc://363000.6875.64?pid=100&tableid=254Ŀǰ��֧�ֵĲ����У�
- pid����ʾ������������pid��10������������pid=100��
- tableid����ʾ������������TableId,10������������tableid=254��
- md5,��ʾ�������ݵ�md5У��ֵ,10���ƣ���md5=2b166501e6b8a40911975c54d3c4297b��
- version,��ʾ�����������汾��10���ƣ���version=100��
- type����ʾ�������ͣ��ַ�������type=auto,type=manual��
@note ������Ҫ֧����Щ���͵�URL�������Ŀ��CA����ȷ����
@note pUrlΪCSUDI_NULL��ʾȡ��������
@note �ýӿڽ���������OTA��Ϣ����Ϻ󲢲�ִ����������������������MMCPӦ����ʽ����
*/
CSUDI_Error_Code CSUDIOTASetOta(const char* pUrl)
{	
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

