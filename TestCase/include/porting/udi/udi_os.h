/**@defgroup CSOS os  ���ڳ��ò���ϵͳ��صĲ������紴��������ṩͳһ�ӿڼ�����
@brief ���ڳ��ò���ϵͳ��صĲ������紴��������ṩͳһ�ӿڼ����塣
@brief ���˱��ĵ�����Ľӿڣ�����Ҫʵ��pthread
 
@version 0.2 2008/10/17 �����ĵ�һ���汾
@version 0.1 2008/10/16 ��ʾ�汾
@{
*/

#ifndef _UDI_OS_H_
#define _UDI_OS_H_

#include "udi_typedef.h"
#include "udi_assert.h"

#ifdef  __cplusplus
extern "C" {
#endif

//����Ҫ����ĳ��ģ����ڴ�й¶ʱ,�򿪸ú�,clean����ȫ���±����ģ�鼴��
//#define USE_DBG_MALLOC

/**@brief ���ظ�ֵ��ʾ�ɹ�*/
#define CS_OSP_SUCCESS					(0)
/**@brief ���ظ�ֵ��ʾʧ��*/
#define CS_OSP_FAILURE					(1)
/**@brief ���ظ�ֵ��ʾ��ʱ*/
#define CS_OSP_TIMEOUT					(2)

/**@brief ��������������������ѡ��*/
#define	CS_OSP_MUTEX_NOTOBTAIN				(0)
/**@brief ��������������������ѡ��*/
#define	CS_OSP_MUTEX_OBTAIN				(1)

/**@brief CSWaitForSingleEvent֮���¼��Զ���Чѡ��*/
#define CS_OSP_EVENT_MANUAL_RESET			(0x01)
/**@brief ��ʼ���¼���������Чѡ��*/
#define CS_OSP_EVENT_INITIAL_STATUS			(0x10)

/**@brief ��ʱ����֮������ʱѡ��*/
#define CS_OSP_TIMEOUT_IMMEDIATE  			(0)
/**@brief ��ʱ����֮������ʱѡ��*/
#define CS_OSP_TIMEOUT_INFINITY   			(0xFFFFFFFF)

/**@brief ������������������ѡ��*/
#define CS_OSP_TASK_FLAGS_RUN_IMMEDIATELY  		(0)
/**@brief �����������Զ�����ѡ��*/
#define CS_OSP_TASK_FLAGS_SUSPENDED 			(1)
/**@brief ����������󳤶�*/
#define OS_OSP_THREAD_NAME_MAX_LEN		(32)

/**@brief ��ʱ��ѡ��*/
typedef enum
{
    TIM_MODE_REPEAT = 0,		/**< �ظ�ģʽѡ���ÿָ��ʱ�䴥��һ��*/
    TIM_MODE_ONE_SHOT,			/**< ����ģʽѡ�������ָ��ʱ��󴥷�һ��*/
    TIM_MODE_END				/**< ģʽѡ�������ǣ���Ч����*/
}TIMER_MODE;

/**@brief ����Ļ�����Ϣ�ṹ��*/
typedef struct
{
	unsigned int uThreadId;	///<����ID
	unsigned int uPriority;		///<�������ȼ�
	char szThreadName[OS_OSP_THREAD_NAME_MAX_LEN];	///<��������
}CSThreadInfo_S;

/**
@brief ������ں���

�½�һ������ʱ������һ�������͵ĺ�����Ϊ��ں��������������󣬸ú��������������б����á�
@param[in] pParam ��ڲ�������ֵΪ����CSCreateThread����ʱ��pvArg������
@see CSCreateThread CSCreateThreadEx
*/
typedef void (ThreadEntry)(void *pParam);

/**
@brief ��ʱ���ص�����

�½�һ����ʱ��ʱ������һ�������͵ĺ�����Ϊ�ص���������ʱ��ÿ�δ������ú����ᱻ���á�
@param[in] pParam ��ڲ�������ֵΪ����CSCreateTimer����ʱ��pvArg������
@see CSCreateTimer
*/
typedef void (*TimerCallback)(void* pParam);

/*******************************************************************
 **                         Task definitions                      **
 *******************************************************************/
/**
@brief ��������

*����һ����������乲���߼���ַ���൱���̵߳ĸ��
@param[in] pstrName ��������,���Ȳ��ܳ���32���ַ�������ΪNULL
@param[in] nPriority ��������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ����
@param[in] nStackSize �����ջ��С,�������ϵͳ֧���Զ���С�����ջ����,����ʵ�ֺ��Ըò���
@param[in] pThreadEntry �������ں���
@param[in] pvArg �������ڲ���
@param[out] pdwThreadID ������������ID������ΪNULL�������ڵ���ʱΨһȷ��һ�����񣬲��������κ��������
@return ����ɹ�����������;���ʧ�ܷ���ΪNULL;��û��ϵͳ�ڴ�,��pThreadEntry = NULLʱ,����ʧ��
@note pstrName����Ӱ�촴������ɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
@note �������в�����ʹ�÷��صľ����pdwThreadID����Ψһ��ʶһ������֮��
*/
HCSHANDLE CSCreateThread (const char* pstrName,int nPriority,int nStackSize, ThreadEntry * pThreadEntry,PVOID pvArg, PDWORD pdwThreadID);

/**
@brief ����������չ�ӿ�

CSCreateThread����չ�汾��˵���μ�CSCreateThread
@param[in] pstrName ��������,���Ȳ��ܳ���32���ַ�������ΪNULL
@param[in] nPriority ��������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ����
@param[in] nStackSize �����ջ��С,�������ϵͳ֧���Զ���С�����ջ����,����ʵ�ֺ��Ըò���
@param[in] pThreadEntry �������ں���
@param[in] pvArg �������ڲ���
@param[in] dwInitStatus ����ѡ���ѡֵ��
- CS_OSP_TASK_FLAGS_RUN_IMMEDIATELY	���񴴽�����������
- CS_OSP_TASK_FLAGS_SUSPENDED		���񴴽����Զ�����
@param[out] pdwThreadID ������������ID������ΪNULL�������ڵ���ʱΨһȷ��һ�����񣬲��������κ��������
@return ����ɹ�����������;���ʧ�ܷ���ΪNULL;��û��ϵͳ�ڴ�,��pThreadEntry = NULLʱ,����ʧ��
@note pstrName����Ӱ�촴������ɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
@note �������в�����ʹ�÷��صľ����pdwThreadID����Ψһ��ʶһ������֮��
@see CSCreateThread
*/
HCSHANDLE CSCreateThreadEx (const char* pstrName,int nPriority, int nStackSize, ThreadEntry * pThreadEntry,PVOID pvArg, DWORD dwInitStatus, PDWORD pdwThreadID);

/**
@brief ɾ������

ͨ�����ɾ��һ���Ѿ�����������
@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;��hThread��Чʱ,ɾ��ʧ��
@note �����з��ص���������״ε��øýӿڲ��÷���ʧ��
@note ����������ں����������������з��أ�Ȼ���ٵ��ñ��ӿ��������ͷ���Դ
@note �����������е��ñ��ӿڽ��������٣������Ϊ��δ�����
*/
DWORD CSDestroyThread (HCSHANDLE hThread);

/**
@brief ��������

ͨ���������һ���������е�����ʹ��ֹͣ����
@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;��hThread��Чʱ,ɾ��ʧ��
@note ����һ���Ѿ�����������Է��سɹ�
@note ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated ����������̲߳���Σ�գ�Ҳ��������posixʵ�֣���Ȼ���ڵ�linuxThread������������NPTL�Ѿ�������
*/
DWORD CSSuspendThread(HCSHANDLE hThread);

/**
@brief �����ѹ�������

ͨ���������һ���ѹ��������ʹ��ָ�����
@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;��hThread��Чʱ,ɾ��ʧ��
@note ����һ���������е������Է��سɹ�
@note ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated ͬCSSuspendThread
*/
DWORD CSResumeThread(HCSHANDLE hThread);

/**
@brief ʹһ������ȴ���һ���������

@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;
@note 
- ��hThread��Чʱ,��������CS_OSP_FAILURE��
- �����ȴ����񷵻�ʱ���ýӿڷ���CS_OSP_SUCCESS��
- �ȴ�һ���Ѿ����ص���û�б����ٵ�����ʱ���ýӿڷ���CS_OSP_SUCCESS��
- ���������ͬʱ���ô˽ӿ�ȥ�ȴ�һ������ʱ��������δ����ģ�
- ������ȴ�һ���Ѿ������ٵ�����
*/
DWORD CSJoinThread(HCSHANDLE hThread); 

/**
@brief ��ȡ��ǰ���������ID

@return ��ǰ�����������ID
@note ����ֵ����CSCreateThreadʱ��pdwThreadID�����������ڵ���ʱΨһȷ��һ�����񣬲��������κ��������
*/
DWORD CSGetThreadId(void);

/**
@brief �����������ȼ�

ͨ���������һ����������ȼ�
@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@param[in] nPriority ��Ҫ�趨���������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ����
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;��hThread��Чʱ,ɾ��ʧ��
@note ����һ���Ѿ����з��ص���������ȼ�����ʧ��
*/
DWORD CSSetThreadPriority(HCSHANDLE hThread, int  nPriority);

/**
@brief ��ȡ�������ȼ�

ͨ�������ȡһ����������ȼ�
@param[in] hThread �Ѵ���������������CSCreateThread�ķ���ֵ
@param[out] pnPriority ������������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ����
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���ΪCS_OSP_FAILURE;��hThread��Чʱ,ɾ��ʧ��
@note һ���Ѿ����з��ص������Կ��Ի�ȡ�����ȼ�
@note ʵ�ֿ������е�����������ȼ������Եõ��Ĳ���һ���������õ�ֵ
*/
DWORD CSGetThreadPriority(HCSHANDLE hThread, int* pnPriority);

/**
@brief ����ǰ����һ��ʱ��

@param[in] dwMilliSeconds �����ʱ�䣬��λ�Ǻ���
*/
void CSSleep(DWORD dwMilliSeconds);


/*******************************************************************
 **                    Message Queue definitions                  **
 *******************************************************************/
/**
@brief ������Ϣ����

@param[in] pstrName ��Ϣ��������,���Ȳ��ܳ���32���ֽڣ�����ΪNULL
@param[in] nMaxMsgs ��Ϣ��������������Ϣ����
@param[in] nMaxMsgLen ÿһ����Ϣ����󳤶�,Byte��λ
@param[in] dwOptions ����������Ŀǰ��Ч�����봫NULL������������Բ�����ʧ��
@return ����ɹ�������Ϣ���о��;���ʧ�ܷ���ΪNULL;��û��ϵͳ�ڴ�ʱ,��nMaxMsgs=0,��nMaxMsgLen=0 ʱ,����ʧ��
@note pstrName����Ӱ�촴����Ϣ���гɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
*/
HCSHANDLE CSCreateMsgQueue(const char* pstrName,int nMaxMsgs,int nMaxMsgLen, DWORD dwOptions);

/**
@brief ������Ϣ����

ͨ���������һ����Ϣ����
@param[in] hMsgQueue ��Ϣ���о������CSCreateMsgQueue�ķ���ֵ
@return ����ɹ�����:CS_OSP_SUCCESS;���ʧ�ܷ���:CS_OSP_FAILURE;��hMsgQueue��Чʱ,ɾ��ʧ��
@note ɾ��һ�����ڱ��ȴ����߷�����Ϣ����Ϣ���У�����ǲ�ȷ����
*/
DWORD CSDestroyMsgQueue(HCSHANDLE hMsgQueue);

/**
@brief ����Ϣ���н���һ����Ϣ

����Ϣ���н���һ����Ϣ�������Ϣ����Ϊ����ȴ�ֱ����Ϣ����������Ϣ���߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSCreateMsgQueue�ķ���ֵ
@param[out] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMaxMsgBytes ��ַ������ڴ泤��
@param[in] dwTimeout ��ʱʱ��,���뵥λ����λΪ����;ΪCS_OSP_TIMEOUT_INFINITYʱ���ò���ʱ
@return ����ɹ�����:CS_OSP_SUCCESS;�����ʱ����:CS_OSP_TIMEOUT;���ʧ�ܷ���:CS_OSP_FAILURE
@note ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMaxMsgBytes==0ʱ,����ʧ��,ͬʱ������Ϣ�����ж�ȡ��Ϣ
@note ��nMaxMsgBytesС����Ϣ����ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ�����Ϣ�Ӷ�����ȡ��
*/
DWORD CSReceiveMsg(HCSHANDLE hMsgQueue,void* pvMsg,int nMaxMsgBytes,DWORD dwTimeout);

/**
@brief ����Ϣ�����﷢��һ����Ϣ

����Ϣ�����﷢��һ����Ϣ�������Ϣ����������ȴ�ֱ����Ϣ�������п���λ�û��߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSCreateMsgQueue�ķ���ֵ
@param[in] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMsgBytes ��ַ������ڴ泤��
@param[in] dwTimeout ��ʱʱ��,���뵥λ����λΪ����;ΪCS_OSP_TIMEOUT_INFINITYʱ���ò���ʱ
@return ����ɹ�����:CS_OSP_SUCCESS;�����ʱ����:CS_OSP_TIMEOUT;���ʧ�ܷ���:CS_OSP_FAILURE
@note ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMsgBytes==0ʱ,����ʧ��
@note ��nMsgBytes������󳤶�ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ�
@note ��Ϣ�����ѿ���ģʽ������pvMsg�ڱ��������غ󼴿��ͷ�
*/
DWORD CSSendMsg(HCSHANDLE hMsgQueue, const PVOID pvMsg, int nMsgBytes, DWORD dwTimeout);

/**
@brief ��ȡ��Ϣ��������Ϣ��������

ͨ����Ϣ���о����ȡ��Ϣ��������Ϣ��������
@param[in] hMsgQueue ��Ϣ���о������CSCreateMsgQueue�ķ���ֵ
@return ����ɹ�����>=0;���ʧ�ܷ���-1;��hMsgQueue��Чʱ����-1;
*/
int CSGetMsgNum(HCSHANDLE hMsgQueue);


/*******************************************************************
 **                     Semaphore definitions                     **
 *******************************************************************/
/**
@brief �����ź���

@param[in] pstrName �ź�������,���Ȳ��ܳ���32���ַ�
@param[in] nInitialCount ��Ч�ź��������൱��PVԭ���еĳ�ʼֵ
@param[in] nMaxCount ����ź�����
@return ����ɹ������ź������;���ʧ�ܷ���ΪNULL
@note pstrName����Ӱ�촴���ź����ɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
@note �ж������ȴ�ͬһ���ź���ʱ��һ�����ź������ͷţ���ȷ���ĸ�������Գɹ�����ź���
*/
HCSHANDLE CSCreateSemaphore(const char* pstrName,int nInitialCount,int nMaxCount);

/**
@brief �����ź���

@param[in] hSemaphore �ź������
@return ���hSemaphore��Ч����CS_OSP_SUCCESS;���hSemaphoreΪ��Ч�������CS_OSP_FAILURE
@warning ����һ�����ڱ���������ȴ������ź�����һ����ȷ������Ϊ
*/
DWORD CSDestorySemaphore(HCSHANDLE hSemaphore);

/**
@brief �ȴ��ź������൱��P����

@param[in] hSemaphore �ź������
@param[in] dwTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CS_OSP_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CS_OSP_TIMEOUT_INFINITY	��Զ����ʱ
@return ���hSemaphore��Ч����CS_OSP_FAILURE;���hSemaphore��Ч�������ʱ����CS_OSP_TIMEOUT;����ɹ�����CS_OSP_SUCCESS
*/
DWORD CSWaitForSemaphore(HCSHANDLE hSemaphore,DWORD dwTimeout);

/**
@brief �ͷ��ź������൱��V����

@param[in] hSemaphore �ź������
@param[in] nReleaseCount �ͷ��ź����Ĵ���
@return �ɹ�����CS_OSP_SUCCESS;ʧ�ܷ���CS_OSP_FAILURE
- ���hSemaphore��Ч����lReleaseCountΪ0����CS_OSP_FAILURE
- ���hSemaphore��Ч,lReleaseCount>=1��ÿ���ͷųɹ�����CS_OSP_SUCCESS
- ���lReleaseCount>1���ж���1��û���ͷųɹ�����CS_OSP_FAILURE
*/
DWORD CSReleaseSemaphore(HCSHANDLE hSemaphore, int nReleaseCount);


/*******************************************************************
 **                     Mutex definitions                         **
 *******************************************************************/
/**
@brief ����������

@param[in] pstrName ����������,���Ȳ��ܳ���32���ַ�
@param[in] dwFlags ����ź�����
- CS_OSP_MUTEX_OBTAIN ��������������������
- CS_OSP_MUTEX_NOTOBTAIN ��������������������
@return ����ɹ������ź������;���ʧ�ܷ���ΪNULL
@note pstrName����Ӱ�촴���������ɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
@note ͬһ�������Ѿ���һ�������������󣬿��ٴζ������
@note һ�������һ���������������������������п���������ʧ��
@note �����������ڸ߼����ԣ�����������߼�����֮
@note �ж������ȴ�ͬһ��������ʱ��һ���û��������ͷţ���ȷ���ĸ�������Գɹ�����
*/
HCSHANDLE CSCreateMutex(const char* pstrName, DWORD  dwFlags);

/**
@brief ���ٻ�����

@param[in] hMutex ���������
@return ���hMutex��Ч����CS_OSP_SUCCESS;���hMutex��Ч����CS_OSP_FAILURE
@warning ����һ�����ڱ���������ȴ����Ļ�������һ����ȷ������Ϊ
*/
DWORD CSDestroyMutex(HCSHANDLE hMutex);

/**
@brief �ȴ�������������������

@param[in] hMutex ���������
@param[in] dwTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CS_OSP_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CS_OSP_TIMEOUT_INFINITY	��Զ����ʱ
@return ���hMutex��Ч����CS_OSP_FAILURE;���hMutex��Ч�������ʱ����CS_OSP_TIMEOUT;����ɹ�����CS_OSP_SUCCESS
*/
DWORD CSWaitForMutex(HCSHANDLE hMutex,DWORD dwTimeout);

/**
@brief �ͷŻ�����������������

@param[in] hMutex ���������
@return ���hMutex��Ч����CS_OSP_FAILURE;���hMutex��Ч����CS_OSP_SUCCESS
*/
DWORD CSReleaseMutex(HCSHANDLE hMutex);


/*******************************************************************
 **                     Event definitions                         **
 *******************************************************************/
/**
@brief �����¼�

@param[in] pstrName �¼�����,���Ȳ��ܳ���32���ַ�
@param[in] dwFlags �¼�ѡ��,��־��ͬʱʹ��,Ҳ�ɷֱ�ʹ��
- CS_OSP_EVENT_MANUAL_RESET CSWaitForSingleEvent֮���¼��Զ���Ч
- CS_OSP_EVENT_INITIAL_STATUS ��ʼ�����¼�������Ч�������Ͽ���Wait��
@return ����ɹ������¼����;���ʧ�ܷ���CS_OSP_FAILURE
@note pstrName����Ӱ�촴���¼��ɹ���񣬼�����ΪNULL�������ظ�,�����Ƽ�������
@note �ж������ȴ�ͬһ���¼�ʱ��һ�����¼���Ч����ȷ���ĸ�����(��)�ȵ��¼�
*/
HCSHANDLE CSCreateEvent(const char* pstrName,DWORD  dwFlags);

/**
@brief �����¼�

@param[in] hEvent �¼����
@return ���hEvent��Ч����CS_OSP_SUCCESS;���hMutex��Ч����CS_OSP_FAILURE
@warning ����һ�����ڱ���������ȴ������¼���һ����ȷ������Ϊ
*/
DWORD CSDestroyEvent(HCSHANDLE hEvent);

/**
@brief �����¼���Ч

�¼���Ч��CSWaitForSingleEvent������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���CS_OSP_FAILURE
*/
DWORD CSResetEvent(HCSHANDLE hEvent);

/**
@brief �����¼���Ч

�¼���Ч��CSWaitForSingleEvent����������
@param[in] hEvent �¼����
@return ����ɹ�����CS_OSP_SUCCESS;���ʧ�ܷ���CS_OSP_FAILURE
*/
DWORD CSSetEvent(HCSHANDLE hEvent);

/**
@brief �ȴ��¼�

�¼���Чʱ��������;��Чʱ��������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@param[in] dwTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CS_OSP_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CS_OSP_TIMEOUT_INFINITY	��Զ����ʱ
@return ���hEvent��Ч����CS_OSP_FAILURE;���hEvent��Ч�������ʱ����CS_OSP_TIMEOUT;����ɹ�����CS_OSP_SUCCESS
*/
DWORD CSWaitForSingleEvent(HCSHANDLE hEvent,DWORD dwTimeout);

/**
@brief ��ʼ����ʱ��

@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
@note ֻ�����ʼ��һ��
@note ����һ��Coshipר�ýӿڣ�ֻ����Ӧ�ó�ʼ��ʱʹ�ã����������м������
*/
DWORD  CSInitTimer(void);

/**
@brief ������ʱ��

@param[in] enmTimerMode ��ʱ������ģʽ
- TIM_MODE_REPEAT �ظ�ģʽ����ÿָ��ʱ�䴥��һ��
- TIM_MODE_ONE_SHOT ����ģʽѡ�������ָ��ʱ��󴥷�һ��
@param[in] pCallback ��ʱ�������Ļص�����
@param[in] pvArg ��ʱ���ص������Ĳ���
@param[out] phTimer ��ʱ�����
@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
@note ��ʱ���ľ��ȱ�����100ms����
@note �뾡��ʹ��ʱ���ص��������أ������������ʱ������Ӱ��
@note ��ʱ���ص������У��Կ��Ե��ö�ʱ���������в���
*/
DWORD CSCreateTimer(TIMER_MODE enmTimerMode, TimerCallback pCallback, PVOID pvArg, HCSHANDLE *phTimer);

/**
@brief ����һ����ʱ��

������ʱ����ԼdwMilliSeconds����󣬽�������Ӧ�Ļص�����
@param[in] hTimer ��ʱ�����
@param[in] dwMilliSeconds ��ʱʱ�䣬��λ����
@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
@note ����һ���Ѿ������Ķ�ʱ�������ü�ʱʱ�䣬�����سɹ�
*/
DWORD CSStartTimer(HCSHANDLE hTimer, DWORD dwMilliSeconds);

/**
@brief ֹͣһ����ʱ��

ֹͣһ����ʱ�����ص����������ٱ�����
@param[in] hTimer ��ʱ�����
@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
@note ֹͣһ���Ѿ�ֹͣ�Ķ�ʱ���Կɷ��سɹ�
*/
DWORD CSStopTimer(HCSHANDLE hTimer);

/**
@brief ����һ����ʱ��

����һ����ʱ������ʱʱ���ڵ��ú����¿�ʼ��¼
@param[in] hTimer ��ʱ�����
@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
@note ����һ���Ѿ�ֹͣ�Ķ�ʱ���Է��سɹ�����������Ų�������
*/
DWORD CSResetTimer(HCSHANDLE hTimer);

/**
@brief ����һ����ʱ��

����һ����ʱ�������ٺ�ö�ʱ��������Ч��Ҳ�޷��ٴ�����
@param[in] hTimer ��ʱ�����
@return �����ʼ���ɹ�����CS_OSP_SUCCESS;�����ʼ��ʧ�ܷ���CS_OSP_FAILURE
*/
DWORD CSDestroyTimer(HCSHANDLE hTimer);

/**
@brief ��ȡ��ϵͳ�����󾭹���ʱ�������

@return ������ϵͳ�����󾭹���ʱ�������
*/
DWORD CSGetTickCount(void);

/**
@brief ����ϵͳʱ��

����ϵͳʱ�䣬֮����Զ���ʱ�Ա�֤ʱ����׼ȷ��
@param[in] dwSeconds ��λ���룬��1970��1��1���������ڵ���������
@return ����ɹ�����TRUE;���ʧ�ܷ���FALSE
*/
BOOL CSSetSoftClock(DWORD dwSeconds);

/**
@brief ��ȡ��ǰϵͳʱ��

@param[out] pdwSeconds ��λ���룬��1970��1��1���������ڵ���������
@return ����ɹ�����TRUE;���ʧ�ܷ���FALSE
@note ��δ����ʱ��֮ǰ�����ص�ʱ���ǲ�ȷ����
*/
BOOL CSGetSoftClock(DWORD* pdwSeconds);

/**
@brief ����ϵͳʱ��

����һ�ֵ�λ������ϵͳʱ��. ��CSSetSoftClockЧ����ͬ
@param[in] pDateTime ʱ�����ݽṹ����������ʱ�����ʾ
@return ����ɹ�����TRUE;���ʧ�ܷ���FALSE
*/  	
BOOL CSSetSoftTime(TSYSTEMTIME* pDateTime);

/**
@brief ��ȡϵͳʱ��

����һ�ֵ�λ����ȡϵͳʱ��. ��CSGetSoftClockЧ����ͬ
@param[in] pDateTime ʱ�����ݽṹ����������ʱ�����ʾ
@return ����ɹ�����TRUE;���ʧ�ܷ���FALSE
*/ 
BOOL CSGetSoftTime(TSYSTEMTIME* pDateTime);


#ifndef USE_DBG_MALLOC /*�˶����������Ҫ�ڴ��ļ����ֶ����*/
/*******************************************************************
 **                     Memory definitions                        **
 *******************************************************************/
/**
@brief �����ڴ棬������ʼ��

�൱��malloc���·�����ڴ��������ǲ�ȷ����
@param[in] nMemorySize ��������ڴ��С
@return ����ɹ������ڴ�ָ��;���ʧ�ܷ���ΪNULL
@note nMemorySize����0ʱ�ķ���ֵ�ǲ�ȷ���ģ�������0Ҳ��������һ����ַ
*/
PVOID CSMalloc( unsigned int nMemorySize );

/**
@brief �ط����ڴ�

�൱��realloc�����·����ڴ�Ϊ�µĴ�С������������������ݣ�������ڴ�ռ�ȾɵĴ����������ֵ������ǲ�ȷ����
@param[in] pvOldMemoryAddr �����·�����ڴ��ַ
@param[in] nMemorySize ��������ڴ��С
@return ����ɹ������ڴ�ָ��;���ʧ�ܷ���ΪNULL
@note nMemorySize����0ʱ�ķ���ֵ�ǲ�ȷ���ģ�������0Ҳ��������һ����ַ
���pvOldMemoryAddr����һ��֮ǰCSMalloc,CSRealloc,CSCalloc���صĵ�ַ������ǲ�ȷ����
���pvOldMemoryAddr==NULL��������CSMallocһ��
�������ʧ�ܣ��ɵ��ڴ治���Զ��ͷţ�������Ҳ�������仯
����ֵ������pvOldMemoryAddr��һ�£�Ҳ����һ��
*/
PVOID CSRealloc( PVOID pvOldMemoryAddr,unsigned int nMemorySize );

/**
@brief �����ڴ棬����ʼ��

�൱��calloc������nElements��nElementSize��С��objects,�����ڴ�ȫ����Ϊ0
@param[in] nElements �������Ԫ�ظ���
@param[in] nElementSize �������ÿ��Ԫ�صĴ�С
@return ����ɹ������ڴ�ָ��;���ʧ�ܷ���ΪNULL
@note nElements��nElementSize����0ʱ�ķ���ֵ�ǲ�ȷ���ģ�������0Ҳ��������һ����ַ
*/
PVOID CSCalloc(unsigned int nElements, unsigned int nElementSize);

/**
@brief �ͷŷ�����ڴ�

�൱��free���ͷ�CSMalloc,CSRealloc,CSCalloc������ڴ�
@param[in] pvMemoryAddr ���ͷŵ��ڴ��ַ
@return ����ɹ�����TRUE;���ʧ�ܷ���FALSE
@note ���pvMemoryAddr����һ��֮ǰCSMalloc,CSRealloc,CSCalloc���صĵ�ַ������ǲ�ȷ����
@note ����ͷ�ͬһ���ڴ潫����ʧ��
*/
BOOL CSFree( PVOID pvMemoryAddr );

/**
@brief Dump �ڴ�ʹ�����

@return ��
@note 
@note 
*/
#define CSMemDump() {}
#else
/*���ļ�ר�����ڵ���*/
#include "cs_memleak.h"
#endif

/**
@brief ��ȡ���õ�RAM�ռ�
@param[out] pnFreeCount ϵͳ���õ�RAM�ռ�
@return �����ʼ���ɹ�����CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetFreeRAM(unsigned int * puFreeCount);

/**
@brief ��ȡCPU����
@param[out] pnCpuCount CPU�ĸ���
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetCPUCount(unsigned int * puCpuCount);


/**
@brief ��ȡָ������CPU�Ĵ�����������(%)
@param[out] pnUsage CPU�����������ʣ�ȡֵ��ΧΪ0--1000;
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetCPUUsage(unsigned int index, unsigned int * puUsage);

/**
@brief ��ȡϵͳ����������ľ��
@param[out]  hThread ϵͳ����������ľ������
@param[in/out]   nSize hThread����Ĵ�С������ʵ��Thread�ĸ���
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetAllThreadHandle(HCSHANDLE* phThread, unsigned int*  puSize);

/**
@brief ��ȡָ������Ļ�����Ϣ�������������ơ�����ID���������ȼ�
@param[in]   hThread ָ������ľ��
@param[out]  psThreadInfo ����Ļ�����Ϣ
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��psThreadInfo��ΪNULL��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetThreadInfo(HCSHANDLE hThread, CSThreadInfo_S* psThreadInfo);


/**
@brief ��ȡָ�������RAM����
@param[in]   hThread ָ������ľ��
@param[out]  pnUsedRam ָ�������RAM����
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetThreadUsedRAM(HCSHANDLE hThread, unsigned int * puUsedRam);


/**
@brief ��ȡָ�������CPU������(%)
@param[in]   hThread ָ������ľ��
@param[out] pnUsage ָ�������CPU���أ�ȡֵ��ΧΪ:0--1000
@return ��ȡ�ɹ��򷵻�CS_OSP_SUCCESS��ʧ���򷵻�CS_OSP_FAILURE��
*/
DWORD CSGetThreadCPUUsage(HCSHANDLE hThread, unsigned int * puUsage);

//add ended
#ifdef  __cplusplus
}
#endif

/** @} */

#endif
