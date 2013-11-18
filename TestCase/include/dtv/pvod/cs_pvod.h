/*************************************************************************
* Copyright (c) 2009, ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
*
* File: cs_pvod.h
* File Identify:
* Summary: PVOD�ӿڶ���
*
* Current Version: 1.0
* Author:
* date: 2009-06-05
*
* history:
/-------------------------------------------------------------------------/
revision             author                 reason            date

/-------------------------------------------------------------------------/
**************************************************************************/

/**
@defgroup cs_pvod PVOD�ն˴���ӿ�API
@{
*/

/**
@file cs_pvod.h
*/

#ifndef _CS_PVOD_H_
#define _CS_PVOD_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"{
#endif

/** @brief PVOD�ļ�·����󳤶� */
#define CSPVOD_PATH_MAX (255)

/** @brief PVOD��PMT��ʹ�õ�stream_type���� */
typedef enum _CSPVOD_StreamType_E
{
    EM_CSPVOD_StreamType = 0x06    ///< private_stream_2: ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
}CSPvodStreamType_E;

/** @brief PVOD��SDT��ʹ�õ�service_type���� */
typedef enum _CSPVOD_ServiceType_E
{
    EM_CSPVOD_ServiceType = 0xF0   ///< �Զ����PVOD��ҵ������
}CSPvodServiceType_E;

/** @brief PVOD����״̬���� */
typedef enum _CSPvodState_E
{
    EM_CSPVOD_STATE_UNUSED = 0,     ///< δʹ��
    EM_CSPVOD_STATE_CREATED,        ///< ����
    EM_CSPVOD_STATE_INITED,         ///< �ѳ�ʼ��
    EM_CSPVOD_STATE_LISTENING,      ///< ������
    EM_CSPVOD_STATE_UPDATING,       ///< ������
    EM_CSPVOD_STATE_STOPPED        ///< ��ֹͣ
}CSPvodState_E;

typedef enum _CSPvodUpdateProgressMsg_E
{
	EM_CSPVOD_MSG_UpdateProgress = 0x9898	///< ���½�����Ϣ
}CSPvodMsg_E;
/** @brief PVOD�������Ͷ��嶨�� */
typedef enum _CSPvodError_E
{
    EM_CSPVOD_ERR_NONE = 0,         ///< �޴���
    EM_CSPVOD_ERR_DEMUX,            ///< Demux����, �޷�����Demux(���������������ͷ�)
    EM_CSPVOD_ERR_DEMUX_TIMEOUT,    ///< Demux����, ����Demux��ʱ
    EM_CSPVOD_ERR_FORMAT_PES,       ///< PES��ʽ����
    EM_CSPVOD_ERR_FORMAT_PVOD,      ///< PVOD��ʽ����
    EM_CSPVOD_ERR_DSC,              ///< ���������ļ�����

    EM_CSPVOD_ERR_DISK_NOSPACE,     ///< ���̿ռ䲻��

    EM_CSPVOD_ERR_FS_ACCESS,        ///< �ļ���Ŀ¼����ʧ��
    EM_CSPVOD_ERR_FS_CREATE,        ///< �����ļ���Ŀ¼ʧ��
    EM_CSPVOD_ERR_FS_DELETE,        ///< ɾ���ļ���Ŀ¼ʧ��
    EM_CSPVOD_ERR_FS_READ,          ///< �ļ���Ŀ¼��ȡʧ��
    EM_CSPVOD_ERR_FS_WRITE         ///< �ļ���Ŀ¼д��ʧ��
}CSPvodError_E;

/** @brief PVOD�ն�ģ��Ļص��¼�����, ��ת���¼����ͻ���Ӧ���Լ����߳�����������@see CSPvod_RegisterCallback() */
typedef enum _CSPvodEventType
{
    EM_CSPVOD_EVENT_PROC_START = 0, ///< �ɹ�����Demux, ���ɹ�ʱ�򷵻�EM_CSPVOD_EVENT_ERROR�¼�, ����ΪEM_CSPVOD_ERR_DEMUX
    EM_CSPVOD_EVENT_VERSION_CHANGE, ///< ��⵽�汾����
    EM_CSPVOD_EVENT_UPDATE_SUCCESS, ///< �汾�������
    EM_CSPVOD_EVENT_ERROR,          ///< �����¼�
    EM_CSPVOD_EVENT_TIMEOUT         ///< ��ʱ
}CSPvodEventType_E;

/** @brief PVOD�ն�ģ����¼�����,  ��ת���¼����ͻ���Ӧ���Լ����߳�����������@see CSPvod_RegisterCallback()*/
typedef struct
{
    CSPvodEventType_E eEventType;   ///< �¼�����
    CSPvodError_E eErrType;         ///< �¼�ΪEM_PVOD_UPDATE_ERRORʱָʾ�������
    void* pvData;                   ///< �������������ָ��, һ��ֻ���demux�ķ��ص�����
    int nDatalen;                   ///< ������������ݵĳ���
}CSPvodEvent_S;

/** @brief CSPvod_Create�Ĳ���@see CSPvod_Create() */
typedef struct
{
    char* pcRootPath;   ///< PVOD���ݴ洢��Ŀ¼
}CSPvodInstanParam_S;

/** @brief CSPvod_Start�Ĳ���@see CSPvod_Start() */
typedef struct
{
    DWORD dwDemuxId; ///< demux���, ��demux/��tuner�Ļ�����ͨ��Ϊ0
    WORD wPesPid;    ///< ����PVOD���ݵ�PES����PID
}CSPvodStartParam_S;

/**
@brief PVODģ���¼��ص�����
@param[in] psEvent �������, �¼�����
@param[in] pUserData �������, �û�ע��ص�ʱ���Զ������
@param[in] hPvodHandle �������, �¼��ķ����߾��(CSPvod_Create�ķ���ֵ)
@note �����ڱ��ص���ִ�й������, �����׳��¼��������̴߳���
@see CSPvod_RegisterCallback
*/
typedef void (*CSOnPvodCallback_F)(CSPvodEvent_S* psEvent, void* pvUserData, HCSHANDLE hPvodHandle);
typedef void (*CSOnPvodUpdateCallBack_F)(void* pvUserData);

/**
@brief PVODģ��ĳ�ʼ��
@return ����ģ���ʼ���Ľ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@note ������CSPvod_Create֮ǰ����, ��ֻ�ܱ�����һ��, ���õڶ���ʱ������
@post PVODģ�����ʹ�ú�ɵ��� ::CSPvod_ModuleTerminate() �ͷ���Դ
@see CSPvod_ModuleTerminate()
*/
BOOL CSPvod_ModuleInit(void);

/**
@brief PVODģ�����ֹ, �ͷ��� ::CSPvod_ModuleInit()�������Դ
@return ����ģ������Ľ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@note PVODģ�����, ������Դ, ��δ����::CSPvod_ModuleInit()֮ǰҲ��ʹ��
@see CSPvod_ModuleInit()
*/
BOOL CSPvod_ModuleTerminate(void);

/**
@brief ����һ��PVOD����ʵ��, �ɴ������PVODʵ��

����PVODʵ����, ʹ�� ::CSPvod_Destroy()����ʵ��
@param[in] psInstanParam �������, ����ʱ�������
@return ���ش���PVODʵ���Ľ��
@retval һ��PVODʵ���ľ�� �ɹ�
@retval NULL ʧ��
@note PVODģ��֧�ֶ�ʵ��
@code

HCSHANDLE hPvodHandle = NULL;
CSPvodInstanParam_S sInstanParam;
sInstanParam.pcRootPath = "/pvod_root";//linux��ʽ
hPvodHandle = CSPvod_Create(&sInstanParam);

@endcode
@pre @e psInstanParam ��ΪNULL
@see CSPvod_Destroy(), CSPvod_Start(), CSPvod_Stop()
*/
HCSHANDLE CSPvod_Create(CSPvodInstanParam_S* psInstanParam);

/**
@brief ����һ��PVODʵ��
@param[in] hPvodHandle �������, ::CSPvod_Create()��������PVODʵ�����
@return ��������PVODʵ���Ľ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@pre @e hPvodHandle ��ΪNULL, Ϊ::CSPvod_Create() ����ֵ
@see CSPvod_Create(), CSPvod_Start(), CSPvod_Stop()
*/
BOOL CSPvod_Destroy(HCSHANDLE hPvodHandle);

/**
@brief ����PVOD�ص��¼�������
@param[in] hPvodHandle �������,  ::CSPvod_Create() ��������PVODʵ���ľ��
@param[in] fnCallback �������, �ͻ��˵Ļص�����
@param[in] pvUserData �������, �Զ�������, �ڻص�������ʹ��
@return ����ע��ص��������Ľ��
@note ��::CSPvod_Create()����֮��, ::CSPvod_Start()����֮ǰ����, �ص�����fnCallback����Ϊ��, �Զ�������pvUserData��Ϊ��
@code

static void PvodCallback(CSPvodEvent_S* psEvent, void* pvUserData, HCSHANDLE hPvodHandle);//��������
BOOL bRet = FALSE;
bRet = CSPvod_RegisterCallback(hPvodHandle, PvodCallback, pvUserData);

@endcode
@retval TRUE �ɹ�
@retval FALSE ʧ��
@pre @e hPvodHandle ��ΪNULL, Ϊ::CSPvod_Create() ����ֵ
@pre @e fnCallback ��ΪNULL
*/
BOOL CSPvod_RegisterCallback(HCSHANDLE hPvodHandle, CSOnPvodCallback_F fnCallback, PVOID pvUserData);

/**
@brief ��ʼPVOD����ʹ��Demux��������, ��ʼPVOD���ݵĽ��ջ����

����Ҫֹͣ����ʱ, ʹ�� ::CSPvod_Stop() �ӿ�
@param[in] hPvodHandle �������, ::CSPvod_Create() ������PVODʵ���ľ��
@param[in] psStartParam �������, ��ʼPVOD����ʱ�Ĳ���
@return ���ؿ�ʼPVODʵ�����еĽ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@note
- ÿ�ε��ñ�API, ���   ::EM_CSPVOD_EVENT_PROC_START
- �����г�����        ::EM_CSPVOD_EVENT_ERROR
- ��⵽�汾���·���    ::EM_CSPVOD_EVENT_VERSION_CHANGE
- ĳ���汾���³ɹ�����  ::EM_CSPVOD_EVENT_UPDATE_SUCCESS
@code

BOOL bRet = FALSE;
CSPvodStartParam_S sStartParam;
sStartParam.dwDemuxId = 0;
sStartParam.wPesPid = 0x1DEF;
bRet = CSPvod_Start(hPvodHandle, &sStart);

@endcode
@pre @e hPvodHandle ��ΪNULL, Ϊ::CSPvod_Create() ����ֵ
@pre @e psStartParam ��ΪNULL
@see CSPvod_Create(), CSPvod_Destroy(), CSPvod_Stop()
*/
BOOL CSPvod_Start(HCSHANDLE hPvodHandle, CSPvodStartParam_S* psStartParam);

/**
@brief ֹͣPVOD����ȡ��Demux����������, ֹͣ ::CSPvod_Start()�ӿڿ�ʼ��PVOD����
@param[in] hPvodHandle �������, ::CSPvod_Create() ������PVODʵ�����
@return ����ֹͣPVODʵ�����еĽ��
@retval TRUE �ɹ�
@retval FALSE ʧ��
@note ǿ��ֹͣPVOD����, ���ڷ���������Ϣ���л�Ƶ�㡢���ݸ�����ɡ������趨�Ĺ���ʱ��ʱ����
@pre @e hPvodHandle ��ΪNULL, Ϊ::CSPvod_Create() ����ֵ
@see CSPvod_Create(), CSPvod_Destroy(), CSPvod_Start(),
*/
BOOL CSPvod_Stop(HCSHANDLE hPvodHandle);

/**
@brief ��ѯPVOD��״̬�ʹ���
@param[in] hPvodHandle �������, ::CSPvod_Create() ��������PVODʵ��
@param[out] peState �������, PVOD�Ĵ���״̬
@param[out] peError �������, PVOD�Ĵ���
@return ���ػ�ȡPVODʵ��״̬�ʹ���Ľ��
@retval TRUE �ɹ���ȡ
@retval FALSE ʧ��
@pre @e hPvodHandle ��ΪNULL
@pre @e peState ��ΪNULL
@pre @e peError ��ΪNULL
*/
BOOL CSPvod_GetStatus(HCSHANDLE hPvodHandle, CSPvodState_E* peState, CSPvodError_E* peError);

/**
@brief ��ȡPVOD�İ汾��
@param[out] pcBuf �ַ�����ʽ�汾�Ŵ�ŵ�ַ��ָ��
@param[in] nSize �ַ���Ż�������С
@return �����������Ȳ���ʱ, ����-1
@retval >0 �汾�ŵ��ַ�������
@retval -1 ʧ��
@pre @e pcBuf ��ΪNULL
@pre @e nSize ����0
@note ��������Сһ��ȡ32
*/
int CSPvod_GetVersion(char *pcBuf, int nSize);

/**
@brief ��ȡPVOD�Ƿ��и���
@param[in] hPvodHandle �������, ::CSPvod_Create() ��������PVODʵ��
@return ��ȡʧ��ʱ����-1��PVOD���ڸ�������ʱ����1��PVOD�����ڸ�������ʱ����0
@retval -1 ʧ��
@retval 0 �Ǹ�����
@retval 1 ������
*/
int CSPvod_IsUpdated(HCSHANDLE hPvodHandle);

/**
@brief ��ȡPVOD���½���
@param[in] hPvodHandle �������, ::CSPvod_Create() ��������PVODʵ��
@return ��ǰ���½���
@retval -1 ʧ��
@retval 0 ׼������
@retval ����0����С��100 ��ǰ���½���
@retval 100 �������
*/
int CSPvod_GetUpdateProgress(HCSHANDLE hPvodHandle);

/**
@brief ��ȡPVOD���½��Ȱ�����
@param[in] hPvodHandle �������, ::CSPvod_Create() ��������PVODʵ��
@return ���°�����
@retval -1 ʧ��
@retval 0 ����Ҫ����
@retval ���°�����
*/
int CSPvod_GetUpdatePacketNumber(HCSHANDLE hPvodHandle);

#ifdef __cplusplus
}
#endif

#endif /* _CS_PVOD_H_ */

/**
@}
*/

/* End of File */
