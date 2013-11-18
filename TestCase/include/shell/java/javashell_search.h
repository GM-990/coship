#ifndef _SEARCH_INIT_PARAM_H_
#define _SEARCH_INIT_PARAM_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "CSUSPDMCommonType.h"
#include "mmcp_tuner.h"

//�����洢ѡ��
enum
{
	SEARCH_SAVE_OKEND,  //�����ɹ������ű���
	SEARCH_SAVE_ANY     //�κ�����¶�����
};

//����4��������ʽ
enum
{
	SEARCH_SINGLE,      //��Ƶ��
	SEARCH_NIT,         //NIT��������
	SEARCH_LIST,        //�б�����:
	SEARCH_FAST        //SDT�������� 
};

//����ĳ���������������˽������������
#define  MAX_SEARCH_PRVTAG              (8)
void javashell_searchGetDefaultDeliver(CSUDITunerCableDeliver_S *sDeliver);

//������˽�����ݽ����ص�:
//����pDatahServiceNode: USP-Search buffer�еġ���ǰ������Service��
//����pPrivateData: ˽�����ݣ���ʽ��USP-Search����
//����nDataLen:     ˽�����ݳ���
//����ֵ: TRUE��ʾ���˵���Service, FALSE��ʾ������Service
typedef BOOL (* javashell_searchPrivateParse) (HCSHANDLE hServiceNode, PBYTE pPrivateData, int nDataLen);

//����������Service�б����û��Լ���ʽ��ӵ���������
//����phServiceHandle: Service�б���ַ
//����nHandleCount:    Service����
//����ֵ: TRUE��ʾ��ʵ�����(�����������غ��������������б�)
//        FALSE��ʾû��ʵ�������Ч����
//ע��: �����������ͷż�����������ʧ�ܵ�ServiceHandle
typedef BOOL (* javashell_searchAddContainer) (HCSHANDLE *phServiceHandle, int nHandleCount);


//����������NIT���׸��û������Ƿ������NITΪ��ڿ�ʼ����
//����pNITNode: SINetworkInfo ��ָ�룬 �û�����ͨ����Щ��Ϣ���ж�NIT���Ƿ����Ҫ��
//����ֵ: 1 ��ʾ�����Ǹ�NIT�������������NIT
//        0 ��ʾ�����NITΪ��ڿ�ʼ����
//ע��: ��������Ҫ�������Ҫ�Զ�������NIT�����
//typedef int (*FOnCheckNit) (SINetworkNode *pNITNode);


//���������˽������
typedef struct _PrivateRequest
{
	//NIT
	int			  m_nNITTagCount;
	BYTE		  m_bNITTags[MAX_SEARCH_PRVTAG];
	javashell_searchPrivateParse  m_pNITParseCallback;

	//SDT
	int			  m_nSDTTagCount;
	BYTE		  m_bSDTTags[MAX_SEARCH_PRVTAG];
	javashell_searchPrivateParse  m_pSDTParseCallback;

	//BAT
	WORD		  m_wMinBouquetID;
	WORD          m_wMaxBouquetID;
	BOOL          m_bBouquetFilter;
	BOOL          m_bBouquetSearch;
	int			  m_nBATTagCount;
	BYTE		  m_bBATTags[MAX_SEARCH_PRVTAG];	
	javashell_searchPrivateParse  m_pBATParseCallback;

	//PMT
	javashell_searchPrivateParse  m_pPMTParseCallback;

	//CAT
	javashell_searchPrivateParse  m_pCATParseCallback;

}JAVASHELLPRIVATEREQUEST;


//NIT����ʱ�Զ����һЩ����
#if 0
typedef struct _SearchNitCustom
{
    BOOL        m_bCustomNitSearch;         //����ļ���ѡ���Ƿ���Ч
    FOnCheckNit m_pCheckNit;                //����NITʱ���ȷ��NIT�Ƿ���Ч�ĺ���

    BOOL        m_bSearchPatTsIdMatch;      //�Ƿ����Ƶ������ʱ��Ҫ���PAT��TsID�Ƿ���NIT��һ�£� ��һ����������Ƶ�� 
    BOOL        m_bSearchSdtTsIdMatch;      //�Ƿ����Ƶ������ʱ��Ҫ���SDT��TsID�Ƿ���NIT��һ�£� ��һ����������Ƶ��
    BOOL        m_bSearchSdtOriNetWorkMatch;//�Ƿ����Ƶ��������ʱ�� Ҫ���SDT��OriNetWorkID�Ƿ���NIT��һ�£� ��һ����������Ƶ��

}SEARCHNITCUSTOM;
#endif


//����ģ���ʼ����Ҫ�趨�Ĳ�����
typedef struct _SearchInitParam
{
	//��������˽������
	JAVASHELLPRIVATEREQUEST   m_stPrivateRequest;

	//���û��Զ��巽ʽ���Service�����ݹ���
	javashell_searchAddContainer     m_pAddContainer;

	//����Ƶ�㳬ʱʱ��:xxx_mili_sec
	int              m_nTimeOut;

	//�����洢����ѡ��
	int              m_nSaveMode;

	//�Ƿ���Ҫ��ʾService,������WM_POP_SERVICE(��SVSCALL_POP_SERVICE)
	BOOL             m_bPopService;

  //  SEARCHNITCUSTOM  m_stSearchNitCustom;

}JAVASHELLSEARCHINITPARAM,*LPJAVASHELLSEARCHINITPARAM;

/*========================================================
//����ԭ��:SearchInit()
//�������:��
//�������:��
//����ֵ:  ��
//����:    ����������ʼ������,��Ӧ�÷��ڴ�ͷ�ļ���-----tfl
=========================================================*/
//void SearchInit(void);

/*==========================================================
//����ԭ��:SVSInitSearch()
//�������:pInitParam �û���ʼ������
//�������:��
//����ֵ: void
//���� :��������Ӫ��������й���ѡ�񣬳�ʼ��Ƶ����������
//ע������:������ʼ������һ�� ��USPInit��ͨ�� SearchInit ����
=========================================================== */
BOOL javashell_InitSearchParam(LPJAVASHELLSEARCHINITPARAM pInitParam);

/*========================================================
//������Ŀ����ص�
//�������:��
//�������:��
//����ֵ:  ��
//����:    �Խ�Ŀ��������
=========================================================*/
typedef void (*javashell_SearchSortServiceCallback)(void);

/*========================================================
//���������Ŀ�ص�
//�������:type      	��Ŀ����
		   pParam 		���ز���
//�������:��
//����ֵ:  1  	 �����浱ǰ��Ŀ
		   0     ���浱ǰ��Ŀ
//����:    ���ڱ���/���˽�Ŀ
=========================================================*/
typedef int (*javashell_SearchSaveServiceCallback)(CSUSPDMServiceType type, void* pParam);

/*========================================================
//�������Ź��˻ص�
//�������:bIsFiltrateSearchData    TRUE ���� FALSE ������
//�������:��
//����ֵ:  ��
//����:    �����������˱�־
=========================================================*/
typedef void (*javashell_SetSearchFiltrationTagCallBack)(BOOL bIsFiltrateSearchData);

/*========================================================
//���������ֶ�������־�ص�
//�������:bIsManulSearch    TRUE �ֶ����� FALSE �����ֶ�����
//�������:��
//����ֵ:  ��
//����:    ���������ֶ�������־
=========================================================*/
typedef void (*javashell_SetManulSearchTagCallBack)(BOOL bIsManulSearch);

/*========================================================
//��¼�������Ľ�Ŀ��Ҫ�ػص�
//�������:serviceHandle  ��Ŀ���
//�������:isClearRecordInfo  TRUE:�����ǰ��¼������ ���Ҳ���serviceHandle��Ч   FALSE:��¼����
//�������:��
//����ֵ:  ��
//����:    ��¼�������Ľ�Ŀ�е�һ�����Ӻ͹㲥��Ŀ����Ҫ��
=========================================================*/
typedef void (*javashell_RecordSearchInfoCallBack)(HCSHANDLE serviceHandle, BOOL isClearRecordInfo);

/*========================================================
//��ȡ���������м�¼�ĵ��ӻ�㲥����Ҫ��
//�������: type  0:TV 1:RADIO
//�������: elementIndex  0:��ȡonid 1:��ȡtsid 2:��ȡserviceid
//�������:��
//����ֵ:  �����Ҫ��ֵ(onid or tsid or serviceid)
//����:    ��ȡ���������м�¼�ĵ��ӻ�㲥����Ҫ��
=========================================================*/
typedef int (*javashell_GetSearchRecordInfoCallBack)(int type, int elementIndex);

/*========================================================
//����ԭ��:RegisterSearchSaveCallBack()
//�������:searchSaveCallback �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע�������ص�������ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterSearchSaveCallBack(javashell_SearchSaveServiceCallback searchSaveCallback);

/*========================================================
//����ԭ��:RegisterSearchSortServiceCallBack()
//�������:searchSortServiceCallBack �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע�������ص�������ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterSearchSortServiceCallBack(javashell_SearchSortServiceCallback searchSortServiceCallBack);

/*========================================================
//����ԭ��:RegisterSetManulSearchTagCallBack()
//�������:setMaunulTag �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע�������ֶ�������־��ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterSetManulSearchTagCallBack(javashell_SetManulSearchTagCallBack setMaunulTag);

/*========================================================
//����ԭ��:RegisterSetSearchFiltrationTagCallBack()
//�������:setFiltrationTag �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע�����ú��Ź��˱�־��ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterSetSearchFiltrationTagCallBack(javashell_SetSearchFiltrationTagCallBack setFiltrationTag);

/*========================================================
//����ԭ��:RegisterRecordSearchInfoCallBack()
//�������:recordSearchInfo �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע���¼�������Ľ�Ŀ��Ҫ�ص�ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterRecordSearchInfoCallBack(javashell_RecordSearchInfoCallBack recordSearchInfo);

/*========================================================
//����ԭ��:RegisterGetSearchRecordInfoCallBack()
//�������:getSearchRecordInfo �ص�������ʵ��
//�������:��
//����ֵ:  ��
//����:    ע���ȡ�������Ľ�Ŀ��Ҫ�ص�ʵ��(���ڿ�������)
=========================================================*/
void javashell_RegisterGetSearchRecordInfoCallBack(javashell_GetSearchRecordInfoCallBack getSearchRecordInfo);
//��ʱû���õ�
HCSHANDLE  javashell_GetSearchUIHandle( void);

#ifdef __cplusplus
}
#endif

#endif

