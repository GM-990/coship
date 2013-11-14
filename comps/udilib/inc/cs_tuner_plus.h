/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

/**@defgroup TUNER TUNER ������ͳһ��tuner��ʼ����ؽӿڲ�������
@brief ��ģ����Ҫ������ͳһ��tuner��ʼ����ؽӿڲ�������
@brief ͳһ��tuner�ӿ�֧��DVB-C��׼(QAM���Ʒ�ʽ),DVB-S/DVB-S2��׼(QPSK,8PSK���Ʒ�ʽ),DVB-T��׼(COFDM��DMB-TH���Ʒ�ʽ)
@{
*/

#ifndef _UDI2_TUNER_PLUS_H_
#define _UDI2_TUNER_PLUS_H_

#ifdef __cplusplus
extern "C" 
{
#endif
	
#include "udi2_typedef.h"
#include "udi2_error.h"
#include "cs_hdicommon_plus.h"



typedef enum
{
    EM_UDITUNER_NAME_NONE,                      ///<�Զ�ʶ��tuner
    ///< ����tuner����
    EM_UDITUNER_CAB_CU1216_TDA10021,     ///< CU1216+TDA10021
    EM_UDITUNER_CAB_CU1216_TDA10023,     ///< CU1216+TDA10023

    ///<����tuner����
    EM_UDITUNER_TER_TD1611_MT353,         ///< TD1611+MT353
    EM_UDITUNER_TER_TD1611_MT352,         ///< TD1611+MT352

    /// ����tuner����
    EM_UDITUNER_SAT_0165
}CSUDITunerName_E;



typedef struct 
{
	unsigned int m_uI2CIndex;                             ///< I2C����
	unsigned int m_uResetGPIONum;                   ///< demodulator reset gpio ����
	CSHDITsMode m_eTSOutMode;                       ///< TS�����ģʽ
	int               m_nPriority;                                  ///< tunerģ���߳����ȼ�
	CSUDITunerName_E m_eTunerName;                  ///< tuner�ͺ�
}CSUDITunerInitParam_S;



 /**
@brief Tunerģ���ʼ��

@param[in] uTunerId tuner�豸��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerInit(unsigned int uTunerId, CSUDITunerInitParam_S *psTunerInitParam);


 /**
@brief RDI Tuner��� 

@param[in] nPriority ����̵߳����ȼ�  
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSRDITunerDetect(int nPriority);



 /**
@brief Tunerģ�鷴��ʼ��

@param[in] uTunerId tuner�豸��
@return 
	- �ɹ����ظ���ʼ������CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
	- �������󷵻�CSUDITUNER_ERROR_BAD_PARAMETER
*/
CSUDI_Error_Code CSUDITunerTerm(unsigned int uTunerId);


 /**
@brief ע��QAMģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQam(void);


 /**
@brief ע��COFDMģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterCofdm(void);


 /**
@brief ע��QPSKģ����غ���

@param[in] ��
@param[in] psTunerInitParam Tuner��ʼ����ز�������
@return 
	- �ɹ�����CSUDI_SUCCESS,ʧ�ܷ���CSUDI_FAILURE
*/
CSUDI_Error_Code CSUDITunerRegisterQpsk(void);


#ifdef __cplusplus
}
#endif

/** @} */

#endif
