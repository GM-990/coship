/******************************************************************************
FileName:  cspm_record.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVR¼�Ƶ���ؽӿ�
ģ����PVRREC

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef CSPM_RECORD_H
#define CSPM_RECORD_H

#ifdef __cplusplus 
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"
#include "CSUSPCommonType.h"
#include "mmcp_os.h"

/*pvr���Ƶĳ���*/
#define CSPVR_NAME_SIZE 60

/*!
	��ȡϵͳ֧�ֵ�¼�ƾ������
\return ϵͳ֧�ֵ�¼�ƾ������
*/
DWORD CSPVRRECGetSupportRecCount(void);

/*!����¼�ƽ��������CSPVRRECStartRec�ķ���ֵ*/
typedef enum _CSPVRRECRet_E
{
	/*!����¼�Ƴɹ�*/
	EM_PVRREC_RET_SUCCUSS = 0,
	/*!�豸��ͻ*/
	EM_PVRREC_RET_NO_DEVICE,	
	/*!Ӳ������*/
	EM_PVRREC_RET_HDD_FULL,	
	/*!Ӳ��δ׼����*/
	EM_PVRREC_RET_NO_HDD,		
	/*!����¼����,�޷�����*/
	EM_PVRREC_RET_IS_WORKING,		
	/*!δ֪��������ʧ��*/
	EM_PVRREC_RET_UNKOWN_FAILED		
}CSPVRRECRet_E;

/*!
	����¼��
@param[in] pszLocator ��ʶͷΪdtv:��Locator��ʽΪ "dtv:// OrigNetId.TsID.ServiceID"������:"dtv:45.36.119"
@param[out] phRecHandle ���¼�ƾ��(����ʧ�������NULL)
\return ����¼�ƵĽ��(��CSPVRRECRet_E����)
\note 
*/
CSPVRRECRet_E CSPVRRECStartRec(const char *pszLocator, HCSHANDLE *phRecHandle);

/*!
	��ȡ����¼�Ƶľ��
@param[out] phOutRecHandleArray �������¼�Ƶľ������
@param[in] nArraySize �����С
\return ���ص�����¼�ƾ������
\note 
*/
int CSPVRRECGetRecording(HCSHANDLE* phOutRecHandleArray,int nArraySize);

/*!
	����¼�Ƶ�ǰ����ֱ����Ƶ��
@param[out] phRecHandle ���¼�ƾ��(����ʧ�������NULL)
\return ����¼�ƵĽ��(��CSPVRRECRet_E����)
*/
CSPVRRECRet_E CSPVRRECStartRecCurrentChnl(HCSHANDLE *phRecHandle);

/*!
	ֹͣ¼��
@param[in] hPvrRecHandle ¼�ƾ��
\return �Ƿ�ɹ�ֹͣ¼��
\note
*/
BOOL CSPVRRECStop(HCSHANDLE hPvrRecHandle);

/*!¼�Ƶ�����ģʽ*/
typedef enum _CSPVRRECRecStartMode_E
{
	/*!��ʱ¼��*/
	EM_PVRREC_STARTMODE_QUICK = 0,	
	/*!Ԥ��¼��*/
	EM_PVRREC_STARTMODE_SCHEDULE,	
	/*!¼������ģʽ������*/
	EM_PVRREC_STARTMODE_COUNT
}CSPVRRECRecStartMode_E;

/*!����¼�Ƶ���Ϣ*/
typedef struct _CSPVRRECRecInfo_S
{
	/*!Ƶ������WideWord �ַ���*/
	char					wsChnlName[CSPVR_NAME_SIZE];	
	/*!�û����޸ĵ��û��Ѻ���,WideWord�ַ���*/
	char					wsUserName[CSPVR_NAME_SIZE];
	/*!����¼�Ƶ�ʱ��*/
	CSUDIPLUSOSSysTime_S				sStartTime;	
	/*!¼�Ƶ�����ģʽ*/
	CSPVRRECRecStartMode_E	eStartMode;				
}CSPVRRECRecInfo_S;

/*!
	��ȡ¼����Ϣ
@param[in] hPvrRecHandle ¼�ƾ��
@param[out] psInfo ���¼����Ϣ
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note
*/
BOOL CSPVRRECGetRecInfo(HCSHANDLE  hPvrRecHandle, CSPVRRECRecInfo_S *psInfo);

/*!
	��ȡ¼��ʱ��
@param[in] hPvrRecHandle ¼�ƾ��
@param[out] pdwDurationMin ¼�Ƶ��ܳ���ʱ��,��λΪ����
@param[out] pdwPastTimeSec ¼���ѹ�ȥ��ʱ��,��λΪ��
@param[out] pdwRecLenthSec ¼�Ƶ���Чʱ��,��λΪ��
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note
*/
BOOL CSPVRRECGetRecTime(	HCSHANDLE  hPvrRecHandle, 
					   	DWORD *pdwDurationMin,
					   	DWORD *pdwPastTimeSec,
					   	DWORD *pdwRecLenthSec);
/*!
	����¼�Ƴ���ʱ��
@param[in] hPvrRecHandle ¼�ƾ��
@param[in] dwDurationMin ¼�Ƴ�����ʱ��,��λΪ����
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note ���õĳ���ʱ�䲻��С����¼��ʱ��
*/
BOOL CSPVRRECSetDuration(HCSHANDLE hPvrRecHandle, DWORD dwDurationMin);

/*!
	����¼��Ŀ���ļ����û��Ѻ���
@param[in] hPvrRecHandle ¼�ƾ��
@param[in] wsUserName �û��Ѻ���,WideWord�ַ���
@param[in] nNameLen �û��Ѻ������ַ������ȣ�����UserNameΪ���ַ���"abc",�򳤶�Ϊ3
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSPVRRECSetUserName(HCSHANDLE hPvrRecHandle, const char *wsUserName, int nNameLen);

/*!
	�ж�service �Ƿ�����¼��
@param[in] hService  Service���
\return ����¼�Ʒ���TRUE,δ¼�Ʒ���FALSE
*/
BOOL  CSPVRREC_IsRecording(HCSHANDLE hService);

/*!
	¼�ƻص�֪ͨö�����Ͷ���
*/
typedef enum _CSPVRRECNotifyType_E
{
	/*!
		¼��ֹ֪ͣͨ
		\note	dwNotifyType:	EM_PVRREC_NOTIFY_STOP 
		\note	pNotifyData:	δ����, 
		\note	hSender: 		hPvrRecHandle¼�ƾ��
	*/
	EM_PVRREC_NOTIFY_STOP = 0,
	/*!
		¼��ģ��֪ͨ����ĸ���
	*/
	EM_PVRREC_NOTIFY_TYPE_COUNT
}CSPVRRECNotifyType_E;

typedef enum _CSPVRRECStopCase_E
{
	EM_PVRREC_STOPEDBY_UNKOWN = 0,
	EM_PVRREC_STOPEDBY_STORAGE_FULL,
	EM_PVRREC_STOPEDBY_STORAGE_REMOVED,
	EM_PVRREC_STOPEDBY_TIMEOUT,
	EM_PVRREC_STOPEDBY_IOERROR
}CSPVRRECStopCase_E;
/*!
	��ȡ��Ϣ֪ͨ�������ľ��
\return ��Ϣ֪ͨ�������ľ��
*/
HCSHANDLE CSPVRRECGetNotifyMgr(void);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

