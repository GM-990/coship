/**@defgroup OS_PLUS OS Plus UDI OS ģ��Ĳ���ģ�飬�ṩ��ʱ����ʱ��Ƚӿ�
@brief ʹ��UDI OSģ��,��һЩ����,������Ҫƽ̨��ص�OS�ӿ�,���������װ

@note ��ģ��Ķ���Ĳ��ֽӿڿ��ܴ���Ч�ʻ��߾�������,�벻Ҫ�����������ϸ񾫶�,�綨ʱ����
@warning  ��ģ�����MMCP�ڲ�ʹ�ã���������ģ����κζ��塢�ӿ��ṩ��MMCP�����ģ��ʵ�ã�������������UDI�����ɽӿ���
@warning  ����������Ҫ��MMCP�ⲿ������ͷ�ļ��а�����ͷ�ļ�

@version 2.0.2 2009/09/16 ����UDI2.1�汾�Ķ��壬���µ�����ģ��
@version 2.0.1 2009/04/02 UDI2����
- ��ԭUDI OS�еĶ�ʱ��,ʱ�ӹ����Ƶ��˴�
@{
*/

#include "udi2_typedef.h"
#include "udi2_os.h"
#include "udi2_error.h"
#include "udiplus_error.h"

#ifndef _UDIPLUS_OS_H_
#define _UDIPLUS_OS_H_

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief OS Plus ģ����ش������ֵ*/
enum
{
	CSUDIOSPLUS_ERROR_BAD_PARAMETER = CSUDIPLUS_OS_ERROR_BASE,	///< ��������
	CSUDIOSPLUS_ERROR_FEATURE_NOT_SUPPORTED,					///< ��֧�ִ˲���
	CSUDIOSPLUS_ERROR_UNKNOWN_ERROR,							///< һ�����
	CSUDIOSPLUS_ERROR_NO_MEMORY,								///< ���ڴ���ô���
	CSUDIOSPLUS_ERROR_TIMEOUT,									///< ��ʱ����
	
	CSUDIOSPLUS_ERROR_MAX										///< �߽�ֵ
};

/**@brief ��ʱ��ѡ��*/
typedef enum
{
    EM_UDIPLUS_OS_TIMERMODE_REPEAT,			///< �ظ�ģʽѡ���ÿָ��ʱ�䴥��һ��
    EM_UDIPLUS_OS_TIMERMODE_ONESHOT,		///< ����ģʽѡ�������ָ��ʱ��󴥷�һ��
    EM_UDIPLUS_OS_TIMERMODE_MAX			    ///< ģʽѡ�������ǣ���Ч����
}CSUDIPLUSOSTimerMode_E;

typedef struct
{
	unsigned int m_nYear_13         :13;	///< ��,[1970,...]
	unsigned int m_nMonth_8         :8;		///< ��,[1, 12]��ע������Ķ�����C99��׼��ͬ
	unsigned int m_nDay_8           :8;		///< ��,[1, 31]
	unsigned int m_nWeekDay_3       :3;		///< days since Sunday,[0, 6]
}CSUDIPLUSOSDATE_S;

typedef struct
{
	unsigned int m_nHour_5         :5;		///< ʱ,[0, 23]
	unsigned int m_nMinute_6       :6;		///< ��,[0, 59]
	unsigned int m_nSecond_6       :6;		///< ��,[0, 59]
	unsigned int m_nMilliSeconds_15 :15;
}CSUDIPLUSOSTIME_S;

typedef struct
{
	CSUDIPLUSOSDATE_S m_sDate;
	CSUDIPLUSOSTIME_S m_sTime;
}CSUDIPLUSOSSysTime_S;

/**
@brief ��ʱ���ص�����

�½�һ����ʱ��ʱ,����һ�������͵ĺ�����Ϊ�ص�����,��ʱ��ÿ�δ���,�ú����ᱻ���á�
@param[in] pParam ��ڲ�������ֵΪ����CSUDI_Timer_Create����ʱ��pvArg������
@see CSUDI_Timer_Create
*/
typedef void (*CSUDIOSTimerCallback_F)(void* pParam);

typedef CSUDI_INT32	CSUDIPLUS_TIME_T;		///< ��ʾʱ��������ר�ö���,����ֱ�Ӽ����ֵ���ߴ�ӡ������Ҫ������λ��(Ŀǰ��32λ���������ܱ䳤)

/*******************************************************************
 **                     Clock definitions                         **
 *******************************************************************/
/**
@brief ����ϵͳʱ��

����ϵͳʱ��,֮����Զ���ʱ�Ա�֤ʱ����׼ȷ��
@param[in] nSeconds ��1970��1��1���賿�������ڵ�������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
*/
CSUDI_Error_Code CSUDIPlusOSClockSet(CSUDIPLUS_TIME_T nSeconds);

/**
@brief ��ȡ��ǰϵͳʱ��

һ������֮��,���Զ���ʼ��ʱ,��ʹ�ľ���������CSUDIOSGetTime�ľ���
@param[out] pnSeconds ��1970��1��1���賿�������ڵ�������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ��δ����ʱ��֮ǰ,��Ϊ����ʱ��Ϊ1970��1��1���賿
*/
CSUDI_Error_Code CSUDIPlusOSClockGet(CSUDIPLUS_TIME_T* pnSeconds);

/**
@brief ��ȡ��ϵͳ�����󾭹���ʱ�������

@return �ɹ�������ϵͳ�����󾭹���ʱ���������ʧ�ܷ���0
*/
unsigned int CSUDIPlusOSGetTickCount(void);

/**
@brief ��ϵͳʱ��ת��Ϊ����

��������ʱ������ʽ��ʱ��ת������1970��1��1���������ڵ�������
@param[in] psDateTime ����ʱ��,��������ʱ�����ʾ���ṹ����Week,m_MilliSeconds_8�ֶοɲ���д
@param[out] pnSeconds ���ʱ�䣬����1970��1��1���賿�������ڵ���������ʾ
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
*/  	
CSUDI_Error_Code CSUDIPlusOSSystime2Second(CSUDIPLUSOSSysTime_S* psDateTime,CSUDIPLUS_TIME_T* pnSeconds);

/**
@brief ������ת��Ϊϵͳʱ��

����1970��1��1���������ڵ�������ת����������ʱ������ʽ
@param[in] nSeconds ����ʱ�䣬����1970��1��1���������ڵ���������ʾ
@param[out] psDateTime ���ʱ��,��������ʱ�����ʾ������ṹ����m_MilliSeconds_8����0
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
*/ 
CSUDI_Error_Code CSUDIPlusOSSecond2Systime(CSUDIPLUS_TIME_T nSeconds,CSUDIPLUSOSSysTime_S* psDateTime);

/*******************************************************************
 **                     Timer definitions                         **
 *******************************************************************/
 /**
@brief ������ʱ��

@param[in] eMode ��ʱ������ģʽ,�꽫CSUDIPLUSOSTimerMode_E����
@param[in] pCallback ��ʱ�������Ļص���������ʱ������ʱ����ͬһ�������е��ô˺���
@param[in] pvArg ��ʱ���ص������Ĳ�����������pCallbackʱ�Ĳ���
@param[out] phTimer ��ʱ�����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ��ʱ���ľ���ԼΪ100ms������������ȵĶ�ʱ�����Ƿǳ���׼ȷ��
@note ���Զ�ʱ���ص���ͬһ�������з���������뾡��ʹ��ʱ���ص��������أ����򽫶�������ʱ������Ӱ��
@note ��ʱ���ص������У��Կ��Ե��ö�ʱ���������в���
*/
CSUDI_Error_Code CSUDIPlusOSTimerCreate(CSUDIPLUSOSTimerMode_E eMode, CSUDIOSTimerCallback_F pCallback, void* pvArg, CSUDI_HANDLE *phTimer);

/**
@brief ����һ����ʱ��

������ʱ��,ԼnMilliSeconds�����,��������Ӧ�Ļص�����
@param[in] hTimer ��ʱ�������CSUDIPlusOSTimerCreate�ӿڵ��������
@param[in] nMilliSeconds ��ʱʱ��,��λ����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ����һ���Ѿ������Ķ�ʱ�������ü�ʱʱ��,�����سɹ�
*/
CSUDI_Error_Code CSUDIPlusOSTimerStart(CSUDI_HANDLE hTimer, unsigned int unMilliSeconds);

/**
@brief ֹͣһ����ʱ��

ֹͣһ����ʱ��,�ص����������ٱ�����
@param[in] hTimer ��ʱ�����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ֹͣһ���Ѿ�ֹͣ�Ķ�ʱ���Կɷ��سɹ�
*/
CSUDI_Error_Code CSUDIPlusOSTimerStop(CSUDI_HANDLE hTimer);

/**
@brief ����һ����ʱ��

����һ����ʱ��,��ʱʱ���ڵ��ú����¿�ʼ��¼
@param[in] hTimer ��ʱ�����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ����һ���Ѿ�ֹͣ�Ķ�ʱ���Է��سɹ�,���������û���κ�����
*/
CSUDI_Error_Code CSUDIPlusOSTimerReset(CSUDI_HANDLE hTimer);

/**
@brief ����һ����ʱ��

����һ����ʱ��,���ٺ�ö�ʱ�����������Ч,Ҳ�޷��ٴ�����
@param[in] hTimer ��ʱ�����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ������ֹͣ���Ǽ���״̬������������һ����ʱ��
*/
CSUDI_Error_Code CSUDIPlusOSTimerDestroy(CSUDI_HANDLE hTimer);


#ifdef	__cplusplus
}
#endif
/** @} */

#endif

