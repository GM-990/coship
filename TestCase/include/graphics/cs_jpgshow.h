
#ifndef _CS_JPGSHOW_H
#define _CS_JPGSHOW_H

#include "CS_Windows.h"

#ifdef __cplusplus
extern "C" {
#endif

//#ifndef JPG_USE_RMFS
//#define JPG_USE_RMFS
//#endif

/*������������������������������������������������������������������������������������������
* �ṹ����ShowJPGPar
* �ṹ˵�������ڵ���JPGģ����ʾ�Լ����뺯��ʱ��Ҫ���õ�һЩ������Ϣ
* ��Ա˵����
*		m_pShowBuff;	���ڱ���JPEGͼƬ��������ݵ��ڴ���ʼ��ַ
*		m_rcShowRect��JPEGͼƬ�Ľ������򣬱���Ϊ��0��0��ͼƬ��ȣ�ͼƬ�߶ȣ�
*		m_dwShowBuffWidth��ͼƬ���
*		m_dwShowBuffHeight;  ͼƬ�߶�
*		m_nBits����ʾλ���
*		m_bActPicSize���Ƿ���ȫ����JPEGͼƬ
* ע�����
*		1.m_pShowBuff����Ϊ�ڴ��ַʱ����JPEGͼƬ���н��룬��������JPEGͼƬ��������ݱ���
*		2.m_rcShowRect JPEGͼƬ�Ľ������򣬱���Ϊ��0��0��ͼƬ��ȣ�ͼƬ�߶ȣ�
*       3.m_dwShowBuffWidth����ΪJPEGͼƬ�Ŀ��,m_dwShowBuffHeight����ΪJPEGͼƬ�ĸ߶ȣ�����ͨ������CSGetJPGInfo���
������������������������������������������������������������������������������������������*/
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
* ��������CSGetJpgVersion
* �������ܣ����JPG��İ汾�ţ�
* ����˵����
*		pszVer����OUT����õİ汾�ŵ��׵�ַ
*		nVerLen����IN��JPG�汾�ŵ��ַ�������
* ����ֵ��
*		��
* ע�����
*		��
================================================================*/
void CSGetJpgVersion( char* pszVer, int nVerLen );


/*================================================================
* ��������CSInitJPG
* �������ܣ���ʼ��JPG��������Ҫ���ڴ�ռ䣻
* ����˵����
*		pucMemory����IN������JPGͼƬ��Ҫ���ڴ����ʼ��ַ
*		dwMemoryLen����IN������JPGͼƬ��Ҫ���ڴ��Ĵ�С
* ����ֵ��
*		TRUE����ʼ���ɹ�
*		FALSE����ʼ��ʧ��
* ע�����
*		������ʾJPGͼƬ��ʾ�ĵ�һ��
================================================================*/
BOOL CSInitJPG( PBYTE pucMemory, DWORD dwMemoryLen );

/*================================================================
* ��������CSTermJPG
* �������ܣ���init��Ӧ���ͷ���Դ��
* ����˵����
*		��
* ����ֵ��
*		TRUE����ʼ���ɹ�
*		FALSE����ʼ��ʧ��
* ע�����
*		��
================================================================*/
BOOL CSTermJPG(void);


/*================================================================
* ��������CSShowJPG
* �������ܣ���ʾJPGͼƬ��
* ����˵����
*		pucScreenAddr����IN���Դ��ַ
*		pucJPEGDataBuf����IN��JPGͼƬ���ݵ���ʼ��ַ
*		dwBufSize����IN��JPGͼƬ���ݵĴ�С
*		nShowX����IN��JPGͼƬ��ʾ�ĺ�����ʼλ��
*		nShowY����IN��JPGͼƬ��ʾ��������ʼλ��
*		nWidth����IN��JPGͼƬ��ʾ�Ŀ��
*		nHeight����IN��JPGͼƬ��ʾ�ĸ߶�
*		bActPicSize����IN���Զ�����ʾ����ʱΪFALSE��Ĭ����ʾ����ʱ��ΪTRUE
* ����ֵ��
*		TRUE����ʾ�ɹ�
*		FALSE����ʾʧ��
================================================================*/
BOOL CSShowJPG( PBYTE pucScreenAddr, PBYTE pucJPEGDataBuf, DWORD dwBufSize, unsigned int nShowX, unsigned int nShowY, unsigned int nWidth, unsigned int nHeight, BOOL bActPicSize );


/*================================================================
* ��������CSShowJPGFile
* �������ܣ�����JPGͼƬ
* ����˵����
*		pucJPEGDataBuf����IN��JPGͼƬ���ݵ���ʼ��ַ
*		dwBufSize����IN��JPGͼƬ���ݵĴ�С
*		pInitParam:��IN����ʾ���߽���JPEGͼƬ��Ҫ�Ľṹָ�룬�����ShowJPGPar��˵����������
* ����ֵ��
*		TRUE������ɹ�
*		FALSE������ʧ��
* ע�����
*		�ýӿڲ���������ʾJPEGͼƬ
================================================================*/
BOOL CSShowJPGParam( PBYTE pucJPEGDataBuff, DWORD dwBuffSize, ShowJPGPar* pInitParam );


/*================================================================
* ��������CSShowJPGFile
* �������ܣ���ʾJPGͼƬ��
* ����˵����
*		pucScreenAddr����IN���Դ��ַ
*		lpFileName����IN��JPG�ļ����ļ���
*		nShowX����IN��JPGͼƬ��ʾ�ĺ�����ʼλ��
*		nShowY����IN��JPGͼƬ��ʾ��������ʼλ��
*		nWidth����IN��JPGͼƬ��ʾ�Ŀ��
*		nHeight����IN��JPGͼƬ��ʾ�ĸ߶�
*		bActPicSize����IN���Զ�����ʾ����ʱΪFALSE��Ĭ����ʾ����ʱ��ΪTRUE
* ����ֵ��
*		TRUE����ʾ�ɹ�
*		FALSE����ʾʧ��
================================================================*/
BOOL CSShowJPGFile( PBYTE pucScreenAddr, LPCTSTR lpFileName, unsigned int nShowX, unsigned int nShowY, unsigned int nWidth, unsigned int nHeight, BOOL bActPicSize );


/*================================================================
* ��������CSSetJPGClipRectList
* �������ܣ�����JPGͼƬ�ü��б�
* ����˵����
*		pCSRECTJPGRectList����IN��ָ��ü��б��ָ��
*		nRectNum����IN��JPG�ü��������
* ����ֵ��
*		TRUE�����òü��б�ɹ�
*		FALSE�����òü��б�ʧ��
* ע�����
*		����һ��JPGͼƬ��Ч
================================================================*/
BOOL CSSetJPGClipRectList(CSRECT *pCSRECTJPGRectList, unsigned int nRectNum);


/*================================================================
* ��������CSDecodeJPG
* �������ܣ�����JPEGͼƬ���������������ݱ������û�ָ�����ڴ���
* ����˵����
*		pucJPEGDataBuf����IN��JPEGͼƬ������ʼ��ַ
*		dwBufSize����IN��JPEGͼƬ���ݴ�С
*		nWidth����IN��ָ������JPEGͼƬ�Ĳ��ֿ�Ȼ���ȫ������ڵ�����
*		nHeight����IN��ָ������JPEGͼƬ�Ĳ��ָ߶Ȼ���ȫ���߶��ڵ�����
*		pucBitBuf����IN����������JPEGͼƬ���ݵ��ڴ���ʼ��ַ
*		dwColorDepth����IN����ʾλ��ȣ���16λ��32λ
* ����ֵ��
*		TRUE������ɹ�
*		FALSE������ʧ��
* ע�����
*		1.����һ��JPGͼƬ��Ч
*       2.�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
================================================================*/
BOOL CSDecodeJPG(PBYTE pucJPEGDataBuf, DWORD dwBufSize, unsigned int nWidth, unsigned int nHeight, PBYTE pucBitBuf, DWORD dwColorDepth);


/*================================================================
* ��������CSDecodeJPGFile
* �������ܣ������ļ�ϵͳ�е�JPEGͼƬ���������������ݱ������û�ָ�����ڴ���
* ����˵����
*		lpFileName����IN��JPEGͼƬ���ļ�ϵͳ�е�����
*		nWidth����IN��ָ������JPEGͼƬ�Ĳ��ֿ�Ȼ���ȫ������ڵ�����
*		nHeight����IN��ָ������JPEGͼƬ�Ĳ��ָ߶Ȼ���ȫ���߶��ڵ�����
*		pucBitBuf����IN����������JPEGͼƬ���ݵ��ڴ���ʼ��ַ
*		dwColorDepth����IN����ʾλ��ȣ���16λ��32λ
* ����ֵ��
*		TRUE������ɹ�
*		FALSE������ʧ��
* ע�����
*		1.����һ��JPGͼƬ��Ч
*       2.�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
================================================================*/
BOOL CSDecodeJPGFile(LPCTSTR lpFileName, unsigned int nWidth, unsigned int nHeight, PBYTE pucBitBuf, DWORD dwColorDepth);


/*================================================================
* ��������CSGetJPGInfo
* �������ܣ����JPEGͼƬ��Ϣ
* ����˵����
*		pucJPEGDataBuf����IN��JPEGͼƬ������ʼ��ַ
*		dwBufSize����IN��JPEGͼƬ���ݴ�С
*		pnWidth����OUT��JPEGͼƬ�Ŀ�ȣ�������Ϊ��λ��
*		pnHeight����OUT��JPEGͼƬ�ĸ߶ȣ�������Ϊ��λ��

* ����ֵ��
*		TRUE����ȡ�ɹ�
*		FALSE����ȡʧ��
* ע�����
*		�������ȵ���CSInitJPG
================================================================*/
BOOL CSGetJPGInfo(PBYTE pucJPEGDataBuf, DWORD dwBufSize, DWORD *pdwWidth, DWORD *pdwHeight);


/*================================================================
* ��������CSGetJPGFileInfo
* �������ܣ���ñ������ļ�ϵͳ�е�JPEGͼƬ����Ϣ
* ����˵����
*		lpFileName����IN��JPEGͼƬ�ļ���
*		pnWidth����OUT��JPEGͼƬ�Ŀ�ȣ�������Ϊ��λ��
*		pnHeight����OUT��JPEGͼƬ�ĸ߶ȣ�������Ϊ��λ��

* ����ֵ��
*		TRUE����ȡ�ɹ�
*		FALSE����ȡʧ��
* ע�����
*       �ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
================================================================*/
BOOL CSGetJPGFileInfo(LPCTSTR lpFileName, unsigned int *pnWidth, unsigned int *pnHeight);

#ifdef __cplusplus
}
#endif

#endif

