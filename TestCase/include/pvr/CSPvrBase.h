#ifndef _CS_PVRBASE_H_
#define _CS_PVRBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmcp_typedef.h"
//#include <CSUSPCommonType.h>
#include <mmcp_os.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define PVR_INVALID_SECOND_LENGTH (-1)

#define PVR_INVALID_PID (0x1fff)
#define PVR_MAX_PIDS_COUNT 16
#define PVR_PIDTYPE(PID) (PID>>13)
#define PVR_PIDVALUE(PID) (PID & 0x1fff)
#define PVR_COMPOSE_PID(PID, TYPE) ((WORD) ((PID & 0x1fff) | (TYPE << 13)) )
#define PVR_PID_IS_VALID(PID) ( (PVR_PIDVALUE(PID)!=0) && (PVR_PIDVALUE(PID)!=0x1fff) ) /*PAT�����ô˺�*/
#define PVR_PID_NOT_VALID(PID) (!PVR_PID_IS_VALID(PID)) /*PAT�����ô˺�*/

#define PVR_MAX_CRYPT_KEY_SIZE (128)

#define PVR_AUD_PRE 0x0100
#define PVR_AUD_MP1 0
#define PVR_AUD_MP2 1
#define PVR_AUD_MP4 2
#define PVR_AUD_MP3  3
#define PVR_AUD_AC3 4
#define PVR_AUD_HE_AAC  5
#define PVR_AUD_MPG_AAC 6     /*Mpeg_AAC*/
#define PVR_AUD_AC3_PLUS 7  

#define PVR_VID_PRE 0x0200
#define PVR_VID_MP1 1
#define PVR_VID_MP2 0
#define PVR_VID_MP4 2
#define PVR_VID_H264 3


#define AUD_TYPE_MP1 (PVR_AUD_PRE | PVR_AUD_MP1)
#define AUD_TYPE_MP2 (PVR_AUD_PRE | PVR_AUD_MP2)
#define AUD_TYPE_MP3 (PVR_AUD_PRE | PVR_AUD_MP3)
#define AUD_TYPE_MP4 (PVR_AUD_PRE | PVR_AUD_MP4)
#define AUD_TYPE_AC3 (PVR_AUD_PRE | PVR_AUD_AC3)
#define AUD_TYPE_HE_AAC (PVR_AUD_PRE | PVR_AUD_HE_AAC)
#define AUD_TYPE_MPG_AAC (PVR_AUD_PRE | PVR_AUD_MPG_AAC) 
#define AUD_TYPE_AC3_PLUS (PVR_AUD_PRE | PVR_AUD_AC3_PLUS) 


#define VID_TYPE_MP1 (PVR_VID_PRE | PVR_VID_MP1)
#define VID_TYPE_MP2 (PVR_VID_PRE | PVR_VID_MP2)
#define VID_TYPE_MP4 (PVR_VID_PRE | PVR_VID_MP4)
#define VID_TYPE_H264 (PVR_VID_PRE | PVR_VID_H264)

#define PVR_OTHER_PRE 0x0300
#define PCR_TYPE (PVR_OTHER_PRE | 1)
#define TTX_TYPE (PVR_OTHER_PRE | 2)
#define SUB_TYPE (PVR_OTHER_PRE | 3)
#define CC_TYPE  (PVR_OTHER_PRE | 4)

#define PVR_TABLE_PRE 0x0400
#define PAT_TYPE (PVR_TABLE_PRE | 1)
#define CAT_TYPE (PVR_TABLE_PRE | 2)
#define PMT_TYPE (PVR_TABLE_PRE | 3)
#define SDT_TYPE (PVR_TABLE_PRE | 4)
#define EIT_TYPE (PVR_TABLE_PRE | 5)
#define NIT_TYPE (PVR_TABLE_PRE | 6)

#define NOT_SUPPORTED_TYPE 0xFFFF    

typedef struct 
{
	WORD awPid[PVR_MAX_PIDS_COUNT];
	WORD awType[PVR_MAX_PIDS_COUNT];
	BYTE ucCount;
}SPvrBasePidData;



typedef HCSHANDLE (*CSOnPvrBaseOpenFile_F)(const char *szFileName);
typedef int (*CSOnPvrBaseWriteFile_F)(HCSHANDLE hFile, BYTE *pbuf, int nBufSize);
typedef int (*CSOnPvrBaseReadFile_F)(HCSHANDLE hFile, BYTE *pbuf, int nBufSize);
typedef BOOL (*CSOnPvrBaseCloseFile_F)(HCSHANDLE hFile);

typedef struct
{
	CSOnPvrBaseOpenFile_F m_fnOnOpenFile;
	CSOnPvrBaseWriteFile_F m_fnOnWriteFile;
	CSOnPvrBaseReadFile_F m_fnOnReadFile;
	CSOnPvrBaseCloseFile_F m_fnOnCloseFile;
}CSPvrBaseFileIo_S; /*added by wanglin, 2007.9.26 */
/*removed by luojinhua 2006-12-15
typedef int(*FPvrBaseNotify)(void *pUserData);*/

typedef struct _CSPvrBaseInitParam_S
{
	int nMaxRecordCount;
	int nMaxPlayCount;
}CSPvrBaseInitParam_S;

/*!
����������
	��ʼ����

������
	��

����ֵ
	�ɹ���ʼ������TRUE��
	���򷵻�FALSE��

��ע��
	�������������Record��Playģ���ʼ��֮ǰ������
*/
BOOL CSPvrBase_Init(const CSPvrBaseInitParam_S* sInitParam);

/*!
����������
	����Tuner��

������
	nDemux
		[in]	Demux��������
	nTuner
		[in]	Tuner��

����ֵ
	�ɹ�����TRUE��
	���򷵻�FALSE��

��ע��
	��
*/
BOOL CSPvrBase_ConnectDemuxAndTuner(const int nDemux, const int nTuner);

/*!
����������
	��ȡ�Ƿ��ѳ�ʼ����

������
	��

����ֵ
	�Ƿ��ѳ�ʼ��

��ע��
	��
*/
BOOL CSPvrBase_HaveInited(void);


/*[[[add by luojinhua 2007-08-04*/
/*!
����������
	��ȡ����TSע���Demux������

������
	��

����ֵ
	<0   , ��ʾʧ��
	>= 0, ��������TSע���Demux����

��ע��
	����SWTS Demux,����ͨ��DMA�ķ�ʽ���ڴ滺����������ע�뵽Demux�С�
	�˽ӿڱ������Ժ��ȡע��Demux����RM�л�ȡ��
*/
int CSPvrBase_GetSWTSDemuxIdx(void);



/*!
����������
	ע������PVR���ļ�������
	
������
	sFileIo : �����ļ������ĺ������Ͻṹ��

����ֵ
	��

��ע��
	�û������Լ�ʵ���ļ��Ĳ������Ա����洢���ʡ�

*/
void CSPvrBase_SetIpPumpFileIo(CSPvrBaseFileIo_S sFileIo);


/*!
����������
	����PVRģ��ȱʡ�ļ���key
	
������
	pucCryptKey : ����key���׵�ַ
	uKeySize      :  key�ĳ���

����ֵ
	�ɹ�����TRUE��ʧ�ܷ���FALSE

��ע��
	��
*/
BOOL CSPvrBase_SetDefaultCryptKey(const BYTE * pucCryptKey, UINT uKeySize);

/*!
����������
	��ȡPVRģ��ȱʡ�ļ���key
	
������
	pucCryptKey : ���������׵�ַ
	uKeySize      :  Ҫ���ȡkey�ĳ���

����ֵ
	ʵ�ʷ��ص�key�ĳ���

��ע��
	��
*/
UINT CSPvrBase_GetDefaultCryptKet(BYTE * pucCryptKey, UINT uKeySize);

/*!
����������
	�򿪻�ر�PvrEngineģ���Trace��ӡ��Ϣ
	
������
	bEnable  : �����Ƿ��PvrEngine��Trace��Ϣ��
			  TRUE��򿪣�FALSE��رա�
	
����ֵ
	��

��ע��
	��
*/
void CSPvrBase_EnableTraceInfo(BOOL bEnable);

/*!
����������
	ע���ȡ�豸·��(path) �Ļص�����
	
������
	fnCb  : ��ȡ�豸·���Ļص�����
	pUserData : �û��Զ�������
	
����ֵ
	��

��ע��
	��
*/
typedef HCSHANDLE (*PvrBaseGetPath_F)(int nWorkId,BOOL bIsPlay,int nDemux, void *pUserData);
void CSPvrBase_RegGetPathCb(PvrBaseGetPath_F fnCb, void *pUserData);


/*!
����������
	ע��ת��·���Ļص�����
	
������
	fnCb  : ת��·���Ļص�����
	
����ֵ
	��

��ע��
	pvrEngine�ĵ����ߴ��ݸ���pvrEngine��·��ͨ��pvrEngine����ʶ��
	pvrEngineʹ�õ���������ʶ���·���������Ҫһ��ת��
*/
typedef int (*PvrPathConvert_F)(const char * szCsfsPath,int nDriverNameLen,char *szDriverPath);
void CSPvrBase_RegPathConvertCb(PvrPathConvert_F fnCb);
/*!
����������
	��ȡpvrEngine�İ汾��Ϣ
	
������
	��
	
����ֵ
	pvrEngine�İ汾��Ϣ

��ע��
	��
*/
char * CSPvrBase_GetVersionInfo(void);
#ifdef __cplusplus
}
#endif

#endif
