#ifndef USPTeletextInterface_H
#define USPTeletextInterface_H

#include "mmcp_typedef.h"
#include "mwapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	VBI_MODE,	           //VBI��ʾ��ʽ
	STB_MODE               //�����н��뷽ʽ

}DisplayPattern;

typedef enum _TELECallbackType
{
	START_DISPLAY,         //���濪ʼ��ʾ
	PAGE_UPDATE,           //ҳ�����
	LINK_FLASH,            //Linkҳ��˸
	PAGE_FLASH,             //һ��ҳ������˸
	CAPTION_PAGE           //��ʾClose Captionҳ

}TELECallbackType;

typedef struct
{
	int nLeft;
	int nRight;
	int nTop;
	int nBottom;
}CSTELERect_S;

typedef struct
{
	long lCx;
	long lCy;
}CSTELESize_S;

typedef struct
{
	int nClientId;
	GR_WINDOW_ID nWinId;
	GR_GC_ID nGcId;
}CSTELEContext_S;

//��MicroWin�ж���һ��
#define TELE_RGBA(r,g,b,a)  ((DWORD)(((unsigned long)a<<24)|(((unsigned char)(b)|(unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(r))<<16)))
//��GUI�ж���һ��
//#define TELE_RGBA(r,g,b,a)  ((DWORD)(((unsigned long)a<<24)|(((unsigned char)(r)|(unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

typedef void(*TELECallback)(TELECallbackType enmType, void* pParam);

/* {{{USR
����Teletextģ����,ʹ��GUI��ͼ
Parameter:
   [in]munDisplayPattern   ������ʾģʽ��VBI_MODE �� STB_MODE
   [in]pCallback            �û�ע��Ļص��������
   [in]hSectionSource       Sectionģ����
   [in]hMemoryPool           �ڴ�ؾ��
   [in]nDispatchTaskIndex   Section�ַ������ȼ�
   [in]nDemaxID             Demax�豸��IDֵ

return :
   HCSHANDLE  Teletextģ����
   }}}USR */
HCSHANDLE CSUSPCreateTELEHandle( DisplayPattern munDisplayPattern, TELECallback pCallback, HCSHANDLE hSectionSource, HCSHANDLE hMemoryPool, int nDispatchTaskIndex, int nDemuxID);

/* {{{USR
����Teletextģ����,ʹ��MicroWin��ͼ
Parameter:
   [in]munDisplayPattern   ������ʾģʽ��VBI_MODE �� STB_MODE
   [in]pCallback            �û�ע��Ļص��������
   [in]hSectionSource       Sectionģ����
   [in]hMemoryPool           �ڴ�ؾ��
   [in]nDispatchTaskIndex   Section�ַ������ȼ�
   [in]nDemaxID             Demax�豸��IDֵ

return :
   HCSHANDLE  Teletextģ����
   }}}USR */
HCSHANDLE CSUSPCreateMWTELEHandle( DisplayPattern munDisplayPattern, TELECallback pCallback, HCSHANDLE hSectionSource, HCSHANDLE hMemoryPool, int nDispatchTaskIndex, int nDemuxID);

/* {{{USR
ɾ��Teletextģ��ľ��

Parameter:
    [in]hTeletextHandle   Teletextģ����
    
   }}}USR */
BOOL CSUSPDestroyTELEHandle( HCSHANDLE hTeletextHandle );

/* {{{USR
����Teletext�Ľ��գ�������ʾ��Ҫ���߳�

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]nTeletextPID     Teletext������PIDֵ

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveTeletext( HCSHANDLE hTeletextHandle, int nTeletextPID, int nMagazineNum, int nPageNum );

/* {{{USR
ֹͣTeletext�Ľ���,ֹͣ����Teletext�������߳�

Parameter:
    [in]hTeletextHandle   Teletextģ����

return:
    BOOL
   }}}USR */
BOOL CSUSPStopReceiveTeletext( HCSHANDLE hTeletextHandle );

/* {{{USR
��ʾָ��ҳ��Teletext���ݡ�

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������
    [in]ucPageNum    ҳ��

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowSelectPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, BYTE ucPageNum );

/* {{{USR
��ʾ��ǰҳ���һ��ҳ��Teletext����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowDownPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
��ʾ��ǰҳ���һ��ҳ��Teletext����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowUpPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
��ʾ�����ʾ����Teletextҳ����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowPreShowPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
��ʾ��ǰҳ��Teletext����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowCurPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
��ʾ�û�ѡ���Linkҳ���ҳ����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowLinkPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
ʹ��˸�����Ƶ���һ��Linkҳ��

Parameter:
    [in]hTeletextHandle   Teletextģ����

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEGotoLinkgeDownPage( HCSHANDLE hTeletextHandle );

/* {{{USR
ʹ��˸�����Ƶ���һ��Linkҳ��

Parameter:
    [in]hTeletextHandle   Teletextģ����

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEGotoLinkgeUpPage( HCSHANDLE hTeletextHandle );

/* {{{USR
��250����ִ��һ�κ��� �ı�Link����ɫ

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEChangeLinkPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
ע��VBI��ʾģʽ�Ļص�����
   }}}USR */
BOOL CSUSPTELERegVBICallbackFun( HCSHANDLE hTeletextHandle );

/* {{{USR
ע���ֿ⼯��Teletextģ��

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]nFontCount        �ֿ����ĸ���
    [in]hFontArray        �����ֿ���������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELERegisterFont( HCSHANDLE hTeletextHandle, int nFontCount, HCSHANDLE * hFontArray );

/* {{{USR
��250����ִ��һ�κ��� �ı���Ҫ��˸�����ݵ���ɫ

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEChangeFlashPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
���ҳ���һ�е�ҳ��ֵ

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEClearTitle( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
��ҳ��ĵ�һ����ʾ��ǰʱ��

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������
    [in]pucTime     ����ʱ�����ݵ��ַ���
    [in]nTimeLen    pucTime�ַ����ĳ���

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEDisplayTime( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, BYTE * pucTime, int nTimeLen );

/* {{{USR
��������һҳ����֮ǰ��ʾ����������ʾ

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]hDC         HDC���
    [in]tShowRect   ��ʾ������

return:
    BOOL
   }}}USR */
BOOL CSUSPTELEdisplaySearchTitle( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
����Teletext�Ľ��գ�������ʾ��Ҫ���߳�

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]nTeletextPID     Teletext������PIDֵ

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveCaption( HCSHANDLE hTeletextHandle, int nCaptionPID, int nMagazineNum, int nPageNum );

/* {{{USR

   }}}USR */
BOOL CSUDPTELESetRowHeight( HCSHANDLE hTeletextHandle, DWORD dwHeight );

/* {{{USR
��ʾClose Captionҳ��
   }}}USR */
BOOL CSUSPShowCaptionPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR
�õ�TELEģ��İ汾����Ϣ��
   }}}USR */
int CSUSPTELEGetVersionInfo( char * pcVersionInfo, int nSize );

/* {{{USR
�õ�TELEʹ���ڴ����������ֵ����Сֵ��
   }}}USR */
BOOL CSUSPTELEGetMemInfo( int * pnMin, int * pnMax );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetBGColor( HCSHANDLE hTeletextHandle, DWORD dwGBColor, DWORD dwTransparency );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetString( HCSHANDLE hTeletextHandle, char * pString1, int nString1Len, char * pString2, int nString2Len );

/* {{{USR

   }}}USR */
BOOL CSUSPTELEIsSetEmptyRowBColor( HCSHANDLE hTeletextHandle, BOOL bIsSetBColor );

/* {{{USR
��ʾ��ǰҳ��Teletext����

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [HCSDC]hDC			  HDC���
    [CSRECT *]tShowRect   ��ʾ������
	[int]nColorIndex      ��ɫ����ɫ��ţ�Ϊ0��1��2��3,��Ӧ����ɫΪ�졢�̡��ơ���
return:
    BOOL
   }}}USR */
BOOL CSUSPTELEShowFastextPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, int nColorIndex );

/* {{{USR
������ǰ��ʾ��ҳ�롣

hTeletextHandle [in]:
		������Teletext�����
*wPageNum [out]:
		������ǰ��ʾ��ҳ�롣

return:
BOOL
   }}}USR */
BOOL CSUSPTELEGetCurPage( HCSHANDLE hTeletextHandle, WORD * wPageNum );

/* {{{USR
�ӵ�ǰҳ��ʼ��ת�û�ָ����ҳ��

hTeletextHandle [in]
		Teletext���
hDC [in]
		�豸����������
*tShowRect [in]:
		��ʾ����
nJumpNum [in]:
		��Ҫ�ӵ�ǰҳ��ת��ҳ����������Ϊ��ֵ��������Ϊ��ֵ�������Ҫ��ת��ҳ�볬������ʾ�����ֵ������ʾ��ʼҳ������Ҫ��ת��ҳ��С����ʼҳ��Ҳ��ʾ��ʼҳ��
		���������Ҫ�ӵ�ǰҳ��ת����100ҳ������Ϊ100������Ҫ�ӵ�ǰҳ��ת��ǰ100ҳ������Ϊ-100��

return:
BOOL
   }}}USR */
BOOL CSUSPTELEJumpPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect, int nJumpNum );

/* {{{USR
��ʾTTX��TV Station��ʱ��

hTeletextHandle [in]
Teletext���
bDisplayHead [in]:
�Ƿ���ʾTV Station��ʱ���־��ΪTRUE��ʾ��ΪFALSE����ʾ��Ĭ��ΪFALSE

return:
BOOL
   }}}USR */
BOOL CSUSPTELEDisplayPageHead( HCSHANDLE hTeletextHandle, BOOL bDisplayHead );

/* {{{USR
����teletext��ˢ��Ƶ�ʣ�Ĭ��Ƶ����ÿ3����ˢ��һ�Σ������ڳ�ʼ��Teletext�ӿ�֮�󣬵�һ�ε��ýӿ�CSUSPStartReceiveTeletext֮ǰ���ô˽ӿڲ���Ч

nTime [in]:
ÿ��������ˢ��һ�Σ������ó�5��ʾÿ��5����ˢ��һ��
   }}}USR */
BOOL CSUSPTELESetRefreshFre( HCSHANDLE hTeletext, int nTime );

/* {{{USR
���ñ���ȫ��800ҳ���ݱ�־��Ĭ��ֻ���浱ǰ100ҳ�����ݣ������ڳ�ʼ��Teletext�ӿ�֮ǰ����

bFlag [in]:
TRUE ��ʾ����800ҳ��ע���ʱҪ��Teletext������ڴ�������1200k��
FALSE ��ʾֻ���浱ǰ100ҳ
   }}}USR */
BOOL CSUSPTELESetSaveAllFlag( BOOL bFlag );

/* {{{USR
����Teletext�Ľ��գ�������ʾ��Ҫ���߳�

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]nTeletextPID     Teletext������PIDֵ

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveTeletextExt( HCSHANDLE hTeletextHandle, int nTeletextPID, int nMagazineNum, int nPageNum, int nDemuxId );

/* {{{USR
����Teletext�Ľ��գ�������ʾ��Ҫ���߳�

Parameter:
    [in]hTeletextHandle   Teletextģ����
    [in]nTeletextPID     Teletext������PIDֵ

return:
    BOOL
   }}}USR */
BOOL CSUSPStartReceiveCaptionExt( HCSHANDLE hTeletextHandle, int nCaptionPID, int nMagazineNum, int nPageNum, int nDemuxId );

BOOL CSUSPTELEShowNextChildPage( HCSHANDLE hTeletextHandle, HCSHANDLE hContext, CSTELERect_S * tShowRect );

/* {{{USR

   }}}USR */
BOOL CSUSPTELESetStepShowFlag( HCSHANDLE hTeletextHandle, BOOL bFlag );

//ΪTRUE����ʾ��ҳ����ʾ
//ΪFALSE����ʾ��ҳ��ʾ�ϸ�ҳ������
BOOL CSUSPTELESetShowEmptyPageMode( HCSHANDLE hTeletextHandle, BOOL bMode );

/* {{{USR
����CCExt�ľ��
Parameter:
��
return:
    �����ľ��
   }}}USR */
HCSHANDLE CSUSPCCExtCreateHandle( void );

/* {{{USR
������������ʾ��VIDEO PES�л�ȡ��CC����
Parameter:
	hCCExtHandle: �Ѿ�������CCExt���
	psWinContex: OSD�����ʹ�õĴ�����Ϣ
	nChannel :  ѡ��Χ��1��4��Ĭ��Ϊchannel1
	unBackgroundColor:��Ļ�ı���ɫ��Ĭ��Ϊȫ�ڣ���ʽΪARGB
return:0����ɹ���-1��ʾʧ��

   }}}USR */
int CSUSPCCExtStart( HCSHANDLE hCCExtHandle, CSTELEContext_S *psWinContex, int nChannel, int nBackgroundColor );

/* {{{USR
ֹͣ��������ʾ��VIDEO PES�л�ȡ��CC����
Parameter:
��
return:0����ɹ���-1��ʾʧ��
    �����ľ��
   }}}USR */
int CSUSPCCExtStop( HCSHANDLE hCCExtHandle );

/* {{{USR
����CCExt�ľ��
Parameter:
��
return:
    ��
   }}}USR */
void CSUSPCCExtClose( void );


// {{{USR
#ifdef __cplusplus
}
#endif
// }}}USR

#endif /* USPTeletextInterface_H */
