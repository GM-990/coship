#ifndef CSSHELL_VOIP_PRIVATE_H
#define CSSHELL_VOIP_PRIVATE_H

#include "mmcp_typedef.h"
#include "csshell_voip.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ��ȡota���������ӿ�
@param ��
@return psParam ota����shell��Ҫʵ�ֵĽӿ�
*/
const CSShellVOIPInitParam_S* CSShellVOIPgetParam();

#ifdef __cplusplus
}
#endif

#endif

