#ifndef CSSHELL_OTA_PRIVATE_H
#define CSSHELL_OTA_PRIVATE_H

#include "mmcp_typedef.h"
#include "csshell_ota.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡota���������ӿ�
@param ��
@return psParam ota����shell��Ҫʵ�ֵĽӿ�
*/
const CSShellOtaInitParam_S* CSShellOtaGetParam();

#ifdef __cplusplus
}
#endif

#endif

