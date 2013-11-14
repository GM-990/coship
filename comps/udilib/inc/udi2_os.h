/**@defgroup OS OS �����˲���ϵͳ��ص�һϵ�в������������ź����ȡ�

@brief OSģ���Ӧͷ�ļ�<udi2_os.h>�������˲���ϵͳ��ص�һϵ�в������������ź���
�ȡ���������Ҫ�������񲿷֡���Ϣ���в��֡��ź������֡����������֡�ʱ�䲿�֡��¼�
���ֺ��ڴ沿�֡�

@section  udi2 os��pthread�Ĺ�ϵ:
udi2 osʵ���Ͽ�������pthread,Ҳ�ɲ�������pthread������ǿ��Ҫ�󡣽����ڵײ���pthread֧�ֵģ�
osʵ���ڲ�����pthread��

���ǣ�mmcp��������pthread�ġ�ƽ̨�����ṩpthread��ʵ�֡����ƽ̨û��pthread����mmcp
��Ȼ�޷��������С���û��pthread��ƽ̨�ϣ�coship�����ṩpthread�Ŀ⣬Ҳ�ɲ��õ�����
�Լ�ʵ�ֵ�pthread�����ǣ�����˭ʵ��pthread�� ���뱣֤���ڵ�������udi�ӿ�֮ǰ��pthread
�Ѿ����ã����Ҫ����udi_init()�г�ʼ��pthread. �ڵ����������У�����udi�ӿ��Ǻ�����˾
ʵ�ֵģ���ˣ�Ӧ���������ṩpthread��ʼ���ӿ��Ա���udi_init�е���.

@par ���񲿷�
���񲿷ְ������������١����𡢵ȴ�����Ƚӿڡ�����������൱��OS�е��̸߳����
�����������ַ�ռ䡣��Ҫ�ر�ע�������������һ�������Ƿǳ�Σ�յģ���ȷ�������ǣ�\n
1���ȷ���Ϣ֪ͨ����������\n
2������������ӵ�����Ϣ���ͷ�������Դ�����ź�������Ϣ���еȣ�������������ں������ء�\n
3���ȴ�Join�������سɹ����ٵ���Destroy�����������١�

@note
- ��ͼȥ����һ���������е����񣨼�������ں�����û�з��ص�����ʱ���᷵��CSUDIOS_ERROR_THREAD_BUSY��
- �ڱ����������ٱ������ǲ�������ġ�

@note ���⣬�Ա����ֵ�Suspend��Resume�ӿ��Լ�Yield�ӿ�������˵����
- Suspend��Resume�ӿڽ����ڵ��ԣ�Ŀǰ���Ƿ����ڲ��ֲ���ϵͳ������ʵ�֣������NPTL��
�̿�Linuxϵͳ������������������ӿڿɲ�ʵ�֣�����CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED
���ɡ���ͬʱ�������Ƽ�����ʵ���������ӿڣ��Է�������ĵ��ԡ�
- Yield�ӿ�����������������ε��ȵ�ʱ��Ƭ���Է�ֹ��һ�����ȼ�����ʱ����ռCPU��Դ��
������������޷�ִ�С���Ȼ���������£�SleepһС���ʱ��Ҳ�ܴﵽͬ����Ч��������ͬ��
ƽ̨���Sleepʱ��ļ���ֵ�ǲ�ͬ�ģ����磺��Щƽ̨���ʱ����Ϊ0��������Sleep������
����Щƽ̨ȴ��Ҫ�������ʱ�䳬��һ����Сֵ������BCM7403����СֵԼΪ3ms���ſ�����
Sleep����������ͻ���ԡ�Ϊ��������Щƽ̨�Ĳ��죬���Զ����˴˽ӿڡ�

@par ��Ϣ���в���
��Ϣ���в��ְ���������������Ϣ���м����͡�������Ϣ�Ƚӿڡ���Ϣ����������䴫����Ϣ
���������ݵ����ѡ����ͼ��ʾ����Ϣ���еĻ������ܡ���Ϣ����Ϣ��������ǳ������ʽ����
�ģ�������ͨ����Ϣ���д��ʹ������ݣ���Section�ȡ���Ȼ������Ϣ���е������Ϣ��������
Ϣ���Ȳ���̫�󣬵��Խ����������������Ϣ�������ڴ����ֱ��ʹ��ϵͳ���ڴ�ķ�ʽ������
Ϣ���е�������
@image HTML udi_os_msgq.png

@par �ź�������
�ź������ְ������������١��ȴ����ͷ��ź����Ƚӿڡ������ڶ������Э�������߳�, �Ա�֤
�����ܹ���ȷ�������ʹ�ù�����Դ��\n
��ֵ�ź����뻥����֮�����������ϵ�����ġ��뻥������ȣ���ֵ�ź�����ʾһ����Դ�����
��Դֻ��һ�����ѱ���һ�������ȡ�����Ǹ������ͷŸ���Դ�������κ����񣨰����Ѿ���ȡ��
�����Դ�����񣩾��޷���ȡ����Դ��
@note �ж������ȴ�ͬһ���ź���ʱ��һ�����ź������ͷţ���ȷ���ĸ�������Գɹ���ø�
�ź�����

@par ����������
���������ְ������������١���ȡ���ͷŻ������Ƚӿڡ��������ǽ�����������ΪǶ�����ĸ��
�䱾���൱��һ�������ṩ�Թ�����Դ�ķ��ʱ�����\n
�뵥ֵ�ź�����ȣ���������ʾһ����Դ������Ȩ״̬����һ�������ȡ��ĳ����Դ��ʹ��Ȩ��
֮�󣬳��Ǹ������ͷŸ�Ȩ�ޣ����������κ�������޷���ȡ��Ȩ�ޣ������������������
���������Լ�ӵ�и�Ȩ����û������ġ���ͬһ�������Ѿ���һ�������������󣬿��ٴζ����
��������μ�������Ҫ����ͷŲ����ͷŸ�����\n
@note �ж������ȴ�ͬһ��������ʱ��һ���û��������ͷţ���ȷ���ĸ�������Գɹ�������

@par ʱ�䲿��
ʱ�䲿�ֽ�����һ����ȡϵͳ�����󾭹���ʱ��ӿڡ�ʵ����Eastwin����������ʹ�øýӿڻ��
ʱ��Ĳ�ֵ�������˽����ε���֮���ȥ�˶೤ʱ�䣬�����ʱ���0ʱ���Ƿ��Ǵӿ�����һ˲��
������Ҫ�����ڲ�̫Ӱ��ϵͳЧ�ʵ�ǰ���£������ṩ���߾��ȵ�ʱ����Ϣ���Ƽ���ȷ��CPU Tick
�������پ�ȷ�����뼶�𣬷�����ܻ��ϵͳ��������Ԥ�ϵ�Ӱ�졣\n\n

@par �¼�����
�¼����ְ������������١����á����á��ȴ��¼��Ƚӿڡ�����˼�壬�¼����ڱ�ʾһЩ�����
������δ��������������첽����Ƶ���������ǿ����ȴ���һ��Event��Ȼ���ڵ����첽��Ƶ�ӿ�
��ȥ�ȴ����Event��������Ƶ�ɹ��Ļص���Set���Event��
һ����˵���ַ�ʽ��ͬ���첽�¼������ѡ����Ȼʹ���ź�������Ϣ���е�Ҳ��Ҳ�ﵽ���Ƶ�
���ã������ڲ���ƽ̨Event����ͨ���ź���������Ϣ������ʵ�ֵģ�������֮���Դ��������ϵ�
���죬�ֱ������ڲ�ͬ�������\n
�¼�����ģʽ���������������
- �������ʱCSUDIOS_EVENT_AUTO_RESETѡ����ã����¼��ڷ��������κ�λ�ñ��������
���Զ���Ч��Ҳ����˵���һ��Event������λ���ڵȴ�����ֻ��Set��һ�Σ����������ֻ��һ��
λ�ÿ��Եȵ����¼�������
- �������ʱCSUDIOS_EVENT_AUTO_RESETѡ��û�б����ã����¼���������Ǳ�����Reset������
��Զ��Ч��Ҳ����˵���һ��Event�ж��λ���ڵȴ����ұ�Set��һ�κ���������£�����λ��
�����Եȵ����¼�������

@par �ڴ沿��
�ڴ沿�ְ���Malloc��Realloc��Calloc��Free�ȱ�׼C�����ӿڡ�Calloc������Ч�ʻ��
Malloc+memset��һ�㣬��Ȼ��Ҫ�Բ����ر�󣬵����ڴ����ƽ̨��������ӿڣ����Է�װһ��Ҳ
�ǳ����ס�Realloc��ƽ̨�޷��ṩ������£�����ʹ��Malloc+memcpy+Free��ʵ�֣����Ƽ�����ʹ
��ƽ̨ԭ���ӿ�ʵ�֣���Ϊ�ںܶ�����£�Realloc������ԭ�����ڴ�Ļ���������һ�ζ�����ȫ��
�·��䣬Ч�ʻ�ߵĶ࣬Ҳ�����ײ����ڴ���Ƭ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/15 ����һЩ�������������ƴд����
@version 2.0.2 2009/09/07 ����yield�ӿ�
@version 2.0.2 2009/08/24 ��һ���ۺ�������޸ĵİ汾
@version 2.0.1 2009/08/16 ��ʾ�汾
@{
*/

#ifndef _UDI2_OS_H_
#define _UDI2_OS_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDIOS_TIMEOUT_IMMEDIATE  		(0)			///< ��ʱ����֮������ʱѡ��

#define CSUDIOS_TIMEOUT_INFINITY   		(~0)		///< ��ʱ����֮������ʱѡ��

#define CSUDIOS_EVENT_AUTO_RESET		(1 << 0)	///< CSUDIOSEventWait֮���¼��Զ���Чѡ��

#define CSUDIOS_EVENT_INITIAL_STATUS	(1 << 1)	///< ��ʼ���¼���������Чѡ��

#define CSUDIOS_MUTEX_DEFAULT			(0)			///< ����������ʱ��ʹ��Ĭ��ѡ��

#define	CSUDIOS_MUTEX_OBTAIN			(1 << 0)	///< ������������������ȡ����ѡ��
#define CSUDIOS_THREAD_NAME_MAX_LEN    32

/**@brief OS ��ش������ֵ*/
enum
{
	CSUDIOS_ERROR_BAD_PARAMETER = CSUDI_OS_ERROR_BASE,	///< ��������
	CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED,				///< ��֧�ִ˲���
	CSUDIOS_ERROR_UNKNOWN_ERROR,						///< һ�����
	CSUDIOS_ERROR_NO_MEMORY,							///<	 ���ڴ���ô���
	CSUDIOS_ERROR_TIMEOUT,								///< ��ʱ����
	CSUDIOS_ERROR_THREAD_BUSY									///< ������������

};

/**@brief ʱ����ؽṹ��*/
typedef struct 
{
	int  m_nSecond;				///< ��
	int  m_nMiliSecond;			///< ����
	long m_lUSecond;            ///< ΢�룬��һ�����֮һ��
}CSUDIOSTimeVal_S;

/**@brief ����Ļ�����Ϣ�ṹ��*/
typedef struct
{
	unsigned int uThreadId;	///<����ID
	unsigned int uPriority;		///<�������ȼ�
	char szThreadName[CSUDIOS_THREAD_NAME_MAX_LEN];	///<��������
}CSUDIOSThreadInfo_S;

/*******************************************************************
**                         Task definitions                       **
**    ���ǵ�ɾ��,����,�������ȼ��Ȳ����Ĳ���ȫ��,����ɾ����Щ�ӿ�
 *******************************************************************/

/**
@brief ������ں���

�½�һ������ʱ������һ�������͵ĺ�����Ϊ��ں���,���������󣬸ú��������������б�����.
@param[in] pvParam ��ڲ�������ֵΪ����CSUDIOSThreadCreate����ʱ��pvArg����.
@see CSUDIOSThreadCreate
*/
typedef void (*CSUDIOSThreadEntry_F)(void * pvParam);

/**
@brief ��������

*����һ������,����乲���߼���ַ,�൱���̵߳ĸ��
@param[in] pcName ��������,���Ȳ��ܳ���32�ֽ�,����ΪCSUDI_NULL��������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER���������񴴽�ʧ��
@param[in] nPriority ��������ȼ�,��ΧΪ0-255;0���ȼ����,255���ȼ���ߡ��������������ȼ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER,�������񴴽�ʧ��
@param[in] nStackSize �����ջ��С,����֧��512KB�Ĵ�С���������ϵͳ֧���Զ���С�����ջ����,����ʵ�ֺ��Ըò���,��ʹ���߱�������ò�������Ӧ��֧���Զ�ջ��С��ϵͳ
@param[in] fnThreadEntry �������ں���
@param[in] pvArg �������ڲ���
@param[out] phThread ���ɵ�������,��������ʹ�øþ��
@return �ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����phThreadΪCSUDI_NULL
@note 
- pcName����Ӱ�촴������ɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- �����Ը���������в�����ʹ��phThread
- �߳�����Ӧ����ո�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadCreate(const char * pcName,int nPriority,int nStackSize, CSUDIOSThreadEntry_F fnThreadEntry,void * pvArg,CSUDI_HANDLE * phThread);

/**
@brief ɾ������

ͨ�����ɾ��һ���Ѿ�����������
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ,ɾ��ʧ��
@note 
- Ҫ��������ں����������������з��أ�Ȼ���ٵ��ñ��ӿ��������ͷ���Դ
- �������ڱ����������ٱ�����
- ��ͼȥ����һ���������е����񣨼�������ں�����û�з��ص�����ʱ���᷵��CSUDIOS_ERROR_TASK_BUSY��
@see  CSUDIOSThreadJoin

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadDestroy (CSUDI_HANDLE hThread);

/**
@brief ��������

ͨ���������һ���������е�����ʹ��ֹͣ����
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ,�������ʧ��
@note 
- ����һ���Ѿ�����������Է��سɹ�
- ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated �˽ӿڽ����ڵ��ԣ����ƽ̨��֧�֣��ɲ�ʵ�֣���ʱ�˽ӿڷ���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadSuspend(CSUDI_HANDLE hThread);

/**
@brief �����ѹ�������

ͨ���������һ���ѹ��������ʹ��ָ�����
@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;��hThread��Чʱ����������ʧ��
@note 
- ����һ���������е������Է��سɹ�
- ����һ���Ѿ����з��ص����񷵻�ʧ��
@deprecated �˽ӿڽ����ڵ��ԣ����ƽ̨��֧�֣��ɲ�ʵ�֣���ʱ�˽ӿڷ���CSUDIOS_ERROR_FEATURE_NOT_SUPPORTED

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadResume(CSUDI_HANDLE hThread);

/**
@brief ʹһ������ȴ���һ���������

@param[in] hThread �Ѵ���������������CSUDIOSThreadCreate���������phThread
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ
@note 
- �����ȴ����񷵻�ʱ���ýӿڷ���CSUDI_SUCCESS��
- �ȴ�һ���Ѿ����ص���û�б����ٵ�����ʱ���ýӿڷ���CSUDI_SUCCESS��
- ���������ͬʱ���ô˽ӿ�ȥ�ȴ�һ������ʱ��������δ����ġ�
- ������ȴ�һ���Ѿ������ٵ�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSThreadJoin (CSUDI_HANDLE hThread); 

/**
@brief ��ȡ��ǰ����ľ��
@param[out] phThread ��ǰ������ľ������CSUDIOSThreadCreate���������phThread
@return  ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ;

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code  CSUDIOSThreadSelf(CSUDI_HANDLE * phThread);

/**
@brief ����ǰ����һ��ʱ��

@param[in] uMilliSeconds �����ʱ�䣬��λ�Ǻ���
@note 
�ýӿ�ǿ�Ƶ�ǰ�����������uMilliSeconds���룬���ָ�������ʱ��uMilliSecondsΪ0����ʱ������ƽ̨����������������Ϊ��
- ��������ִ��Ȩ��
- ����ע��ֵ�� 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadSleep(unsigned int uMilliSeconds);

/**
@brief ʹ�����������ִ�еĻ���

����������ִ��Ȩ��
@note 
- �ýӿڵ��ú�������Ч���������ء�
- �ڶ����񻷾�(����)�У�Ϊ�˷�ֹĳ�����ռCPU��Դ(��ʱ���������޷��õ���Ӧ)�������õ�ǰִ�е������������ִ�еĻ��ᡣ
- ���øýӿڲ����ܱ�֤��һ�����е������Ǹ����񡣼�����ýӿڷ���ʱ���������ٴλ��ִ��Ȩ����ô���ýӿ���ͣ����������Ͽ�ʼ���С�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void CSUDIOSThreadYield (void); 

/*******************************************************************
 **                    Message Queue definitions                  **
 *******************************************************************/
/**
@brief ������Ϣ����

@param[in] pcName ��Ϣ��������,���Ȳ��ܳ���32���ֽڣ�����ΪCSUDI_NULL��������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER��������Ϣ���д���ʧ��
@param[in] nMaxMsgs ��Ϣ��������������Ϣ������һ����˵���ᳬ��128������������һ����Ϣ��������Ϣ���д���ʧ��
@param[in] nSizePerMsg ÿһ����Ϣ����󳤶�,��λ�ֽڣ�һ����˵���ᳬ��128�ֽڡ�����֧��һ���ֽڣ�������Ϣ���д���ʧ��
@param[out] phMsgQueue ���ɵ���Ϣ���о��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������ֵ����phMsgQueΪCSUDI_NULL��
@note 
- pcName����Ӱ�촴����Ϣ���гɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- ��Ȼ������Ϣ���е������Ϣ��������Ϣ���Ȳ���̫�󣬵��Խ����������������Ϣ�������ڴ����ֱ��ʹ��ϵͳ���ڴ�ķ�ʽ������Ϣ���е�����
@note 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueCreate(const char * pcName,int nMaxMsgs,int nSizePerMsg,CSUDI_HANDLE * phMsgQueue);

/**
@brief ������Ϣ����

ͨ���������һ����Ϣ����
@param[in] hMsgQueue ��Ϣ���о������CSUDIOSMsgQueueCreate���������phMsgQueue��
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������ֵ
@warning ɾ��һ�����ڱ��ȴ����߷�����Ϣ����Ϣ����,����ǲ�ȷ����,�����øýӿ�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueDestroy(CSUDI_HANDLE hMsgQueue);

/**
@brief ����Ϣ���н���һ����Ϣ

����Ϣ���������һ����Ϣ�������Ϣ����Ϊ����ȴ�ֱ����Ϣ����������Ϣ���߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSUDIMsgQueueCreate���������phMsgQueue
@param[out] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMaxMsgBytes ��ַ������ڴ泤��
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������
@note 
- ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMaxMsgBytes==0ʱ,����ʧ��,ͬʱ������Ϣ�����ж�ȡ��Ϣ
- ��nMaxMsgBytesС����Ϣ����ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ�����Ϣ�Ӷ�����ȡ��
- ��nMaxMsgBytes������Ϣ����ʱ����Ϣ����ȫ����pvMsg,��������Ϣ���Ⱥ󲿷����ݲ�ȷ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueReceive(CSUDI_HANDLE hMsgQueue,void * pvMsg,int nMaxMsgBytes,unsigned int uTimeout);

/**
@brief ����Ϣ�����﷢��һ����Ϣ

�����Ϣ����������ȴ�ֱ����Ϣ�������п���λ�û��߳�ʱΪֹ
@param[in] hMsgQueue ��Ϣ���о������CSUDIOSMsgQueueCreate���������phMsgQueue
@param[in] pvMsg ������Ϣ���ݵ��ڴ��ַ
@param[in] nMsgBytes ��ַ������ڴ泤��
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������
@note 
- ��hMsgQueue��Чʱ,��pvMsg��Ч,��nMsgBytes==0ʱ,����ʧ�ܡ�
- ��nMsgBytes������󳤶�ʱ,ȡ��Ч������Ϣ���ݣ�ͬʱ���سɹ���
- ��Ϣ�����Կ���ģʽ������pvMsg�ڱ��������غ󼴿��ͷš�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMsgQueueSend(CSUDI_HANDLE hMsgQueue, const void * pvMsg, int nMsgBytes, unsigned int uTimeout);




/*******************************************************************
 **                     Semaphore definitions                     **
 *******************************************************************/
/**
@brief �����ź���

@param[in] pcName �ź�������,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�������ź�������ʧ��
@param[in] nInitialCount ��Ч�ź��������൱��PVԭ���еĳ�ʼֵ
@param[in] nMaxCount ����ź�����
@param[out] phSemaphore ���ɵ��ź������
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ��ش������ֵ����phSemΪCSUDI_NULL
@note 
- pcName����Ӱ�촴���ź����ɹ���񣬼�����ΪCSUDI_NULL�������ظ�,�����Ƽ�������
- �ж������ȴ�ͬһ���ź���ʱ,һ�����ź������ͷ�,��ȷ���ĸ�������Գɹ�����ź���
- �ź�������Ӧ����ո�
@todo Ŀǰ�����ƽ̨��nMaxCount��Ч���벻Ҫʹ�ã�nMaxCountΪ���޴�С

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemCreate(const char * pcName,int nInitialCount,int nMaxCount,CSUDI_HANDLE * phSemaphore);

/**
@brief �����ź���

@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@warning ����һ�����ڱ���������ȴ������ź�����һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemDestroy(CSUDI_HANDLE hSemaphore);

/**
@brief �ȴ��ź������൱��P����(P��������Դ)

@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@param[in] uTimeout �ȴ���ʱʱ�䣬���뵥λ��Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش������

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemWait(CSUDI_HANDLE hSemaphore,unsigned int uTimeout);

/**
@brief �ͷ��ź������൱��V����(V���ͷ���Դ��
@param[in] hSemaphore �ź����������CSUDIOSMsgQueueCreate���������phMsgQueue
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@note ���ź������ͷŴ����ﵽ������ʱ������CSUDIOS_ERROR_UNKNOWN_ERROR

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSSemRelease(CSUDI_HANDLE hSemaphore);




/*******************************************************************
 **                     Mutex definitions                         **
 *******************************************************************/
/**
@brief ������Ƕ�׵Ļ�����

@param[in] pcName ����������,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�������ź�������ʧ��
@param[in] uOptions ������ѡ��,Ŀǰ֧��:(��ѡ������)
- CSUDIOS_MUTEX_OBTAIN ������������������ø���(��λΪ0��CSUDIOS_MUTEX_DEFAULT��ʾ����ø���)
@param[out] phMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������,��phMutexΪCSUDI_NULL
@note 
- pcName����Ӱ�촴���������ɹ����,������ΪCSUDI_NULL,�����ظ�,�����Ƽ�������
- ͬһ�������Ѿ���һ��������������,���ٴζ������,��ʹ��Ƕ����,����μ�������Ҫ����ͷŲ����ͷŸ�����
- һ�������һ��������������,�������������п���������ʧ�ܡ�
- �����������ڸ߼�����,����������߼�����֮
- ����������Ӧ����ո�
- �ж������ȴ�ͬһ��������ʱ,һ���û��������ͷ�,��ȷ���ĸ�������Գɹ�����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexCreate(const char * pcName, unsigned int uOptions,CSUDI_HANDLE * phMutex);

/**
@brief ���ٻ�����

@param[in] hMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��س������
@warning ����һ�����ڱ���������ȴ����Ļ�������һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexDestroy(CSUDI_HANDLE hMutex);

/**
@brief �ȴ�������,����������

@param[in] hMutex ���������
@param[in] uTimeout �ȴ���ʱʱ��,���뵥λ,Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ�,���ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return �ɹ�����CSUDI_SUCCESS;��ʱ����CSUDIOS_ERROR_TIMEOUT;ʧ�ܷ��ش�����롣 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexWait(CSUDI_HANDLE hMutex,unsigned int uTimeout);

/**
@brief �ͷŻ�����,����������

@param[in] hMutex ���������
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note ��ͼȥ�ͷ�û�м��������Ѿ������Ļ�����������CSUDIOS_ERROR_UNKNOWN_ERROR����

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSMutexRelease(CSUDI_HANDLE hMutex);

/**
@brief ��ȡ��ϵͳ�����󾭹���ʱ��

@param[out] psTime ϵͳ�����󾭹���ʱ��,���CSUDIOSTimeVal_S���塣ΪCSUDI_NULL�򷵻�CSUDIOS_ERROR_BAD_PARAMETER
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������
@note 
- Ҫ��ȷ������ϵͳ�����ṩ����󾫶ȣ����ٵ�����,������ܻ��ϵͳ��������Ԥ�ϵ�Ӱ��!
- ʵ����Eastwin����������ʹ�øýӿڻ��ʱ��Ĳ�ֵ�����ڻ�ȡ���ε��õ�ʱ�����������ʱ���0ʱ���Ƿ��Ǵӿ�����һ˲�����𲢲���Ҫ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetTime(CSUDIOSTimeVal_S * psTime);




/*******************************************************************
 **                     Event definitions                         **
 *******************************************************************/
/**
@brief �����¼�

@param[in] pcName �¼�����,���Ȳ��ܳ���32���ֽڣ�������ȳ���32���ֽ��򷵻�CSUDIOS_ERROR_BAD_PARAMETER�����Ҵ����¼�ʧ��
@param[in] uFlags �¼�ѡ��,��־��ͬʱʹ��,Ҳ�ɷֱ�ʹ�á�����˲���Ϊ����ֵ�򷵻�CSUDIOS_ERROR_BAD_PARAMETER��
@note uFlags ��ȡֵ����(��������):
- CSUDIOS_EVENT_INITIAL_STATUS ��ʼ�����¼�������Ч�������Ͽ���Wait����(����λΪ0���ʾ�¼���ҪSet����Ч)
- CSUDIOS_EVENT_AUTO_RESET CSUDIOSEventWait֮���¼��Զ���Ϊ��Ч��(����λΪ0���ʾWait�¼����¼���Ȼ��Ч����������Reset)

@note ����ζ�����uFlags��ȡֵΪ:
- CSUDIOS_EVENT_AUTO_RESET | CSUDIOS_EVENT_INITIAL_STATUS ��ʼ�����¼�������Ч�������Ͽ���Wait����Wait֮���¼��Զ���Ϊ��Ч��
- 0 ��ʼ���¼�֮����ҪSetʹ�¼���Ч�����¼���reset֮ǰ��Զ��Ч��
@see CSUDIOSEventSet CSUDIOSEventReset
@param[out] phEvent �¼����
@return �ɹ�����CSUDI_SUCCESS;ʧ�ܷ��ش������,��phEventΪCSUDI_NULL
@note 
- pstrName����Ӱ�촴���¼��ɹ���񣬼�����ΪCSUDI_NULL�������ظ�,�����Ƽ�������
- �ж������ȴ�ͬһ���¼�ʱ��һ�����¼���Ч����ȷ���ĸ�����(��)�ȵ��¼�
- �¼�����Ӧ����ո�

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventCreate(const char * pcName,unsigned int  uFlags,CSUDI_HANDLE * phEvent);

/**
@brief �����¼�

@param[in] hEvent �¼����
@return ���hEvent��Ч����CSUDI_SUCCESS;���hEvent��Ч�򷵻�CSUDIOS_ERROR_BAD_PARAMETER
@warning ����һ�����ڱ���������ȴ������¼���һ����ȷ������Ϊ

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventDestroy(CSUDI_HANDLE hEvent);

/**
@brief �����¼���Ч

�¼���Ч��CSUDIOSEventWait������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventReset(CSUDI_HANDLE hEvent);

/**
@brief �����¼���Ч

�¼���Ч��CSWaitForSingleEvent����������
@param[in] hEvent �¼����
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventSet(CSUDI_HANDLE hEvent);

/**
@brief �ȴ��¼�

�¼���Чʱ��������;��Чʱ��������ֱ���¼���Ч���߳�ʱ
@param[in] hEvent �¼����
@param[in] uTimeout �ȴ���ʱʱ�䣬��λΪ���룬Ҳ��ѡ��:
- CSUDIOS_TIMEOUT_IMMEDIATE	���ȴ������ϳ�ʱ
- CSUDIOS_TIMEOUT_INFINITY	��Զ����ʱ
@return 
	   - ����ɹ�����CSUDI_SUCCESS
	   - ���hEvent���󷵻�CSUDIOS_ERROR_BAD_PARAMETER;
	   - ��hEvent��ȷ������£������ʱ�򷵻�CSUDIOS_ERROR_TIMEOUT��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSEventWait(CSUDI_HANDLE hEvent,unsigned int uTimeout);


/*******************************************************************
 **                     Memory definitions                        **
 *******************************************************************/
/**
@brief �����ڴ�,������ʼ��

�൱��malloc,�·�����ڴ��������ǲ�ȷ����
@param[in] uSize ��������ڴ��С
@return �ɹ������ڴ�ָ��;ʧ�ܷ���CSUDI_NULL
@note uSize����0ʱ����������ʧ�ܣ��ҷ���ֵΪCSUDI_NULL��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void * CSUDIOSMalloc( unsigned int uSize );

/**
@brief ���·����ڴ�

�൱��realloc,���·����ڴ�Ϊ�µĴ�С,�����������������,������ڴ�ռ�ȾɵĴ�,���������ֵ������ǲ�ȷ����
@param[in] pvAddr �����·�����ڴ��ַ
@param[in] uSize ��������ڴ��С
@return �ɹ������ڴ�ָ��;ʧ�ܷ���CSUDI_NULL
@note 
- uSize����0ʱ�ú���ִ��ʧ��,������CSUDI_NULLҲ������һ����ַ��
- ���pvAddr����һ��֮ǰCSUDIOSMalloc,CSUDIOSRealloc���صĵ�ַ,����ǲ�ȷ����
- ���pvAddr==CSUDI_NULL,������CSUDIOSMallocһ��
- �������ʧ��,�ɵ��ڴ治���Զ��ͷ�,������Ҳ�������仯������ֵ������pvAddr��һ��,Ҳ����һ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSRealloc( void * pvAddr,unsigned int uSize );

/**
@brief �����ڴ棬����ʼ��

�൱��calloc������nElements��nElementSize��С��objects,�����ڴ�ȫ����Ϊ0
@param[in] uElements �������Ԫ�ظ���
@param[in] uElementSize �������ÿ��Ԫ�صĴ�С
@return ����ɹ������ڴ�ָ��;���ʧ�ܷ���ΪNULL
@note nElements��nElementSize����0ʱ�ķ���ֵ�ǲ�ȷ���ģ�������CSUDI_NULLҲ������һ����ַ�� 

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
void* CSUDIOSCalloc(unsigned int uElements, unsigned int uElementSize);

/**
@brief �ͷŷ�����ڴ�

�൱��free,�ͷ�CSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc������ڴ�
@param[in] pvAddr ���ͷŵ��ڴ��ַ
@return ����ɹ�����CSUDI_SUCCESS;���ʧ�ܷ���CSUDI_FAILURE
@note 
- ���pvAddr����һ��֮ǰCSUDIOSMalloc,CSUDIOSRealloc,CSUDIOSCalloc���صĵ�ַ,����ǲ�ȷ����
- ����ͷ�ͬһ���ڴ潫����ʧ��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is required for Android DVB-S2 plus OTT project.
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSFree( void * pvAddr );


/**
@brief ��ȡ���õ�RAM�ռ�
@param[out] pnFreeCount ϵͳ���õ�RAM�ռ�
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetFreeRAM(unsigned int * puFreeCount);

/**
@brief ��ȡCPU����
@param[out] pnCpuCount CPU�ĸ���
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUCount(unsigned int * puCpuCount);


/**
@brief ��ȡָ������CPU�Ĵ�����������(%)
@param[out] pnUsage CPU�����������ʣ�ȡֵ��ΧΪ0--1000;
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetCPUUsage(unsigned int index, unsigned int * puUsage);

/**
@brief ��ȡϵͳ����������ľ��
@param[out]  hThread ϵͳ����������ľ������
@param[in/out]   nSize hThread����Ĵ�С������ʵ��Thread�ĸ���
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetAllThreadHandle(CSUDI_HANDLE* phThread, unsigned int*  puSize);

/**
@brief ��ȡָ������Ļ�����Ϣ�������������ơ�����ID���������ȼ�
@param[in]   hThread ָ������ľ��
@param[out]  psThreadInfo ����Ļ�����Ϣ
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��psThreadInfo��ΪNULL��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadInfo(CSUDI_HANDLE hThread, CSUDIOSThreadInfo_S* psThreadInfo);


/**
@brief ��ȡָ�������RAM����
@param[in]   hThread ָ������ľ��
@param[out]  pnUsedRam ָ�������RAM����
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadUsedRAM(CSUDI_HANDLE hThread, unsigned int * puUsedRam);


/**
@brief ��ȡָ�������CPU������(%)
@param[in]   hThread ָ������ľ��
@param[out] pnUsage ָ�������CPU���أ�ȡֵ��ΧΪ:0--1000
@return ��ȡ�ɹ��򷵻�CSUDI_SUCCESS��ʧ���򷵻�CSUDI_FAILURE��

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP API
	2013/11/06
		This API is NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

*/
CSUDI_Error_Code CSUDIOSGetThreadCPUUsage(CSUDI_HANDLE hThread, unsigned int * puUsage);


#ifdef	__cplusplus
}
#endif
/** @} */

#endif
