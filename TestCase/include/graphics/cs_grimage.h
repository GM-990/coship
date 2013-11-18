/******************************************************************************
** FileName:  CSGrImage.h
** Copyright (c) 2007�� Shenzhen Coship Electronics CO.,LTD.��
** 
** Version:       Ver 1.0
** Author:        xiachao (xiachao@coship.com)
** 
** File Description:
**       Imageģ��Ĺ����ļ�
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
//ͼƬ�ĸ�ʽ���ֱ���BMP��GIF��JPG��PNG�⼸�ָ�ʽ
typedef enum _CSGrImageType
{
	EM_GR_IMAGE_TYPE_BMP,
	EM_GR_IMAGE_TYPE_GIF,
	EM_GR_IMAGE_TYPE_JPG,
	EM_GR_IMAGE_TYPE_PNG,
	EM_GR_IMAGE_TYPE_IFR
}CSGrImageType_E;

//ͼƬ��Ϣ�Ľṹ��
typedef struct _CSGrImageInfo
{
	CSGrImageType_E	m_eImageType;	//ԭʼͼƬ�����ͣ�bmp��gif��jpg��png
	int				m_nPanels;		//ͼƬ��֡����gif������ͼƬ���ܻ��ж�֡
	int				m_nBpp;			//ͼƬ��λɫ������bmpͼƬ��Ч������ͼƬ��λɫͬ��ǰϵͳ
	int				m_nWidth;		//ͼƬ�Ŀ��
	int				m_nHeight;		//ͼƬ�ĸ߶�
	int				m_nPitch;		//ÿ����ռ���ֽ���
	
	PBYTE			m_pucBits;
	DWORD			m_dwBitLen;
	int				m_nDuration;	//��ʾ��֡ͼƬ���ʱ���������ڶ�֡ͼƬ��Ч
}CSGrImageInfo_S;

//Mask�ṹ�壬��������ͼ��
typedef struct _CSGrImageMask
{
	int		m_nWidth;		//mask����Ŀ��
	int		m_nHeight;		//mask����ĸ߶�

	PBYTE	m_pucMaskBits;	//mask����
	int		m_nMaskDatalen;	//mask���ݳ���
}CSGrImageMask_S;


/******************************************************************************
*                                 Global Function Declare                     *
******************************************************************************/

/******************************************************************************
** Function Name:    CSGrImageInit
** 
** Description:
**		��ʼ��PNG��JPG��GIFģ�顣
** 
** Input:
**		pucDecodeMem :�ڴ�ռ���׵�ַ��
**		nMemLen : �ڴ�ռ�ĳ��ȡ�
** 
** Output:
** 
** Return Value:
**		success : 	����TRUE
**		fail :		����FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrImageInit(PBYTE pucDecodeMem, int nMemLen);

/******************************************************************************
** Function Name:    CSGrImageTerm
** 
** Description:
**		�ͷų�ʼ���������������Դ��
** 
** Input:
**		�ޡ�
** 
** Output:
** 
** Return Value:
**		success : 	����TRUE
**		fail :		����FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrImageTerm(void);

/******************************************************************************
** Function Name:    CSGrLoadImageFromBuffer
** 
** Description:
**		ͨ�������ͼƬ��������һ��ͼƬ��bTranslatedΪTrueʱ��������ͼƬ��ʽת��
**	Ϊλͼ��ʽ�����򲻽���ת����������ʾ��ʱ���ٱ�ת������ʾ��
** 
** Input:
**		pcImageData : ͼƬ���ݵ��׵�ַ��
**		nDataLen : ͼƬ���ݵĳ��ȡ�
**		bTranslated : �Ƿ���б���ת����
** 
** Output:
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromBuffer(const char* pcImageData, int nDataLen, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrLoadImageFromBufferScaled
** 
** Description:
**		ͨ�������ͼƬ��������һ��ͼƬ��bTranslatedΪTrueʱ��������ͼƬ��ʽת��
**	Ϊλͼ��ʽ�����������任(�任��С��nWidth��nHeightָ��)�����򲻽���ת����������ʾ��ʱ���ٱ�ת������ʾ��
** 
** Input:
**		pcImageData : ͼƬ���ݵ��׵�ַ��
**		nDataLen :    ͼƬ���ݵĳ��ȡ�
**		nWidth :      ת��֮���ͼƬ��ȡ�
**		nHeight :     ת��֮���ͼƬ�߶ȡ�
**		bTranslated : �Ƿ���б���ת����
** 
** Output:
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromBufferScaled(const char* pcImageData, int nDataLen, int nWidth, int nHeight, BOOL bTranslated);
	

/******************************************************************************
** Function Name:    CSGrLoadImageFromFile
** 
** Description:
**		ͨ�������ͼƬ�ļ�����һ��ͼƬ��bTranslatedΪTrueʱ��������ͼƬ��ʽת��
**	Ϊλͼ��ʽ�����򲻽���ת����������ʾ��ʱ���ٱ�ת������ʾ��
** 
** Input:
**		pcFileName : ͼƬ�ļ�����
**		bTranslated : �Ƿ���б���ת����
** 
** Output:
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromFile(const char* pcFileName, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrLoadImageFromFileScaled
** 
** Description:
**		ͨ�������ͼƬ�ļ�����һ��ͼƬ��bTranslatedΪTrueʱ��������ͼƬ��ʽת��
**	Ϊλͼ��ʽ�����������任(�任��С��nWidth��nHeightָ��)�����򲻽���ת����������ʾ��ʱ���ٱ�ת������ʾ��
** 
** Input:
**		pcFileName :  ͼƬ�ļ�����
**		nWidth :      ת��֮���ͼƬ��ȡ�
**		nHeight :     ת��֮���ͼƬ�߶ȡ�
**		bTranslated : �Ƿ���б���ת����
** 
** Output:
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrLoadImageFromFileScaled(const char* pcFileName, int nWidth, int nHeight, BOOL bTranslated);


/******************************************************************************
** Function Name:    CSGrNewPixmap
** 
** Description:
**		����һ��ͼƬ����pvPixels��Ϊ�գ������pvPixels���ڴ洴��λͼ�������Ҫ
**	�����ڴ�֮���ٴ�����
** 
** Input:
**		nWidth : ͼƬ��ȡ�
**		nHeight : ͼƬ�߶ȡ�
**		pvPixels : ��������λͼ���ڴ�ռ䣬Ϊ��ʱ����һ���ڴ档
** 
** Output:
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
HCSHANDLE CSGrNewPixmap(int nWidth, int nHeight, void *pvPixels);

/******************************************************************************
** Function Name:    CSGrGetImageInfo
** 
** Description:
**		��ȡͼƬ����Ϣ��
** 
** Input:
**		hImage : ͼƬ�����
** 
** Output:
**		psImageInfo : ���ص�ͼƬ��Ϣ����ϸ�μ�CSGrImageInfo_S�ṹ�塣
** 
** Return Value:
**		success : 	����һ��������ͼƬ���
**		fail :		����NULL
** 
** Other:
*******************************************************************************/
BOOL CSGrGetImageInfo(HCSHANDLE hImage, CSGrImageInfo_S * psImageInfo);

/******************************************************************************
** Function Name:    CSGrDrawImage
** 
** Description:
**		����һ��ͼƬ��ͼƬ��hImage���ָ����
** 
** Input:
**		hWnd : ����ͼƬ�Ĵ��ڡ�
**		nXDst : Ŀ������ĺ����ꡣ
**		nYDst : Ŀ������������ꡣ
**		nWidth : ���ƵĿ�ȡ�
**		nHeight : ���Ƶĸ߶ȡ�
**		hImage : ͼƬ�����
**		nXSrc : ����ʱͼƬ����ʼ�����ꡣ
**		nYSrc : ����ʱͼƬ����ʼ�����ꡣ
**		psImageMask : ͼƬ��mask��Ϣ��
**		dwColorKey : �����ģʽΪEM_OSG_OP_MODE_COLORKEYʱ������ɫ���������ģ
**			ʽ����ɫֵΪARGB�ĸ�ʽ����ɫ��ģʽ��Ϊ��ɫ������
**		eOpMode : ͼƬ�Ļ��ģʽ���μ�CSOSGOpMode�Ķ��塣
** 
** Output:
** 
** Return Value:
**		success : 	����TRUE
**		fail :		����FALSE
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
**		����ͼƬ���ͷ�ͼƬ�����������������Դ��
** 
** Input:
**		hImage : ͼƬ�����
** 
** Output:
** 
** Return Value:
**		success : 	����TRUE
**		fail :		����FALSE
** 
** Other:
*******************************************************************************/
BOOL CSGrFreeImage(HCSHANDLE hImage);

#ifdef __cplusplus
}
#endif

#endif

