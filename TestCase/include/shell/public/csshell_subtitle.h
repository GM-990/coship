#ifndef CSSHELL_SUBTITLE_H
#define CSSHELL_SUBTITLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief subtitle矩形区域结构定义 */
typedef struct _SubtitleRect
{
    int m_nX;                ///< 矩形区域左上角横坐标 
    int m_nY;                ///< 矩形区域左上角纵坐标 
	int m_nWidth;            ///< 矩形区域宽度 > 0
	int m_nHeight;           ///< 矩形区域高度 > 0
}SUBTITELRECT;

/**
@brief 用户设置subtitle矩形显示区域的回调，该回调同时作为输入输出，在显示和清除subtitle数据的过程中调用
@param[in] 码流中解析出的subtitle矩形显示区域的数据信息
@param[out] 经过用户处理后的subtitle矩形显示区域的数据信息
@notes
    用户可以按照此方法缩放图片的大小并水平居中    720 / 576 =5/4 
 	subtitleRect.m_nY = (5*psSubtitlePar->m_dwSrcY)/4;
	subtitleRect.m_nHeight  = (5*psSubtitlePar->m_dwSrcHeight)/4;
	subtitleRect.m_nWidth = (5*psSubtitlePar->m_dwSrcWidth)/4;
	subtitleRect.m_nX = (1280-subtitleRect->m_dwSrcWidth)/2;

*/
typedef void (* CSSubtitleTranslateRect_F)(SUBTITELRECT* subtitleRect);

/*@brief subtitle初始化参数*/
typedef struct _CSShellSubtitleInitParam
{
	int 	m_nSize;			///<本结构体大小，必须为sizeof(CSShellSubtitleInitParam_S)
	void*	m_pvUserData;		///<用户数据，目前暂未使用
	
	BYTE*	m_pvMemAddr;		///<subtitle内存池的地址
	int		m_nMemSize;			///< 内存池大小	
	
	int 	m_nThreadPriority;	///<线程优先级
	int		m_nStackSize;		///<线程栈大小
	CSSubtitleTranslateRect_F m_fnSubtitleRect; ///<subtitle矩形区域设置回调

	BYTE*	m_pvDCIIMemAddr;	///<dcii subtitle内存池的地址
	int		m_nDCIIMemSize;		///<dcii subtitle内存池大小

	int   m_nDemuxID_PVR;    //录制时启动subtitle、时移播放和文件回放时启动subtitle采用的demux ID，此demux需支持注入
}CSShellSubtitleInitParam_S;

/**
@brief 设置subtitle初始化参数
@param [in] psSubtitle 初始化参数结构体实例
@return 成功返回TRUE,否则返回FALSE
@note 
*/
BOOL CSShellSubtitleSetParam(const CSShellSubtitleInitParam_S* psSubtitle);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

