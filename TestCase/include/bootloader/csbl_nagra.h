/**@defgroup  �ṩNagra�淶booter�ӿ�
@brief NAGRABOOTģ���Ӧͷ�ļ�<csbl_nagra.h>��������Nagra�淶booter���еĹ��ܼ����ýӿڡ�

@version 1.0.0 2012/2/03
@{
*/
#ifndef _CSBL_NAGRA_H_
#define _CSBL_NAGRA_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "csbl_gconfig.h"
#include "udi2_toolset.h"
#include "udiplus_typedef.h"


/*
 �ṹ��˵��:
1�� ��MMCP BL�У�������SPI FLASH������NOR FLASH��APP��LOADER��Ҫ���ȡ���ڴ��н���У��
2���ṩ��NAGRA��JUMPCODEҪ���Ϊ�ڴ��ַ��������FLASH��ַ
	 
*/
typedef struct
{
    CSBLDataPos_S m_sDA2Info;
    CSBLDataPos_S m_sDa2BakInfo;
    CSBLDataPos_S m_sDa2LockInfo;
    CSBLDataPos_S m_sPkInfo;
    DWORD 		m_LoaderDdrPos;	//����ĵ��б�ʶ��LOADER�ڴ�У���ַ
    DWORD 		m_AppDdrPos;		//����ĵ��б�ʶ��APP�ڴ�У���ַ
}CSBLNagraConfig_S;


/*
����˵��:
CSBLNagraSBPCheck ������ָ��ģ�����NAGRA SBPУ��

�������:
pCheckType[in]: У���������͵�ַ��Ϣ��У����������ΪCSUDIToolsetExecType_E
pConfigInfo [in]: NARGA���������Ϣ
pPos[in]:		     У�����ݵ�ַ��Ϣ
pJcAddr [in]:       ����JUMPCODE ��ַ��Ϣ�ĵ�ַ

����ֵ:
	FALURE:CHECKʧ�ܣ�SUCCESS:У��ɹ�
*/

int CSBLNagraSBPCheck(CSUDIToolsetExecType_E* pCheckType,CSBLNagraConfig_S* pConfigInfo,CSBLDataPos_S* pPos,PDWORD pJcAddr);


#ifdef  __cplusplus
}
#endif

#endif



