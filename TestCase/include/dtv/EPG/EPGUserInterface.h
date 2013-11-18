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

/*EPG事件的语言种类*/
typedef enum 
{
	EM_EPG_ALL,		//不考虑语言种类,表示所有
	EM_EPG_ZHO,		//中文
	EM_EPG_ENG		//英文
}eEPGLangType;

typedef struct _EPGINFO
{

	WORD m_wEPGINFOSize;         //此结构体尺寸

	BYTE m_ucLanguage;           //编码方式

	BYTE m_ucParental;           //节目成人级别
	
	BYTE m_pucBaseName[ EPG_MAX_EVENT_NAME_LEN ];       //节目名称

	HCSHANDLE m_pucUserData;                            //用户私有数据（暂无用）
	CSUDIPLUSOSSysTime_S m_StartTSYSTEMTIME;                     //节目开始时间
	CSUDIPLUSOSSysTime_S m_EndTSYSTEMTIME;                       //节目结束时间
	CSUDIPLUSOSTIME_S m_DurationTIME;                               //节目持续时间
 
	BYTE m_ucContentLevel1;                             //节目分类1
	BYTE m_ucContentLevel2;                             //节目分类2

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

	BYTE m_pucDescription[ EPG_MAX_INFO_EXT_DESCRIPTION_LENGTH ];      //节目简介

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

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //短节目简介

	int m_nExtDesLen;              //节目简介长度
	BYTE * m_pucDescription;      //节目简介

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

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //短节目简介

	int m_nExtDesLen;              //节目简介长度
	BYTE * m_pucDescription;      //节目简介

	BYTE m_pucISO639LanguageCode[3];
	BYTE m_bRunningStatus;
	BYTE m_bFreeCAMode;

	int m_nDescriptorCount;
	PBYTE m_DescriptorArray[EPG_MAX_DESCRIPTOR_COUNT];

	int m_nExtItemsLen;		//Item数据总长度，用户用来保存Item数据的buffer大小
	BYTE* m_pucExtItems;	//Item所有数据的buffer，由用户在获取时分配

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

	BYTE m_pucShortDescription[ EPG_MAX_INFO_DESCRIPTION_LENGTH ]; //短节目简介

	BYTE m_pucDescription[EPG_MAX_INFO_DESCRIPTION_LENGTH];      //节目简介

	BYTE m_pucISO639LanguageCode[3];
	BYTE m_bRunningStatus;
	BYTE m_bFreeCAMode;

	int m_nDescriptorCount;
	PBYTE m_DescriptorArray[EPG_MAX_DESCRIPTOR_COUNT];

	int m_nExtItemsLen;		//Item数据总长度，用户用来保存Item数据的buffer大小
	BYTE* m_pucExtItems;	//Item所有数据的buffer，由用户在获取时分配
	WORD m_wEventId;

}EPGEXT3INFO,*PEPGEXT3INFO;

/*
GetEPGFindExtItemDesbyIndex
 获取指定Network指定TS流指定Service指定Event指定索引的ItemDescription 

Parameters：
  pucExtItems
        [in]Items的buffer,由EPGEXT2INFO的m_pucExtItems获得
  nExtItemsLen
        [in]Items的buffer大小
  nIndex
        [in]要获取的ItemDescription索引，从0开始
  pucItemDes 
        [out]获取到的ItemDescription数据，其中第一个字节为item_description_length，后面为item_description原始数据
  nItemDesLen 
        [int]用来获取ItemDescription数据buffer的长度

Return Values
  BOOL TRUE表示成功，FALSE表示失败
 */
BOOL GetEPGFindExtItemDesbyIndex(const BYTE* pucExtItems, int nExtItemsLen, int nIndex, BYTE* pucItemDes, int nItemDesLen);

/*
GetEPGFindExtItembyIndex
 获取指定Network指定TS流指定Service指定Event指定索引的Item 

Parameters：
  pucExtItems
        [in]Items的buffer,由EPGEXT2INFO的m_pucExtItems获得
  nExtItemsLen
        [in]Items的buffer大小
  nIndex
        [in]要获取的Item索引，从0开始
  pucItem 
        [out]获取到的Item数据，其中第一个字节为item_length，后面为item原始数据
  nItemLen 
        [int]用来获取Item数据buffer的长度

Return Values
  BOOL TRUE表示成功，FALSE表示失败
 */
BOOL GetEPGFindExtItembyIndex(const BYTE* pucExtItems, int nExtItemsLen, int nIndex, BYTE* pucItem, int nItemLen);

/* {{{USR
GetPresentEPG
 获取指定Network指定TS流指定Service的Present EPG 
 BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
      wServiceID, EPGINFO * pEPGINFO );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, EPGINFO * pEPGINFO );

/* {{{USR
GetPresentEPG
 获取指定Network指定TS流指定Service的Present EPG 
 BOOL GetPresentEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
      wServiceID, EPGINFO * pEPGINFO );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetPresentEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, EPGINFO * pEPGINFO );

/* {{{USR
GetFollowEPG
 获取指定Network指定TS流指定Service的Following EPG 
 BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID, EPGINFO * pEPGINFO );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, EPGINFO * pEPGINFO );

/* {{{USR
GetFollowEPG
 获取指定Network指定TS流指定Service的Following EPG 
 BOOL GetFollowEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID, EPGINFO * pEPGINFO );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetFollowEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, EPGINFO * pEPGINFO );

/* {{{USR
GetDayTimeEPG
  获取指定Network指定TS流指定Service指定时间段的Schedule EPG信息
  HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
   wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S *pEndTSYSTEMTIME );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pStartTSYSTEMTIME
        [in]开始时间
  pEndTSYSTEMTIME 
        [in]结束时间

Return Values
    EPG数据的句柄

   }}}USR */
HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
GetDayTimeEPG
  获取指定Network指定TS流指定Service指定时间段的Schedule EPG信息
  HCSHANDLE GetDayTimeEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
   wServiceID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S *pEndTSYSTEMTIME );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  pStartTSYSTEMTIME
        [in]开始时间
  pEndTSYSTEMTIME 
        [in]结束时间

Return Values
    EPG数据的句柄

   }}}USR */
HCSHANDLE GetDayTimeEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME, CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );

/* {{{USR
GetDayTimeEPGCount
  获取指定时间段内,指定EPG数据句柄的EPG信息的个数
  int GetDayTimeEPGCount( HCSHANDLE hGetData, CSUDIPLUSOSSysTime_S * pStartTSYSTEMTIME,
            CSUDIPLUSOSSysTime_S * pEndTSYSTEMTIME );
 
Parameters:
   hGetData
        [in]EPG数据句柄
   pStartTSYSTEMTIME
        [in]开始时间
   pEndTSYSTEMTIME 
        [in]结束时间

Return Values:
           EPG信息的个数，返回0表示失败
   }}}USR */
int GetDayTimeEPGCount( HCSHANDLE hGetData );

/* {{{USR
GetNewHandleIndex
  获取某个指定EPG信息在新的数据句柄中的索引值
  int GetNewHandleIndex( HCSHANDLE hSrcGetData, int nSrcIndex, HCSHANDLE 
          hNewGetData );

Parameters:
   hSrcGetData
        [in]原先的EPG数据句柄
   nSrcIndex
        [in]指定EPG信息在原先EPG数据句柄中的索引
   hNewGetData 
        [in]新的EPG数据句柄

Return Values:
        EPG信息在新的数据句柄中的索引值，返回0表示失败
   }}}USR */
int GetNewHandleIndex( HCSHANDLE hSrcGetData, int nSrcIndex, HCSHANDLE hNewGetData );

/* {{{USR
GetNextEPG
  获取指定EPG数据句柄的EPG信息，并返回下一个EPG数据的句柄
  HCSHANDLE GetNextEPG( HCSHANDLE hData, EPGINFO * pEPGINFO );

Parameters:
   hData
        [in]EPG数据句柄
   pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values:
        下一个EPG数据的句柄，返回NULL表示结束
   }}}USR */
HCSHANDLE GetFirstEPG( HCSHANDLE hGetData, EPGINFO * pEPGINFO );

/* {{{USR
GetNextEPG
  获取指定EPG数据句柄的EPG信息，并返回下一个EPG数据的句柄
  HCSHANDLE GetNextEPG( HCSHANDLE hData, EPGINFO * pEPGINFO );

Parameters:
   hData
        [in]EPG数据句柄
   pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values:
        下一个EPG数据的句柄，返回NULL表示结束
   }}}USR */
HCSHANDLE GetNextEPG( HCSHANDLE hGetData, EPGINFO * pEPGINFO );

/* {{{USR
GetEPGByIndex
   获取指定EPG数据句柄，指定索引的EPG数据
   BOOL GetEPGByIndex( HCSHANDLE hGetData, int nIndex, EPGINFO * pEPGINFO );

Parameters：
  hGetData
        [in]EPG数据句柄
  nIndex
        [in]索引值
  pEPGINFO 
        [out]包含EPG信息的结构体指针

Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetEPGByIndex( HCSHANDLE hGetData, int nIndex, EPGINFO * pEPGINFO );

/* {{{USR
GetTDATECount
  获取指定Network指定TS流指定Service包含的EPG数据的天数
  int GetTDATECount( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
            wServiceID );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值

Return Values:
        返回天数
   }}}USR */
int GetTDATECount( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
GetTDATECount
  获取指定Network指定TS流指定Service包含的EPG数据的天数
  int GetTDATECount( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
            wServiceID );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值

Return Values:
        返回天数
   }}}USR */
int GetTDATECountEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR
GetTDATEByOffset
   获取指定Network指定TS流指定Service指定天数索引的EPG数据的详细日期信息
   BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
        wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  nDayIndex
        [in]天数索引
  pTDATE 
        [out]包含日期信息的结构体指针

Return Values:
        BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

/* {{{USR
GetTDATEByOffset
   获取指定Network指定TS流指定Service指定天数索引的EPG数据的详细日期信息
   BOOL GetTDATEByOffset( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
        wServiceID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  nDayIndex
        [in]天数索引
  pTDATE 
        [out]包含日期信息的结构体指针

Return Values:
        BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetTDATEByOffsetEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID, int nDayIndex, CSUDIPLUSOSDATE_S * pTDATE );

/* {{{USR
DeleteServiceEPG
  删除指定Network指定TS流指定Service的所有EPG数据
  BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID )；

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  
Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID );

/* {{{USR
DeleteServiceEPG
  删除指定Network指定TS流指定Service的所有EPG数据
  BOOL DeleteServiceEPG( HCSHANDLE hEPG, WORD wNetworkID, WORD wTsID, WORD 
       wServiceID )；

Parameters：
  hEPG
        [in]EPG资源句柄
  wNetworkID
        [in]指定Network的ID值
  wTsID
        [in]指定TS的ID值
  wServiceID 
        [in]指定Service的ID值
  
Return Values
  BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL DeleteServiceEPGEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTsID, WORD wServiceID, DWORD dwUserOriginalNetworkID );

/* {{{USR
DeleteAllEPG
  删除所有EPG数据
  BOOL DeleteAllEPG( HCSHANDLE hEPG )；

Parameters：
  hEPG
        [in]EPG资源句柄
   
Return Values
    BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL DeleteAllEPG( HCSHANDLE hEPG );

/* {{{USR
暂时无用
   }}}USR */
BOOL FreeGetDataHandle( HCSHANDLE hGetDataHandle );

/* {{{USR
SetTimeZone
  设置EPG信息的时间偏移（时区）。（时区：[-12，13] ）

Parameters：
  hEPG
    [in]EPG资源句柄
 fTimeOffset
	[In]：时间偏移量 （例如：2.5  表示2小时30分）

Return Values
    BOOL TRUE表示成功，FALSE表示失败

Note
	建议不再使用SetTimeZone，改用对应函数CSUSPEPGSetTimeZone.
	函数原来是SetTimeZone,但是由于在linux平台下有同名函数，所以，如果平台是linux(包括broadCom)，
	必须调用替代函数CSUSPEPGSetTimeZone。
   }}}USR */

BOOL SetTimeZone( HCSHANDLE hEPG, float fTimeOffset );
BOOL CSUSPEPGSetTimeZone( HCSHANDLE hEPG, float fTimeOffset );

/* {{{USR
GetStreamCurTime
  获取当前码流的当前时间。

Parameters：
  hEPG
    [in]EPG资源句柄
 wOriginalNetworkID
	[in]指定Network的ID值
 pStreamTSYSTEMTIME
    [out] 输出年-月-日 时-分-秒数据。
 bIsAddTimeZone  
    [In] 表示是否把时间偏移加入时间的运算中。False表示不加时间偏移。True表示加偏移

Return Values
    BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetStreamCurTime( HCSHANDLE hEPG, WORD wOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStreamTSYSTEMTIME, BOOL bIsAddTimeZone );

/* {{{USR
GetStreamCurTimeEx
    获取当前码流的当前时间。此接口与GetStreamCurTime的区别在于多了一个dwUserOriginalNetworkID参数。
	给网络添加了一个标示符。 

Parameters：
  hEPG
    [in]EPG资源句柄
 wOriginalNetworkID
	[in]指定Network的ID值
 dwUserOriginalNetworkID
    [in]用户指定的网络的标示符
 pStreamTSYSTEMTIME
    [out] 输出年-月-日 时-分-秒数据。
 bIsAddTimeZone  
    [In] 表示是否把时间偏移加入时间的运算中。False表示不加时间偏移。True表示加偏移

Return Values
    BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL GetStreamCurTimeEx( HCSHANDLE hEPG, WORD wOriginalNetworkID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStreamTSYSTEMTIME, BOOL bIsAddTimeZone );

/* {{{USR
PostStreamTSYSTEMTIME
   向指定网络设置当前码流时间和当前系统的Tick数值。此接口用于向EPG模块通知流时间，
   以删除过期的EPG数据。

Parameters：
  hEPG
    [in]EPG资源句柄
 wOriginalNetworkID
	[in]指定Network的ID值
 dwUserOriginalNetworkID
    [in]用户指定的网络的标示符
 pStreamTSYSTEMTIME
    [In]： 输入年-月-日 时-分-秒数据。
 nTickCount  
    [In]：操作系统的Tick值。

Return Values
    BOOL TRUE表示成功，FALSE表示失败

   }}}USR */
BOOL PostStreamTSYSTEMTIME( HCSHANDLE hEPG, WORD wOriginalNetworkID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * streamTSYSTEMTIME, DWORD nTickCount );

/* {{{USR
PostOneSystemTick
   指示EPG模块获取当前系统的Tick数值，以便更新EPG存储的当前流时间，并且删除过期的EPG信息。

Parameters：
  hEPG
    [in]EPG资源句柄

Return Values
    BOOL TRUE表示成功，FALSE表示失败
   }}}USR */
BOOL PostOneSystemTick( HCSHANDLE hEPG );

/* {{{USR
用户指定EPG的搜索天数（即时间段）。
 如果wOriNetworkID, wTSID和wServiceID都不为0xffff,
       说明只接收指定Service在时间段内的EPG。
 如果wOriNetworkID, wTSID不为0xffff, wServiceID 等于0xffff,
       说明只接收指定TS在时间段内的EPG。
 如果wOriNetworkID不为0xffff, wTSID,wServiceID 等于0xffff,
       说明接收指定Network在时间段内的EPG。
 pStartTime 和 pEndTime 是时间段的开始时间和结束时间。
 
 return BOOL
   }}}USR */
BOOL CSUSPSetEPGRecDuration( HCSHANDLE hEPG, WORD wOriginalNetworkID, WORD wTSID, WORD wServiceID, DWORD dwUserOriginalNetworkID, CSUDIPLUSOSSysTime_S * pStartTime, CSUDIPLUSOSSysTime_S * pEndTime );

/* {{{USR
CSUSPEPGGetCurrentIndex
   得到指定EPG数据句炳中当前正在播放的EPG节点的索引值。

Parameters：
 hGetData
    [in]EPG数据句炳
 pCurTSYSTEMTIME
    [In]：当前时间，用于定位当前正在播放的EPG节点。

Return Values
    int 返回当前正在播放的EPG节点的索引值。

   }}}USR */
int CSUSPEPGGetCurrentIndex( HCSHANDLE hGetData, CSUDIPLUSOSSysTime_S * pCurTSYSTEMTIME );

/* {{{USR
设置多网络时间偏移（专为卫星机使用），可以向每个网络设置一个时间偏移。
   }}}USR */
BOOL CSUSPEPGSetTimeOffsetForNetwork( HCSHANDLE hEPG, WORD wOrgNetworkID, DWORD dwUserData, int nSecTime );

/* {{{USR
CSUSPEPGSetEventNameCCTableIndexFlag
设定EPG数据中码表类型标志位的处理类型，

Parameters：
hEPG
[in]EPG数据句炳
[out]bIsGetOriginal: 是否取得原始节目码表类型标志位
TRUE:  m_ucLanguage值为原始节目码表类型标志位
FALSE: m_ucLanguage为USP_CODING_UNICODE或USP_CODING_ASC，默认为该种状态

Return:

SUCCESS: 返回TRUE
FAILURD: 返回FALSE

Description:

当hEPG无效时，返回失败
}}}USR */
BOOL CSUSPEPGSetEventNameCCTableIndexFlag( HCSHANDLE hEPG, BOOL bIsGetOriginal );

/* {{{USR
CSUSPEPGSetEventNameLanguageType
设定搜索EPG事件信息的时候的语言类型

Parameters：
hEPG
[in]eLangCode 见枚举定义类型。
Return:
TRUE:  设置成功
FALSE: 设置失败

Description:

}}}USR */
BOOL CSUSPEPGSetEventNameLanguageType(eEPGLangType eLangCode);

// {{{USR
#ifdef  __cplusplus
}
#endif
// }}}USR

#endif /* EPGUserInterface_H */

