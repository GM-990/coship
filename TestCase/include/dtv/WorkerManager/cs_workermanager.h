/******************************************************************************
FileName:	cs_workermanager.h
Copyright (c) 2007�� Shen Zhen Coship Electronics CO.,LTD.��

Version:	Ver 1.0    
Author:		zhudengyu
Author:		pengyunjuan
File Description:
	����ϵͳ��ֲ���ResourceManager����Tuner��AV��Demux�ȿɹ������Դ����ͳһ�ĳ�ͻ��⡢
	������������ϲ�ģ�����Ϊ���������Դ��ռ��ͬʱ���ʹ�õķ����ԡ�

Modify History:
	Version		Date		Author		Description
	--------	----------	--------	------------
	1.0			2007-8-13	zhudengyu		Create
	1.0         2007-8-13   pengyunjuan     Create
	1.2			2008-1-30	zhaodemin	Modify
*******************************************************************************/


#ifndef _CS_WORKERMANAGER_H_
#define _CS_WORKERMANAGER_H_

#include "mmcp_typedef.h"
#include "mmcp_tuner.h"

#ifdef MMCP_UDI2
#include "udi2_rdi.h"
#else
#include "cs_rm.h"
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

/******data sturct*********/
typedef enum _CSWMRequestType_E
{
	EM_WM_TUNER,                                           //��Ƶ��
	EM_WM_TUNER_DMX,                                //��Ƶ������demux���ݣ�
	EM_WM_TUNER_DMX_REC,                     //��Ƶ������demux���ݣ�������¼��
	EM_WM_TUNER_DMX_PLAY,		//��Ƶ������demux���ݣ������в���
	EM_WM_DMX_DEC_AVOUT,                       //����IP��FILE���ݣ����ݸ�ʽΪTS
	EM_WM_DEC_AVOUT,                                   //����IP��FILE���ݣ����ݸ�ʽΪES/PES;
	EM_WM_DEC_AOUT_PCM,                                   //����PCM����
	EM_WM_TUNER_DMX_AOUT,		//��Ƶ������demux���ݣ���������Ƶ����
	EM_WM_DEC_VOUT                         //����I֡����
} CSWMRequestType_E;

/*
	������Դʱ���õ����ȼ�
*/
typedef enum _CSWMPRIORITY_E
{
	EM_WM_PRIORITY_1 = 1,
	EM_WM_PRIORITY_2 = 2,
	EM_WM_PRIORITY_3 = 3,
	EM_WM_PRIORITY_4 = 4,
	EM_WM_PRIORITY_5 = 5,
	EM_WM_PRIORITY_6 = 6,
	EM_WM_PRIORITY_7 = 7,
	EM_WM_PRIORITY_8 = 8,
	EM_WM_PRIORITY_9 = 9,
	EM_WM_PRIORITY_10 = 10,
	EM_WM_PRIORITY_11 = 11
}CSWMPRIORITY_E;

typedef struct _CSWMRequestInfo_S
{
	CSUDITunerSRCDeliver_U		unDeliver;		//�����TunerҪ������Ƶ��,�������ע����Ƶ�㣬����ȫ����0
	CSUDITunerType_E		emTunerType;			//Ҫʹ�� Tuner������
	DWORD dwTunerMask;		//�ɹ�ѡ���Tuner����
	DWORD dwServiceUniTag;		//����serviceid������ָ��demuxҪ�����serviceId����������Ĵ�ֵ����Ҫ��0
	CSWMPRIORITY_E         emRequestPriority;		//��������ȼ�
	int         nVIndex;                         //video decoder index����ֵҲ�Ǵ���index
} CSWMRequestInfo_S;

typedef enum _CSWMDeviceType_E
{
    EM_WM_DEVICE_TUNER, 	/**< ��Ƶͷ*/
    EM_WM_DEVICE_DEMUX,              /**< �⸴����*/
    EM_WM_DEVICE_VIDEO,              /**< ��Ƶ������*/
    EM_WM_DEVICE_AUDIO,              /**< ��Ƶ������*/
    EM_WM_DEVICE_UNDEF
}CSWMDeviceType_E;

typedef struct _CSWMDeviceInfo_S
{
    CSWMDeviceType_E eDeviceType;           //eDeviceType���豸������
    HCSHANDLE nDeviceHandle;                                          //nDeviceId���豸�ľ��
}CSWMDeviceInfo_S;

typedef enum _CSWMEvent_E
{
    EM_WM_EVENT_UNKNOWN = 0,    ///δ֪�豸�¼�,�Ƿ�ֵ
    EM_WM_EVENT_PLUGOUT = 1,     //�豸�Ƴ�,pEventDataΪCSWMDeviceInfo_S,����Ϣ֪ͨ���غ󽫲��ٿ�ͨ�����豸�������ȡ�豸��Ϣ
    EM_WM_EVENT_PLUGIN = 2,  //�豸����,pEventDataΪCSWMDeviceInfo_S
    EM_WM_EVENT_GRABBED = 3,     //�豸Ҫ�����������ȼ���������ռ��Ӧ���յ�����Ϣ����Ҫ�ڻص���ͬ���Ľ��Լ���path�ͷŵ�,pEventDataΪ�������豸��CSWMDeviceInfo_S
    EM_WM_EVENT_PATH_AFTER_CREATE = 10,//���µ�path�������ˣ�pEventDataֵΪ�´�����path�ľ��hDevicePath
    EM_WM_EVENT_PATH_AFTER_DESTROY = 20 //��path���ͷ��ˣ�pEventDataֵ��ʱ�޶��壬������ʹ��
}CSWMEvent_E;

/*****************************************************
�����豸֪ͨ����ԭ�Ρ�
�����豸״̬�����仯(���롢�γ���)ʱ,WordManagerģ��ͨ�������͵Ļص�����֪ͨ�û�
    hDevicePath����WorkManager�����뵽���豸��·�ľ��,��ֵ�����CSWMAddCallbackʱ�����hDevicePathֵ��ͬ
    eEvent: �¼�����,��ϸ�����CSWMEvent_E˵��
    pvEventData: ���¼����������ݣ���ϸ�����CSWMEvent_E˵��
    pvUserData: �û�����,����CSWMAddCallback_Fע��ûص�ʱ�����pvUserData
*****************************************************/
typedef void(*CSWMCallback_F)(HCSHANDLE hDevicePath, CSWMEvent_E eEvent, const void * pvEventData, void * pvUserData);

#define CS_WM_PRIORITY_HIGHEST         90          //��ʾ������ȼ�
#define CS_WM_PRIORITY_HIGHER         70          //��ʾ�ϸ����ȼ�
#define CS_WM_PRIORITY_NORMAL       50          //��ʾ�������ȼ�
#define CS_WM_PRIORITY_LOWER          30          //��ʾ�ϵ����ȼ�
#define CS_WM_PRIORITY_LOWEST          10          //��ʾ������ȼ�

/******************************************************************************
Function Name:    CSWMInitStyleSimple

Description:
    ��ʼ��WorkManager�Ĺ�����ʽΪ�򵥹���ģʽ
    WorkManager�ڴ�ģʽ��ʹ�����豸������
    ��ʱ���ٵ���WorkManager�������ӿڻ�ȡ�豸��ص�����ʱ��
    WorkManagerֻ�᷵��Ψһ��һ����Чpath,�������豸ID��Ϊ0

Input:

Output:

Return Value:
    �ɹ�����TRUE��ʧ�ܷ���FALSE��

Other:
    ע��һ�㣬��������˴˷������������ٵ���CSWMInit����
*******************************************************************************/
BOOL CSWMInitStyleSimple(void);

/******************************************************************************
Function Name:    CSWMInit

Description:
	��ʼ��WorkManagerģ�顣

Input:

Output:

Return Value:
	�ɹ�����TRUE��ʧ�ܷ���FALSE��

Other:
	�����ģ���Ѿ���ʼ������Ҳ����TRUE��
*******************************************************************************/
BOOL CSWMInit(void);

/******************************************************************************
Function Name:    CSWMGetDefaultRequestInfo

Description:
	��ʼ��CSWMRequestInfo_S�ṹ�塣

Input:
	psRequestInfo:�ⲿ������CSWMRequestInfo_S�ṹ��ʵ����ַ
Output:


Return Value:
	�ɹ�����TRUE��ʧ�ܷ���FALSE��

Other:

*******************************************************************************/
BOOL CSWMGetDefaultRequestInfo(CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMTryRequest

Description:
	�����������һ���豸����

Input:
	enmRequestType��������豸������;��
	psRequestInfo�������豸��ʱ����Ϣ��
Output:


Return Value:
	������������豸���򷵻�TRUE�����򷵻�FALSE��

Other:
*******************************************************************************/
BOOL CSWMTryRequest(CSWMRequestType_E enmRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMRequest

Description:
	�������һ���豸����

Input:
	enmRequestType��������豸������;��
	psRequestInfo�������豸��ʱ����Ϣ��

Return Value:
	������������豸���򷵻��豸���ľ�������򷵻�NULL��
	����������ΪEM_WM_TUNER��ʱ�򣬴��豸����������tuner
    ����������ΪEM_WM_TUNER_DMXʱ�����豸����������tuner+demux
    ����������ΪEM_WM_TUNER_DMX_RECʱ�����豸����������tuner+demux
    ����������ΪEM_WM_TUNER_DMX_PLAYʱ�����豸����������tuner+demux+a+v
    ����������ΪEM_WM_DEMUXʱ�����豸����ֻ����demux
    ����������ΪEM_WM_DMX_DEC_AVOUTʱ�����豸����������demux+a+v
    ����������ΪEM_WM_DEC_AVOUTʱ�����豸����������a+v


Other:
*******************************************************************************/
HCSHANDLE CSWMRequest(CSWMRequestType_E eRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMRequestByName

Description:
	�������һ���豸����

Input:
	enmRequestType��������豸������;��
	psRequestInfo�������豸��ʱ����Ϣ��
	pcModuleName:���ô˽ӿڵ�ģ������

Return Value:
	������������豸���򷵻��豸���ľ�������򷵻�NULL��
	����������ΪEM_WM_TUNER��ʱ�򣬴��豸����������tuner
    ����������ΪEM_WM_TUNER_DMXʱ�����豸����������tuner+demux
    ����������ΪEM_WM_TUNER_DMX_RECʱ�����豸����������tuner+demux
    ����������ΪEM_WM_TUNER_DMX_PLAYʱ�����豸����������tuner+demux+a+v
    ����������ΪEM_WM_DEMUXʱ�����豸����ֻ����demux
    ����������ΪEM_WM_DMX_DEC_AVOUTʱ�����豸����������demux+a+v
    ����������ΪEM_WM_DEC_AVOUTʱ�����豸����������a+v


Other:
*******************************************************************************/
/*ģ����                   ���ȼ�
   dtvmx.ads                   EM_WM_PRIORITY_6
   dtvmx.davic                 EM_WM_PRIORITY_7
   dtvmx.dtv                   EM_WM_PRIORITY_8/EM_WM_PRIORITY_10/EM_WM_PRIORITY_6
   mp                          EM_WM_PRIORITY_5
   mp.ffmpeg                   EM_WM_PRIORITY_5
   mp.sa                       EM_WM_PRIORITY_6
   cfg.nvod                    EM_WM_PRIORITY_6
   cfg.ota                     EM_WM_PRIORITY_6/EM_WM_PRIORITY_11/EM_WM_PRIORITY_10
   cfg.TuneMainFreq            EM_WM_PRIORITY_5
   cfg.citv                    EM_WM_PRIORITY_6
   shell.ota                   EM_WM_PRIORITY_6
   shell.ngodsa                EM_WM_PRIORITY_5 /EM_WM_PRIORITY_4
   kernel.occtrl               EM_WM_PRIORITY_9
   jsext                       EM_WM_PRIORITY_11
*/ 

HCSHANDLE CSWMRequestByName(const char * pcModuleName, CSWMRequestType_E eRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMGetRequestParam

Description:
	����path��ȡ�����pathʱ���������

Input:
        hDevicePath : �豸��·���

Output :
	peRequestType���豸������;��
	psRequestInfo���豸������Ϣ

Return Value:
	�����pathʱ���������

Other:
*******************************************************************************/
BOOL CSWMGetRequestParam(HCSHANDLE hDevicePath, CSWMRequestType_E *peRequestType, CSWMRequestInfo_S * psRequestInfo);

/******************************************************************************
Function Name:    CSWMCancel

Description:
	ֹͣ����һ���豸����ʹ�á�

Input:
	handle��Ҫͣ�����豸���ľ����
Output:


Return Value:
	�ɹ�����TRUE��ʧ�ܷ���FALSE��

Other:
*******************************************************************************/
BOOL CSWMCancel(HCSHANDLE hDevicePath);

/******************************************************************************
Function Name:    CSWMGetDeviceId

Description:
	����WorkManagerģ���е��豸��·��� ��ȡָ���豸���͵�ID
Input:
	hDevicePath����WorkManager�����뵽���豸��·�ľ��
	eDeviceType���豸����
Output:

Return Value:
	����ָ�����͵��豸��ID�����ָ�����͵��豸�д���·�в����ڣ��򷵻�-1
	�����tuner�豸���򷵻�tuner_id�������Demux����decoder���򷵻��豸������

Other:
*******************************************************************************/
int CSWMGetDeviceId(HCSHANDLE hDevicePath, CSWMDeviceType_E  eDeviceType);

/******************************************************************************
 * Function Name:    CSWMGetDeviceCount
 * 
 * Description:
 * ͨ���豸���ͻ�ȡWorkManager�м�¼���豸���ܸ���
 * 
 * Input:
 * emDeviceType: �豸����
 * 
 * Output:
 * ��
 * 
 * Return Value:
 * ����ָ�����豸������WorkManager�м�¼�ĸ���
 * 
 * Other:
*******************************************************************************/
int CSWMGetDeviceCount(CSWMDeviceType_E emDeviceType);

/******************************************************************************
Function Name:    CSWMAddCallback

Description:
    ����豸֪ͨ�ص�����
Input:
    hDevicePath: �˲����Ƽ���0�������¼����ᱻ֪ͨ�˻ص������������ָ�������path����ֻ����յ����path��ص��¼�
    fnCallback���ص�����,���豸״̬�����仯ʱ���øú���
    pvUserData���û�����,����Ϊ�ص�������pvUserData��������

Output:

Return Value:
    �ɹ�����TRUE��ʧ�ܷ���FALSE

Other:
    ��ӻص�ǰ�������豸�仯��Ϣ,���Ჹ��֪ͨ.��������ʱ��Щ�豸�ʹ���,���ñ�����ʱ����õ�֪ͨ
*******************************************************************************/
BOOL CSWMAddCallback(HCSHANDLE hDevicePath, CSWMCallback_F fnCallback, const void * pvUserData);

/******************************************************************************
Function Name:    CSWMRemoveCallback

Description:
    ɾ���豸֪ͨ�ص�����
Input:
    hDevicePath: ����CSWMAddCallbackע��ʱ�����hDevicePath
    fnCallback���ص�����,֮ǰע��ĺ���
    pvUserData���û�����,������ע��ʱ��һ��

Output:

Return Value:
    �ɹ�����TRUE��ʧ�ܷ���FALSE

Other:
    fnCallback��pvUserDataһ��Ψһȷ��һ���ص�����
*******************************************************************************/
BOOL CSWMRemoveCallback(HCSHANDLE hDevicePath, CSWMCallback_F fnCallback, const void * pvUserData);

#ifdef __cplusplus
}
#endif

#endif

