
#ifndef _CS_JPGSHOW_H
#define _CS_JPGSHOW_H

#include "CS_Windows.h"

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef JPG_USE_RMFS
//#define JPG_USE_RMFS
//#endif

/*＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
* 结构名：ShowJPGPar
* 结构说明：用于调用JPG模块显示以及解码函数时需要设置的一些参数信息
* 成员说明：
*		m_pShowBuff;	用于保存JPEG图片解码后数据的内存起始地址
*		m_rcShowRect：JPEG图片的解码区域，必须为（0，0，图片宽度，图片高度）
*		m_dwShowBuffWidth：图片宽度
*		m_dwShowBuffHeight;  图片高度
*		m_nBits：显示位深度
*		m_bActPicSize：是否完全解码JPEG图片
* 注意事项：
*		1.m_pShowBuff设置为内存地址时讲对JPEG图片进行解码，并将保存JPEG图片解码后数据保存
*		2.m_rcShowRect JPEG图片的解码区域，必须为（0，0，图片宽度，图片高度）
*       3.m_dwShowBuffWidth设置为JPEG图片的宽度,m_dwShowBuffHeight设置为JPEG图片的高度，可以通过函数CSGetJPGInfo获得
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝*/
typedef struct _ShowJPGPar_ 
{
	PBYTE   m_pShowBuff;
	CSRECT  m_rcShowRect;

	DWORD   m_dwShowBuffWidth;
	DWORD   m_dwShowBuffHeight;

	int     m_nBits;
	BOOL    m_bActPicSize;
}ShowJPGPar;


/*================================================================
* 函数名：CSGetJpgVersion
* 函数功能：获得JPG库的版本号；
* 参数说明：
*		pszVer：【OUT】获得的版本号的首地址
*		nVerLen：【IN】JPG版本号的字符串长度
* 返回值：
*		无
* 注意事项：
*		无
================================================================*/
void CSGetJpgVersion( char* pszVer, int nVerLen );


/*================================================================
* 函数名：CSInitJPG
* 函数功能：初始化JPG解码所需要的内存空间；
* 参数说明：
*		pucMemory：【IN】解析JPG图片需要的内存块起始地址
*		dwMemoryLen：【IN】解析JPG图片需要的内存块的大小
* 返回值：
*		TRUE：初始化成功
*		FALSE：初始化失败
* 注意事项：
*		调用显示JPG图片显示的第一步
================================================================*/
BOOL CSInitJPG( PBYTE pucMemory, DWORD dwMemoryLen );

/*================================================================
* 函数名：CSTermJPG
* 函数功能：与init对应，释放资源；
* 参数说明：
*		无
* 返回值：
*		TRUE：初始化成功
*		FALSE：初始化失败
* 注意事项：
*		无
================================================================*/
BOOL CSTermJPG(void);


/*================================================================
* 函数名：CSShowJPG
* 函数功能：显示JPG图片；
* 参数说明：
*		pucScreenAddr：【IN】显存地址
*		pucJPEGDataBuf：【IN】JPG图片数据的起始地址
*		dwBufSize：【IN】JPG图片数据的大小
*		nShowX：【IN】JPG图片显示的横向起始位置
*		nShowY：【IN】JPG图片显示的纵向起始位置
*		nWidth：【IN】JPG图片显示的宽度
*		nHeight：【IN】JPG图片显示的高度
*		bActPicSize：【IN】自定义显示区域时为FALSE，默认显示区域时设为TRUE
* 返回值：
*		TRUE：显示成功
*		FALSE：显示失败
================================================================*/
BOOL CSShowJPG( PBYTE pucScreenAddr, PBYTE pucJPEGDataBuf, DWORD dwBufSize, unsigned int nShowX, unsigned int nShowY, unsigned int nWidth, unsigned int nHeight, BOOL bActPicSize );


/*================================================================
* 函数名：CSShowJPGFile
* 函数功能：解码JPG图片
* 参数说明：
*		pucJPEGDataBuf：【IN】JPG图片数据的起始地址
*		dwBufSize：【IN】JPG图片数据的大小
*		pInitParam:【IN】显示或者解码JPEG图片需要的结构指针，请参照ShowJPGPar的说明进行设置
* 返回值：
*		TRUE：解码成功
*		FALSE：解码失败
* 注意事项：
*		该接口不能用来显示JPEG图片
================================================================*/
BOOL CSShowJPGParam( PBYTE pucJPEGDataBuff, DWORD dwBuffSize, ShowJPGPar* pInitParam );


/*================================================================
* 函数名：CSShowJPGFile
* 函数功能：显示JPG图片；
* 参数说明：
*		pucScreenAddr：【IN】显存地址
*		lpFileName：【IN】JPG文件的文件名
*		nShowX：【IN】JPG图片显示的横向起始位置
*		nShowY：【IN】JPG图片显示的纵向起始位置
*		nWidth：【IN】JPG图片显示的宽度
*		nHeight：【IN】JPG图片显示的高度
*		bActPicSize：【IN】自定义显示区域时为FALSE，默认显示区域时设为TRUE
* 返回值：
*		TRUE：显示成功
*		FALSE：显示失败
================================================================*/
BOOL CSShowJPGFile( PBYTE pucScreenAddr, LPCTSTR lpFileName, unsigned int nShowX, unsigned int nShowY, unsigned int nWidth, unsigned int nHeight, BOOL bActPicSize );


/*================================================================
* 函数名：CSSetJPGClipRectList
* 函数功能：设置JPG图片裁剪列表；
* 参数说明：
*		pCSRECTJPGRectList：【IN】指向裁剪列表的指针
*		nRectNum：【IN】JPG裁剪区域个数
* 返回值：
*		TRUE：设置裁剪列表成功
*		FALSE：设置裁剪列表失败
* 注意事项：
*		仅对一幅JPG图片有效
================================================================*/
BOOL CSSetJPGClipRectList(CSRECT *pCSRECTJPGRectList, unsigned int nRectNum);


/*================================================================
* 函数名：CSDecodeJPG
* 函数功能：解码JPEG图片，并将解码后的数据保存在用户指定的内存中
* 参数说明：
*		pucJPEGDataBuf：【IN】JPEG图片数据起始地址
*		dwBufSize：【IN】JPEG图片数据大小
*		nWidth：【IN】指定解码JPEG图片的部分宽度或者全部宽度内的数据
*		nHeight：【IN】指定解码JPEG图片的部分高度或者全部高度内的数据
*		pucBitBuf：【IN】保存解码后JPEG图片数据的内存起始地址
*		dwColorDepth：【IN】显示位深度，如16位，32位
* 返回值：
*		TRUE：解码成功
*		FALSE：解码失败
* 注意事项：
*		1.仅对一幅JPG图片有效
*       2.该接口为cs_image模块添加，应用请不要使用
================================================================*/
BOOL CSDecodeJPG(PBYTE pucJPEGDataBuf, DWORD dwBufSize, unsigned int nWidth, unsigned int nHeight, PBYTE pucBitBuf, DWORD dwColorDepth);


/*================================================================
* 函数名：CSDecodeJPGFile
* 函数功能：解码文件系统中的JPEG图片，并将解码后的数据保存在用户指定的内存中
* 参数说明：
*		lpFileName：【IN】JPEG图片在文件系统中的名称
*		nWidth：【IN】指定解码JPEG图片的部分宽度或者全部宽度内的数据
*		nHeight：【IN】指定解码JPEG图片的部分高度或者全部高度内的数据
*		pucBitBuf：【IN】保存解码后JPEG图片数据的内存起始地址
*		dwColorDepth：【IN】显示位深度，如16位，32位
* 返回值：
*		TRUE：解码成功
*		FALSE：解码失败
* 注意事项：
*		1.仅对一幅JPG图片有效
*       2.该接口为cs_image模块添加，应用请不要使用
================================================================*/
BOOL CSDecodeJPGFile(LPCTSTR lpFileName, unsigned int nWidth, unsigned int nHeight, PBYTE pucBitBuf, DWORD dwColorDepth);


/*================================================================
* 函数名：CSGetJPGInfo
* 函数功能：获得JPEG图片信息
* 参数说明：
*		pucJPEGDataBuf：【IN】JPEG图片数据起始地址
*		dwBufSize：【IN】JPEG图片数据大小
*		pnWidth：【OUT】JPEG图片的宽度（以象素为单位）
*		pnHeight：【OUT】JPEG图片的高度（以象素为单位）

* 返回值：
*		TRUE：获取成功
*		FALSE：获取失败
* 注意事项：
*		必须首先调用CSInitJPG
================================================================*/
BOOL CSGetJPGInfo(PBYTE pucJPEGDataBuf, DWORD dwBufSize, DWORD *pdwWidth, DWORD *pdwHeight);


/*================================================================
* 函数名：CSGetJPGFileInfo
* 函数功能：获得保存在文件系统中的JPEG图片的信息
* 参数说明：
*		lpFileName：【IN】JPEG图片文件名
*		pnWidth：【OUT】JPEG图片的宽度（以象素为单位）
*		pnHeight：【OUT】JPEG图片的高度（以象素为单位）

* 返回值：
*		TRUE：获取成功
*		FALSE：获取失败
* 注意事项：
*       该接口为cs_image模块添加，应用请不要使用
================================================================*/
BOOL CSGetJPGFileInfo(LPCTSTR lpFileName, unsigned int *pnWidth, unsigned int *pnHeight);

#ifdef __cplusplus
}
#endif

#endif

