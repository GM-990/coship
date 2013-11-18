#ifndef CS_NDSVOD_H
#define CS_NDSVOD_H
#include "mmcp_typedef.h"
#include "udi2_tuner.h"

#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**@brief ��Ƶ�������ֵ*/
typedef enum
{
	CSNDSVOD_MSG_SUCCESS = 1,    ///< ��ȡ�ɹ�
	CSNDSVOD_MSG_TIMEOUT,		 ///< ��ʱ
	CSNDSVOD_MSG_ERROR 			 ///< ʧ��
}CSVODMSGType_E;

/**
@brief ��ȡNDSVOD���
@return �ɹ����ؾ����ʧ���򷵻�-1
*/
CSUDI_HANDLE  CSNDSVODGetHandle();
/**
@brief     ��ȡVOD Baker  Ƶ��Ļص�����
@param[in] hVod  NDS VOD���
@param[in] eType �¼�����  ��CSVODMSGType_E
@param[in] psCableDeliver  Baker  Ƶ����Ϣ ��CSUDITunerCableDeliver_S
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note   CSVODMSGType_E Ϊ CSNDSVOD_MSG_SUCCESS ʱ ��psCableDeliver Ϊ  CSUDITunerCableDeliver_S�ṹָ��
        CSVODMSGType_E Ϊ CSNDSVOD_MSG_TIMEOUT �� CSNDSVOD_MSG_ERRORʱ �� psCableDeliver  ΪNULL.   
*/
typedef int (*CSNDSVODBakerCallback_F)(CSUDI_HANDLE hVod,
	CSVODMSGType_E  eType,
	const CSUDITunerCableDeliver_S * psCableDeliver,
	void * pvUserData);

/**
@brief      ��ȡVOD Service Group �Ļص�����
@param[in] hVod  voip���
@param[in] eType �¼�����  ��CSVODMSGType_E
@param[in] nServiceGroup  VOD Service Group
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return  �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
@note   CSVODMSGType_E Ϊ CSNDSVOD_MSG_SUCCESS ʱ ��nServiceGroup Ϊ VOD Service Group ID.
        CSVODMSGType_E Ϊ CSNDSVOD_MSG_TIMEOUT �� CSNDSVOD_MSG_ERRORʱ ��nServiceGroup Ϊ-1.
*/
typedef int (*CSNDSVODSGCallback_F)(CSUDI_HANDLE hVod,
	CSVODMSGType_E  eType,
	int  nServiceGroup,
	void * pvUserData);
/**
@brief  ����VOD Baker  Ƶ�㣬 �첽�ӿڡ�
@param[in] hVod  voip���
@param[in] fnBakerCallback  �ص�����
@param[in] pvUserData �û��Զ�������
@return �ɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSNDSVODRequestQAMBarker(CSUDI_HANDLE  hVod, CSNDSVODBakerCallback_F fnBakerCallback ,void *  pvUserData);

/**
@brief  ����ServiceGroup �� �첽�ӿڡ�
@param[in] hVod  voip���
@param[in] fnSGCallback  �ص�����
@param[in] pvUserData �û��Զ�������
@return �ɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL  CSNDSVODRequestServiceGroup(CSUDI_HANDLE  hVod, CSNDSVODSGCallback_F fnSGCallback ,void *  pvUserData);
/**
@brief  ��ֹ ���� , ������ֹ����ServiceGroup �� BakerƵ��
@param[in] hVod  voip���
@param[in] fnSGCallback  �ص�����
@param[in] pvUserData �û��Զ�������
@return �ɹ�����SUCCESS��ʧ���򷵻�FALSE
@note ������ֹ����ServiceGroup �� BakerƵ�㣬�������̲���Ҫ���á�
*/
BOOL CSNDSVODCancel (CSUDI_HANDLE  hVod);


#ifdef __cplusplus
}
#endif

#endif


