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
//个性化配置参数结构体
//用来保存应用在调用本库是传的sn和ca的地址
typedef struct _CaInvidiuationCfg
{
	DWORD m_dwSnflashAdress;
	DWORD m_dwCaFlashAdress;
	DWORD m_dwCaBackFlashAdress;
	WORD   m_wSnLength;
	DWORD m_dwOption;
	BYTE     m_aucCusName[64];
	WORD   m_SerialMode;
	BOOL     m_bSMI; //smi 芯片需要在returnfile中提供cvc值
}CaInvidiuationCfg_S;


/**************************************************************************
* Function Name: CSCASetInividuaParamters
*   从应用传下去的用于个性化的参数
*
*Parameters:
* psCaInvidiuationCfg:个性化过程中CA和SN的FLASH地址
*
*Return Values:
*     0: 表示个性化启动成功	             
*     1: 表示个性化启动失败
*   
*	
**************************************************************************/
INT CSCASetInividuaParamters(CaInvidiuationCfg_S * psCaInvidiuationCfg);
/**************************************************************************
* Function Name: CSCAStartIndividuation
*   启动个性化过程
*
*Parameters:
* 	无
*
*Return Values:
*     0: 表示个性化启动成功	             
*     1: 表示个性化启动失败
*    
*	
**************************************************************************/
INT CSCAStartIndividuation(void);

/**************************************************************************
* Function Name: CSCAStopIndividuation
*   停止
*
*Parameters:
*   无
*
*Return Values:
*     0: 表示个性化启动成功	             
*     1: 表示个性化启动失败
*    
*	
**************************************************************************/
INT CSCAStopIndividuation(void);

#ifdef  __cplusplus
}
#endif
#endif //_CS_COMVENIENT_DMX_H_






