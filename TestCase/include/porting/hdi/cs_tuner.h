/** @addtogroup CSTUNER
@{
*/
#ifndef _CS_TUNER_H_
#define _CS_TUNER_H_

#include "cs_typedef.h"
#include "udi_tuner.h"

#ifdef __cplusplus
extern "C" 
{
#endif
    
typedef struct _CSUSPSourceUserCallback
{
	CSTUNCallback  m_SourceCallback;
	void * m_pvUserData;
	int m_nSourceHDITunerNo;
} CSUSPSourceUserCallback;

 /**
@brief ���ñ��ؾ�γ��

@param[in] dLocLongitude ���ؾ��ȣ��Ը��������룬����114.5������114.5������Ϊ������-114.5������Ϊ��
@param[in] dLocLatitude ����γ�ȣ��Ը��������룬����22.6������22.6����γΪ������-22.6����γΪ��
@return ���óɹ�����TRUE�� ���򷵻�FALSE
*/
BOOL CSTunerSetPosition(double dLocLongitude, double dLocLatitude);

 /**
@brief ȡ�õ�ǰ�����������ź�Ƶ��

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@param[out] pdwFrequency ���������ȡ���ź�Ƶ��
@return ����ź�������Ӧ�÷���TRUE,����FALSE
*/
BOOL CSTunerGetCurrentFreq( HCSHANDLE hHandle, DWORD *pdwFrequency);

 /**
@brief ��ȡTuner���ź�����

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@return �����ź�������0��100��
*/
int CSTunerGetSignalQuality(HCSHANDLE hHandle);

 /**
@brief ��ȡTuner���ź�ǿ��

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@return �����ź�ǿ�ȣ�0 ��100��
*/
int CSTunerGetSignalStrength(HCSHANDLE hHandle);

 /**
@brief ��ȡTuner�Ĵ�����

@param[in] hHandle ������Ƶ��Tuner�豸���������Դ�����ĵ��ж����CSRMAlloc������ CSRMConnect()�������ء�����ο���Դ�����ӿ�����ĵ���
@return ���ش����ʣ�1*107������Ԫ�д�����Ԫ�ĸ�����(����107�͵õ������ʣ�
*/
int CSTunerGetErrorRate(HCSHANDLE hHandle);

/**
@brief ȡ�������İ汾��

@param[in] pchVer ���ڴ��ȡ�õİ汾��
@param[out] nSize pchVer�Ŀռ䳤�ȣ����ֽ�Ϊ��λ
@return ʵ�ʰ汾�ŵĳ���,����Ϊ��
*/
int CSTunerGetVersion( PCHAR pchVer, int nSize );

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SourceInterface_H */


