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
	SI_MSG_TALBE_LOADED = 0x00,  /*表装载成功*/
	SI_MSG_LOAD_TIMEOUT = 0x01,  /*表装载超时*/
	SI_MSG_TALBE_CHANGED = 0x02  /*表发生变化*/
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
SI Information只支持以下类型表的Cache和descriptor解析*/
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
	BYTE  m_ucDescCount; /* m_ppsDesc指针数组的长度*/ 
	CSSICDescriptor_t **  m_ppsDescList; /*指向CSSICDescriptor指针的数组*/
} CSSICAppInfo_t;

typedef struct _CSSICAIT_t
{
	WORD m_wAppType;/**/
	WORD m_wPid;/*ES pid which carry app data*/
	BYTE m_ucAitVer;
	BYTE m_ucDescCount;/* m_ppsDescList 指针数组的长度*/
	CSSICDescriptor_t **m_ppsDescList;/*指向CSSICDescriptor的指针数组*/
	BYTE m_ucAppCount;/* m_ppsAppList 指针数组的长度*/
	CSSICAppInfo_t **m_ppsAppList;/*指向CSSICAppInfo_t指针的数组*/
} CSSICAIT_t; 

typedef struct _CSSICES_t
{
	BYTE  m_ucStreamType;
	WORD m_wElementaryPid;
	BYTE m_ucDescCount;/* m_ppsDescList的数组长度*/
	CSSICDescriptor_t  **m_ppsDescList;/*CSSICDescriptor_t类型的指针数组*/
} CSSICES_t;

typedef struct _CSSICProgram_t
{
	WORD m_wProgNum;
	WORD m_wPmtPid;
	WORD m_wPcrPid;
	BYTE  m_ucVerNum;
	BYTE m_ucDescCount; /* m_ ppsDescList的数组长度*/
	CSSICDescriptor_t ** m_ppsDescList; /*指向CSSICDescriptor_t 结构的指针的数组*/
	BYTE m_ucAitCount; /* m_ppsAitList的数组长度*/
	CSSICAIT_t **m_ppsAitList;/*指向CSSICAIT_t 结构的指针的数组*/     
	BYTE m_ucEsCount; /* m_ppsEsList的数组长度*/
	CSSICES_t **m_ppsEsList; /*指向CSSICES_t结构的指针的数组*/
} CSSICProgram_t;

typedef struct _CSSICCAT_t 
{
	BYTE m_ucVerNum;
	BYTE m_ucDescCount;/* m_ppsDescList的数组长度*/
	CSSICDescriptor_t  **m_ppsDescList;/*CSSICDescriptor_t类型的指针数组*/
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
	BYTE  m_ucDescCount;/* m_ppsDescList指针数组的长度*/
	CSSICDescriptor_t **m_ppsDescList;/* CSSICDescriptor_t指针数组*/
} CSSICService_t;

typedef struct _CSSICPAT_t 
{
	WORD m_wTsId;
	WORD m_wNetPid;
	BYTE m_ucVerNum;/*for Program*/
	BYTE m_ucSdtVerNum;/*for service*/
	BYTE  m_ucProgCount;/* m_ppsProgList 的指针数组长度*/
	CSSICProgram_t **m_ppsProgList; /* From PMT ：CSSICProgram_t 类型的指针数组*/
	BYTE m_ucServCount;/* m_ ppsServList的指针数组长度*/
	CSSICService_t **m_ppsServList; /* From SDT : CSSICService_t类型的指针数组*/
} CSSICPAT_t;

typedef struct _CSSICTS_t
{
	WORD m_wTsId;
	WORD m_wOrgNetId;
	WORD m_wNetId;
	BYTE m_ucDescCount;/* m_ppsDescLis指针数组长度*/
	CSSICDescriptor_t ** m_ppsDescList;/* CSSICDescriptor_t指针数组*/
} CSSICTS_t;

typedef struct _CSSICNetwork_t
{
	BYTE m_ucTableId;/*用来让用户根据此项判断是actual or other*/
	WORD m_wNetId;
	BYTE m_ucVerNum;
	BYTE  m_ucDescCount; /*m_ppsDescList指针数组长度*/
	CSSICDescriptor_t  ** m_ppsDescList; /* CSSICDescriptor_t指针数组*/
	BYTE  m_ucTsCount;/* m_ucTsCount指针数组长度*/
	CSSICTS_t **m_ppsTsList;/* CSSICTS_t 指针数组*/
} CSSICNetwork_t;

typedef struct _CSSICBouque_t 
{
	WORD m_wBouquetId;
	BYTE m_pucBouquetName[64];
	BYTE m_ucVerNum;
	BYTE m_ucDescCount;/* m_ppsDescList指针数组长度*/
	CSSICDescriptor_t ** m_ppsDescList;/*CSSICDescriptor_t指针数组*/
	BYTE m_ucTsCount;/* m_ppsTsList指针数组长度*/
	CSSICTS_t **m_ppsTsList;/* CSSICTS_t指针数组*/
} CSSICBouque_t;

/*表数据结构*/
#define CSSIC_MAX_SEC_IN_TABLE  (256)
typedef struct  _SIC_DloadData
{
	DWORD   m_dwLoadId;
	WORD	m_wPid;        /* PID值 */
	BYTE	m_ucTableId;
	WORD	m_wTableIdExt;
	BYTE	m_ucSecCount;	/* number of sections head dir in m_dwDataAddrs */
       WORD	m_wSectionLen[CSSIC_MAX_SEC_IN_TABLE];	/* Sections length where to store the length of each sections */
       DWORD	m_dwDataAddrs[CSSIC_MAX_SEC_IN_TABLE];	/* Sections memory address where to store the sections */
}CSSICTableData_t;

/*通知用户函数*/
typedef int (*CSSICCallback) (
	DWORD dwDeviceId,
	int  nMsg, /*值为enum类型_CSSICMsgType */
	void * pUserData,/*Load Talbe or Monitor Table时传入的用户数据*/
	CSSICTableData_t *pTableData
);/* 用户定义的回调函数*/


/*用户调用函数*/
typedef void (*CSSICUserCall) (
	DWORD dwDeviceId,
	int  nAddrType, /* 指明pAddress 的类型*/
	void* pAddress,
	int nCount
);/* SI cache定义的回调函数*/

/*用于保存get信息时返回的个数及错误信息等*/
typedef struct _CSSICRetInfo_t{
	int m_nNumber;/*符合条件的元素的个数*/
	int m_nErrCode;/*函数执行结果的错误代码*/
	int m_nAddrType;/*返回的指针的类型*/
	CSSICUserCall m_FreeRetAddr;/*返回的指针的释放函数,当此项不为NULL时，在使用完指针后请务必调用此函数*/
}CSSICRetInfo_t;

typedef struct _CSSICReqTableInfo_t{
	WORD m_wNetId;/*当m_wPid==0xFFFF时才有意义*/
	WORD m_wTsId;/*当m_wPid==0xFFFF时才有意义*/
	WORD m_wPid;/*为0xFFFF时不关心Pid*/
	BYTE m_ucTableId;
	WORD  m_wTableIdExt;/*为0xFFFF时不关心此项*/
	DWORD m_dwTimeOutMs;
	void*	m_pUserData;
	CSSICCallback m_pCallback;/*通知调用者的回调函数，可以为NULL*/
	BYTE m_ucVersion; //表示所请求的表的版本号，监控时有用。小于0x20时生效，否则以缓存的表的版本号为基准。
}CSSICReqTableInfo_t;

#define CSSIC_PARAM_IGNORE  0xFFFF
#define in
#define out
/*API definations*/
int CSSICInit(void);

/**
@brief CSSICSetPATCacheCount 设置需要缓存的PAT的个数

@param[in]  dwCount  需要缓存的PAT的个数
@return TRUE设置成功，否则失败
@note 请在CSSICInit之前调用此函数
*/
BOOL CSSICSetPATCacheCount(DWORD dwCount);

/**
@brief CSSICSetPATCacheLevel 设置PAT数据的缓存级别(是否为高优先级)，
即当已经缓存的PAT个数达到最大数量时
高优先级的传输流标识为wTsId的PAT数组不会被清除替换。

@param[in]  wTsId   PAT的传输流标识
@param[in]  bHighLevel 是否为高优先级
@return   TRUE设置成功，否则失败
@note 1、高优先级的TsId的个数要小于PAT的最大个数；
          2、若没有设置，则默认为低优先级；
          3、请在CSSICSetPATCacheCount()之后调用；
          4、若网络中有多个频点的TsId相同，则无法保证相关频点数据的
		高优先级，新收到的数据会将旧数据覆盖。
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
CSSICLoadTableByFreq()，请求指定频点指定参数的表数据，只针对请求PAT表的情况

@param[in] dwDeviceId	数据源的设备ID号
@param[in] pReqTable	表的装载参数
@param[out] dwFreq  频点符号率。只有当请求PAT表时该参数才有效
@return  非0的装载ID号
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
@return 如果成功返回CSSICNetwork_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICNetwork_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICNetwork_t*类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICNetwork_t*类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICTS_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICTS_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICTS_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICTS_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICService_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICService_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICService_t *类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICService_t *类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICProgram_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICProgram_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICProgram_t*类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICProgram_t*类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICES_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICES_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICES_t *类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICES_t *类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICBouque_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICBouque_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICBouque_t*类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICBouque_t*类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICCAT_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICCAT_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICAIT_t*类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICAIT_t*类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICAIT_t*类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICAIT_t*类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICAppInfo_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICAppInfo_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICPAT_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICPAT_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@return 如果成功返回CSSICES_t类型的指针，此指针指向的空间由sicache内部分配，需要使用者调用释放函数释放;如果失败返回为NULL
@note 在使用完毕通过此函数取得的CSSICES_t类型的指针后请务必调用
psRet中的是否函数，例如:

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
@brief 在获取数据的时候，不比较pat表的tsid和传入的tsid，直接获取
@return 
*/
void CSSICSetPATTsidInvalid(void);



/**
@brief 判断当前频点的PAT表是否已经在缓存中存在
@param[in] dwFreq 频点频率 
@return 存在返回TRUE,不存在返回FALSE
*/
BOOL CSSICCheckPATExist(DWORD dwFreq);

#ifdef __cplusplus
}
#endif
#endif
