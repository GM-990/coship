/******************************************************************************
FileName:  cspm_config.h
Copyright (c) 2008�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        luojinhua (luojinhua@coship.com)

�ļ�����:
������PVR��ʼ���ӿ�,PVRϵͳ������Ϣ�Ļ�ȡ�����ýӿ�
ģ����PVR

�޸ļ�¼:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0         2008-01-16      �޽�           Create

*******************************************************************************/
#ifndef _CSPM_CONFIG_H
#define _CSPM_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_os.h"
#include "CSUSPCommonType.h"

/*!Pvrϵͳ֧�ֵ���ಥ�Ÿ���*/
#define CSPVR_MAX_PLAY_COUNT  8

/*!��Ч���豸����*/
#define CSPVR_INVALID_DEVICE_IDX -1

/*!Pvr��ĿĬ����󳤶�*/
#define CSPVR_DEFAULT_DATA_MAX_SIZE 0x800

/*!Ĭ��TShiftԤ���ռ��С*/
#define  CSPVR_DEFAULT_RESERVE_BLOCKS_FOR_TSHIFT (10*1024*1024)

/*!TShift����ģʽ*/
typedef enum _CSPVRTShiftMode_E
{
	/*!ÿ������ֱ��ʱ,�Զ�����TShift¼��*/
	EM_PVR_TSHIFTMODE_AWAYS=0,
	/*!ֱ������ͣʱ����TShift¼��*/
	EM_PVR_TSHIFTMODE_START_WHEN_PAUSE,	
	/*!��Զ��Ҫ����TShift*/
	EM_PVR_TSHIFTMODE_NEVER_START	
}CSPVRTShiftMode_E;

/*!PVRģ�����ò���*/
typedef struct _CSPVRConfig_S
{
	/*! Ĭ��¼��ʱ��*/
	DWORD				dwDefaultRecDurationInS;
	/*! ¼��ʱ�Ƿ����EPG�и�*/
	BOOL				bEnableEpgCutRec; 
	/*! TShift����ģʽ*/
	CSPVRTShiftMode_E		eTShiftMode; 
}CSPVRConfig_S;

/*�����豸*/
typedef struct _CSPVRPlayDevice_S
{
	/*!��Ƶ����������*/
	int nVideoDeviceIdx;
	/*!��Ƶ����������*/
	int nAudioDeviceIdx;
	/*!TTX����������*/
	int nTtxDeviceIdx;
	/*!Tshift¼������,�������Ҫ��·����TShift������ΪCSPVR_INVALID_DEVICE_IDX*/
	int nTShiftRecIdx;
	/*!�����������*/
	int nTvWinIdx;
}CSPVRPlayDevice_S;

/*!��ȡ��ǰ����ʱ��Ļص����Ͷ���*/
typedef void (*CSPVRGetSysTime_F)(CSUDIPLUSOSSysTime_S *pOutSysTime);

/*!
	��ȡʱ���Ļص����Ͷ���
@param[in] ��
\return ʱ������+8, -5��
*/
typedef int  (*CSPVRTimeZone_F)(void);


/*!
	����Pvr���ݵĵĻص����Ͷ���
@param[in] pData �豣���������ڴ��е���ʼ��ַ
@param[in] nDataLen �豣�����ݵĳ���
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note �뽫Pvr���ݱ��浽֧�ֵ��籣��Ĵ洢�豸��,���鱣�浽Flash
*/
typedef BOOL (*CSPVRSaveData_F)(PVOID pData,int nDataLen);

/*!
	����Pvr���ݵĻص����Ͷ���
@param[out] pData �������ݵ��ڴ�Ŀ����ʼ��ַ
@param[in] nDataLen ��Ҫ��������ݴ�С
\return �ɹ�����TRUE,ʧ�ܷ���FALSE
\note ������ʵ������CSPVRSaveData_F����ʵ�����,Pvr���ݱ��浽����,�ʹ����ﵼ������
*/
typedef BOOL (*CSPVRLoadData_F)(PVOID pData, int nDataLen);

/*!PVRģ���ʼ������*/
typedef struct _CSPVRInitParam_S
{
	/*!PvrӦ����Ҫ֧�ֵ�Pvr¼�Ƹ���������С�ڵ���ƽ̨֧�ֵĸ�����,*/
	DWORD					dwFileRecCount;
	/*!PvrӦ����Ҫ֧�ֵ�TShift¼�Ƹ���������С�ڵ���ƽ̨֧�ֵĸ�����*/
	DWORD					dwTShiftRecCount;
	/*!ϵͳ֧�ֵĲ��Ÿ�����������С�ڵ���ƽ̨֧�ֵĸ�����*/
	DWORD					dwPlayCount;
	/*!�������Ŷ�Ӧ���豸*/
	CSPVRPlayDevice_S		asPlayDevice[CSPVR_MAX_PLAY_COUNT];	
	/*!��ȡ��ǰ����ʱ��ķ���*/
	CSPVRGetSysTime_F		fnGetSysTime;
	/*!��ȡʱ���ķ���*/
    CSPVRTimeZone_F         fnGetTimeZone;
	/*!����Pvr���ݵķ���*/
	CSPVRSaveData_F			fnSaveData;
	/*!����Pvr���ݵĻص�����*/
	CSPVRLoadData_F			fnLoadData;	
	/*!Pvr�洢���ݵ����ֵ(��Ҫ����PVR���������Ԥ����Ϣ�洢)*/
	DWORD					dwMaxDataSize;
	/*!�Ƿ�֧������Ӳ��*/
	BOOL					bSupportEmbeddedHdd;
	/*!ΪTShift¼��Ԥ���ռ��(���СΪ512)����*/
	DWORD					dwReserveBlocksForTShift;
}CSPVRInitParam_S;
/*!
	��ȡĬ�ϲ���
@param[out] psParam ���Ĭ�ϲ���
\note ���ȵ��ñ�������ʼ��CSPVRPFChnlParam_S�ṹ��,
\note �ٶԸ���Ȥ������и�ֵ
\note �ٵ���CSPVRInit��ʼ��Pvrģ��
*/
void CSPVRGetDefaultParam(CSPVRInitParam_S *psParam);

/*!
	��ʼ��PVR ģ��
@param[in] psParam ��ʼ������
\return �Ƿ�ɹ���ʼ��
\note �������γ�ʼ��
\note ����ϵͳ��ʼ��ʱ��ʼ����ģ��,��ֹ�����й����г�ʼ����ģ��
*/
BOOL CSPVRInit(const CSPVRInitParam_S *pParam);

/*!
	��ȡ��ǰϵͳ������Ϣ
@param[in] psConfig �����ǰϵͳ������Ϣ
\return �Ƿ�ɹ���ȡ��ǰϵͳ������Ϣ
\note
*/
BOOL CSPVRGetConfig(CSPVRConfig_S *psConfig);

/*!
	����ϵͳ������Ϣ
@param[in] psConfig ��ϵͳ������Ϣ
\return �Ƿ�ɹ�����ϵͳ������Ϣ
\note ����ϵͳ������Ϣ���Ժ󣨰����ؿ��������󣩵�PVR���й����о���Ч
*/
BOOL CSPVRSetConfig(const CSPVRConfig_S *psConfig);

/*!
	��ȡPvrMgr ģ��汾��
@param[out] pcVersion ����汾��Ϣ�ַ�����ʽΪ "xx.xx.xx"
@param[in] nSize ������pcVersion�ĳ���
\return ����PVRģ��İ汾��
\note ��һ�ַ���ֵ��������û��ɲ��Ըú�������Ϊ�ڶ�������nSize�ṩ�ο���
����û�����ĵڶ�������С�ڰ汾��Ϣ��ʵ�ʳ��ȣ�β��������ֽڱ��ص�
*/
int CSPVRGetVersion( char * pcVersion, int nSize );


#ifdef __cplusplus
}
#endif

#endif

/* End of File */

