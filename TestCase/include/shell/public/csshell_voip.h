#ifndef CSSHELL_VOIP_H
#define CSSHELL_VOIP_H

#include "mmcp_typedef.h"
#include "udi2_rdi.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSSHELLVOIP_USERID_LEN	128 		///< VOIP�û�������
#define CSSHELLVOIP_PWD_LEN 	128 			///< VOIP�û����볤��

/**@brief  �Ự����ö��  */
typedef enum 
{
	CSSHELLVOIP_Session_Video = 0,   ///<��Ƶͨ��
	CSSHELLVOIP_Session_Audio = 1    ///<��Ƶͨ��
	
}CSShellVoipSessionType_E;

/**@brief  �Ự��Ϣ�ṹ�� */
typedef struct 
{
	CSShellVoipSessionType_E m_eCallType;
	char m_acUserId[CSSHELLVOIP_USERID_LEN];       ///< �û�ID ,����Ϊ�� '\0'	
}CSShellVoipSessionInfo_S;

/**@brief  ��¼ע����Ϣ���� */
typedef enum 
{
	EM_SHELL_VOIP_TYPE_START           = 50,      										///< voip ������ʼ           
	EM_SHELL_VOIP_TYPE_LOGIN_SUCCESS 		=EM_SHELL_VOIP_TYPE_START+1,	///< voip ��¼�ɹ���Ϣ
	EM_SHELL_VOIP_TYPE_LOGIN_FAIL                 	=EM_SHELL_VOIP_TYPE_START+2,	///< voip ��¼ʧ����Ϣ
	EM_SHELL_VOIP_TYPE_LCLLOGOUT_SUCCESS     =EM_SHELL_VOIP_TYPE_START+3,	///< voipע���ɹ���Ϣ
	EM_SHELL_VOIP_TYPE_SRVLOGOUT_SUCCESS   =EM_SHELL_VOIP_TYPE_START+4	///< voip ע��ʧ����Ϣ�򱻷�����߳�
	
}CSShellVoipLoginoutType_E;


/**@brief  ��¼ע����Ϣö�� */
typedef enum 
{
	/** @brief VOIP status code no error. */
	EM_SHELL_VOIP_ERR_NO                     =100,

	/** @brief VOIP status code of register error. */
	EM_SHELL_VOIP_ERR_LOCAL_REQ              	=EM_SHELL_VOIP_ERR_NO+101,	///�����������. 
	EM_SHELL_VOIP_ERR_REG_SEND_MSG           	=EM_SHELL_VOIP_ERR_NO+102,	///����ע����Ϣ���� 
	EM_SHELL_VOIP_ERR_REG_INVALID_USER       	=EM_SHELL_VOIP_ERR_NO+103, 	///��Ч���û���
	EM_SHELL_VOIP_ERR_REG_TIMEOUT           	=EM_SHELL_VOIP_ERR_NO+104, 	///��ʱ
	EM_SHELL_VOIP_ERR_REG_SERV_BUSY          	=EM_SHELL_VOIP_ERR_NO+105, 	///������æ 
	EM_SHELL_VOIP_ERR_REG_SERV_NOT_REACH  =EM_SHELL_VOIP_ERR_NO+106, 	///���������ɴ�
	EM_SHELL_VOIP_ERR_REG_SRV_FORBIDDEN     =EM_SHELL_VOIP_ERR_NO+107, 	///��ֹע��
	EM_SHELL_VOIP_ERR_REG_SRV_UNAVAILIABLE=EM_SHELL_VOIP_ERR_NO+108, 	///brief Register unavailiable. */
	EM_SHELL_VOIP_ERR_REG_OTHER              	=EM_SHELL_VOIP_ERR_NO+109 	///�������� 
}CSShellVoipLoginoutEvent_E;

/**@brief  �Ự��Ϣö�� */
typedef enum 
{
	EM_SHELL_VOIP_SESS_CALLIN		=EM_SHELL_VOIP_ERR_NO+201,		///�µ�����		
	EM_SHELL_VOIP_SESS_CALLOUT		=EM_SHELL_VOIP_ERR_NO+202,		///�����Ѻ���
	EM_SHELL_VOIP_SESS_ALERTED		=EM_SHELL_VOIP_ERR_NO+203,		///�Է�������
	EM_SHELL_VOIP_SESS_TALKING		=EM_SHELL_VOIP_ERR_NO+204,		///����ͨ����
	EM_SHELL_VOIP_SESS_TERM			=EM_SHELL_VOIP_ERR_NO+205,		///�Ự����
	EM_SHELL_VOIP_SESS_HOLDOK		=EM_SHELL_VOIP_ERR_NO+206,		///����ͨ���ɹ�
	EM_SHELL_VOIP_SESS_UNHOLDOK	=EM_SHELL_VOIP_ERR_NO+207,		///�������ͨ���ɹ�
	EM_SHELL_VOIP_POSTSRVEVNT		=EM_SHELL_VOIP_ERR_NO+208		///
}CSShellVoipSessionEvent_E;

/**@brief  �绰����Ϣö�� */
typedef enum 
{
	EM_SHELL_VOIP_PUSH_KEY			=EM_SHELL_VOIP_ERR_NO+301,								
	EM_SHELL_VOIP_HOOKOFFON		=EM_SHELL_VOIP_ERR_NO+302	
}CSShellVoipPhoneEvent_E;

/**@brief  ��Ƶ�豸����ö�� */
typedef enum 
{
	EM_SHELL_VOIP_AUDIODEV_SLIC	= 0,			///< slic ��Ƶ�豸  
	EM_SHELL_VOIP_AUDIODEV_USBMIC = 1,		///< usb mic ��Ƶ�豸  
	EM_SHELL_VOIP_AUDIODEV_USBWireless = 2,	///< usb ���ߵ绰
	EM_SHELL_VOIP_AUDIODEV_USBCard = 3, 		///< usb ����
	EM_SHELL_VOIP_AUDIODEV_COUNT			///< �����豸����
	
}CSShellVoipAudioDeviceType_E;

/**@brief  ��Ƶ�豸��Ϣ�ṹ�� */
typedef struct 
{
	DWORD	m_dwDeviceId;
	CSShellVoipAudioDeviceType_E m_eDeviceType;
	DWORD	m_dwCardNo;
	char	m_caName[32]; 
	
}CSShellVoipAudioDeviceInfo_S;

/**@brief  �����ϴ������� */
typedef enum 
{
	EM_SHELL_VOIP_SCREENCUT_TYPE_OSD	= 0,			///<OSD��
	EM_SHELL_VOIP_SCREENCUT_TYPE_VIDEO,		            ///<video��Ƶ��
	EM_SHELL_VOIP_SCREENCUT_TYPE_CAMERA,	                ///< ��������ͷ��Ƶ��
	EM_SHELL_VOIP_SCREENCUT_TYPE_ALL, 		            ///<osd��video��Ͻ�����������Ļ�����п��Կ�������Ϣ
	EM_SHELL_VOIP_SCREENCUT_TYPE_END
}CSShellVoipSrceenCutType_E;

/**@brief ����Ƶ¼�Ƶ����� */
typedef enum 
{
	EM_SHELL_VOIP_AVRECORD_TYPE_ALL	= 0,			    ///<ͬʱ¼������Ƶ
	EM_SHELL_VOIP_AVRECORD_TYPE_VIDEO,		            ///<ֻ¼����Ƶ
	EM_SHELL_VOIP_AVRECORD_TYPE_AUDIO,	                ///< ֻ¼����Ƶ
	EM_SHELL_VOIP_AVRECORD_TYPE_END
}CSShellVoipAvRecordType_E;

/*
*@brief voip��ʼ������,������һϵ�к���ָ�룬��Щ����ָ����Ӧ�õ�voipʵ��(C����)��Ȼ��ע���shell��
@see OTAInvoke*/

/**
@brief     voip��¼ע���ص�������
@param[in] hVoip  voip���
@param[in] eType �¼�����  ��CSShellVoipLoginoutType_E
@param[in] eCode �¼�����   ��CSShellVoipLoginoutEvent_E
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSShellVOIPLoginoutCallback_F)(HCSHANDLE hVoip, CSShellVoipLoginoutType_E  eType, CSShellVoipLoginoutEvent_E eCode, DWORD dwParam, void *pvUserData);
/**
@brief     voip�Ự�ص�������
@param[in] hVoip  voip���
@param[in] hSession �Ự���  
@param[in] eEvent �¼�����   ��CSShellVoipSessionEvent_E
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSShellVOIPSessionCallback_F)(HCSHANDLE hVoip, HCSHANDLE hSession, CSShellVoipSessionEvent_E eEvent, DWORD dwParam, void * pvUserData);

/**
@brief     voip�绰���ص�������
@param[in] hVoip  voip���
@param[in] eEvent �¼�����   ��CSShellVoipPhoneEvent_E
@param[in] nKeyVal  �����ļ�ֵ
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSShellVOIPPhoneCallback_F)(HCSHANDLE hVoip, CSShellVoipPhoneEvent_E eEvent, int nKeyVal, DWORD dwParam, void * pvUserData);

/**
@brief     voip��Ƶ�豸�ص�
@param[in] eDeviceType �豸���ͣ���� CSShellVoipAudioDeviceType_E
@param[in] eType �¼����ͣ���� CSUDIRDIEvent_E 
@param[in] dwDeviceId �豸id 
@param[in] pvUserData �û��Զ������ݣ�ע��ص�����ʱ�����
@return �ޡ�
*/
typedef int (*CSShellVOIPAudioDeviceCallback_F)(CSShellVoipAudioDeviceType_E eDeviceType, CSUDIRDIEvent_E eType, DWORD dwDeviceId, DWORD dwParam, void * pvUserData);

typedef struct 
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellVOIPInitParam)
	/**
	@brief	 ��ʼ��һ��voip  ʵ��
	@return VOIPʵ�������ʧ�ܷ���NULL
	*/
	HCSHANDLE (* m_fnOpen)();
		
	/**
	@brief	 ����VOIPʵ��	
	@param[in] hVoip  voip	 ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	
	*/ 
	int (* m_fnClose)(HCSHANDLE hVoip);
	/**
	@brief	 ע��voip֪ͨ�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	// ����ע���ص�
	int (* m_fnAddLoginoutListener)(HCSHANDLE hVoip ,  CSShellVOIPLoginoutCallback_F  fnCb, void* pUserData);
	
	/**
	@brief	 ɾ��voip�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb   voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnRemoveLoginoutListener)(HCSHANDLE hVoip, CSShellVOIPLoginoutCallback_F fnCb, void* pUserData);
	/**
	@brief	 ע��voip֪ͨ�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb  voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	// ����ע���ص�
	int (* m_fnAddSessionListener)(HCSHANDLE hVoip, CSShellVOIPSessionCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 ɾ��voip�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb   voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnRemoveSessionListener)(HCSHANDLE hVoip, CSShellVOIPSessionCallback_F fnCb, void* pUserData);
	/**
	@brief	 ע��voip֪ͨ�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb   voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	// ����ע���ص�
	int (* m_fnAddPhoneListener)(HCSHANDLE hVoip, CSShellVOIPPhoneCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 ɾ��voip�ص�������
	@param[in] hVoip  voip ���
	@param[in] fnCb   voip �ص�������
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnRemovePhoneListener)(HCSHANDLE hVoip, CSShellVOIPPhoneCallback_F fnCb, void* pUserData);
	
	/**
	@brief	 voip�û���¼���첽�ӿ�
	@param[in] hVoip  voip ���
	@param[in] pszUserId �û��������Ȳ��ܳ���CSSHELLVOIP_USERID_LEN
	@param[in] pszPassWord �û�����,���Ȳ��ܳ���CSSHELLVOIP_PWD_LEN
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����login �Ļص�
	*/
	int (* m_fnLogin)(HCSHANDLE hVoip, const char *pcUserId, const char *pcPassWord);
	
	/**
	@brief	 ע����ǰ�û�
	@param[in] hVoip  voip ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����logout �Ļص�
	*/
	int (* m_fnLogout)(HCSHANDLE hVoip);
	
	
	/**
	@brief	 ����ͨ������������һ���첽���̣��Ự�����ɹ���
			���ж�Ӧsession ��Ϣ��������
	@param[in] hVoip  voip	 ���
	@param[in] pszUserID �Է��û�ID,���Ȳ��ܳ���CSSHELLVOIP_USERID_LEN
	@param[in] eType �������� ��eType
	@param[out] phSession �Ự��������ڻ�ȡ�Ự������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����session �Ļص�
	*/
	int (* m_fnCall)(HCSHANDLE hVoip,const char * pcUserID, CSShellVoipSessionType_E eType, HCSHANDLE *phSession);
	
	/**
	@brief	 ����ͨ������������һ���첽���̣��Ự�����ɹ���
			���ж�Ӧsession ��Ϣ��������
	@param[in] hVoip  voip	 ���
	@param[in] pszIP�� �Է�IP��ַ
	@param[in] eType �������� ��eType
	@param[out] phSession �Ự��������ڻ�ȡ�Ự������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����session �Ļص�
	*/
	int (* m_fnCallIP)(HCSHANDLE hVoip, const char *pcIP, CSShellVoipSessionType_E eType, HCSHANDLE *phSession);
	/**
	@brief	 ��ȡ�Ự��Ϣ��
	@param[in] hSession  Session ���
	@param[out] psSessionInfo Session��Ϣ ��CSSHELLVOIPSessionInfo
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnGetSessionInfo)(HCSHANDLE hSession, CSShellVoipSessionInfo_S *psSessionInfo);
	/**
	@brief	 ��ȡ�Ự�Է��Ƿ�����Ƶ�����
	
	@param[in] hSession  Session ���
	@return  ����Ƶ�������TRUE�����򷵻�FALSE��
	@note	 �Է������Ự���룬����Է��Ƿ�����Ƶ�Ự���룬����TRUE
			 ��������Ự���룬����Է�Ӧ��λ��Ƶ�Ự������TRUE
	*/
	BOOL (* m_fnHasVideo)(HCSHANDLE hSession);
	/**
	@brief	 voip�������С�
	@param[in] hSession  �Ự������ɻص���������
	@param[in] eType �� CSShellVoipSessionType_E
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����session �Ļص�
	*/
	int (* m_fnAnswer)(HCSHANDLE hSession, CSShellVoipSessionType_E eType);
	
	
	/**
	@brief	 voipֹͣ�Ự������
	@param[in] hSession  �Ự������ɻص���������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����session �Ļص�
	*/
	int (* m_fnBye) (HCSHANDLE  hSession);
	
	/**
	@brief	 voip���ñ�����Ƶ���ڴ�С
	@param[in] hVoip  voip	 ���
	@param[in] x ������ʼ������
	@param[in] y ������ʼ������
	@param[in] w ���ڿ��
	@param[in] h ���ڸ߶�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetLocalVideoPos) (HCSHANDLE hVoip, int x,int y,int w, int h);

	/**
	@brief	 voip���öԷ���Ƶ���ڴ�С
	@param[in] hSession  �Ự���
	@param[in] x ������ʼ������
	@param[in] y ������ʼ������
	@param[in] w ���ڿ��
	@param[in] h ���ڸ߶�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetRemoteVideoPos) (HCSHANDLE hSession, int x,int y,int w, int h);
	
	/**
	@brief	 voip���ñ�����Ƶ�Ƿ�ɼ�
	@param[in] hVoip  voip	 ���
	@param[in] bVisible �Ƿ���ʾ��Ƶ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	
	int (* m_fnSetLocalVideoVisible )( HCSHANDLE hVoip, BOOL  bVisible);

	/**
	@brief	 voip���ñ�����Ƶ�Ƿ�ɼ�
	@param[in] hSession  �Ự���
	@param[in] bVisible �Ƿ���ʾ��Ƶ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	
	int (* m_fnSetRemoteVideoVisible )( HCSHANDLE hSession, BOOL  bVisible);	
	/**
	@brief	 voip���ô���Z��
	@param[in] pnUser	�û������ָ�룬����С�ı�ʾ��ǰ����ʾ��  
					0 ��ʾ���أ� ����ֵ�� hSession �������ĻỰ�Զ�
	@param[in] nNum   �û�����ĳ��ȡ�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetVideoZOrder )(HCSHANDLE  hVoip, int *pnUser,  int nNum);
	
	/**
	@brief	 voip���ñ���(����)����
	@param[in] hVoip  voip	 ���
	@param[in] bMute �Ƿ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetMicMute) (HCSHANDLE hVoip, BOOL  bMute);

	/**
	@brief	 voip���öԷ�(���)����
	@param[in] hSession  �Ự���
	@param[in] bMute �Ƿ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetSpkMute) (HCSHANDLE  hSession, BOOL  bMute);	
	/**
	@brief	 voip���ñ��أ���˷磩���� 
	@param[in] hVoip  voip	 ���
	@param[in] nVolume ��������ֵ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetMicVolume) (HCSHANDLE  hVoip, int nVolume);
	
	/**
	@brief	 voip���öԷ������ȣ�����  
	@param[in] hSession  �Ự���
	@param[in] nVolume ��������ֵ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetSpkVolume) (HCSHANDLE  hSession, int nVolume);
	
	/**
	@brief	 voip�����Ƿ񱣳�
	@param[in] hSession  Session�Ự���
	@param[in] bHold   �Ƿ���к��б���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	@note  �첽�ӿڣ�����session �Ļص�
	*/
	int (* m_fnHold )(HCSHANDLE hSession, BOOL bHold);
	
	/**
	@brief	 voip���ô��������
	@param[in] hVoip  voip	 ���
	@param[in] pszProxyServer	�����������ַ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetProxyServer) (HCSHANDLE  hVoip, const char *pcProxyServer);
	/**
	@brief	 voip����ע�������
	@param[in] hVoip  voip	 ���
	@param[in] pszRegisterServer   ע���������ַ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetRegisterServer) (HCSHANDLE  hVoip, const char *pcRegisterServer);

	/**
	@brief	 �õ�ָ���豸����Ϣ
	@param[out] ppsAudioDeviceInfo, �豸����Ϣ
	@param[in] pnDevicesCount,�������鳤��
	@param[out] pnDevicesCount,����豸������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnGetAudioDeviceInfo)(CSShellVoipAudioDeviceInfo_S *psAudioDeviceInfo, int *pnDevicesCount);

	/**
	@brief	 ���õ�ǰ���õ��豸
	@param[in] hVoip  voip	 ���
	@param[in] dwDeviceId �豸��id,ͨ��m_fnGetAudioDeviceInfo �õ�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnSetActiveDevice)(HCSHANDLE  hVoip, DWORD dwDeviceId);
	
	/**
	@brief	 ע���豸����
	@param[in] hVoip  voip	 ���
	@param[in] fnCb   �ص�����
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnAddAudoDeviceListener) (HCSHANDLE  hVoip, CSShellVOIPAudioDeviceCallback_F fnCb, void* pUserData);

	/**
	@brief	 ɾ���豸����
	@param[in] hVoip  voip	 ���
	@param[in] fnCb   �ص�����
	@param[in] pvEventData �û��Զ������ݣ�ע��ص�����ʱ�����
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnRemoveAudoDeviceListener) (HCSHANDLE  hVoip, CSShellVOIPAudioDeviceCallback_F fnCb, void* pUserData);

	/**
	@brief	 ���Ͷ��β��ż�ֵ
	@param[in] hVoip  voip	 ���
	@param[in] ucType   ���ż�ֵ
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnTone)(HCSHANDLE hVoip, BYTE ucType);

	/**
	@brief	 ��ȡָ���û�������״̬
	@param[in] hVoip  voip	 ���
	@param[in] pcUserId   �û�ID
	@return 0�������ߣ�1��������
	*/
	int (* m_fnGetOnlineState)(HCSHANDLE hVoip, const char* pcUserId);

	/**
	@brief	 �ϴ�������ȡ��osd��video��Ϣ�ϴ���ָ����������ַ��
	@param[in] hVoip  voip	 ���
	@param[in] eUploadType  �����ϴ�������
	@param[in] pcUrl  ָ����������ַ���ϴ���ʽΪhttp post��ʽ����ʽΪ: host:port�� Ĭ�϶˿�Ϊ80
	@note ���ӿ�Ϊͬ���ӿ�
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnScreenCutUpload)(HCSHANDLE hVoip, CSShellVoipSrceenCutType_E eSrceenCutType, char* pcUrl);

	/**
	@brief	 ��ʼ¼����Ƶ����Ƶ������Ƶ��Ϣ������¼�ƽ���ϴ���ָ����������ַ��
	@param[in] hVoip  voip	 ���
	@param[in] eAvRecordType  ¼������Ƶ������
	@param[in] pcUrl  ָ����������ַ���ϴ���ʽΪhttp post��ʽ����ʽΪ: host:port�� Ĭ�϶˿�Ϊ80
	@note ���ӿ�Ϊ�첽�ӿ�
	@note ���ϴ���¼��
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnAvRecordStart)(HCSHANDLE hVoip, CSShellVoipAvRecordType_E eAvRecordType, char* pcUrl);

	/**
	@brief	 ��ͣ¼��
	@param[in] hVoip  voip	 ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnAvRecordPause)(HCSHANDLE hVoip);

	/**
	@brief	 �ָ�¼��
	@param[in] hVoip  voip	 ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnAvRecordResume)(HCSHANDLE hVoip);

	/**
	@brief	 ֹͣ¼�ƣ����¿�ʼ¼�Ʊ������start
	@param[in] hVoip  voip	 ���
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	int (* m_fnAvRecordStop)(HCSHANDLE hVoip);
}CSShellVoipInitParam_S;

/**
@brief ���� VOIP �ӿڲ���
@param psParam  VOIP  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellVoipSetParam(CSShellVoipInitParam_S * psParam);

/**
@brief ��ȡ VOIP �ӿڲ���
@param ��
@�ɹ�����CSShellVoipInitParam_S,ʧ�ܷ���NULL
*/
const CSShellVoipInitParam_S* CSShellVoipgetParam();


#ifdef __cplusplus
}
#endif

#endif

