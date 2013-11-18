/******************************************************************************
FileName:  cs_saporitng.h

File Description:
        citv server porting interface
        �øò�Ĵ�������SAģ��������ԣ��Է���SA������MMCP����֮��ʹ��        
        �����MMCP��ʹ��SA���򲻱ع�ע��ģ��

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2009-02-26  lihui           Create
*******************************************************************************/
#ifndef CS_SA_PORTING_H
#define CS_SA_PORTING_H

#include "mmcp_typedef.h"
#include "mmcp_tuner.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief ����SA��ȡIPQam����Ϣ�ӿ�

��֤ģ��ͨ�������ն�ʵ�ֵĸýӿڵõ�IPQam������Ϣ
Ŀǰȡ����ָ��Section�ĵ�4��5�ֽ�(��data[3],data[4])
@param[in] pDeliverList �����Ϣ����Ƶ���б�����
@param[in] nDeliverCount pDeliverList�����е�Ƶ�����
@param[in] wPid IPQam����Ϣ����Pid
@param[in] ucTableId IPQam����Ϣ����TableId
@param[out] pIpQamDataBuffer ���ڴ��IPQam����Ϣ���ڴ�ָ��
@param[in] nBufferLength pIpQamDataBuffer�ĳ���
@param[in] pCookie ���ò��Զ�������,����SA_Interface�е�pCookie
@return �ɹ���ȡ����TRUE,ʧ�ܷ���FALSE
*/
typedef BOOL (*FOnCSSAGetIPQamDomainInfo)(const CSUDITunerSRCDeliver_U* pDeliverList,
    int nDeliverCount,
    WORD wPid,
    BYTE ucTableId,
    BYTE* pIpQamDataBuffer,
    int nBufferLength,
    void* pCookie
    );

/**
@brief ����SAֹͣ��ȡIPQam����Ϣ�ӿ�
���ڻ�ȡIPQam������Ϣ�ķѵ�ʱ����ܱȽϳ����˽ӿ������û���
��ȡIPQam������Ϣ������ֹͣ��ȡ��
@return �ɹ���ȡ����TRUE,ʧ�ܷ���FALSE
*/
typedef BOOL (*FOnCSSAStopGetIPQamDomainInfo)();
/**@brief SA������ӿڽṹ,SAģ�����������¹���ʵ��*/
typedef struct _SA_Interface
{
    void*                   pCookie;                ///�������Զ�������,����һ�º���ʱ�Բ�����ʽ����������
    FOnCSSAGetIPQamDomainInfo   fnGetIPQamDomainInfo;   ///��ȡIPQam��ӿ�,��֤ʱ��Ҫͨ���ýӿڻ�ȡIPQam����Ϣ
	FOnCSSAStopGetIPQamDomainInfo fnStopGetIPQamDomainInfo;//ֹͣ��ȡIPQam��ӿ�
}CS_SA_Interface;

/**
@brief ע��SA������ӿ�

��ʼ��ǰ���øú���ע��SAģ��������ӿ�
@param[in] pSaInterface �������ӿڽṹ
@return �ɹ�����TRUE,ʧ�ܷ���FALSE
@note �ӿڽ��Խṹ����ǳ����,�������غ�SA_Interface�ṹ��������Ҫ,��pCookieָ������ݲ���Ҳ�޷�����
*/
BOOL CSSARegInterface(CS_SA_Interface* pSaInterface);


#ifdef __cplusplus
}
#endif

#endif


