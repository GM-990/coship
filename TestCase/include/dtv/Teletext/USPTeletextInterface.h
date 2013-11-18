#ifndef USPTeletextInterface_H
#define USPTeletextInterface_H

#include "mmcp_typedef.h"
#include "mwapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	VBI_MODE,	           //VBI显示方式
	STB_MODE               //机顶盒解码方式

}DisplayPattern;

typedef enum _TELECallbackType
{
	START_DISPLAY,         //界面开始显示
	PAGE_UPDATE,           //页面更新
	LINK_FLASH,            //Link页闪烁
	PAGE_FLASH,             //一般页数据闪烁
	CAPTION_PAGE           //显示Close Caption页

}TELECallbackType;

typedef struct
{
	int nLeft;
	int nRight;
	int nTop;
	int nBottom;
}CSTELERect_S;

typedef struct
{
	long lCx;
	long lCy;
}CSTELESize_S;

typedef struct
{
	int nClientId;
	GR_WINDOW_ID nWinId;
	GR_GC_ID nGcId;
}CSTELEContext_S;

//和MicroWin中定义一致
#define TELE_RGBA(r,g,b,a)  ((DWORD)(((unsigned long)a<<24)|(((unsigned char)(b)|(unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(r))<<16)))
//和GUI中定义一致
//#define TELE_RGBA(r,g,b,a)  ((DWORD)(((unsigned long)a<<24)|(((unsigned char)(r)|(unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

typedef void(*TELECallback)(TELECallbackType enmType, void* pParam);

/* {{{USR
创建Teletext模块句柄,使用GUI画图
Parameter:
   [in]munDisplayPattern   设置显示模式：VBI_MODE 或 STB_MODE
   [in]pCallback            用户注册的回调函数句柄
   [in]hSectionSource       Section模块句柄
   [in]hMemoryPool           内存池句柄
   [in]nDispatchTaskIndex   Section分发的优先级
   [in]nDemaxID             Demax设备的ID值

return :
   HCSHANDLE  Teletext模块句柄
   }}}USR */
HCSHANDLE CSUSPCreateTELEHandle( DisplayPattern munDisplayPattern, TELECallback pCallback, HCSHANDLE hSectionSource, HCSHANDLE hMemoryPool, int nDispatchTaskIndex, int nDemuxID);

/* {{{USR
创建Teletext模块句柄,使用MicroWin画图
Parameter:
   [in]munDisplayPattern   设置显示模式：VBI_MODE 或 STB_MODE
   [in]pCallback            用户注册的回调函数句柄
   [in]hSectionSource       Section模块句柄
   [in]hMemoryPool           内存池句柄
   [in]nDispatchTaskIndex   Section分发的优先级
   [in]nDemaxID             Demax设备的ID值

return :
   HCSHANDLE  Teletext模块句柄
   }}}USR */
HCSHANDLE CSUSPCreateMWTELEHandle( DisplayPattern munDisplayPattern, TELECallback pCallback, HCSHANDLE hSectionSource, HCSHANDLE hMemoryPool, int nDispatchTaskIndex, int nDemuxID);

/* {{{USR
删除Teletext模块的句柄

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    
   }}}USR */
BOOL CSUSPDestroyTELEHandle( HCSHANDLE hTeletextHandle );

/* {{{USR
启动Teletext的接收，启动显示需要的线程

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]nTeletextPID     Teletext码流的PID值

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveTeletext( HCSHANDLE hTeletextHandle, int nTeletextPID, int nMagazineNum, int nPageNum );

/* {{{USR
停止Teletext的接收,停止所有Teletext启动的线程

Parameter:
    [in]hTeletextHandle   Teletext模块句柄

return:
    BOOL
   }}}USR */
BOOL CSUSPStopReceiveTeletext( HCSHANDLE hTeletextHandle );

/* {{{USR
显示指定页的Teletext数据。

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域
    [in]ucPageNum    页码

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowSelectPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, BYTE ucPageNum );

/* {{{USR
显示当前页码加一的页的Teletext数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowDownPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
显示当前页码减一的页的Teletext数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowUpPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
显示最近显示过的Teletext页数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowPreShowPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
显示当前页的Teletext数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowCurPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
显示用户选择的Link页码的页数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowLinkPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
使闪烁焦点移到下一个Link页码

Parameter:
    [in]hTeletextHandle   Teletext模块句柄

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEGotoLinkgeDownPage( HCSHANDLE hTeletextHandle );

/* {{{USR
使闪烁焦点移到上一个Link页码

Parameter:
    [in]hTeletextHandle   Teletext模块句柄

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEGotoLinkgeUpPage( HCSHANDLE hTeletextHandle );

/* {{{USR
隔250毫秒执行一次函数 改变Link的颜色

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEChangeLinkPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
注册VBI显示模式的回调函数
   }}}USR */
BOOL CSUSPTELERegVBICallbackFun( HCSHANDLE hTeletextHandle );

/* {{{USR
注册字库集到Teletext模块

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]nFontCount        字库句柄的个数
    [in]hFontArray        包含字库句柄的数组

return:
    BOOL
   }}}USR */
BOOL CSUSPTELERegisterFont( HCSHANDLE hTeletextHandle, int nFontCount, HCSHANDLE * hFontArray );

/* {{{USR
隔250毫秒执行一次函数 改变需要闪烁的数据的颜色

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEChangeFlashPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
清除页面第一行的页码值

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEClearTitle( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
在页面的第一行显示当前时间

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域
    [in]pucTime     包含时间数据的字符串
    [in]nTimeLen    pucTime字符串的长度

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEDisplayTime( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, BYTE * pucTime, int nTimeLen );

/* {{{USR
在搜索第一页出现之前显示正在搜索提示

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]hDC         HDC句柄
    [in]tShowRect   显示的区域

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEdisplaySearchTitle( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
启动Teletext的接收，启动显示需要的线程

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]nTeletextPID     Teletext码流的PID值

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveCaption( HCSHANDLE hTeletextHandle, int nCaptionPID, int nMagazineNum, int nPageNum );

/* {{{USR

   }}}USR */
BOOL CSUDPTELESetRowHeight( HCSHANDLE hTeletextHandle, DWORD dwHeight );

/* {{{USR
显示Close Caption页面
   }}}USR */
BOOL CSUSPShowCaptionPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
得到TELE模块的版本号信息。
   }}}USR */
int CSUSPTELEGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
得到TELE使用内存数量的最大值和最小值。
   }}}USR */
BOOL CSUSPTELEGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetBGColor( HCSHANDLE hTeletextHandle, DWORD dwGBColor, DWORD dwTransparency );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetString( HCSHANDLE hTeletextHandle, char * pString1, int nString1Len, char * pString2, int nString2Len );

/* {{{USR

   }}}USR */
BOOL CSUSPTELEIsSetEmptyRowBColor( HCSHANDLE hTeletextHandle, BOOL bIsSetBColor );

/* {{{USR
显示当前页的Teletext数据

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [HCSDC]hDC			  HDC句柄
    [CSRECT *]tShowRect   显示的区域
	[int]nColorIndex      四色键颜色序号，为0、1、2、3,对应的颜色为红、绿、黄、兰
return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowFastextPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, int nColorIndex );

/* {{{USR
传出当前显示的页码。

hTeletextHandle [in]:
		创建的Teletext句柄。
*wPageNum [out]:
		传出当前显示的页码。

return:
BOOL
   }}}USR */
BOOL CSUSPTELEGetCurPage( HCSHANDLE hTeletextHandle, WORD * wPageNum );

/* {{{USR
从当前页开始跳转用户指定的页数

hTeletextHandle [in]
		Teletext句柄
hDC [in]
		设备上下文描述
*tShowRect [in]:
		显示区域
nJumpNum [in]:
		需要从当前页跳转的页数，增加设为正值，减少设为负值。如果需要跳转的页码超过能显示的最大值，则显示起始页；若需要跳转的页码小于起始页，也显示起始页。
		例：如果需要从当前页跳转到后100页，则设为100；若需要从当前页跳转到前100页，则设为-100。

return:
BOOL
   }}}USR */
BOOL CSUSPTELEJumpPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, int nJumpNum );

/* {{{USR
显示TTX的TV Station及时间

hTeletextHandle [in]
Teletext句柄
bDisplayHead [in]:
是否显示TV Station及时间标志，为TRUE显示，为FALSE则不显示，默认为FALSE

return:
BOOL
   }}}USR */
BOOL CSUSPTELEDisplayPageHead( HCSHANDLE hTeletextHandle, BOOL bDisplayHead );

/* {{{USR
设置teletext的刷新频率，默认频率是每3秒钟刷新一次，必须在初始化Teletext接口之后，第一次调用接口CSUSPStartReceiveTeletext之前调用此接口才有效

nTime [in]:
每隔多少秒刷新一次，如设置成5表示每隔5秒钟刷新一次
   }}}USR */
BOOL CSUSPTELESetRefreshFre( HCSHANDLE hTeletext, int nTime );

/* {{{USR
设置保存全部800页内容标志，默认只保存当前100页的内容，必须在初始化Teletext接口之前调用

bFlag [in]:
TRUE 表示保存800页（注意此时要给Teletext分配的内存必须大于1200k）
FALSE 表示只保存当前100页
   }}}USR */
BOOL CSUSPTELESetSaveAllFlag( BOOL bFlag );

/* {{{USR
启动Teletext的接收，启动显示需要的线程

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]nTeletextPID     Teletext码流的PID值

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveTeletextExt( HCSHANDLE hTeletextHandle, int nTeletextPID, int nMagazineNum, int nPageNum, int nDemuxId );

/* {{{USR
启动Teletext的接收，启动显示需要的线程

Parameter:
    [in]hTeletextHandle   Teletext模块句柄
    [in]nTeletextPID     Teletext码流的PID值

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveCaptionExt( HCSHANDLE hTeletextHandle, int nCaptionPID, int nMagazineNum, int nPageNum, int nDemuxId );

BOOL CSUSPTELEShowNextChildPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetStepShowFlag( HCSHANDLE hTeletextHandle, BOOL bFlag );

//为TRUE，表示空页无显示
//为FALSE，表示空页显示上个页的内容
BOOL CSUSPTELESetShowEmptyPageMode( HCSHANDLE hTeletextHandle, BOOL bMode );

/* {{{USR
创建CCExt的句柄
Parameter:
无
return:
    创建的句柄
   }}}USR */
HCSHANDLE CSUSPCCExtCreateHandle( void );

/* {{{USR
启动解析和显示从VIDEO PES中获取的CC数据
Parameter:
	hCCExtHandle: 已经创建的CCExt句柄
	psWinContex: OSD输出所使用的窗口信息
	nChannel :  选择范围从1到4，默认为channel1
	unBackgroundColor:字幕的背景色，默认为全黑，格式为ARGB
return:0代表成功，-1表示失败

   }}}USR */
int CSUSPCCExtStart( HCSHANDLE hCCExtHandle, CSTELEContext_S *psWinContex, int nChannel, int nBackgroundColor );

/* {{{USR
停止解析和显示从VIDEO PES中获取的CC数据
Parameter:
无
return:0代表成功，-1表示失败
    创建的句柄
   }}}USR */
int CSUSPCCExtStop( HCSHANDLE hCCExtHandle );

/* {{{USR
销毁CCExt的句柄
Parameter:
无
return:
    无
   }}}USR */
void CSUSPCCExtClose( void );


// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* USPTeletextInterface_H */
