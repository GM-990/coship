/**@defgroup CSPower power ΪӦ�ò��ṩͳһ�ĵ�Դ���ƽӿ�
@version 0.1 2009/04/13 ��ʼ�汾
@{
*/

#ifndef _UDI_POWER_H_
#define _UDI_POWER_H_

/* Includes ----------------------------------------- */
#include "udi_typedef.h"
#include "udi_public.h"
#include "cs_ir.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef DWORD CSPOWER_Handle_t ;

/* Revision structure */
typedef const char * CSPOWER_Revision_t;

/**@brief �豸Ĭ�ϵķǷ����*/
#define CSPOWER_INVALID_HANDLE (CSPOWER_Handle_t) 0xFFFFFFFF

/**@brief CSPOWER ģ���ID�ţ���ID�����е�coship driverģ����Ӧ����Ψһ�ģ�Ŀǰ�ݶ�Ϊ9*/
#define CSPOWER_DRIVER_ID       9

/**@brief Base of CSPOWER Driver*/
#define CSPOWER_DRIVER_BASE     (CSPOWER_DRIVER_ID << 16)

/**@brief CSPOWERģ����ʼ�����*/
#define CSPOWER_ERROR_BASE     (CSPOWER_DRIVER_BASE)

/**@brief ÿһ��ע���profile���֧�ֵ���Ŀ��*/
#define MAX_ENTRIES_IN_LIST 100

/**@brief ͨ��open�ӿ����ɻ�õ�handle��*/
#define MAX_POWER_HANDLER (1)

/**@brief porting���ģ��ID��ʼֵ*/
#define MODULE_ID_PORTING_BASE (0)

/**@brief�м���ģ��ID��ʼֵ*/
#define MODULE_ID_MIDDLE_BASE    (1 << 8)

/**@briefӦ�ò��ģ��ID��ʼֵ*/
#define MODULE_ID_APPLICATION_BASE    (1 << 16)

/**@brief ������*/
typedef enum CSPOWER_ErrorCode_e
{
    CSPOWER_NO_ERROR = CSPOWER_DRIVER_BASE,          /**< 0 �޴��� */
    CSPOWER_ERROR_BAD_PARAMETER,                          /**< 1 ����Ĳ���*/
    CSPOWER_ERROR_NO_MEMORY,                                /**< 2 �ڴ����ʧ��   */
    CSPOWER_ERROR_UNKNOWN_DEVICE,                       /**< 3 δ֪���豸        */
    CSPOWER_ERROR_ALREADY_INITIALIZED,                  /**< 4 �豸�ѳ�ʼ�� */
    CSPOWER_ERROR_NO_FREE_HANDLES,                       /**< 5 û�п��е��豸���  */
    CSPOWER_ERROR_OPEN_HANDLE,                              /**< 6 �޷����豸 */
    CSPOWER_ERROR_INVALID_HANDLE,                          /**< 7 �Ƿ��ľ��       */
    CSPOWER_ERROR_FEATURE_NOT_SUPPORTED,            /**< 8 ��֧��ָ��������        */
    CSPOWER_ERROR_INTERRUPT_INSTALL,                        /**< 9 ��װ�ж�ʧ��  */
    CSPOWER_ERROR_INTERRUPT_UNINSTALL,                   /**< 10 ж���ж�ʧ�� */
    CSPOWER_ERROR_TIMEOUT,                                       /**< 11 ��ʱ            */
    CSPOWER_ERROR_DEVICE_BUSY,                                 /**< 12 �豸æ,��ʱ�޷�����  */
    CSPOWER_ERROR_DEVICE_PRESENT,			             /**< 13 �豸������ */
    CSPOWER_ERROR_DEVICE_NOT_PRESENT,		         /**< 14 �豸δ���� */
    CSPOWER_ERROR_PROFILE_NOT_REGISTER,			 /**< 15 δע���profile */
    CSPOWER_ERROR_DRIVER_NOT_INITIALIZED,	            /**< 16 �豸δ��ʼ�� */
    CSPOWER_ERROR_UNKNOWN                                       /**< 17 δ֪����*/
}CSPOWER_ErrorCode_E;

/**@brief ����ģ��ID*/
typedef enum CSPOWER_ModuleID_e
{
    CSPOWER_MODULE_ID_OS = MODULE_ID_PORTING_BASE,		/**< OSģ��ID*/
    CSPOWER_MODULE_ID_DEMUX,
    CSPOWER_MODULE_ID_VIDEO,
    CSPOWER_MODULE_ID_AUDIO,
    CSPOWER_MODULE_ID_OSG,
    CSPOWER_MODULE_ID_IFRAME,
    CSPOWER_MODULE_ID_SMC,
    CSPOWER_MODULE_ID_STORAGE,
    CSPOWER_MODULE_ID_QAM,
    CSPOWER_MODULE_ID_COFDM,
    CSPOWER_MODULE_ID_QPSK,
    CSPOWER_MODULE_ID_DISEQC,
    CSPOWER_MODULE_ID_PANEL,
    CSPOWER_MODULE_ID_VFD,
    CSPOWER_MODULE_ID_LCD,
    CSPOWER_MODULE_ID_GPIO,
    CSPOWER_MODULE_ID_I2C,
    CSPOWER_MODULE_ID_SPI,
    CSPOWER_MODULE_ID_UART,
    CSPOWER_MODULE_ID_EEPROM,
    CSPOWER_MODULE_ID_FLASH,
    CSPOWER_MODULE_ID_ENCODER,
    CSPOWER_MODULE_ID_RFMOD,
    CSPOWER_MODULE_ID_SCART,
    CSPOWER_MODULE_ID_IR,
    /*����porting��ģ���ID�ڴ˶���*/
    CSPOWER_MODULE_ID_PROTOCOL = MODULE_ID_MIDDLE_BASE,		/**< PROTOCOLģ��ID*/
    /*�����м��ģ���ID�ڴ˶���*/
    CSPOWER_MODULE_ID_EPG = MODULE_ID_APPLICATION_BASE,		/**< EPGģ��ID*/
    CSPOWER_MODULE_ID_USER,
    /*����Ӧ�ò�ģ���ID�ڴ˶���*/
    CSPOWER_MODULE_ID_MAX = 0x7FFFFFFF		                    /**< ģ��ID�����ֵ��ֻ���ڱ߽����*/
} CSPOWER_ModuleID_E;


/**@brief �����豸����,��ͬ��ģ���豸�ڵ�Դ����������в�ͬ�����ԣ�����Ŀǰ���������������࣬����ͳһ����ΪCSPOWER_DEVICE*/
typedef enum CSPOWER_ModuleType_e
{
    CSPOWER_DEVICE					/**< 0 ֧��CSPOWER���豸 */
} CSPOWER_ModuleType_E;


/**@brief ����ģ��״̬*/
typedef enum CSPOWER_State_e
{
    CSPOWER_DEVICE_STATE_D0=1<<1,	/**< D0 ����״̬*/
    CSPOWER_DEVICE_STATE_D1=1<<2	/**< D1 ����״̬ */
} CSPOWER_State_E;


/**@brief ����ģ��ĵ�Դ�������ӿ�. 
����ģ��Ӧ��ʵ���Լ��ĵ�Դ������������ע�ᵽCSPOWER ģ�顣
��Ӧ�ó�������ĳ��ģ���л���ָ����״̬ʱ��
CSPOWERģ�齫�ص�ָ��ģ��ע��ĺ�����
������������:
@param[in] State ָ��ģ��Ҫ�л�������״̬
@param[in] ModuleData_p ģ��ע��ʱ���ݵĲ������ص�ʱ��Ϊ�������ظ���Ӧ��ģ��
*/
typedef CSPOWER_ErrorCode_E (*CSPOWER_Func_t) (CSPOWER_State_E State ,const void * ModuleData_p);


/**@brief CSPOWER�ĳ�ʼ������*/
typedef struct CSPOWER_InitParams_s
{
    DWORD m_dwReserved; /* �������� */
} CSPOWER_InitParams_S;

/**@brief CSPOWER��open ����*/
typedef struct CSPOWER_OpenParams_S
{
    DWORD m_dwReserved; /* �������� */
} CSPOWER_OpenParams_S;

/**@brief CSPOWER��term ����*/
typedef struct CSPOWER_TermParams_s
{
   BOOL bForceTerminate;                   /*ǿ����ֹ����ϸ˵����CSPOWERTerm*/
} CSPOWER_TermParams_S;

/**@brief This define CPU states*/
typedef enum CSPOWER_CPUState_e
{
    CSPOWER_CPU_STATE_NORMAL,	/**< Normal״̬����������״̬ */
    CSPOWER_CPU_STATE_SLEEP,	       /**< Sleep״̬�� �͹���״̬(����Ӳ�����ܲ�֧��)*/
    CSPOWER_CPU_STATE_STANDBY	/**< Standby״̬������״̬ */
}
CSPOWER_CPUState_E;

/**@brief ����ԭ��, CPU��Ϊ����ԭ���Sleep����Standby״̬�ָ�����*/
typedef enum CSPOWER_WakeUpReason_e
{
    CSPOWER_WAKEUP_UNKNOWN 		= 0,		/**< δ֪�Ļ���ԭ��*/
    CSPOWER_WAKEUP_NMI 			= 1<<0,		/**< ͨ��Tuner����*/
    CSPOWER_WAKEUP_IR			      = 1<<1,		/**< ͨ��ң������������ */
    CSPOWER_WAKEUP_FRONTPANEL	= 1<<2,		/**< ͨ��ǰ��尴������ */
    CSPOWER_WAKEUP_ETHETNET		= 1<<3,		/**< ͨ�����ڻ��� */
    CSPOWER_WAKEUP_TIMER		      = 1<<4,		/**< ��ʱ������ */
    CSPOWER_WAKEUP_ASC			= 1<<5		/**< ͨ�����ڻ��� */
} CSPOWER_WakeUpReason_E;

/**@brief ����ң������ֵ�ṹ*/
typedef struct CSPOWER_Key_s
{
        CSIRProtocol_t	        m_enmIRProtocol; /**< Э��*/
        DWORD			m_dwUserID;			/**< ң���������룬����ʶ��ң�������� */
	DWORD			m_dwKeyCode;		/**< ң���������ĵ������� */
}CSPOWER_Key_S;

/**@brief ���廽�Ѽ�ֵ�ṹ,֧�ֶ��ֵ����*/
typedef struct CSPOWER_WakeUpKey_s
{
        CSPOWER_Key_S   *pstWakeUpKey;          /**< ң�������Ѱ���ֵ����ָ��*/
        DWORD                   m_dwWakeUpNum;               /**< ң�������Ѱ�������,�ó�Ա����pstWakeUpKeyָ���ֵ����Ĵ�С*/       
}CSPOWER_WakeUpKey_S;

/**@brief ��������wake up reasonʱ��userdate����*/
typedef struct CSPOWER_WakeUpUserData_s
{
    PVOID      pNmiUserData;                   /**< ͨ��Tuner����ʱ���û�����ָ�룬ָ���������δ����*/
    PDWORD   pdwIrUserData;                /**< ͨ��ң��������ʱ���û�����ָ�룬ָ��CSPOWER_WakeUpKey_S�����ݣ�������Ϊָ��ң���������������ֵ*/
    PDWORD   pdwFrontPanelUserData;    /**< ͨ��ǰ��廽��ʱ���û�����ָ�룬ָ��unsigned int�����ݣ�������Ϊָ��ǰ��尴���������ֵ*/
    PVOID      pEthetnetUserData;           /**< ͨ�����ڻ���ʱ���û�����ָ�룬ָ���������δ����*/
    PDWORD   pdwTimerUserData;           /**< ͨ����ʱ������ʱ���û�����ָ��,ָ��unsigned int�����ݣ�������ΪCPU�Ĵ���ʱ��,��λΪ��*/
    PVOID      pAscUserData;                  /**< ͨ�����ڻ���ʱ���û�����ָ��,ָ���������δ����*/
} CSPOWER_WakeUpUserData_S;


/**@brief ����POWER Profile����Ŀ����*/
typedef struct CSPOWER_ProfileEntry_s
{
    CSPOWER_ModuleID_E eModuleID;	        /**< ģ��ID*/
    CSPOWER_ModuleType_E eDeviceType;	/**< ģ������� */
    CSPOWER_State_E eModuleState;		/**< ָ��ģ���л�����״̬*/
} CSPOWER_ProfileEntry_S;





/*Functions -------------------------------------------------------------*/

/**
@brief ��ʼ��CSPOWERģ��

@param[in] psInitParams ��ʼ������ָ��
@return CSPOWER_NO_ERROR ��ʼ���ɹ�
@return CSPOWER_ERROR_ALREADY_INITIALIZED CSPOWER�Ѿ���ʼ��
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_NO_MEMORY û���㹻���ڴ�ռ�.
@note �˺���Ӧ����ϵͳ��ʼ����ʱ�����ȵ���
*/
CSPOWER_ErrorCode_E CSPOWERInit(CSPOWER_InitParams_S *psInitParams);

/**
@brief ��CSPOWER,�������õľ��

@param[in] psOpenParams open����.
@param[out] pHandle open�ɹ��󷵻ؿ��õľ��.
@return CSPOWER_NO_ERROR �ɹ�.
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_NO_FREE_HANDLES open�Ѵ���������޷��ٴ����µľ��.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED ģ��û�г�ʼ��.
@note �˺���Ӧ����CSPOWERInit֮��CSPOWERClose֮ǰ����
*/
CSPOWER_ErrorCode_E CSPOWEROpen(const CSPOWER_OpenParams_S *psOpenParams, CSPOWER_Handle_t *pHandle);


/**
@brief ����CSPOWEROpen�����ľ��.

@param[in] Handle CSPOWER���������ͨ��CSPOWEROpen���.
@return CSPOWER_NO_ERROR  �ɹ�.
@return CSPOWER_ERROR_INVALID_HANDLE ����ľ���Ƿ�.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED ģ��û�г�ʼ��.
@note �˺����ɹ�����֮��֮ǰ����ĺϷ��������Ϊ�Ƿ������ٿ���
*/
CSPOWER_ErrorCode_E CSPOWERClose(const CSPOWER_Handle_t Handle);

/**
@brief ��ֹCSPOWER����.

@param[in] psTermParams ��ֹ������ָ��.
@return CSPOWER_NO_ERROR �ɹ�.
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_OPEN_HANDLE ��δ�����ٵ�CSPOWER����������ڣ���������ֹ.
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWERģ��û�г�ʼ��.
@note �˺�����ֹCSPOWER������������е��ڲ�״̬�Լ��û�ע��ĺ�����profile
@note ��� ForceTerminate Ϊ TRUE ����ʹ��δ�����ٵ�CSPOWER����������ڣ���Ȼǿ����ֹ. 
@note ��� ForceTerminate Ϊ FALSE������δ�����ٵ�CSPOWER�����������ʱ������CSPOWER_ERROR_OPEN_HANDLE
*/
CSPOWER_ErrorCode_E CSPOWERTerm ( const CSPOWER_TermParams_S *  psTermParams);

/**
@brief ��ȡCSPOWERģ��汾��

@return �ַ�����ʽ��CSPOWER�汾��.
*/
CSPOWER_Revision_t CSPOWERGetRevision(void);

/**
@brief ע��profile

@param[in] Handle CSPOWER���������ͨ��CSPOWEROpen���.
@param[in] psProfile profile�б�ָ�룬ָ��CSPOWER_ProfileEntry_t���͵�����
@param[in] dwNumEntries profile�б��а�������Ŀ�������ж��ٸ�profile Entry
@param[out] pdwProfileID ��ע��ɹ�ʱ�������ڲ������profileID
@return CSPOWER_NO_ERROR �ɹ�.
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_NO_MEMORY û���㹻�Ŀռ�
@return CSPOWER_ERROR_UNKNOWN_DEVICE ProfileEntry��ָ�����豸�����ڣ����ߴ��ڵ���û����CSPOWERע���Դ������
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED profile��ָ����ĳ��ģ��ĵ�state��ģ�鲻֧��
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWERģ��û�г�ʼ��.
@note CSPOWERRegisterProfileֻ�ǽ�����ģ�����Ϣ����������
@note ֻ�е���CSPOWERSetProfileʱ�Ż�ͨ�����ø���ģ��ע��Ļص�����ִ�д������߻��Ѷ���
@note ��ͬһ��profileֻ��ע��һ�μ��ɣ����Զ��set
*/
CSPOWER_ErrorCode_E CSPOWERRegisterProfile(const CSPOWER_Handle_t Handle,
                                    CSPOWER_ProfileEntry_S *psProfile,
                                    DWORD dwNumEntries, DWORD *pdwProfileID);

/**
@brief ִ��profile

@param[in] Handle CSPOWER���������ͨ��CSPOWEROpen���.
@param[in] dwProfileID Profile ID��ͨ��CSPOWERRegisterProfile���
@return CSPOWER_NO_ERROR  �ɹ�.
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_INVALID_HANDLE ����Ƿ���handle
@return CSPOWER_ERROR_PROFILE_NOT_REGISTER ָ����profileû��ע��
@return ���������ɻص��������صĴ���
@note CSPOWERSetProfile����ø���ģ��ע��Ļص�����ִ��ʵ�ʵĶ�����
@note ����õ�˳���ϸ��ո���ģ����profile�г��ֵ�˳��ִ��
*/
CSPOWER_ErrorCode_E CSPOWERSetProfile(const CSPOWER_Handle_t Handle, DWORD dwProfileID);

/**
@brief ע���豸(ģ��ĵ�Դ������)

@param[in] eDeviceID ģ����.
@param[in] fnPowerFunc ��Դ��������
@param[in] nSupportedState ��ģ��֧�ֵĵ�Դ״̬����CSPOWER_State_t����֧�ֶ���
@param[in] pModuleData ģ���˽�����ݣ�����ע��ĺ���ʱ���ὫDeviceData_p��Ϊ��������
@return CSPOWER_NO_ERROR  �ɹ�.
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_DEVICE_PRESENT ָ��ID��ģ���Ѿ�ע���˵�Դ������
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWERģ��û�г�ʼ��.
@return CSPOWER_ERROR_NO_MEMORY û���㹻���ڴ�ռ�
@note ÿһ��ģ�鶼�������Լ��ĳ�ʼ�������е��ô˽ӿ�ע���Լ��ĵ�Դ������.
@note ÿ��ģ���ID��Ӧ��ȫ�ֿɼ��Ĳ�����ȫ��Ψһ��
*/
CSPOWER_ErrorCode_E CSPOWERRegisterPowerFunction(const CSPOWER_ModuleID_E eDeviceID,
                                CSPOWER_Func_t fnPowerFunc, INT nSupportedState, void *pModuleData);

/**
@brief �Ƴ���ע��ĵ�Դ������

@param[in] eDeviceID  ģ����
@return CSPOWER_NO_ERROR �ɹ�
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_DEVICE_NOT_PRESENT ָ��ID��ģ��û��ע���Դ������
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWERģ��û�г�ʼ��
@note ÿһ��ģ�鶼�������Լ�����Term �����е��ô˽ӿ�ע���Լ��ĵ�Դ������.
*/
CSPOWER_ErrorCode_E CSPOWERUnRegisterPowerFunction(const CSPOWER_ModuleID_E eDeviceID);

/**
@brief ��ȡCPU֧�ֵĻ��ѷ�ʽ .

@param[in] Handle CSPOWER���������ͨ��CSPOWEROpen���.
@param[out] pnWakeUpReason ����CPU֧�ֵĻ��ѷ�ʽ
@return CSPOWER_NO_ERROR �ɹ�
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED ��֧�ֵ�����
*/
CSPOWER_ErrorCode_E CSPOWERGetCPUSupportWakeUpReason(const CSPOWER_Handle_t Handle, INT *pnWakeUpReason);

/**
@brief ����CPU״̬����CPU�л���ָ����״̬

@param[in] Handle CSPOWER���������ͨ��CSPOWEROpen���.
@param[in] eCPUState ָ��CPUҪ�л���״̬
@param[in] nWakeUpReasonSelect�û����õĻ��ѷ�ʽ,������CPU֧�ֵķ�ʽ�����Զ��ַ�ʽ���
@param[out]  peWakeUpReason �������н���ʱ����CPU�����ѵ�ԭ��(��CPU���Ժ��ַ�ʽ�����ѵ�)
@param[in] pUserData ��������:
WakeUpReason_pΪCSPOWER_WAKEUP_TIMER(����ʱ����)ʱ��UserDataָ��unsigned int�����ݣ�������ΪCPU�Ĵ���ʱ��,��λΪ��
WakeUpReason_pΪCSPOWER_WAKEUP_IR(ң��������)ʱ��UserDataָ��unsigned int�����ݣ�������Ϊָ��ң���������������ֵ
WakeUpReason_pΪCSPOWER_WAKEUP_FRONTPANEL(ǰ��廽��)ʱ��UserDataָ��unsigned int�����ݣ�������Ϊָ��ǰ��尴���������ֵ
@return CSPOWER_NO_ERROR �ɹ�
@return CSPOWER_ERROR_BAD_PARAMETER ����Ĳ�������.
@return CSPOWER_ERROR_INVALID_HANDLE �Ƿ���handle
@return CSPOWER_ERROR_DRIVER_NOT_INITIALIZED CSPOWERģ��û�г�ʼ��
@return CSPOWER_ERROR_FEATURE_NOT_SUPPORTED ��֧�ֵ����ԣ�������CPU��֧���л���ָ����״̬���߲�֧��ָ���Ļ��ѷ�ʽ
*/

CSPOWER_ErrorCode_E CSPOWERSetCPUState(const CSPOWER_Handle_t Handle, CSPOWER_CPUState_E eCPUState, 
										INT nWakeUpReasonSelect, CSPOWER_WakeUpReason_E *peWakeUpReason, CSPOWER_WakeUpUserData_S* psUserData);



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _UDI_POWER_H_ */





