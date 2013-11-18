/******************************************************************************
** FileName:  CSGrImage.h
** Copyright (c) 2007， Shenzhen Coship Electronics CO.,LTD.。
** 
** Version:       Ver 1.0
** Author:        xiachao (xiachao@coship.com)
** 
** File Description:
**       Image模块的公共文件
** 
** Modify History:
** Version     Date        Author          Description
** --------    ----------  --------        ------------
** 1.0         2007-07-31  xiachao         Create
*******************************************************************************/
#ifndef _CS_GR_IMAGE_H
#define _CS_GR_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
*                                 Header File Include                         *
******************************************************************************/
#include "CS_Windows.h"
#include "cs_osg.h"

/******************************************************************************
*                                 Macro/Define/Structs                        *
******************************************************************************/
//图片的格式，分别有BMP、GIF、JPG及PNG这几种格式
typedef enum _CSGrImageType
{
	EM_GR_IMAGE_TYPE_BMP,
	EM_GR_IMAGE_TYPE_GIF,
	EM_GR_IMAGE_TYPE_JPG,
	EM_GR_IMAGE_TYPE_PNG,
	EM_GR_IMAGE_TYPE_IFR
}CSGrImageType_E;

//图片信息的结构体
typedef struct _CSGrImageInfo
{
	CSGrImageType_E	m_eImageType;	//原始图片的类型：bmp、gif、jpg、png
	int				m_nPanels;		//图片的帧数，gif解码后的图片可能会有多帧
	int				m_nBpp;			//图片的位色，仅对bmp图片有效，其他图片的位色同当前系统
	int				m_nWidth;		//图片的宽度
	int				m_nHeight;		//图片的高度
	int				m_nPitch;		//每行所占的字节数
	
	PBYTE			m_pucBits;
	DWORD			m_dwBitLen;
	int				m_nDuration;	//表示两帧图片间的时间间隔，对于多帧图片有效
}CSGrImageInfo_S;

//Mask结构体，用来过滤图像
typedef struct _CSGrImageMask
{
	int		m_nWidth;		//mask区域的宽度
	int		m_nHeight;		//mask区域的高度

	PBYTE	m_pucMaskBits;	//mask数据
	int		m_nMaskDatalen;	//mask数据长度
}CSGrImageMask_S;


/******************************************************************************
*                                 Global Function Declare                     *
******************************************************************************/

/******************************************************************************
** Function Name:    CSGrImageInit
** 
** Description:
**		初始化PNG、JPG、GIF模块。
** 
** Input:
**		pucDecodeMem :内存空间的首地址。
**		nMemLen : 内存空间的长度。
** 
** Output:
** 
** Return Value:
**		success : 	返回TRUE
**		fail :		返回FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrImageInit(PBYTE pucDecodeMem, int nMemLen);

/******************************************************************************
** Function Name:    CSGrImageTerm
** 
** Description:
**		释放初始化过程中申请的资源。
** 
** Input:
**		无。
** 
** Output:
** 
** Return Value:
**		success : 	返回TRUE
**		fail :		返回FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrImageTerm(void);

/******************************************************************************
** Function Name:    CSGrLoadImageFromBuffer
** 
** Description:
**		通过传入的图片数据载入一幅图片，bTranslated为True时，将其他图片格式转换
**	为位图格式，否则不进行转换，仅在显示的时候再边转换边显示。
** 
** Input:
**		pcImageData : 图片数据的首地址。
**		nDataLen : 图片数据的长度。
**		bTranslated : 是否进行编码转换。
** 
** Output:
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromBuffer(const char* pcImageData, int nDataLen, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrLoadImageFromBufferScaled
** 
** Description:
**		通过传入的图片数据载入一幅图片，bTranslated为True时，将其他图片格式转换
**	为位图格式，并做伸缩变换(变换大小由nWidth及nHeight指定)，否则不进行转换，仅在显示的时候再边转换边显示。
** 
** Input:
**		pcImageData : 图片数据的首地址。
**		nDataLen :    图片数据的长度。
**		nWidth :      转换之后的图片宽度。
**		nHeight :     转换之后的图片高度。
**		bTranslated : 是否进行编码转换。
** 
** Output:
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromBufferScaled(const char* pcImageData, int nDataLen, int nWidth, int nHeight, BOOL bTranslated);
	

/******************************************************************************
** Function Name:    CSGrLoadImageFromFile
** 
** Description:
**		通过传入的图片文件载入一幅图片，bTranslated为True时，将其他图片格式转换
**	为位图格式，否则不进行转换，仅在显示的时候再边转换边显示。
** 
** Input:
**		pcFileName : 图片文件名。
**		bTranslated : 是否进行编码转换。
** 
** Output:
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromFile(const char* pcFileName, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrLoadImageFromFileScaled
** 
** Description:
**		通过传入的图片文件载入一幅图片，bTranslated为True时，将其他图片格式转换
**	为位图格式，并做伸缩变换(变换大小由nWidth及nHeight指定)，否则不进行转换，仅在显示的时候再边转换边显示。
** 
** Input:
**		pcFileName :  图片文件名。
**		nWidth :      转换之后的图片宽度。
**		nHeight :     转换之后的图片高度。
**		bTranslated : 是否进行编码转换。
** 
** Output:
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromFileScaled(const char* pcFileName, int nWidth, int nHeight, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrNewPixmap
** 
** Description:
**		创建一幅图片，若pvPixels不为空，则采用pvPixels的内存创建位图，否则就要
**	申请内存之后再创建。
** 
** Input:
**		nWidth : 图片宽度。
**		nHeight : 图片高度。
**		pvPixels : 用来创建位图的内存空间，为空时申请一块内存。
** 
** Output:
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrNewPixmap(int nWidth, int nHeight, void *pvPixels);

/******************************************************************************
** Function Name:    CSGrGetImageInfo
** 
** Description:
**		获取图片的信息。
** 
** Input:
**		hImage : 图片句柄。
** 
** Output:
**		psImageInfo : 返回的图片信息，详细参加CSGrImageInfo_S结构体。
** 
** Return Value:
**		success : 	返回一个正常的图片句柄
**		fail :		返回NULL
** 
** Other:
*******************************************************************************/
BOOL CSGrGetImageInfo(HCSHANDLE hImage, CSGrImageInfo_S * psImageInfo);

/******************************************************************************
** Function Name:    CSGrDrawImage
** 
** Description:
**		绘制一幅图片，图片由hImage句柄指定。
** 
** Input:
**		hWnd : 绘制图片的窗口。
**		nXDst : 目标区域的横坐标。
**		nYDst : 目标区域的纵坐标。
**		nWidth : 绘制的宽度。
**		nHeight : 绘制的高度。
**		hImage : 图片句柄。
**		nXSrc : 绘制时图片的起始横坐标。
**		nYSrc : 绘制时图片的起始纵坐标。
**		psImageMask : 图片的mask信息。
**		dwColorKey : 当混合模式为EM_OSG_OP_MODE_COLORKEY时，该颜色不画，真彩模
**			式下颜色值为ARGB的格式，调色板模式下为颜色索引。
**		eOpMode : 图片的混合模式，参加CSOSGOpMode的定义。
** 
** Output:
** 
** Return Value:
**		success : 	返回TRUE
**		fail :		返回FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrDrawImage(HCSDC hdc, int nXDst, int nYDst, int nWidth, int nHeight, 
				   HCSHANDLE hImage, int nXSrc, int nYSrc, 
				   CSGrImageMask_S *psImageMask, DWORD dwColorKey, 
				   int nOpMode);

/******************************************************************************
** Function Name:    CSGrFreeImage
** 
** Description:
**		销毁图片及释放图片创建过程中申请的资源。
** 
** Input:
**		hImage : 图片句柄。
** 
** Output:
** 
** Return Value:
**		success : 	返回TRUE
**		fail :		返回FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrFreeImage(HCSHANDLE hImage);

#ifdef __cplusplus
}
#endif

#endif

