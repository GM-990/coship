/** @addtogroup CSIR

@version CS_IR_0.2
- ���Ӽ�ֵ��UserId����ϸ����

@{
*/
#ifndef _CS_IR_PLUS_H_
#define _CS_IR_PLUS_H_

#include  "udiplus_typedef.h"
#include  "cs_hdicommon_plus.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief ���ң��������*/
#define MAX_IR_EQUIPMENT	10

/**@brief enum of return values */
typedef enum _CSIRErrorCode_t
{
	CSIR_SUCCESS,
	CSIR_FAILURE,
	CSIR_ERR_BAD_PARAMETER,		/**< ��������*/
	CSIR_ERR_ALREADY_INIT,		/**< �Ѿ���ʼ��*/
	CSIR_ERR_EQUIPMENT_FULL,	/**< ��ӵ�ң�����Ѵ�����֧�ֵ����ֵ*/
	CSIR_ERR_EQUIPMENT_EXIST,	/**< Ҫ��ӵ�ң�����Ѿ�����*/
	CSIR_ERR_SEMPHA_CREATE,		/**< �����ź�������*/
	CSIR_ERR_TASK_CREATE,		/**< �����������*/
	CSIR_ERR_PIO_SET			/**< ����PIO����*/
}CSIR_Error_t;


/**@brief enum of IR protocols */ 
typedef  enum _CSIRProtocol_t
{
	CSIR_PROTOCOL_NEC,			/**< NECЭ�� */		
	CSIR_PROTOCOL_PHILIPS_RC6,	/**< Philips_RC6Э��*/
	CSIR_PROTOCOL_PHILIPS,		/**< PhilipsЭ�� */
	CSIR_PROTOCOL_TOPWAY,		/**< ��ͨ����1  ��ң����*/
	CSIR_PROTOCOL_RC5,			/**< Philips_RC5Э��*/
	CSIR_PROTOCOL_TOPWAY_II,	/**< ����2��ң���� */
	CSIR_PROTOCOL_TOPWAY_GZ,	/**< ��������Э�� */	
	CSIR_PROTOCOL_YIWU,			/**< �㽭����Э�� */
	CSIR_PROTOCOL_GD,  			/**< �㶫ʡ���µ�Э������ */
	CSIR_PROTOCOL_WUXI,         /**< ����ң����Э��*/
	CSIR_PROTOCOL_GD2000,		/**< GD2000Э�� ���ױ���*/
	CSIR_PROTOCOL_TC9012,		/**< TC9012Э�� ӡ��A3*/
	CSIR_PROTOCOL_RCMM,         /**< Philips rcmmң����Э��(ī�������telmex��Ŀ)*/
	CSIR_PROTOCOL_RF0,			/**< RFң����*/
	CSIR_PROTOCOL_NUM
}CSIRProtocol_t;

/**@brief enum of key status */
typedef enum _CSIRKeyStatus_t
{
	CSIR_KEY_PRESS,			/**< ��ʾ�������� */
	CSIR_KEY_REPEAT,		/**< ��ʾ�����ظ� */
	CSIR_KEY_RELEASE		/**< ��ʾ�������� */
} CSIRKeyStatus_t;

/**@brief repeat����*/
typedef enum _CSIRRepeatType_t
{
	CSIR_Press_Repeat,	/**< ��press״̬��repeat״̬*/
	CSIR_Repeat_Repeat	/**< ��repeat״̬��repeat״̬*/
}CSIRRepeatType_t;

/**@brief struct of the second parameter of KeySendCallback */
typedef struct _CSIRKeyParam_t
{
	HCSHANDLE		m_hdlIRHandle;		/**< ң������� */
	CSIRKeyStatus_t	m_enmIRKeyStatus;	/**< ����״̬����ʾ�������»��� */
	DWORD			m_dwIRLogicValue;	/**< �����Ĺ����� */
}CSIRKeyParam_t;

/**@brief struct of the second parameter of DebugCallback */
typedef struct _CSIRDebug_t
{
	CSIRKeyStatus_t	m_ucKeyStatus;		/**< ����״̬����ʾ�������»��� */
	DWORD			m_dwUserID;			/**< ң���������룬����ʶ��ң�������� */
	DWORD			m_dwKeyCode;		/**< ң���������ĵ������� */
}CSIRDebug_t;	


/**@brief ң�����������߼�ֵӳ��ṹ

@note ң�����еļ�ֵ��big-endian��ʽ���壬����������ֵ���ڴ�����Ϊ0x12|0x34����keyCode=0x1234
@note ��ֵΪң�����������ԣ����������м��������κ����ݣ���UserId�ȡ�
*/
typedef struct _CSIRDecode_t
{
	int 	m_nKeyValue;     		/**< real key value(scan code)*/
	int	 	m_nLogicValue;   		/**< logic value*/
	BOOL  	m_bIsRepeat;     		/**< whether to generate repeat keys*/
}CSIRDecode_t;

typedef struct _CSIREquipList_t
{
	CSIRProtocol_t	m_enmIRProtocol; /**< Э��*/
	DWORD			m_dwUserID;		 /**< �û���*/
    int 			m_nKeyNumber;	 /**< ������*/
	CSIRDecode_t *	m_ptKey;		 /**< ӳ���*/
} CSIREquipList_t;	

/**
@brief ң������ʼ�����û���Ҫָ��ң�����������������ȼ���

@param[in] nPriority  ָ��ң�����������������ȼ���
@return 
- CSIR_SUCCESS��ң������ʼ���ɹ�
- CSIR_ERR_ALREADY_INIT��ң����֮ǰ��ʼ�����ˡ�
- CSIR_ERR_BAD_PARAMETER�������������
@note ���ȼ�0-255
*/
CSIR_Error_t CSIRInitialize( int nPriority );

/**
@brief ң������ʼ�����û���Ҫָ��ң�����������������ȼ���ң�����̵߳�ջ��С��

@param[in] nPriority ָ��ң�����������������ȼ���
@param[in] nStackSize ָ��ң���������������ջ��С��
@return
- CSIR_SUCCESS��ң������ʼ���ɹ�
- CSIR_ERR_ALREADY_INIT��ң����֮ǰ��ʼ�����ˡ�
- CSIR_ERR_BAD_PARAMETER�������������
@note ���ȼ�0-255
*/
CSIR_Error_t CSIRInitialize_Ext( int nPriority, int nStackSize );

/**
@brief ����֧�ֵ�ң�����ͺţ���Ҫָ��ң������Э����û��롣
@param[in] ptIREquip ָ��ң�������ݽṹ��ָ�룬�ṹ��Ա����ң����Э�顢�û��롢��������ɨ�����빦�����ӳ���
@param[out] phIRHandle ����ң�����ľ����
@return
- CSIR_SUCCESS�����ң�����ɹ���
- CSIR_ERR_EQUIPMENT_EXIST���豸�Ѿ����ڡ�
- CSIR_ERR_EQUIPMENT_FULL���豸�б�������
- CSIR_ERR_BAD_PARAMETER�������������
@note �û����������MAX_IR_EQUIPMENT��ң������ 
@note ���ң����Э����û�ж�������(��UserId��KeyCode)���ֽ�����Ĭ�ϰ�little-endian��ʽ����������ڴ��е�����Ϊ0x12|0x34��UserId=0x3412
@note ���ִ���ʽ����Ϊ�����Ǵ�0λ��ʼ���α����յģ���16λ�����ݽ���˳��Ϊ:D0 D1 D2...D15
@note Ŀǰ��֪�Ĵ����ֽ����Э����:
- NECЭ����ʵ��Ӧ���о�����UserId��չ��16λ(����ȡ������)���򰴴˹����λ��ǰ��ԭUserId�ֽ���Ϊ��λ��UserId�����ֽ���Ϊ��λ������NECЭ���������һ�µ�
- TopwayIЭ���е�UserId��4�ֽڣ����˹����λ��ǰ��Ҳ���䶨����C0-C31�ķ���˳��һ��
- TopwayIЭ����KeyCode��UserId����ͬ�Ĺ���
@note ��ֵΪң�����������ԣ����������м��������κ����ݣ���UserId�ȡ�
*/
CSIR_Error_t CSIRAddEquipment( const CSIREquipList_t	* ptIREquip, HCSHANDLE* phIRHandle );


/**
@brief ɾ��֧�ֵ�ң�����豸����Ҫָ������豸ʱ���صľ����
@param[in] hIRHandle ָ������豸ʱ���ص�ң�����ľ����
@return
- CSIR_SUCCESS��ɾ��ң�����豸�ɹ���
- CSIR_ERR_BAD_PARAMETER�������������
- CSIR_FAILURE: ����ʧ��
*/
CSIR_Error_t CSIRDelEquipment(HCSHANDLE hIRHandle );


/**
@brief �����ڰ�����������ʱ���Ƿ���������ظ�

@param[in] nLogicValue Ҫ���õİ����Ĺ�����
@param[in] bIsRepeat TRUE��ʾ���������ظ���FALSE��ʾ��ֹ���������ظ�
*/
void CSIRSetSingleKeyRepeat( int nLogicValue, BOOL bIsRepeat );

/**
@brief ע��ң���������ص�������

�ú����������ã��û����Բ���Ҫ�����豸�Ϳ��Ի�ȡң�����Ļص���Ϣ�� 
@param[in] DebugCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�ң��������Ͱ�����Դ��״̬�͹�����
@return CSIR_SUCCESS��ע��ɹ���CSIR_ERR_BAD_PARAMETER�������������
*/
CSIR_Error_t CSIRRegisterDebugCallback( CSHDICallback DebugCallback );

/**
@briefע��ң���������ص�����������ң������������ɨ����ͨ��ע��Ļص�����֪ͨӦ�ò㡣

@param[in] KeySendCallback ָ��ص������ĺ���ָ�룬�û�ͨ���ص������õ�ң��������Ͱ�����Դ��״̬�͹����룬���CSHDICallback���塣
@return CSIR_SUCCESS��ע��ɹ�;CSIR_ERR_BAD_PARAMETER�������������
@note
*/
CSIR_Error_t CSIRRegisterKeySendCallback( CSHDICallback KeySendCallback );

/**
@brief ����ң�����ķ�Ӧ�ٶȡ�

@param[in] enmIRProtocolType Ҫ���õ�ң����Э�顣 
@param[in] enmIRRepeatType ����repeat������:��press��repeat ����repeat��repeat��
@param[in] nRepeatTimes ���ü�⵽repeat��Ϣ�Ĵ�����Ĭ��ֵ��
- NecЭ�飺press��repeat 2;repeat��repeat 0��
- Philip RC5Э��: press��repeat 4;repeat��repeat 1��
- Philip RC6Э��: press��repeat 2;repeat��repeat 1��
- Topway Э�飺press��repeat 2;repeat��repeat 0��
@return CSIR_SUCCESS:���óɹ���CSIR_ERR_BAD_PARAMETER:��������
*/
CSIR_Error_t CSIRSetRepeatSpeed(CSIRProtocol_t enmIRProtocolType, CSIRRepeatType_t enmIRRepeatType,int nRepeatTimes);

/**
@brief �������������ң����Э��

@param[in] enmProtocolIndex  ָ����ң����Э������
@return CSIR_SUCCESS�����óɹ� ��CSIR_FAILURE������ʧ��
@note �ýӿڽ�Ϊ���ֲ����Զ�ʶ��Э���ƽ̨���裬�����ڳ�ʼ��֮ǰ���ã���ʼ��������ֻ��֧�����һ�����õ�Э�飻��ʼ��֮��ýӿ���Ч
@note ���ڿ����Զ�ʶ��Э���ƽ̨���ýӿ���Ч
*/
CSIR_Error_t  CSIRSelectProtocol(CSIRProtocol_t  enmProtocolIndex);

/**
@brief get nkeyNum keys during dwTimeOut ms

@param[out] pKeyValue Logic value of key to detect
@param[in] dwTimeOut time out (ms)
@return CSIR_SUCCESS�����óɹ� CSIR_FAILURE������ʧ��
*/
CSIR_Error_t CSIRDetectKey(DWORD *pKeyValue, DWORD dwTimeOut);

/**
@brief�رպ���ң�ؽ��չ��ܣ�

���øú����󣬽�������Ӧң��������
@return CSIR_SUCCESS�����óɹ���
*/
CSIR_Error_t CSIRDisable ( void );

/**
@brief �򿪺���ң�ؽ��չ���

ֻ��CSIRDisable֮����ã����ڻָ�����ң�ؽ��չ��ܡ�
@return CSIR_SUCCESS�����óɹ���
*/
CSIR_Error_t CSIREnable( void );

/**
@brief ��ȡң����Դ��İ汾�š�

@param[in] pchVer ���ڴ�Ű汾��Ϣ
@param[in] nSize �汾��Ϣ�ĳ���
@return ���ذ汾����
@note �汾��ʽΪ��"[Module Name]-[Module Type]-[Version Number]-[Date]-[Time]\n"
*/
int CSIRGetVersion( PCHAR pchVer, int nSize );

#ifdef __cplusplus
}
#endif

/** @} */

#endif   

