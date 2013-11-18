#ifndef _CS_THIRDPARTY_MIS_H
#define _CS_THIRDPARTY_MIS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "udiplus_typedef.h"
#include "udi2_input.h"

typedef enum _CSMISCallbackMessageType_E
{
	MIS_SHOW_ADV = 0				// ��ʾ���ݹ��
	, MIS_HIDE_ADV					// �������ݹ��
}CSMISCallbackMessageType_E;		// Mis���ͻص�����

typedef enum _CSMISAdvType_E
{
	MIS_GENERAL_ADV = 0				// ��ͨ���ݹ��
	, MIS_CAPTION_ADV				// ��Ļ���ݹ��
}CSMISAdvType_E;					// ������� 

typedef enum _CSMISEpgAdvType_E
{
	MIS_CHANNELBAR_EPGADV = 0		// ��̨���λ(channelbar)
	, MIS_VOLBAR_EPGADV				// ���������λ(volbar)
	, MIS_OKLIST_EPGADV				// ��Ŀ�������λ(oklist)
	, MIS_PUB_CHANNEL_EPGADV		// ����Ƶ�����λ
	, MIS_FEE_CHANNEL_EPGADV		// ����Ƶ�����λ
	, MIS_NVOD_THEATRA_EPGADV		// �㲥��Ժ���λ
	, MIS_BROADCAST_EPGADV			// ���ֹ㲥���λ
	, MIS_VOD_CHANNEL_EPGADV		// vod���������λ
	, MIS_VOD_PAUSE_EPGADV			// vod��ͣ���λ
	, MIS_MENU_EPGADV				// ���˵����λ
	, MIS_UNDEFINE_EPGADV			// δ����Ĺ��λ
}CSMISEpgAdvType_E;				// ���Ĺ������



typedef struct _CSMISAdvInfo_S {
	int nIndex;			// ���Ĺ������
	int nWidth;			// �����͹������Ӧ�Ŀ�Ӧ�ο��ַ��淶
	int nHeight;		// �����͹������Ӧ�ĸߣ�Ӧ�ο��ַ��淶
} CSMISAdvInfo_S;

typedef struct _CSMISAdvConfig_S {
	int nAdvCount;		// ���Ĺ�����
	CSMISAdvInfo_S * psAdvConfigInfo;	// ���Ĺ��������Ϣ
} CSMISAdvConfig_S;

/**
@brief  ��ȡϵͳ�˵�͸����
@ 
*/
typedef int (*CSMISGetAlpha_F)(void);

typedef struct  _CSMISConfig_S
{	
	int nMisFreq;   			// ��ӹ�濪��Ĭ��Ƶ�㣬Ƶ��(mHz),����:443 mHz���粻��ע���������Ϊ-1
	int nMisSymb;	 			// Ĭ�Ϸ����ʣ������� (Ks/s)����6875Ks/s���粻��ע���������Ϊ-1
	int nMisModu;	 			// Ĭ��ģʽ��(1: 16Qam; 2: 32Qam; 3: 64Qam; 4: 128Qam; 5: 256Qam)���粻��ע���������Ϊ-1
	int nMisOpenPid ;			// Ĭ�Ͽ���Ӧ��PID���粻��ע���������Ϊ-1
	int nMisNormalPid ; 		// ��ͨӦ��PID���粻��ע���������Ϊ-1
	int nMisMoveInterval;		// ��Ļˢ��Ƶ�ʣ�ȡֵ��Χ-10~3��Ĭ��0,����ÿ40����ˢһ֡�����2.0�����ø�����Ч���粻��ע���������Ϊ0
	int nMisMovePixel; 			// ��Ļÿ֡�ƶ���������ȡֵ��Χ-2~3��Ĭ��0������ÿ֡�ƶ��������أ��粻��ע���������Ϊ0
	int nMisDefaultFromFreq; 	// ��ӹ��Ĭ����תƵ��,release�ⲻ��ע�������debug�����ʹ��
	int nMisSwitchToFreq;		// ָ����ӹ����ת��Ƶ��,release�ⲻ��ע����,����debug�����ʹ��
	int nMisShowIframeTime; 	// ��ӹ��I֡��ʾʱ������λ���룻�粻��ע���������Ϊ-1
	CSMISGetAlpha_F m_fnMisAlpha;	// ��ȡϵͳ�˵�͸���ȣ��粻��ע���������ΪNULL
} CSMISConfig_S;  				



/**
@brief  ��ȡ�������ݵĿռ��ַ
@param  pcAddress:  �����������ݿռ�ĵ�ַ
@param  pnSize�����ݴ�С��С��128�ֽ�
@return 0:��ʾ���óɹ���-1:��ʾ����ʧ��
*/
typedef int (*CSMISNvramInfo_F)(char **pcAddress, int *pnSize);

/**
@brief  ��ȡ��������
@param  nFlashAddress:  ��ȡ������ʼ��ַ
@param  pcBuffAddress��������ʼ���ݵ�ַ
@param  nBytes ����ȡ�����ֽ���
@return 0:��ʾ���óɹ���-1:��ʾ����ʧ��
*/
typedef int (*CSMISNvramRead_F)(unsigned int nFlashAddress, char * pcBuffAddress, int nBytes);

/**
@brief  ������д�뱣�����ݵĵ�ַ��DSM-CC������
@param  nFlashAddress:  �������ݵ���ʼ��ַ
@param  pcBuffAddress��д�����ݿ����ʼ��ַ
@param  nLength ����Ҫд����ֽ���
@return 0:��ʾ���óɹ���-1:��ʾ����ʧ��
*/
typedef int (*CSMISNvramBurn_F)(unsigned int nFlashAddress, const char * pcBuffAddress, int nLength);


typedef struct _CSMISNvramFunc_S 
{
	CSMISNvramInfo_F  m_fnInfo;
	CSMISNvramRead_F  m_fnRead;
	CSMISNvramBurn_F  m_fnBurn;
} CSMISNvramFunc_S;

typedef struct _CSMISFsPath_S
{
    char* psFsPath;
} CSMISFsPath_S;

typedef enum _CSMISOption_E
{
	EM_MIS_OPT_ADV_CONFIG = 1,	// �������Ĺ��������Ϣ��pvValue�ǽṹ��CSMISAdvConfig_S��ַ
	EM_MIS_OPT_NVRAM_REG_FUNC = 2,  // ���ûص�������pvValue�ǽṹ��CSMISNvramFunc_S��ַ
	EM_MIS_OPT_MIS_CONFIG = 3,	// ������ӹ�����ò���, pvValue�ǽṹ��CSMISConfig_S �ĵ�ַ
	EM_MIS_OPT_FS_PATH = 4 // ������ӿ���ͼƬ����·������ʽ:"/workdir/app/JFFS/rojia"��
} CSMISOption_E;


/**
@brief  ���ø������λ���������Ϣ

@param[in] eOption:  ���ò��� �ṹ�� CSMISOption_E ���ж���
@return TURE:��ʾ���óɹ���FALSE:��ʾ����ʧ��
@note �ýӿ�һ����Ҫ�ڵ�����ӹ���ʼ������CSMISInit()֮ǰ���á�
*/
BOOL CSMISSetOption(CSMISOption_E eOption, void * pvValue);

/**
@brief ��ʼ����ӹ��ϵͳ

@return TRUE:��ʼ���ɹ�; FALSE:��ʼ��ʧ��
@note �ýӿ�һ����ϵͳ��ʼ��ʱ����
*/
BOOL CSMISInit(void);

/**
@brief ������ӿ���Ӧ�ã��ú���Ϊ����ģʽ����ʾ�꿪��Ӧ�ú�ſɽ��к�������

@return TRUE:��������Ӧ�óɹ�; FALSE:��������Ӧ��ʧ��
@note �ýӿ�һ����ϵͳ��ʼ��ʱ����Ҫ���ã�ǰ��������CSMISInit�Ѿ����ù�
*/
BOOL CSMISStartBootApp(void);

/**
@brief �ַ�ң������������ӹ��ϵͳ

@param[in] eKeyState:������Ϣ״̬���簴�¡����𣬾���ο��μ�udi2_inpu.h��CSUDIInputKeyStatus_E����
@param[in] nMsg:����ֵ���μ�udi2_inpu.h����
@return TURE:��ʾ��ӹ��ϵͳ��ռ�������ð��������ٷ��͸��м����FALSE:��ʾ��ӹ��ϵͳ������˰������ð������������̴����������м��
@note �ýӿ�һ����ң���������¼����׸��м���ĵط����ã����̲ο����£�
-   if ( CS_MIS_DispatchInput(keystatus, psInputInfo->m_nKeyCode) == FALSE ) //��Ӳ�����˰�����Ϣ
-		{
-			nRet = MMCP_EvtSendEvent(0, &event, NULL);
-		}
*/
BOOL CSMISDispatchInput(CSUDIInputKeyStatus_E eKeyState, int nMsg);

/**
@brief ���ô��Ĭ�Ϲ��ͼƬ��·��

@param[in] pcPathName:Ĭ��ͼƬ��·��
@return TURE:�ɹ���FALSE:ʧ��
@note �ýӿ�һ������ӹ��ϵͳ��ʼ��֮����������;һ�㽫��Ŀ¼����ΪI֡���ڵ�Ŀ¼
*/
BOOL CSMISSetAdImagePath(const char * pcPathName);

/**
@brief 	�û��ص�,mis����Ϣ���ظ��û�

@param[in] enMsg:��Ϣ����
@param[in] dwPara:����
@return: 
@note:
enMsgΪMIS_SHOW_ADV��MIS_HIDE_ADVʱ��dwParaΪ������ͣ����ö������
*/
typedef void (* FOnCSMISCallback)(CSMISCallbackMessageType_E enMsg, DWORD dwPara);

/**
@brief 	ע��ص�����

@param[in] callback:�û��ص�����
@return TURE:�ɹ���FALSE:ʧ��
@note:
*/
BOOL CSMISRigisterCallback(FOnCSMISCallback callback);

/**
@brief 	��ȡ���Ĺ��id

@param[in] enType: ���Ĺ������
@param[out] puId:���Ĺ��id
@return TURE :�ɹ���FALSE :ʧ��
@note:
*/
BOOL CSMISGetEpgAdvId(CSMISEpgAdvType_E enType, UINT * puId);

#ifdef __cplusplus
}
#endif

#endif

