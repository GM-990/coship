#ifndef _CS_SAVEBOOTPIC_H_
#define _CS_SAVEBOOTPIC_H_



#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
@brief 	дBOOTER ��������
@Input: pcUrl ��ʶ���������·��
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
int CSSaveBootScreen(const char *pcUrl);
/**
@brief 	�Ƿ���ʾĬ�Ͽ������滹�Ǹ��µĿ�������
@input:nFlag 0��ʾ��ʾĬ�ϣ�1��ʾ��ʾ���µĿ�������
@return �ɹ�����TRUE,ʧ���򷵻�FALSE
*/
BOOL CSSaveBootTag(int nFlag);

#ifdef	__cplusplus
}
#endif

#endif 
