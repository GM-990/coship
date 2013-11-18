#ifndef JAVASHELL_STANDBY_H
#define JAVASHELL_STANDBY_H

#ifdef __cplusplus
extern "C" {
#endif


#include "mmcp_typedef.h"

/**����״̬ö��*/
typedef enum _CSStandbyState_E {
	EM_Standby_IN,       //�������״̬
	EM_Standby_OUT      //�˳�����״̬
} CSStandbyState_E;


/**
@brief ��ȡ����״̬�ص���������
@param[in] state ���CSStandbyState_E˵��
@param[in] pvUserData �û�����
*/
typedef void(*CSShellStandbyCallback_F) (CSStandbyState_E state, void* pUserData);

/**
@brief ע���ȡ����״̬�Ļص�������
@param[in] fnCallback ָ��ص������ĺ���ָ��
@param[in] pvUserData �û����ݣ�����ΪNULL
@note  ��������ע�����ص�
*/
DWORD CSShellAddStandbyCallback (CSShellStandbyCallback_F fnCallback ,void* pUserData);

/**
@brief ɾ����ȡ����״̬�Ļص�������
@param[in] fnCallback ָ��ص������ĺ���ָ��
*/
void CSShellDelStandbyCallback(CSShellStandbyCallback_F fnCallback);


/******************************************************************************
Function Name:    CSShellStandby
	
Description:
	���д�������
	
Input:
	standby:����ģʽ����   1�������   0���ٴ���

Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
BOOL CSShellStandby(BOOL standby);

/******************************************************************************
Function Name:    CSShellSetStandbyMode
	
Description:
	���ô���ģʽ
	
Input:
	standby:����ģʽ����   1�������   0���ٴ���

Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShellSetStandbyMode(int model);

/******************************************************************************
Function Name:    CSShellGetStandbyMode
	
Description:
	��ȡ����ģʽ
	
Input:
	None		

Output:
   	standby:����ģʽ   1�������   0���ٴ���

Return Value:
    None
Other:
*******************************************************************************/
BOOL  CSShellGetStandbyMode();


/******************************************************************************
Function Name:    CSShellSetParamForRealStandby
	
Description:
	���������ģʽ�Ĳ���
	
Input:
	nGPIO:���������źŵ�GPIO��
	lowVol:�������跽���͵�ƽʱ��
	highVol:�������跽���ߵ�ƽʱ��
	
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void CSShellSetParamForRealStandby(int nGPIO,int lowVol,int highVol );

/******************************************************************************
Function Name:    CSShellReboot
	
Description:
	����
	
Input:
    None	
	
Output:
    None

Return Value:
    None
Other:
*******************************************************************************/
void  CSShellReboot();

#ifdef __cplusplus
}
#endif

#endif

