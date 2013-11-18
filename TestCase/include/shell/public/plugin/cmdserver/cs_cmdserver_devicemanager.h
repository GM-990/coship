#ifndef _CS_CMDSERVER_DEVICEMANAGER_H_
#define _CS_CMDSERVER_DEVICEMANAGER_H_

#include "cs_cmdserver.h"
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief �豸����
*/
typedef enum 
{
	EM_CMDSERVER_DEVMNG_USB,
	EM_CMDSERVER_DEVMNG_ETH
}CSCMDSERVERDeviceType_E;

/**
@brief �豸״̬����
*/
typedef enum
{
	EM_CMDSERVER_DEVMNG_USB_FREE,           /* USB is working normally */
	EM_CMDSERVER_DEVMNG_USB_RUNNING,             /* USB is not inserted */
	EM_CMDSERVER_DEVMNG_ETH_FREE,        /* Network is working normally */
	EM_CMDSERVER_DEVMNG_ETH_RUNNING        /* Network cable is not inserted*/
} CSCMDSERVERDeviceState_E;

/**
@brief �豸��Ϣ����
*/
typedef struct _CSDEVICEInfo
{
	char      	m_szDeviceName[128]; ///< �豸��Ψһ�豸���ƣ�����������豸Ӧ˵��������etn0��eth1
	char 	m_szMountPoint[256];  ///���ص㣬���������豸û�й��ص�
	CSCMDSERVERDeviceType_E m_eDeviceType;
	CSCMDSERVERDeviceState_E m_eDeviceState;
} CSCMDServerDeviceInfo_S;

/**
@brief �����豸״̬֪ͨ����ԭ��

@param[in] sMsgType Ҫ��ȡ�豸��Ϣ���豸����,��ϸ�����CSCMDSERVERDeviceType_E˵��
@param[in] ppsDeviceInfo ָ���豸���͵�����״̬��Ϣ,��ϸ�����CSCMDServerDeviceInfo_S˵��
@param[in/out] pnDeviceCount �豸������usb�ӿڸ�������eth������Ĭ����4��
@param[out] pvUserData �û�����,����CSCMDServerDevMngAddCallBackע��ûص�ʱ�����pvUserData��
@return ��ȡ�豸״̬�ɹ�����1,ʧ�ܷ���0�����pnDeviceCount�豸�������㷵��-1
@note
-�ûص������ɾ߱��ܻ�ȡ�豸״̬�����ģ��ʵ�֣����յ�֪ͨ�󣬸���sMsgType������\n
-��ȡ�豸������Ϣ�����豸������Ϣͨ��psDeviceInfo���أ�������豸������Ϣ��CSCMDServerDeviceInfo_S	\n
*/
typedef int (*CSOnCMDServerDeviceManager_F)(CSCMDSERVERDeviceType_E sMsgType, CSCMDServerDeviceInfo_S** ppsDeviceInfo, int* pnDeviceCount, void* pvUserData);

 /**
@brief ע���ȡ�豸״̬�ı���Ϣ�ص�

@param[in] fnDevMngCallBack�ص�����,�ɾ߱��ܻ�ȡ�豸״̬�����ģ��ʵ��
@param[in] pvUserData�û�����,����Ϊ�ص�������pUserData�������� 
@return ����ɹ�����TRUE,���򷵻�FALSE
@note
- 
*/
BOOL CSCMDServerDevMngAddCallBack(CSOnCMDServerDeviceManager_F fnDevMngCallBack, void* pvUserData);

 /**
@brief ɾ��ע���fnDevMngCallBack�ص�����

@param[in] fnDevMngCallBack�ص�����
@param[in] pvUserData�û�����,����Ϊ�ص�������pUserData�������� 
@return ����ɹ�����TRUE,���򷵻�FALSE
@note
- 
*/
BOOL CSCMDServerDevMngDelCallBack(CSOnCMDServerDeviceManager_F fnDevMngCallBack, void* pvUserData);
 
   /**
@brief ���豸״̬�б仯ʱ����֪ͨDeviceManagerģ��

@param[in] sMsgType �豸����
@param[in] psDeviceInfo �豸״̬��Ϣ
@param[in] pvUserData ˽�в�������Ϊ���� 
@return ����ɹ�����TRUE,���򷵻�FALSE
@note �ýӿ���Ӧ���������ã������ܻ�ȡ�豸״̬�仯��ģ�����
- 
*/
BOOL  CSCMDServerDeviceStateNotify(CSCMDSERVERDeviceType_E sMsgType, CSCMDServerDeviceInfo_S* psDeviceInfo, void* pvUserData);
   
/**
@brief �õ��豸����ģ���ʵ��

@param[in] ��
@return �ɹ����ض�Ӧ���ʵ���Ľṹ�壬���CSCmdInterface_S��ʧ�ܷ���NULL
@note 
- ��ϸ�����ɲμ�cs_cmdserver_avplayer.h
*/
CSCmdInterface_S* CSCMDServerGetDeviceManagerInstance( void );

/**
@brief��ʼ���豸������

@param[in] ��
@return �ɹ�����1��ʧ�ܷ���0
@note 
- ��ϸ�����ɲμ�cs_cmdserver_avplayer.h
*/
int CSPluginCMDServer_DeviceManagerInit(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif

