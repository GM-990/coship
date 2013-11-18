

#ifndef _CS_GIFSHOW_H_
#define _CS_GIFSHOW_H_

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef GIF_USE_RMFS
//#define GIF_USE_RMFS
//#endif
#include "CS_Windows.h"

typedef enum _CSGifMsgType
{
	GIF_MSG_FIRST_FIRSTFRAME_FINISH,//第一帧第一次解析完抛出消息
	GIF_MSG_ONEFRAME_FINISH,   //一帧解析完后抛出消息
	GIF_MSG_ALLOC_MEMORY, //分配内存
	GIF_MSG_FREE_MEMORY  //释放内存
}CSGifMsgType;

typedef struct 
{
	int  m_nSize;
	unsigned char **pBuf;
}CSCACHE_IMAGE;
/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 结构名：ShowGifPar
* 结构说明：用于调用GIF模块显示以及解码函数时需要设置的一些参数信息
* 成员说明：
*		nBitCount：显示位深度
*		rect：显示区域，如果将GIF图片解码到内存中，则区域设置为全0
*		m_nShowBuffWidth：指定宽度用于解码GIF图片部分或者全部宽度范围内的数据
*		m_nShowBuffHeight;  指定高度用于解码GIF图片部分或者全部高度范围内的数据
*		m_pucDoubleBuf;	缓存地址
*		m_pucScreenBuf;	显存地址
* 注意事项：
*		1.rect仅当用于显示GIF时设置，如果仅仅是对GIF图片进行解码而不是显示，rect的成员全部应该设置为0；
*		2.m_nShowBuffWidth和m_nShowBuffHeight仅当对GIF图片进行解码时候设置，如果是对GIF图片进行显示，则它们都应该设置为0；
*		3.m_pucDoubleBuf和m_pucScreenBuf都设置时，GIF图片显示时将采用m_pucScreenBuf；
*		4.m_pucDoubleBuf和m_pucScreenBuf可以设置为用于保存GIF图片解码数据的内存起始地址，这时其余成员的设定请按照1和2进行设置。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
typedef struct __ShowGifPar__
{
	DWORD  nBitCount;  

	CSRECT rect;	  

	int m_nShowBuffWidth;  
	int m_nShowBuffHeight;  

	PBYTE m_pucDoubleBuf;	
	PBYTE m_pucScreenBuf;	

}ShowGifPar;


//显示GIF的显示策略
typedef enum _DISPLAYMODE
{
	SINGLE_MEMORY,	//单缓存
	DOUBLE_MEMORY	//双缓存

}GIF_DISPLAY_MODE;

typedef void (*CSGifCallback)(CSGifMsgType emGifMsgType, PVOID pvData);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGifRegisterCallback
* 函数功能：注册GIF回调函数
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄
*		pfnCallback：【IN】回调函数指针
* 返回值：
*		成功，返回TRUE
*       失败，返回FALSE
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSGifRegisterCallback(HCSHANDLE hShowGif, CSGifCallback pfnCallback);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGetGifVersion
* 函数功能：获得GIF库的版本号；
* 参数说明：
*		pszVer：【OUT】获得的版本号的首地址
*		nVerLen：【IN】GIF版本号的字符串长度
* 返回值：
*		无
* 注意事项：
*		无
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
void CSGetGifVersion( char* pszVer, int nVerLen );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSGifSetDisplayProperty
* 函数功能：设置显示GIF采用的显示策略，单缓还是双缓；
* 参数说明：
*		enmDisplayMode：【IN】显示策略
*		pucDoubleMemAddr：【IN】双缓存的内存地址，仅对双缓存策略有效
*		nMemSize：【IN】缓存的大小
* 返回值：
*		TRUE：设置成功
*		FALSE：设置失败
* 注意事项：
*		此函数在创建GIF句柄之前调用，设置GIF的显示策略，如果没有调用此函数，默认为单缓存策略；
*		如果采用单缓存策略，则第二个参数传NULL即可，第三个参数可以传入0
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSGifSetDisplayProperty( GIF_DISPLAY_MODE enmDisplayMode, BYTE* pucDoubleMemAddr, int nMemSize );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSCreateGifHandle
* 函数功能：创建显示GIF图片的句柄；
* 参数说明：
*		pucMemory：【IN】解析GIF图片需要的内存块起始地址
*		dwMemoryLen：【IN】解析GIF图片需要的内存块的大小
* 返回值：
*		GIF图片的句柄
* 注意事项：
*		调用显示GIF图片显示的第一步
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSCreateGifHandle( PBYTE pucMemory, DWORD dwMemoryLen );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSCreateGifHandle
* 函数功能：创建显示GIF图片的句柄；
* 参数说明：
*		pucMemory：【IN】解析GIF图片需要的内存块起始地址
*		dwMemoryLen：【IN】解析GIF图片需要的内存块的大小
nTaskPriority：	【IN】当GIF为多桢画面时，创建任务的优先级
* 返回值：
*		GIF图片的句柄
* 注意事项：
*		调用显示GIF图片显示的第一步
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSCreateGifHandleEx( PBYTE pucMemory, DWORD dwMemoryLen, int nTaskPriority );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSDestroyGifHandle
* 函数功能：销毁显示GIF图片的句柄；
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄
* 返回值：
*		TRUE：销毁成功
*		FALSE：销毁失败
* 注意事项：
*		GIF图片显示完后，调用此销毁销毁GIF句柄，释放解析GIF需要的内存；
*		如果是多帧的GIF图片，如果销毁后，则停留在最后显示的一帧上，如果要显示GIF动画，则要为该GIF分配独立的一块
*		内存，不能够销毁，对于只有一帧的GIF图片，则显示完成后可以销毁解析GIF的内存块
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSDestroyGifHandle( HCSHANDLE hShowGif );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowGif
* 函数功能：显示GIF图片；
* 参数说明：
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
* 返回值：
*		对于单帧GIF图片，该函数返回1，
*      对于多帧GIF图片，该函数返回当前显示多帧GIF图片的句柄，
*      对于非法输入，该函数返回0。
* 注意事项：
*		要在创建GIF句柄以后调用；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSShowGif( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowGifEx
* 函数功能：显示GIF图片；
* 参数说明：
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
*		dwTransparentColor：【IN】用户指定的颜色用于替换GIF图片中的透明色
* 返回值：
*		对于单帧GIF图片，该函数返回1，
*      对于多帧GIF图片，该函数返回当前显示多帧GIF图片的句柄，
*      对于非法输入，该函数返回0。
* 注意事项：
*		要在创建GIF句柄以后调用；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSShowGifEx(PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowGifFile
* 函数功能：显示GIF图片；
* 参数说明：
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
* 返回值：
*		对于单帧GIF图片，该函数返回1，
*      对于多帧GIF图片，该函数返回当前显示多帧GIF图片的句柄，
*      对于非法输入，该函数返回0。
* 注意事项：
*		要在创建GIF句柄以后调用；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSShowGifFile( LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowGifFileEx
* 函数功能：显示GIF图片；
* 参数说明：
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
*		dwTransparentColor：【IN】用户指定的颜色用于替换GIF图片中的透明色
* 返回值：
*		对于单帧GIF图片，该函数返回1，
*      对于多帧GIF图片，该函数返回当前显示多帧GIF图片的句柄，
*      对于非法输入，该函数返回0。
* 注意事项：
*		要在创建GIF句柄以后调用；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
HCSHANDLE CSShowGifFileEx(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStopOneGif
* 函数功能：停止显示某GIF动画；
* 参数说明：
*		hOneGifShow：【IN】由CSShowGif返回某GIF动画显示句柄
*		hShowGif：【IN】GIF图片显示的句柄
* 返回值：
*		TRUE：停止成功
*		FALSE：停止失败
* 注意事项：
*		要在创建GIF句柄以后调用；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStopOneGif(HCSHANDLE hOneGifShow, HCSHANDLE hShowGif);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStartShowGif
* 函数功能：重新启动显示GIF图片，仅仅对于多帧的GIF动画有效果；
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄
* 返回值：
*		TRUE：启动成功
*		FALSE：启动失败
* 注意事项：
*		无；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStartShowGif( HCSHANDLE hShowGif );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSStopShowGif
* 函数功能：停止显示GIF图片，仅仅对于多帧的GIF动画有效果；
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄
* 返回值：
*		TRUE：停止多帧GIF动画显示成功
*		FALSE：停止多帧GIF动画显示失败
* 注意事项：
*		无；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSStopShowGif( HCSHANDLE hShowGif );


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowOneFrameGif
* 函数功能：重新启动显示GIF图片，对于多帧的GIF动画仅显示第一页；
* 参数说明：
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
* 返回值：
*		TRUE：启动成功
*		FALSE：启动失败
* 注意事项：
*		无；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowOneFrameGif( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowOneFrameGifFile
* 函数功能：重新启动显示GIF图片，对于多帧的GIF动画仅显示第一页；
* 参数说明：
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
*		dwTransparentColor：【IN】用户指定的颜色用于替换GIF图片中的透明色
* 返回值：
*		TRUE：启动成功
*		FALSE：启动失败
* 注意事项：
*		无；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowOneFrameGifFile(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSShowOneFrameGifEx
* 函数功能：重新启动显示GIF图片，对于多帧的GIF动画仅显示第一页；
* 参数说明：
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		hShowGif：【IN】GIF图片显示的句柄
*		pShowGifPar：【IN】GIF图片需要配置的参数
*		dwTransparentColor：【IN】用户指定的颜色用于替换GIF图片中的透明色
* 返回值：
*		TRUE：启动成功
*		FALSE：启动失败
* 注意事项：
*		无；
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowOneFrameGifEx( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSSetGifClipRectList
*函数功能：
*		设置GIF图片刷新时不画的矩形列表
*参数说明：
*		hShowGif   :【IN】GIF图片显示的句柄
*		pCSRECTClipRectList :【IN】指向矩形列表的指针
*		nRectListNum:【IN】矩形列表的矩形个数
*返回值：
*		成功，返回TRUE
*		失败，返回FALSE
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetGifClipRectList(HCSHANDLE hShowGif, CSRECT *pCSRECTClipRectList, int nRectNum);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSClearGifClipRectList
*函数功能：
*		清除GIF图片刷新时不画的矩形列表
*参数说明：
*		hShowGif:【IN】GIF图片显示的句柄
*返回值：
*		成功，返回TRUE
*		失败，返回FALSE
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSClearGifClipRectList(HCSHANDLE hShowGif);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSShowNextFrameOfOneGif
*函数功能：
*		立即显示一幅GIF图片的下一祯,仅对多祯GIF动画有效
*参数：
*		hShowGif: 【IN】GIF图片显示的句柄
*		hOneGifHandle:【IN】由CSShowGif返回的某GIF动画显示句柄
*返回值：
*		成功，返回TRUE
*		失败，返回FALSE
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowNextFrameOfOneGif(HCSHANDLE hShowGif, HCSHANDLE hOneGifShow);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSShowNextFrameOfAllGif
*函数功能：
*		立即显示所有GIF图片的下一祯,仅对多祯GIF动画有效
*参数：
*		hShowGif: 【IN】GIF图片显示的句柄
*返回值：
*		成功，返回TRUE
*		失败，返回FALSE
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSShowNextFrameOfAllGif(HCSHANDLE hShowGif);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSGetGifPicInfo
*函数功能：
*		获得GIF图片的信息，包括是否使用透明色、图片宽度、图片高度、以及每祯间的延时
*参数：
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		pbUseTransparent: 【OUT】是否使用了透明色
*		pdwGifWidth: 【OUT】GIF图片的宽度
*		pdwGifHeight: 【OUT】GIF图片的高度
*		pdwDelayTime: 【OUT】GIF图片各祯间延时
*返回值：
*		FALSE:获取失败
*		TRUE:获取成功
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSGetGifPicInfo(PBYTE pucGifData, DWORD dwGifDataLen, BOOL *pbUseTransparent, DWORD *pdwGifWidth, DWORD *pdwGifHeight, DWORD *pdwDelayTime);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSGetGifFileInfo
*函数功能：
*		获得以RMFS文件系统文件形式GIF图片的信息，包括是否使用透明色、图片宽度、图片高度、以及每祯间的延时
*参数：
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*		pbUseTransparent: 【OUT】是否使用了透明色
*		pdwGifWidth: 【OUT】GIF图片的宽度
*		pdwGifHeight: 【OUT】GIF图片的高度
*		pdwDelayTime: 【OUT】GIF图片各祯间延时
*返回值：
*		FALSE:获取失败
*		TRUE: 获取成功
*注意事项：
*		该接口为cs_image模块添加，应用请不要使用
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSGetGifFileInfo(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, BOOL *pbUseTransparent, DWORD *pdwGifWidth, DWORD *pdwGifHeight, DWORD *pdwDelayTime);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSGetGifFrameCount
*函数功能：
*		获取GIF图片的帧数
*参数：
*		hShowGif: 【IN】GIF图片显示的句柄
*		pucGifData：【IN】GIF图片数据的起始地址
*		nGifDataLen：【IN】GIF图片数据的大小
*返回值：
*		GIF图片的帧数，如果为0则表示不是一幅GIF图片
* 注意事项：
*		该接口为cs_image模块添加，应用请不要使用
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
int CSGetGifFrameCount(HCSHANDLE hShowGif, PBYTE pucGifData, int nGifDataLen);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
*函数名称：CSGetGifFileFrameCount
*函数功能：
*		获取以RMFS文件系统文件形式保存的GIF图片的帧数
*参数：
*		hShowGif: 【IN】GIF图片显示的句柄
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*返回值：
*		GIF图片的帧数，如果为0则表示不是一幅GIF图片
* 注意事项：
*		该接口为cs_image模块添加，应用请不要使用
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
int CSGetGifFileFrameCount(HCSHANDLE hShowGif, LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSDecodeGif
* 函数功能：将GIF图片各帧解码到用户指定的内存中
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄】
*		pucBitBuf：【IN】保存GIF图片解码后数据的内存起始地址
*		dwBitBufLen：【IN】保存GIF图片解码后数据的内存大小
*		dwFrameCount：【IN】GIF图片数据帧数
*		pucGifData：【IN】GIF图片数据的起始地址
*		dwGifDataLen：【IN】GIF图片数据的大小
*		dwColorDepth：【IN】颜色模式：如16位、32位色
* 返回值：
*		解码成功返回TRUE，否则返回FALSE
* 注意事项：
*		该接口为cs_image模块添加，应用请不要使用
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSDecodeGif(HCSHANDLE hShowGif, PBYTE pucBitBuf, DWORD dwBitBufLen, DWORD dwFrameCount, PBYTE pucGifData, DWORD dwGifDataLen, DWORD dwColorDepth);


/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSDecodeGifFile
* 函数功能：将以RMFS文件系统文件形式保存的GIF图片各帧解码到用户指定的内存中
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄】
*		pucBitBuf：【IN】保存GIF图片解码后数据的内存起始地址
*		dwBitBufLen：【IN】保存GIF图片解码后数据的内存大小
*		nFrameCount：【IN】GIF图片数据帧数
*		lpFileName：【IN】GIF文件名（COSHIP RMFS）
*		pucFileBufMemory：【IN】GIF图片数据的缓冲区地址
*		dwFileBufMemoryLen：【IN】GIF图片数据的缓冲区大小
*		dwColorDepth：【IN】颜色模式：如16位、32位色
* 返回值：
*		解码成功返回TRUE，否则返回FALSE
* 注意事项：
*       该接口为cs_image模块添加，应用请不要使用
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSDecodeGifFile(HCSHANDLE hShowGif, PBYTE pucBitBuf, DWORD dwBitBufLen, DWORD dwFrameCount, LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, DWORD dwColorDepth);

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 函数名：CSSetBkShowFlag
* 函数功能：将以RMFS文件系统文件形式保存的GIF图片各帧解码到用户指定的内存中
* 参数说明：
*		hShowGif：【IN】GIF图片显示的句柄】
*		bBkFlag：【IN】中山项目中透明GIF图片显示问题修改，默认设置为TRUE，无论背景是否透明，都需要重新绘制背景，FALSE则如果是透明背景，则不绘制背景色；
* 返回值：
*		成功设置返回TRUE,否则返回FALSE
* 注意事项：
*       该接口为中山项目N5200项目增加，其他项目尽量不要使用该接口
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
BOOL CSSetBkShowFlag(HCSHANDLE hShowGif, BOOL bBkFlag);

#ifdef __cplusplus
}
#endif

#endif

