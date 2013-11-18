#ifndef EPGReceiverInterface_H
#define EPGReceiverInterface_H

#include "mmcp_typedef.h"
#include "mmcp_debug.h"

#ifdef  __cplusplus
extern "C" {
#endif

//��������
#define	EPG_SERVICE_DETECTIVE			0x08      //�����Զ����ӵ�ǰSercice
#define	EPG_SEARCH_ORINETWORK			0x04      //Network������
#define	EPG_SEARCH_TS					0x02      //TS������
#define	EPG_SEARCH_SERVICE				0x01      //Service������

//�洢����
#define	EPG_SAVE_ALL					0x80      //�洢��������
#define	EPG_SAVE_ORINETWORK				0x40      //�洢��ǰNetwork����
#define	EPG_SAVE_TS						0x20      //�洢��ǰTS����
#define	EPG_SAVE_SERVICE				0x10      //�洢��ǰService����

//ע��ص����������ͣ�REG��
//��ǰ����PresentEPG�ѵ�
#define EPG_REG_CURSERVICE_SIMPLE_PRESENT_FINISHED	(0x00000001)
//��ǰ����FollowingEPG�ѵ�
#define	EPG_REG_CURSERVICE_SIMPLE_FOLLOW_FINISHED	(0x00000002)
//��ǰ����PresentEPG�ı�
#define	EPG_REG_CURSERVICE_SIMPLE_PRESENT_CHANGED	(0x00000004)
//��ǰ����FollowingEPG�ı�
#define	EPG_REG_CURSERVICE_SIMPLE_FOLLOW_CHANGED	(0x00000008)
//�����P/F EPG��ȫ
#define	EPG_REG_ALLSERVICE_SIMPLE_FINISHED			(0x00000010)
//PG��Ϣ	
#define	EPG_REG_CURSERVICE_SIMPLE_PRESENT_PGCHANGED	(0x00000020)
//һ��Service��ScheduleEPG��ȫ
#define	EPG_REG_CURSERVICE_SCHEDLE_FINISHED			(0x00000040)
//�����Schedule EPG��ȫ
#define	EPG_REG_ALLSERVICE_SCHEDLE_FINISHED			(0x00000080)
#define	EPG_REG_CUREVENT_ALLDATA					(0x00000100)
#define	EPG_REG_CUREVENT_FREEPRIVATEDATA			(0x00000200)
//�ڴ治��
#define	EPG_REG_MEMORY_FULL							(0x00000400)
//�ѵ�һ��Schedule EPG��Section
#define	EPG_REG_EIT_REC 							(0x00000800)

#define EPG_REG_SCHEDULE_CONTENTTYPE                (0x00001000)
#define EPG_REG_SCHEDULE_EIT_VERSION_CHANGED 	(0x00002000)

//wangshazhou 2006.4.9
//ɾ������EPG
#define EPG_REG_DELETE_EPGHANDLE                    (0x00002000)
//P/F EPG��ʱ��ֹͣ�����Ļص�
#define EPG_REG_SIMPLE_TIMEOUT                      (0x00004000)
//Schedule EPG��ʱ��ֹͣ�����Ļص�
#define EPG_REG_SCHEDULE_TIMEOUT                    (0x00008000)

//Simple EPG����ص�
#define EPG_REG_SIMPLE_CONTENTTYPE                  (0x00010000)

//����Event����������
//����������Ŀ���
#define	EPG_EVENT_DESDATA							(0x00000001)
//���������û�˽������
#define	EPG_EVENT_PRIVATEDATA						(0x00000002)

//����ֻ�洢��ǰService�Ľ�Ŀ���
#define	EPG_NEEDSERVICE_DEC							(0x00000004)
//����ֻ�洢��ǰTS�Ľ�Ŀ���
#define	EPG_NEEDTS_DEC							(0x00000008)

//���ñ���������
#define EPG_NEEDSAVE_PRIVATEDATA						(0x00000010)

#define EPG_DEC_SHORT        0x01  //��short �������еü��
#define EPG_DEC_EXTEND       0x02  //��extend �������еü��

#define EPG_TIMEOUT_BY_REPEAT   0x10   //�ж��ظ����ĳ�ʱ
#define EPG_TIMEOUT_DIRECT      0x20   //ֱ�ӳ�ʱ

//�ص����ͣ�������ġ�ע��ص����������͡����Ӧ��
typedef enum _EPGCallbackType
{
	EPG_CURSERVICE_SIMPLE_PRESENT_FINISHED,
	EPG_CURSERVICE_SIMPLE_FOLLOW_FINISHED,
	EPG_CURSERVICE_SIMPLE_PRESENT_CHANGED,   //EPGUserParam
	EPG_CURSERVICE_SIMPLE_FOLLOW_CHANGED,	 //EPGUserParam
	EPG_ALLSERVICE_SIMPLE_FINISHED,  //NULL
	
	EPG_CURSERVICE_SIMPLE_PRESENT_PGCHANGED,     //EPGPGParam

	EPG_CURSERVICE_SCHEDLE_FINISHED,//EPGUserParam
	EPG_ALLSERVICE_SCHEDLE_FINISHED,//NULL

	EPG_ANALYSE_PRIVATEDATA,//EPGEventData
	EPG_FREE_PRIVATEDATA,

	EPG_MEMORY_FULL,

	EPG_EIT_REC,

	EPG_SCHEDULE_CONTENTTYPE,

	EPG_DELETE_EPGHANDLE,
	EPG_SIMPLE_TIMEOUT,
	EPG_SCHEDULE_TIMEOUT,

	EPG_GET_PRIVATEDATA,

	EPG_SIMPLE_CONTENTTYPE,
	EPG_SCHEDULE_EIT_VERSION_CHANGED
}EPGCallbackType;

typedef struct _ServiceSourceID
{
	WORD m_wOriginalNetworkID;
	WORD m_wTsID;
	WORD m_wServiceID;
}ServiceSourceID;

typedef struct _PGParam
{
	ServiceSourceID m_ServiceSourceID;

	int m_nPGRate;
}EPGPGParam;

typedef struct _EPGUserParam
{
	ServiceSourceID m_ServiceSourceID;

	void* m_pUserData;
}EPGUserParam;

typedef struct _EPGEventData
{
	PBYTE m_pEventData;
	int	  m_nEnentDataLen;

	HCSHANDLE m_hUserData;
}EPGEventData;

typedef struct _EPGContentTypeData
{
	WORD m_wOriginalNetworkID;
	WORD m_wTsID;
	WORD m_wServiceID;
	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;
	DWORD m_dwUserData;
}EPGContentTypeData;

//�û������Ļص�������ָ��
//enmType ���� SERVICE_SUCCESSʱ ����ʾһ����Ŀ������ɣ�pParamΪServiceParam���͵Ľṹ�壬����NetworkID��TSID��ServiceID��
//        ���� PG_INFOʱ ����ʾ��ǰEvent����PG��Ϣ��pParamΪPG��Ϣ��������ڵ�NetworkID,TSID,ServiceIDֵ��
//                         ��һ��Ƶ�����꣬���ô˺�����
typedef void*(*EPGCallBack)(EPGCallbackType enmType, void* pParam);

/* {{{USR
CreateEPG
  ����EPG��Դ���
HCSHANDLE CreateEPG( HCSHANDLE hSection, HCSHANDLE hMemoryPool, EPGCallBack pCallBack, DWORD dwRegCallbackType, DWORD dwEventSimpleDataType, DWORD dwEventScheduleDataType );

Parameters��
  hSection
     [in]����sectionģ��ľ��
  hMemoryPool
     [in]MemoryPoolģ����ڴ�ؾ��
  dwRegCallbackType 
     [in]�û�ע��Ļص�����
  dwRegCallbackType 
     [in]�˲�������ע���û���Ҫ�Ļص��������ͣ��˲����ɽӿڶ���ĺ�ָ����
  dwEventSimpleDataType 
     [in]��ʾ�Ƿ��ȡPresent ��Following EPG�Ľ�Ŀ������Ϣ���˲����ɽӿڶ���ĺ�ָ����
  dwEventScheduleDataType 
     [in] ��ʾ�Ƿ��ȡSchedule EPG�Ľ�Ŀ������Ϣ���˲����ɽӿڶ���ĺ�ָ����

Return Values
  ����EPG��Դ������������NULL����ʾ����ʧ��
   }}}USR */
HCSHANDLE CreateEPG( HCSHANDLE hSection, HCSHANDLE hMemoryPool, EPGCallBack pCallBack, DWORD dwRegCallbackType, DWORD dwEventSimpleDataType, DWORD dwEventScheduleDataType );

/* {{{USR
FreeEPG
  �ͷ�EPG��Դ���
  BOOL FreeEPG( HCSHANDLE hEPG );

Parameters��
  hEPG
   [in]EPG��Դ��� 

Return Values
  BOOL TRUE��ʾ�ɹ�
       FALSE��ʾʧ��
   }}}USR */
BOOL FreeEPG( HCSHANDLE hEPG );

/* {{{USR
StartRecEPG
  ����ָ���������Ժʹ洢���Ե�Present/Following EPG �� Schedule EPG ��Ϣ����,
  ���ղ��Ժʹ洢�����ɺ���SetEPGRecPolicy �� SetEPGSavePolicy�ֱ�ָ����
StartRecEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );
    Parameters��
      hEPG
        [in]EPG��Դ��� 
      wOriginalNetworkID
        [in]ָ��Network��IDֵ
      wTsID
        [in]ָ��TS��IDֵ  
      wServiceID
        [in]ָ��Service��IDֵ

    Return Values
        BOOL TRUE��ʾ�ɹ�
        FALSE��ʾʧ��

   }}}USR */
BOOL StartRecEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
BOOL StartRecEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );
    ����ָ���������Ժʹ洢���Ե�Present/Following EPG �� Schedule EPG ��Ϣ���գ�
    ���ղ��Ժʹ洢�����ɺ���SetEPGRecPolicy �� SetEPGSavePolicy�ֱ�ָ�����˽ӿ���StartRecEPG���������ڶ���һ��dwUserOriginalNetworkID�����������������һ����ʾ�������ڴ����ڶ����������£�wOriginalNetworkID��׼ȷ�������
  
  Parameter:  
    hEPG
      [in]EPG��Դ���
    wOriginalNetworkID
      [in]ָ��Network��IDֵ
    wTsID
      [in]ָ��TS��IDֵ
    wServiceID
      [in]ָ��Service��IDֵ
    dwUserOriginalNetworkID
      [in]�û�ָ��������ı�ʾ��

  RETURN VALUE  
      BOOL TRUE��ʾ�������ճɹ�
      FALSE��ʾ��������ʧ�ܡ�

   }}}USR */
BOOL StartRecEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR

   }}}USR */
BOOL StartRecEPGWithUserData( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, void * pUserData );

/* {{{USR
����EPG����������
   ������
     nSimpleEPG������P/F EPG����������
	 nSckeduleEPG������Schedule EPG����������
   }}}USR */
BOOL SetEPGRecPolicy( HCSHANDLE hEPG, int nSimpleEPG, int nSckeduleEPG );

/* {{{USR
����EPG�Ĵ洢����
   ������
     nSimpleEPG������P/F EPG�Ĵ洢����
	 nSckeduleEPG������Schedule EPG�Ĵ洢����
   }}}USR */
BOOL SetEPGSavePolicy( HCSHANDLE hEPG, int nSimpleEPG, int nScheduleEPG );

/* {{{USR
StopRecEPG
 ֹͣ����EPG��Ϣ�Ľ���
 BOOL StopRecEPG( HCSHANDLE hEPG );

Parameters��
  hEPG
   [in]EPG��Դ���

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL StopRecEPG( HCSHANDLE hEPG );

/* {{{USR
�õ�EPGģ��İ汾����Ϣ��
   }}}USR */
int CSUSPEPGGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
�õ�EPGʹ���ڴ����������ֵ����Сֵ��
   }}}USR */
BOOL CSUSPEPGGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
����Ҫ����EPG��Ŀ���ʱ�����ý�Ŀ��������
  ������
   nEPGDecDataType����Ŀ�������ͣ�ʹ�ú�EPG_DEC_SHORT��EPG_DEC_EXTEND��
                    ����������Ͷ�Ҫ����ô������������
   }}}USR */
BOOL SetEPGDecDataType( HCSHANDLE hEPG, int nEPGDecDataType );

/* {{{USR
����EPG�����õ�DumaxIDֵ
   }}}USR */
BOOL SetEPGDemaxID( HCSHANDLE hEPG, int nDemaxID );

/* {{{USR
����������EPG������ʱ��������Ĳ�����ǰһ�δ���Ĳ�����ͬʱ���������������𣩣�
��ô��β����ٴ�����EPG����������ʱģ�������Ĳ������˶Աȼ�顣
������ʱ�û�����Ҫ����ܣ��û�ϣ��ÿ������EPG����ʱ��������EPG������
��ʱ�Ϳ��Ե��ô˽ӿ�ʵ�ִ˹��ܡ�
  ������
   bSimpleCheck����ʾ�Ƿ�����SimpleEPG�Ĳ������
   bScheduleCheck����ʾ�Ƿ�����Schedule EPG�Ĳ������
   }}}USR */
BOOL SetEPGCheckParam( HCSHANDLE hEPG, BOOL bSimpleCheck, BOOL bScheduleCheck );

/* {{{USR
  ����Service�����EPG�����������û�����ʲô��������������ô˽ӿ�ִֻ��Service
�����������ӿڵ�bSimpleOnlyServ����ָ��Simple EPG�Ƿ�ǿ��ִ��Service��������
����bScheduleOnlyServָ��Schedule EPG�Ƿ�ǿ��ִ��Service��������
   }}}USR */
BOOL StartRecEPGbyService( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, void * pUserData, BOOL bSimpleOnlyServ, BOOL bScheduleOnlyServ );

/* {{{USR
����EPG�ĳ�ʱʱ����Ƿ��������ж�������ɡ��Ĺ��ܡ���������ó�ʱ������Ϊ-1����
��ô��ʾ���ó�ʱ��
 ������
  nSimpleTime�� Simple EPG�ĳ�ʱʱ�䣨���룩
  bIsSimpleAll������True��ʾ��Simple EPG�������ж�������ɡ��Ĺ��ܣ�
                  ����False��ʾ��������
  nScheduleTime��Schedule EPG�ĳ�ʱʱ�䣨���룩
  bIsScheduleAll������True��ʾ��Schedule EPG�������ж�������ɡ��Ĺ��ܣ�
                 ����False��ʾ��������
   }}}USR */
BOOL SetEPGTimeOut( HCSHANDLE hEPG, int nSimpleTime, BOOL bIsSimpleAll, int nScheduleTime, BOOL bIsScheduleAll );

/* {{{USR
�����Ƿ�ɾ������ȷ��EPG���ݣ�����һ��Service��EPGʱ����12��00����13��00������������һ��
EPG��ʱ����11��00����14��00����12��10����12��40����ô����Ϊ�ѵ��������Ǵ�ģ������뵽EPG���ݿ��С�
 ������
  nIsDelBadData�� ΪTRUE,��ʾɾ������ȷ��EPG����
                  ΪFALSE,��ʾ��ɾ������ȷ��EPG����

return BOOL
   }}}USR */
BOOL CSUSPEPGSetDelBadData( HCSHANDLE hEPG, int nIsDelBadData );

/* {{{USR
����DemuxID
   }}}USR */
BOOL SetEPGDemuxID( HCSHANDLE hEPG, int nDemuxID );

/* {{{USR
���ó�ʱ���ͣ�����ѡ��ֱ�ӳ�ʱ�����ж��ظ�����ʱ(�����ظ���ʱ���¼�ʱ)
   }}}USR */
BOOL EPGSetTimeoutType( HCSHANDLE hEPG, int nTimeoutType );

/* {{{USR
������EPG����ʱ�Ƿ���SectionNum���ж�һ�����Ƿ��ظ����˽ӿ���Ϊ�˼���һЩ����������
����ͬһ��Servcie����ͬTableID��SectionNum�Ķο�����������
   }}}USR */
BOOL CSUSPEPGIsNeedSecNumWithSearch( HCSHANDLE hEPG, BOOL bIsNeedSecNum );

/* {{{USR
������EPG�����У��Ƿ���Ҫ�����Ѿ����ڵ�EPG���ݡ�
   }}}USR */
BOOL CSUSPEPGIsNeedUpdateExistEPG( HCSHANDLE hEPG, BOOL bIsSimpleEPGUpdate, BOOL bIsScheduleEPGUpdate );

/* {{{USR
��EPGģ���ڲ�����ռ��Ա��������Ϣ������չ����������Ϣ��
CSUSPEPGMallocMemory

Parameter:  
    [in] EPG    : ��Դ���
    [in] nCount ����ʾ������Ŀռ��С�� 

RETURN VALUE  
    NULL��ʾ����ʧ�ܣ���NULL ��ʾ����ɹ���
}}}USR */
PVOID CSUSPEPGMallocMemory( HCSHANDLE hEPG, int nCount );

/* {{{USR
�ͷ���ǰ��EPGģ���ڲ�����ռ�,����չ����������Ϣ��
CSUSPEPGFreeMemory

Parameter:  
    [in] hEPG         : EPG��Դ���
    [in] pvMemoryAddr ���ڴ�ָ�롣 
RETURN VALUE  
    1������ɹ�����TRUE
    2�����ʧ�ܷ���FALSE
}}}USR */
BOOL CSUSPEPGFreeMemory( HCSHANDLE hEPG, PVOID pvMemoryAddr );

/* {{{USR
�����Ƿ��EIT�İ汾�ŵı仯���к��ԣ�����EPGģ���ʼ��������EPG����ǰ���øú���
CSUSPEPGSetIsIgnoreEitVersionChange
Parameter:  
	[in] hEPG			: EPG��Դ���
	[in] bSimple		���Ƿ����P/F EIT��汾�ŵı仯�� ��TRUE����ʾ���ԣ�FALSE����ʾ�����ԣ�
	[in] bSchedule		���Ƿ����Schedule EIT��汾�ŵı仯����TRUE����ʾ���ԣ�FALSE����ʾ�����ԣ� 
RETURN VALUE  
	1������ɹ�����TRUE
	2�����ʧ�ܷ���FALSE

}}}USR */
BOOL CSUSPEPGSetIsIgnoreEitVersionChange( HCSHANDLE hEPG, BOOL bSimple, BOOL bSchedule );


/* {{{USR
���ý������ҳ��ּ���������ʱ�Ĺ��Ҵ��룬����EPGģ���ʼ��������EPG����ǰ���øú���
CSUSPEPGSetCountryCode
Parameter:  
	[in] hEPG				: EPG��Դ���
	[in] dwCountryCode		�����Ҵ��루Ӣ����3�ַ����롰GBR����ʾ������Ϊ����0100 0111 0100 0010 0101 0010����

	
Remark��
���Ҵ���  country_code��
	24λ�ֶΣ�����ISO 3166��3�ַ�����ָ�����ҡ�
	ÿ���ַ�����GB/T 15273.1-1994����Ϊ8λ�������β���24λ�ֶΡ�
	����3���ַ�������һ��900��999�����֣���ôcountry_codeָ����һ��ETSI����Ĺ��ҡ�������ETR 162
	���磺Ӣ����3�ַ����롰GBR����ʾ������Ϊ����0100 0111 0100 0010 0101 0010����

RETURN VALUE  
	1�����óɹ�����TRUE
	2������ʧ�ܷ���FALSE
}}}USR */
BOOL CSUSPEPGSetCountryCode( HCSHANDLE hEPG, DWORD dwCountryCode );

BOOL CSUSPEPGSetTunerDemuxPath( HCSHANDLE hEPG, HCSHANDLE hRCPath );

typedef struct _EPGTagNeedSaveInfo
{
	int m_nTagIDCount;              //Tag IDʵ�ʸ���
	BYTE m_ucTagID[8];  		    //Ҫ�����descriptor��TagID
} EPGTagNeedSaveInfo;

/* 
������Ҫ�������������Ϣ
CSUSPEPGSetNeedSaveTagInfo
Parameter:  
	[in] hEPG			: EPG��Դ���
	[in] psSimpleTagInfo		��Ҫ�����simple EPG����������Ϣ
	[in] psScheduleTagInfo		��Ҫ�����schedule EPG����������Ϣ
RETURN VALUE  
	1������ɹ�����TRUE
	2�����ʧ�ܷ���FALSE
Remark:
	�˽ӿ�����EPG_NEEDSAVE_PRIVATEDATAѡ��һ��ʹ�ã�
	��ʵ�ֱ����������Ĺ�����Ҫ����EPG_NEEDSAVE_PRIVATEDATAѡ�
	����EPG��ʼ��֮��	��ʼ����EPG��Ϣ֮ǰ����CSUSPEPGSetNeedSaveTagInfo
*/
BOOL CSUSPEPGSetNeedSaveTagInfo(HCSHANDLE hEPG, EPGTagNeedSaveInfo * psSimpleTagInfo, EPGTagNeedSaveInfo * psScheduleTagInfo);

/* {{{USR
����Schedule EIT �汾�ŷ����仯ʱ�Ƿ�ɾ��ԭ�����ݣ�����EPGģ���ʼ��������EPG����ǰ���øú���
CSUSPEPGSetIsDeleteOldDataWhenEitVerChange
Parameter:  
	[in] hEPG			: EPG��Դ���
	[in] bSchedule	: �Ƿ�ɾ��ԭ�����ݡ���TRUE����ʾɾ����FALSE����ʾ��ɾ����Ĭ�������ɾ���� 
RETURN VALUE  
	1������ɹ�����TRUE
	2�����ʧ�ܷ���FALSE

}}}USR */
BOOL CSUSPEPGSetIsDeleteOldDataWhenEitVerChange( HCSHANDLE hEPG, BOOL bSchedule );

#ifdef  __cplusplus
}
#endif
// }}}USR

#endif /* EPGReceiverInterface_H */
