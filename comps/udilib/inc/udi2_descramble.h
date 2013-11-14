/**@defgroup DESCRAMBLE DESCRAMBLE ���ݽ�����ؽӿڶ���
@brief DESCRAMBLEģ���Ӧͷ�ļ�<udi2_descramble.h>��������������صĽӿڡ�

���н����������ӿڣ���SetPID����ʵ����Ҫ������ȴ����ͨ�����������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ���ǿ��Խ��ܵģ�MMCP��CloudTV ���м��ƽ̨��ʹ���лᱣ֤�ȴ򿪸�PIDͨ����Ȼ��Ը�ͨ�����н�����ز�����

@note ��ΪNDS CAʹ��ICAMģ��������������������ΪNDS��Ʒ��������ѭNDS�淶ʹ��оƬ�����ṩ��ICAMģ�飬������ʵ�ֱ�ģ�� ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.1 2009/8/17 
@{
*/

#ifndef _UDI2_DESCRAMBLE_H_
#define _UDI2_DESCRAMBLE_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif 

/**@brief ������ش������ֵ*/
enum
{
	CSUDIDSM_ERROR_BAD_PARAMETER = CSUDI_DESCRAMBLE_ERROR_BASE,		///< �������
	CSUDIDSM_ERROR_INVALID_HANDLE,						///< �Ƿ����
	CSUDIDSM_ERROR_INVALID_DEVICE_ID,					///< �Ƿ��豸ID
	CSUDIDSM_ERROR_INVALID_STATUS,						///< �Ƿ�״̬
	CSUDIDSM_ERROR_FEATURE_NOT_SUPPORTED,				///< ��֧�ִ˲���
	CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER,					///< û�н���ͨ��
	CSUDIDSM_ERROR_NO_MEMORY							///< û�п����ڴ�
};

/**
@brief ����һ������ͨ�������ؽ���ͨ�����

@param[in] nDemuxIndex  demux�豸������
@param[out] phDSM ���ڽ��շ��ؽ���ͨ�����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ����pHandle������δ����
@note ����Ѿ����������н���ͨ�����˽ӿڷ���CSUDIDSM_ERROR_NO_FREE_DESCRAMBLER��
@note Eastwin�ὫCSUDI_NULL����һ���Ƿ��ľ��������ʵ��ʱ�ر�ע������Handle��ȡֵ��Χ
*/
CSUDI_Error_Code CSUDIDSMAllocate(int nDemuxIndex, CSUDI_HANDLE * phDSM);

/**
@brief �ͷ�һ������ͨ��

@param[in] hDSM ����ͨ���ľ������CSUDIDSMAllocate���صõ�
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/ 
CSUDI_Error_Code CSUDIDSMFree(CSUDI_HANDLE hDSM );

/**
@brief Ϊ���ŵ�ͨ��ָ�����˵�PID

@param[in] hDSM ����ͨ�����
@param[in] ushPID ����PID
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ���ý���PIDǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵģ�ʹ��������Ҫע���˳������
*/ 
CSUDI_Error_Code CSUDIDSMSetPID(CSUDI_HANDLE hDSM,  unsigned short ushPID );

/**
@brief ��ս���ͨ��ָ���Ĺ���PID

@param[in] hDSM ����ͨ�����
@param[in] ushPID ����PID
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/ 
CSUDI_Error_Code CSUDIDSMClearPID(CSUDI_HANDLE hDSM, unsigned short ushPID );

/**
@brief ���ý���ͨ������Keyֵ

@param[in] hDSM ����ͨ�����
@param[in] pucOddKey ���ŵ���KEY
@param[in] ucOddLength Key����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
CSUDI_Error_Code CSUDIDSMSetOddKey(CSUDI_HANDLE hDSM, const unsigned char * pucOddKey, unsigned char ucOddLength);

/**
@brief ���ý���ͨ����żKeyֵ

@param[in] hDSM ����ͨ�����
@param[in] pucEvenKey ���ŵ�żKEY
@param[in] ucEvenLength Key����
@return �ɹ���ȡ�򷵻�CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
@note ��keyǰ��Ҫ���ȴ򿪸�PIDͨ��(�������PID�µ����ݡ����Ÿ�PID�µĽ�Ŀ)�ǿ��Խ��ܵ�
*/ 
CSUDI_Error_Code CSUDIDSMSetEvenKey(CSUDI_HANDLE hDSM, const unsigned char * pucEvenKey, unsigned char ucEvenLength);



#ifdef  __cplusplus
}
#endif
/** @} */
#endif
