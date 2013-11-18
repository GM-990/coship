/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_CI_Interface.h  
* Author:      HuYuanfeng
* Date:        Oct 2006 
* Description: CI interface
*            
*****************************************************************************************************************************/

/****************************************************************************************************************************
* 			                Revision History                                *
*****************************************************************************************************************************
Revision: 1.0  
date October 11,2006
author:Hu Yuanfeng 
reason:Create.

Revision: 1.1  
date: November 21,2007
author:Hu Yuanfeng 
reason:change some fuction definitons.

Revision: 1.2  
date: March,19,2008
author:Hu Yuanfeng 
reason:delete some old functions ,and add new function for Twin Tuner&Twin CI



*****************************************************************************************************************************/


#ifndef _CS_CI_INTERFACE_H_
#define _CS_CI_INTERFACE_H_


#ifdef _cplusplus
extern "C"{
#endif

#define MAX_ANSW_LEN  		100
#define MAX_TITLIE_LEN  		200
#define MAX_SUBTITILE_LEN 	 200
#define MAX_BOTTOM_LEN  	200
#define MAX_ITEM_NO		25
#define MAX_MESSAGE_LEN  	256

#define CS_CI_MESSAGE_BASE  (0)


typedef enum _CiTraceFlow
{
	CI_FLOW_NONE = 0x0,
	CI_FLOW_INIT = 0x01,
	CI_FLOW_PLAY = 0x02,
	CI_FOLW_UI   = 0x04,
	CI_FOLW_MSG  = 0x08,
	CI_FLOW_CIS  = 0x10,
	CI_FLOW_BUFFER  = 0x20,
	CI_FLOW_WRITE  = 0x40,
	CI_FLOW_READ  = 0x80
}T_CiTraceFlow;


typedef enum _CI_MSG
{
	CI_ALARM = CS_CI_MESSAGE_BASE,
	CI_COMMAND,
	CI_STATUS,
	CI_INFO,
	CI_MMI
}E_CI_MSG;

typedef enum _CI_Alarm
{
	CI_ALARM_CAM_OUT,             /*Please Insert CAM Card*/
	CI_ALARM_CAM_UNKNOW,          /*Unknow CAM Card*/
	CI_ALARM_CAM_IN,              /*CAM Card In*/	
	CI_ALARM_CAM_INIT,           //CAM ���ڳ�ʼ��
	CI_ALARM_CAM_OK,              /*CAM Card Ok*/

	//CAM_SCRAMBLED - Ƶ�����Ų��ܲ��� ,����: 0 - PMT��Ч����ͼ���ܲ���(scrambled); 1 - PMT��Ч��ͼ���ܲ���(bad)
	CI_ALARM_SCRAMBLED,//��Ŀ����, ���Ų��ɹ�

	CI_ALARM_NEED_TECHNICAL   	//����Ϣ��UI,��֪���ּ������⣬������������ޣ�
									//��Ҫ���û�ѡ����ٵĻ�����	
}E_CI_Alarm;

typedef enum _CAM_STATUS
{
	CAM_STATUS_EMPTY,//�գ������ڰγ�����û�в忨��ʱ��                  
	CAM_STATUS_INITIALIZE, //��ʼ��ʼ���������ڲ���CAM��ʱ��     
	CAM_STATUS_WORK_DELAY, //׼�������У���ʱCAM����Ϣ�Ѿ����
	CAM_STATUS_WORKING,   //���������� 
	CAM_STATUS_LOCKED,     //ģ�鱻����
	CAM_STATUS_UNLOCK      //ģ��δ����
}E_CAM_STATUS;

typedef enum _CI_MMI
{
	CI_MMI_ENQ, //��ѯmonitor
	CI_MMI_MENU, //�г��˵�
	CI_MMI_LIST, //�б�
	CI_MMI_CLOSE//�رնԻ�
}E_CI_MMI;
	
typedef enum _CI_INFO
{
	CI_INFO_CAMINFO,
	CI_INFO_RESERVED
}E_CI_INFO;

typedef enum _CaPmtListOperationModel 
{
	PMT_SECTION_MORE,
	PMT_SECTION_FIRST,
	PMT_SECTION_LAST,
	PMT_SECTION_ONLY,
	PMT_SECTION_ADD,
	PMT_SECTION_UPDATE,
	PMT_SECTION_RESERVED
} E_CaPmtListOperationModel;



typedef struct _CAM_INFO
{
	BYTE  ucAppType; //ȡֵ: 1:CA   0: EPG  
	INT   nAppManufacture;//CA system ID
	INT   nManufactureCode;//���������Լ����������
	BYTE ucNameLen;
	BYTE  acSystemName[30];//�ã����̵�����
}T_CAM_INFO;



/*========================================================================================*/
/*���ṹ�����������ط�:
	1����Ϊ��Ϣ����ΪCI_MMI_ENQ�Ļص������ĵ��ĸ���������ʱ���¼���ֵ
		������:
		  ucBlind [out]:    Ϊ1ʱ����ʾ��������ݲ���ʾ������*����,��Ϊ����ֵʱ
		  			    ���� �� ��ʾ
		  			    
		  ucAnswLen[out]: Ϊ0xff ʱ,���������ϲ㶨���������ݵĳ���,��Ϊ��Ϊ0xff
		  				ʱ,��ʾ�û�����������ݵĳ���
		  				
		  acAnswText[out] Ϊ�ڽ�������ʾ���ı���Ϣ
		  
	2����Ϊ����CS_CI_MMI_AnswEnq ��ȡ�û�Ӧ��Ĳ���,��ʱ���¼���ֵ������:
	
		ucAnswId[in]:�����û���ѡ��ȡֵ,���û�ͬ��Ӧ��ʱȡֵΪ1,���û�ȡ��
					Ӧ��ʱ,ȡֵΪ0
		
		ucAnswLen[in\out]: ��ȡֵΪ0xff ʱ,�ϲ��Զ��峤��,���ܳ���MAX_ANSW_LEN.
						  ��ȡֵΪĳһ����ֵʱ,�ͱ�ʾ�û�������ĳ���,��ʱ
						  �û������Զ��峤��
		
		acAnswText[in]:�û����������

==========================================================================================*/

typedef struct _Answ_Info
{
	BYTE ucAnswId;   
	BYTE ucBlind;
	BYTE ucAnswLen;
	CHAR acAnswText[MAX_ANSW_LEN];
}T_Answ_Info;




/*=======================================================================================
��������list ��menu �Ľṹ�壬��Ϊ��Ϣ����ΪCI_MMI_MENU��CI_MMI_LIST �Ļص�����
�ĵ��ĸ�����,�����ϲ����¼���ֵ������:

	ucItem_NB[out]:ѡ��ĸ���,Ϊ����aacMessage�ĵ�һ���±� �����ֵ

	acTitle[out]:��������,�����ĳ���С��MAX_TITLIE_LEN ���߳���Ϊ0 ��ʱ��,������
			    ��'\0'����,  �����ĳ��ȴ��ڵ���MAX_TITLIE_LEN ��ʱ��,ֻ��ʾǰ
			    MAX_TITLIE_LEN ��

	acSubtitle[out]:�ӱ�������,�����ĳ���С��MAX_SUBTITILE_LEN ���߳���Ϊ0 ��ʱ��,
				   ������ ��'\0'����,  �����ĳ��ȴ��ڵ���MAX_SUBTITILE_LEN ��ʱ��,ֻ
				   ��ʾǰMAX_TITLIE_LEN ��

	acBottom[out]:��������,�����ĳ���С��MAX_BOTTOM_LEN ���߳���Ϊ0 ��ʱ��,
				 ������ ��'\0'����,  �����ĳ��ȴ��ڵ���MAX_BOTTOM_LEN ��ʱ��,ֻ��
				 ʾǰMAX_BOTTOM_LEN ��
	aacMessage[out]:ѡ����Ϣ,�����ʾucItem_NB ��,ÿ���ĳ���С��MAX_MESSAGE_LEN ����
				    ����Ϊ0 ��ʱ��,������ ��'\0'����,  �����ĳ��ȴ��ڵ���MAX_MESSAGE_LEN 
				    ��ʱ��,ֻ��ʾǰMAX_MESSAGE_LEN ��

=======================================================================================*/

typedef struct _MMI_Event
{
	BYTE  ucItem_NB;
	WORD wTag_Header;
	BYTE ucTag_Value;

	BYTE acTitle[MAX_TITLIE_LEN];
	BYTE acSubtitle[MAX_SUBTITILE_LEN];
	BYTE acBottom[MAX_BOTTOM_LEN];
	BYTE aacMessage[MAX_ITEM_NO][MAX_MESSAGE_LEN];
}T_MMI_Event;


typedef struct _CI_InitPara
{
	//ci task priority
	INT m_nTaskPrio_Phy;
	INT m_nTaskPrio_Link_SS;
	INT m_nTaskPrio_Rm;

	//task stack size
	INT m_nStackSize_Phy;
	INT m_nStackSize_Link_SS;
	INT m_nStackSize_Rm;
	
	//ci capabilites
	DWORD m_dwSlotNumber;
	DWORD m_dwTunerNumber;
	DWORD m_dwTsChannel;	
	
}T_CI_InitPara;

/*=======================================================================================*/

									/*����ΪAPI ����*/

/*=======================================================================================*/

typedef void (*T_CI_Msg)(HCSHANDLE hCiSlot, DWORD dwParm1, DWORD dwParm2, DWORD dwParm3);


/**************************************************************************
*Function Name: CS_CI_RegisterMsgCallback
*    CI��Ϣ����ע��ص�����
*
*Parameters	
*     hcbMsg [in]:
*         T_CI_Msg���ͻص�����.
*           hCiSlot;    //SlotID
*           dwParm1;    //Msg type
*           dwParm2;   //item
*           dwParm3;   //ֻ����Ϣ����ΪCI_MMI��CI_INFO��ʱ�����ã��������ʱΪ0 
*
*Return Values
*       0 : SUCCESS ע��ص������ɹ�
*       -1: FAILURE ���������ע��ص�����ʧ��
**************************************************************************/
INT CS_CI_RegisterMsgCallback(T_CI_Msg hcbMsg);	

/**************************************************************************
*Function Name: CS_CI_Init
*
* ����:���ӳ�ʼ������
*
*  ����: ��ʼ��CIģ��
*
*Parameters	
*    stInitPara[in]:CI ģ���ʼ������
*		           
*Return Values
*		0 : SUCCESS ��ʼ��CIģ��ɹ�
*		-1: FAILURE ��ʼ��CIģ��ʧ��
**************************************************************************/
INT CS_CI_Init(T_CI_InitPara stInitPara);


/**************************************************************************
*Function Name: CS_CI_StartService
*
*  ����˵�������øú���������PMT��֪ͨCIģ�鿪ʼ����ĳ�׽�Ŀ
*
*Parameters
*   eModel[in]:����ȡֵΪö������T_CaPmtListOperationModel��ѡ��
*   pacPmtSectionBuf[in]:ָ������������PMT SECTION������TABLEID�ֶκ�CRC��
*   dwBufSize[in]�����PMT SECTION�Ļ������Ĵ�С��>=3+SECTION_LENGTH��
*		           
*Return Values
*		0 : SUCCESS pmt ���ͳɹ������Կ�ʼ����
*		-1: FAILURE  pmt����ʧ��
**************************************************************************/
INT CS_CI_StartService(E_CaPmtListOperationModel eModel, BYTE* pacPmtSectionBuf,  DWORD dwBufSize);

/**************************************************************************
*Function Name: CS_CI_Stop
*
*����:Ϊ��ͳһ������񣬸��Ľӿ����ƣ�����ͬCS_StopCI
*
*  ����˵����Ҫ��CI ģ��ֹͣ����
*Parameters
*   
*   
*Return Values
*		0 : SUCCESS  ֹͣ����
*		-1: FAILURE  ֹͣ����ʧ��
**************************************************************************/
INT CS_CI_Stop(void);
/**************************************************************************
*Function Name: CS_CI_StopService
*
*  ����˵����Ҫ��CI ģ��ֹͣ����ĳһ����Ŀ
*
*Parameters
*   nVideoPID[in]:
*   nAudioPID[in]:
*   nPcrPID[in]: ���PcrPID
*   
*Return Values
*		0 : SUCCESS  ֹͣ����ĳһ����Ŀ
*		-1: FAILURE  ֹͣ����ʧ��
**************************************************************************/
INT CS_CI_StopService( INT nVideoPID, INT nAudioPID, INT nPcrPID);

/******************************************************************************
*Function Name :	CS_CI_GetCamStatus
*    ���CAM������״̬
*Parameters :		
*������hCiSlot[in]:	����ѯ�Ķ�Ӧ��CAM����SlotID
*      pcStatus[out]: ��õ�CAM������״̬��
*			ȡֵΪE_CAM_STATUS�е�:
*			CAM_STATUS_LOCKED,     //ģ�鱻����
*	        CAM_STATUS_UNLOCK      //ģ��δ����
*Return��Values:
*������0 : SUCCESS ����ɹ�
*	�� -1: FAILURE ����ʧ��
******************************************************************************/
INT CS_CI_GetCamLockStatus(HCSHANDLE hCiSlot, BYTE* pcStatus);

/**************************************************************************
*Function Name: CS_CI_GetCamInfo
*  ��ȡCAM����Ϣ
*
*Parameters 
*   hCiSlot[in]:
*       ��Ӧ��CAM����SlotID
*
*Return Values: 
*		0 : SUCCESS ����ɹ�
*	�� -1: FAILURE ����ʧ��
**************************************************************************/
 INT CS_CI_GetCamInfo(HCSHANDLE hCiSlot);

/******************************************************************************
*Function Name :	CS_CI_MMI_EnterMenu
*    ʵ���˻��������������CAMӦ�ò˵������ж�ģ��Ӧ�ò˵�����ǰ�������������˵�
*Parameters :		
*������hCiSlot[in]:	����ѯ�Ķ�Ӧ��CAM����SlotID
*Return��Values:
*������0 : SUCCESS ����ɹ�
*	��     -1: FAILURE ����ʧ��
******************************************************************************/
INT CS_CI_MMI_EnterMenu(HCSHANDLE hCiSlot);

/**************************************************************************
*Function Name: CS_CI_MMI_CloseMmi
*  ����ر�CAM MMI�Ự���ú�����������ã�ģ��Ӧ�ý��˳�ȫ���˵�
*
*Parameters 
*   hCiSlot[in]:
*       ����ѯ�Ķ�Ӧ��CAM����SlotID
*
*Return Values: 
*������0 : SUCCESS ����ɹ�
*	��	     -1: FAILURE ����ʧ��
**************************************************************************/
INT CS_CI_MMI_CloseMmi(HCSHANDLE hCiSlot);

/**************************************************************************
*Function Name: CS_CI_MMI_AnswMenu
*  ѡ��CAMӦ�õĲ˵��������Ӧ���Ӳ˵����߶�Ӧ�Ĳ�ѯ��Ϣ
*  �б�
*
*Parameters 
*   hCiSlot[in]:
*       ����ѯ�Ķ�Ӧ��CAM����SlotID
*   ucChoice_Ref[in]:
*       �û�ѡ��Ĳ˵�ѡ��ı�ţ���ucChoice_Ref = 0x00��ʱ�򣬱�ʾ
*       �û�û������ѡ�񣬶����˳��˵����б�
*Return Values: 
*������0 : SUCCESS ����ɹ�
*	��     -1: FAILURE ����ʧ��
**************************************************************************/
INT CS_CI_MMI_AnswMenu(HCSHANDLE hCiSlot, DWORD dwChoice_Ref);
	
/**************************************************************************
*Function Name: CS_CI_MMI_AnswEnq
*  ��Ӧ�ã�ģ��Ĳ�ѯ������ͻ�Ӧ��Ϣ
*
*Parameters 
*   hCiSlot[in]:
*       ����ѯ�Ķ�Ӧ��CAM����SlotID
*   tAnswerInfo[in]:
*       ��ʾ֪ͨģ���Ƿ�Դ�Ӧ��������Ӧ��0ȡ��Ӧ��  1 Ӧ��
*
*Return Values: 
*������0 : SUCCESS ������Ϣ�ɹ�
*	��	    -1: FAILURE ������Ϣʧ��
**************************************************************************/
INT CS_CI_MMI_AnswEnq(HCSHANDLE hCiSlot, T_Answ_Info* tAnswerInfo);

/**************************************************************************
*Function Name: CS_CI_SetDebugMode
*  CI ��ӡ����
*
*Parameters 
*   dwMode[in]:
*       ��T_CiTraceFlow
*
*Return Values: 
*������
**************************************************************************/
void CS_CI_SetDebugMode(DWORD dwMode);

/**************************************************************************************

		Ϊ����¹��ֳ����Եĺڿ�CAM  �ظ���ʼ��,���������½ӿ� 

***************************************************************************************/



/**************************************************************************
*Function Name: CS_CI_NotifyPid
* �����ŵĽ�Ŀ������ƵPID ֪ͨCI  ģ��
*
*Parameters 
*   pwPID[in]:
*       ��������ƵPID ������ĵ�ַ
*   ucNumOfPID[in]:
*    PID����
*hTuner[in]:
*Tuner�ľ��
*
*Return Values: 
*������0 : SUCCESS �ɹ�
*	��	    -1: FAILURE ��������
**************************************************************************/
INT CS_CI_NotifyPid(WORD  * pwPID, DWORD  dwNumOfPID,HCSHANDLE hTuner);


/*=========================================================================

			����Ϊ�����ṹ���Լ������ӿ�,����·����ʱʹ��

===========================================================================*/
#define CI_MAX_PID_NUM		 (10)


typedef enum _CSCIOutputType_E
{
 	EM_CI_OUTPUT_PLAY   = 0x01, /*����*/
 	EM_CI_OUTPUT_RECORD = 0x02  /*¼��*/
}CSCIOutputType_E;

 typedef struct _CIChannelInfo_S
{
	WORD	m_wTunerID;			 /*tuner��*/
	WORD	m_wCPUChnl;			 /*cpu ts�����*/
}CIChannelInfo_S;

typedef struct _CISrvInfo_S
{
	WORD	m_wOrginalNetworkID; /*��ʼ����ID*/
	WORD    m_wTsID;             /*��ID��*/
	WORD    m_wServiceID;        /*��ĿID��*/
	WORD    m_wPidNum;      /*Pid ��Ŀ*/
 	WORD    m_awPid[CI_MAX_PID_NUM];/*Pid*/
	DWORD   m_dwUserData;        /*Ԥ��*/
	CSCIOutputType_E m_eOutputType; /*�������*/
	CIChannelInfo_S m_stCIChannelInfo;
	
}CISrvInfo_S;

/**************************************************************************
*Function Name: CS_CI_StopService
*
*  ����˵������������/¼��
*
*Parameters
*   pstCISrvInfo[in]: ��Ŀ�����Ϣ
*   
*Return Values
 *   NULL  - ��������/¼�Ʋ��ɹ�
 *   ��NULL- ��������/¼�Ƴɹ�
**************************************************************************/
HCSHANDLE CSCIStartService(CISrvInfo_S* pstCISrvInfo);

/**************************************************************************
*Function Name: CS_CI_StopService
*
*  ֹͣ����/¼��
*
*Parameters
*   hCIService[in]:����/¼��ʱ�����Handle
*   
*Return Values
*		 SUCCESS - ֹͣ����/¼�Ʋ��ɹ�
*        FAILURE - ֹͣ����/¼�Ƴɹ�
**************************************************************************/
INT CSCIStopService( HCSHANDLE hCIService );

/**************************************************************************
*Function Name: CSCINotifyNewPmt
*
*  ����˵�������øú���������PMT��֪ͨCIģ�鿪ʼ����ĳ�׽�Ŀ
*
*Parameters
*   hCIService[in]:����/¼��ʱ�����Handle
*   eModel[in]:����ȡֵΪö������T_CaPmtListOperationModel��ѡ��
*   pacPmtSectionBuf[in]:ָ������������PMT SECTION������TABLEID�ֶκ�CRC��
*   dwBufSize[in]�����PMT SECTION�Ļ������Ĵ�С��>=3+SECTION_LENGTH��
*		           
*Return Values
*		0 : SUCCESS pmt ���ͳɹ������Կ�ʼ����
*		-1: FAILURE  pmt����ʧ��
**************************************************************************/
INT CSCINotifyNewPmt(HCSHANDLE hCIService, E_CaPmtListOperationModel eModel, BYTE* pacPmtSectionBuf, DWORD dwBufSize);
	
#ifdef  __cplusplus
}
#endif

#endif 

