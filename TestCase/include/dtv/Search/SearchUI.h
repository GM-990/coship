#ifndef SearchUI_H
#define SearchUI_H

#ifdef PRAGMA
#pragma interface "SearchUI.h"
#endif
#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"

/* {{{USR

   }}}USR */

// {{{USR
#ifdef __cplusplus
extern "C"
{
#endif

//��ǿ�������ļ������͵ĺ궨��
//��Щ��ǿ�����Ͳ���ͬʱʹ�á�
// SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS��SEARCH_NIT_MAIN_CUSTOM��
//SEARCH_NIT_GET_TAG��SEARCH_NIT_GET_TAG_ONCE����ͬʱʹ��
#define SEARCH_BAT_BOUQUETID_TRANSPONDER      (0x00000001)     				//��BAT�����Ҫ������Ƶ�����,���Ҳ���BAT��Ŀ����, ����bouquet_id��
#define SEARCH_BAT_BOUQUETID_SERVICE          (0x00000002)     				//��BAT������������Ƶ�㼰��Ŀ�Ĺ��ˣ�����bouquet_id��
#define SEARCH_BAT_GET_TAG                    (0x00000004)     				//����BAT�����ӽ�Ŀʱ������ص�BAT Tag��
#define SEARCH_BAT_GET_SECTION                (0x00000008)     				//����BAT����ʱ������ص�BAT Section��
#define SEARCH_EASY_INSTALL                   (0x00000010)     				//���м�����������Search������Ƶ���õ�diseqc�˿ڡ�
#define SEARCH_NIT_GET_TAG                    (0x00000020)     				//����NITPATPMT��NITSDT�����ӽ�Ŀʱȡ��NIT Tag�������ظ���
                                                               				//��,Ĭ���������д���Ƶ���������Ƶ���TAG
#define SEARCH_SDT_GET_TAG                    (0x00000040)     				//����NITPATPMT�����ӽ�Ŀʱȡ��SDT Tag�����ظ��û���
#define SEARCH_SERVICE_SAVE_CUSTOM            (0x00000100)     				//���û������Զ���Ĵ洢��
#define ONLY_SEARCH_EXTERNAL_FREQ_NIT_TAG     (0x00000200)     				//����NIT tagʱ������ú��ʾ"ֻ��������Ƶ���NIT tag",�ú�ֻ�ں�SEARCH_NIT_GET_TAG
                                                               				//ͬʱʹ�ò���Ч������������û�ж���SEARCH_NIT_GET_TAG���ú���Ч.
#define SEARCH_NIT_SYNCHRONOUS                (0x00000400)     				//NITͬ���������������Ƕ�NIT�������Ż���Ŀǰ�������ǻ�������Ч

#define ADVANCED_EASYINSTALL_SEARCH           (0x00000800)     				//��������ʱ����û��Ѿ����ö˿ڣ����ٽ����Զ����ö˿�
#define ADVACNED_EASYINSTALL_SEARCH           (ADVANCED_EASYINSTALL_SEARCH) //�ɵĴ���ƴд

#define SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS	  (0x00001000)					//NITPATPMT������ʽ�����ѵ���һ��NITƵ��ʱ�����Deliver list����SEARCH_NIT_MAIN_CUSTOM����ͬʱʹ��
#define SEARCH_NIT_MAIN_CUSTOM				  (0x00002000)			//������Ƶ���������ڶ������Ƶ��������,ֱ���ҵ���Ƶ���з��͵�NIT�����Ҫ������ҵ������Դ�NIT��Ϊ��ڽ���NITPATPMT��������SEARCH_ON_FRIST_NIT_CLEAR_DELIVERS����ͬʱʹ��
//��������ʱҪ���ƥ����Ϣ
#define SEARCH_PAT_TSID_MATCH					(0x00004000)			//����PAT��ʱҪ��PAT�е�TS_IDͬNIT���е�������ƥ��
#define SEARCH_SDT_TSID_MATCH					(0x00008000)			//����SDT��ʱҪ��SDT���е�TS_IDͬNIT���е�������ƥ��	
#define SEARCH_SDT_ORINETWORKID_MATCH			(0x00010000)			//����SDT��ʱҪ��SDT���е�Ori_Network_IDͬNIT���е�������ƥ��
#define SEARCH_BAT_ALL						  (0x00020000)					//����ÿһ��Ƶ���BAT��Ĭ�������ֻ������Ƶ���BAT��
#define SEARCH_NIT_GET_TAG_ONCE				(0x00040000)				//������NIT���ָ��������һ�Σ��ѵ��Ͳ�������
#define SEARCH_NIT_THROW_TAG_IF_HAVE			(0x00080000)				//ֻҪ��NIT���з�����ָ�����������׳��������ܸ��������������Ƿ���չ�����������Ľ�Ŀ��������NIT���ж��Section������һ����������һ�µ����,
																		//ֻ�����Ҳ�������Ŀ��ص�������������²�ִ�д˲��ԣ�����ָ�������������׳����Զ������������ԣ�����������Ҫ����SEARCH_NIT_GET_TAG��SEARCH_GET_TAG_ONCE���ֲ����е�һ�ִ˲��Բ�������

#define QUICK_SEARCH_EXT	(0x00100000)					//��չNITSDT�������˲������������NIT���ͬʱ��������SDT other���ٶȸ���
#define SEARCH_IP_SERVICE			(0x00200000)					//IP����
#define SEARCH_NIT_OTHER	(0x00400000)		//NIT����ʱ����NIT Other��
#define SEARCH_SAVE_TS_WITH_NOSERVICE	(0x00800000)			//������Ϻ󱣴�û�н�Ŀ��Ƶ�㣬Ĭ��������ǲ������
#define QUICK_SEARCH_ONLY_SDT   (0x01000000)    //��չNITSDT�������˲�������SDT_Actrual��
#define QUICK_SEARCH_SDT        (0x02000000)    //��չNITSDT�������˲�������SDT_Actual���SDT_other��

#define QUICK_SEARCH_SDT_EXT_TIMEOUT   (0x04000000)       //��չNITSDT�������˲������������NIT���ͬʱ��������SDT other���ٶȸ��죬������QUICK_SEARCH_EXT��ͬ��SDT�����ȫ����SDT ��ʱ
#define QUICK_SEARCH_SDT_TIMEOUT       (0x08000000)        //��չNITSDT�������˲�������SDT_Actual���SDT_other�� ������QUICK_SEARCH_SDT��ͬ��SDT other�����ȫ����SDT��ĳ�ʱ

typedef enum callbackType
{
	SEARCH_NIT_FREQLIST,			//��ǰƵ���NIT���������
	SEARCH_FREQ_START,				//��ʼ������ǰƵ��
	SEARCH_FREQ_FINISHED,			//��ǰƵ��Ľ�Ŀ�������
	SEARCH_FREQ_NITSDT,				//��ǰƵ���NITSDT�������(����������ǰ����)
	SEARCH_FINISHED,				//�������

	SIGNAL_CONNECTED,				//Ƶ�������ɹ�
	SIGNAL_SEARCHING,				//��������Ƶ��
	SIGNAL_LOST,					//Ƶ������ʧ��
	SIGNAL_DROPPED,					//��Ƶ�������ɹ�תΪ����ʧ��

	SOURCE_PARAM_CHANGED,			//Ƶ������ı�
	SOURCE_SIMILARLY_PARAM,			//Ƶ�������ͬ
	SOURCE_FAILD,					//���ܻ�ȡTuner
	NIT_SEARCH_FINISHED,			//����Ƶ���NIT �������
	START_TRANSPONDER_SEARCH,		//��ʼ����Ƶ��,Ŀǰֻ�������������õ�

    SERVICE_ISNEEDSAVE,             //�Ƿ���Ҫ����ý�Ŀ

	SEARCH_BAT_SECTION_COMING,      //BAT����ʱ��Section���ݵ���
	SEARCH_BAT_FREQLIST,            //��ǰƵ���BAT���������
	SEARCH_BAT_FINISHED,            //����Ƶ���BAT �������
	CAT_SECTION_COMMING,            //�׳�CAT��

	SEARCH_PMT_SERVICE_TYPE,		//�׳�SearchPMTServiceType�������ý�Ŀ����,����ΪSearchPMTServiceType
	NIT_ISCUSTOM,					//�Ƿ�Ϊ�Զ�����Ƶ��?�׳���Ƶ���������Ϣ��Ӧ���ж�,����SEARCH_NIT_MAIN_CUSTOM
	NIT_SEARCH_NEW_DELIVER,			//�ڴ�NIT�������У�ÿ��nit�����½�����һ��tsloop���׳��������ݣ��׳�����Ϊָ��SITsInfo�ṹ��ָ��

	SEARCH_NITSDT_TIMEOUT,		//��������ʱ����nitsdt��ʱ,����searchģ�����sau(��Ŀ�Զ�����)����ʱ������ݴ˻ص��ж��Ƿ���Ҫ������µ�������
	SEARCH_PATPMT_TIMEOUT,		//patpmt����ʱpatpmtsdt��ʱ���÷�ͬ��
	SEARCH_SERVICEINFO_EXT    //SIServiceInfoExt��Ӧ������
}CallBackType;

typedef enum _DATAPROCESSSTRATEGY
{
	SEARCH_PROCESS_DATA_DEFAULT = 0, //Ĭ�����ݴ������
	SEARCH_PROCESS_DATA_SKIP, 		//�����Ը�Ƶ�����ݵĴ���
	SEARCH_PROCESS_DATA_STOP		//ֹͣ���������ݵĴ����ᵼ��ֹͣ����
}DATAPROCESSSTRATEGY;

//Ϊÿ��������������ʱʱ�䣬�ɸ���ʵ�����������չ
//�贴����������Ǵ���ĳ�ʱʱ��ΪM�룬��PAT��ʱʱ��Ϊ1/5M��PMT��ʱʱ��ΪM-2��
//SDT��ʱʱ��ΪM����Щ����ʱ���ṩ��������
typedef enum _SEARCHTABLETYPE
{
	USP_SEARCH_NIT = 0,  //����NIT
	USP_SEARCH_BAT,  //����BAT
	USP_SEARCH_CAT   //����CAT
}SEARCHTABLETYPE;

typedef enum _DELIVERSTATE
{
	SEARCH_DELIVER_RESET,			//��ʼ״̬
	SEARCH_DELIVER_NIT,				//��ǰƵ��NIT������
	SEARCH_DELIVER_SERVICE,			//��ǰƵ���Ŀ�������
	SEARCH_DELIVER_NOSIGN,			//��ǰƵ��û���ź�
	SEARCH_DELIVER_SKIP,			//��ǰƵ��ȡ������
	SEARCH_DELIVER_TIMEOUT			//��ǰƵ��������Ŀ��ʱ��ֻ����Ƶ��patpmtsdt��ʱʱ�Ż��¼Ϊ��ʱ��nit,cat,bat��ʱ��Ȼ��ΪSEARCH_DELIVER_SERVICE״̬
}DELIVERSTATE;

typedef struct _SEARCHNITLIST
{
	int m_nDeliverIndex;			//��ǰNITƵ�������(�Դ���SearchUIʱ�����Ƶ�������˳��Ϊ׼)
    int m_nNITCount;				//��ǰNIT���а�����Ƶ����(����������Ƶ�㣬���Ƶ���Ѱ�����Ƶ���б��У����Ƶ�㲻�ظ���ӣ�
	int m_nNITStartIndex;			//��ǰƵ��NIT���а����Ŀ�ʼƵ�����µ�Ƶ����е�����
}SEARCHNITLIST,*PSEARCHNITLIST;

typedef struct _SEARCHFREQLIST
{
	int m_nDeliverIndex;			//��ǰƵ�������(����������Ƶ���˳��Ϊ׼)
    int m_nVideoCount;				//��ǰƵ������������Video��Ŀ��
	int m_nRadioCount;				//��ǰƵ������������Radio��Ŀ��
	int m_nVidoeStartIndex;			//��ǰƵ���еĵ�һ��Video��Ŀ��Video�����е�����
	int m_nRadioStartIndex;			//��ǰƵ���еĵ�һ��Radio��Ŀ��Radio�����е�����
}SEARCHFREQLIST,*PSEARCHFREQLIST;

typedef struct _SEARCHFREQLISTEx
{
	SEARCHFREQLIST m_FreqList;
	int m_ServiceCount;			//��ǰƵ������������Service��
	int m_ServiceStartIndex;		//��ǰƵ���еĵ�һ��Service ��Ŀ�ڶ����е�����
}SEARCHFREQLISTEx,*PSEARCHFREQLISTEx;

typedef enum 
{
	SEARCH_UI_TYPE_SATELLITE,       //���������µ�����Ƶ�㣬����NIT
	SEARCH_UI_TYPE_SATELLITE_NIT,   //���������µ�����Ƶ�㣬��NIT
	SEARCH_UI_TYPE_TRANSPONDER,     //����Ƶ�㣬����NIT
	SEARCH_UI_TYPE_TRANSPONDER_NIT, //����Ƶ�㣬��NIT
    SEARCH_UI_TYPE_BLINDSCAN,        //äɨ����
    SEARCH_UI_TYPE_NITSDT
} SearchType;

typedef struct _SERVICEINFOFORCLIENT
{
	HCSHANDLE m_hServiceData;		//��Ŀ�����ݹ����еĴ�ž��
    void *m_pvReserved;				//Ԥ������
} SERVICEINFOFORCLIENT;

//������BAT Tag��BouquetID��Ϣʱ��������Ϣ
typedef struct _BATRequestInfo
{
	BYTE m_ucTagID[8];  		    //BAT TagID
	int m_nTagIDCount;              //Tag IDʵ�ʸ���
	WORD m_wBouquetID[32];       	//BouquetID
	int m_nBouquetIDCount;          //BouquetIDʵ�ʸ���
} BATRequestInfo;

//������Tag(˽��������)ʱ��������Ϣ
typedef struct _SearchTagRequestInfo
{
	BYTE m_ucTagID[8];  		    //��������TAG��TagID
	int m_nTagIDCount;              //Tag IDʵ�ʸ���
} SearchTagRequestInfo;

//��������ʱ��������Ϣ��
typedef struct _EasyInstallRequestInfo
{
   BOOL m_bScanAll;              //�Ƿ�ɨ������Ƶ�㡣TRUEɨ������Ƶ��ֱ���ҵ���һ������ס��Ϊֹ��FALSEֻɨ���һ�����粻����ס���˳�������������ס�����������������Ƶ�㡣
   BOOL m_bModifyDiseqcInfo;     //��סƵ����޸����ǵ�Diseqc������TRUE���޸ģ�FALSE�����޸ġ�
   DWORD m_dwDiseqcType;         //0-diseqc1.0, 1-diseqc1.1, 2-diseqc1.2
} EasyInstallRequestInfo;
//äɨ��������Ϣ��
typedef struct _BlindScanRequestInfo
{
    DWORD  dwStartFreq;           //��ʼƵ��Ƶ��
    DWORD  dwEndFreq;             //����Ƶ��Ƶ��
    DWORD  dwStepLength;       	  //����        
    DWORD  dwSymbolRate[8];       //�����ʵ�����
    DWORD  dwSymbolRateCount;  	  //��������Чֵ����
    DWORD  dwPolarization[2];     //������ʽ
    DWORD  dwPolarizationCount;   //������ʽ��Ч����
    DWORD  dwFEC_inner;           //�ڲ�ǰ�������
    HCSHANDLE hSatellite;         //Ҫ���������Ǿ��
    HCSHANDLE hAntenna;           //Ҫ���������߾����
    BOOL    m_bScanAll;           //�Ƿ�ɨ������Ƶ�㡣TRUEɨ������Ƶ�㲢�ҵ���������//ס��Ƶ�㣬FALSEֻ�ҵ���һ������ס��Ƶ��
} BlindScanRequestInfo;

//��ǿ����ʱ������������Ϣ
typedef struct _SearchExRequestInfo
{
	BATRequestInfo m_BATRequestInfo;   //BAT������Ϣ
	SearchTagRequestInfo m_NITTagRequestInfo;   //NIT Tag������Ϣ��
	SearchTagRequestInfo m_SDTTagRequestInfo;   //SDT Tag������Ϣ��
} SearchExRequestInfo;

//�ص�ʱ����BAT����Ϣ�Ľṹ��
typedef struct _BATInfo
{
	WORD m_wBouquetID;                //��Ӧ�����BouquetID
	BYTE *m_pucBATInfo;               //bat��Ϣ�ַ���
	DWORD m_dwBATInfoLength;          //bat��Ϣ���ܳ���
    BYTE *m_pucBouquetName;           //BouquetName
    DWORD m_dwBouquetNameLength;      //BouquetName�ĳ���
} BATInfo;

//�ص�ʱ���ص�Tag(˽��������)��Ϣ�Ľṹ��,���ڷ���CAT��ʱ�����������Ϣ
typedef struct _SearchPrivateInfo
{
   BYTE *m_pucPrivateInfo;                //��Ϣ�ַ���.����cat��m_pucPrivateInfo��һ��cat section�����ݡ�
   										//����nit��sdt��bat��m_pucPrivateInfo�ɶ���������������ɣ�m_dwPrivateInfoLength���������������ȼ��������ܳ��ȡ�
   										//��ʽ����:||des1.tag|des1.dataLen|des1.data||des2.tag|des2.dataLen|des2.data||...||desn.tag|desn.dataLen|desn.data
   											//
   DWORD m_dwPrivateInfoLength;           //m_pucPrivateInfo��Ϣ���ܳ��ȡ�
} SearchPrivateInfo;

//��ǿ����ʱ�������Ӧ��Ϣ
typedef struct _SearchExResponseInfo
{
	int m_nDeliverIndex;	  		                 //��ǰƵ�������(����������Ƶ���˳��Ϊ׼)
	BATInfo m_BATResponseInfo;                       //������
    SearchPrivateInfo m_NITResponseInfo;             //NIT Tag��Ӧ��Ϣ�����ص���һ��Ƶ�����Ϣ��
	SearchPrivateInfo m_SDTResponseInfo;             //SDT Tag��Ӧ��Ϣ�����ص���һ��Ƶ�����Ϣ��
} SearchExResponseInfo;

//�Զ���洢��Ŀʱ�Ľ�Ŀ�����Ϣ�ṹ��
typedef struct _SearchServiceHandleInfo{
	HCSHANDLE m_hService;           //��δ�������뵽���ݹ����еĽ�Ŀ���������CSUSPDMAddTailDataHandle�����������롡
	WORD m_wServiceID;              //�����Ľ�ĿID
	BYTE m_ucServiceType;           //��Ŀ����
	int m_nDeliverIndex;            //����������Ƶ���������
	HCSHANDLE m_nDeliverHandle;     //�ý�Ŀ����Ƶ�������ݹ����еľ��
} SearchServiceHandleInfo;


typedef struct _SearchPMTServiceType
{
	BYTE m_ucStreamType;		//��ǰPMTѭ����StreamType�ֶ�
	WORD m_wElementaryPid;		//��ǰPMTѭ����elementary_PID�ֶ�
	PBYTE m_pDescriptors;		//��ǰPMT��һ������ǰѭ������������ʱ��ָ�����ĵ�������
	BYTE m_ucDescriptorsLen;	//����������
	HCSHANDLE hService;			//���ڵ�Service���
}SearchPMTServiceType;

/*
void(*SearchCallBack)
	�����ص�����ԭ��

	int(*SearchCallBack)(
		CallBackType enmType,
		 void* pParam
	};	

Parameters
	
	enmType
		[out]��Ϣ����
			1)enmType=SEARCH_NIT_FREQLIST ��ʾ��ǰƵ���NIT���������
					pParamΪָ��SEARCHNITLIST��ָ��
					ָ��SEARCHNITLIST��ָ���е�m_nDeliverIndex����ǰNITƵ�������(�Դ���SearchUIʱ�����Ƶ�������˳��Ϊ׼)
					ָ��SEARCHNITLIST��ָ���е�m_nNITCount����ǰƵ��NIT���е�Ƶ�����
					ָ��SEARCHNITLIST��ָ���е�m_nNITStartIndex����ǰƵ��NIT���а����Ŀ�ʼƵ�����µ�Ƶ����е�����

			2)enmType=SEARCH_FREQ_FINISHED ��ʾ��ǰƵ��Ľ�Ŀ�������
					pParamΪָ��SEARCHFREQLIST��ָ��
					ָ��SEARCHFREQLIST��ָ���е�m_nDeliverIndex����ǰƵ�������(����������Ƶ���˳��Ϊ׼)
					ָ��SEARCHFREQLIST��ָ���е�m_nVideoCount����ǰƵ������������Video��Ŀ��
					ָ��SEARCHFREQLIST��ָ���е�m_nRadioCount����ǰƵ������������Radio��Ŀ��
					ָ��SEARCHFREQLIST��ָ���е�m_nVidoeStartIndex����ǰƵ���еĵ�һ��Video��Ŀ��Video�����е�����
					ָ��SEARCHFREQLIST��ָ���е�m_nRadioStartIndex����ǰƵ���еĵ�һ��Radio��Ŀ��Radio�����е�����

			3)enmType=SEARCH_FREQ_NITSDT ��ʾ��ǰƵ���NITSDT���ٽ�Ŀ�������
					pParamΪָ��SEARCHFREQLIST��ָ��
					ָ��SEARCHFREQLIST��ָ���е�m_nDeliverIndex����ǰƵ�������(����������Ƶ���˳��Ϊ׼)
					ָ��SEARCHFREQLIST��ָ���е�m_nVideoCount����ǰƵ������������Video��Ŀ��
					ָ��SEARCHFREQLIST��ָ���е�m_nRadioCount����ǰƵ������������Radio��Ŀ��
					ָ��SEARCHFREQLIST��ָ���е�m_nVidoeStartIndex����ǰƵ���еĵ�һ��Video��Ŀ��Video�����е�����
					ָ��SEARCHFREQLIST��ָ���е�m_nRadioStartIndex����ǰƵ���еĵ�һ��Radio��Ŀ��Radio�����е�����

					ע:�ڷ��͸���Ϣ֮ǰ���ᷢ��	SEARCH_NIT_FREQLIST ��Ϣ

			4)enmType=SEARCH_FINISHED ��ʾ�������
					pParam = NULL
					
			5)enmType=SERVICE_ISNEEDSAVE ��ʾ�Ƿ���Ҫ����ý�Ŀ��
					pParamΪָ��SERVICEINFOFORCLIENT��ָ�롣
					ָ��SERVICEINFOFORCLIENT��ָ���е�m_hServiceDataΪ��Ŀ�����ݹ����еľ��ֵ���û��ݴ˴����ݹ�����ȡ����Ӧ���ݽ����ж��Ƿ���Ҫ�洢�������Ҫ�洢�򷵻�0,���洢�򷵻ط���ֵ��
					ָ��SERVICEINFOFORCLIENT��ָ���m_pvReservedΪԤ��ָ�룬��ָ��SearchExResponseInfo��ָ�롣
					
			6)enmType = SEARCH_PMT_SERVICE_TYPE ��ʾ�û��Ƿ���Ҫ����PMT��Ϣָ��PID���Ͳ�ָ��
					pParamָ��SearchPMTServiceType
					���ط�0��ʾ�ѽ��ո�ѭ��������search�ٷ�������Ӱ�����ѭ��

			7)enmType = NIT_ISCUSTOM, ѯ�ʸ�Ƶ���Ƿ��Ƿ���Ҫ����Զ���Ƶ��
					pParamΪָ��SINetworkInfoExt��ָ�롣
					�û���NIT������ж�,������Զ����NIT��,�򷵻�0,�����뷵��1
			8)NIT_SEARCH_FINISHED,	Ƶ��������ɡ�pParamΪNULL
					�û����ڴ˻ص���Ϣ������Զ����Ƶ�㡣
	pParam
		[out]���ص�ʵ�ʲ���(����Ϣ�����������)

Return Values
		����������˵���������뷵��0
*/
typedef int(*SearchCallBack)(CallBackType enmType, void* pParam);
// }}}USR

/* {{{USR
CreateSearch
    �����һ�������Դ������Searching��ľ������������Searchģ��ĵ�һ����

	HCSHANDLE CreateSearch( void );

Parameters	
        ��

Return Values
		1������ɹ������������Դ���
		2�����ʧ�ܷ���ΪNULL

   }}}USR */
HCSHANDLE CreateSearch( void );

/* {{{USR
CloseSearch
    �ͷŵ�һ������ľ����Դ(CreateSearch�ķ��ؾ��)

	BOOL CloseSearch( HCSHANDLE hSearch );

Parameters	
        hSearch
           [in]��һ������ľ����Դ

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL CloseSearch( HCSHANDLE hSearch );

/* {{{USR
CreateNITPATPMTSearchUI
    �����ṩNITPATPMT������ʽ�ľ����Դ����������Searchģ��ĵڶ�����

	HCSHANDLE CreateNITPATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]�ɵ�һ���������������
        pDeliverList
           [in]Ƶ��������׵�ַ        
        nDeliverListCount
           [in]Ƶ��ĸ���
        callback 
           [in]�ص�����
        mTimeOut
           [in]��ʱʱ��

Return Values
		1������ɹ������������Դ���
		2�����ʧ�ܷ���ΪNULL
        
        ��hSearchΪNULL��pDeliverListΪNULL��nDeliverListCountС�ڵ�����ʱ��������ʧ�ܡ�

   }}}USR */
HCSHANDLE CreateNITPATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreatePATPMTSearchUI
    �����ṩPATPMT������ʽ�ľ����Դ����������Searchģ��ĵڶ�����

	HCSHANDLE CreatePATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]�ɵ�һ���������������
        pDeliverList
           [in]Ƶ��������׵�ַ        
        nDeliverListCount
           [in]Ƶ��ĸ���
        callback 
           [in]�ص�����
        mTimeOut
           [in]��ʱʱ��

Return Values
		1������ɹ������������Դ���
		2�����ʧ�ܷ���ΪNULL

        ��hSearchΪNULL��pDeliverListΪNULL��nDeliverListCountС�ڵ�����ʱ��������ʧ�ܡ�
   }}}USR */
HCSHANDLE CreatePATPMTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreateNITSDTSearchUI
    �����ṩNITSDT������ʽ�ľ����Դ����������Searchģ��ĵڶ�����

	HCSHANDLE CreateNITSDTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                   int nDeliverListCount, SearchCallBack callback, int mTimeOut );

Parameters	
        hSearch
           [in]�ɵ�һ���������������
        pDeliverList
           [in]Ƶ��������׵�ַ        
        nDeliverListCount
           [in]Ƶ��ĸ���
        callback 
           [in]�ص�����
        mTimeOut
           [in]��ʱʱ��

Return Values
		1������ɹ������������Դ���
		2�����ʧ�ܷ���ΪNULL

        ��hSearchΪNULL��pDeliverListΪNULL��nDeliverListCountС�ڵ�����ʱ��������ʧ�ܡ�

   }}}USR */
HCSHANDLE CreateNITSDTSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, int nDeliverListCount, SearchCallBack callback, int mTimeOut );

/* {{{USR
CreateSearchUI
    �����ṩ����������ʽ�ľ����Դ����������Searchģ��ĵڶ�����

	HCSHANDLE CreateSearchUI( HCSHANDLE hSearch, Deliver * pDeliverList, 
                int nDeliverListCount, SearchCallBack callback, int mTimeOut, SearchType enmSearchType ) ;

Parameters	
        hSearch
           [in]�ɵ�һ���������������
        pDeliverList
           [in]Ƶ��������׵�ַ        
        nDeliverListCount
           [in]Ƶ��ĸ���
        callback 
           [in]�ص�����
        mTimeOut
           [in]��ʱʱ��
        enmSearchType 
           [in]��������

Return Values
		1������ɹ������������Դ���
		2�����ʧ�ܷ���ΪNULL

        ��hSearchΪNULL��pDeliverListΪNULL��nDeliverListCountС�ڵ�����ʱ��������ʧ�ܡ�

   }}}USR */
HCSHANDLE CreateSearchUI( HCSHANDLE hSearch, HCSHANDLE * phSearchDataList, int nSearchDataCount, SearchCallBack callback, int mTimeOut, SearchType enmSearchType );

/* {{{USR
CloseSearchUI
    �ͷŵڶ�������ľ����Դ

	BOOL CloseSearchUI( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL CloseSearchUI( HCSHANDLE hSearchUI );

/* {{{USR
StartSearch
    ��ʼSearchģ�����������������Searchģ��ĵ�������

	BOOL StartSearch( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        hDataContainer
           [in]���������ľ��
        hSource
           [in]������Ƶ��Source�ľ��

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL StartSearch( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource );

/* {{{USR
StopSearch
    ��ֹ�������˳���������

	BOOL StopSearch( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL StopSearch( HCSHANDLE hSearchUI );

/* {{{USR
SkipSearch
    ������ǰ������Ƶ�㣬������һ��Ƶ��Ľ�Ŀ��Ƶ�㣨��NIT������

	BOOL SkipSearch( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL SkipSearch( HCSHANDLE hSearchUI );

/* {{{USR
GetAllVideoProgNum
    �õ���ǰ��������Video��Ŀ������

	int GetAllVideoProgNum( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
		���ص�ǰ��������Video��Ŀ����

   }}}USR */
int GetAllVideoProgNum( HCSHANDLE hSearchUI );

/* {{{USR
GetAllRadioProgNum
    �õ���ǰ������������Radio��Ŀ��������

	int GetAllRadioProgNum( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
		���ص�ǰ������������Radio��Ŀ������

   }}}USR */
int GetAllRadioProgNum( HCSHANDLE hSearchUI );

/* {{{USR
GetSearchDeliver
    �õ�����������Ƶ�㡣

	BOOL GetSearchDeliver( HCSHANDLE hSearchUI, int nIndex, Deliver * pDeliver );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nIndex
           [in]Ƶ������
        pDeliver
           [out]�û����Ƶ����Ϣ�����ݿ�ĵ�ַ
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

        ��hSearchUIΪNULL��nIndexС�����pDeliverΪNULLʱ����ȡ��ʧ�ܡ�

   }}}USR */
BOOL GetSearchDeliver( HCSHANDLE hSearchUI, int nIndex, Deliver * pDeliver );

/* {{{USR
GetSearchDeliverHandle
    �õ�����������Ƶ�������ݹ����еľ����

	BOOL GetSearchDeliverHandle( HCSHANDLE hSearchUI, int nIndex, HCSHANDLE * pDeliverHandle );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nIndex
           [in]Ƶ������
        pDeliverHandle
           [out]�û����Ƶ������ָ��
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

        ��hSearchUIΪNULL��nIndexС�����pDeliverHandleΪNULLʱ����ȡ��ʧ�ܡ�

   }}}USR */
BOOL GetSearchDeliverHandle( HCSHANDLE hSearchUI, int nIndex, HCSHANDLE * pDeliverHandle );

/* {{{USR
GetSearchState
    �õ�Ƶ���״̬��

	BOOL GetSearchState( HCSHANDLE hSearchUI, int nIndex, int * pnState );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nIndex
           [in]Ƶ������
        pnState
           [out]���״ֵ̬��ָ��
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

        ��hSearchUIΪNULL��nIndexС�����pnStateΪNULLʱ����ȡ��ʧ�ܡ�

   }}}USR */
BOOL GetSearchState( HCSHANDLE hSearchUI, int nIndex, int * pnState );

/* {{{USR
GetSearchServerDataHandle
    �õ�Service�����ݹ����еľ����

	BOOL GetSearchServerDataHandle( HCSHANDLE hSearchUI, int nIndex, int nServiceType, 
                HCSHANDLE * phServerData );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nIndex
           [in]��Ŀ�ڱ�����������е����
        nServiceType
           [in]Service���ͣ�TV or Radio��
        phServerData
           [out]���service�ľ��ָ��
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE
		
		���������Զ���洢��ʽ���Զ���洢��ο�GetSearchServiceHandleInfo().
        ��hSearchUIΪNULL��nIndexС�����phServerDataΪNULLʱ����ȡ��ʧ�ܡ�

   }}}USR */
BOOL GetSearchServerDataHandle( HCSHANDLE hSearchUI, int nIndex, int nServiceType, HCSHANDLE * phServerData );

/* {{{USR
CSUSPSearchGetVersionInfo
    ��ȡģ��汾��Ϣ�� �����汾�š�����޶����ڡ��û�����ѡ���Եص��ø÷�����ȡUSP��������ģ��İ汾��Ϣ��

	 int CSUSPSearchGetVersionInfo( char * pcVersionInfo, int nSize );

Parameters
       pcVersionInfo
       [out]��Ű汾��Ϣ�Ļ������׵�ַ���������,��Ű汾��Ϣ����ʽ���£�
            ��Module Name: EPG\nCurrent Version: 1.2\nRevision Date: 2005-5-11��
       nSize
       [in]    �������ĳ���

Return Values
		1)	�����һ������pcVersionInfoΪ�գ����ذ汾��Ϣ��ʵ�ʳ��ȡ������û�����ڶ�������Ϊ���汾��Ϣ��ʵ�ʳ���+       
1�� 
       2)	�����һ������pcVersionInfo��Ϊ�գ��Һ������óɹ������ذ汾��Ϣ��ʵ�ʳ��ȣ�
       3)	���ʧ�ܣ�����0��

       ���õ�һ�ַ���ֵ��������û��ɲ��Ըú�������Ϊ�ڶ�������,nSize,�ṩ�ο���
       ����û�����ĵڶ�������С�ڰ汾��Ϣ��ʵ�ʳ��ȣ�β��������ֽڱ��ص���
   }}}USR */
int CSUSPSearchGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSearchGetMemInfo
    ��ȡģ��Ҫ����ڴ�ռ����Сֵ�����ֵ������ģ��Ҫ��һ���̶��ռ�ʱ��*pnMin ��*pnMax��ȣ��е�ģ��������Ҫ*pnMin bytes�����������У��ڷ���*pnMax�ռ�ʱ���ɻ�ø��õ����ܡ�

	 BOOL CSUSPSearchGetMemInfo( int * pnMin, int * pnMax );

Parameters
       pnMin
       [out]ģ��������С�ڴ� (in bytes)
       pnMax
       [out]ģ����������ڴ�(in bytes)

Return Values
       1)�ɹ�������TRUE;
       2)ʧ�ܣ����� FALSE

Remarks
       1��	��ģ��Ҫ��̶��ڴ�ʱ�����ص�����ڴ����С�ڴ���ȡ���
       2��	��ģ��Ҫ����С�ڴ�Ϊ*pnMin�� ����*pnMin ��*pnMax֮��ɻ�ø��õ����ܻ���๦��ʱ������ڴ����С�ڴ治�ȡ�
       3��	��pnMin =NULL �� pnMax = NULL����������ʧ�ܡ�
       4�� �÷������ڴ�ĺ���ʹ�ã�Ӳ�������þ��вο���ֵ��    

   }}}USR */
BOOL CSUSPSearchGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
GetServiceNum
    �������͵õ���ǰ�������ĸ����͵Ľ�Ŀ������

	int GetServiceNum( HCSHANDLE hSearchUI, int nServiceType );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nServiceType
           [in]Service���ͣ�TV(0x01) or Radio(0x02) ����������

Return Values
		���ص�ǰ�������Ľ�Ŀ����
		
Remark
    ��nServiceTypeΪ0xffffffffʱ��ȡ�������ͽ�Ŀ���ܸ���


   }}}USR */
int GetServiceNum( HCSHANDLE hSearchUI, int nServiceType );

/* {{{USR
StartSearchEx
    ��ʼSearchģ�����ǿ��������������Searchģ��ĵ�������

	BOOL StartSearchEx( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource,  int nThreadStackSize, int nSearchServiceType, void *pvParam );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        hDataContainer
           [in]���������ľ��
        hSource
           [in]������Ƶ��Source�ľ��
		nThreadStackSize
		   [in]�����̶߳�ջ��С��Ĭ�ϵ�Ϊ8192*2 Bytes�ֽڴ�С����ֵ��������㣬С�ڵ����㱻���ԣ���Ĭ�ϴ�С��
		nSearchServiceType
		   [in]Ҫ���������ͣ��궨���ͷ�ļ�����ǿ�������Ķ��壬����֮����û�Ĺ�ϵ��
	    pvParam
		   [in]Ϊ�˴����ֲ�ͬ���͵Ĳ����õ�void����ָ�롣
           ��ΪSEARCH_BAT_BOUQUETID_TRANSPONDER��SEARCH_BAT_BOUQUETID_SERVICE��SEARCH_BAT_GET_TAG��SEARCH_NIT_GET_TAG��SEARCH_SDT_GET_TAG��������ʱ��pvParamΪָ��SearchExRequestInfo�ṹ���ָ�롣
		   ��ΪSEARCH_EASY_INSTALL��������ʱ��pvParamΪָ��EasyInstallRequestInfo�ṹ���ָ�롣
		   ��ΪSEARCH_SERVICE_SAVE_CUSTOM��������ʱ��pvParamָ���ָ��NULL���������κ����ݡ�

Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

   }}}USR */
BOOL StartSearchEx( HCSHANDLE hSearchUI, HCSHANDLE hDataContainer, HCSHANDLE hSource, int nThreadStackSize, int nSearchServiceType, void * pvParam );

/* {{{USR
GetSearchServiceHandleInfo
    �õ�Service�����ݹ����е���Ϣ��

	BOOL GetSearchServiceHandleInfo( HCSHANDLE hSearchUI, int nIndex, BYTE ucServiceType, SearchServiceHandleInfo * pSearchServiceHandleInfo );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ
        nIndex
           [in]��Ŀ�ڱ�����������е����
        ucServiceType
           [in]Service���ͣ�TV(0x01) or Radio(0x02)�� 
        pSearchServiceHandleInfo
           [out]���SearchServiceHandleInfo�ṹ���ָ��
Return Values
		1������ɹ�����TRUE
		2�����ʧ�ܷ���ΪFALSE

        ��hSearchUIΪNULL��nIndexС�����pSearchServiceHandleInfoΪNULLʱ����ȡ��ʧ�ܡ�

   }}}USR */
BOOL GetSearchServiceHandleInfo( HCSHANDLE hSearchUI, int nIndex, BYTE ucServiceType, SearchServiceHandleInfo * pSearchServiceHandleInfo );

/* {{{USR
CSUSPSearchGetNITVersion
    �õ�NIT�汾��

	BYTE CSUSPSearchGetNITVersion( HCSHANDLE hSearchUI );

Parameters	
        hSearchUI
           [in]�ڶ�����������������Դ

Return Values
	����������ֵΪ0xffʱ��ʾʧ�ܣ�����С�ڵ���0x1f��ֵΪ�ɹ���
NOTE
	������������⣬�䷵�صİ汾���������������NIT��İ汾�ţ�����һ���ǵ�ǰƵ�����ڵ�NIT��İ汾�š�
	�����Ҫ�õ�ÿ��Ƶ���е�NIT��İ汾�ţ���ǿ�������������SEARCH_NIT_GET_TAG��
	Ҫ�õ���ǰƵ�����ڵ������NIT��İ汾�ţ������DM�ӿ�CSUSPDMGetTPNITVersion();

   }}}USR */
BYTE CSUSPSearchGetNITVersion( HCSHANDLE hSearchUI );

/* {{{USR
GetServiceFirstBATInfo
	��ȡService������һ��Bouquet��Ϣ(һ��ServiceID�����ڶ��BouquetID,
	Ҳ������һ��bouquetID�¶����ͬ��transport_stream_id+original_network_id).

	HCSHANDLE GetServiceFirstBATInfo( HCSHANDLE hSearchUI, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

Parameters
		HCSHANDLE hSearchUI 	
			[in]�������ھ��
		WORD wServiceID     	
			[in]Service��ʶ
		BATInfo * pBatInfo  	
			[out]Service������һ��Bouquet��Ϣ
		WORD    * pwTsID       	
			[out]Service������transport_stream_id(����û�ȷ������serviceID����
		         ��ts_id�ж�Ψһ�����Բ����Ĵ��򣬴���NULL����)
        WORD    * pwOriNetID  	
        	[out]Service������original_network_dd(����û�ȷ������serviceID����
		         ��ts_id�ж�Ψһ�����Բ����Ĵ��򣬴���NULL����)	
Return Values
		NULL 	��ʾ������ϣ�δ�ҵ���ӦBouquet��Ϣ
		��NULL	��ʾ�ҵ���ӦBouquet��Ϣ������û�н���.�����������
   }}}USR */
HCSHANDLE GetServiceFirstBATInfo( HCSHANDLE hSearchUI, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

/* {{{USR
GetServiceNextBATInfo
	��ȡService������һ��Bouquet��Ϣ(һ��ServiceID�����ڶ�
	��BouquetID,Ҳ������һ��bouquetID�¶����ͬ��transport_stream_id+original_network_id).

	HCSHANDLE GetServiceNextBATInfo( HCSHANDLE hSearchUI, HCSHANDLE hBATHandle, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		HCSHANDLE hBATHandle 	
			[in]��һ�ε���GetServiceFirstBATInfo��GetServiceNextBATInfo���ص��������	
		WORD wServiceID      	
			[in]Service��ʶ
		BATInfo * pBatInfo  	
			[out]Service������һ��Bouquet��Ϣ
		WORD    * pwTsID       	
			[out]Service������transport_stream_id(����û�ȷ������serviceID����
		         ��ts_id�ж�Ψһ�����Բ����Ĵ��򣬴���NULL����)
        WORD    * pwOriNetID   	
        	[out]Service������original_network_dd(����û�ȷ������serviceID����
		         ��ts_id�ж�Ψһ�����Բ����Ĵ��򣬴���NULL����)		                    
Return Values
		NULL 	��ʾ������ϣ�δ�ҵ���ӦBouquet��Ϣ
		��NULL	��ʾ�ҵ���ӦBouquet��Ϣ������û�н���.�����������

   }}}USR */
HCSHANDLE GetServiceNextBATInfo( HCSHANDLE hSearchUI, HCSHANDLE hBATHandle, WORD wServiceID, BATInfo * pBatInfo, WORD * pwTsID, WORD * pwOriNetID );

/* {{{USR
CSUSPSearchSetPmtRequestTagParam
	��������pmt������������

	BOOL CSUSPSearchSetPmtRequestTagParam( HCSHANDLE hSearchUI, BYTE * pucTagID, DWORD dwTagCnt );


Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		BYTE * pucTagID 	
			[in]��������pmt��������tagID
		DWORD dwTagCnt      	
			[in]�������Ч��tag��
Return Values
		TRUE 	���ò����ɹ�
		FALSE	���ò���ʧ��
   }}}USR */
BOOL CSUSPSearchSetPmtRequestTagParam( HCSHANDLE hSearchUI, BYTE * pucTagID, DWORD dwTagCnt );

/* {{{USR
CSUSPSearchGetCurServicePmtFirstTagInfo
	��ISNEEDSAVE�ص��У���ȡ��ǰ��Ŀ��pmt tag����������
	WARNING: �ú�������ֻ����ISNEEDSAVE�ص��е��ò���ȷ����ȷ��

	BOOL CSUSPSearchGetCurServicePmtFirstTagInfo( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );


Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		DWORD * pdwBufAddr	
			[out]�õ�tag buf��ַ
		DWORD * pdwBufLen
			[out]�õ�tag buf�ĳ���
Return Values
		TRUE 	�õ�pmt tag����
		FALSE	δ�õ�pmt tag ����

   }}}USR */
BOOL CSUSPSearchGetCurServicePmtFirstTagInfo( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchGetCurServiceFirstElementInfo
	��SERVICE_ISNEEDSAVE�ص��е��øú�����ȡPMT��ELEMENT�������Ϣ
	HCSHANDLE CSUSPSearchGetCurServiceFirstElementInfo( HCSHANDLE hSearchUI, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		DWORD * pdwStreamType
			[out]����stream_type ֵ
		DWORD * pdwElementPid
			[out]����pidֵ
		DWORD * pdwBufAddr
			[out]������element�а����������tag buf�ĵ�ַ
        DWORD * pdwBufLen
        	[out]������element�а����������tag buf�ĳ���
Return Values
		NULL 	��ʾ�������
		��NULL	��ʾ�����ɹ���û��������ϣ������������
   }}}USR */
HCSHANDLE CSUSPSearchGetCurServiceFirstElementInfo( HCSHANDLE hSearchUI, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchGetCurServiceNextElementInfo
	��SERVICE_ISNEEDSAVE�ص��е��øú�����ȡPMT��ELEMENT�������Ϣ
	HCSHANDLE CSUSPSearchGetCurServiceNextElementInfo( HCSHANDLE hSearchUI, HCSHANDLE hPreSearch, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		HCSHANDLE hPreSearch
			[in]��һ�ε���CSUSPSearchGetCurServiceFirstElementInfo �� CSUSPSearchGetCurServiceNextElementInfo���ص��������
		DWORD * pdwStreamType
			[out]����stream_type ֵ
		DWORD * pdwElementPid
			[out]����pidֵ
		DWORD * pdwBufAddr
			[out]������element�а����������tag buf�ĵ�ַ
        DWORD * pdwBufLen
        	[out]������element�а����������tag buf�ĳ���
Return Values
		NULL 	��ʾ�������
		��NULL	��ʾ�����ɹ���û��������ϣ������������
   }}}USR */
HCSHANDLE CSUSPSearchGetCurServiceNextElementInfo( HCSHANDLE hSearchUI, HCSHANDLE hPreSearch, DWORD * pdwStreamType, DWORD * pdwElementPid, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
CSUSPSearchSetValidBouquetIDRange
	����BAT �Ϸ���bouquetID����[wMinBouquetID,wMaxBouquetID],�պ�����(����wMinBouquetID��wMaxBouquetID)
	��������øú�����searchĬ�ϵĺϷ�����Ϊ[0,0xFFFF]
	
	BOOL CSUSPSearchSetValidBouquetIDRange( HCSHANDLE hSearchUI, WORD wMinBouquetID, WORD wMaxBouquetID );

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		WORD wMinBouquetID	
			[in]��С�ĺϷ�bouquetIDֵ
		WORD wMaxBouquetID
			[in]���ֵ�ĺϷ�bouquetIDֵ
Return Values
		TRUE 	���óɹ�
		FALSE	����ʧ��
   }}}USR */
BOOL CSUSPSearchSetValidBouquetIDRange( HCSHANDLE hSearchUI, WORD wMinBouquetID, WORD wMaxBouquetID );

/* {{{USR
CSUSPSearchRequestCATTable
	����������ǰ���øú��� search���׳�CAT��ԭʼ����	
	void CSUSPSearchRequestCATTable( HCSHANDLE hSearchUI )

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
Return Values
		��
   }}}USR */
void CSUSPSearchRequestCATTable( HCSHANDLE hSearchUI );

/* {{{USR
CSUSPSearchRequestPMTTable
	����������ǰ���øú��� search��֧����SERVICE_ISNEEDSAVE�ص�����ȡPMT��ԭʼ����	
	void CSUSPSearchRequestPMTTable( HCSHANDLE hSearchUI )

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
Return Values
		��
   }}}USR */
void CSUSPSearchRequestPMTTable( HCSHANDLE hSearchUI );

/* {{{USR
CSUSPSearchGetCurServicePmtSection
	SERVICE_ISNEEDSAVE�ص��е��øú�����ȡ��ǰ��Ŀ��PMT��ԭʼ����,��Ҫ������֮ǰ����CSUSPSearchRequestPMTTable	
	BOOL CSUSPSearchGetCurServicePmtSection( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

Parameters
		HCSHANDLE hSearchUI  	
			[in]�������ھ��
		DWORD * pdwBufAddr
			[out]�����洢pmtԭʼ���ݵ�buf��ַ
		DWORD * pdwBufLen
			[out]�����洢pmtԭʼ���ݵ�buf�ĳ���
Return Values
		TURE  ��ȡ�ɹ�
		FALSE ��ȡʧ��
   }}}USR */
BOOL CSUSPSearchGetCurServicePmtSection( HCSHANDLE hSearchUI, DWORD * pdwBufAddr, DWORD * pdwBufLen );

/* {{{USR
	����BAT������BouquetID���������ڣ�������������BouquetIDѭ������������ɣ�Ĭ��Ϊ1��ѭ����
   }}}USR */
BOOL CSUSPSearchSetBouquetIDComingCnt( HCSHANDLE hSearchUI, int nMaxCnt );

/* {{{USR
	�Ƿ�ǿ������BAT����ǿ��״̬�£���ʹָ����BouquetID����Ȼ��������Bouquet
   }}}USR */
BOOL CSUSPSearchSetBatForceAutoSearch( HCSHANDLE hSearchUI, BOOL bForeceAutoSearch );

/* {{{USR
��CSUSPSearchSetBouquetIDComingCnt����չ�����������nNormalComingCnt��BouquetIDѭ����BouquetID������Ȼû�дﵽnLessBouquetIDCnt����BAT������BouquetID������������չ��nAbnormalComingCnt
   }}}USR */
BOOL CSUSPSearchSetBouquetIDComingCntAutoExtern( HCSHANDLE hSearchUI, int nNormalComingCnt, int nLessBouquetIDCnt, int nAbnormalComingCnt );

/* {{{USR
���ô˺�������SDT����ȫ����Ϊ��ȫ����PAT��������Service���ýӿ���ʱ�����ǻ���Ч
   }}}USR */
void CSUSPSearchSetSdtCompleteByPat( HCSHANDLE hSearchUI );

/* {{{USR
��ȡĳ��Ƶ�����ѵ���NIT�е�network_id�����û�ѵ�����0xffff)
   }}}USR */
BOOL GetSearchDeliverNitNetworkID( HCSHANDLE hSearchUI, int nIndex, WORD * pwNetWorkID );

/* {{{USR
��SERVICE_ISNEEDSAVE�ص��еõ���ǰ��Ŀ��EIT_Schedule_Flag��Ϣ
   }}}USR */
BOOL CSUSPSearchGetCurServiceEITScheduleFlag( HCSHANDLE hSearchUI, BYTE * pbEITScheduleFlag );

/* {{{USR
��SERVICE_ISNEEDSAVE�ص��еõ���ǰ��Ŀ��EIT_PF_Flag��Ϣ
   }}}USR */
BOOL CSUSPSearchGetCurServiceEITPFFlag( HCSHANDLE hSearchUI, BYTE * pbEITPFFlag );

/* {{{USR
��SERVICE_ISNEEDSAVE�ص��еõ���ǰ��Ŀ��ServiceID
   }}}USR */
BOOL CSUSPSearchGetCurServiceID( HCSHANDLE hSearchUI, WORD * pwServiceID );

/* {{{USR
NITPATPMT�����У�����NIT����Ϻ���Ƶ���б������Ƶ�㣬��������������ЩƵ���еĽ�Ŀ����NITPATPMT��������£����NIT����ȱ��ĳЩƵ��������ӣ������ڻص���ϢNIT_SEARCH_FINISHED�е��ô˺��������Ƶ������Ѿ�����Ҫ�����Ƶ�㣬�����ʧ�ܷ���FALSE����ӳɹ��򷵻�TRUE��

   }}}USR */
BOOL CSUSPSearchAddDeliver( HCSHANDLE hSearchUI, Deliver * pDeliver );

/* {{{USR
����չNITSDT�����У����sdt other��ķ��Ͳ����򣬿��Ե���CSUSPSearchSetSdtOtherComingCnt������ͬһ��sdtother�������ظ���������������ĳ��sdt other������nMaxCnt+1�κ���Ϊ������ϡ�Ĭ����1��
   }}}USR */
BOOL CSUSPSearchSetSdtOtherComingCnt( HCSHANDLE hSearchUI, int nMaxCnt );

/*����Ҫ����ʱ���õ�Tuner��Demux�豸�ľ��*/
BOOL CSUSPSearchSetTunerDemuxPath(HCSHANDLE hSearchUI, HCSHANDLE hRCPath);

//����searchģ�鱣�������Ƶ�����Ƿ�֧��mediaplayerֱ��ʹ�ã�Ĭ��ΪFALSE
//FALSE,����cs_sysav.h�ж���洢����Ƶ���ͣ�����Ƶ����Ϊ2��bit������֮ǰSM��Ҫ���.
//TRUE, ����cs_sysaudio.h��cs_sysvidio.h�ж���洢����Ƶ���ͣ�����Ƶ����Ϊ8��bit������mediaplayerҪ��ġ�
void CSUSPSearchSetSupportMPlayer( BOOL bSupportMplayer);

/**
@brief ���ý�Ŀ����������������ͱ�־λ�Ĵ������ͣ���ʼ����֮ǰ����

@param[in] hSearchUI  �������ھ��
@param[in] bIsGetOriginal  �Ƿ񱣴�ԭʼ������ͱ�־λ
TRUE:����ԭʼ������ͱ�־λ
FALSE:ΪUSP_CODING_UNICODE��USP_CODING_ASC��Ĭ��Ϊ����״̬
@return �ɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSUSPSearchSetServiceNameCCTableIndexFlag(HCSHANDLE hSearchUI, BOOL bIsGetOriginal);

/**
@brief �ֱ���������������ĳ�ʱʱ�䣬�ڿ�ʼ����ǰ������Ч��������������Դ�������ʱ
���õ��ܵĳ�ʱʱ��Ϊ׼

@param[in] hSearchUI  �������ھ��
@param[in] emTableType �����������
@param[in] nTimeout ������ĳ�ʱʱ��
@return ���óɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSUSPSearchSetSearchTableTimeout(HCSHANDLE hSearchUI, SEARCHTABLETYPE emTableType, int nTimeout);

/**
@brief ���õ����ڶ��NIT�ӱ�ʱ�Ƿ���Ҫ�������NIT�ӱ�

@param[in] TRUE��ʾ��Ҫ���ڶ��NIT�ӱ�ʱ��Ҫ��һ����������FALSEʱ��ʾ����Ҫ����
@return ��
*/
void CSUSPSearchNeedSearchMultiNIT(BOOL bIsNeedSearchMultiNIT);

/**
@brief ��ȡ��ǰ������nit�汾��

@param[in]��
@return ��ǰƵ���������nit�汾��
*/
BYTE CSUSPSearchGetNitVersion();

/**
@brief ��ȡ��ǰ������BAT �汾��

@param[in]��
@return ��ǰƵ���������BAT �汾��
*/
BYTE CSUSPSearchGetBatVersion();

/**
@brief ��ʾpat��tsid��Ч����sdt��tsidΪ׼
@param[in]hSearchUI  �������ھ��
@return ���óɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSUSPSearchSetPATTsidInvalid(HCSHANDLE hSearchUI); 
// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* SearchUI_H */
