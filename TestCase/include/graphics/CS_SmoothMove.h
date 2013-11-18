
#ifndef _CS_SMOOTHMOVE_H
#define _CS_SMOOTHMOVE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _CSSmoothMoveMsgType
{
	SMOOTHMOVE_MSG_HIDE, //走马灯隐藏消息
	SMOOTHMOVE_MSG_STOP, //走马灯停止消息
	SMOOTHMOVE_MSG_OVER  //走马灯滚动完毕消息
}CSSmoothMoveMsgType;

typedef void (*CSSmoothMoveCallback)(CSSmoothMoveMsgType emSmoothMoveMsgType, PVOID pvData);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSmoothMoveRegisterCallback
* 函数功能：注册走马灯回调函数
* 参数说明：
*		hSmoothMove：【IN】走马灯实例的句柄
*		pfnCallback：【IN】回调函数指针
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSmoothMoveRegisterCallback(HCSHANDLE hSmoothMove, CSSmoothMoveCallback pfnCallback);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSmoothMoveGetVersion
* 函数功能：获得走马灯库的版本号；
* 参数说明：
*		pszVersionBuf：【OUT】获得的版本号的首地址
*		dwBufSize：【IN】走马灯模块版本号的字符串长度
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSmoothMoveGetVersion(char *pszVersionBuf, DWORD dwBufSize);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGetSmoothMoveMinMemory
* 函数功能：获得开启一个走马灯实例需要的最小内存
* 参数说明：
*		nMaxInputStrLen：【IN】滚动字符串的最大长度
*		nStrWidht：【IN】字体宽度
*		nStrHeight：【IN】字体高度
* 返回值：
*		返回0
* 注意事项：
*		该接口为保留接口，返回值永远为０
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
int CSGetSmoothMoveMinMemory(int nMaxInputStrLen, int nStrWidht, int nStrHeight);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGetSmoothMoveMinMemoryEx
* 函数功能：获得开启一个走马灯实例需要的最小内存
* 参数说明：
*		nMaxInputStrLen：【IN】滚动字符串的最大长度
*		nStrWidht：【IN】字体宽度
*		nStrHeight：【IN】字体高度
* 返回值：
*		返回需要的最小内存
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
int CSGetSmoothMoveMinMemoryEx(int nMaxInputStrLen, int nStrWidth, int nStrHeight, DWORD dwOsdWidth, DWORD dwOsdHeight);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGetSmoothMoveMinMemoryEx2
* 函数功能：获得开启一个走马灯实例需要的最小内存
* 参数说明：
*		nMaxInputStrLen：【IN】滚动字符串的最大长度
*		nStrWidht：【IN】字体宽度
*		nStrHeight：【IN】字体高度
*		dwOsdWidth：【IN】走马灯显示区域宽度
*		dwOsdHeight：【IN】走马灯显示区域高度
*		nMoveModel：【IN】滚动方式（１-右往左单行，２－下往上，３－左往右，４－上往下，５－右往左多行）
* 返回值：
*		返回需要的最小内存
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
int CSGetSmoothMoveMinMemoryEx2(int nMaxInputStrLen, int nStrWidth, int nStrHeight, DWORD dwOsdWidth, DWORD dwOsdHeight, int nMoveModel);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSOpenSmoothMove
* 函数功能：开启一个走马灯实例
* 参数说明：
*		pucDataBuf：【OUT】滚动字符串的最大长度
*		dwDataBufLen：【IN】走马灯私有内存的大小
*		nMaxInputStrLen：【IN】最大需要显示的宽字节的个数
*		nStrWidth：【IN】字符显示的宽度
*		nStrHeight：【IN】字符显示的高度
*		pucShowBuf：【IN】显存地址
*		nOneLineBufLen：【IN】存储一行显示数据需要的字节数
*		nShowBits：【IN】几位色
*		dwOsdHeight：【IN】走马灯显示区域高度
*		dwBKColor：【IN】背景色RGBA值
*		dwTextColor：【IN】显示文字的颜色
*		hFont：【IN】字库实例句柄
*		dwStepTimeOut：【IN】刷新时间间隔
* 返回值：
*		返回走马灯实例句柄
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSOpenSmoothMove(PBYTE pucDataBuf, DWORD dwDataBufLen, int nMaxInputStrLen, int nStrWidth, int nStrHeight, PBYTE pucShowBuf, int nOneLineBufLen, int nShowBits, DWORD dwOsdHeight, DWORD dwBKColor, DWORD dwTextColor, HCSHANDLE hFont, DWORD dwStepTimeOut);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSOpenSmoothMoveEx
* 函数功能：开启一个走马灯实例扩展接口
* 参数说明：
*		pucDataBuf：【OUT】滚动字符串的最大长度
*		dwDataBufLen：【IN】走马灯私有内存的大小
*		nMaxInputStrLen：【IN】最大需要显示的宽字节的个数
*		nStrWidth：【IN】字符显示的宽度
*		nStrHeight：【IN】字符显示的高度
*		pbShowAddr：【IN】显存基址
*		nShowBits：【IN】几位色
*		dwBKColor：【IN】背景色RGBA值
*		dwTextColor：【IN】显示文字的颜色
*		hFont：【IN】字库实例句柄
*		dwStepTimeOut：【IN】刷新时间间隔
*		prcSmoothMove：【IN】走马灯显示区域
*		nMoveModel：【IN】走马灯滚动模式
*		nTaskPriority：【IN】走马灯实例优先级
* 返回值：
*		返回走马灯实例句柄
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSOpenSmoothMoveEx(PBYTE pucDataBuf, DWORD dwDataBufLen, int nMaxInputStrLen, int nStrWidth, int nStrHeight, PBYTE pbShowAddr, int nShowBits, DWORD dwBKColor, DWORD dwTextColor, HCSHANDLE hFont, DWORD dwStepTimeOut, CSRECT *prcSmoothMove, int nMoveModel, int nTaskPriority);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSCloseSmoothMove
* 函数功能：关闭走马灯实例
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSCloseSmoothMove(HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStartSmoothMove
* 函数功能：启动显示走马灯
* 参数说明：
*		dwShowTime：【IN】走马灯持续显示时间
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStartSmoothMove(DWORD dwShowTime, HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStartSmoothMoveEx
* 函数功能：启动显示走马灯
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
*		dwShowTime：【IN】走马灯持续显示时间
*		dwMoveTimes：【IN】滚动屏数
*		bOnePerPage：【IN】是否按屏滚动
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		如果设置了bOnePerPage 为TRUE，则走马灯按照设置滚动dwMoveTimes屏，用户设置的滚动时间不起作用;
*		如果设置bOnePerPage 为FALSE，则走马灯滚动的时间为dwShowTime,用户设置的滚动屏数dwShowTime不起作用。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStartSmoothMoveEx(HCSHANDLE hSmoothMove, DWORD dwShowTime, DWORD dwMoveTimes, BOOL bOnePerPage);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStopSmoothMove
* 函数功能：走马灯停止显示
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStopSmoothMove(HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowSmoothMove
* 函数功能：Hide之后重新恢复显示
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowSmoothMove(HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSHideSmoothMove
* 函数功能：隐藏走马灯显示
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSHideSmoothMove(HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetInputStr
* 函数功能：设置走马灯显示字符串
* 参数说明：
*		pcInputStr：【IN】显示内容的指针
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetInputStr(const char* pcInputStr, HCSHANDLE hSmoothMove);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetInputStrEx
* 函数功能：设置走马灯显示字符串
* 参数说明：
*		pcInputStr：【IN】显示内容的指针
*		hSmoothMove：【IN】走马灯实例句柄
*		nchStrLen：【IN】 字符串长度，大小为用户设置字符串的长度
*	  　nSpaceLen：【IN】字符串间空格数
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetInputStrEx(HCSHANDLE hSmoothMove, const char* pcInputStr, int nchStrLen, int nSpaceLen);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSClearInputStr
* 函数功能：清除显示内容，并且刷新显示区域
* 参数说明：
*		hSmoothMove：【IN】走马灯实例句柄
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSClearInputStr(HCSHANDLE hSmoothMove);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetOneStepPixel
* 函数功能：设置每步滚动的像素个数
* 参数说明：
*		hSmoothMove【IN】：走马灯实例句柄
*		nOneStepPixel【IN】：每步滚动的像素个数
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetOneStepPixel(HCSHANDLE hSmoothMove, int nOneStepPixel);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetSmoothFirstShowMode
* 函数功能：设置每步滚动的像素个数
* 参数说明：
*		hSmoothMove【IN】：走马灯实例句柄
*		bShowAllAtFirst【IN】：开始显示的时候是否显示全屏
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetSmoothFirstShowMode( HCSHANDLE hSmoothMove, BOOL bShowAllAtFirst );

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetSmoothCharInterSpaceAndLineSpace
* 函数功能：设置每步滚动的像素个数
* 参数说明：
*		hSmoothMove【IN】：走马灯实例句柄
*		nCharInterSpace【IN】: 字符间距
*	    nLineSpace【IN】:      字符行距
* 返回值：
*		成功，返回TRUE
*		失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetSmoothCharInterSpaceAndLineSpace( HCSHANDLE hSmoothMove, int nCharInterSpace, int nLineSpace );

#ifdef __cplusplus
}
#endif

#endif

