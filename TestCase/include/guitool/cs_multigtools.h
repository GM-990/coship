/**@defgroup GUITOOL 
@brief 
@note Copyright (c) 2008 Coship
@version 1.0  2008-03-04  hjl             Create
@{
*/

#ifndef CSMULTIGTOOLS_H
#define CSMULTIGTOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WEBKIT_EXPORTS
#define WEBKIT_API __declspec(dllexport)
#else
#define WEBKIT_API 
#endif

/******************************************************************************
 *                                 Header File Include                         *
 ******************************************************************************/
#include "mmcp_typedef.h"
#include "cs_gtools.h"
/******************************************************************************
 *                                 Macro/Define/Structus                       *
 ******************************************************************************/
typedef struct _CSGTInitConfig
{
	char * pBufferAddr;			
	int nBufferSize;			// ��ʼ��������ڴ�ش�С		
	int nCacheBufferSize;		//  ��ʼ������������С	
}CSGTInitConfig_S;

/**
@brief �������ʼ���ӿ�
return 0  		��ʼ��ʧ��
return > 0 	��ʼ���ɹ�
*/
HCSHANDLE CSGTMultiInit(CSGTInitConfig_S* pstInitCfg);

/**
@brief �����������ѡ����
@param[in] HCSHANDLE hHandle = 0;
@param[in] CSGTOption_E enmOption  �μ�CSGTOption_E ˵��
@param[in] void *pvValue
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
CSGTRet_E  CSGTMultiSetOpt(HCSHANDLE hHandle,CSGTOption_E enmOption, void *pvValue);

/**
@brief ָ�����ø�������Ĵ���
@param[in] HCSHANDLE hWid
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
CSGTRet_E CSGTMultiSetWinId(HCSHANDLE hWid);

/**
@brief Ӧ��������������¼���Ϣ��
@param[in] HCSHANDLE hGtPageHandle hGtPageHandle = 0 ,������Ϣ����ǰ���� 
								   hGtPageHandle > 0 ,������Ϣ��ָ������
@param[in] sEvent ������Ϣ��
@return �ɹ�ʱ����GT_RET_SUCCESS,ʧ�ܷ���GT_RET_FAILURE��
@note �ο�����:
@note ������Ϣ:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = nKeyCode; // ��ֵ
@note sEvent.m_dwPara1 = nEventType; // ��������
@note sEvent.m_dwPara2 = nEventAction; // ��������
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMultiPostMsg(sEvent);

@note ������Ϣ:
@note CSBRWEvent sEvent;
@note sEvent.m_dwPara0 = BRW_SYSTEM_TYPE_GOBACK;
@note sEvent.m_dwPara1 = BRW_EVENT_TYPE_SYSTEM;
@note sEvent.m_dwPara2 = 0;
@note sEvent.m_dwPara3 = 0;
@note sEvent.m_dwPara4 = 0;
@note CSGTMultiPostMsg(sEvent);
*/
CSGTRet_E CSGTMultiPostMsg(HCSHANDLE hGtPageHandle, CSBRWEvent sEvent);

/**
@brief ָ������ page
@return     HCSHANDLE ����page�ľ��

@note ������Ϣ:
@note CSBRWRect_S sRect;
@note sRect.nX = 0;
@note sRect.nY = 0;
@note sRect.nWidth = 1280;
@note sRect.nHeight = 720;
@note ���õ����������ƽ̨֧�ֿ��Դ�����
*/
HCSHANDLE  CSGTMultiCreatePage(CSBRWRect_S sRect);

/**
@brief ͬ������ page
@return     HCSHANDLE ����page�ľ��

@note ��������̲߳ſ��Ե��ô˽ӿ�
@note CSBRWRect_S sRect;
@note sRect.nX = 0;
@note sRect.nY = 0;
@note sRect.nWidth = 1280;
@note sRect.nHeight = 720;
@note ���õ����������ƽ̨֧�ֿ��Դ�����
*/
HCSHANDLE  CSGTMultiCreatePage_S(CSBRWRect_S sRect);

/**
@brief ��һ����ҳ ��
@param[in] HCSHANDLE hGtPageHandle hGtPageHandle == 0 �ڵ�ǰ���ڴ���ҳ 
								   hGtPageHandle >    0 (������CSGTMultiCreatePage����page),��ָ�����ڴ���ҳ
@param[in] pcUrl  ���Է��ʵ���ҳ pcUrl( ascall ��)��
@param[in] nLen pcUrl �ĳ��ȡ�
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
@note �ޡ�
*/
CSGTRet_E CSGTMultiOpenURL(HCSHANDLE hGtPageHandle, const char *pcUrl,int nLen);	

/**
@brief ��̬����������Ĵ��ڣ���Ҫ�ڽ����˳������������ʱ����Ҫ�ʵ��ĵ�������
@param[in] HCSHANDLE hClient
@param[in] HCSHANDLE hWindow
@param[in] int nScreenWidth
@param[in] int nScreenHeight
@return     SUCCESS :  GT_RET_SUCCESS;    FAILURE :  GT_RET_FAILURE��
*/
CSGTRet_E  CSGTMultiAdjustWin(HCSHANDLE hClient, HCSHANDLE hWindow, int nScreenWidth, int nScreenHeight); 
/**
@brief ��ȡ��ǰ��������е�pageCount
*/
int CSGTMultiGetPageCount();

/**
@brief ��ȡ��ǰ����������page
*/
HCSHANDLE   CSGTMultiGetCurActivePage();

/**
@brief ����ָ����page
*/
CSGTRet_E    CSGTMultiHidePage(HCSHANDLE hGtPageHandle);

/**
@brief ����ָ����page,����ֹͣ������Ϣ
*/
CSGTRet_E    CSGTMultiStopPage(HCSHANDLE hGtPageHandle);

/**
@brief ����ָ����page
*/
CSGTRet_E   CSGTMultiActivePage(HCSHANDLE hGtPageHandle);	

/**
@brief ����ָ����page
*/
CSGTRet_E   CSGTMultiDestroyPage(HCSHANDLE hGtPageHandle);		

/**
@brief ���������
*/
CSGTRet_E   CSGTMultiDestroy();

#ifdef __cplusplus
}
#endif
#endif

