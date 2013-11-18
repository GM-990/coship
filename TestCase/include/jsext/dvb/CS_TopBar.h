/*************************************************************************
* Copyright (c) 2006,ShenZhen Coship Electronic Ltd Co.
* All rights reserved.
* 
* File Name��CS_TopBar.h
*
* Summary�� from ESP, zhangshm modify for proc-ctrl
*
**************************************************************************/
#ifndef _CS_TopBar_H_
#define _CS_TopBar_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
typedef struct _CSTopBarMsg
{
	DWORD	dwMsgType;							//��Ϣ����
	DWORD	dwMsgSubType;						//��Ϣ���ݣ�������Ϣ���Ͳ�ͬ�ֱ�Ϊ��
	int nTimeOut;								//��ʱʱ�䣨ms�����������ʱ��TopBar���Զ����������Ϊ-1����Ĭ��Ϊ����ʱ���ȴ��¼�����������ʧ��ֻ��ICON��ALARM��Ϣ��Ч
	BOOL    bMsgSubTypeCoexist;					//�Ƿ����������TopBar��ʾ��Ԫ�ع���,��Ҫ����CA�е�COMMAND
	char    acShowStr[256];                     // topbar��ʾ���ַ�������
}CSTopBarMsg;

/*	��������CSTopBarMsgRegister
 *	���ܣ�ע���û��Զ������Ϣ
 *	����˵����
 *	pstTopBarMsg���û��Զ������Ϣ��
 *	nMsgCount����Ϣ������
 */
BOOL CSTopBarMsgRegister( CSTopBarMsg* pstTopBarMsg, int nMsgCount );

/*	��������CSTopBarClear
 *	���ܣ����topbarģ���Ѵ��ڵ�topbar��Ϣ
 *	����˵����
 *	    ��
 */
void CSTopBarClear(DWORD dwMsgType, DWORD dwMsgSubType);

/*	��������CSTopBarMsgProc
 *	���ܣ�TopBar�������
 *	����˵����
 *	dwMsgType��������Ϣ����Ϣ����
 *     pTopBarPostMsg Ҫ��ʾ����Ϣ
 *	bShow����ʾ���ر�־
 */
void CSTopBarMsgProc(DWORD dwMsgType,CSTopBarMsg *pTopBarPostMsg, BOOL bShow);


/*	��������CSTopBarGetRequireDisplayCount
*	���ܣ���ȡ��ǰ��Ҫ��ʾ��Topbar ����
*/
int CSTopBarGetRequireDisplayCount(void);

/*	��������CSTopBarGetRequireDisplayItemInfo
*	���ܣ���ȡ��ǰ��Ҫ��ʾ��ĳһ��Topbar ������
*     ����˵��: 
			nIndex-����
			nProperty -���ԣ�0-MsgID ; 1-MsgContent; 2-Timeout; 3-Msg Rect
			pParam - ����ֵ������nProperty�Ķ��巵�ز�ͬ�ı����������ͣ��ַ������ṹ���
			nDataLen - ����ֵ��Buffer����
*/
int CSTopBarGetRequireDisplayItemInfo(int nIndex,int nProperty, void *pParam, int nDataLen);

/*	��������CSTopBarSetRegisterItemParam
*	���ܣ�����ĳһ����ע���Topbar ������
*     ����˵��: 
			nMsgID-��ע�����ϢID
			nProperty -���ԣ�0-MsgID ; 1-MsgContent; 2-Timeout; 3-Msg Rect
			pParam - ���õľ���ֵ�������ͣ��ַ������ṹ���
			nDataLen - ����ֵ��Buffer����
			
*/
int CSTopBarSetRegisterItemParam(int dwMsgType,int nProperty, void *pParam, int nDataLen);

int CSGetTopBarInfoIndex(int dwMsgType);

/*	��������CSJSMsgProc
 *	���ܣ���JS������Ϣ
 *	���������
 *	    dwMessageType�����͵���Ϣ�����ͣ�ȡֵ��Χ�μ�JSMessage.hͷ�ļ�����
 *	    caJsonStr������Ϣ������Ӧ����Ϣ�ĸ�����Ϣ����json�ַ�������ʽ��ʾ���������Ҫ�˲�������0;
 *	               Ҫע�⣬���ַ����ĳ�������ܳ���1000���ֽڣ��������Ļᱻ�ض�;
 *                 ��: ���caJsonStr��Ϊ��,���ӿ��ڲ��´��caJsonStr�����ݣ��ʴ˽ӿڷ��غ�
 *                   caJsonStrָ��ĵ�ַ�ռ䲻�ٱ�������ʹ��;
 *	    nLength��caJsonStr�ַ����ĳ���
 *  ���������
 *      ��
 *  ����ֵ��
 *      0��ʾ�ɹ�������ֵ��ʾʧ��
 *  ע��㣺
 *      caJsonStr�ַ����б�����type���ԣ���ֵΪdwMessageType
 */
int CSJSMsgProc(DWORD dwMessageType, const char* caJsonStr, int nLength);

/*	��������CSJSMsgProcEx
 *	���ܣ����page������ָ��ҳ��JS������Ϣ
 *	���������
 *		hPage:������ӿڴ����Ķ�page���
 *	    dwMessageType�����͵���Ϣ�����ͣ�ȡֵ��Χ�μ�JSMessage.hͷ�ļ�����
 *      pcJsonStr������Ϣ������Ӧ����Ϣ�ĸ�����Ϣ����json�ַ�������ʽ��ʾ���������Ҫ�˲�������NULL;
 *	               Ҫע�⣬���ַ����ĳ�������ܳ���1000���ֽڣ��������Ļᱻ�ض�;
 *                 ��: ���caJsonStr��Ϊ��,���ӿ��ڲ��´��caJsonStr�����ݣ��ʴ˽ӿڷ��غ�
 *                   caJsonStrָ��ĵ�ַ�ռ䲻�ٱ�������ʹ��; *	    
 *      nLength��caJsonStr�ַ����ĳ���
 *  ���������
 *      ��
 *  ����ֵ��
 *      0��ʾ�ɹ�������ֵ��ʾʧ��
 *  ע��㣺
 *      caJsonStr�ַ����б�����type���ԣ���ֵΪdwMessageType
 */
int CSJSMsgProcEx(HCSHANDLE hPage, DWORD dwMessageType, const char* pcJsonStr, int nLength);

#ifdef __cplusplus
}
#endif

#endif
