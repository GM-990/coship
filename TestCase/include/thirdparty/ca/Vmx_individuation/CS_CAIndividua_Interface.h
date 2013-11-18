/******************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    cs_CAInvidiua_Interface.h
* Author:      HOUJINFENG
* Date:        3 2012
* Description: Invidiualtion interface file
*            
*******************************************************************************/
/*******************************************************************************
* 			                                  Revision History                                				     *
********************************************************************************
* - Revision 1.0  2013/3/19
*   Programmer:HOUJINFENG
*   Create.
*******************************************************************************/
#ifndef _CS_CAINVIDIUA_INTERFACE_H_
#define _CS_CAINVIDIUA_INTERFACE_H_
#ifdef  __cplusplus
extern "C" {
#endif
typedef enum _SERIAL_MODE
{
	SERIAL_UPGRADE_MODE = 0x0001,
	SERIAL_VERIFY_MODE = 0x0002,
	SERIAL_ERASE_MODE = 0x0004,
}SERIAL_MODE_E;
//���Ի����ò����ṹ��
//��������Ӧ���ڵ��ñ����Ǵ���sn��ca�ĵ�ַ
typedef struct _CaInvidiuationCfg
{
	DWORD m_dwSnflashAdress;
	DWORD m_dwCaFlashAdress;
	DWORD m_dwCaBackFlashAdress;
	WORD   m_wSnLength;
	DWORD m_dwOption;
	BYTE     m_aucCusName[64];
	WORD   m_SerialMode;
	BOOL     m_bSMI; //smi оƬ��Ҫ��returnfile���ṩcvcֵ
}CaInvidiuationCfg_S;


/**************************************************************************
* Function Name: CSCASetInividuaParamters
*   ��Ӧ�ô���ȥ�����ڸ��Ի��Ĳ���
*
*Parameters:
* psCaInvidiuationCfg:���Ի�������CA��SN��FLASH��ַ
*
*Return Values:
*     0: ��ʾ���Ի������ɹ�	             
*     1: ��ʾ���Ի�����ʧ��
*   
*	
**************************************************************************/
INT CSCASetInividuaParamters(CaInvidiuationCfg_S * psCaInvidiuationCfg);
/**************************************************************************
* Function Name: CSCAStartIndividuation
*   �������Ի�����
*
*Parameters:
* 	��
*
*Return Values:
*     0: ��ʾ���Ի������ɹ�	             
*     1: ��ʾ���Ի�����ʧ��
*    
*	
**************************************************************************/
INT CSCAStartIndividuation(void);

/**************************************************************************
* Function Name: CSCAStopIndividuation
*   ֹͣ
*
*Parameters:
*   ��
*
*Return Values:
*     0: ��ʾ���Ի������ɹ�	             
*     1: ��ʾ���Ի�����ʧ��
*    
*	
**************************************************************************/
INT CSCAStopIndividuation(void);

#ifdef  __cplusplus
}
#endif
#endif //_CS_COMVENIENT_DMX_H_






