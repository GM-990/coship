/**@defgroup EW200 EW200

@brief ��ģ��Ϊ�ٻ�200�м����Ӧ�ò�ӿ�,�����ɰٻ�200��Ʒʱʹ��
@note �����ٻ�200����UDI,����δ��ע����,��μ�UDI�ĵ�
@{
*/
#ifndef _CS_EW_PUBLIC_H_
#define _CS_EW_PUBLIC_H_

#include "udi2_typedef.h"
#include "udi2_public.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**@mainpage EestWin200 User Interface

@version <B>EW200V0.4</B> 2009/03/25
- �����ӿ�λ�ã�����ʼ����ʹ�ýӿڷֿ���������������ɼ�ʹ��
- �޸�CSEWVODGetEpgEvent�ӿ�,ʹ֮�������ʹ�ú�ʵ��
- �޸�dvbЭ���URL,���ڸ�Э���URL��ʹ��16���Ʊ�ʾ

@version <B>EW200V0.3</B> 2009/03/20
- �����������,��EWShell���,ʹ���ǿ��Զ�������,��չ

@version <B>EW200V0.2</B> 2009/03/16
- �����������,����ģ���ļ���������ϵ

@version <B>EW200V0.1</B> 2009/03/11
- ��ʼ�汾
*/

/**
@brief ����ٻ�200ͨ����Ϣ�����ṹ������������������Ϣ����ȷ��?
@see CSEWEventType_E
@see CSEWMsgType
@note ��CSEWEventType_Eһ��ʹ��ʱ���京����:���=����(����)����ʽ����������������ο�CSEWEventType_E
@note ��CSEWMsgTypeһ��ʹ��ʱ����ΪCSEWCallback_F�����׳����京������(���������У�psParam->px���Ϊpx):
����1) dwMsg=EM_EW_OCʱ����ʾ���ݹ㲥��Ϣ����ʱ,
		p0=EM_EW_OC��p1ΪCSEWOCMsgType��p0��p1��Ȼ����ָ����ʽ�������ģ�����ʵ������int�ģ�ʹ��ʱ�밴��int��ֵ
		���ã�������Ϣ�����û����ݲ�ͬ����������м���ĵ���ʾ��ӿڵ�����Ӧ��ʾ��p2=NULL,p3=NULL��
		
����2) dwMsg=EM_EW_NETWORKʱ����ʾ��������ʧ�ܣ���ʱ
		p0Ϊ�������ӵĴ����룬��ö��CSEWNetworkMsgType����������ʧ��ʱ���м���и�Ĭ�ϴ������һ��"������ʧ
		��"�Ĵ���ҳ�棻p1Ϊһ�������������ʾ�û��Ƿ�Ҫ�Լ�������������ʧ�ܣ�����û�Ҫ�Լ������뽫�����������Ϊ
		4���м���Ͳ���򿪴���ҳ���ˡ���Ŀǰ����Ϣ���û�ʵ�ֲ��ֻ��������⣬���Խ�����ʱʹ���м����Ĭ�ϴ�����
		p2=NULL,p3=NULL��

����3) dwMsg=EM_EW_WINDOW_CLOSEʱ����ʾ�м����ҳ����ͨ��Window.close()�˳���������û����յ�����Ϣʱ���Ϳ��Ե���CSEWHide��
		���м������ʱ��psParam=NULL.


    4) dwMsg=EM_EW_SYSTEM_OPENURLʱ����ʾ�м����һ��url������Ϣ����Ϊ֪ͨ�û���һ�㲻��Ҫ�û�������ʱ,psParam=NULL.

����5) dwMsg=EM_EW_SYSTEM_GOBACK��ʾ�м��������һ�η��صĲ���������Ϣ����Ϊ֪ͨ�û���һ�㲻��Ҫ�û�������ʱ,psParam=NULL.
����6) dwMsg=EM_EW_SYSTEM_REPAINT��ʾ�м��������һ���ػ�Ĳ���������Ϣ����Ϊ֪ͨ�û���һ�㲻��Ҫ�û�������ʱ,psParam=NULL.
����7) dwMsg=EM_EW_SYSTEM_REFRESH��ʾ�м��������һ��ˢ�µĲ���������Ϣ����Ϊ֪ͨ�û���һ�㲻��Ҫ�û�������ʱ,psParam=NULL.
����8) dwMsg=EM_EW_SYSTEM_EXIT��ʾ֪ͨ�û��м���յ����˳���Ϣ���û����յ�����Ϣʱ����Ҫ�ж�pPara0�����pPara0=0�Ϳ��Ե���
		CSEWHide�����м����pPara0��0���ʾ����Ҫ�˳����û�����Ҫ���κδ���
    
������������Ϣ���ͣ��м����ʱû��ʹ�ã��û�����Ҫ������
�������г���dwMsg ΪEM_EW_NETWORKʱp1Ϊһ�������������ָ�룬����������£�p0��p1����һ��int��ֵ���û�ʹ��ʱ��Ҫǿ��ת��Ϊ
	int��ֵ��

@note �����ر�˵��,����ָ��ָ�����������Ϣ���غ�����Ч.
*/
typedef struct
{
    void* pPara0;                         ///< ����0,���p0
    void* pPara1;                         ///< ����1,���p1
    void* pPara2;                         ///< ����2,���p2 Ԥ��
    void* pPara3;                         ///< ����3,���p3 Ԥ��
} CSEWParam_S;


/**
@brief ����ٻ�200ͨ��������е���������ṹ
�ڵ������� ����ȷ�ϣ�ȡ������ʱ�Ĵ���
*/
typedef enum _CSEWMsgActionType_E
{
    EM_EW_BUTTON_ENTER = 1,  	///< ����OK �¼�
    EM_EW_BUTTON_CANCEL,		///< ����CANCEL �¼�
    EM_EW_BUTTON_TIMEOUT	        ///< ����ʱ �¼�
} CSEWMsgActionType_E;

/**
@brief ����ٻ�200ͨ��������е���������ṹ
�������а�ť�ĸ���
*/
typedef enum _CSEWMsgBoxType_E
{
    EM_EW_BUTTON_NULL 	= 0,	///< ���������ͣ���button
    EM_EW_BUTTON_OK 	= 1,	/// ���������ͣ�ok button
    EM_EW_BUTTON_OKCANCEL	= 2	///< ���������ͣ�Cancel sure button
} CSEWMsgBoxType_E;

/**
 @brief ����ٻ�200ͨ��������е���������ṹ
 ���������Ķ��뷽ʽ
 */
typedef enum _CSEWMsgBoxAlign_E
{
    EM_EW_MSG_ALIGN_LEFT	= 0,    ///< �����
    EM_EW_MSG_ALIGN_CENTER	= 1,    ///< ����
    EM_EW_MSG_ALIGN_RIGHT	= 2	///< �Ҷ���
} CSEWMsgBoxAlign_E;


/**@brief �ٻ�200�����¼�����

���ڷ��Ͱ���,ˢ��ҳ����¼�,���帳ֵ�����¼�����ȷ��.������������ΪCSEWParam_S
@see CSEWParam_S
@see CSEWSendMessagege
*/
typedef enum
{
    EM_EW_EVENT_UNDEFINED,               ///< δ����
    EM_EW_EVENT_KEYBOARD,                ///< ������Ϣ,�������CSEWParam_S��p0=GTKeyStatus_E(GTKeyStatus_E),p1=��ֵ(DWORD)
    EM_EW_EVENT_CMD                      ///< ������Ϣ,�������CSEWParam_S��p0=GTSystemCmd_E(GTSystemCmd_E)
} CSEWEventType_E;

/**
@brief �������������״̬

������������״̬,ÿ�ΰ������������ķ���Press,Release����.
��GTEventType_EΪEM_EW_EVENT_KEYBOARDʱ��Ч
@see CSEWSendMessagege
*/
typedef enum
{
    EM_EW_KEY_UNDEFINED,                 ///< δ����
    EM_EW_KEY_PRESS,                     ///< ��������ʱ���͸���Ϣ
    EM_EW_KEY_REPEATED,                  ///< �������º�һֱ���Ų���ʱ�ظ����͸���Ϣ
    EM_EW_KEY_RELEASE                    ///< �����ɿ�ʱ���͸���Ϣ
} GTKeyStatus_E;

/**
@brief �������������
��GTEventType_EΪEM_EW_EVENT_CMDʱ��Ч
@see CSEWSendMessagege
*/
typedef  enum
{
    EM_EW_CMD_UNDEFINED,                ///< δ����
    EM_EW_CMD_GOBACK,                   ///< ����
    EM_EW_CMD_GOFORWARD,                ///< ǰ��
    EM_EW_CMD_STOP,                     ///< ֹͣ
    EM_EW_CMD_REFRESH,                  ///< ˢ��
    EM_EW_CMD_REPAINT,                  ///< �ػ�(�����»�ȡҳ��)
    EM_EW_CMD_CLEARHISTORY,             ///<�����ʷ��¼
    EM_EW_CMD_SETHOMEPAGE,		    ///<������ҳ�湦��
    EM_EW_CMD_OPENURL,
    EM_EW_CMD_SHOW_MSGBOX,
    EM_EW_CMD_HIDE_MSGBOX,
    EM_EW_CMD_CLEARCACHE

} GTSystemCmd_E;

/**
@brief ����ٻ�200 �����֪ͨӦ�õ���Ϣ,��CSEWCallback_F�л��׸��û���������������ΪCSEWParam_S
@see CSEWRegisterCallback 
@see CSEWCallback_F
*/
typedef enum
{
    EM_EW_OC,               ///<  ���������ݹ㲥ʱ��Ӧ�ý��յ����������Ϣ,p1�����CSEWOCMsgType�ж��������
    EM_EW_NETWORK,			///<  ������http�����У�Ӧ�ý��յ��������Ϣ,p1�����CSEWNetworkMsgType�ж��������
    EM_EW_SYSTEM,			///< �������ϵͳ��,  p1 ����μ�CSEWSystemMsgType �ж���
    EM_EW_KEYCODETRANS,     ///<  ҳ�沶���ֵ������Ŀ��Ҫ����ת����p1 �����Ĭ�ϼ�ֵ���������p1ת����ļ�ֵ(int) �������
    EM_EW_WINDOW_CLOSE,     ///<  ͨ��Window.close() �˳������
    EM_EW_SYSTEM_OPENURL,	///<  ֪ͨӦ�����������ҳ���
    EM_EW_SYSTEM_GOBACK, 	///<  ֪ͨӦ��������������˳ɹ�
    EM_EW_SYSTEM_GOFORWARD,	///<  ֪ͨӦ�����������ǰ���ɹ�
    EM_EW_SYSTEM_REPAINT,	///<  ֪ͨӦ�������REPAINT �ɹ�
    EM_EW_SYSTEM_REFRESH, 	///<  ֪ͨӦ�������ˢ��ҳ��ɹ�
    EM_EW_SYSTEM_EXIT,	    ///<  ֪ͨӦ��������˳��Ĺ��ܴ�����Ϣ,���������ͣ�p0 Ϊ0����ʾ�˳������;p0Ϊ����ֵ����ʾ�˳�����������ҳ���ƣ�Ӧ�ò���Ҫ����
    EM_EW_SYSTEM_NOHISTORY  ///<  ֪ͨӦ������Ѿ��ص���ҳ
} CSEWMsgType;


/**
@brief ����ٻ�200 ��Ϣ�� ��OC�У������֪ͨӦ�õ���Ϣ����
@note  ���յ�EM_EW_OC�����Ϣ��p1 ��Ӧ����Ϣ����
*/
typedef enum
{
    EM_EW_CABLE_LOST = 1,      ///< Cable �ź��ж�
    EM_EW_CABLE_RESUME,  	   ///< Cable �źŻָ�
    EM_EW_OPEN_DVB_FAIL,	   ///< ��dvb����ʧ��
    EM_EW_OPEN_HOMEPAGE_FAIL,  ///< ��dvb��ҳʧ��
    EM_EW_OPEN_DVB_TIMEOUT,    ///< ��dvb���ӳ�ʱ
    EM_EW_AIT_VER_CHANGE	   ///< AIT�汾�仯
} CSEWOCMsgType;


/**
@brief ����ٻ�200 ��Ϣ�� ������http�����У�Ӧ�ý��յ��������Ϣ
@note  ���յ�EM_EW_NETWORK�����Ϣ��p1��Ӧ����Ϣ����
*/
typedef enum
{
    EM_EW_SOCKET_CONNECT_TIMEOUT    = 1001 , ///< socket���ӳ�ʱ
    EM_EW_COULDNT_RESOLVE_HOST      = 2001 , ///< �޷���������
    EM_EW_NO_DNS_SERVER             = 2002 , ///< �Ҳ���DNS������
    EM_EW_NO_PROXY_SERVER           = 3001 , ///< ���Ӳ���PROXY������
    EM_EW_PROXY_USER_PSD_WRONG      = 3002 , ///< PROXY�û����������
    EM_EW_HTTP_STATUS_CODE_100      = 4100 , ///< HTTP״̬��100
    EM_EW_HTTP_STATUS_CODE_101      = 4101 , ///< HTTP״̬��101
    EM_EW_HTTP_STATUS_CODE_201      = 4201 , ///< HTTP״̬��201
    EM_EW_HTTP_STATUS_CODE_202      = 4202 , ///< HTTP״̬��202
    EM_EW_HTTP_STATUS_CODE_203      = 4203 , ///< HTTP״̬��203
    EM_EW_HTTP_STATUS_CODE_204      = 4204 , ///< HTTP״̬��204
    EM_EW_HTTP_STATUS_CODE_205      = 4205 , ///< HTTP״̬��205
    EM_EW_HTTP_STATUS_CODE_206      = 4206 , ///< HTTP״̬��206
    EM_EW_HTTP_STATUS_CODE_300      = 4300 , ///< HTTP״̬��300
    EM_EW_HTTP_STATUS_CODE_301      = 4301 , ///< HTTP״̬��301
    EM_EW_HTTP_STATUS_CODE_303      = 4303 , ///< HTTP״̬��303
    EM_EW_HTTP_STATUS_CODE_304      = 4304 , ///< HTTP״̬��304
    EM_EW_HTTP_STATUS_CODE_307      = 4307 , ///< HTTP״̬��307
    EM_EW_HTTP_STATUS_CODE_400      = 4400 , ///< HTTP״̬��400
    EM_EW_HTTP_STATUS_CODE_401      = 4401 , ///< HTTP״̬��401
    EM_EW_HTTP_STATUS_CODE_402      = 4402 , ///< HTTP״̬��402
    EM_EW_HTTP_STATUS_CODE_403      = 4403 , ///< HTTP״̬��403
    EM_EW_HTTP_STATUS_CODE_404      = 4404 , ///< HTTP״̬��404
    EM_EW_HTTP_STATUS_CODE_405      = 4405 , ///< HTTP״̬��405
    EM_EW_HTTP_STATUS_CODE_406      = 4406 , ///< HTTP״̬��406
    EM_EW_HTTP_STATUS_CODE_407      = 4407 , ///< HTTP״̬��407
    EM_EW_HTTP_STATUS_CODE_408      = 4408 , ///< HTTP״̬��408
    EM_EW_HTTP_STATUS_CODE_409      = 4409 , ///< HTTP״̬��409
    EM_EW_HTTP_STATUS_CODE_410      = 4410 , ///< HTTP״̬��410
    EM_EW_HTTP_STATUS_CODE_411      = 4411 , ///< HTTP״̬��411
    EM_EW_HTTP_STATUS_CODE_412      = 4412 , ///< HTTP״̬��412
    EM_EW_HTTP_STATUS_CODE_413      = 4413 , ///< HTTP״̬��413
    EM_EW_HTTP_STATUS_CODE_414      = 4414 , ///< HTTP״̬��414
    EM_EW_HTTP_STATUS_CODE_415      = 4415 , ///< HTTP״̬��415
    EM_EW_HTTP_STATUS_CODE_416      = 4416 , ///< HTTP״̬��416
    EM_EW_HTTP_STATUS_CODE_417      = 4417 , ///< HTTP״̬��417
    EM_EW_HTTP_STATUS_CODE_500      = 4500 , ///< HTTP״̬��500
    EM_EW_HTTP_STATUS_CODE_501      = 4501 , ///< HTTP״̬��501
    EM_EW_HTTP_STATUS_CODE_502      = 4502 , ///< HTTP״̬��502
    EM_EW_HTTP_STATUS_CODE_503      = 4503 , ///< HTTP״̬��503
    EM_EW_HTTP_STATUS_CODE_504      = 4504 , ///< HTTP״̬��504
    EM_EW_HTTP_STATUS_CODE_505      = 4505   ///< HTTP״̬��505
} CSEWNetworkMsgType;

/**
@brief ����ٻ�200 ��Ϣ��
@note  ���յ�EM_EW_SYSTEM�����Ϣ��p0��Ӧ����Ϣ����
*/
typedef enum
{
    EM_EW_NOMEMORY,
    EM_EW_PARSERERROR,
    EM_EW_NOTINIT
} CSEWSystemMsgType;

/**
@brief ����ٻ�200ͨ��������е���������ṹ,������Ĵ���ص�
@param[in] enType ������Ķ���
@param[in] void *pUserData ����չ����
*/
typedef void (*CSEWMsgBoxActionCallBack_F)(CSEWMsgActionType_E enType, void *pData, void *pUserData);

/**
@brief ����ٻ�200ͨ�õ��������Բ����ṹ
*/
typedef struct _CSEWMsgBoxConfig_S
{
    unsigned int			dwMsgBoxId;     ///< �û���ʾ��id
    unsigned int			dwTimeOut; 	///< ms 0 ����ʱ
    char 			*pcBoxAption;   ///< ��ʾ���������
    char			*pcBoxContent;  ///< ��ʾ������
    int			nBoxAptionLen;	///< ��ʾ�����ĳ���
    int 			nBoxContentLen; ///< ��ʾ�����ݵĳ���
    void 			*pUserdata;     ///< �û����ݣ���m_fnMsgBoxCB�д��ظ��û�
    CSUDIWinRect_S		sRect;		///< Msg ��ʾλ��
    CSEWMsgBoxType_E 	emMsgType;  	///< type 0 - 2, button numble
    CSEWMsgBoxAlign_E       emAlign;		///< ���뷽ʽ��left = 0; right = 1;center = 2;
    CSEWMsgBoxActionCallBack_F  fnMsgBoxCallback;	///< ������Ļص�
} CSEWMsgBoxConfig_S;

/**
@brief ����ٻ�200��Ϣ֪ͨ�ص�����

�ٻ�200��ͨ���ûص����ϲ�Ӧ�ý��и�����Ľ���,��״̬֪ͨ,��Ϣ��ȡ��.
@param[in] dwMsg ��Ϣ����,ȡֵΪCSEWMsgType��ö��
@param[in] psParam ��Ϣ����,����������ͼ���Ϣ���Ͷ���
@param[in] pvUserData �û�����,���ڻص�ע��ʱ����Ӧ����
@return �ɹ��������Ϣ����1,���򷵻�0
@note �����ر�˵��,�뾡����Ҫ��ʱ�������ص�����,���齫�ص���Ϣ����Ϣ��ʽ�׳�����
*/
typedef int (*CSEWCallback_F)(unsigned int dwMsg, CSEWParam_S *psParam, void* pvUserData);



/**
@brief ������ӿ�,���յ�������ص���Ϣ ʱ����Ҫ������ʾ��ʱ��ʹ��
@param[in] psMsgBox �������ʼ������������ṹ�嶨��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note �ӿڽ��Խṹ����ǳ����,�������غ�pArgList�ṹ��������Ҫ
*/
CSUDI_BOOL CSEWShowMsgBox(CSEWMsgBoxConfig_S sMsgBox);


/**
@brief ����/���ذٻ�200�м��

����ʱ����Ҫʹ�ðٻ�200ʱ,�����Ӧ�ÿ��Ƶ�VODȫ������ʱ��������.
@param[in] bHide TRUEΪ����,FALSEΪ����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWHide(CSUDI_BOOL bHide);

/**
@brief �ٻ�200��Ϣ���ͽӿ�

������ٻ�200���͸��ֿ�����Ϣ,�簴��,ˢ�µ�.
@param[in] eEventType ��Ϣ����
@param[in] sEventParam ��Ϣ����,�������Ϣ����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note ��eEventTypeΪEM_EW_EVENT_KEYBOARD ���ͣ���Ҫ��ʼ��sEventParam��p0,p1;p0=GTKeyStatus_E(GTKeyStatus_E),p1=��ֵ(DWORD)
@note ��eEventTypeΪEM_EW_EVENT_CMD ���ͣ���Ҫ��ʼ��sEventParam��p0 ,p0��Ӧ��GTSystemCmd_E
*/
CSUDI_BOOL CSEWSendMessage(CSEWEventType_E eEventType, CSEWParam_S sEventParam);

/**
@brief ʹ�ðٻ�200��һ��URL

������ҳ���
@param[in] pUrl Ŀ��URL
@param[in] nLength URL����
@note ���øýӿڻ��Զ����ٻ�200������ٻ����������ǰ̨
@note ��׼�汾����http://��https://�ȿ�ͷ��URL
@note ��ѡdvb://networkid.tsid.serviceid(16����)��OC��ʽ���͵�ҳ��
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWOpenUrl(const char* pUrl, int nLength);


/**
@brief  ע���������Ӧ�õĻص�������

@param[in] fnCallback  �������Ӧ�õĻص�������,
@param[in] pUserdata �û�����
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
@note �ص�������dwMsg������CSEWMsgType��ȡֵ
*/
CSUDI_BOOL CSEWRegisterCallback(CSEWCallback_F fnCallback, void *pUserdata);

/**
@brief �˳��ٻ�200�м��

������Ҫʹ�ðٻ�200ʱ������Ӧ�ÿ��Ƶ�VODȫ������ʱ���˳����������DVB����.
Ӧ���յ�ң�����˳�����������CSEWShowMsgBox������ʾ����ʾ�û��Ƿ�Ҫ�˳���
Ӧ������ʾ��ص����������Ϣ�������ȷ�Ͼͷ���Ϣ��Ӧ�õ��̣߳�Ӧ�õ���CSEWExit�˳��м����
@note һ����Ҫ����ʾ��Ļص�����ֱ�ӵ���CSEWExit�������м���̻߳ᱻ������
@return �ɹ�����CSUDI_TRUE,ʧ�ܷ���CSUDI_FALSE
*/
CSUDI_BOOL CSEWExit();

#ifdef __cplusplus
}
#endif

/** @} */

#endif



