#ifndef SubtitleInterface_H
#define SubtitleInterface_H

/* {{{USR

   }}}USR */

// {{{USR
#ifdef __cplusplus
extern "C" 
{
#endif
#include "mmcp_typedef.h"

typedef enum _SubtitleCallBackType
{
	SubtitleDispaly,			/*显示Subtitl*/
	SubtitleClear,				/*清除所显示的Subtitl信息*/
	SubtitleClearAndDispaly		/*清除所显示的Subtitl信息之后,再显示Subtitl*/
}SubtitleCallBackType;

//区分当前subtitle的类型
typedef enum _CSUSPSBTLType_E{
	EM_USPSBTL_DVBSubtitle = 0,/*采用DVB标准的Subtitle方式解码*/
	EM_USPSBTL_DCIISubtitle    /*采用DCII标准进行解码*/
}CSUSPSBTLType_E;

typedef void(*SubtitleCallBack)(SubtitleCallBackType enmType, void* pParam);
/*
	enmType : 回调消息类型,分别为:
		SubtitleDispaly
		SubtitleClear
		SubtitleClearAndDispaly

	当消息为SubtitleDispaly和SubtitleClearAndDispaly时pParam为指向SUBTITLESHOWPAR实例的指针,该实例在Subtitle销毁之前有效,
	但其中的参数在调用CSUSPSBTLShowFinished或者两秒钟之后,参数无效.在参数无效之后,继续显示Subtitle信息不会导致该模块
	稳定性问题,但显示的信息为错误信息
*/

typedef struct TagPTSLL{
	DWORD high;
	DWORD low;
} PTSLL;// PTSLL;

typedef void*(*SubtitleCallBackForPts)(void* pParam);

typedef struct tagSUBTITLESHOWPAR
{
	PBYTE m_pucBitmapMemorySrc;				/*位图信息的首地址*/
	DWORD m_nBitmapMemorySrcCount;			/*位图信息的长度*/
	DWORD m_colorDepth;						/*位图颜色位数,取值为1,2,4,8*/
	PBYTE m_pucPaletteData;					/*位图调色板信息的首地址*/
	DWORD m_dwPaletteDataCount;				/*位图调色板信息的长度*/
	DWORD m_dwSrcX;							/*位图显示位置(X坐标)*/
	DWORD m_dwSrcY;							/*位图显示位置(Y坐标)*/
	DWORD m_dwSrcWidth;						/*位图宽度*/
	DWORD m_dwSrcHeight;					/*位图高度*/
}SUBTITLESHOWPAR,*PSUBTITLESHOWPAR;

//DCIISubtitle显示的坐标系统
typedef struct{
	WORD wTopH;								/*左上角顶点x坐标*/                      
	WORD wTopV;								/*左上角顶点y坐标*/  
	WORD wBottomH;							/*右下角顶点x坐标*/  
	WORD wBottomV;							/*右下角顶点y坐标*/  
}CSUSPSBTLCoordinateSys_S;

//字幕显示样式
typedef enum _CSUSPSBTLDisplayStyle_E{
	EM_USPSBTL_NORMAL = 0,					/*普通显示样式*/
	EM_USPSBTL_OUTLINE,    					/*带边框的显示样式*/
	EM_USPSBTL_DROP_SHADOW,   				/*带阴影的显示样式*/
	EM_USPSBTL_RESERVED       				/*预留显示样式*/
}CSUSPSBTLDisplayStyle_E;

//subtitle基本结构单元
typedef struct {
	BOOL m_bBackgroundStyle;                 	  /*字幕背景模式，0:透明，1:有背景*/
	CSUSPSBTLDisplayStyle_E m_emOutlineStyle;     /*字幕样式，0:普通(none)，1:带边框(outline)，2:带阴影(drop_shadow)*/
	DWORD m_dwCharacterColor;                     /*字幕颜色(RGBA)*/
	CSUSPSBTLCoordinateSys_S m_stBmpCoordinate;   /*字幕显示位置*/
	DWORD m_dwFrameColor;                         /*背景颜色(RGBA)，仅在背景模式为1时有效*/
	CSUSPSBTLCoordinateSys_S m_stFrameCoordinate; /*背景显示位置，仅在背景模式为1时有效*/
	DWORD m_dwOutlineThickness;                   /*边框宽度，仅在字幕样式为1时有效*/
	DWORD m_dwOutlineColor;                       /*边框颜色(RGBA)，仅在字幕样式为1时有效*/
	DWORD m_dwShadowRight;                        /*阴影往右的偏移量，仅在字幕样式为2时有效*/
	DWORD m_dwShadowBottom;                       /*阴影往下的偏移量，仅在字幕样式为2时有效*/
	DWORD m_dwShadowColor;                        /*阴影部分的颜色(RGBA)，仅在字幕样式为2时有效*/
	DWORD m_dwBmpLength;                          /*压缩位图的数据长度*/
	PBYTE m_pucBmpMemorySrc;                      /*压缩位图信息的首地址*/
	DWORD m_dwWindowWidth;						  /*参考坐标系宽度*/
	DWORD m_dwWindowHeight;						  /*参考坐标系高度*/
}CSUSPSBTLDCIIShowPar_S;

// }}}USR

/* {{{USR
CSUSPSBTLCreate
	创建Subtitle,支持多个实例

	HCSHANDLE CSUSPSBTLCreate(
		HCSHANDLE hMemoryPool,
		HCSHANDLE hDemuxHandle,
		HCSHANDLE hSectionSource,
		int nDispatchTaskIndex
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄;内存池大小可通过CSUSPSBTLGetMemorySizeScope获取

	hDemuxHandle
		[in]Demux标识

	hSectionSource
		[in]SectionSource句柄

	nDispatchTaskIndex
		[in]分发任务索引
		
Return Values
		1）如果成功返回Subtitle句柄
		2）如果失败返回为NULL

		当hMemoryPool为空或hSectionSource无效或nBuffSize<8192时，创建将不会成功
   }}}USR */
HCSHANDLE CSUSPSBTLCreate( HCSHANDLE hMemoryPool, HCSHANDLE hDemuxHandle, HCSHANDLE hSectionSource, int nDispatchTaskIndex );

/* {{{USR
CSUSPSBTLCreateEx
	根据传入的subtitle类型，创建Subtitle,支持多个实例

	HCSHANDLE CSUSPSBTLCreateEx(
		HCSHANDLE hMemoryPool,
		HCSHANDLE hDemuxHandle,
		HCSHANDLE hSectionSource,
		int nDispatchTaskIndex,
		CSUSPSBTLType_E emSubType
	};	

Parameters
	
	hMemoryPool
		[in]内存池句柄;内存池大小可通过CSUSPSBTLGetMemorySizeScope获取

	hDemuxHandle
		[in]Demux标识

	hSectionSource
		[in]SectionSource句柄

	nDispatchTaskIndex
		[in]分发任务索引

	emSubType
		[in]subtitle 的类型
		
Return Values
		1）如果成功返回Subtitle句柄
		2）如果失败返回为NULL

		当hMemoryPool为空或hSectionSource无效或nBuffSize<8192时，创建将不会成功
   }}}USR */
HCSHANDLE CSUSPSBTLCreateEx( HCSHANDLE hMemoryPool, HCSHANDLE hDemuxHandle, HCSHANDLE hSectionSource, int nDispatchTaskIndex, CSUSPSBTLType_E emSubType);

/* {{{USR
 CSUSPSBTLDestroy

    删除SubTitle模块的句柄. 
    如果参数hSubtitle为NULL或非法指针，函数返回FALSE。 

Parameters

     hSubtitle 
       [In]: Subtitle 模块句柄

RETURN VALUE  
     BOOL TRUE表示删除成功，FALSE表示删除失败

   }}}USR */
BOOL CSUSPSBTLDestroy( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLSetCallBack
    创建Subtitle句柄后，启动Subtitle的搜索前。调用此接口注册用户回调函数。
    如果参数SubtitleCallBack或pSubtitleCallBack为NULL，返回FALSE，表示注册失败，
	数据将无法正常显示。

Parameter  
      hSubtitle
        [In]: Subtitle模块句柄 

     pSubtitleCallBack 
        [In]: Subtitle模块的回调函数句柄。

RETURN VALUE  
     BOOL TRUE表示注册成功，FALSE表示注册失败
   }}}USR */
BOOL CSUSPSBTLSetCallBack( HCSHANDLE hSubtitle, SubtitleCallBack pSubtitleCallBack );

/* {{{USR
 CSUSPSBTLStart
    启动Subtitle数据的搜索。并在搜索的过程中给用户回调Subtitle数据。　
    如果参数hSubtitle为NULL或非法指针，返回FALSE，表示启动失败。

Parameter  
    hSubtitle
       [In]: Subtitle模块句柄 

RETURN VALUE  
    BOOL：TRUE表示启动成功，FALSE表示启动失败
   }}}USR */
BOOL CSUSPSBTLStart( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLStartEx
    启动Subtitle数据的搜索。并在搜索的过程中给用户回调Subtitle数据。　
    如果参数hSubtitle为NULL或非法指针，返回FALSE，表示启动失败。

Parameter  
    hSubtitle
       [In]: Subtitle模块句柄 
    hDemuxHandle
       [In]: 指定demux ID

RETURN VALUE  
    BOOL：TRUE表示启动成功，FALSE表示启动失败
   }}}USR */
BOOL CSUSPSBTLStartEx( HCSHANDLE hSubtitle, HCSHANDLE hDemuxHandle );

/* {{{USR
 CSUSPSBTLStop
    停止Subtitle的搜索（即停止Section 搜索）。
    如果参数hSubtitle为NULL或非法指针，返回FALSE，表示停止失败。

 Parameter  
    hSubtitle
       [In]: Subtitle模块句柄 

 RETURN VALUE  
    BOOL：TRUE表示操作成功，FALSE表示操作失败
   }}}USR */
BOOL CSUSPSBTLStop( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLSetPar
    创建Subtitle句柄后，启动Subtitle的搜索前。调用此接口设置Subtitle的参数值。 
    如果参数hSubtitle为NULL或非法指针，返回FALSE，表示设置参数失败。 
    如果不设置参数，启动Subtitle的搜索，将没有任何数据显示。

Parameter  
    hSubtitle
      [In]:Subtitle模块句柄 
 
    nPID 
      [In]:Subtitle数据在TS流中的PID值，此数值从ＰＭＴ的描述符中获得 

    nPageID
      [In]:Subtitle页面的ID值，此数值从ＰＭＴ的描述符中获得 

    nAncillaryPageID 
      [In]:Subtitle副页面的ID值，此数值从ＰＭＴ的描述符中获得

RETURN VALUE  
    BOOL TRUE表示设置成功，FALSE表示设置失败
   }}}USR */
BOOL CSUSPSBTLSetPar( HCSHANDLE hSubtitle, int nPID, int nPageID, int nAncillaryPageID );

/* {{{USR
 CSUSPSBTLShowFinished
      当用户处理完一个用于显示的回调函数时，必须调用此接口释放此次回调Subtitle占用的资源，
	调用CSUSPSBTLShowFinished或者两秒钟之后,，回调函数pParam参数值将无效.在参数无效之后,
	继续显示Subtitle信息不会导致该模块稳定性问题  但显示的信息为错误信息。
      如果参数hSubtitle为NULL或非法指针，返回FALSE，表示操作失败。

 Parameter  
    hSubtitle
       [In]: Subtitle模块句柄 

 RETURN VALUE  
   BOOL：TRUE表示释放成功，FALSE表示释放失败 
   }}}USR */
BOOL CSUSPSBTLShowFinished( HCSHANDLE hSubtitle );

/* {{{USR

   }}}USR */
BOOL CSUSPSBTLSetThreadParam( HCSHANDLE hSubtitle, DWORD dwPriority, DWORD dwStackCount, void * pParam );

/* {{{USR
 CSUSPSBTLGetMemorySizeScope
    得到Subtitle使用内存数量的最大值和最小值。
    如果参数pnMinSize或pnMaxSize为NULL、则返回FALSE。

 Parameter  
    pnMinSize
      [In]:使用内存最小值

    pnMaxSize　
      [In]: 使用内存最大值

 RETURN VALUE  
    BOOL：TRUE表示操作成功，FALSE表示操作失败
   }}}USR */
BOOL CSUSPSBTLGetMemorySizeScope( int * pnMinSize, int * pnMaxSize );

/* {{{USR
 CSUSPSBTLGetVersionInfo
   得到TELE模块的版本号信息。返回版本信息的长度。
   如果参数pcVersionInfo为NULL、则返回版本信息的长度。

 Parameter  
   pcVersionInfo
     [In]:用于存储版本信息的数组　

   nSize
     [In]: pcVersionInfo 数组长度

 RETURN VALUE  
   版本信息的长度。
   }}}USR */
int CSUSPSBTLGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSBTLSetIsNeedSyncByPts
设置Subtitle模块是否需要根据实时的PTS同步字幕的显示。

Parameter  
	hSubtitle
		[In]:模块句柄　

	bIsNeedSync
		[In]: 是否需要根据PTS的同步功能，TRUE：需要，FALSE：不需要

	pCallbackForPts
		[In]:回调函数，由上层应用实现，通过该函数获取实时PTS数据，当bIsNeedSync设置为FALSE，此值无效，可以传NULL。

RETURN VALUE  
	TRUE： 设置成功
	FALSE：设置失败
}}}USR */
BOOL CSUSPSBTLSetIsNeedSyncByPts( HCSHANDLE hSubtitle, BOOL bIsNeedSync, SubtitleCallBackForPts pCallbackForPts );

// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* SubtitleInterface_H */
