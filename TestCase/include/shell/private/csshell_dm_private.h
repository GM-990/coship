/**@defgroup PublicShellDM PublicShellDM ����DM����ӿ�

@version 2.0.0 2009/12/16 ����
@{
*/
#ifndef CSSHELL_DM_PRIVATE_H
#define CSSHELL_DM_PRIVATE_H

#include "csshell_dm.h"

#ifdef __cplusplus
extern "C"
{
#endif
		
/**
@brief ��ȡDMʵ�ֽӿ�
@return �ɹ�����֮ǰ���õ�DMʵ�ֽӿ�
@see CSShellDMGetDefaulFunc
*/
const CSShellDMFunc_S* CSShellDMGetFunc(void);


#ifdef __cplusplus
}
#endif
/** @} */
#endif


