#ifndef CS_FREETYPE_H
#define CS_FREETYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	���ͣ�
 *	FONTTYPE_BOLD������
 *	FONTTYPE_ITALIC	��б��
 *	FONTTYPE_AUTOHINT���Զ�����
 *	FONTTYPE_GRAYRENDER������ݴ���
 *	
 */
#define FONTTYPE_DEFAULT	(0x0000)
#define FONTTYPE_BOLD		(0x0001)
#define FONTTYPE_ITALIC		(0x0002)
#define FONTTYPE_AUTOHINT	(0x0004)
#define FONTTYPE_GRAYRENDER	(0x0008)

/*
 * ��������CSVFGetVersion
 * �������ܣ���ÿ�İ汾�ţ�
 * ����˵����
 *		pcVer����OUT���汾�Ż������׵�ַ
 *		nVerLen����IN���汾�Ż���������
 * ����ֵ��
 *		��
 * ע�����
 *		��
 */
void CSVFGetVersion(char* pcVer, int nVerLen);

/*
 * ��������CSVFInit
 * �������ܣ���ʼ��Vector Font �⣻
 * ����˵����
 *		pucMemory����IN������Vector Fontģ����ڴ�ռ��׵�ַ
 *		nMemoryLen����IN������Vector Fontģ����ڴ�ռ��׵�ַ
 * ����ֵ��
 *		TRUE����ʼ���ɹ�
 *		FALSE����ʼ��ʧ��
 * ע�����
 *		��
 */
BOOL CSVFInit(PBYTE pucMemory, int nMemoryLen);

/*
 * ��������CSVFOpenFont
 * �������ܣ���һ���ֿ⣻
 * ����˵����
 *		pucDataBuf����IN���ֿ����ݣ���TTF�ļ��Ķ��������ݣ����׵�ַ
 *		dwDataLen����IN���ֿ����ݵĳ���
 * ����ֵ��
 *		�򿪳ɹ��򷵻�һ�����
 *		ʧ�ܷ���NULL
 * ע�����
 *		������CSVFInit�����
 */
HCSHANDLE CSVFOpenFont(PBYTE pucDataBuf, DWORD dwDataLen);

/*
 * ��������CSVFCloseFont
 * �������ܣ��ر�һ���ֿ⣻
 * ����˵����
 *		hFont����IN��Ҫ�ر��ֿ�ľ��
 * ����ֵ��
 *		TRUE����ʼ���ɹ�
 *		FALSE����ʼ��ʧ��
 * ע�����
 *		��
 */
BOOL CSVFCloseFont(HCSHANDLE hFont);

/*
 * ��������CSVFSetResolutions
 * �������ܣ������豸�ֱ��ʣ�
 * ����˵����
 *		hFont����IN���ֿ���
 *		wXResolution����IN��ˮƽ�ֱ���
 *		wYResolution����IN����ֱ�ֱ���
 * ����ֵ��
 *		TRUE����ʼ���ɹ�
 *		FALSE����ʼ��ʧ��
 * ע�����
 *		Ĭ��ֵΪ96��������CSVFOpenFont�����
 */
BOOL CSVFSetResolutions(HCSHANDLE hFont, WORD wXResolution, WORD wYResolution);

/*
 * ��������CSVFSetCharSizes
 * �������ܣ������ֿ��е������С��
 * ����˵����
 *		hFont����IN���ֿ���
 *		dCharWidth����IN��������
 *		dCharHeight����IN������߶�
 * ����ֵ��
 *		TRUE����ʼ���ɹ�
 *		FALSE����ʼ��ʧ��
 * ע�����
 *		������CSVFOpenFont�����
 */
BOOL CSVFSetCharSizes(HCSHANDLE hFont, double dCharWidth, double dCharHeight);

/*
 * ��������CSVFGetCharSizes
 * �������ܣ���ȡһ���ַ��Ŀ�Ⱥ͸߶ȣ�
 * ����˵����
 *		hFont����IN���ֿ���
 *		pwCharWidth����OUT�������ַ������
 *		pwCharHeight����OUT�������ַ����߶�
 *		wCharID����IN���ַ���Unicode����
 *		dwFontType����IN������
 * ����ֵ��
 *		TRUE����ʼ���ɹ�
 *		FALSE����ʼ��ʧ��
 * ע�����
 *		������CSVFOpenFont�����
 */
BOOL CSVFGetCharSizes(HCSHANDLE hFont, WORD* pwCharWidth, WORD* pwCharHeight, WORD wCharID, DWORD dwFontType);

/*
 * ��������CSVFGetCharFontData
 * �������ܣ���ȡһ���ַ����ĵ���
 * ����˵����
 *		hFont����IN���ֿ���
 *		pucDataBuf����IN/OUT����ȡ�����Buf
 *		dwDataBufLen����IN����ȡ�����Buf����
 *		wCharID����IN���ַ���Unicode����
 *		dwFontType����IN������
 *		nOffX����IN��X�����ϵ�ƫ��
 *		nOffY����IN��Y�����ϵ�ƫ��
 *		nOneLineWidth����IN��ÿ���ֵĿ��
 * ����ֵ��
 *		TRUE����ȡ�ɹ�
 *		FALSE����ȡʧ��
 * ע�����
 *		������CSVFOpenFont�����
 */
BOOL CSVFGetCharFontData(HCSHANDLE hFont, PBYTE pucDataBuf, DWORD dwDataBufLen, WORD wCharID, DWORD dwFontType, int nOffX, int nOffY, int nOneLineWidth);

/*
 * ��������CSVFGetAdvanceX
 * �������ܣ���ȡһ���ֵĲ�����ȣ�
 * ����˵����
 *		hFont����IN���ֿ���
 *		wCharID����IN���ַ���Unicode����
 *		dwFontType����IN������
 * ����ֵ��
 *		���ز������
 *		ʧ�ܷ���-1
 * ע�����
 *		������CSVFOpenFont�����
 */
int CSVFGetAdvanceX(HCSHANDLE hFont, WORD wCharID, DWORD dwFontType);

/*
 * ��������CSVFGetAdvanceY
 * �������ܣ���ȡһ���ֵĲ����߶ȣ�
 * ����˵����
 *		hFont����IN���ֿ���
 *		wCharID����IN���ַ���Unicode����
 *		dwFontType����IN������
 * ����ֵ��
 *		���ز����߶�
 *		ʧ�ܷ���-1
 * ע�����
 *		������CSVFOpenFont�����
 */
int CSVFGetAdvanceY(HCSHANDLE hFont, WORD wCharID, DWORD dwFontType);

/*
 * ��������CSVFGetFontAsent
 * �������ܣ���ȡһ���ֿ�����и߶ȣ�
 * ����˵����
 *		hFont����IN���ֿ���
 * ����ֵ��
 *		�������и߶�
 *		ʧ�ܷ���-1
 * ע�����
 *		������CSVFOpenFont����ã�һ��Ϊ��ֵ
 */
int CSVFGetFontAsent(HCSHANDLE hFont);

/*
 * ��������CSVFGetFontDesent
 * �������ܣ���ȡһ���ֿ�����и߶ȣ�
 * ����˵����
 *		hFont����IN���ֿ���
 * ����ֵ��
 *		�������и߶�
 *		ʧ�ܷ���-1
 * ע�����
 *		������CSVFOpenFont����ã�һ��Ϊ��ֵ
 */
int CSVFGetFontDesent(HCSHANDLE hFont);

/*
 * ��������CSVFGetLastError
 * �������ܣ���ȡ���һ������
 * ����˵����
 *		��
 * ����ֵ��
 *		��
 * ע�����
 *		��
 */
int CSVFGetLastError(void);

#ifdef __cplusplus
}
#endif

#endif /* CS_VECTORFONT_H */

/* END */
