#ifndef QTADAPTER_H
#define QTADAPTER_H


#include "udi2_osg.h"
#include "udi2_error.h"
#include "mmcp_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
QT窗口接口消息结构体

m_dwPara0: 输入设备来源类型
	参见udi2_input.h的CSUDIInputType_E
	目前仅支持遥控器和前面板
	
m_dwPara1: 输入按键状态类型
	参见udi2_input.h的CSUDIInputKeyStatus_E
	
m_dwPara2: 按键键值
	参见udi2_input.h中的键值定义
	
m_dwPara3: 用户数据usrInt
	目前暂无使用
*/
typedef struct  _QtEvent_S
{
	unsigned int m_dwPara0;	
  	unsigned int m_dwPara1;
	unsigned int m_dwPara2;
	unsigned int m_dwPara3;
}QtEvent_S;

/*
QT和udi消息映射结构体

m_nUDIKeyCode: udi2的键值
	参见udi2_input.h中的键值定义
	
m_nQTKeyCode: QT中的键值
	参见qnamespace.h
	
m_bSpecial: 该键值是否为特殊键值
	此处的特殊表示在发QT的postEvent消息时，text值是否为空
	目前已知的为上下左右四个键，为功能键，QT内部会做特殊处理
*/
typedef struct  _QtUdiKeyMapElement_S
{
	unsigned int m_nUDIKeyCode;	
  	unsigned int m_nQTKeyCode;
	BOOL 		 m_bSpecial;
}QtUdiKeyMapElement_S;



typedef	void *	CSQT_HANDLE;

CSUDI_Error_Code QTMWInit(void);

CSUDI_Error_Code QTMWCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat,int nWidth, int nHeight, CSQT_HANDLE * phSurface);

CSUDI_Error_Code QTMWDestroySurface(CSQT_HANDLE hSurface);

CSUDI_Error_Code QTMWGetSurfaceInfo(CSQT_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

CSUDI_Error_Code QTMWSetSurfaceInfo(CSQT_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

CSUDI_Error_Code QTMWGetDisplaySurface(int nIndex, CSQT_HANDLE * phSurface);

CSUDI_Error_Code QTMWBlit(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,CSQT_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode);

CSUDI_Error_Code QTMWFill(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel);

CSUDI_Error_Code QTMWSync(void);

CSUDI_Error_Code QTMWHideDisplay(void);

CSUDI_Error_Code QTMWShowDisplay(int nX, int nY, int nWidth, int nHeight);

CSUDI_Error_Code QTMWDispatchFilterKeyEvent(QtEvent_S sEvent);
CSUDI_Error_Code QTMWRaiseWidget(BOOL isRaise);

CSUDI_Error_Code QTMWSoftRefresh(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, const CSUDIOSGRect_S * psSrcRect);
void QTMWWidgetOnBrowser(HCSHANDLE hWebview, BOOL bOnBrowser);

/**
@brief  功能描述:		
	在 webview 获取2D Canvas画布大小，实现2D和浏览器同时画图/共存
@param[in] 
    hWebivew : 2D所要画的webivew
	x, y, w, h: 2D Canvas画布大小
@exception
@note  
	只在浏览器 GUI中使用，只能2D中使用, 退出2D时必须调用QTMWSet2DCanvasRect将画布大小设成0
*/
void QTMWSet2DCanvasRect(HCSHANDLE hWebivew, int x, int  y,  int width,  int height);
/**
@brief  功能描述:
        添加QT向JAVA发键值的键值map表

@param[in]
int(* pnKeyTableAddr)[2]    二维键值表指针
int nFirstDimension         键值表映射键值的个数

@return
int         如果键值表指针为空或者传入映射键值个数值小于等于0， 返回 0
            保存键值表成功，                                    返回 键值表保存映射键值的个数

@exception
@note
*/
int CSJavaMapKeyTable(int(* pnKeyTableAddr)[2], int nFirstDimension);

/**
@brief  功能描述:
        从键值表中取出QT向JAVA发按键的经过映射过的键值

@param[in]
int nKey    映射前的键值

@return
int         映射表保存的键值个数为 0，  返回   0
            传入的值在映射表中不存在，  返回   0
            成功映射键，                返回   映射后的键值

@exception
@note
*/
int CSJavaMapKey(int nKey);

int qtadaptermain(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
#endif
