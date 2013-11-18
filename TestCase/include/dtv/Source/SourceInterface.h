#ifndef SourceInterface_H
#define SourceInterface_H

#ifdef PRAGMA
#pragma interface "SourceInterface.h"
#endif

/* {{{USR

   }}}USR */

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h" 

// {{{USR

#ifdef __cplusplus
extern "C" 
{
#endif

typedef struct _SourceSatelliteDeliver
{
	SatelliteDeliver m_SatelliteDeliver;  //卫星机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    LNB m_LNB;                            //高频头信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	DiSEqC1020 m_DiSEqC1020;              //diseq1.0信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    DiSEqC1121 m_DiSEqC1121;              //diseq1.1信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	DiSEqC1222 m_DiSEqC1222;              //diseq1.2信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	SatellitePar m_SatellitePar;          //卫星参数信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	AntennaPar m_AntennaPar;              //天线参数信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    TransponderOffset m_TransponderOffset;//频点偏移值信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
} SourceSatelliteDeliver;

typedef union _SourceDeliver
{
	CableDeliver m_CableDeliver;                        //有线机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	SourceSatelliteDeliver m_SourceSatelliteDeliver;    //见上面定义
	TerrestrialDeliver m_TerrestrialDeliver;            //地面机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
}SourceDeliver;

typedef struct _SourceSatelliteDeliverExt
{
	SatelliteDeliverExt m_SatelliteDeliver;  //卫星机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    LNB m_LNB;                            //高频头信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	DiSEqC1020 m_DiSEqC1020;              //diseq1.0信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    DiSEqC1121 m_DiSEqC1121;              //diseq1.1信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	DiSEqC1222 m_DiSEqC1222;              //diseq1.2信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	SatellitePar m_SatellitePar;          //卫星参数信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	AntennaPar m_AntennaPar;              //天线参数信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
    TransponderOffset m_TransponderOffset;//频点偏移值信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
} SourceSatelliteDeliverExt;

typedef union _SourceDeliverExt
{
	CableDeliver m_CableDeliver;                        //有线机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
	SourceSatelliteDeliverExt m_SourceSatelliteDeliver;    //见上面定义
	TerrestrialDeliver m_TerrestrialDeliver;            //地面机频点信息结构体，详见DataManager的CSUSPDMCommonType.h头文件中说明。
}SourceDeliverExt;

//下面的结构与HDI的结构体CSHDICallbackType类似
typedef enum
{
	CSOURCE_CALLBACK_IR,			// callback from cs_ir
	CSSOURCE_CALLBACK_PANEL,		// callback from cs_panel
	CSSOURCE_CALLBACK_QPSK,		    // callback from cs_qpsk
	CSSOURCE_CALLBACK_COFDM,	    // callback from cs_cofdm
	CSSOURCE_CALLBACK_QAM		    // callback from cs_qam
} CSSourceCallbackType;

typedef enum
{
 SOURCE_SIGNAL_CONNECTED,           //频点锁定成功
 SOURCE_SIGNAL_DROPPED,             //由频点锁定成功转为锁定失败
 SOURCE_SIGNAL_LOST,                //频点锁定失败
 SOURCE_SIGNAL_SEARCHING,           //正在锁定频点
 SOURCE_SIGNAL_NOSIGNAL				//无信号
}SourceCallbackEvents;

//回调消息类型定义
typedef struct _SourceCallbackMessage
{
 SourceCallbackEvents m_SourceCallbackEvent;    //见前面定义
 DWORD dwFrequency;                             //回调消息对应的频点，单位KHZ
 DWORD dwSymbolRate;                            //回调消息对应的符号率，单位Kbps
 DWORD dwBandWidth;                             //回调消息对应的波段，实际对应UDI中的CSUDITunerBanuidth_E枚举类型，值含义：0----8M, 1----7M, 2----6M。
 DWORD dwQAMMode;                               //回调消息对应的调制方式，实际对应UDI中的CSUDITunerQAMMode_E枚举类型，值含义：0----Not Define, 1----16QAM, 2----32QAM, 3----64QAM, 4----128QAM, 5----256QAM。
 DWORD dwTunerIndex;                            //回调消息对应的Tuner索引号
 void *pvUserData;                              //用户在注册回调函数时传入的指针
} SourceCallbackMessage;

typedef  void ( *CSUSPSourceCallback )( CSSourceCallbackType nSourceCallbackType, SourceCallbackMessage* pSourceCallbackMessage);

// }}}USR

/* {{{USR
CreateFileSource
    创建文件型用于锁频的Source句柄(未实现具体内容)。

	 HCSHANDLE CreateFileSource( void );

Parameters
       无
Return Values
		1）如果成功返回用于锁频的Source句柄
		2）如果失败返回为NULL

   }}}USR */
HCSHANDLE CreateFileSource( void );

/* {{{USR
CreateHDISatelliteSource
    创建情卫星的用于锁频的Source句柄。(针对HDI驱动)

	 HCSHANDLE CreateHDISatelliteSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner的索引号
Return Values
		1）如果成功返回用于锁频的Source句柄
		2）如果失败返回为NULL

   }}}USR */
HCSHANDLE CreateHDISatelliteSource( HCSHANDLE hDevice );

/* {{{USR
CreateHDICableSource
    创建情有线的用于锁频的Source句柄。(针对HDI驱动)

	 HCSHANDLE CreateHDICableSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner的索引号
Return Values
		1）如果成功返回用于锁频的Source句柄
		2）如果失败返回为NULL

   }}}USR */
HCSHANDLE CreateHDICableSource( HCSHANDLE hDevice );

/* {{{USR
CreateHDITerrestriaSource
    创建情地面的用于锁频的Source句柄。(针对HDI驱动)

	 HCSHANDLE CreateHDITerrestriaSource( HCSHANDLE hDevice );

Parameters
       hDevice
       [in]Tuner的索引号
Return Values
		1）如果成功返回用于锁频的Source句柄
		2）如果失败返回为NULL

   }}}USR */
HCSHANDLE CreateHDITerrestriaSource( HCSHANDLE hDevice );

/* {{{USR
CloseSource
    释放用于锁频的Source句柄资源。

	 BOOL CloseSource( HCSHANDLE hSource );

Parameters
       hSource 
       [in]用于锁频的Source句柄
Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL CloseSource( HCSHANDLE hSource );

/* {{{USR
GetSignalQuality
    获取Tuner的信号质量。

	 int GetSignalQuality( HCSHANDLE hSource );

Parameters
       hSource 
       [in]用于锁频的Source句柄
Return Values
		返回信号质量

   }}}USR */
int GetSignalQuality( HCSHANDLE hSource );

/* {{{USR
GetSignalStrength
    获取Tuner的信号强度。

	 int GetSignalStrength( HCSHANDLE hSource );

Parameters
       hSource 
       [in]用于锁频的Source句柄
Return Values
		返回信号强度
   }}}USR */
int GetSignalStrength( HCSHANDLE hSource );

/* {{{USR
GetErrorRate
    获取Tuner的错误率。

	 int GetErrorRate( HCSHANDLE hSource );

Parameters
       hSource 
       [in]用于锁频的Source句柄
Return Values
		返回错误率
   }}}USR */
int GetErrorRate( HCSHANDLE hSource );

/* {{{USR
ConnectSource
    根据指定的频点进行锁频操作。

	 BOOL ConnectSource( HCSHANDLE hSource, Deliver * pDeliver );

Parameters
       hSource 
       [in]用于锁频的Source句柄
       pDeliver 
       [in]用户存放频点信息的数据块的地址
Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL ConnectSource( HCSHANDLE hSource, const SourceDeliver * pSourceDeliver );

/* {{{USR
ConnectSourceFromDM

   　根据指定的频点进行锁频操作。	 　

     BOOL ConnectSourceFromDM( HCSHANDLE hSource, HCSHANDLE hTransponder, HCSHANDLE hDataContainer);

Parameters
       hSource 
       [in]用于锁频的Source句柄
       hTransponder
       [in] 频点句柄，也就是在数据管理中的存放地址。
       hDataContainer
       [in] 数据容器的句柄

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL ConnectSourceFromDM( HCSHANDLE hSource, HCSHANDLE hTransponder, HCSHANDLE hDataContainer );

/* {{{USR
CSUSPSourceGetVersionInfo
    获取模块版本信息， 包括版本号、最后修订日期。用户可以选择性地调用该方法获取USP或者其子模块的版本信息。

	 int CSUSPSourceGetVersionInfo( char * pcVersionInfo, int nSize );;

Parameters
       pcVersionInfo
       [out]存放版本信息的缓冲区首地址，输出参数,存放版本信息。格式如下：
            “Module Name: EPG\nCurrent Version: 1.2\nRevision Date: 2005-5-11”
       nSize
       [in]    缓冲区的长度

Return Values
		1)	如果第一个参数pcVersionInfo为空，返回版本信息的实际长度。建议用户输入第二个参数为：版本信息的实际长度+       
1。 
       2)	如果第一个参数pcVersionInfo不为空，且函数调用成功，返回版本信息的实际长度；
       3)	如果失败，返回0。

       利用第一种返回值的情况，用户可测试该函数，并为第二个参数nSize提供参考。
       如果用户输入的第二个参数小于版本信息的实际长度，尾部多余的字节被截掉。
   }}}USR */
int CSUSPSourceGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSourceGetMemInfo
    获取模块要求的内存空间的最小值和最大值。当该模块要求一个固定空间时，*pnMin 和*pnMax相等；有的模块至少需要*pnMin bytes才能正常运行；在分配*pnMax空间时，可获得更好的性能。

	 BOOL CSUSPSourceGetMemInfo( int * pnMin, int * pnMax );;

Parameters
       pnMin
       [out]模块所需最小内存 (in bytes)
       pnMax
       [out]模块所需最大内存(in bytes)

Return Values
       1)成功，返回TRUE;
       2)失败，返回 FALSE

Remarks
       1）	当模块要求固定内存时，返回的最大内存和最小内存相等。；
       2）	当模块要求最小内存为*pnMin， 而在*pnMin 和*pnMax之间可获得更好的性能或更多功能时，最大内存和最小内存不等。
       3）	当pnMin =NULL 或 pnMax = NULL，函数调用失败。
       4） 该方法对内存的合理使用，硬件的配置具有参考价值。    

   }}}USR */
BOOL CSUSPSourceGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR
CSUSPSourceAddCallback
     注册指定的回调函数。

     BOOL CSUSPSourceAddCallback(HCSHANDLE hSource, CSUSPSourceCallback callback, void * pData);
  　
Parameters
       hSource 
       [in]用于锁频的Source句柄
       callback
       [in]回调函数句柄
       pData
       [in]用户传入的自定义指针

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

Remarks
       1)回调函数的最大注册个数为20个，超过20个将返回FALSE
       2)注册时的以tuner号与callback为键值。

   }}}USR */
BOOL CSUSPSourceAddCallback( HCSHANDLE hSource, CSUSPSourceCallback callback, void * pData );

/* {{{USR
CSUSPSourceDelCallback
     删除指定的回调函数。

     BOOL CSUSPSourceDelCallback( HCSHANDLE hSource, CSUSPSourceCallback callback );

Parameters
       hSource 
       [in]用于锁频的Source句柄
       callback
       [in]回调函数句柄

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

Remarks
      1)如果该回调函数根本就没有注册，将返回FALSE

   }}}USR */
BOOL CSUSPSourceDelCallback( HCSHANDLE hSource, CSUSPSourceCallback callback );

/* {{{USR
CSUSPSourceSetPosition
       设置用户所在的经度与纬度。

       BOOL CSUSPSourceSetPosition (DWORD dLongitude, DWORD dLatitude);
Parameters
       dLongitude
       [in]经度值
       dLatitude
       [in]纬度值

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL CSUSPSourceSetPosition( double dLongitude, double dLatitude );

/* {{{USR
CSUSPSourceGetPosition

       获取用户所在的经度与纬度。

Parameters
       pdOutLongitude
       [out]经度值，不能为NULL，否则返回失败
       pdOutLatitude
       [out]纬度值，不能为NULL，否则返回失败

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL CSUSPSourceGetPosition( double * pdOutLongitude, double * pdOutLatitude );


/* {{{USR
CSUSPSourceSetS2Flag
       设置Source调用驱动新锁频接口。

Parameters
       hSatSource
       [in]用于锁频的Source句柄，必须是卫星的
       hContainerHandle
       [in]DM容器句柄
       bFlag
       [in]需要用新功能：S2或者option，就要传入TRUE

Return Values
		1）如果成功返回TRUE
		2）如果失败返回FALSE

   }}}USR */
BOOL CSUSPSourceSetS2Flag( HCSHANDLE hSatSource, HCSHANDLE hContainerHandle, BOOL bFlag );

BOOL CSUSPSourceSetSearchingCount( HCSHANDLE hSource, int nCount );

BOOL CSUSPSourceSetTunerDemuxPath(HCSHANDLE hSource, HCSHANDLE hPath);

// {{{USR
#ifdef __cplusplus
}
#endif

// }}}USR

#endif /* SourceInterface_H */
