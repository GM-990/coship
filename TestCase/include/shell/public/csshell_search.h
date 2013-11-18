/**@defgroup PublicShellSearch PublicShellSearch ����������صĶ���ӿڼ�����
@brief ��ģ������Ӧ�ô���˽���������̣���������뾡����SVS����һ��

@note Ӧ��������ػص��е���DTV(USP)-Search����ؽӿڽ���������������SVS����

@version 2.0.0 2009/12/11 ����
@{
*/
#ifndef CSSHELL_SEARCH_H
#define CSSHELL_SEARCH_H

#include "mmcp_typedef.h"
#include "SearchUI.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CSSHELL_SEARCH_MAX_TAG_COUNT    (8) ///< ÿ�������������ĳ��������������������
#define CSSHELL_SEARCH_MAX_BOUQUETID_COUNT    (32)  ///ÿ�����������õ�����BAT���˵�BOUQUETID��������

/**@brief �����洢ѡ��������ʽ������ģʽ����ȷ�������洢���ԡ����ԣ�����ʱ��ʽ��ģʽ����Ӧ�þ��������洢��ʽ����ǰ���߾���*/
typedef enum
{
    EM_CSSHELL_SEARCH_SAVE_OKEND,  ///< �����ɹ������ű���
    EM_CSSHELL_SEARCH_SAVE_ANY     ///< �κ�����¶�����
}CSShellSearchSaveMode_E;

/**
@brief ������������ģʽ
@note ��ЩģʽĿǰ����nit��bat��Ч����sdt actual�������ã���Ϊÿ��Ƶ���sdt actual����ͬ�����붼����	
*/
typedef enum
{
	EM_CSSHELL_SEARCH_TAG_DEFAULT,		///<Ĭ�ϡ�����nit��˵����ÿ��Ƶ�㶼����������bat��˵����������һ�ξͲ�������
	EM_CSSHELL_SEARCH_TAG_ONCE,			///<������һ�ξͲ�������
	EM_CSSHELL_SEARCH_TAG_EVERY_DELIVER,	///<ÿ��Ƶ�㶼����
	EM_CSSHELL_SEARCH_TAG_EXTERNAL_DELIVER	///<ֻ��������Ƶ�����������ֻ��nit ������������Ч
}CSShellSearchTagMode_E;

/**
@brief ������ʽ,��java ��SearchManager�е��������Ͷ�Ӧ.��������������ʱ��������ʽ��ģʽ�Ƿǳ���Ҫ�ġ�
������ʽ���Ǵ������ܹ��̶Ա�����������˵�ġ�����ģʽ�����Ǵ������ܹ�Ӧ�öԻص��Ľ�Ŀ���ݵĴ��������˵��
@see CSShellSearchMode_E
*/
typedef enum
{
	EM_CSSHELL_SEARCH_TYPE_PATPMT = 0,   ///< ��Ƶ�㣬�б�ȫƵ������
	EM_CSSHELL_SEARCH_TYPE_NITSDT =1,  
	EM_CSSHELL_SEARCH_TYPE_NITPATPMT = 2,   ///< ��������
	EM_CSSHELL_SEARCH_TYPE_SATELLITE = 3,   ///������������NIT
	EM_CSSHELL_SEARCH_TYPE_SATELLITE_NIT=4,   //����������NIT
	EM_CSSHELL_SEARCH_TYPE_TRANSPONDER = 5,   ///Ƶ����������NIT
	EM_CSSHELL_SEARCH_TYPE_TRANSPONDER_NIT= 6,  //Ƶ��������NIT
	EM_CSSHELL_SEARCH_TYPE_NITSDT_EX = 7,
	EM_CSSHELL_SEARCH_TYPE_SDT = 8
 
///< ������������NIT_Other,SDT_other
}CSShellSearchType_E;

/**@brief ����ģʽ*/
typedef enum
{
    EM_CSSHELL_SEARCH_MODE_NORMAL,      ///< ������������һ������µĴӽ�����������ģʽ��Ĭ��Ϊ��ģʽ
    EM_CSSHELL_SEARCH_MODE_NO_FILTER,     ///< �޹���������ʽ��һ�����ں���������ͨ��Java�ӿ�setFiltratorFlag��������������ģʽ
    EM_CSSHELL_SEARCH_MODE_SAU			///<sau����ģʽ.����sauģʽ��һ���Ƚ���ͳ�ĸ��sauģʽ�Ƿ���˽�Ŀ���ɾ�����Ŀ���������
}CSShellSearchMode_E;

/**
@brief ������˽�����ݽ����ص�
@param[in] hServiceNode DTV(USP)-Search buffer�еġ���ǰ������Service��
@param[in] pPrivateData ˽�����ݣ���ʽ��USP-Search���塣��Ӧ��SearchPrivateInfo�ṹ�е�m_pucPrivateInfo
@param[in] nDataLen ˽�����ݳ���
@param[in] pUserData �û����ݣ���ֵ���ڶ�ӦShellCommonSearch_S�ṹ�е�m_pUserData�ֶΣ����ֶ���������������
@return TRUE��ʾ���˵���Service, FALSE��ʾ������Service
@see SearchPrivateInfo
*/
typedef BOOL (* OnCSShellSearchPrivateParse_F) (HCSHANDLE hServiceNode, PBYTE pbPrivateData, int nDataLen,void* pvUserData);

/**
@brief ����������Service�б����û��Լ���ʽ��ӵ���������
@param[in] phServiceHandle Service�б���ַ
@param[in] nHandleCount Service����
@param[in] pUserData �û����ݣ���ֵ���ڶ�ӦShellCommonSearch_S�ṹ�е�m_pUserData�ֶΣ����ֶ���������������
@return TRUE��ʾ��ʵ�����(�����������غ��������������б�);FALSE��ʾû��ʵ�������Ч����
@note �����������ͷż�����������ʧ�ܵ�ServiceHandle
*/
typedef BOOL (* OnCSShellSearchAddContainer_F) (HCSHANDLE *phServiceHandle, int nHandleCount,void* pvUserData);


/**
@brief������ʼ֮ǰ�Ļص�����ʱsearchui����Ѿ�����������Ӧ���Ͽ��ܻ���Ҫ�����Լ���һЩ����
@param[in] ��
@return TRUE��ʱ�û������Լ������������ɹ���FALSE��ʾʧ��
@note ֮�����ṩ�ýӿڣ�����ΪSearch����̫�࣬Ҫ�����������CSShellCommonSearch_S���޷���ȫ����������û����Լ��������������Ե��������Ľӿ��Լ�����
*/
typedef BOOL (* OnCSShellSearchStart_F) (void);

/**
@brief�û���������Ļص�����Ӧ����������java������ӿ�ʱ�����ã��������������û�ѡ���Ƿ��������������
@param[in] ��
*/
typedef void (* OnCSShellSearchSort_F) (void);

/**
@briefӦ�û�ȡ�Ƿ��������½�Ŀ�ص�
@param[in] ��
*/
typedef BOOL (* CSShellSearchHaveNewService_F) (void);


/**
���ṹ����������ʱ�õ�����Ϣ�����еĻص�����dtv search�ļ�ӻص�����ֱ�ӻص���dtvm native��ʵ�֡�dtvm nativeͨ���ӿ�CSShellSearchGetFactory
�����ɻ�ȡCSShellCommonSearch_S,�ɵõ���ӻص���Ȼ����native�н��޷��������Ϣת������ӻص�����ӻص���shellʵ�֡�
*/
typedef struct _CSShellCommonSearch
{
    int                    m_nSize;                    ///< ���ṹ��Ĵ�С�����븳ֵΪsizeof(CommonSearch_S)
	void*                  m_pvUserData;                            ///< �û����ݣ��˲��������ű��ṹ�еĺ���ָ��һ�𴫵ݸ�����ʵ��
	
    //NIT
    int			           m_nNITTagCount;                         ///< ���ע��NIT˽������������
    BYTE		           m_abNITTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< ���ע��NIT˽��������Tags
    OnCSShellSearchPrivateParse_F       m_fnNITParseCallback;                    ///< NIT����������ص�

    //SDT
    int			           m_nSDTTagCount;                         ///< ���ע��SDT˽������������
    BYTE		           m_abSDTTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< ���ע��SDT˽��������Tags
    OnCSShellSearchPrivateParse_F        m_fnSDTParseCallback;                    ///< SDT����������ص�

    //BAT
    WORD		           m_wMinBouquetID;                        ///< BAT������СBouquetID,�������ã��������ע�����������Ϊ0
    WORD                   m_wMaxBouquetID;                        ///< BAT�������BouquetID,�������ã��������ע�����������Ϊ0xffff
    BOOL                   m_bBouquetFilter;                       ///< �Ƿ�Ҫ����BAT����
    int			           m_nBATTagCount;                         ///< ���ע��BAT˽������������
    BYTE		           m_abBATTags[CSSHELL_SEARCH_MAX_TAG_COUNT]; ///< ���ע��BAT˽��������Tags
    OnCSShellSearchPrivateParse_F         m_fnBATParseCallback;                    ///< BAT����������ص�

    //PMT
    OnCSShellSearchPrivateParse_F         m_fnPMTParseCallback;                    ///< PMT����ص�����

    //CAT
    OnCSShellSearchPrivateParse_F         m_fnCATParseCallback;                    ///< CAT����ص�����

    OnCSShellSearchAddContainer_F         m_fnAddContainer;                        ///< ���û��Զ��巽ʽ���Service�����ݹ���ĺ���
    
    int                    				m_nTimeOut;                             ///< @deprecated, Ŀǰδʹ�ã�����Ƶ��������ʱ��java��jsӦ�����õ�Ϊ׼ 
    CSShellSearchSaveMode_E  			m_eSaveMode;                            ///< �����洢����

	OnCSShellSearchStart_F				m_fnBeforStart;		///<������ʼ֮ǰ���ûص�����ִ��

	CSShellSearchTagMode_E				m_eNitTagSearchMode;	///<����nit��������ʱ����ÿ��Ƶ�㶼����������������֮��Ͳ�������,Ĭ��Ϊÿ��Ƶ�㶼����
	CSShellSearchTagMode_E				m_eBatTagSearchMode;	///<����bat��������ʱ����ÿ��Ƶ�㶼����������������֮��Ͳ���������Ĭ��Ϊ������֮��Ͳ�������
	SearchCallBack						m_fnDtvSearchCallback;	///<��ԭʼ��dtv search�ص��������øûص�������ֻ�ж�dtv search�ǳ���Ϥ������²ſ�ʹ�á�
																///<���ע�ᣬ�ûص����������лص�����֮ǰ�ȷ����������ȷ������ֵ������ֵ��0������£�searchmanager��������Դ˻ص����κδ���ֱ�ӷ��ء�
	int m_nBouquetIDCnt;                  ///��Ҫ�����BouquetID�ĸ����������עm_wMinBouquetID��m_wMaxBouquetID�������������������˸�ֵ������Ҫ��m_nBouquetIDCnt����Ϊ0��
									///������������BouquetID����ķ�ʽ����BAT����
	WORD m_awBouquetID[CSSHELL_SEARCH_MAX_BOUQUETID_COUNT];   ///��Ҫ�����BouquetID��ֵ�������m_nBouquetIDCnt��Ӧʹ�ã����m_nBouquetIDCnt��ֵΪ0��������鲻�����ã�����������m_nBouquetIDCnt�������˸�ֵ
	                                                                                                             ///�򲻹�עm_wMinBouquetID��m_wMaxBouquetID������ʱ��ע�⣬���ϸ�Ҫ������!!!!!!!!
	OnCSShellSearchSort_F					m_fnSortCallback;		///<Ӧ����������ص�����			
	CSShellSearchHaveNewService_F		m_fnHaveNewService;		///<Ӧ�û�ȡ�Ƿ����½�Ŀ�ص�	
	BOOL 								m_bInitMainCustom; ///<����ΪTRUE�����Զ��������Լ���SEARCH_NIT_MAIN_CUSTOM���ͣ��û��Զ���
	BOOL                                m_bNetworkidFilter;      ///<�����Ƿ�ָ��networkid����������,TRUE��ʾָ����FALSE��ʾ��ָ��
	DWORD								m_dwNetworkId;      ///<����ָ����networkid                                 
}CSShellCommonSearch_S;

/**
@brief ��������ʵ���ӿڶ���
@param[in] eSearchType ������ʽ
@param[in] eMode ����ģʽ
@return ���������ڸ÷�ʽ����ģʽ���������󣻷���NULL��Ĭ�����̴���
*/
typedef const CSShellCommonSearch_S* (* CSShellSearchCreate_F )(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode);

/**
@brief ����search����
@param[in] pFactory search������Shell��ͨ���ýӿڴ���������ز���������
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellSearchSetFactory(CSShellSearchCreate_F pfnFactory);

/**
@brief ��ȡ��ǰ�����ľ��
@param ��
@return ��������������У��򷵻�������������򷵻�NULL
@note �ú���һ����search�Ļص�����,����ֱ�ӻص��ͼ�ӻص�
*/
HCSHANDLE CSShellSearchGetUI();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   
