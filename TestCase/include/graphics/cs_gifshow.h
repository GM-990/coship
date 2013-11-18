

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
	GIF_MSG_FIRST_FIRSTFRAME_FINISH,//��һ֡��һ�ν������׳���Ϣ
	GIF_MSG_ONEFRAME_FINISH,   //һ֡��������׳���Ϣ
	GIF_MSG_ALLOC_MEMORY, //�����ڴ�
	GIF_MSG_FREE_MEMORY  //�ͷ��ڴ�
}CSGifMsgType;

typedef struct 
{
	int  m_nSize;
	unsigned char **pBuf;
}CSCACHE_IMAGE;
/*������������������������������������������������������������������������������������������������������������������������������������������
* �ṹ����ShowGifPar
* �ṹ˵�������ڵ���GIFģ����ʾ�Լ����뺯��ʱ��Ҫ���õ�һЩ������Ϣ
* ��Ա˵����
*		nBitCount����ʾλ���
*		rect����ʾ���������GIFͼƬ���뵽�ڴ��У�����������Ϊȫ0
*		m_nShowBuffWidth��ָ��������ڽ���GIFͼƬ���ֻ���ȫ����ȷ�Χ�ڵ�����
*		m_nShowBuffHeight;  ָ���߶����ڽ���GIFͼƬ���ֻ���ȫ���߶ȷ�Χ�ڵ�����
*		m_pucDoubleBuf;	�����ַ
*		m_pucScreenBuf;	�Դ��ַ
* ע�����
*		1.rect����������ʾGIFʱ���ã���������Ƕ�GIFͼƬ���н����������ʾ��rect�ĳ�Աȫ��Ӧ������Ϊ0��
*		2.m_nShowBuffWidth��m_nShowBuffHeight������GIFͼƬ���н���ʱ�����ã�����Ƕ�GIFͼƬ������ʾ�������Ƕ�Ӧ������Ϊ0��
*		3.m_pucDoubleBuf��m_pucScreenBuf������ʱ��GIFͼƬ��ʾʱ������m_pucScreenBuf��
*		4.m_pucDoubleBuf��m_pucScreenBuf��������Ϊ���ڱ���GIFͼƬ�������ݵ��ڴ���ʼ��ַ����ʱ�����Ա���趨�밴��1��2�������á�
������������������������������������������������������������������������������������������������������������������������������������������*/
typedef struct __ShowGifPar__
{
	DWORD  nBitCount;  

	CSRECT rect;	  

	int m_nShowBuffWidth;  
	int m_nShowBuffHeight;  

	PBYTE m_pucDoubleBuf;	
	PBYTE m_pucScreenBuf;	

}ShowGifPar;


//��ʾGIF����ʾ����
typedef enum _DISPLAYMODE
{
	SINGLE_MEMORY,	//������
	DOUBLE_MEMORY	//˫����

}GIF_DISPLAY_MODE;

typedef void (*CSGifCallback)(CSGifMsgType emGifMsgType, PVOID pvData);


/*����������������������������������������������
* ��������CSGifRegisterCallback
* �������ܣ�ע��GIF�ص�����
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pfnCallback����IN���ص�����ָ��
* ����ֵ��
*		�ɹ�������TRUE
*       ʧ�ܣ�����FALSE
* ע�����
*		��
����������������������������������������������*/
BOOL CSGifRegisterCallback(HCSHANDLE hShowGif, CSGifCallback pfnCallback);


/*����������������������������������������������
* ��������CSGetGifVersion
* �������ܣ����GIF��İ汾�ţ�
* ����˵����
*		pszVer����OUT����õİ汾�ŵ��׵�ַ
*		nVerLen����IN��GIF�汾�ŵ��ַ�������
* ����ֵ��
*		��
* ע�����
*		��
����������������������������������������������*/
void CSGetGifVersion( char* pszVer, int nVerLen );


/*����������������������������������������������
* ��������CSGifSetDisplayProperty
* �������ܣ�������ʾGIF���õ���ʾ���ԣ���������˫����
* ����˵����
*		enmDisplayMode����IN����ʾ����
*		pucDoubleMemAddr����IN��˫������ڴ��ַ������˫���������Ч
*		nMemSize����IN������Ĵ�С
* ����ֵ��
*		TRUE�����óɹ�
*		FALSE������ʧ��
* ע�����
*		�˺����ڴ���GIF���֮ǰ���ã�����GIF����ʾ���ԣ����û�е��ô˺�����Ĭ��Ϊ��������ԣ�
*		������õ�������ԣ���ڶ���������NULL���ɣ��������������Դ���0
����������������������������������������������*/
BOOL CSGifSetDisplayProperty( GIF_DISPLAY_MODE enmDisplayMode, BYTE* pucDoubleMemAddr, int nMemSize );


/*����������������������������������������������
* ��������CSCreateGifHandle
* �������ܣ�������ʾGIFͼƬ�ľ����
* ����˵����
*		pucMemory����IN������GIFͼƬ��Ҫ���ڴ����ʼ��ַ
*		dwMemoryLen����IN������GIFͼƬ��Ҫ���ڴ��Ĵ�С
* ����ֵ��
*		GIFͼƬ�ľ��
* ע�����
*		������ʾGIFͼƬ��ʾ�ĵ�һ��
����������������������������������������������*/
HCSHANDLE CSCreateGifHandle( PBYTE pucMemory, DWORD dwMemoryLen );


/*����������������������������������������������
* ��������CSCreateGifHandle
* �������ܣ�������ʾGIFͼƬ�ľ����
* ����˵����
*		pucMemory����IN������GIFͼƬ��Ҫ���ڴ����ʼ��ַ
*		dwMemoryLen����IN������GIFͼƬ��Ҫ���ڴ��Ĵ�С
nTaskPriority��	��IN����GIFΪ���廭��ʱ��������������ȼ�
* ����ֵ��
*		GIFͼƬ�ľ��
* ע�����
*		������ʾGIFͼƬ��ʾ�ĵ�һ��
����������������������������������������������*/
HCSHANDLE CSCreateGifHandleEx( PBYTE pucMemory, DWORD dwMemoryLen, int nTaskPriority );


/*����������������������������������������������
* ��������CSDestroyGifHandle
* �������ܣ�������ʾGIFͼƬ�ľ����
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
* ����ֵ��
*		TRUE�����ٳɹ�
*		FALSE������ʧ��
* ע�����
*		GIFͼƬ��ʾ��󣬵��ô���������GIF������ͷŽ���GIF��Ҫ���ڴ棻
*		����Ƕ�֡��GIFͼƬ��������ٺ���ͣ���������ʾ��һ֡�ϣ����Ҫ��ʾGIF��������ҪΪ��GIF���������һ��
*		�ڴ棬���ܹ����٣�����ֻ��һ֡��GIFͼƬ������ʾ��ɺ�������ٽ���GIF���ڴ��
����������������������������������������������*/
BOOL CSDestroyGifHandle( HCSHANDLE hShowGif );


/*����������������������������������������������
* ��������CSShowGif
* �������ܣ���ʾGIFͼƬ��
* ����˵����
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
* ����ֵ��
*		���ڵ�֡GIFͼƬ���ú�������1��
*      ���ڶ�֡GIFͼƬ���ú������ص�ǰ��ʾ��֡GIFͼƬ�ľ����
*      ���ڷǷ����룬�ú�������0��
* ע�����
*		Ҫ�ڴ���GIF����Ժ���ã�
����������������������������������������������*/
HCSHANDLE CSShowGif( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );


/*����������������������������������������������
* ��������CSShowGifEx
* �������ܣ���ʾGIFͼƬ��
* ����˵����
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
*		dwTransparentColor����IN���û�ָ������ɫ�����滻GIFͼƬ�е�͸��ɫ
* ����ֵ��
*		���ڵ�֡GIFͼƬ���ú�������1��
*      ���ڶ�֡GIFͼƬ���ú������ص�ǰ��ʾ��֡GIFͼƬ�ľ����
*      ���ڷǷ����룬�ú�������0��
* ע�����
*		Ҫ�ڴ���GIF����Ժ���ã�
����������������������������������������������*/
HCSHANDLE CSShowGifEx(PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*����������������������������������������������
* ��������CSShowGifFile
* �������ܣ���ʾGIFͼƬ��
* ����˵����
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
* ����ֵ��
*		���ڵ�֡GIFͼƬ���ú�������1��
*      ���ڶ�֡GIFͼƬ���ú������ص�ǰ��ʾ��֡GIFͼƬ�ľ����
*      ���ڷǷ����룬�ú�������0��
* ע�����
*		Ҫ�ڴ���GIF����Ժ���ã�
����������������������������������������������*/
HCSHANDLE CSShowGifFile( LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );


/*����������������������������������������������
* ��������CSShowGifFileEx
* �������ܣ���ʾGIFͼƬ��
* ����˵����
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
*		dwTransparentColor����IN���û�ָ������ɫ�����滻GIFͼƬ�е�͸��ɫ
* ����ֵ��
*		���ڵ�֡GIFͼƬ���ú�������1��
*      ���ڶ�֡GIFͼƬ���ú������ص�ǰ��ʾ��֡GIFͼƬ�ľ����
*      ���ڷǷ����룬�ú�������0��
* ע�����
*		Ҫ�ڴ���GIF����Ժ���ã�
����������������������������������������������*/
HCSHANDLE CSShowGifFileEx(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*����������������������������������������������
* ��������CSStopOneGif
* �������ܣ�ֹͣ��ʾĳGIF������
* ����˵����
*		hOneGifShow����IN����CSShowGif����ĳGIF������ʾ���
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
* ����ֵ��
*		TRUE��ֹͣ�ɹ�
*		FALSE��ֹͣʧ��
* ע�����
*		Ҫ�ڴ���GIF����Ժ���ã�
����������������������������������������������*/
BOOL CSStopOneGif(HCSHANDLE hOneGifShow, HCSHANDLE hShowGif);


/*����������������������������������������������
* ��������CSStartShowGif
* �������ܣ�����������ʾGIFͼƬ���������ڶ�֡��GIF������Ч����
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
* ����ֵ��
*		TRUE�������ɹ�
*		FALSE������ʧ��
* ע�����
*		�ޣ�
����������������������������������������������*/
BOOL CSStartShowGif( HCSHANDLE hShowGif );


/*����������������������������������������������
* ��������CSStopShowGif
* �������ܣ�ֹͣ��ʾGIFͼƬ���������ڶ�֡��GIF������Ч����
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
* ����ֵ��
*		TRUE��ֹͣ��֡GIF������ʾ�ɹ�
*		FALSE��ֹͣ��֡GIF������ʾʧ��
* ע�����
*		�ޣ�
����������������������������������������������*/
BOOL CSStopShowGif( HCSHANDLE hShowGif );


/*����������������������������������������������
* ��������CSShowOneFrameGif
* �������ܣ�����������ʾGIFͼƬ�����ڶ�֡��GIF��������ʾ��һҳ��
* ����˵����
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
* ����ֵ��
*		TRUE�������ɹ�
*		FALSE������ʧ��
* ע�����
*		�ޣ�
����������������������������������������������*/
BOOL CSShowOneFrameGif( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar );

/*����������������������������������������������
* ��������CSShowOneFrameGifFile
* �������ܣ�����������ʾGIFͼƬ�����ڶ�֡��GIF��������ʾ��һҳ��
* ����˵����
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
*		dwTransparentColor����IN���û�ָ������ɫ�����滻GIFͼƬ�е�͸��ɫ
* ����ֵ��
*		TRUE�������ɹ�
*		FALSE������ʧ��
* ע�����
*		�ޣ�
����������������������������������������������*/
BOOL CSShowOneFrameGifFile(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*����������������������������������������������
* ��������CSShowOneFrameGifEx
* �������ܣ�����������ʾGIFͼƬ�����ڶ�֡��GIF��������ʾ��һҳ��
* ����˵����
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		hShowGif����IN��GIFͼƬ��ʾ�ľ��
*		pShowGifPar����IN��GIFͼƬ��Ҫ���õĲ���
*		dwTransparentColor����IN���û�ָ������ɫ�����滻GIFͼƬ�е�͸��ɫ
* ����ֵ��
*		TRUE�������ɹ�
*		FALSE������ʧ��
* ע�����
*		�ޣ�
����������������������������������������������*/
BOOL CSShowOneFrameGifEx( PBYTE pucGifData, DWORD dwGifDataLen, HCSHANDLE hShowGif, ShowGifPar* pShowGifPar, DWORD dwTransparentColor);


/*����������������������������������������������
*�������ƣ�CSSetGifClipRectList
*�������ܣ�
*		����GIFͼƬˢ��ʱ�����ľ����б�
*����˵����
*		hShowGif   :��IN��GIFͼƬ��ʾ�ľ��
*		pCSRECTClipRectList :��IN��ָ������б��ָ��
*		nRectListNum:��IN�������б�ľ��θ���
*����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
����������������������������������������������*/
BOOL CSSetGifClipRectList(HCSHANDLE hShowGif, CSRECT *pCSRECTClipRectList, int nRectNum);


/*����������������������������������������������
*�������ƣ�CSClearGifClipRectList
*�������ܣ�
*		���GIFͼƬˢ��ʱ�����ľ����б�
*����˵����
*		hShowGif:��IN��GIFͼƬ��ʾ�ľ��
*����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
����������������������������������������������*/
BOOL CSClearGifClipRectList(HCSHANDLE hShowGif);


/*����������������������������������������������
*�������ƣ�CSShowNextFrameOfOneGif
*�������ܣ�
*		������ʾһ��GIFͼƬ����һ��,���Զ���GIF������Ч
*������
*		hShowGif: ��IN��GIFͼƬ��ʾ�ľ��
*		hOneGifHandle:��IN����CSShowGif���ص�ĳGIF������ʾ���
*����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
����������������������������������������������*/
BOOL CSShowNextFrameOfOneGif(HCSHANDLE hShowGif, HCSHANDLE hOneGifShow);


/*����������������������������������������������
*�������ƣ�CSShowNextFrameOfAllGif
*�������ܣ�
*		������ʾ����GIFͼƬ����һ��,���Զ���GIF������Ч
*������
*		hShowGif: ��IN��GIFͼƬ��ʾ�ľ��
*����ֵ��
*		�ɹ�������TRUE
*		ʧ�ܣ�����FALSE
����������������������������������������������*/
BOOL CSShowNextFrameOfAllGif(HCSHANDLE hShowGif);


/*����������������������������������������������
*�������ƣ�CSGetGifPicInfo
*�������ܣ�
*		���GIFͼƬ����Ϣ�������Ƿ�ʹ��͸��ɫ��ͼƬ��ȡ�ͼƬ�߶ȡ��Լ�ÿ�������ʱ
*������
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		pbUseTransparent: ��OUT���Ƿ�ʹ����͸��ɫ
*		pdwGifWidth: ��OUT��GIFͼƬ�Ŀ��
*		pdwGifHeight: ��OUT��GIFͼƬ�ĸ߶�
*		pdwDelayTime: ��OUT��GIFͼƬ��������ʱ
*����ֵ��
*		FALSE:��ȡʧ��
*		TRUE:��ȡ�ɹ�
����������������������������������������������*/
BOOL CSGetGifPicInfo(PBYTE pucGifData, DWORD dwGifDataLen, BOOL *pbUseTransparent, DWORD *pdwGifWidth, DWORD *pdwGifHeight, DWORD *pdwDelayTime);


/*����������������������������������������������
*�������ƣ�CSGetGifFileInfo
*�������ܣ�
*		�����RMFS�ļ�ϵͳ�ļ���ʽGIFͼƬ����Ϣ�������Ƿ�ʹ��͸��ɫ��ͼƬ��ȡ�ͼƬ�߶ȡ��Լ�ÿ�������ʱ
*������
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*		pbUseTransparent: ��OUT���Ƿ�ʹ����͸��ɫ
*		pdwGifWidth: ��OUT��GIFͼƬ�Ŀ��
*		pdwGifHeight: ��OUT��GIFͼƬ�ĸ߶�
*		pdwDelayTime: ��OUT��GIFͼƬ��������ʱ
*����ֵ��
*		FALSE:��ȡʧ��
*		TRUE: ��ȡ�ɹ�
*ע�����
*		�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
����������������������������������������������*/
BOOL CSGetGifFileInfo(LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, BOOL *pbUseTransparent, DWORD *pdwGifWidth, DWORD *pdwGifHeight, DWORD *pdwDelayTime);


/*����������������������������������������������
*�������ƣ�CSGetGifFrameCount
*�������ܣ�
*		��ȡGIFͼƬ��֡��
*������
*		hShowGif: ��IN��GIFͼƬ��ʾ�ľ��
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		nGifDataLen����IN��GIFͼƬ���ݵĴ�С
*����ֵ��
*		GIFͼƬ��֡�������Ϊ0���ʾ����һ��GIFͼƬ
* ע�����
*		�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
����������������������������������������������*/
int CSGetGifFrameCount(HCSHANDLE hShowGif, PBYTE pucGifData, int nGifDataLen);

/*����������������������������������������������
*�������ƣ�CSGetGifFileFrameCount
*�������ܣ�
*		��ȡ��RMFS�ļ�ϵͳ�ļ���ʽ�����GIFͼƬ��֡��
*������
*		hShowGif: ��IN��GIFͼƬ��ʾ�ľ��
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*����ֵ��
*		GIFͼƬ��֡�������Ϊ0���ʾ����һ��GIFͼƬ
* ע�����
*		�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
����������������������������������������������*/
int CSGetGifFileFrameCount(HCSHANDLE hShowGif, LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen);


/*����������������������������������������������
* ��������CSDecodeGif
* �������ܣ���GIFͼƬ��֡���뵽�û�ָ�����ڴ���
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ����
*		pucBitBuf����IN������GIFͼƬ��������ݵ��ڴ���ʼ��ַ
*		dwBitBufLen����IN������GIFͼƬ��������ݵ��ڴ��С
*		dwFrameCount����IN��GIFͼƬ����֡��
*		pucGifData����IN��GIFͼƬ���ݵ���ʼ��ַ
*		dwGifDataLen����IN��GIFͼƬ���ݵĴ�С
*		dwColorDepth����IN����ɫģʽ����16λ��32λɫ
* ����ֵ��
*		����ɹ�����TRUE�����򷵻�FALSE
* ע�����
*		�ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
����������������������������������������������*/
BOOL CSDecodeGif(HCSHANDLE hShowGif, PBYTE pucBitBuf, DWORD dwBitBufLen, DWORD dwFrameCount, PBYTE pucGifData, DWORD dwGifDataLen, DWORD dwColorDepth);


/*����������������������������������������������
* ��������CSDecodeGifFile
* �������ܣ�����RMFS�ļ�ϵͳ�ļ���ʽ�����GIFͼƬ��֡���뵽�û�ָ�����ڴ���
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ����
*		pucBitBuf����IN������GIFͼƬ��������ݵ��ڴ���ʼ��ַ
*		dwBitBufLen����IN������GIFͼƬ��������ݵ��ڴ��С
*		nFrameCount����IN��GIFͼƬ����֡��
*		lpFileName����IN��GIF�ļ�����COSHIP RMFS��
*		pucFileBufMemory����IN��GIFͼƬ���ݵĻ�������ַ
*		dwFileBufMemoryLen����IN��GIFͼƬ���ݵĻ�������С
*		dwColorDepth����IN����ɫģʽ����16λ��32λɫ
* ����ֵ��
*		����ɹ�����TRUE�����򷵻�FALSE
* ע�����
*       �ýӿ�Ϊcs_imageģ����ӣ�Ӧ���벻Ҫʹ��
����������������������������������������������*/
BOOL CSDecodeGifFile(HCSHANDLE hShowGif, PBYTE pucBitBuf, DWORD dwBitBufLen, DWORD dwFrameCount, LPCTSTR lpFileName, PBYTE pucFileBufMemory, DWORD dwFileBufMemoryLen, DWORD dwColorDepth);

/*����������������������������������������������
* ��������CSSetBkShowFlag
* �������ܣ�����RMFS�ļ�ϵͳ�ļ���ʽ�����GIFͼƬ��֡���뵽�û�ָ�����ڴ���
* ����˵����
*		hShowGif����IN��GIFͼƬ��ʾ�ľ����
*		bBkFlag����IN����ɽ��Ŀ��͸��GIFͼƬ��ʾ�����޸ģ�Ĭ������ΪTRUE�����۱����Ƿ�͸��������Ҫ���»��Ʊ�����FALSE�������͸���������򲻻��Ʊ���ɫ��
* ����ֵ��
*		�ɹ����÷���TRUE,���򷵻�FALSE
* ע�����
*       �ýӿ�Ϊ��ɽ��ĿN5200��Ŀ���ӣ�������Ŀ������Ҫʹ�øýӿ�
����������������������������������������������*/
BOOL CSSetBkShowFlag(HCSHANDLE hShowGif, BOOL bBkFlag);

#ifdef __cplusplus
}
#endif

#endif

