/*
*
*
*
*
*/

#ifndef _CSSI_CACHE_H_
#define _CSSI_CACHE_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "mmcp_typedef.h"
#include "SICommonType.h"

enum 
{
	ERR_SIC_NO_ERROR         = 0,
	ERR_SIC_NO_MEMORY = 1,
	ERR_SIC_LOAD_FAILED         = 2,
	ERR_SIC_NO_PROGRAM          = 3,
	ERR_SIC_NO_ES               = 4,
	ERR_SIC_NO_NETWORK          = 5,
	ERR_SIC_NO_TS               = 6,
	ERR_SIC_NO_SERVICE          = 7,
	ERR_SIC_NO_DESCRIPTOR       = 8,
	ERR_SIC_CREATE_DAEMON_FAILED= 9,
	ERR_SIC_MONITOR_FAILED      = 10,
	ERR_SIC_UNKNOWN_ERROR       = 11,
	ERR_SIC_NO_AIT              = 12,
	ERR_SIC_INVALID_LOADID              = 13,
	ERR_SIC_NO_BOUQUET              = 14,
	ERR_SIC_NOT_SUPPORT              = 15,
	ERR_SIC_CBID_NOT_EXIST              = 16,
	ERR_SIC_REQ_TABLE_ERR              = 17,
	ERR_SIC_NO_LOADID              = 18,
	ERR_SIC_NO_CAINFO	= 19
};

/* stream type */
enum _CSSICStreamType 
{
	STREAM_VIDEO       =  0x02,
	STREAM_AUDIO    =    0x03,
	STREAM_DATA_CAROUSEL   = 0x06,
	STREAM_OBJECT_CAROUSEL     =   0x07,
	STREAM_DATA_BROADCAST  = 0x0B,
	STREAM_PRIVATE_DATA  = 0x80
};

/*Message Type definition */
enum _CSSICMsgType 
{
	SI_MSG_TALBE_LOADED = 0x00,  /*��װ�سɹ�*/
	SI_MSG_LOAD_TIMEOUT = 0x01,  /*��װ�س�ʱ*/
	SI_MSG_TALBE_CHANGED = 0x02  /*�����仯*/
};

/*pid definition*/
enum _CSSICPID
{
	SIC_PAT_PID = 0x00,
    SIC_CAT_PID = 0x01,
    SIC_SDT_PID = 0x11,
    SIC_BAT_PID = 0x11,
    SIC_NIT_PID = 0x10
};

/*Table Id Definition 
SI Informationֻ֧���������ͱ��Cache��descriptor����*/
enum _CSSICTableId 
{
    SIC_PAT = 0x00,
    SIC_CAT = 0x01,
    SIC_PMT = 0x02,
    SIC_SDT = 0x42,
    SIC_AIT = 0x74,
    SIC_BAT = 0x4A,
    SIC_NIT = 0x40,
    SIC_TSDT = 0x03
  };

#define SIC_MAX_PAT_COUNT 16

typedef struct _CSSICDescriptor_t
{
	BYTE  m_ucTag;
	BYTE  m_ucLength;
	BYTE  m_pucData[256];
} CSSICDescriptor_t;


 typedef struct _CSSICAppInfo_t
{
	DWORD m_dwOrgId;
	WORD m_wAppId;
	BYTE  m_ucCtrlCode;  
	BYTE  m_ucDescCount; /* m_ppsDescָ������ĳ���*/ 
	CSSICDescriptor_t **  m_ppsDescList; /*ָ��CSSICDescriptorָ�������*/
} CSSICAppInfo_t;

typedef struct _CSSICAIT_t
{
	WORD m_wAppType;/**/
	WORD m_wPid;/*ES pid which carry app data*/
	BYTE m_ucAitVer;
	BYTE m_ucDescCount;/* m_ppsDescList ָ������ĳ���*/
	CSSICDescriptor_t **m_ppsDescList;/*ָ��CSSICDescriptor��ָ������*/
	BYTE m_ucAppCount;/* m_ppsAppList ָ������ĳ���*/
	CSSICAppInfo_t **m_ppsAppList;/*ָ��CSSICAppInfo_tָ�������*/
} CSSICAIT_t; 

typedef struct _CSSICES_t
{
	BYTE  m_ucStreamType;
	WORD m_wElementaryPid;
	BYTE m_ucDescCount;/* m_ppsDescList�����鳤��*/
	CSSICDescriptor_t  **m_ppsDescList;/*CSSICDescriptor_t���͵�ָ������*/
} CSSICES_t;

typedef struct _CSSICProgram_t
{
	WORD m_wProgNum;
	WORD m_wPmtPid;
	WORD m_wPcrPid;
	BYTE  m_ucVerNum;
	BYTE m_ucDescCount; /* m_ ppsDescList�����鳤��*/
	CSSICDescriptor_t ** m_ppsDescList; /*ָ��CSSICDescriptor_t �ṹ��ָ�������*/
	BYTE m_ucAitCount; /* m_ppsAitList�����鳤��*/
	CSSICAIT_t **m_ppsAitList;/*ָ��CSSICAIT_t �ṹ��ָ�������*/     
	BYTE m_ucEsCount; /* m_ppsEsList�����鳤��*/
	CSSICES_t **m_ppsEsList; /*ָ��CSSICES_t�ṹ��ָ�������*/
} CSSICProgram_t;

typedef struct _CSSICCAT_t 
{
	BYTE m_ucVerNum;
	BYTE m_ucDescCount;/* m_ppsDescList�����鳤��*/
	CSSICDescriptor_t  **m_ppsDescList;/*CSSICDescriptor_t���͵�ָ������*/
} CSSICCAT_t;

typedef struct _CSSICService_t
{
	WORD m_wServiceId;
	BYTE m_ucActual;
	BYTE m_ucServiceType;
	BYTE  m_ucEitScheduleFlag;
	BYTE  m_ucEitPfFlag;
	BYTE  m_ucRunningStatus;
	BYTE  m_ucFreeCaMode;
	BYTE  m_ucDescCount;/* m_ppsDescListָ������ĳ���*/
	CSSICDescriptor_t **m_ppsDescList;/* CSSICDescriptor_tָ������*/
} CSSICService_t;

typedef struct _CSSICPAT_t 
{
	WORD m_wTsId;
	WORD m_wNetPid;
	BYTE m_ucVerNum;/*for Program*/
	BYTE m_ucSdtVerNum;/*for service*/
	BYTE  m_ucProgCount;/* m_ppsProgList ��ָ�����鳤��*/
	CSSICProgram_t **m_ppsProgList; /* From PMT ��CSSICProgram_t ���͵�ָ������*/
	BYTE m_ucServCount;/* m_ ppsServList��ָ�����鳤��*/
	CSSICService_t **m_ppsServList; /* From SDT : CSSICService_t���͵�ָ������*/
} CSSICPAT_t;

typedef struct _CSSICTS_t
{
	WORD m_wTsId;
	WORD m_wOrgNetId;
	WORD m_wNetId;
	BYTE m_ucDescCount;/* m_ppsDescLisָ�����鳤��*/
	CSSICDescriptor_t ** m_ppsDescList;/* CSSICDescriptor_tָ������*/
} CSSICTS_t;

typedef struct _CSSICNetwork_t
{
	BYTE m_ucTableId;/*�������û����ݴ����ж���actual or other*/
	WORD m_wNetId;
	BYTE m_ucVerNum;
	BYTE  m_ucDescCount; /*m_ppsDescListָ�����鳤��*/
	CSSICDescriptor_t  ** m_ppsDescList; /* CSSICDescriptor_tָ������*/
	BYTE  m_ucTsCount;/* m_ucTsCountָ�����鳤��*/
	CSSICTS_t **m_ppsTsList;/* CSSICTS_t ָ������*/
} CSSICNetwork_t;

typedef struct _CSSICBouque_t 
{
	WORD m_wBouquetId;
	BYTE m_pucBouquetName[64];
	BYTE m_ucVerNum;
	BYTE m_ucDescCount;/* m_ppsDescListָ�����鳤��*/
	CSSICDescriptor_t ** m_ppsDescList;/*CSSICDescriptor_tָ������*/
	BYTE m_ucTsCount;/* m_ppsTsListָ�����鳤��*/
	CSSICTS_t **m_ppsTsList;/* CSSICTS_tָ������*/
} CSSICBouque_t;

/*�����ݽṹ*/
#define CSSIC_MAX_SEC_IN_TABLE  (256)
typedef struct  _SIC_DloadData
{
	DWORD   m_dwLoadId;
	WORD	m_wPid;        /* PIDֵ */
	BYTE	m_ucTableId;
	WORD	m_wTableIdExt;
	BYTE	m_ucSecCount;	/* number of sections head dir in m_dwDataAddrs */
       WORD	m_wSectionLen[CSSIC_MAX_SEC_IN_TABLE];	/* Sections length where to store the length of each sections */
       DWORD	m_dwDataAddrs[CSSIC_MAX_SEC_IN_TABLE];	/* Sections memory address where to store the sections */
}CSSICTableData_t;

/*֪ͨ�û�����*/
typedef int (*CSSICCallback) (
	DWORD dwDeviceId,
	int  nMsg, /*ֵΪenum����_CSSICMsgType */
	void * pUserData,/*Load Talbe or Monitor Tableʱ������û�����*/
	CSSICTableData_t *pTableData
);/* �û�����Ļص�����*/


/*�û����ú���*/
typedef void (*CSSICUserCall) (
	DWORD dwDeviceId,
	int  nAddrType, /* ָ��pAddress ������*/
	void* pAddress,
	int nCount
);/* SI cache����Ļص�����*/

/*���ڱ���get��Ϣʱ���صĸ�����������Ϣ��*/
typedef struct _CSSICRetInfo_t{
	int m_nNumber;/*����������Ԫ�صĸ���*/
	int m_nErrCode;/*����ִ�н���Ĵ������*/
	int m_nAddrType;/*���ص�ָ�������*/
	CSSICUserCall m_FreeRetAddr;/*���ص�ָ����ͷź���,�����ΪNULLʱ����ʹ����ָ�������ص��ô˺���*/
}CSSICRetInfo_t;

typedef struct _CSSICReqTableInfo_t{
	WORD m_wNetId;/*��m_wPid==0xFFFFʱ��������*/
	WORD m_wTsId;/*��m_wPid==0xFFFFʱ��������*/
	WORD m_wPid;/*Ϊ0xFFFFʱ������Pid*/
	BYTE m_ucTableId;
	WORD  m_wTableIdExt;/*Ϊ0xFFFFʱ�����Ĵ���*/
	DWORD m_dwTimeOutMs;
	void*	m_pUserData;
	CSSICCallback m_pCallback;/*֪ͨ�����ߵĻص�����������ΪNULL*/
	BYTE m_ucVersion; //��ʾ������ı�İ汾�ţ����ʱ���á�С��0x20ʱ��Ч�������Ի���ı�İ汾��Ϊ��׼��
}CSSICReqTableInfo_t;

#define CSSIC_PARAM_IGNORE  0xFFFF
#define in
#define out
/*API definations*/
int CSSICInit(void);

/**
@brief CSSICSetPATCacheCount ������Ҫ�����PAT�ĸ���

@param[in]  dwCount  ��Ҫ�����PAT�ĸ���
@return TRUE���óɹ�������ʧ��
@note ����CSSICInit֮ǰ���ô˺���
*/
BOOL CSSICSetPATCacheCount(DWORD dwCount);

/**
@brief CSSICSetPATCacheLevel ����PAT���ݵĻ��漶��(�Ƿ�Ϊ�����ȼ�)��
�����Ѿ������PAT�����ﵽ�������ʱ
�����ȼ��Ĵ�������ʶΪwTsId��PAT���鲻�ᱻ����滻��

@param[in]  wTsId   PAT�Ĵ�������ʶ
@param[in]  bHighLevel �Ƿ�Ϊ�����ȼ�
@return   TRUE���óɹ�������ʧ��
@note 1�������ȼ���TsId�ĸ���ҪС��PAT����������
          2����û�����ã���Ĭ��Ϊ�����ȼ���
          3������CSSICSetPATCacheCount()֮����ã�
          4�����������ж��Ƶ���TsId��ͬ�����޷���֤���Ƶ�����ݵ�
		�����ȼ������յ������ݻὫ�����ݸ��ǡ�
*/
BOOL CSSICSetPATCacheLevel(WORD wTsId, BOOL bHighLevel);

DWORD CSSICRegisterCallback(in CSSICCallback pCallback,in DWORD dwTableId,in void* pUserData);

int CSSICUnregisterCallback(in DWORD dwCallbackId);

int CSSICChangeCallback(in DWORD dwCallbackId, in CSSICCallback pCallback,in DWORD dwTableId,in void* pUserData);

DWORD  CSSICLoadTable (in DWORD dwDeviceId, 
							in CSSICReqTableInfo_t *pReqTable,
							out int* pnErrCode);


 /**
@brief 
CSSICLoadTableByFreq()������ָ��Ƶ��ָ�������ı����ݣ�ֻ�������PAT������

@param[in] dwDeviceId	����Դ���豸ID��
@param[in] pReqTable	���װ�ز���
@param[out] dwFreq  Ƶ������ʡ�ֻ�е�����PAT��ʱ�ò�������Ч
@return  ��0��װ��ID��
@note 
 */
DWORD CSSICLoadTableByFreq(in DWORD dwDeviceId,
							in CSSICReqTableInfo_t *pReqTable,
							in DWORD dwFreq,
							out int* pnErrCode);

int CSSICCancelLoad (in DWORD dwLoadId);

DWORD CSSICMonitorTable (in DWORD dwDeviceId, 
								in CSSICReqTableInfo_t *pReqTable,
								out int* pnErrCode);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICNetwork_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICNetwork_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetNetwork(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICNetwork_t* CSSICGetNetwork (in DWORD dwDeviceId, 
									in WORD wNetId,
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICNetwork_t*���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICNetwork_t*���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllNetwork(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICNetwork_t** CSSICGetAllNetwork (in DWORD dwDeviceId, 
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICTS_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICTS_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetTS(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICTS_t* CSSICGetTS (in DWORD dwDeviceId,
						in WORD wOrgNetId, 
						in WORD wTsId,
						out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICTS_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICTS_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllTS(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICTS_t ** CSSICGetAllTS (in DWORD dwDeviceId, 
							in WORD wOrgNetId, 
							out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICService_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICService_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetService(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICService_t * CSSICGetService (in DWORD dwDeviceId, 
									in WORD wOrgNetId, 
									in WORD wTsId, 
									in WORD wServiceId,
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICService_t *���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICService_t *���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllService(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICService_t ** CSSICGetAllService (in DWORD dwDeviceId, 
										in WORD wOrgNetId, 
										in WORD wTsId, 
										out CSSICRetInfo_t *psRet);

/*
 * Hongtao LIAO (2007/11/16):ProgramNum ? what this is ? It should be ServiceID
 */
/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICProgram_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICProgram_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetProgram(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICProgram_t* CSSICGetProgram (in DWORD dwDeviceId,
									in WORD wOrgNetId, 
									in WORD wTsId, 
									in WORD wProgNum,
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICProgram_t*���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICProgram_t*���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllProgram(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICProgram_t ** CSSICGetAllProgram (in DWORD dwDeviceId, 
										in WORD wOrgNetId, 
										in WORD wTsId,
										out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICES_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICES_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetES(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICES_t * CSSICGetES (in DWORD dwDeviceId,
							in WORD wOrgNetId, 
							in WORD wTsId, 
							in WORD wProgNum,
							in WORD wPid,
							in BYTE* pucLangCode,
							out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICES_t *���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICES_t *���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllES(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICES_t ** CSSICGetAllES (in DWORD dwDeviceId, 
							in WORD wOrgNetId, 
							in WORD wTsId, 
							in WORD wProgNum, 
							out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICBouque_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICBouque_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetBouquet(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICBouque_t* CSSICGetBouquet (in DWORD dwDeviceId, 
									in WORD wBouquetId,
									out CSSICRetInfo_t *psRet);
/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICBouque_t*���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICBouque_t*���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAllBouquets(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICBouque_t** CSSICGetAllBouquets (in DWORD dwDeviceId,
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICCAT_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICCAT_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetCAInfo(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICCAT_t* CSSICGetCAInfo (in DWORD dwDeviceId, 
								in WORD wCaSystemId,
								out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICAIT_t*���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICAIT_t*���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAITInfos(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICAIT_t** CSSICGetAITInfos (in DWORD dwDeviceId, 
								in WORD wOrgNetId, 
								in WORD wTsId, 
								in WORD wProgNum, 
								in WORD wAppType, 
								out CSSICRetInfo_t *psRet);


/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICAIT_t*���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICAIT_t*���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAITInfosByPID(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICAIT_t** CSSICGetAITInfosByPID (in DWORD dwDeviceId, 
			       in WORD wOrgNetId, 
			       in WORD wTsId, 
			       in WORD wProgNum,
			       in WORD  wAitPid ,    
			       in WORD wAppType, 
			       out CSSICRetInfo_t *psRet);



/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICAppInfo_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICAppInfo_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetAppInfo(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICAppInfo_t* CSSICGetAppInfo (in DWORD dwDeviceId, 
									in WORD wOrgNetId, 
									in WORD wTsId, 
									in WORD wProgNum, 
									in WORD wAppId, 
									out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICPAT_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICPAT_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetPat(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICPAT_t *CSSICGetPat(in DWORD dwDeviceId,
							in WORD wTsId,
							out CSSICRetInfo_t *psRet);

/**
@brief 

@param[in]
@param[in] 
@return ����ɹ�����CSSICES_t���͵�ָ�룬��ָ��ָ��Ŀռ���sicache�ڲ����䣬��Ҫʹ���ߵ����ͷź����ͷ�;���ʧ�ܷ���ΪNULL
@note ��ʹ�����ͨ���˺���ȡ�õ�CSSICES_t���͵�ָ�������ص���
psRet�е��Ƿ���������:

pResult = CSSICGetESByComTag(...);
...
if (psRet->m_FreeRetAddr != NULL)
{
	psRet->m_FreeRetAddr(dev, psRet->m_nAddrType, pResult, psRet->m_nNumber);
}

*/
CSSICES_t * CSSICGetESByComTag (in DWORD dwDeviceId,
							in WORD wOrgNetId, 
							in WORD wTsId, 
							in WORD wProgNum,
							in WORD wComTag,
							out CSSICRetInfo_t *psRet);

void CSSICSetPidOffset(int nPidOffset, int nNewTableId, int nTableId );

/**
@brief �ڻ�ȡ���ݵ�ʱ�򣬲��Ƚ�pat���tsid�ʹ����tsid��ֱ�ӻ�ȡ
@return 
*/
void CSSICSetPATTsidInvalid(void);



/**
@brief �жϵ�ǰƵ���PAT���Ƿ��Ѿ��ڻ����д���
@param[in] dwFreq Ƶ��Ƶ�� 
@return ���ڷ���TRUE,�����ڷ���FALSE
*/
BOOL CSSICCheckPATExist(DWORD dwFreq);

#ifdef __cplusplus
}
#endif
#endif
