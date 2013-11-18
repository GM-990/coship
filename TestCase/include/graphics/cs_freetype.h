#ifndef CS_FREETYPE_H
#define CS_FREETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	字型：
 *	FONTTYPE_BOLD：粗体
 *	FONTTYPE_ITALIC	：斜体
 *	FONTTYPE_AUTOHINT：自动调整
 *	FONTTYPE_GRAYRENDER：防锯齿处理
 *	
 */
#define FONTTYPE_DEFAULT	(0x0000)
#define FONTTYPE_BOLD		(0x0001)
#define FONTTYPE_ITALIC		(0x0002)
#define FONTTYPE_AUTOHINT	(0x0004)
#define FONTTYPE_GRAYRENDER	(0x0008)

/*
 * 函数名：CSVFGetVersion
 * 函数功能：获得库的版本号；
 * 参数说明：
 *		pcVer：【OUT】版本号缓冲区首地址
 *		nVerLen：【IN】版本号缓冲区长度
 * 返回值：
 *		无
 * 注意事项：
 *		无
 */
void CSVFGetVersion(char* pcVer, int nVerLen);

/*
 * 函数名：CSVFInit
 * 函数功能：初始化Vector Font 库；
 * 参数说明：
 *		pucMemory：【IN】用于Vector Font模块的内存空间首地址
 *		nMemoryLen：【IN】用于Vector Font模块的内存空间首地址
 * 返回值：
 *		TRUE：初始化成功
 *		FALSE：初始化失败
 * 注意事项：
 *		无
 */
BOOL CSVFInit(PBYTE pucMemory, int nMemoryLen);

/*
 * 函数名：CSVFOpenFont
 * 函数功能：打开一个字库；
 * 参数说明：
 *		pucDataBuf：【IN】字库数据（即TTF文件的二进制数据）的首地址
 *		dwDataLen：【IN】字库数据的长度
 * 返回值：
 *		打开成功则返回一个句柄
 *		失败返回NULL
 * 注意事项：
 *		必须在CSVFInit后调用
 */
HCSHANDLE CSVFOpenFont(PBYTE pucDataBuf, DWORD dwDataLen);

/*
 * 函数名：CSVFCloseFont
 * 函数功能：关闭一个字库；
 * 参数说明：
 *		hFont：【IN】要关闭字库的句柄
 * 返回值：
 *		TRUE：初始化成功
 *		FALSE：初始化失败
 * 注意事项：
 *		无
 */
BOOL CSVFCloseFont(HCSHANDLE hFont);

/*
 * 函数名：CSVFSetResolutions
 * 函数功能：设置设备分辨率；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		wXResolution：【IN】水平分辨率
 *		wYResolution：【IN】垂直分辨率
 * 返回值：
 *		TRUE：初始化成功
 *		FALSE：初始化失败
 * 注意事项：
 *		默认值为96，必须在CSVFOpenFont后调用
 */
BOOL CSVFSetResolutions(HCSHANDLE hFont, WORD wXResolution, WORD wYResolution);

/*
 * 函数名：CSVFSetCharSizes
 * 函数功能：设置字库中的字体大小；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		dCharWidth：【IN】字体宽度
 *		dCharHeight：【IN】字体高度
 * 返回值：
 *		TRUE：初始化成功
 *		FALSE：初始化失败
 * 注意事项：
 *		必须在CSVFOpenFont后调用
 */
BOOL CSVFSetCharSizes(HCSHANDLE hFont, double dCharWidth, double dCharHeight);

/*
 * 函数名：CSVFGetCharSizes
 * 函数功能：获取一个字符的宽度和高度；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		pwCharWidth：【OUT】返回字符串宽度
 *		pwCharHeight：【OUT】返回字符串高度
 *		wCharID：【IN】字符的Unicode编码
 *		dwFontType：【IN】字型
 * 返回值：
 *		TRUE：初始化成功
 *		FALSE：初始化失败
 * 注意事项：
 *		必须在CSVFOpenFont后调用
 */
BOOL CSVFGetCharSizes(HCSHANDLE hFont, WORD* pwCharWidth, WORD* pwCharHeight, WORD wCharID, DWORD dwFontType);

/*
 * 函数名：CSVFGetCharFontData
 * 函数功能：获取一个字符串的点阵；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		pucDataBuf：【IN/OUT】获取点阵的Buf
 *		dwDataBufLen：【IN】获取点阵的Buf长度
 *		wCharID：【IN】字符的Unicode编码
 *		dwFontType：【IN】字型
 *		nOffX：【IN】X方向上的偏移
 *		nOffY：【IN】Y方向上的偏移
 *		nOneLineWidth：【IN】每行字的宽度
 * 返回值：
 *		TRUE：获取成功
 *		FALSE：获取失败
 * 注意事项：
 *		必须在CSVFOpenFont后调用
 */
BOOL CSVFGetCharFontData(HCSHANDLE hFont, PBYTE pucDataBuf, DWORD dwDataBufLen, WORD wCharID, DWORD dwFontType, int nOffX, int nOffY, int nOneLineWidth);

/*
 * 函数名：CSVFGetAdvanceX
 * 函数功能：获取一个字的步进宽度；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		wCharID：【IN】字符的Unicode编码
 *		dwFontType：【IN】字型
 * 返回值：
 *		返回步进宽度
 *		失败返回-1
 * 注意事项：
 *		必须在CSVFOpenFont后调用
 */
int CSVFGetAdvanceX(HCSHANDLE hFont, WORD wCharID, DWORD dwFontType);

/*
 * 函数名：CSVFGetAdvanceY
 * 函数功能：获取一个字的步进高度；
 * 参数说明：
 *		hFont：【IN】字库句柄
 *		wCharID：【IN】字符的Unicode编码
 *		dwFontType：【IN】字型
 * 返回值：
 *		返回步进高度
 *		失败返回-1
 * 注意事项：
 *		必须在CSVFOpenFont后调用
 */
int CSVFGetAdvanceY(HCSHANDLE hFont, WORD wCharID, DWORD dwFontType);

/*
 * 函数名：CSVFGetFontAsent
 * 函数功能：获取一个字库的上行高度；
 * 参数说明：
 *		hFont：【IN】字库句柄
 * 返回值：
 *		返回上行高度
 *		失败返回-1
 * 注意事项：
 *		必须在CSVFOpenFont后调用，一般为正值
 */
int CSVFGetFontAsent(HCSHANDLE hFont);

/*
 * 函数名：CSVFGetFontDesent
 * 函数功能：获取一个字库的下行高度；
 * 参数说明：
 *		hFont：【IN】字库句柄
 * 返回值：
 *		返回下行高度
 *		失败返回-1
 * 注意事项：
 *		必须在CSVFOpenFont后调用，一般为负值
 */
int CSVFGetFontDesent(HCSHANDLE hFont);

/*
 * 函数名：CSVFGetLastError
 * 函数功能：获取最后一个错误；
 * 参数说明：
 *		无
 * 返回值：
 *		无
 * 注意事项：
 *		无
 */
int CSVFGetLastError(void);

#ifdef __cplusplus
}
#endif

#endif /* CS_VECTORFONT_H */

/* END */
