
#ifndef _CS_JSPorting_H_
#define _CS_JSPorting_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"
#include "cs_mediaplayer.h"

#ifdef MMCP_UDI2
#include "udi2_vout.h"
#endif

typedef void (*CSJSSearchCallback_F)(DWORD dwMessageType, const char* caJsonStr, int nLength);

typedef struct _JSSearchDataInfo_S{
	void* m_pUserData;
	CSJSSearchCallback_F m_fnSearchCallback;
}JSSearchDataInfo_S;

//CSGTIoctlWriteʱ�����keyֵ

//���keyֵ��Ӧ��value������
#define CS_JS_RESTORE_FACTORY_DEFAULT           "JS.RestoreFactoryDefault" 

//���keyֵ��Ӧ��value������
#define CS_JS_STANDBY                                           "JS.Standby"

//���keyֵ��Ӧ��value��ʽΪ"{appName:namevalue},{args:argsvalue}"
#define CS_JS_STARTAPP                                          "JS.StartApp"

//���keyֵ��Ӧ��valueֵΪ��Ŀ���,ʹ��ʱ�ɽ�char*����ת��Ϊ(HCSHANDLE)
#define CS_JS_SET_SERVICE_VOLUME                                    "JS.SetServiceVolume"

//���keyֵ��Ӧ��value������
#define CS_JS_RESTORE_FACTORY_DEFAULT           "JS.RestoreFactoryDefault" 

//���keyֵ��Ӧ��value������
#define CS_NGB_JS_RESTORE_FACTORY_DEFAULT		"JS.NGB.RestoreFactoryDefault" 


//��Ŀ���͵ĺ궨��
#define SERVICE_LIST_TYPE_SERVICE       0
#define SERVICE_LIST_TYPE_FAV           1
#define SERVICE_LIST_TYPE_BAT           2
#define SERVICE_LIST_TYPE_USEDEF        3
#define SERVICE_LIST_TYPE_HIDEATTR      4
#define SERVICE_LIST_TYPE_SAT      5
#define SERVICE_LIST_TYPE_FTA      6
#define SERVICE_LIST_TYPE_SCR      7
#define SERVICE_LIST_TYPE_SIMILAR_NAME      8
#define SERVICE_LIST_TYPE_TP     9

#define FILESTORAGE_MAX_DIR_LENGTH   256

#define DM_STATES_MAX_CALLBACK_COUNT  10

typedef enum _CSJSMediaplayerVolumeMode_E    //������������
{
	EM_JSMP_VOLUMEMODE_0_31,                 //ϵͳĬ��0-31
	EM_JSMP_VOLUMEMODE_0_100                 //����0-100
}CSJSMediaplayerVolumeMode_E;
typedef enum _CS_JSDATATYPE_E
{
    EM_JS_DATA_SYSPAR,		//ע��ϵͳ������صĶ������ڲ�ʹ��
    EM_JS_DATA_VOLCHA,         //��Ŀ����������
    EM_JS_DATA_COMDATA,        //�������ݣ�����Ԥ�����ݡ���ʱ���ػ����ݵ�
    EM_JS_DATA_NUMBER
}CS_JSDATATYPE_E;

/**
@brief DM״̬ö��
*/
typedef enum _CS_DM_STATES_E
{
    EM_DM_STATE_SEARCH_FINISHED,
    EM_DM_STATE_SAVE_TO_FLASH,
    EM_DM_STATE_REVERT_FROM_FLASH,
    EM_DM_STATE_RESTORE_FACTORY_DEFAULT
}CS_DM_STATES_E;

/**
@brief DM״̬�仯�ص�����
@param [in] dwUserData �û��Զ������ݣ���Ӧ��ע��ص�����ʱ������
@param [in] eStatus DM״̬
*/
typedef void (*CSDmStatesChangeCallback_F)(DWORD dwUserData, CS_DM_STATES_E eStatus);


/**
@brief ����ipģʽ�ص�����
@param [in] pUserData �û��Զ������ݣ���Ӧ��ע��ص�����ʱ������
@param [in] nEthernetIndex �û��Զ������ݣ���Ӧ��ע��ص�����ʱ������
@param [out] pcIpMode �ַ��� "DHCP" ����"MANUAL_CONFIG"
*/
typedef void (*CSEthernetModeCallback_F)(void *pUserData, int nEthernetIndex, char *pcIpMode);

typedef struct _CSDMStatesCallbackPair_S
{
	DWORD				dwUserData;
	CSDmStatesChangeCallback_F	fnCallback;
}CSDMStatesCallbackPair_S;

typedef struct _CSJSSearchParam_S
{
	int nBouquetID; 		//��������������
	int nDescriptorType; 	//����ָ��˽������������
	int nDescriptorTag;		//����ָ��˽��������tagֵ
	int anType[16];			//������������Ҫ����channel����Ľ�Ŀ����
	int nTypeCount;			//���������д���channel����Ľ�Ŀ���͸���
	int nSearchMode;		//����ģʽ(�Զ��������ֶ�������)
}CSJSSearchParam_S;

typedef struct
{
	DWORD m_dwFileNumber; ///< �ļ���ţ�����������
    DWORD m_dwFormat;     ///< gif, bmp, jpg, mpg��mpg��ָI֡��ʽ��
    DWORD m_dwXPos;         ///< x����
    DWORD m_dwYPos;			///< y����
    DWORD m_dwWidth;   ///< ���
    DWORD m_dwHeight;    ///< �߶�
    DWORD m_dwDuration; ///< ����ʱ�䣬��λ���룬Ϊ0��ʾһֱ��ʾ��
    DWORD m_dwVersion; ///< �汾
	int   m_nDataLen;    ///< ��Ӧ�����ݳ���
	BYTE *m_pucBuffer;	///< ��Ӧ����������
}CSSkyworthADInfo;


/*==========================================================
����ԭ��:CSJSPortingRegisterFileDir()
�������:
    eDataType: ������CS_JSDATATYPE_E����
    pcDir: ��Ӧ��eDataType�������ݵ��ļ��ı���Ŀ¼
    nLength: pcDir�ĳ��ȣ����Ȳ��ܳ���FILESTORAGE_MAX_DIR_LENGTH
�������:
    ��
����ֵ:  
    �ɹ� 0
    ʧ�� -1
���� :  
    ע��Ҫ���ļ�������������͵��ļ�Ŀ¼
ע������: 
    �����˴˷���˵��������Щ������Ҫ���ļ�ϵͳ�ķ�ʽ�����棬ע�⣬�˷����ݲ�֧����CSJSPortingRegisterFlashAddrͬʱʹ�õ����
    ����ֻ��ѡ��һ
=========================================================== */
int CSJSPortingRegisterFileDir(CS_JSDATATYPE_E eDataType, const char* pcDir, int nLength);


/*==========================================================
����ԭ��:CSJSPortingInit()
�������:
    ��
�������:
    ��
����ֵ:  
    �ɹ� 0
    ʧ�� -1
���� :  
    ��ʼ��JS native
ע������: 
    �˷����������
=========================================================== */
int CSJSPortingInit(void);

/******************************************************************************
Function Name:    JSProcessCAMessage
	
Description:
	����CA��Ϣ
	�������Ϣ���ݰ�����CӦ�����޷��������Ϣ���󴫵ݸ�ҳ�����Ϣ
Input:
	��
Output:
    None

Return Value:
	�ɹ�����0��ʧ��-1��
Other:
    �����CA��Ϣ��������ͨCA�У���ȡ���ܿ���Ϣ��ͨ����Ϣ�첽�ķ�ʽ�׸�ҳ�棬
    ����CӦ���У��޷������ܿ���Ϣ���js�����͸�ҳ�棬��ʱ��Ҫ���ô˷���������
*******************************************************************************/
int JSProcessCAMessage( DWORD dwType, DWORD wParam, DWORD lParam );

/******************************************************************************
Description:
	�ж�ָ����Ŀ�Ƿ�����ָ�����û�����
Input:
	hService : Ҫ�жϵĽ�Ŀ�ľ��
    nUserGroupID : �û��Զ�������ID����1��ʼ����ʾ�ڼ����û����飩
Output:
    None

Return Value:
	���ָ���Ľ�Ŀ���ڴ˷��飬 �򷵻�TRUE,���򷵻�FALSE
Other:
    ��
*******************************************************************************/
typedef BOOL (*CSJSServiceCanMatch_F)(HCSHANDLE hService, int nUserGroupID);

/******************************************************************************
Description:
	ע���ж�ָ����Ŀ�Ƿ�����ָ�����û�����Ļص�
Input:
	pfnMatchCallback : �ص�����ָ��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������ֵ
Other:
    ��
*******************************************************************************/
int CSJSRegisterUserGroupCallback(CSJSServiceCanMatch_F pfnMatchCallback);

/******************************************************************************
Description:
	�������ص�����
Input:
	hServiceLhs : ��Ŀ���
	hServiceRhs : ��Ŀ���
	nCompareUserData : ͨ��JSӦ��͸��������ֵ�����Ա�ʾָ�����������
Output:
    None

Return Value:
	���ǰһ����ĿӦ�����ں�һ����Ŀ֮���򷵻ش���0��ֵ
	���򣬷���С��0��ֵ
Other:
    ��
*******************************************************************************/
typedef int (*CSJSCompareService) ( HCSHANDLE hServiceLhs, HCSHANDLE hServiceRhs, int nCompareUserData );

/******************************************************************************
Description:
	ע���û��Զ����������ص�
Input:
	pfnCompareRule : �ص�����ָ��
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������ֵ
Other:
    ��
*******************************************************************************/
int CSJSRegisterCompareRule(CSJSCompareService pfnCompareRule);


/******************************************************************************
Description:
	���ݽ�Ŀ�������ͺ�ֵ���½�����Ŀ�б�,�����û��Զ�����飬����Ŀ���鷢���ı�ʱ��Ӧ�õ��ô˽ӿڡ�
Input:
	nListType:��Ŀ���ͣ�ȡֵΪ��Ŀ���͵ĺ궨�壬����ϲ��Ϊ:SERVICE_LIST_TYPE_FAV
	nValue:��Ŀ���Ͷ�Ӧ��ֵ���ο����ֵ��ӱ�׼������:TV��Ӧ��nValueȡ1,RADIOȡ2�ȵ�.
Output:
    None

Return Value:
	�ɹ�����0��ʧ�ܷ�������ֵ
Other:
    ��nListType��nValue������-1����ʾ���¹������еĽ�Ŀ�б�
*******************************************************************************/
int CSJSRebuildServiceList(int nListType, int nValue);

/******************************************************************************
Description:
	����Schedule epg����ģʽ��true��ʾֻҪһ������Schedule epg��ɺ󣬺���ֻ�Ǽ��Schedule epg����̨������������
       ��Ŀ�в����øú�����Ĭ��Schedule epg����ģʽΪbOnlyOnce = false

Input:
    bOnlyOnce:�Ƿ�ֻ����һ��schedule EPG
Output:
    None

Return Value:
    ��
*******************************************************************************/
void CSJSEPGSetScheduleEpgSearchMode(BOOL bOnlyOnce);

/**
@brief ���DM״̬�仯�ص�
@param [in] dwUserData �û��Զ�������
@param [in] fncallback DM״̬�仯�ص�
@return �ɹ�����TRUE, ʧ�ܷ���FALSE
@note ��fncallback��dwUserDataΪ�ؼ����ж��Ƿ��ظ����������ظ���ע��
*/
BOOL CSJSDmStatesAddCallback(DWORD dwUserData, CSDmStatesChangeCallback_F fncallback);

/**
@brief ɾ��DM״̬�仯�ص�
@param [in] dwUserData �û��Զ�������
@param [in] fncallback DM״̬�仯�ص�
@return �ɹ�����TRUE, ʧ�ܷ���FALSE
@��fncallbackΪ�ؼ���ɾ����ע��ĺ���
*/
BOOL CSJSDmStatesRemoveCallback(DWORD dwUserData, CSDmStatesChangeCallback_F fncallback);

/******************************************************************************
Description:
	�ָ���������

Input:
    None
    
Output:
    None

Return Value:
    ��
*******************************************************************************/
void CSJSRestoreFactoryDefault(void);
/********************************************************************************
Description: �˽ӿ���ΪJS Mediaplayer���� setVolume �������ã������ӿڶ���setVolume �����ֵΪ0-100
      �½ӿڶ���Ĵ���ֵ�ķ�ΧΪ0-31.���е���Ŀ���½ӿڶ���Ĳ��õ��ô˽ӿڣ��м��Ĭ�ϵ�Ϊ0-31��
      �����������Ŀ����Ҫ�ڹ����е��ô˽ӿ�CSJSMediaplayerSetVolumeMode(EM_JSMP_VOLUMEMODE_0_100)
Input:
      eMode: EM_JSMP_VOLUMEMODE_0_31 ��ʾ���������ֵ��ΧΪ0-31
  	  EM_JSMP_VOLUMEMODE_0_100 ��ʾ���������ֵ��ΧΪ0-100
Output:
      ��
Return Value:
      ��
      
**********************************************************************************/
void CSJSMediaplayerSetVolumeMode(CSJSMediaplayerVolumeMode_E eMode);

/******************************************************************************
Description:
	�˽ӿ���Ϊ��Ƶ�ɹ����Զ����£�����һ���Զ�����δ�����������и��£�������NIT����
Input:
	puDeliver��ǰ��Ƶ��Ƶ�����
Output:
    None

Return Value:
	None
*******************************************************************************/
void CSJSStartUpdate(Deliver *puDeliver);

/******************************************************************************
Description:
	�˽ӿ�����Ƶǰ���ã���Ҫ����ֹͣnit���������Զ�����δ���ʱֹͣ���θ���
Input:
	None
Output:
    None

Return Value:
	None
*******************************************************************************/
void CSJSStopUpdate();

/******************************************************************************
Description:
	��ȡ������������������ֵCSSearchParam_S�ж���
Input:
	None
Output:
    pstSearchParam ���������ṹ��
    
Return Value:
	None
*******************************************************************************/
 void CSJSGetSearchParam(CSJSSearchParam_S *pstSearchParam);
 
 /******************************************************************************
Description:
	���������������Ƿ���JSCALL_POP_SERVICE��Ϣ
Input:
	bIsSend:ΪTRUE��ʾ���ͣ�ΪFALSE��ʾ�����͡�Ĭ�ϲ�����
Output:
    ��
    
Return Value:
	None
*******************************************************************************/
 void CSJSSetSearchIsSendPopServiceMsg(BOOL bIsSend);

/******************************************************************************
Description:
	������ͨ�����˽���Logic Screen������ʱ������PMT��Ϊ׼������SDT��Ϊ׼
	ע:������ֻ��һ������������ʱ�����������Ϊ׼�������ýӿ���Ч
Input:
	nFlag:
		0��ʾLogic Screen����������PMT��Ϊ׼
		1��ʾLogic Screen����������SDT��Ϊ׼
Output:
    ��
    
Return Value:
	None
*******************************************************************************/
 void CSJSSTMosaicSetParseFlagForLScreen(int nFlag);

/******************************************************************************
Description:
    ���ù��������˵�������

Input:
    nAreaCode:������

Output:
    None

Return Value:
    ��
*******************************************************************************/
void CSJSHmMosaicSetAreaCode(int nAreaCode); 

/******************************************************************************
Description:
    ��ȡ��ά����������Ϣ������ӿ�����Ŀ��ʵ�֣�����ĿҪʹ��������ܵ�ʱ����Ҫע��void CSJSRegisteSkyworthAD(void);����ӿ�

Input:
nTypeIndex���������
nOrgNetworkID����Ŀ��ԭʼ����ID
nTsID:��Ŀ��TSID
nServiceID����Ŀ��ServiceID
Output:
    pstADInfo����ά����������Ϣ

Return Value:
    �ɹ�����TRUE,ʧ�ܷ���FALSE
*******************************************************************************/
BOOL CSJSGetSkyworthADInfoByService(int nTypeIndex, int nOrgNetworkID,int nTsID, int nServiceID ,CSSkyworthADInfo *pstADInfo);

/******************************************************************************
Description:
    NGB������ʼ����Ƶ������Ƶ�豸����
Input:
    nAreaCode:������

Output:
    None

Return Value:
    ��
*******************************************************************************/
BOOL CSJSAddAudioOutputInterface(CSMPAUDOutputDevice_E  eDeviceType,char *pcAudioDeviceName, BOOL bOpenStatus);


#ifdef MMCP_UDI2
/******************************************************************************
Description:
    NGB������ʼ����Ƶ������Ƶ��Ƶ����

Input:
    eDeviceType:��Ƶ����豸����
    pcVideoDeviceName:��Ƶ�豸����
    bOpenStatus:

Output:
    None

Return Value:
    ��
*******************************************************************************/
BOOL CSJSAddVideoOutputInterface(CSUDIVOUTDevices_E  eDeviceType, char *pcVideoDeviceName, BOOL bOpenStatus);
#endif

/******************************************************************************
Description:
    ע���ȡ����ģʽ

Input:
    fnCallback:ע��Ļص�����
    pUserData:�û�����

Output:
    None

Return Value:
    ��
*******************************************************************************/
BOOL CSJSRegisteGetEthernetMode(CSEthernetModeCallback_F fnCallback, void *pUserData);

void CSJSAPPSyncDMtoFlash(void);
int CSJSStopSearch(void);
BOOL CSJSAddSearchCallback(CSJSSearchCallback_F fnSearchCallback, void* pUserData);
BOOL CSJSRemoveSearchCallback(CSJSSearchCallback_F fnSearchCallback);
int CSJSStartCableSearch(int nSearchType, Deliver *pDeliver, int nDeliverCount);

#ifdef __cplusplus
}
#endif

#endif

