/**@defgroup CSDISEQC [CSDISEQC]ͬ����Ŀר��DISEQC�ӿ�
@brief ��ģ����Ҫ������ͬ����Ŀר��ƽ̨�޹�DISEQC�����ӿ�
@brief  DiSEqCӢ��ΪDigital Satellite Equipment Control��ֱ��Ϊ�����������豸���ơ��������������ǵ��ӽ��ջ����ƣ�����ָ�������ָ�����Ӧ�豸�����л����ء��л��������������豸��LNB�ȡ� DiSEqC ��ʵ����һ������Э�飬������ĳ��Ӳ������Ȼ��һ��Э����в�ͬ�İ汾������udi��MMCP�м�㲻���õ����ģ�飬�����ھ���Ļ���MMCP����Ŀ�������ļ���Ӧ�û��õ���
 
@version 0.3 2008/11/5  ��һ��ϸ���汾
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _CS_DISEQC_H_
#define _CS_DISEQC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_typedef.h"
#include "CSHDI_typedef.h"

/**
@brief ��ָ������ת��ָ����ʱ��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@return CSQPSK_SUCCESS���ϵ�ɹ�;CSQPSK_INVALID_PARAM����������

*/
CSQPSK_Error_t CSDSC10PowerOn ( CSHDITunerIndex bTunerIndex );

/**
@brief ��ָ������ת��ָ����ʱ��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bDirection ָ������
@param[in] bTime ��Ҫ���˵��
@return CSQPSK_SUCCESS���ϵ�ɹ���CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12RotateTimes(CSHDITunerIndex bTunerIndex, BYTE bDirection,BYTE bTime);

/**
@brief ��ָ������ת��ָ���Ļ���

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bDirection ָ������
@param[in] bCycles ת������
@return CSQPSK_SUCCESS���ϵ�ɹ���CSQPSK_FAILURE��ת��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12RotateCycles (CSHDITunerIndex bTunerIndex, BYTE bDirection,BYTE bCycles );

/**
@brief ����ǰλ�ñ���Ϊ��XX������λ��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bPosition ��ǰλ��
@return CSQPSK_SUCCESS���ϵ�ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12StorePosition (CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief ת�����ǵĵ�XX��λ

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bPosition ��Ҫת������λ��
@return CSQPSK_SUCCESS���ϵ�ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12GotoPosition ( CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief ת���̶�Ϊ0��λ

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12ResetPosition ( CSHDITunerIndex bTunerIndex );

/**
@brief ���ת��ֱ�����(������)����λ�û���յ�STOP�����ֹͣת��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bDirection �޶�λ��
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12RotateWithoutStop ( CSHDITunerIndex bTunerIndex, BYTE bDirection );

/**
@brief  ֹͣ���ת��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12RotateStop (CSHDITunerIndex bTunerIndex);

/**
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12DisableLimit (CSHDITunerIndex bTunerIndex);

/**
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bDirection ���õ�ǰλ��Ϊ���ת�������/����������
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12SetLimit ( CSHDITunerIndex bTunerIndex, BYTE bDirection );

/**
@brief ���ݵ�XX�����ǵ�ƫ��ͬ���������ǵ�λ��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] bPosition ��ǰ����ƫ��λ��
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12AdjustTAB ( CSHDITunerIndex bTunerIndex, BYTE bPosition );

/**
@brief ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] nAngle_10Multiply ���߷�λ��(*10>0 East, <0 West)
@return CSQPSK_SUCCESS���ɹ���CSQPSK_FAILURE��ʧ��;CSQPSK_INVALID_PARAM����������
*/
extern CSQPSK_Error_t CSDSC12GotoSTABPosition  ( CSHDITunerIndex bTunerIndex, int nAngle_10Multiply );

/**
@brief  ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in] sat_longitude ���Ǿ���
@param[in] my_longitude ���ؾ���
@param[in] my_latitude ����γ��
@return ���߽Ƕ�
*/
double CSDSC12GetAntennaAngle(double sat_longitude, double my_longitude, double my_latitude);

/**
@brief  ������ﵽָ���ľ�γ��

@param[in] bTunerIndex tuner��������Tuner_0 ����0��tuner��param[in] Tuner_1  ����1��tuner��
@param[in] nSateLongtiude ��Ҫ���˵��
@param[in] nBaseLongitude ��Ҫ���˵��
@param[in] nLatitude ��Ҫ���˵��
@return  ��Ҫ���˵��
*/
CSQPSK_Error_t CSDSC12TurnSateWithUsals( CSHDITunerIndex bTunerIndex, double nSateLongtiude, double nBaseLongitude, double nLatitude);

#ifdef __cplusplus
}
#endif
/** @} */
#endif


