#ifndef sitime_H
#define sitime_H

#ifdef PRAGMA
#pragma interface "sitime.h"
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "CSUSPCommonType.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum _ENFLAG		//ʱ��ƫ�Ʊ�־
{
	ENFLAG_POSITIVE,		//������ʾ����ʱ��ƫ��
	ENFLAG_NEGATIVE			//������ʾ����ʱ��ƫ��
}enTimeOffsetFlag;

#define TDT 0				//����TDT��
#define TOT 1				//����TOT��

typedef struct
{
	long lTimeout;			//��ʱʱ�䣬-1��ʾ����ʱ
	DWORD dwDemuxID;		//demux�豸ID��
	float fTimeOffset;		//ʱ��ƫ����
	DWORD dwTable;			//ָ�������Ǹ�SI table��0 ��ʾ TDT ��1��ʾ TOT��������TOT��fTimeOffset�ᱻ����.
}SITM_InitParams;

/*
������ TimeNotifyCallback
	ʱ��֪ͨ�ص�����������si�����еõ��µ�ʱ�䣬���øú���֪ͨ�û�

	void(*TimeNotifyCallback)(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	

Parameters
	
	pTime
		[out]ʱ��ṹ��
	pDate
		[out]���ڽṹ��

Return Values
		��
*/
typedef void (*TimeNotifyCallback)(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);
/*
������ TimeNotifyCallbackEx
	ʱ��֪ͨ�ص�����������si�����еõ��µ�ʱ�䣬���øú���֪ͨ�û�

	void(*TimeNotifyCallback)(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate,
		float* pfTimeOffset,
		void* pUserData
	);	

Parameters
	
	pTime
		[out]ʱ��ṹ��
	pDate
		[out]���ڽṹ��
	pfTimeOffset
		[out]ʱ��ƫ��ֵ
	pUserData
		[out]�û�����

Return Values
		��
*/

typedef void (*TimeNotifyCallbackEx)(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate, float* pfTimeOffset,void* pUserData);

/*
������ SITM_Init
	��ʼ��SITIME����������������ǰ�ú��������ȵ��øú���,�ڵ��øú���֮ǰһ��Ҫ��ȷ��ʼ��SITM_InitParams�ṹ�塣

	void SITM_Init(
		SITM_InitParams* pInitParams
	);	

Parameters
	
	pInitParams
		[in] ��ʼ���ṹ��ָ��

Return Values

BOOL: ����TRUE����ʾ��ʼ���ɹ�
����FALSE����ʾ��ʼ��ʧ��
*/
BOOL SITM_Init(SITM_InitParams* pInitParams);


/*
������ SITM_Final
	SIʱ��ģ�����ٺ���,��������ģ��ע��Ļص��������ã�

	void SITM_Final();	

Parameters
	
	��

Return Values

BOOL�� TRUE��ʾע���ɹ���
FALSE��ʾע��ʧ�ܡ�
*/
BOOL SITM_Final();

/*
������ SITM_RequestCurrentTime
	����ǰʱ�䣬�첽���̣���ʱ�����ʱ��callback������ָ���ĺ����ᱻ���ã��ú���Ӧ�����ں���SITM_Init֮��

	void SITM_RequestCurrentTime(
		TimeNotifyCallback callback
	);	

Parameters
	
	callback
		[in] TimeNotifyCallback�ص�

Return Values

int�� -1����ʾע��ʧ�ܣ�
����ֵ���ڵ���0����ʾע��Ļص������
*/
int SITM_RequestCurrentTime(TimeNotifyCallback callback);
/*
������ SITM_RequestCurrentTimeEx
	����ǰʱ�䣬�첽���̣���ʱ�����ʱ��callback������ָ���ĺ����ᱻ���ã��ú���Ӧ�����ں���SITM_Init֮��

	void SITM_RequestCurrentTimeEx(
		TimeNotifyCallback callback,
		void * pvUserData
	);	

Parameters
	
	callback
		�û���ȡʱ��Ļص���
	pvUserData
		�û����ݡ�ʵ��������һ��BYTE�ַ��������У��ֽ�0��1��2��countryCode����'C','H','I',�ֽ�3��regionId,��2.�����Ȳ��ޣ���������ֽڱ����ԡ�	

Return Values

int�� -1����ʾע��ʧ�ܣ�
����ֵ���ڵ���0����ʾע��Ļص������
*/

int SITM_RequestCurrentTimeEx(TimeNotifyCallbackEx callback,void* pvUserData);
//�ú���ָ��SITIME����ʱ��ʱ������ʱ��ƫ����Ϣ����ʱ��ƫ����Ϊ0
int SITM_RequestCurrentTimeWithoutTimeoffset(TimeNotifyCallbackEx callback,void* pUserData);

/*
������ SITM_Cancel
	ע���ص�

	void SITM_Cancel(
		int handle
	);	

Parameters
	
	handle
		[in]ע��Ļص������

Return Values

int�� -1����ʾע��ʧ�ܣ�
����ֵ���ڵ���0����ʾע��Ļص������
*/
void SITM_Cancel(int handle);


/*
������ SITM_GetNearTime
	��ȡ���Ƶĵ�ǰʱ�䣬ͬ�����������ú����Ϸ��أ�����ֵΪ������µĵ�ǰʱ��

	void SITM_GetNearTime(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	

Parameters
	
	pTime
		[out]ʱ��ṹ��ָ��
	pDate
		[out]���ڽṹ��ָ��

Return Values

BOOL�� TRUE��ʾ���ò��Գɹ���
FALSE��ʾ���ò���ʧ�ܡ�

*/
BOOL SITM_GetNearTime(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);

/*
������ SITM_GetNearTimeEx
	��ȡ���Ƶĵ�ǰʱ�䣬ͬ�����������ú����Ϸ��أ�����ֵΪ������µĵ�ǰʱ��

	void SITM_GetNearTime(
		CSUDIPLUSOSTIME_S* pTime,
		CSUDIPLUSOSDATE_S* pDate
	);	
	��SITM_GetNearTime�Ĳ�֮ͬ���������������ϵͳʱ��������������SITM_GetNearTime����ȷЩ

Parameters
	
	pTime
		[out]ʱ��ṹ��ָ��
	pDate
		[out]���ڽṹ��ָ��

Return Values

BOOL�� TRUE��ʾ���ò��Գɹ���
FALSE��ʾ���ò���ʧ�ܡ�

*/
BOOL SITM_GetNearTimeEx(CSUDIPLUSOSTIME_S* pTime, CSUDIPLUSOSDATE_S* pDate);

/*
������ SITM_GetTimeOffset
	�õ���ǰʱ���ƫ���������������TOT����ƫ����ΪTOT�й涨��ƫ����������Ϊ�û���ʼ�����ݵ�ƫ����

	void SITM_GetTimeOffset(
		float* pfTimeOffset
	);	

Parameters
	
	pfTimeOffset
		[out]ʱ��ƫ��ֵ

Return Values

  BOOL�� TRUE��ʾ�ɹ���
FALSE��ʾʧ��

*/
BOOL SITM_GetTimeOffset(float* pfTimeOffset);


/*
������ SITM_SetTimeOffset
	����ʱ��ƫ��

	void SITM_SetTimeOffset(
		enTimeOffsetFlag enFlag,
		CSUDIPLUSOSTIME_S* pTimeOffset
	);	

Parameters
	
	enFlag
		[in]ʱ��ƫ�Ʊ�־������ʾ��ʱ��ƫ����������ʾ��ʱ��ƫ������
	pTimeOffset
		[in]ʱ��ƫ������

Return Values

BOOL�� TRUE��ʾ���óɹ���
FALSE��ʾ����ʧ��

*/
BOOL SITM_SetTimeOffset(enTimeOffsetFlag enFlag, CSUDIPLUSOSTIME_S* pTimeOffset);


/*
������ SITM_TimeConvert
	ʱ��ת��������ǰʱ���ۼ�ʱ��ƫ��ֵ

	void SITM_TimeConvert(
		CSUDIPLUSOSDATE_S* pDate,
		TTIME* pTime,
		float fOffsetHour
	);	

Parameters
	
	pDate
		[in/out]���ڽṹ�塣
	pTime
		[in/out]ʱ��ṹ�塣
	fOffsetHour
		[in]ʱ��ƫ������

Return Values

��

*/
void SITM_TimeConvert(CSUDIPLUSOSDATE_S* pDate,CSUDIPLUSOSTIME_S* pTime,float fOffsetHour);


/*
������ SITM_MJDConvert
	ʱ��ת��������ǰʱ�����ʱ��ƫ����

	void SITM_MJDConvert(
		int nMJD,
		int nUTC,
		float fOffsetHour
		CSUDIPLUSOSDATE_S* pDate,
		TTIME* pTime,	
	);	

Parameters
	
	nMJD
		[in]��ǰ����
	nUTC
		[in]��ǰʱ��
	fOffsetHour
		[in]ʱ��ƫ������
	pDate
		[out]ת����ļ���ʱ��ƫ����������
	pTime
		[out]ת����ļ���ʱ��ƫ������ʱ��

Return Values

��

*/
void SITM_MJDConvert(int nMJD,int nUTC,float fOffsetHour,CSUDIPLUSOSDATE_S* pDate,CSUDIPLUSOSTIME_S* pTime );

/*
������ SITM_Start
����ʱ�����������¿�ʼ����TDT����TOT��
SITM_Init֮��ʱ���������Զ������ģ�������ñ��ӿ�
���ӿڽ���SITM_Stop֮�����ڻָ�ʱ������
���ӿڵĵ��ò���Ӱ��ص�������ע�ᣬ��SITM_Stop�󣬻ص�����������ֱ���ٴε���SITM_StartΪֹ
���ӿڽ�ΪTDT���TOT��������ƵPID��ͻʱ����ͣTDT��TOT���������ͷ���Դ�����ã�һ�����������ʹ��
*/
BOOL SITM_Start(void);

/*
������ SITM_Stop
ֹͣʱ����������ֹͣ����TDT����TOT��
���ӿڵĵ��ò���Ӱ��ص�������ע�ᣬ��SITM_Stop�󣬻ص�����������ֱ���ٴε���SITM_StartΪֹ
���ӿڽ�ΪTDT���TOT��������ƵPID��ͻʱ����ͣTDT��TOT���������ͷ���Դ�����ã�һ�����������ʹ��
*/
BOOL SITM_Stop(void);

/**
@brief �������������ֹͣ����TDT��Ȼ�����½��� TDT 

@param[in] pInitParams:��������ṹ��
@return TRUE :������������TDT�ɹ���FALSE:������������TDTʧ��
@note 
*/

BOOL SITM_Restart(SITM_InitParams* pInitParams);
#ifdef __cplusplus
}
#endif


#endif /* sitime_H */
