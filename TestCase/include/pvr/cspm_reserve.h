/******************************************************************************
FileName:  cs_pvrmgrreserve.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVRԤ������ؽӿ�,��֧��Ԥ��¼��
ģ����PVRFRSV

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef CSPM_RESERVE_H
#define CSPM_RESERVE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "cspm_notifymgr.h"
#include "CSUSPCommonType.h"
#include "cspm_record.h"

enum{
	/*!������*/
	CSPVRRSV_WEEKDAY_7 = 0x01,
	/*!����һ*/
	CSPVRRSV_WEEKDAY_1 = 0x02,
	/*!���ڶ�*/
	CSPVRRSV_WEEKDAY_2 = 0x04,
	/*!������*/
	CSPVRRSV_WEEKDAY_3 = 0x08,
	/*!������*/
	CSPVRRSV_WEEKDAY_4 = 0x10,
	/*!������*/
	CSPVRRSV_WEEKDAY_5 = 0x20,
	/*!������*/
	CSPVRRSV_WEEKDAY_6 = 0x40
};
/*!
	Ԥ���ص�֪ͨö�����Ͷ���
*/
typedef enum _CSPVRFRSVNotifyType_E
{
	/*!
		Ԥ��¼�Ƽ�������֪ͨ
		\note	dwNotifyType:	EM_PVRRSV_BEFORE_START 
		\note	pNotifyData:	��������¼�Ƶ�ʱ��(��λΪ��)
		\note	hSender:Ԥ��¼�ƾ��
	*/
	EM_PVRRSV_BEFORE_START,
	/*!
		Ԥ��¼���ѳɹ�����֪ͨ
		\note	dwNotifyType:EM_PVRRSV_START_SUCCESS
		\note	pNotifyData:������¼�ƾ��,�����CSPVRRFStartRec���صľ��ʹ�÷���һ��
		\note	hSender:Ԥ��¼�ƾ��
	*/
	EM_PVRRSV_START_SUCCESS,
	/*!
		Ԥ��¼������ʧ��֪ͨ
		\note	dwNotifyType:EM_PVRRSV_START_FAILED
		\note	pNotifyData:����ʧ�ܵ�ԭ��
		\note	hSender:Ԥ��¼�ƾ��
	*/
	EM_PVRRSV_START_FAILED,
	/*!
		Ԥ��¼�Ƽ�������֪ͨ
		\note	dwNotifyType:EM_PVRRSV_BEFORE_STOP
		\note	pNotifyData:������¼�ƾ��,�����CSPVRRFStartRec���صľ��ʹ�÷���һ��
		\note	hSender:Ԥ��¼�ƾ��
	*/
	EM_PVRRSV_BEFORE_STOP,
	/*!
		Ԥ��¼������ڱ�ɾ����֪ͨ
		\note	dwNotifyType:EM_PVRRSV_AUTO_DEL
		\note	pNotifyData:Ԥ��¼�ƾ��
		\note	hSender:��
	*/
	EM_PVRRSV_OUTOF_DATE,
	/*!
		¼��ģ��֪ͨ����ĸ���
	*/
	EM_PVRRSV_NOTIFY_TYPE_COUNT
}CSPVRRSVNotifyType_E;

/*!
	��ȡ��Ϣ֪ͨ�������ľ��
\return ��Ϣ֪ͨ�������ľ��
*/
HCSHANDLE CSPVRRSVGetNotifyMgr(void);

/*!
	Ԥ��¼�ƽ����������ö�ٶ���
*/
typedef enum _CSPVRRSVRet_E
{
	/*!Ԥ��������*/
	EM_PVRSV_RET_FULL = 0,
	/*!Ԥ���ɹ�*/
	EM_PVRRSV_RET_SUCC,
	/*!Ԥ����ͻ*/
	EM_PVRRSV_RET_CONFLICT,
	/*!��Ԥ�����Ѵ���*/
	EM_PVRRSV_RET_HAS_EXIST,
	/*!Ԥ��ʧ��*/
	EM_PVRRSV_RET_FAILED,
}CSPVRRSVRet_E;

/*!
	Ԥ��¼�Ƶ��ظ�ģʽ
*/
typedef enum _CSPVRRSVMode_E
{
	/*����
		dwModeExtData��Ч*/
	EM_PVRRSV_MODE_ONCE =0,
	/*ÿ��
		dwModeExtData��Ч*/
	EM_PVRRSV_MODE_DAY,
	/*ÿ��
		dwModeExtData��־ÿ�ܵ���Щ������¼��
		ʹ�ó���CSPVRRSV_WEEKDAY_1......7�ֱ��ʾÿ�ܵ���Щ������¼��*/
	EM_PVRRSV_MODE_WEEK, 
}CSPVRRSVMode_E;

/*!
	Ԥ����������
*/
typedef struct _CSPVRRSVParam_S
{
	/*!Ԥ����ʼʱ��*/
	CSUDIPLUSOSSysTime_S 		sStartTime;
	/*!Ԥ������ʱ��*/
	CSUDIPLUSOSSysTime_S 		sEndTime;
	/*!Ԥ�����ظ�ģʽ*/
	CSPVRRSVMode_E 		eMode;
	/*!Ԥ���ظ�ģʽ����չ����,��Ԥ���ظ�ģʽö��˵��*/
	DWORD				dwModeExtData;
	/*!Ԥ��¼��Ŀ���ļ����û��Ѻ���*/
	char					wsUserName[CSPVR_NAME_SIZE];
}CSPVRRSVParam_S;

/*!
	��ȡԤ��������Ĭ��ֵ
@param[out] psParam Ĭ�ϲ���ֵ
\note ���ȵ��ñ�������ʼ��PVRRFRsvParam_S�ṹ��,
\note �ٶԸ���Ȥ������и�ֵ,�ٵ���CSPVRRRSVRecord����Ԥ��¼��
*/
void CSPVRRSVGetDefaultParam(CSPVRRSVParam_S *psParam);

/*!
	Ԥ��¼��
@param[in] szLocator ��ʶͷΪdtv:��Locator��ʽΪ "dtv:// OrigNetId.TsID.ServiceID"������:"dtv:45.36.119"
@param[in] psReserveParam Ԥ������
@param[out] phRsvHandle ���Ԥ��¼����ľ����
\return Ԥ�����,(��CSPVRRSVRet_E�Ķ���)
*/
CSPVRRSVRet_E CSPVRRSVRecord(const char *szLocator, const CSPVRRSVParam_S* psReserveParam, HCSHANDLE *phRsvHandle);

/*!
	Ԥ����ͻ�ж�
@param[in] szLocator ��ʶͷΪdtv:��Locator��ʽΪ "dtv:// OrigNetId.TsID.ServiceID"������:"dtv:45.36.119"
@param[in] psReserveParam Ԥ������
@param[out] phConfilctItems �����ͻ��Ԥ��¼������
@param[in] dwConfilctItemsArrayLen ���Ԥ��¼�����������С
\return ���ص�Ԥ����ͻ����
\note �����ͻ��Ԥ�����������dwRsvRecArrayLen���򷵻�dwRsvRecArrayLen
*/
DWORD CSPVRRSVGetConflict(const char *szLocator, const CSPVRRSVParam_S* psReserveParam, HCSHANDLE *phConfilctItems, DWORD dwConfilctItemsArrayLen);

/*!
	����Ƶ����ʱ����Ϣ��ȡԤ�����
@param[in] szLocator ��ʶͷΪdtv:��Locator��ʽΪ "dtv:// OrigNetId.TsID.ServiceID"
@param[in] psStartTime Ԥ���Ŀ�ʼʱ��
@param[out] psEndTime Ԥ���Ľ���ʱ��
\return Ԥ���������������ڸ�ʱ�εĸ�Ƶ��Ԥ�����򷵻�NULL
\note ���ҽ���Ԥ����Ŀ�ʼʱ��ͽ���ʱ�䶼��������ʱ�����ظ�Ԥ����
*/
HCSHANDLE CSPVRRSVGetHandleByInfo(const char *szLocator, const CSUDIPLUSOSSysTime_S* psStartTime,const CSUDIPLUSOSSysTime_S* psEndTime);

/*!
	��ȡԤ������
\return ����Ԥ���ĸ���
*/
int CSPVRRSVGetCount(void);

/*!
	��ȡԤ��������
@param[in] nIdx Ԥ��¼�Ƶ�����
\return ָ��������Ԥ������
*/
HCSHANDLE CSPVRRSVGetHandle(int nIdx);

/*!
	�޸�Ԥ����Ϣ
@param[in] hReserveHandle Ԥ�����
@param[in] psReserveParam ��Ԥ����Ϣ
\return �޸Ĳ����Ľ��
*/
CSPVRRSVRet_E CSPVRRSVModify(HCSHANDLE hReserveHandle, const CSPVRRSVParam_S* psReserveParam);

/*!
	��ȡԤ��¼����Ϣ
@param[in] hReserveHandle Ԥ�����
@param[out] psRsvInfo Ԥ����Ϣ
\return �ɹ��򷵻�TRUE,���򷵻�FALSE
*/
BOOL CSPVRRSVGetReserveInfo(HCSHANDLE hReserveHandle, CSPVRRSVParam_S *psReserveInfo);

/*!
	��ȡԤ����Ƶ��
@param[in] hReserveHandle Ԥ�����
@param[out] pdwTpId ���Ƶ����TPId
@param[out] pdwServiceId ���Ƶ����serviceId
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note 
*/
BOOL CSPVRRSVGetReserveChnl(HCSHANDLE hReserveHandle, DWORD *pdwTpId, DWORD *pdwServiceId);

/*!
	�ж�Ԥ���Ƿ��Ѿ�����
@param[in] hRsvHandle Ԥ�����
\return ��Ԥ��¼���Ƿ����ڽ���¼��
*/
BOOL CSPVRRSVIsWorking(HCSHANDLE hRsvHandle);

/*!
	ɾ��Ԥ��
@param[in] hRsvHandle Ԥ��¼�ƾ��
\return �ɹ��򷵻�TRUE,���򷵻�FALSE
\note ����¼�Ƶ�Ԥ�����޷�ɾ�����û�������ֹͣ¼�Ʋ���ɾ��
*/
BOOL CSPVRRSVDel(HCSHANDLE hRsvHandle);

/*!
	ȡ��Ԥ��
@param[in] hRsvHandle Ԥ�����
\return �ɹ��򷵻�TRUE,���򷵻�FALSE
\note ֻ��ȡ������������Ԥ��¼����ı���Ԥ��
\note ���ڽ���¼�Ƶ�Ԥ���޷�ȡ��
\note ÿ�ܻ�ÿ��Ԥ����ֻ��ȡ������Ԥ�����ڶ����Ԥ������������
*/
BOOL CSPVRRSVCancel(HCSHANDLE hRsvHandle);


/*!
	����Ԥ������ǰ֪ͨƫ��ʱ��
@param[in] nSeconds ƫ��ʱ�䣬��λΪ��
\return �ɹ��򷵻�TRUE,���򷵻�FALSE
*/
BOOL CSPVRRSVSetPrevStartNotifyTime(int nSeconds);

/*!
	��ȡԤ������ǰ֪ͨƫ��ʱ��
\return ƫ��ʱ�䣬��λΪ��
*/
int CSPVRRSVGetPrevStartNotifyTime();

/*!
	�ָ�Ĭ��ʱ���ã���pvrģ�����ò�����Ԥ�����ݻָ�����������
\return �ɹ��򷵻�TRUE,���򷵻�FALSE
*/
BOOL CSPVRRSVConfigWithDefault(void);

#ifdef __cplusplus
}
#endif

#endif

/* End of File */

