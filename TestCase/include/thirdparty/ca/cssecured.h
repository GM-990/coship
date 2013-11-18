/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cssecured.h  
* Author:      heyingbo
* Date:        Nov 2008
* Description:  ST SecuredChipset driver
*              
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
*                           Revision History                                *
*****************************************************************************************************************************
* - Revision 1.0  2008/11/11
*   Programmer:heyingbo
*   Create.
*****************************************************************************************************************************/
#ifndef _CSSECURED_H_
#define _CSSECURED_H_

#include "udiplus_os.h"
#include "udiplus_typedef.h"
#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif

//�߼���ȫ����
typedef enum _SecuredType
{
    SECURED_NACHIPSET_NASECURED,    //�Ǹ߼���ȫоƬ�������߼���ȫ����
    SECURED_CHIPSET_SECURED,        //�߼���ȫоƬ�����߼���ȫ����
    SECURED_CHIPSET_NASECURED   	//�߼���ȫоƬ�������߼���ȫ����    
}SecuredType_EM;

typedef enum _Algorithm
{
    ALGORITHM_TEDS  
}Algorithm_E;

typedef enum _SCWCaType
{
    EM_SCW_UNKNOWN_CA = 0,
    EM_SCW_CONAX_CA,  
    EM_SCW_VIACCESS_CA , 
    EM_SCW_TONGFANG_CA ,
    EM_SCW_DVN_CA , 
    EM_SCW_CRYPTOWORKS_CA , 
    EM_SCW_MEDIAGUARD_CA , 
    EM_SCW_SUMAVISION_CA ,
    EM_SCW_ST_CA,
    EM_SCW_NAGRAVISON_CA , 
    EM_SCW_IRDETO_CA 
}E_SCW_CaType;

/*�߼���ȫ�ӿڵ�IOCtl����*/
typedef enum _SecIOCtlType
{
	EM_SEC_SET_BOOTROM = 0,  /*�򿪰�ȫ����(ǩ��У��).��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_SET_BY_APPʵ��*/
	EM_SEC_GET_BOOTROM,	     /*��ȡ��ȫ������˿״̬.��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_GET_BY_APPʵ��*/
	EM_SEC_SET_JTAGKEY,      /*keyedģʽ��,�ر�JTAG(�رճɹ���,������CA��������������н���).��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_SET_BY_APPʵ��*/
	EM_SEC_GET_JTAGKEY,      /*keyedģʽ��,�ر�JTAG��Ӧ����˿״̬.��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_GET_BY_APPʵ��*/
	EM_SEC_SET_JTAGLOCK,     /*�����Թر�JTAG(�رճɹ���JTAG������).��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_SET_BY_APPʵ��*/
	EM_SEC_GET_JTAGLOCK,     /*��ȡ�����Թر�JTAG��Ӧ����˿״̬.��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_GET_BY_APPʵ��*/
	EM_SEC_SET_SCW,          /*�۶����Ŀ�������˿(��˿�۶Ϻ�ֻ֧�����Ŀ�����).��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_SET_BY_APPʵ��*/
	EM_SEC_GET_SCW,	         /*��ȡ���Ŀ�������˿״̬.��ͬ��CA���ʹ򿪵���˿���ܲ�ͬ,���wiki���CA�����µĸ߼���ȫ.��ָ������˿,�����EM_SEC_GET_BY_APPʵ��*/
	EM_SEC_SET_BY_APP,       /*Ӧ���û��۶�ָ������˿,���÷�ʽ�ο�wiki���CA�����µĸ߼���ȫ*/
	EM_SEC_GET_BY_APP,       /*Ӧ���û���ȡָ����˿��״̬,���÷�ʽ�ο�wiki���CA�����µĸ߼���ȫ*/
	EM_SEC_SET_MARKETID,     /*Mstarƽ̨����MarketID*/
	EM_SEC_GET_MARKETID      /*Mstarƽ̨��ȡMarketID*/
}E_SecIOCtlType;

#define SEC_MAX_DATA_SIZE	32
#define SEC_MAX_CONFIG_NUM	8

typedef struct _SECSetParam
{
	BOOL 	m_bBulkData;					//���ͣ��Ƿ��Ǵ�����ݣ�Ĭ��ΪFALSE
	INT  	m_nItem;						//��˿λ����Ӧ��STSECTOOLFUSE_ITEM_t, ������ֵ����ѯ��ӦCA������
	DWORD 	m_dwLocation;					//��ʼ��ַ��m_bBulkDataΪTRUEʱ��Ч��Ĭ��ΪFALSE
	DWORD	m_dwSize;						//���ݴ�С��m_bBulkDataΪTRUEʱΪ���������С��m_bBulkDataΪFALSEʱ��ֵΪ1
	BYTE	m_aucData[SEC_MAX_DATA_SIZE];	//���ݣ�m_bBulkDataΪFALSEʱ������ֻ��һ������
}SECSetParam_S;

typedef struct _SECConfigParam
{
	INT 			m_nItemCount;           //��Ҫ�۶���˿�ĸ��������ֵΪSEC_MAX_CONFIG_NUM					
	SECSetParam_S	m_stSetParam[SEC_MAX_CONFIG_NUM];
}SECConfigParam_S;

#ifndef SUCCESS
#define SUCCESS (0)
#endif

#ifndef FAILURE
#define FAILURE (-1)
#endif

typedef struct _SECDsmInfo
{
	DWORD 	m_dwDescrambleID;  	   //dsmid
	DWORD   m_dwKeyType     : 1;   //0:evenkey    1:oddkey
	DWORD 	m_dwNotSumCheck : 1;   //�Ƿ���к�У��, Ϊ0ʱ����к�У��, Ϊ1ʱ�����к�У��
	DWORD   m_dwReserved    : 30;  //Ԥ��
}SECDsmInfo_S;


/**************************************************************************
* Function Name: CSCASCWInit
* ��ʼ���߼���ȫ����
* Parameters :      ��
* Return Values:    �ɹ�SUCCESS
*                   ʧ��FAILURE
**************************************************************************/
INT CSCASCWInit(void);

/**************************************************************************
* Function Name: CSCASCWGetSecuredType
* ��ȡоƬ�����Լ�оƬ�߼������Ƿ��
* Parameters :      pemSecuredType:��ȡ������Ϣ
* Return Values:    �ɹ�SUCCESS
*                   ʧ��FAILURE
**************************************************************************/
INT CSCASCWGetSecuredType(SecuredType_EM* pemSecuredType);

/**************************************************************************
* Function Name: CSCAGetChipId
* ��ȡоƬID
* Parameters :      ��
* Return Values:    ����chipidֵ
 *                  ʧ��FAILURE
**************************************************************************/
DWORD CSCASCWGetChipId(void);

/**************************************************************************
* Function Name: CSCAProcessCW
* �����ģ����Ŀ����ִ���
* ������Ҫ���ݲ�����ַdwPara������SEDsmInfo_S��������
* 1 ����STƽ̨�Ĵ���:-->����Ƕ����Ŀ����ִ���, �ɹ�����0, ʧ�ܷ���FAILURE
                        ���ݽ�������SEDsmInfo_S�ṹ�е�m_dwNotSumCheck�ж��Ƿ��У�飬Ȼ�󿽱������
					 -->����Ƕ����Ŀ����ִ����ɹ�����0, ʧ�ܷ���FAILURE
*                       ���Ŀ�������Ҫ���������CW(�Ĵ���index)
* 2 ����ALI��MSTARƽ̨:-->����Ƕ����Ŀ����ִ���, �ɹ�����0, ʧ�ܷ���FAILURE
*                         ���ݽ�������SEDsmInfo_S�ṹ�е�m_dwNotSumCheck�ж��Ƿ��У�飬Ȼ�󿽱������
*                      -->����Ƕ����Ŀ����ִ����ɹ�����1, ʧ�ܷ���FAILURE
                          ��Ҫ�ڴ˽ӿ��Ƚ������Ŀ����֣�Ȼ��ֱ�����õ��������У�dsmid��keytype���ڽṹ��SEDsmInfo_S������
                          ��ʱpcOutputKey������Ч
*Parameters : 		pcInputKey :�����CW
*           		cLength : key����
*           		pcOutputKey : ������CW
*					bIsClearCW : �Ƿ�Ϊ���Ŀ����֣�1:���ģ�0:����
*           		dwPara : SEDsmInfo_S�ṹ���ַ
*Return Values :  	�ɹ�:0:��Ҫ�����ô�����CW
*                        1:����Ҫ�����ô�����CW   
*              		ʧ��:FAILURE
**************************************************************************/
INT CSCASCWProcessCW(BYTE *pcInputKey, BYTE cLength, BYTE *pcOutputKey, BOOL bIsClearCW, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASCWLoadCWPK
* ��CWPK���ص�оƬ�ڲ�
*Parameters :  		pcKey :�����key
*              		cLength: key�ĳ���
*              		dwPara : ��������, Ԥ���ӿ�
*Return Values :  	�ɹ�SUCCESS
*              		ʧ��FAILURE
**************************************************************************/
INT CSCASCWLoadCWPK(BYTE *pcKey, BYTE cLength, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASCWEncryptData
* ��������
*Parameters :  		pucCPCW :�������ݲ��õ�key������Ϊ16�ֽ�,��ΪNULL,��Ĭ�ϲ��ø�CA�߼���ȫоƬ����SCK��Ϊ��Կ���м���
                	bIsNeedCBCVector :�Ƿ���ҪCBC Vector
                	pucVector :CBC Vector��ַ
                	nVentorLen:CBC Vector���ȣ�ST7111ƽ̨֧�ֵĳ������Ϊ4
                	pucSrcBuf :���������ݵ�ַ
                	pucDesBuf :���ܺ����ݵ�ַ
                	dwLen :����(��)���ݳ���
                	emType : ��������ѡ�õ��㷨��ĿǰST7111ƽ̨֧���㷨ΪTDES
*Return Values :  	�ɹ�SUCCESS
*              		ʧ��FAILURE
**************************************************************************/
INT CSCASCWEncryptData(BYTE* pucCPCW, BOOL bIsCBC,BOOL bIsNeedCBCVector,INT nVentorLen,BYTE* pucVector,
						BYTE* pucSrcBuf, INT nLen,Algorithm_E emType, BYTE* pucDesBuf);

/**************************************************************************
* Function Name: CSCASCWDecryptData
* ��������
*Parameters :  		pucCPCW :�������ݲ��õ�key������Ϊ16�ֽ�,��ΪNULL,��Ĭ�ϲ��ø�CA�߼���ȫоƬ����SCK��Ϊ��Կ���н���
                	bIsNeedCBCVector :�Ƿ���ҪCBC Vector,����ΪTRUE��ΪCBC��FALSEΪECB
                	pucVector :CBC Vector��ַ
                	nVentorLen:CBC Vector���ȣ�ST7111ƽ̨֧�ֵĳ������Ϊ4
                	pucSrcBuf :���������ݵ�ַ
                	pucDesBuf :���ܺ����ݵ�ַ
                	dwLen :����(��)���ݳ���
                	emType : ��������ѡ�õ��㷨��ĿǰST7111ƽ̨֧���㷨ΪTDES
*Return Values :  	�ɹ�SUCCESS
*              		ʧ��FAILURE
**************************************************************************/
INT CSCASCWDecryptData(BYTE* pucCPCW,BOOL bIsCBC, BOOL bIsNeedCBCVentor,INT nVentorLen,BYTE* pucVentor,
						BYTE* pucSrcBuf, INT nLen,Algorithm_E emType, BYTE* pucDesBuf);

/**************************************************************************
* Function Name: CSCASCWSetCAType
* ����CA����
*Parameters :  		E_SCW_CaType : CAö������
               		dwPara :Ԥ������
*Return Values :  	��
***************************************************************************/
void CSCASCWSetCAType(E_SCW_CaType emType, DWORD dwPara);

/**************************************************************************
* Function Name: CSCASecureIoctl
* ���úͻ�ȡ�߼���ȫ�ӿ�BOOTROM,JTAGKEY,JTAGLOCK,SCW;
* Parameters :      nDeviceIndex ʹ���豸��index, ��0��ʼ, Ŀǰ�贫0;
* Parameters :      emSecIOCtlType ʹ�øýӿ�ʱ���봫������������;
* Parameters :      pParams ���ݴ����E_SecIOCtlType �������û��߻�ȡ�Ĳ���ָ��,
	         emSecIOCtlTypeΪEM_SEC_SET_BOOTROM��EM_SEC_SET_JTAGKEY��
	                EM_SEC_SET_JTAGLOCK��EM_SEC_SET_SCWʱ, �ò�������ΪNULL��
	         emSecIOCtlTypeΪEM_SEC_SET_BY_APP��EM_SEC_GET_BY_APPʱ, �ò���Ϊ
	         		SECConfigParam_S�ṹ������ĵ�ַ��
	         emSecIOCtlTypeΪ����ֵʱ, �ò���Ϊunsigned int�����ĵ�ַ��
* Return Values:   �����ɹ�SUCCESS
*                  ����ʧ��FAILURE
��ע:emSecIOCtlTypeΪEM_SEC_SET_BY_APPʱ�����۶���˿���ɹ�����ҪӦ�õ���EM_SEC_GET_BY_APP
     �ж���˿״̬������˿û���۶ϣ����ٴν����۶ϡ�
**************************************************************************/
INT CSCASecureIoctl(INT nDeviceIndex, E_SecIOCtlType emSecIOCtlType, unsigned int * pParams);

/**************************************************************************
* Function Name: CSCASecureGetSMICVCValue
* Mstarƽ̨SMIоƬ��ȡCVCֵ(CVC valueֵ���Ȳ�С��16BYTE);
* Parameters :      pucData Ӧ�÷����������CVC value���ڴ��ַ;
* Parameters :      dwLen Ӧ�÷����������CVC value���ڴ��С;

* Return Values:   �����ɹ�SUCCESS
*                  ����ʧ��FAILURE
��ע:Mstarƽ̨ר�нӿ�
**************************************************************************/
INT CSCASecureGetSMICVCValue(BYTE *pucData, DWORD dwLen);
//���½ӿڷ�������������ʹ��
void CSCASCWWriteFuse(void);
void CSCASetCsdForNagra(BOOL bFlag);

#ifdef  __cplusplus
}
#endif

#endif 

