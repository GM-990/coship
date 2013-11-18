#ifndef CSSHELL_SUBTITLE_H
#define CSSHELL_SUBTITLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**@brief subtitle��������ṹ���� */
typedef struct _SubtitleRect
{
    int m_nX;                ///< �����������ϽǺ����� 
    int m_nY;                ///< �����������Ͻ������� 
	int m_nWidth;            ///< ���������� > 0
	int m_nHeight;           ///< ��������߶� > 0
}SUBTITELRECT;

/**
@brief �û�����subtitle������ʾ����Ļص����ûص�ͬʱ��Ϊ�������������ʾ�����subtitle���ݵĹ����е���
@param[in] �����н�������subtitle������ʾ�����������Ϣ
@param[out] �����û�������subtitle������ʾ�����������Ϣ
@notes
    �û����԰��մ˷�������ͼƬ�Ĵ�С��ˮƽ����    720 / 576 =5/4 
 	subtitleRect.m_nY = (5*psSubtitlePar->m_dwSrcY)/4;
	subtitleRect.m_nHeight  = (5*psSubtitlePar->m_dwSrcHeight)/4;
	subtitleRect.m_nWidth = (5*psSubtitlePar->m_dwSrcWidth)/4;
	subtitleRect.m_nX = (1280-subtitleRect->m_dwSrcWidth)/2;

*/
typedef void (* CSSubtitleTranslateRect_F)(SUBTITELRECT* subtitleRect);

/*@brief subtitle��ʼ������*/
typedef struct _CSShellSubtitleInitParam
{
	int 	m_nSize;			///<���ṹ���С������Ϊsizeof(CSShellSubtitleInitParam_S)
	void*	m_pvUserData;		///<�û����ݣ�Ŀǰ��δʹ��
	
	BYTE*	m_pvMemAddr;		///<subtitle�ڴ�صĵ�ַ
	int		m_nMemSize;			///< �ڴ�ش�С	
	
	int 	m_nThreadPriority;	///<�߳����ȼ�
	int		m_nStackSize;		///<�߳�ջ��С
	CSSubtitleTranslateRect_F m_fnSubtitleRect; ///<subtitle�����������ûص�

	BYTE*	m_pvDCIIMemAddr;	///<dcii subtitle�ڴ�صĵ�ַ
	int		m_nDCIIMemSize;		///<dcii subtitle�ڴ�ش�С

	int   m_nDemuxID_PVR;    //¼��ʱ����subtitle��ʱ�Ʋ��ź��ļ��ط�ʱ����subtitle���õ�demux ID����demux��֧��ע��
}CSShellSubtitleInitParam_S;

/**
@brief ����subtitle��ʼ������
@param [in] psSubtitle ��ʼ�������ṹ��ʵ��
@return �ɹ�����TRUE,���򷵻�FALSE
@note 
*/
BOOL CSShellSubtitleSetParam(const CSShellSubtitleInitParam_S* psSubtitle);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_SUBTITLE_H */

