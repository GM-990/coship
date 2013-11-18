#ifndef EPGUserInterface_H
#define EPGUserInterface_H

#include "mmcp_typedef.h"
#include "mmcp_debug.h"
#include "mmcp_os.h"

#include "CSUSPCommonType.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define EPG_MAX_EVENT_NAME_LEN 			    (64)
#define EPG_MAX_INFO_DESCRIPTION_LENGTH     (256)

//#ifdef LONG_EXT_DEC
//#define EPG_MAX_INFO_EXT_DESCRIPTION_LENGTH     (512) //NEC 9200A use it
//#else
#define EPG_MAX_INFO_EXT_DESCRIPTION_LENGTH     (256)

#define EPG_MAX_DESCRIPTOR_COUNT				(32)
//#endif

/*EPG�¼�����������*/
typedef enum 
{
	EM_EPG_ALL,		//��������������,��ʾ����
	EM_EPG_ZHO,		//����
	EM_EPG_ENG		//Ӣ��
}eEPGLangType;

typedef struct _EPGINFO
{

	WORD m_wEPGINFOSize;         //�˽ṹ��ߴ�

	BYTE m_ucLanguage;           //���뷽ʽ

	BYTE m_ucParental;           //��Ŀ���˼���
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];       //��Ŀ����

	HCSHANDLE m_pucUserData;                            //�û�˽�����ݣ������ã�
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;                     //��Ŀ��ʼʱ��
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;                       //��Ŀ����ʱ��
	CSUDIPLUSOSTIME_S m_DurationTIME;                               //��Ŀ����ʱ��
 
	BYTE m_ucContentLevel1;                             //��Ŀ����1
	BYTE m_ucContentLevel2;                             //��Ŀ����2

}EPGINFO,*PEPGINFO;

typedef struct _EPGDECINFO
{
	WORD m_wEPGINFOSize;
	
	BYTE m_ucLanguage;

	BYTE m_ucParental;
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];

	HCSHANDLE m_pucUserData;
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;
	CSUDIPLUSOSTIME_S m_DurationTIME;

	BYTE m_pucDescription[ EPG_MAX_INFO_EXT_DESCRIPTION_LENGTH ];      //��Ŀ���

	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ];

}EPGDECINFO,*PEPGDECINFO;


typedef struct _EPGEXTINFO
{
	WORD m_wEPGINFOSize;
	
	BYTE m_ucLanguage;

	BYTE m_ucParental;
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];

	HCSHANDLE m_pucUserData;
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;
	CSUDIPLUSOSTIME_S m_DurationTIME;

	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //�̽�Ŀ���

	int m_nExtDesLen;              //��Ŀ��鳤��
	BYTE * m_pucDescription;      //��Ŀ���

}EPGEXTINFO,*PEPGEXTINFO;

typedef struct _EPGEXT2INFO
{
	WORD m_wEPGINFOSize;
	
	BYTE m_ucLanguage;

	BYTE m_ucParental;
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];

	HCSHANDLE m_pucUserData;
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;
	CSUDIPLUSOSTIME_S m_DurationTIME;

	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //�̽�Ŀ���

	int m_nExtDesLen;              //��Ŀ��鳤��
	BYTE * m_pucDescription;      //��Ŀ���

	BYTE m_pucISO639LanguageCode[3];
	BYTE m_bRunningStatus;
	BYTE m_bFreeCAMode;

	int m_nDescriptorCount;
	PBYTE m_DescriptorArray[EPG_MAX_DESCRIPTOR_COUNT];

	int m_nExtItemsLen;		//Item�����ܳ��ȣ��û���������Item���ݵ�buffer��С
	BYTE* m_pucExtItems;	//Item�������ݵ�buffer�����û��ڻ�ȡʱ����

}EPGEXT2INFO,*PEPGEXT2INFO;

typedef struct _EPGEXT3INFO
{
	WORD m_wEPGINFOSize;
	
	BYTE m_ucLanguage;

	BYTE m_ucParental;
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];

	HCSHANDLE m_pucUserData;
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;
	CSUDIPLUSOSTIME_S m_DurationTIME;

	BYTE m_ucContentLevel1;
	BYTE m_ucContentLevel2;

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //�̽�Ŀ���

	BYTE m_pucDescription[EPG_MAX_INFO_DESCRIPTION_LENGTH];      //��Ŀ���

	BYTE m_pucISO639LanguageCode[3];
	BYTE m_bRunningStatus;
	BYTE m_bFreeCAMode;

	int m_nDescriptorCount;
	PBYTE m_DescriptorArray[EPG_MAX_DESCRIPTOR_COUNT];

	int m_nExtItemsLen;		//Item�����ܳ��ȣ��û���������Item���ݵ�buffer��С
	BYTE* m_pucExtItems;	//Item�������ݵ�buffer�����û��ڻ�ȡʱ����
	WORD m_wEventId;

}EPGEXT3INFO,*PEPGEXT3INFO;

/*
GetEPGFindExtItemDesbyIndex
 ��ȡָ��Networkָ��TS��ָ��Serviceָ��Eventָ��������ItemDescription 

Parameters��
  pucExtItems
        [in]Items��buffer,��EPGEXT2INFO��m_pucExtItems���
  nExtItemsLen
        [in]Items��buffer��С
  nIndex
        [in]Ҫ��ȡ��ItemDescription��������0��ʼ
  pucItemDes 
        [out]��ȡ����ItemDescription���ݣ����е�һ���ֽ�Ϊitem_description_length������Ϊitem_descriptionԭʼ����
  nItemDesLen 
        [int]������ȡItemDescription����buffer�ĳ���

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
 */
BOOL GetEPGFindExtItemDesbyIndex(const BYTE* pucExtItems, int nExtItemsLen, int nIndex, BYTE* pucItemDes, int nItemDesLen);

/*
GetEPGFindExtItembyIndex
 ��ȡָ��Networkָ��TS��ָ��Serviceָ��Eventָ��������Item 

Parameters��
  pucExtItems
        [in]Items��buffer,��EPGEXT2INFO��m_pucExtItems���
  nExtItemsLen
        [in]Items��buffer��С
  nIndex
        [in]Ҫ��ȡ��Item��������0��ʼ
  pucItem 
        [out]��ȡ����Item���ݣ����е�һ���ֽ�Ϊitem_length������Ϊitemԭʼ����
  nItemLen 
        [int]������ȡItem����buffer�ĳ���

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
 */
BOOL GetEPGFindExtItembyIndex(const BYTE* pucExtItems, int nExtItemsLen, int nIndex, BYTE* pucItem, int nItemLen);

/* {{{USR
GetPresentEPG
 ��ȡָ��Networkָ��TS��ָ��Service��Present EPG 
 BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
      wServiceID, EPGINFO * pEPGINFO );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, EPGINFO * pEPGINFO );

/* {{{USR
GetPresentEPG
 ��ȡָ��Networkָ��TS��ָ��Service��Present EPG 
 BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
      wServiceID, EPGINFO * pEPGINFO );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetPresentEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, EPGINFO * pEPGINFO );

/* {{{USR
GetFollowEPG
 ��ȡָ��Networkָ��TS��ָ��Service��Following EPG 
 BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID, EPGINFO * pEPGINFO );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, EPGINFO * pEPGINFO );

/* {{{USR
GetFollowEPG
 ��ȡָ��Networkָ��TS��ָ��Service��Following EPG 
 BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID, EPGINFO * pEPGINFO );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetFollowEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, EPGINFO * pEPGINFO );

/* {{{USR
GetDayTimeEPG
  ��ȡָ��Networkָ��TS��ָ��Serviceָ��ʱ��ε�Schedule EPG��Ϣ
  HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
   wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S *pEndTSYSTEMTIME );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pStartTSYSTEMTIME
        [in]��ʼʱ��
  pEndTSYSTEMTIME 
        [in]����ʱ��

Return Values
    EPG���ݵľ��

   }}}USR */
HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
GetDayTimeEPG
  ��ȡָ��Networkָ��TS��ָ��Serviceָ��ʱ��ε�Schedule EPG��Ϣ
  HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
   wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S *pEndTSYSTEMTIME );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  pStartTSYSTEMTIME
        [in]��ʼʱ��
  pEndTSYSTEMTIME 
        [in]����ʱ��

Return Values
    EPG���ݵľ��

   }}}USR */
HCSHANDLE GetDayTimeEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
GetDayTimeEPGCount
  ��ȡָ��ʱ�����,ָ��EPG���ݾ����EPG��Ϣ�ĸ���
  int GetDayTimeEPGCount( HCSHANDLE hGetData, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME,
            CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );
 
Parameters:
   hGetData
        [in]EPG���ݾ��
   pStartTSYSTEMTIME
        [in]��ʼʱ��
   pEndTSYSTEMTIME 
        [in]����ʱ��

Return Values:
           EPG��Ϣ�ĸ���������0��ʾʧ��
   }}}USR */
int GetDayTimeEPGCount( HCSHANDLE hGetData );

/* {{{USR
GetNewHandleIndex
  ��ȡĳ��ָ��EPG��Ϣ���µ����ݾ���е�����ֵ
  int GetNewHandleIndex( HCSHANDLE hSrcGetData, int nSrcIndex, HCSHANDLE 
          hNewGetData );

Parameters:
   hSrcGetData
        [in]ԭ�ȵ�EPG���ݾ��
   nSrcIndex
        [in]ָ��EPG��Ϣ��ԭ��EPG���ݾ���е�����
   hNewGetData 
        [in]�µ�EPG���ݾ��

Return Values:
        EPG��Ϣ���µ����ݾ���е�����ֵ������0��ʾʧ��
   }}}USR */
int GetNewHandleIndex( HCSHANDLE hSrcGetData, int nSrcIndex, HCSHANDLE hNewGetData );

/* {{{USR
GetNextEPG
  ��ȡָ��EPG���ݾ����EPG��Ϣ����������һ��EPG���ݵľ��
  HCSHANDLE GetNextEPG( HCSHANDLE hData, EPGINFO * pEPGINFO );

Parameters:
   hData
        [in]EPG���ݾ��
   pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values:
        ��һ��EPG���ݵľ��������NULL��ʾ����
   }}}USR */
HCSHANDLE GetFirstEPG( HCSHANDLE hGetData, EPGINFO * pEPGINFO );

/* {{{USR
GetNextEPG
  ��ȡָ��EPG���ݾ����EPG��Ϣ����������һ��EPG���ݵľ��
  HCSHANDLE GetNextEPG( HCSHANDLE hData, EPGINFO * pEPGINFO );

Parameters:
   hData
        [in]EPG���ݾ��
   pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values:
        ��һ��EPG���ݵľ��������NULL��ʾ����
   }}}USR */
HCSHANDLE GetNextEPG( HCSHANDLE hGetData, EPGINFO * pEPGINFO );

/* {{{USR
GetEPGByIndex
   ��ȡָ��EPG���ݾ����ָ��������EPG����
   BOOL GetEPGByIndex( HCSHANDLE hGetData, int nIndex, EPGINFO * pEPGINFO );

Parameters��
  hGetData
        [in]EPG���ݾ��
  nIndex
        [in]����ֵ
  pEPGINFO 
        [out]����EPG��Ϣ�Ľṹ��ָ��

Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetEPGByIndex( HCSHANDLE hGetData, int nIndex, EPGINFO * pEPGINFO );

/* {{{USR
GetTDATECount
  ��ȡָ��Networkָ��TS��ָ��Service������EPG���ݵ�����
  int GetTDATECount( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
            wServiceID );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ

Return Values:
        ��������
   }}}USR */
int GetTDATECount( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
GetTDATECount
  ��ȡָ��Networkָ��TS��ָ��Service������EPG���ݵ�����
  int GetTDATECount( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
            wServiceID );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ

Return Values:
        ��������
   }}}USR */
int GetTDATECountEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR
GetTDATEByOffset
   ��ȡָ��Networkָ��TS��ָ��Serviceָ������������EPG���ݵ���ϸ������Ϣ
   BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
        wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  nDayIndex
        [in]��������
  pTDATE 
        [out]����������Ϣ�Ľṹ��ָ��

Return Values:
        BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

/* {{{USR
GetTDATEByOffset
   ��ȡָ��Networkָ��TS��ָ��Serviceָ������������EPG���ݵ���ϸ������Ϣ
   BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
        wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  nDayIndex
        [in]��������
  pTDATE 
        [out]����������Ϣ�Ľṹ��ָ��

Return Values:
        BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetTDATEByOffsetEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

/* {{{USR
DeleteServiceEPG
  ɾ��ָ��Networkָ��TS��ָ��Service������EPG����
  BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID )��

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  
Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
DeleteServiceEPG
  ɾ��ָ��Networkָ��TS��ָ��Service������EPG����
  BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID )��

Parameters��
  hEPG
        [in]EPG��Դ���
  wNetworkID
        [in]ָ��Network��IDֵ
  wTsID
        [in]ָ��TS��IDֵ
  wServiceID 
        [in]ָ��Service��IDֵ
  
Return Values
  BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL DeleteServiceEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR
DeleteAllEPG
  ɾ������EPG����
  BOOL DeleteAllEPG( HCSHANDLE hEPG )��

Parameters��
  hEPG
        [in]EPG��Դ���
   
Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL DeleteAllEPG( HCSHANDLE hEPG );

/* {{{USR
��ʱ����
   }}}USR */
BOOL FreeGetDataHandle( HCSHANDLE hGetDataHandle );

/* {{{USR
SetTimeZone
  ����EPG��Ϣ��ʱ��ƫ�ƣ�ʱ��������ʱ����[-12��13] ��

Parameters��
  hEPG
    [in]EPG��Դ���
 fTimeOffset
	[In]��ʱ��ƫ���� �����磺2.5  ��ʾ2Сʱ30�֣�

Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��

Note
	���鲻��ʹ��SetTimeZone�����ö�Ӧ����CSUSPEPGSetTimeZone.
	����ԭ����SetTimeZone,����������linuxƽ̨����ͬ�����������ԣ����ƽ̨��linux(����broadCom)��
	��������������CSUSPEPGSetTimeZone��
   }}}USR */

BOOL SetTimeZone( HCSHANDLE hEPG, float fTimeOffset );
BOOL CSUSPEPGSetTimeZone( HCSHANDLE hEPG, float fTimeOffset );

/* {{{USR
GetStreamCurTime
  ��ȡ��ǰ�����ĵ�ǰʱ�䡣

Parameters��
  hEPG
    [in]EPG��Դ���
 wOriginalNetworkID
	[in]ָ��Network��IDֵ
 pStreamTSYSTEMTIME
    [out] �����-��-�� ʱ-��-�����ݡ�
 bIsAddTimeZone  
    [In] ��ʾ�Ƿ��ʱ��ƫ�Ƽ���ʱ��������С�False��ʾ����ʱ��ƫ�ơ�True��ʾ��ƫ��

Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetStreamCurTime( HCSHANDLE hEPG, WORD wOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStreamTSYSTEMTIME, BOOL bIsAddTimeZone );

/* {{{USR
GetStreamCurTimeEx
    ��ȡ��ǰ�����ĵ�ǰʱ�䡣�˽ӿ���GetStreamCurTime���������ڶ���һ��dwUserOriginalNetworkID������
	�����������һ����ʾ���� 

Parameters��
  hEPG
    [in]EPG��Դ���
 wOriginalNetworkID
	[in]ָ��Network��IDֵ
 dwUserOriginalNetworkID
    [in]�û�ָ��������ı�ʾ��
 pStreamTSYSTEMTIME
    [out] �����-��-�� ʱ-��-�����ݡ�
 bIsAddTimeZone  
    [In] ��ʾ�Ƿ��ʱ��ƫ�Ƽ���ʱ��������С�False��ʾ����ʱ��ƫ�ơ�True��ʾ��ƫ��

Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL GetStreamCurTimeEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStreamTSYSTEMTIME, BOOL bIsAddTimeZone );

/* {{{USR
PostStreamTSYSTEMTIME
   ��ָ���������õ�ǰ����ʱ��͵�ǰϵͳ��Tick��ֵ���˽ӿ�������EPGģ��֪ͨ��ʱ�䣬
   ��ɾ�����ڵ�EPG���ݡ�

Parameters��
  hEPG
    [in]EPG��Դ���
 wOriginalNetworkID
	[in]ָ��Network��IDֵ
 dwUserOriginalNetworkID
    [in]�û�ָ��������ı�ʾ��
 pStreamTSYSTEMTIME
    [In]�� ������-��-�� ʱ-��-�����ݡ�
 nTickCount  
    [In]������ϵͳ��Tickֵ��

Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��

   }}}USR */
BOOL PostStreamTSYSTEMTIME( HCSHANDLE hEPG, WORD wOriginalNetworkID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * streamTSYSTEMTIME, DWORD nTickCount );

/* {{{USR
PostOneSystemTick
   ָʾEPGģ���ȡ��ǰϵͳ��Tick��ֵ���Ա����EPG�洢�ĵ�ǰ��ʱ�䣬����ɾ�����ڵ�EPG��Ϣ��

Parameters��
  hEPG
    [in]EPG��Դ���

Return Values
    BOOL TRUE��ʾ�ɹ���FALSE��ʾʧ��
   }}}USR */
BOOL PostOneSystemTick( HCSHANDLE hEPG );

/* {{{USR
�û�ָ��EPG��������������ʱ��Σ���
 ���wOriNetworkID, wTSID��wServiceID����Ϊ0xffff,
       ˵��ֻ����ָ��Service��ʱ����ڵ�EPG��
 ���wOriNetworkID, wTSID��Ϊ0xffff, wServiceID ����0xffff,
       ˵��ֻ����ָ��TS��ʱ����ڵ�EPG��
 ���wOriNetworkID��Ϊ0xffff, wTSID,wServiceID ����0xffff,
       ˵������ָ��Network��ʱ����ڵ�EPG��
 pStartTime �� pEndTime ��ʱ��εĿ�ʼʱ��ͽ���ʱ�䡣
 
 return BOOL
   }}}USR */
BOOL CSUSPSetEPGRecDuration( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTSID, WORD wServiceID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStartTime, CSUDIPLUSOSSysTime_S * pEndTime );

/* {{{USR
CSUSPEPGGetCurrentIndex
   �õ�ָ��EPG���ݾ���е�ǰ���ڲ��ŵ�EPG�ڵ������ֵ��

Parameters��
 hGetData
    [in]EPG���ݾ��
 pCurTSYSTEMTIME
    [In]����ǰʱ�䣬���ڶ�λ��ǰ���ڲ��ŵ�EPG�ڵ㡣

Return Values
    int ���ص�ǰ���ڲ��ŵ�EPG�ڵ������ֵ��

   }}}USR */
int CSUSPEPGGetCurrentIndex( HCSHANDLE hGetData, CSUDIPLUSOSSysTime_S * pCurTSYSTEMTIME );

/* {{{USR
���ö�����ʱ��ƫ�ƣ�רΪ���ǻ�ʹ�ã���������ÿ����������һ��ʱ��ƫ�ơ�
   }}}USR */
BOOL CSUSPEPGSetTimeOffsetForNetwork( HCSHANDLE hEPG, WORD wOrgNetworkID, DWORD dwUserData, int nSecTime );

/* {{{USR
CSUSPEPGSetEventNameCCTableIndexFlag
�趨EPG������������ͱ�־λ�Ĵ������ͣ�

Parameters��
hEPG
[in]EPG���ݾ��
[out]bIsGetOriginal: �Ƿ�ȡ��ԭʼ��Ŀ������ͱ�־λ
TRUE:  m_ucLanguageֵΪԭʼ��Ŀ������ͱ�־λ
FALSE: m_ucLanguageΪUSP_CODING_UNICODE��USP_CODING_ASC��Ĭ��Ϊ����״̬

Return:

SUCCESS: ����TRUE
FAILURD: ����FALSE

Description:

��hEPG��Чʱ������ʧ��
}}}USR */
BOOL CSUSPEPGSetEventNameCCTableIndexFlag( HCSHANDLE hEPG, BOOL bIsGetOriginal );

/* {{{USR
CSUSPEPGSetEventNameLanguageType
�趨����EPG�¼���Ϣ��ʱ�����������

Parameters��
hEPG
[in]eLangCode ��ö�ٶ������͡�
Return:
TRUE:  ���óɹ�
FALSE: ����ʧ��

Description:

}}}USR */
BOOL CSUSPEPGSetEventNameLanguageType(eEPGLangType eLangCode);

// {{{USR
#ifdef  __cplusplus
}
#endif
// }}}USR

#endif /* EPGUserInterface_H */

