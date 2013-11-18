#ifndef SubtitleInterface_H
#define SubtitleInterface_H

/* {{{USR

   }}}USR */

// {{{USR
#ifdef __cplusplus
extern "C" 
{
#endif
#include "mmcp_typedef.h"

typedef enum _SubtitleCallBackType
{
	SubtitleDispaly,			/*��ʾSubtitl*/
	SubtitleClear,				/*�������ʾ��Subtitl��Ϣ*/
	SubtitleClearAndDispaly		/*�������ʾ��Subtitl��Ϣ֮��,����ʾSubtitl*/
}SubtitleCallBackType;

//���ֵ�ǰsubtitle������
typedef enum _CSUSPSBTLType_E{
	EM_USPSBTL_DVBSubtitle = 0,/*����DVB��׼��Subtitle��ʽ����*/
	EM_USPSBTL_DCIISubtitle    /*����DCII��׼���н���*/
}CSUSPSBTLType_E;

typedef void(*SubtitleCallBack)(SubtitleCallBackType enmType, void* pParam);
/*
	enmType : �ص���Ϣ����,�ֱ�Ϊ:
		SubtitleDispaly
		SubtitleClear
		SubtitleClearAndDispaly

	����ϢΪSubtitleDispaly��SubtitleClearAndDispalyʱpParamΪָ��SUBTITLESHOWPARʵ����ָ��,��ʵ����Subtitle����֮ǰ��Ч,
	�����еĲ����ڵ���CSUSPSBTLShowFinished����������֮��,������Ч.�ڲ�����Ч֮��,������ʾSubtitle��Ϣ���ᵼ�¸�ģ��
	�ȶ�������,����ʾ����ϢΪ������Ϣ
*/

typedef struct TagPTSLL{
	DWORD high;
	DWORD low;
} PTSLL;// PTSLL;

typedef void*(*SubtitleCallBackForPts)(void* pParam);

typedef struct tagSUBTITLESHOWPAR
{
	PBYTE m_pucBitmapMemorySrc;				/*λͼ��Ϣ���׵�ַ*/
	DWORD m_nBitmapMemorySrcCount;			/*λͼ��Ϣ�ĳ���*/
	DWORD m_colorDepth;						/*λͼ��ɫλ��,ȡֵΪ1,2,4,8*/
	PBYTE m_pucPaletteData;					/*λͼ��ɫ����Ϣ���׵�ַ*/
	DWORD m_dwPaletteDataCount;				/*λͼ��ɫ����Ϣ�ĳ���*/
	DWORD m_dwSrcX;							/*λͼ��ʾλ��(X����)*/
	DWORD m_dwSrcY;							/*λͼ��ʾλ��(Y����)*/
	DWORD m_dwSrcWidth;						/*λͼ���*/
	DWORD m_dwSrcHeight;					/*λͼ�߶�*/
}SUBTITLESHOWPAR,*PSUBTITLESHOWPAR;

//DCIISubtitle��ʾ������ϵͳ
typedef struct{
	WORD wTopH;								/*���ϽǶ���x����*/                      
	WORD wTopV;								/*���ϽǶ���y����*/  
	WORD wBottomH;							/*���½Ƕ���x����*/  
	WORD wBottomV;							/*���½Ƕ���y����*/  
}CSUSPSBTLCoordinateSys_S;

//��Ļ��ʾ��ʽ
typedef enum _CSUSPSBTLDisplayStyle_E{
	EM_USPSBTL_NORMAL = 0,					/*��ͨ��ʾ��ʽ*/
	EM_USPSBTL_OUTLINE,    					/*���߿����ʾ��ʽ*/
	EM_USPSBTL_DROP_SHADOW,   				/*����Ӱ����ʾ��ʽ*/
	EM_USPSBTL_RESERVED       				/*Ԥ����ʾ��ʽ*/
}CSUSPSBTLDisplayStyle_E;

//subtitle�����ṹ��Ԫ
typedef struct {
	BOOL m_bBackgroundStyle;                 	  /*��Ļ����ģʽ��0:͸����1:�б���*/
	CSUSPSBTLDisplayStyle_E m_emOutlineStyle;     /*��Ļ��ʽ��0:��ͨ(none)��1:���߿�(outline)��2:����Ӱ(drop_shadow)*/
	DWORD m_dwCharacterColor;                     /*��Ļ��ɫ(RGBA)*/
	CSUSPSBTLCoordinateSys_S m_stBmpCoordinate;   /*��Ļ��ʾλ��*/
	DWORD m_dwFrameColor;                         /*������ɫ(RGBA)�����ڱ���ģʽΪ1ʱ��Ч*/
	CSUSPSBTLCoordinateSys_S m_stFrameCoordinate; /*������ʾλ�ã����ڱ���ģʽΪ1ʱ��Ч*/
	DWORD m_dwOutlineThickness;                   /*�߿��ȣ�������Ļ��ʽΪ1ʱ��Ч*/
	DWORD m_dwOutlineColor;                       /*�߿���ɫ(RGBA)��������Ļ��ʽΪ1ʱ��Ч*/
	DWORD m_dwShadowRight;                        /*��Ӱ���ҵ�ƫ������������Ļ��ʽΪ2ʱ��Ч*/
	DWORD m_dwShadowBottom;                       /*��Ӱ���µ�ƫ������������Ļ��ʽΪ2ʱ��Ч*/
	DWORD m_dwShadowColor;                        /*��Ӱ���ֵ���ɫ(RGBA)��������Ļ��ʽΪ2ʱ��Ч*/
	DWORD m_dwBmpLength;                          /*ѹ��λͼ�����ݳ���*/
	PBYTE m_pucBmpMemorySrc;                      /*ѹ��λͼ��Ϣ���׵�ַ*/
	DWORD m_dwWindowWidth;						  /*�ο�����ϵ���*/
	DWORD m_dwWindowHeight;						  /*�ο�����ϵ�߶�*/
}CSUSPSBTLDCIIShowPar_S;

// }}}USR

/* {{{USR
CSUSPSBTLCreate
	����Subtitle,֧�ֶ��ʵ��

	HCSHANDLE CSUSPSBTLCreate(
		HCSHANDLE hMemoryPool,
		HCSHANDLE hDemuxHandle,
		HCSHANDLE hSectionSource,
		int nDispatchTaskIndex
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��;�ڴ�ش�С��ͨ��CSUSPSBTLGetMemorySizeScope��ȡ

	hDemuxHandle
		[in]Demux��ʶ

	hSectionSource
		[in]SectionSource���

	nDispatchTaskIndex
		[in]�ַ���������
		
Return Values
		1������ɹ�����Subtitle���
		2�����ʧ�ܷ���ΪNULL

		��hMemoryPoolΪ�ջ�hSectionSource��Ч��nBuffSize<8192ʱ������������ɹ�
   }}}USR */
HCSHANDLE CSUSPSBTLCreate( HCSHANDLE hMemoryPool, HCSHANDLE hDemuxHandle, HCSHANDLE hSectionSource, int nDispatchTaskIndex );

/* {{{USR
CSUSPSBTLCreateEx
	���ݴ����subtitle���ͣ�����Subtitle,֧�ֶ��ʵ��

	HCSHANDLE CSUSPSBTLCreateEx(
		HCSHANDLE hMemoryPool,
		HCSHANDLE hDemuxHandle,
		HCSHANDLE hSectionSource,
		int nDispatchTaskIndex,
		CSUSPSBTLType_E emSubType
	};	

Parameters
	
	hMemoryPool
		[in]�ڴ�ؾ��;�ڴ�ش�С��ͨ��CSUSPSBTLGetMemorySizeScope��ȡ

	hDemuxHandle
		[in]Demux��ʶ

	hSectionSource
		[in]SectionSource���

	nDispatchTaskIndex
		[in]�ַ���������

	emSubType
		[in]subtitle ������
		
Return Values
		1������ɹ�����Subtitle���
		2�����ʧ�ܷ���ΪNULL

		��hMemoryPoolΪ�ջ�hSectionSource��Ч��nBuffSize<8192ʱ������������ɹ�
   }}}USR */
HCSHANDLE CSUSPSBTLCreateEx( HCSHANDLE hMemoryPool, HCSHANDLE hDemuxHandle, HCSHANDLE hSectionSource, int nDispatchTaskIndex, CSUSPSBTLType_E emSubType);

/* {{{USR
 CSUSPSBTLDestroy

    ɾ��SubTitleģ��ľ��. 
    �������hSubtitleΪNULL��Ƿ�ָ�룬��������FALSE�� 

Parameters

     hSubtitle 
       [In]: Subtitle ģ����

RETURN VALUE  
     BOOL TRUE��ʾɾ���ɹ���FALSE��ʾɾ��ʧ��

   }}}USR */
BOOL CSUSPSBTLDestroy( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLSetCallBack
    ����Subtitle���������Subtitle������ǰ�����ô˽ӿ�ע���û��ص�������
    �������SubtitleCallBack��pSubtitleCallBackΪNULL������FALSE����ʾע��ʧ�ܣ�
	���ݽ��޷�������ʾ��

Parameter  
      hSubtitle
        [In]: Subtitleģ���� 

     pSubtitleCallBack 
        [In]: Subtitleģ��Ļص����������

RETURN VALUE  
     BOOL TRUE��ʾע��ɹ���FALSE��ʾע��ʧ��
   }}}USR */
BOOL CSUSPSBTLSetCallBack( HCSHANDLE hSubtitle, SubtitleCallBack pSubtitleCallBack );

/* {{{USR
 CSUSPSBTLStart
    ����Subtitle���ݵ����������������Ĺ����и��û��ص�Subtitle���ݡ���
    �������hSubtitleΪNULL��Ƿ�ָ�룬����FALSE����ʾ����ʧ�ܡ�

Parameter  
    hSubtitle
       [In]: Subtitleģ���� 

RETURN VALUE  
    BOOL��TRUE��ʾ�����ɹ���FALSE��ʾ����ʧ��
   }}}USR */
BOOL CSUSPSBTLStart( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLStartEx
    ����Subtitle���ݵ����������������Ĺ����и��û��ص�Subtitle���ݡ���
    �������hSubtitleΪNULL��Ƿ�ָ�룬����FALSE����ʾ����ʧ�ܡ�

Parameter  
    hSubtitle
       [In]: Subtitleģ���� 
    hDemuxHandle
       [In]: ָ��demux ID

RETURN VALUE  
    BOOL��TRUE��ʾ�����ɹ���FALSE��ʾ����ʧ��
   }}}USR */
BOOL CSUSPSBTLStartEx( HCSHANDLE hSubtitle, HCSHANDLE hDemuxHandle );

/* {{{USR
 CSUSPSBTLStop
    ֹͣSubtitle����������ֹͣSection ��������
    �������hSubtitleΪNULL��Ƿ�ָ�룬����FALSE����ʾֹͣʧ�ܡ�

 Parameter  
    hSubtitle
       [In]: Subtitleģ���� 

 RETURN VALUE  
    BOOL��TRUE��ʾ�����ɹ���FALSE��ʾ����ʧ��
   }}}USR */
BOOL CSUSPSBTLStop( HCSHANDLE hSubtitle );

/* {{{USR
 CSUSPSBTLSetPar
    ����Subtitle���������Subtitle������ǰ�����ô˽ӿ�����Subtitle�Ĳ���ֵ�� 
    �������hSubtitleΪNULL��Ƿ�ָ�룬����FALSE����ʾ���ò���ʧ�ܡ� 
    ��������ò���������Subtitle����������û���κ�������ʾ��

Parameter  
    hSubtitle
      [In]:Subtitleģ���� 
 
    nPID 
      [In]:Subtitle������TS���е�PIDֵ������ֵ�ӣУͣԵ��������л�� 

    nPageID
      [In]:Subtitleҳ���IDֵ������ֵ�ӣУͣԵ��������л�� 

    nAncillaryPageID 
      [In]:Subtitle��ҳ���IDֵ������ֵ�ӣУͣԵ��������л��

RETURN VALUE  
    BOOL TRUE��ʾ���óɹ���FALSE��ʾ����ʧ��
   }}}USR */
BOOL CSUSPSBTLSetPar( HCSHANDLE hSubtitle, int nPID, int nPageID, int nAncillaryPageID );

/* {{{USR
 CSUSPSBTLShowFinished
      ���û�������һ��������ʾ�Ļص�����ʱ��������ô˽ӿ��ͷŴ˴λص�Subtitleռ�õ���Դ��
	����CSUSPSBTLShowFinished����������֮��,���ص�����pParam����ֵ����Ч.�ڲ�����Ч֮��,
	������ʾSubtitle��Ϣ���ᵼ�¸�ģ���ȶ�������  ����ʾ����ϢΪ������Ϣ��
      �������hSubtitleΪNULL��Ƿ�ָ�룬����FALSE����ʾ����ʧ�ܡ�

 Parameter  
    hSubtitle
       [In]: Subtitleģ���� 

 RETURN VALUE  
   BOOL��TRUE��ʾ�ͷųɹ���FALSE��ʾ�ͷ�ʧ�� 
   }}}USR */
BOOL CSUSPSBTLShowFinished( HCSHANDLE hSubtitle );

/* {{{USR

   }}}USR */
BOOL CSUSPSBTLSetThreadParam( HCSHANDLE hSubtitle, DWORD dwPriority, DWORD dwStackCount, void * pParam );

/* {{{USR
 CSUSPSBTLGetMemorySizeScope
    �õ�Subtitleʹ���ڴ����������ֵ����Сֵ��
    �������pnMinSize��pnMaxSizeΪNULL���򷵻�FALSE��

 Parameter  
    pnMinSize
      [In]:ʹ���ڴ���Сֵ

    pnMaxSize��
      [In]: ʹ���ڴ����ֵ

 RETURN VALUE  
    BOOL��TRUE��ʾ�����ɹ���FALSE��ʾ����ʧ��
   }}}USR */
BOOL CSUSPSBTLGetMemorySizeScope( int * pnMinSize, int * pnMaxSize );

/* {{{USR
 CSUSPSBTLGetVersionInfo
   �õ�TELEģ��İ汾����Ϣ�����ذ汾��Ϣ�ĳ��ȡ�
   �������pcVersionInfoΪNULL���򷵻ذ汾��Ϣ�ĳ��ȡ�

 Parameter  
   pcVersionInfo
     [In]:���ڴ洢�汾��Ϣ�����顡

   nSize
     [In]: pcVersionInfo ���鳤��

 RETURN VALUE  
   �汾��Ϣ�ĳ��ȡ�
   }}}USR */
int CSUSPSBTLGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
CSUSPSBTLSetIsNeedSyncByPts
����Subtitleģ���Ƿ���Ҫ����ʵʱ��PTSͬ����Ļ����ʾ��

Parameter  
	hSubtitle
		[In]:ģ������

	bIsNeedSync
		[In]: �Ƿ���Ҫ����PTS��ͬ�����ܣ�TRUE����Ҫ��FALSE������Ҫ

	pCallbackForPts
		[In]:�ص����������ϲ�Ӧ��ʵ�֣�ͨ���ú�����ȡʵʱPTS���ݣ���bIsNeedSync����ΪFALSE����ֵ��Ч�����Դ�NULL��

RETURN VALUE  
	TRUE�� ���óɹ�
	FALSE������ʧ��
}}}USR */
BOOL CSUSPSBTLSetIsNeedSyncByPts( HCSHANDLE hSubtitle, BOOL bIsNeedSync, SubtitleCallBackForPts pCallbackForPts );

// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* SubtitleInterface_H */
