#ifndef QTADAPTER_H
#define QTADAPTER_H


#include "udi2_osg.h"
#include "udi2_error.h"
#include "mmcp_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

/*
QT���ڽӿ���Ϣ�ṹ��

m_dwPara0: �����豸��Դ����
	�μ�udi2_input.h��CSUDIInputType_E
	Ŀǰ��֧��ң������ǰ���
	
m_dwPara1: ���밴��״̬����
	�μ�udi2_input.h��CSUDIInputKeyStatus_E
	
m_dwPara2: ������ֵ
	�μ�udi2_input.h�еļ�ֵ����
	
m_dwPara3: �û�����usrInt
	Ŀǰ����ʹ��
*/
typedef struct  _QtEvent_S
{
	unsigned int m_dwPara0;	
  	unsigned int m_dwPara1;
	unsigned int m_dwPara2;
	unsigned int m_dwPara3;
}QtEvent_S;

/*
QT��udi��Ϣӳ��ṹ��

m_nUDIKeyCode: udi2�ļ�ֵ
	�μ�udi2_input.h�еļ�ֵ����
	
m_nQTKeyCode: QT�еļ�ֵ
	�μ�qnamespace.h
	
m_bSpecial: �ü�ֵ�Ƿ�Ϊ�����ֵ
	�˴��������ʾ�ڷ�QT��postEvent��Ϣʱ��textֵ�Ƿ�Ϊ��
	Ŀǰ��֪��Ϊ���������ĸ�����Ϊ���ܼ���QT�ڲ��������⴦��
*/
typedef struct  _QtUdiKeyMapElement_S
{
	unsigned int m_nUDIKeyCode;	
  	unsigned int m_nQTKeyCode;
	BOOL 		 m_bSpecial;
}QtUdiKeyMapElement_S;



typedef	void *	CSQT_HANDLE;

CSUDI_Error_Code QTMWInit(void);

CSUDI_Error_Code QTMWCreateSurface(CSUDIOSGPixelFormat_E ePixelFormat,int nWidth, int nHeight, CSQT_HANDLE * phSurface);

CSUDI_Error_Code QTMWDestroySurface(CSQT_HANDLE hSurface);

CSUDI_Error_Code QTMWGetSurfaceInfo(CSQT_HANDLE hSurface, CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

CSUDI_Error_Code QTMWSetSurfaceInfo(CSQT_HANDLE hSurface, const CSUDIOSGSurfaceInfo_S * psSurfaceInfo);

CSUDI_Error_Code QTMWGetDisplaySurface(int nIndex, CSQT_HANDLE * phSurface);

CSUDI_Error_Code QTMWBlit(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect,CSQT_HANDLE hSrcSurface, const CSUDIOSGRect_S * psSrcRect, CSUDIOSGOpMode_E eOpMode);

CSUDI_Error_Code QTMWFill(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, unsigned int uPixel);

CSUDI_Error_Code QTMWSync(void);

CSUDI_Error_Code QTMWHideDisplay(void);

CSUDI_Error_Code QTMWShowDisplay(int nX, int nY, int nWidth, int nHeight);

CSUDI_Error_Code QTMWDispatchFilterKeyEvent(QtEvent_S sEvent);
CSUDI_Error_Code QTMWRaiseWidget(BOOL isRaise);

CSUDI_Error_Code QTMWSoftRefresh(CSQT_HANDLE hDstSurface, const CSUDIOSGRect_S * psDstRect, const CSUDIOSGRect_S * psSrcRect);
void QTMWWidgetOnBrowser(HCSHANDLE hWebview, BOOL bOnBrowser);

/**
@brief  ��������:		
	�� webview ��ȡ2D Canvas������С��ʵ��2D�������ͬʱ��ͼ/����
@param[in] 
    hWebivew : 2D��Ҫ����webivew
	x, y, w, h: 2D Canvas������С
@exception
@note  
	ֻ������� GUI��ʹ�ã�ֻ��2D��ʹ��, �˳�2Dʱ�������QTMWSet2DCanvasRect��������С���0
*/
void QTMWSet2DCanvasRect(HCSHANDLE hWebivew, int x, int  y,  int width,  int height);
/**
@brief  ��������:
        ���QT��JAVA����ֵ�ļ�ֵmap��

@param[in]
int(* pnKeyTableAddr)[2]    ��ά��ֵ��ָ��
int nFirstDimension         ��ֵ��ӳ���ֵ�ĸ���

@return
int         �����ֵ��ָ��Ϊ�ջ��ߴ���ӳ���ֵ����ֵС�ڵ���0�� ���� 0
            �����ֵ��ɹ���                                    ���� ��ֵ����ӳ���ֵ�ĸ���

@exception
@note
*/
int CSJavaMapKeyTable(int(* pnKeyTableAddr)[2], int nFirstDimension);

/**
@brief  ��������:
        �Ӽ�ֵ����ȡ��QT��JAVA�������ľ���ӳ����ļ�ֵ

@param[in]
int nKey    ӳ��ǰ�ļ�ֵ

@return
int         ӳ�����ļ�ֵ����Ϊ 0��  ����   0
            �����ֵ��ӳ����в����ڣ�  ����   0
            �ɹ�ӳ�����                ����   ӳ���ļ�ֵ

@exception
@note
*/
int CSJavaMapKey(int nKey);

int qtadaptermain(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
#endif
