/**@defgroup DISEQC DISEQC ������ͳһ��diseqc udi2.0�ӿڲ�������
@brief DiSEqCģ���Ӧͷ�ļ�<udi2_diseqc.h>��������DiSEqC�ĳ��ò����ӿڡ�
֧��diseqc1.0/1.1/1.2/1.3��׼

@version 1.1.0 2012/1/5 ���ӽӿ�CSUDIDSC12GotoSTABPosition��CSUDIDSC12GetAntennaAngle
@version 1.0.1 2011/12/28 ��һ�汾
@{
*/

#ifndef _UDI2_DISEQC_H_
#define _UDI2_DISEQC_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum 
{
	EM_UDIDISEQC_EASTERN,	/**< ��ת��*/
	EM_UDIDISEQC_WESTERN	/**< ����ת��*/
}CSUDIRotateDirection_E;

typedef struct 
{
	double ldSatLongitude; /**< ���Ǿ���*/
	double ldLongitude;    /**< ָ������*/
	double ldLatitude;     /**< ָ��γ��*/
	int    reserved[2];	   /**< ��������*/
}CSUDISatLocationInfo_S;

/**
@brief ��ָ������ת��ָ����ʱ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] eDirection ����ת������
@param[in] ucTime (s) ����ת��ʱ��(����)
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12RotateTimes(CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection, CSUDI_UINT8 ucTime);

/**
@brief ��ָ������ת��ָ���Ļ���

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] eDirection ָ������
@param[in] ucCycles ת������
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12RotateCycles (CSUDI_UINT32 dwTunerId, CSUDIRotateDirection_E eDirection,CSUDI_UINT8 ucCycles);

/**
@brief ����ǰλ�ñ���Ϊ��XX������λ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucPosition ��ǰλ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12StorePosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief ת�����ǵĵ�XX��λ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucPosition ��Ҫת������λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12GotoPosition (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief ת���̶�Ϊ0��λ

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12ResetPosition (CSUDI_UINT32 dwTunerId);

/**
@brief ���ת��ֱ�����(������)����λ�û���յ�STOP�����ֹͣת��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] ucDirection �޶�λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12RotateWithoutStop (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection);

/**
@brief  ֹͣ���ת��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12RotateStop (CSUDI_UINT32 dwTunerId);

/**
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12DisableLimit (CSUDI_UINT32 dwTunerId);

/**
@brief  ȡ��ת����Χ������(��ߺ�������)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] bDirection ���õ�ǰλ��Ϊ���ת�������/����������
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12SetLimit (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucDirection);

/**
@brief ���ݵ�XX�����ǵ�ƫ��ͬ���������ǵ�λ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] bPosition ��ǰ����ƫ��λ��
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12AdjustTAB (CSUDI_UINT32 dwTunerId, CSUDI_UINT8 ucPosition);

/**
@brief ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] dlAngle ���߷�λ��(>0 East, <0 West)
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12GotoSTABPosition(CSUDI_UINT32 dwTunerId, double dlAngle);

/**
@brief  ת����ָ���Ƕ�(For STAB USALS Motor)

@param[in]  dlSatLongitude ���Ǿ���
@param[in]  dlMyLongitude  ���ؾ���
@param[in]  dlMyLatitude   ����γ��
@param[out] pdlAngle       �洢����ת���Ƕ�
@return CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12GetAntennaAngle(double dlSatLongitude, double dlMyLongitude, double dlMyLatitude, double *pdlAngle);

/**
@brief  ������ﵽָ���ľ�γ��

@param[in] dwTunerId ������Ƶ��Tuner�豸���豸�š�
@param[in] psSatLocationInfo,�������ָ���ľ�γ����Ϣ
@return  CSUDI_SUCCESS���ɹ���ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIDSC12TurnSateWithUsals(CSUDI_UINT32 dwTunerId, const CSUDISatLocationInfo_S *psSatLocationInfo);

#ifdef __cplusplus
}
#endif

#endif

