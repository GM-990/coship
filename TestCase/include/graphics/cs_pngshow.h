#ifndef __CS_SHOWPNG_H
#define __CS_SHOWPNG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "CS_Windows.h"

/*================================================================
* 函数名：CSGetPNGVersion
* 函数功能：获得PNG库的版本号；
* 参数说明：
*		pszVer：【OUT】获得的版本号的首地址
*		nVerLen：【IN】PNG版本号的字符串长度
* 返回值：
*		无
* 注意事项：
*		无
================================================================*/
void CSGetPNGVersion(char* pszVer, int nVerLen);


/*================================================================
* 函数名：CSInitPNG
* 函数功能：初始化PNG解码所需要的内存空间；
* 参数说明：
*		pucShowPNGMemory：【IN】解析PNG图片需要的内存块起始地址
*		dwMemorySize：【IN】解析PNG图片需要的内存块的大小
* 返回值：
*		TRUE：初始化成功
*		FALSE：初始化失败
* 注意事项：
*		调用显示PNG图片显示的第一步
================================================================*/
BOOL CSInitPNG(PBYTE pucShowPNGMemory, DWORD dwMemorySize);

BOOL CSTermPNG(void);
	
/*================================================================
* 函数名：CSShowPNG
* 函数功能：显示PNG图片；
* 参数说明：
*		pucScreenAddr：【IN】显存地址
*		pucPNGDataBuf：【IN】PNG图片数据的起始地址
*		dwPNGDataLen：【IN】PNG图片数据的大小
*		pstShowRect：【IN】自定义PNG图片显示区域指针
*		dwBKColor:【IN】用户自定义PNG背景颜色，（仅在PNG图片未设置图片背景颜色时有效）
*		bActPicSize：【IN】自定义显示区域时为FALSE，默认显示区域时设为TRUE
* 返回值：
*		TRUE：显示成功
*		FALSE：显示失败
================================================================*/
BOOL CSShowPNG(PBYTE pucScreenAddr, PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, CSRECT *pstShowRect, DWORD dwBKColor, BOOL bActPicSize);


/*================================================================
* 函数名：CSShowPNGFile
* 函数功能：显示以COSHIP RMFS文件系统文件存放的PNG图片
* 参数说明：
*		pucScreenAddr：【IN】显存地址
*		lpFileName：【IN】PNG文件的文件名
*		pstShowRect：【IN】自定义PNG图片显示区域
*		dwBKColor:【IN】用户自定义PNG背景颜色，（仅在PNG图片未设置图片背景颜色时有效）
*		bActPicSize：【IN】自定义显示区域时为FALSE，默认显示区域时设为TRUE
* 返回值：
*		TRUE：显示成功
*		FALSE：显示失败
================================================================*/
BOOL CSShowPNGFile(PBYTE pucScreenAddr, char* lpFileName, CSRECT *pstShowRect, DWORD dwBKColor, BOOL bActPicSize);


/*================================================================
* 函数名：CSDecodePNG
* 函数功能：解码PNG图片，并将解码后的数据保存在用户指定的内存中
* 参数说明：
*		pucBitBuf：【IN】保存解码后PNG图片数据的内存起始地址
*		pucPNGDataBuf：【IN】PNG图片数据起始地址
*		dwPNGDataLen：【【IN】PNG图片数据大小
*		dwColorDepth：【IN】显示位深度，如16位，32位
* 返回值：
*		TRUE：解码成功
*		FALSE：解码失败
* 注意事项：
*		仅对一幅PNG图片有效
================================================================*/
BOOL CSDecodePNG(PBYTE pucBitBuf, PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, DWORD dwColorDepth);


/*================================================================
* 函数名：CSDecodePNGFile
* 函数功能：解码文件系统中的PNG图片，并将解码后的数据保存在用户指定的内存中
* 参数说明：
*		pucBitBuf：【IN】保存解码后PNG图片数据的内存起始地址
*		lpFileName：【IN】PNG图片在文件系统中的名称
*		dwColorDepth：【IN】显示位深度，如16位，32位
* 返回值：
*		TRUE：解码成功
*		FALSE：解码失败
* 注意事项：
*		仅对一幅PNG图片有效
================================================================*/
BOOL CSDecodePNGFile(PBYTE pucBitBuf, LPCTSTR lpFileName, DWORD dwColorDepth);


/*================================================================
* 函数名：CSGetPNGInfo
* 函数功能：获得PNG图片信息
* 参数说明：
*		pucPNGDataBuf：【IN】PNG图片数据起始地址
*		dwPNGDataLen：【IN】PNG图片数据大小
*		pnWidth：【OUT】PNG图片的宽度（以象素为单位）
*		pnHeight：【OUT】PNG图片的高度（以象素为单位）

* 返回值：
*		TRUE：获取成功
*		FALSE：获取失败
* 注意事项：
*		并必须首先调用CSInitPNG
================================================================*/
BOOL CSGetPNGInfo(PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, DWORD* pdwWidth, DWORD* pdwHeight);

#ifdef __cplusplus
}
#endif

#endif

