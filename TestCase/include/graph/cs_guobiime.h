/**
�ļ����ƣ�CSGUOBIIME.H
��Ȩ����(c)  2010�� ͬ�޵��ӡ�
 
@version  Ver 1.0    
@author   ��ʽ�� (longshirong@coship.com)
@file
		�ļ�����˵��
 
History:
�汾��		����		����		�޸�����
---------	     --------        -------   	-------------
1.0			2010-10-26	��ʽ��		����
*/

#ifndef CSGUOBIIME_H
#define CSGUOBIIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_imm.h"

/**
@brief  ��������:
		���������뷨ע�ᵽϵͳ
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		����ɹ�ע�ᣬ�������뷨�ı�ţ����򷵻�0
@exception
@note
*/
DWORD CSRegisterGuobiIme(void);

/**
@brief  ��������:
		�Բ����ʽע�����뷨�ӿ�
 
@param[in]
		void *pUserdata �����������δ��
 
@param[out] 
		��
 
@return
 		����ɹ�ע�ᣬ�������뷨�ı�ţ����򷵻�0
@exception
@note
*/
DWORD CSPluginIme_GuobiInit(void *pUserdata);

/**
@brief  ��������:
		��ȡ�������뷨ע��ģ������
 
@param[in]
		��
 
@param[out] 
		��
 
@return
 		���뷨ģ������
@exception
@note
*/
char * CSPluginIme_GuobiGetName(void);

/**
@brief  ��������:
		���ù������뷨����ʱ���·��
 
@param[in]
		const char * pcFilePath �����·��,��ֵֻ�����õ�Ŀ¼���ɣ�����:"workdir/mnt/nfs/ime"
 
@param[out] 
		��
 
@return
 		�ɹ����ã�����E_IMM_RETURN_OK�����򷵻�E_IMM_RETURN_FALSE
@exception
@note
*/
int CSSetGuobiImeUDBPath(const char * pcFilePath);
#ifdef __cplusplus
}
#endif

#endif  /* CSGUOBIIME_H */

/* �ļ����� */

