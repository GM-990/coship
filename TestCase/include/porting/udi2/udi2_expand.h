/**@defgroup ��ģ����Ҫ�������������оƬ��������Χ��չ�豸
����: 1��ӲAP ��wifi �豸
             2��cable modem
             3��EOC
@{
*/

#ifndef _UDI2_EXPAND_H_
#define _UDI2_EXPAND_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "udi2_ipcfg.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
@brief ��ȡ��չ�����豸������״̬������: ӲAP��cable modem��EOC��

@param[in] pcEthName ����չ�����豸��������������
@param[out] pushLinkState ����״̬
@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE
@note ��������������״̬:
@note 1��pushLinkState & CSUDI_ETHMASK_UP_DOWN : Ϊ0�����Ѿ����ӣ�Ϊ1����δ���ӡ�
@note 2�����ɳ��̿������о�����ȡ����״̬�ķ���������: SNMP ��ѯЭ��
*/
CSUDI_Error_Code CSUDIExpandGetStatus(char * pcEthName, unsigned short * pushLinkState);

#ifdef	__cplusplus
}
#endif

/**@}*/

#endif 


