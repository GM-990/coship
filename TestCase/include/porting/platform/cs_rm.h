/** @addtogroup RM
@{
*/
#ifndef _CS_RM_H_
#define _CS_RM_H_

#include "cs_typedef.h"
#include "udi_rm.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
@brief ����ָ��Demux��������Ϣ

����ָ��Demux��һЩ���ԣ����ڳ�ʼ��ʱ����\n
Ϊ��֤��ͬƽ̨�Ĳ�ͬӲ��(�����в�ͬ���豸����������)ʹ��ͬ������������ṩ�����ýӿ�
@param[in] nDmxIndex ������demux�豸����
@param[out] psDemuxInfo Demux������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note CSRMGetDeviceCounts���ص�Demux�����Ǹ��ݸýӿڻ�ȡ��
@note ����ʱ���뱣֤������0��ʼ���ϲ�ģ���ͨ������0--Demux����-1��������ȡ����Demux�豸����Ϣ
*/
BOOL CSRMSetDemuxConfig(int nDmxIndex,CSRMDemuxConfig_S* psDemuxInfo);

/**
@brief ����ָ��Tuner��������Ϣ

����ָ��Tuner��һЩ���ԣ����ڳ�ʼ��ʱ����\n
Ϊ��֤��ͬƽ̨�Ĳ�ͬӲ��(�����в�ͬ���豸����������)ʹ��ͬ������������ṩ�����ýӿ�
@param[in] nTunerIndex ����ѯTuner�豸����
@param[out] psTunerInfo Tuner������Ϣ������ΪNULL
@return �ɹ�����TRUE;ʧ�ܷ���FALSE
@note CSRMGetDeviceCounts���ص�Tuner�����Ǹ��ݸýӿڻ�ȡ��
@note ����ʱ���뱣֤������0��ʼ���ϲ�ģ���ͨ������0--Tuner����-1��������ȡ����Tuner�豸����Ϣ
*/
BOOL CSRMSetTunerConfig(int nTunerIndex,CSRMTunerConfig_S* psTunerInfo);

/**
@brief ���õ�Ȼ�Ƿ�����RMģʽ

����RMģʽʱ�����еײ��豸�Ĳ�������hPathΪ����Ψһ��ʶһ���豸����ģʽ��Ҫ����MMCP\n
������RMģʽʱ�����еײ��豸��indexΪ����Ψһ��ʶһ���豸����ģʽ��Ҫ����USP+GUI��Ŀ\n
����һ��Ϊ����Ŀ���ݶ��ṩ��Coshipר�ýӿڣ�����MMCP��Ŀ����MMCP�м�����ʹ��RMģʽ
@param[in] bSupport TRUE:����RMģʽ��FALSE:������RMģʽ
*/
VOID CSRMSetRMSupport(BOOL bSupport);

/**
@brief ��ѯ��ǰ�Ƿ�����RMģ��

@return bSupport TRUE:��ǰ������RMģʽ��FALSE:��ǰδ����RMģʽ
@see CSRMSetRMSupport
*/
BOOL CSRMIsSupport(void);

/**
@brief ����ĳ��tuner��demux

@param[in] nTunerNo �����ӵ�tuner����
@param[in] nDemuxNo �����ӵ�demux����
@deprecated �ýӿڽ�Ϊ��֤DTVģ��ļ��ݶ�Ԥ�����벻Ҫʹ�ã���ʵ�ּ��ɡ�
*/
void CSRMTunerConnDemux(int nTunerNo, int nDemuxNo);//������ȷ��

#ifdef __cplusplus
}
#endif

/** @} */

#endif
