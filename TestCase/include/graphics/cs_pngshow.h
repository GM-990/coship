#ifndef __CS_SHOWPNG_H
#define __CS_SHOWPNG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "CS_Windows.h"

/*================================================================
* ��������CSGetPNGVersion
* �������ܣ����PNG��İ汾�ţ�
* ����˵����
*		pszVer����OUT����õİ汾�ŵ��׵�ַ
*		nVerLen����IN��PNG�汾�ŵ��ַ�������
* ����ֵ��
*		��
* ע�����
*		��
================================================================*/
void CSGetPNGVersion(char* pszVer, int nVerLen);


/*================================================================
* ��������CSInitPNG
* �������ܣ���ʼ��PNG��������Ҫ���ڴ�ռ䣻
* ����˵����
*		pucShowPNGMemory����IN������PNGͼƬ��Ҫ���ڴ����ʼ��ַ
*		dwMemorySize����IN������PNGͼƬ��Ҫ���ڴ��Ĵ�С
* ����ֵ��
*		TRUE����ʼ���ɹ�
*		FALSE����ʼ��ʧ��
* ע�����
*		������ʾPNGͼƬ��ʾ�ĵ�һ��
================================================================*/
BOOL CSInitPNG(PBYTE pucShowPNGMemory, DWORD dwMemorySize);

BOOL CSTermPNG(void);
	
/*================================================================
* ��������CSShowPNG
* �������ܣ���ʾPNGͼƬ��
* ����˵����
*		pucScreenAddr����IN���Դ��ַ
*		pucPNGDataBuf����IN��PNGͼƬ���ݵ���ʼ��ַ
*		dwPNGDataLen����IN��PNGͼƬ���ݵĴ�С
*		pstShowRect����IN���Զ���PNGͼƬ��ʾ����ָ��
*		dwBKColor:��IN���û��Զ���PNG������ɫ��������PNGͼƬδ����ͼƬ������ɫʱ��Ч��
*		bActPicSize����IN���Զ�����ʾ����ʱΪFALSE��Ĭ����ʾ����ʱ��ΪTRUE
* ����ֵ��
*		TRUE����ʾ�ɹ�
*		FALSE����ʾʧ��
================================================================*/
BOOL CSShowPNG(PBYTE pucScreenAddr, PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, CSRECT *pstShowRect, DWORD dwBKColor, BOOL bActPicSize);


/*================================================================
* ��������CSShowPNGFile
* �������ܣ���ʾ��COSHIP RMFS�ļ�ϵͳ�ļ���ŵ�PNGͼƬ
* ����˵����
*		pucScreenAddr����IN���Դ��ַ
*		lpFileName����IN��PNG�ļ����ļ���
*		pstShowRect����IN���Զ���PNGͼƬ��ʾ����
*		dwBKColor:��IN���û��Զ���PNG������ɫ��������PNGͼƬδ����ͼƬ������ɫʱ��Ч��
*		bActPicSize����IN���Զ�����ʾ����ʱΪFALSE��Ĭ����ʾ����ʱ��ΪTRUE
* ����ֵ��
*		TRUE����ʾ�ɹ�
*		FALSE����ʾʧ��
================================================================*/
BOOL CSShowPNGFile(PBYTE pucScreenAddr, char* lpFileName, CSRECT *pstShowRect, DWORD dwBKColor, BOOL bActPicSize);


/*================================================================
* ��������CSDecodePNG
* �������ܣ�����PNGͼƬ���������������ݱ������û�ָ�����ڴ���
* ����˵����
*		pucBitBuf����IN����������PNGͼƬ���ݵ��ڴ���ʼ��ַ
*		pucPNGDataBuf����IN��PNGͼƬ������ʼ��ַ
*		dwPNGDataLen������IN��PNGͼƬ���ݴ�С
*		dwColorDepth����IN����ʾλ��ȣ���16λ��32λ
* ����ֵ��
*		TRUE������ɹ�
*		FALSE������ʧ��
* ע�����
*		����һ��PNGͼƬ��Ч
================================================================*/
BOOL CSDecodePNG(PBYTE pucBitBuf, PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, DWORD dwColorDepth);


/*================================================================
* ��������CSDecodePNGFile
* �������ܣ������ļ�ϵͳ�е�PNGͼƬ���������������ݱ������û�ָ�����ڴ���
* ����˵����
*		pucBitBuf����IN����������PNGͼƬ���ݵ��ڴ���ʼ��ַ
*		lpFileName����IN��PNGͼƬ���ļ�ϵͳ�е�����
*		dwColorDepth����IN����ʾλ��ȣ���16λ��32λ
* ����ֵ��
*		TRUE������ɹ�
*		FALSE������ʧ��
* ע�����
*		����һ��PNGͼƬ��Ч
================================================================*/
BOOL CSDecodePNGFile(PBYTE pucBitBuf, LPCTSTR lpFileName, DWORD dwColorDepth);


/*================================================================
* ��������CSGetPNGInfo
* �������ܣ����PNGͼƬ��Ϣ
* ����˵����
*		pucPNGDataBuf����IN��PNGͼƬ������ʼ��ַ
*		dwPNGDataLen����IN��PNGͼƬ���ݴ�С
*		pnWidth����OUT��PNGͼƬ�Ŀ�ȣ�������Ϊ��λ��
*		pnHeight����OUT��PNGͼƬ�ĸ߶ȣ�������Ϊ��λ��

* ����ֵ��
*		TRUE����ȡ�ɹ�
*		FALSE����ȡʧ��
* ע�����
*		���������ȵ���CSInitPNG
================================================================*/
BOOL CSGetPNGInfo(PBYTE pucPNGDataBuf, DWORD dwPNGDataLen, DWORD* pdwWidth, DWORD* pdwHeight);

#ifdef __cplusplus
}
#endif

#endif

