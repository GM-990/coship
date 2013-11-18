#ifndef _CS_UDI2_TEST_CASE_H_
#define _CS_UDI2_TEST_CASE_H_

#include "cs_testkit.h"

#ifdef __cplusplus
extern "C" {
#endif

CSUDI_BOOL CSTC_UDI2_Init(void);
CSUDI_BOOL CSTC_UDI2_UnInit(void);

/**
@brief 平台锁频接口

@param[in] nTunerIndex tuner设备索引号
@param[in] pTsFilename 锁频的文件名，由配置文件提供，例如"Audio&Video Test_27Mbps_20070524.ts"
@return 成功返回CSUDI_TRUE
@note 该接口第一次调用会初始化所有配置文件提供的信息，比如频点，符号率，调制方式等
@note 该接口不负责tuner与demux的连接操作，请调用者自行进行连接和取消链接
*/
CSUDI_BOOL CSTC_UDI2PortingLock(int nTunerIndex,const char* pTsFilename);

CSUDI_BOOL CSTC_UDI2_TestTunerDemux_001();

#ifdef __cplusplus
}
#endif

#endif

