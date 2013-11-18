#ifndef _CS_UDI2_TEST_CASE_H_
#define _CS_UDI2_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_UDI2_Init(void);
CSUDI_BOOL CSTC_UDI2_UnInit(void);

/**
@brief ƽ̨��Ƶ�ӿ�

@param[in] nTunerIndex tuner�豸������
@param[in] pTsFilename ��Ƶ���ļ������������ļ��ṩ������"Audio&Video Test_27Mbps_20070524.ts"
@return �ɹ�����CSUDI_TRUE
@note �ýӿڵ�һ�ε��û��ʼ�����������ļ��ṩ����Ϣ������Ƶ�㣬�����ʣ����Ʒ�ʽ��
@note �ýӿڲ�����tuner��demux�����Ӳ���������������н������Ӻ�ȡ������
*/
CSUDI_BOOL CSTC_UDI2PortingLock(int nTunerIndex,const char* pTsFilename);

CSUDI_BOOL CSTC_UDI2_TestTunerDemux_001();

#ifdef __cplusplus
}
#endif

#endif

