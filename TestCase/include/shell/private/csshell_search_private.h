
#ifndef CSSHELL_SEARCH_PRIVATE_H
#define CSSHELL_SEARCH_PRIVATE_H

#include "csshell_search.h"
#include "CSUSPDMCommonType.h"
#include "SearchUI.h"
#ifdef __cplusplus
extern "C"
{
#endif
	/**
	@brief ��ȡ���õ���������
	@param ��
	@note �ýӿڱ���ε���ʱ�����ص������һ�����õ���������
	*/
    CSShellSearchCreate_F CSShellSearchGetFactory();

	/**
	@brief �������������������ui
	@param eSearchType, ����������
	@param eMode, ������ģʽ
	@param psDeliver,����ʱ�������Ƶ��
	@param nDeliverCount,��Ƶ�����
	@param callback, search�Ļص�
	@param nTimeout, ������ʱ��������һ��Ƶ������ʱ�����ֵ
	@return �ɹ����ش�����searchUI,ʧ�ܷ���NULL
	@note callbackʵ����dtvm native�У���dtvm�е���CSShellSearchCreateUI��
	*/
	HCSHANDLE CSShellSearchCreateUI(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode, Deliver * psDeliver, int nDeliverCount, SearchCallBack callback, int nTimeout);

	/**
	@brief ������������ui
	@param eSearchType, �������������͡�������������ֱ��ʹ��searchIU.h�ж���ģ�
	@param eMode, ������ģʽ
	@param phData,����ʱ��������ݾ������Щ������������Ǿ����Ҳ������Ƶ������search�ڲ����Ը���searchType������
	@param nDataCount, ��������ݾ������
	@param callback, search�Ļص�
	@param nTimeout,������ʱ��������һ��Ƶ������ʱ�����ֵ
	@return �ɹ����ش�����searchUI,ʧ�ܷ���NULL
	@note callbackʵ����dtvm native�У���dtvm�е���CSShellSearchCreateSatelliteUI��
	*/
	HCSHANDLE CSShellSearchCreateSatelliteUI(CSShellSearchType_E eSearchType, CSShellSearchMode_E eMode, HCSHANDLE * phData, int nDataCount, SearchCallBack callback, int nTimeout);


	/**
	@brief �ر�����ui
	@param ��
	@return �ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	BOOL CSShellSearchCloseUI();


#ifdef __cplusplus
}
#endif
/** @} */
#endif   
