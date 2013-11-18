/******************************************************************************
FileName:  ca_inject.c
Copyright (c) 2007�� Shen Zhen Coship Electronics CO.,LTD��

Version:       Ver 1.0
Author:        liuzhenghua

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2008-3-11  liuzhenghua      Create
*******************************************************************************/

/******************************************************************************
 *                                 Head file                                 *
 ******************************************************************************/
#ifndef CSCAINJECT_H
#define CSCAINJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"

typedef enum _CaInjectCatCachePolicy
{
	CAT_CACHE_ONLY_CURRENT_TS = 0,		//ֻ���浱ǰƵ���cat
	CAT_CACHE_ALL_TS		//��������Ƶ���cat
}CaInjectCatCachePolicy_E;

/*��ע����ԣ�Ӧ�ÿɽ���CSCAINJECT_ENABLE_ALL_TABLE_INJECT֮���ѡ��������ʹ�ã����Ʊ��ע��*/
#define CSCAINJECT_ENABLE_ALL_TABLE_INJECT		(0)		///<����ע�����еı�����û������ã���ΪĬ��
#define CSCAINJECT_DISABLE_NIT_INJECT			(1<<0)	///<��ֹע��NIT��
#define CSCAINJECT_DISABLE_BAT_INJECT			(1<<1)	///<��ֹע��BAT��


/******************************************************************************
Function Name:    FOnCAHaveMsgToUser

Description:
      ע��CAģ��Ļص��������壬ͨ��������������Ӧ�÷���CA_ALARM��CA_INFO
      CA_COMMAND��CA_STATUS��Ϣ��
Input:
	DWORD nUserData,�û�����
	int nMsg,       ��Ϣ���ͣ�����CA_ALARM��CA_INFO��CA_COMMAND��CA_STATUS
	DWORD dwPara1,  nMsg��Ϣ�е�����Ϣ����
	DWORD dwPara2����Ϣ��Я���Ĳ���
Output:
    	��
Return Value:
    	��
Other:
*******************************************************************************/
typedef int (*FOnCAHaveMsgToUser)(DWORD nUserData,int nMsg,DWORD dwPara1,DWORD dwPara2);
/******************************************************************************
Function Name:    CSRegisterCACallback

Description:
      ע��CAģ��Ļص�������ͨ��������������Ӧ�÷���CA_ALARM��CA_INFO
      CA_COMMAND��CA_STATUS��Ϣ��
Input:
	DWORD nUserData,�û�����
	FOnCAHaveMsgToUser hcbMsg �û�Ҫע��Ļص�����
Output:
    	��
Return Value:
    	��
Other:
*******************************************************************************/
void CSRegisterCACallback(DWORD nUserData,FOnCAHaveMsgToUser fnCallback);

/******************************************************************************
Function Name:    CSInitCA

Description:
      ��ʼ��CAģ�飬���ӿ�ֻ����һ��
	  ��SI_cahceע��CA�����Ļص�����
Input:
	��
Output:
    	��

Return Value:
    	CAģ����
Other:
*******************************************************************************/
HCSHANDLE CSInitCA();

/******************************************************************************
Function Name:    CSDestoryCA

Description:
      ����CAģ�飬���ӿ�ֻ����һ��

Input:
	hCAhandle CSInitCA���صľ��
Output:
    ��

Return Value:
    	��
Other:
*******************************************************************************/
void CSDestroyCA(HCSHANDLE hCAhandle);
/******************************************************************************
Function Name:    CSStartCA

Description:
      ����CA����ÿ��������Ŀ������CA

Input:
	
Output:
    ��

Return Value:
    	�ɹ�����TRUE��ʧ�ܷ���FALSE
Other:
*******************************************************************************/
BOOL CSStartCA( HCSHANDLE hCAhandle, HCSHANDLE hPath, int nOriNetworkID, int nTSID, int nServiceID, int nVideoPID, int nAudioPID  , BOOL bRestartCA);
/******************************************************************************
Function Name:    CSStopCA

Description:
      ֹͣCA����ÿ�ν�����Ŀ��ֹͣCA

		a�� nChangePlay = 0:
		��stopca֮��ʼ���ţ�֮��pmt��ص��϶��ᵽ����
		�ٴε���CSStartCAʱ��bPmtCome = FALSE;//�ȵ�pmt������ע��

		b nChangePlay = 1:
		�ڲ��Ź�����pmt�����֮ǰ�ķ����˱仯����stopcaǰ������pmt��汾�ű仯��
		�ٴε���CSStartCAʱ��bPmtCome = s_bPmtChanged; //ֻ�з�����pmt��汾�ű仯��ע��

		c. nChangePlay = 2��
		�ڲ��Ź���ʱ������pmt���е�����Ƶidͬdm�б���Ĳ�һ�£���������ca��֮�����pmt�����汾�ű仯�����򲻻��ٴ�����
		�ٴε���CSStartCAʱ�����ı�bPmtCome��״̬����Ϊpmt�����ٴε����ˡ�

Input:
	
Output:
    ��

Return Value:
    	�ɹ�����TRUE��ʧ�ܷ���FALSE
Other:
*******************************************************************************/
BOOL CSStopCA( HCSHANDLE hCAhandle, int nVideoPID, int nAudioPID ,int nChangePlay);
/******************************************************************************
@deprecated 	��ʱ�Ľӿڣ�Ŀǰû����ĿҪ��

Function Name:    CSEnableCaInject

Description:
	ֹͣcat�����caģ���ע��
Input:
	void
Output:

Return Value:
	�ɹ���TRUE
	ʧ�ܣ�FALSE

Other:
*******************************************************************************/
BOOL CSEnableCaInject( );

/******************************************************************************
@deprecated 	��ʱ�Ľӿڣ�Ŀǰû����ĿҪ��
Function Name:    CSDisableCaInject

Description:
	����cat����caģ���ע��
Input:
	void
Output:

Return Value:
	�ɹ�����0
	ʧ�ܣ�0

Other:
*******************************************************************************/
BOOL CSDisableCaInject();

//֪ͨcainjectƵ���л�����Ҫ����ϴδ洢��CAT��
void CSCaInjectChangeFreq();

/******************************************************************************
Function Name:    CSCaInjectSetCatCachePolicy

Description:
	����catinject���õ�cat�������
Input:
	eCachePolicy, ������ԡ���������ڴ��ʱ����ΪCAT_CACHE_ALL_TS����������߿�Ƶ�㻻̨�ٶȡ���������ΪCAT_CACHE_ONLY_CURRENT_TS��
		Ĭ�ϲ���ΪCAT_CACHE_ONLY_CURRENT_TS��
Output:

Return Value:
	�ɹ���TRUE
	ʧ�ܣ�FALSE

Other:
	�ú���������CSInitCA֮ǰ���á�������Ч
*******************************************************************************/
BOOL CSCaInjectSetCatCachePolicy(CaInjectCatCachePolicy_E eCachePolicy);
	
/******************************************************************************
Function Name:	  CSCaInjectSetInjectPolicy

Description:
	����catinjectע����caģ��ʱ�Ĳ��ԡ�Ŀǰ������nit,bat�Ĳ���
Input:
	nInjectPolicy,��ע����ԣ�Ĭ��Ϊȫ������ע��(CSCAINJECT_ENABLE_ALL_TABLE_INJECT)�� ����򿪺͹ر�nit��batע��
Output:

Return Value:
	�ɹ���TRUE
	ʧ�ܣ�FALSE

Other:
	�ú���������ʱ�򿪺͹رգ������̰߳�ȫ�ġ������û���ϵͳ������ʼ��ca����á�
	��Ϊcat,pmt�������ֱ��ע�����̶�����ca����һ���ģ���˲������û����á����õ�nit��bat���ע�����ֻ��sicache/cainject��Ӱ�죬
	�û���Ȼ�����������ط�����ע���
	������Դ����Ϊ�е�ca����������Ѷ����Ҫ��ֻע����Ƶ���nit���ṩ�˽ӿڣ��ɹر�cainject��nit��ע�룬����shellע��
*******************************************************************************/
BOOL CSCaInjectSetInjectPolicy(int nInjectPolicy);


#ifdef __cplusplus
}
#endif

#endif

