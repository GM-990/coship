/******************************************************************************
FileName:  properities_parse.h
Copyright (c) 2009�� Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:       
description: 
	ע�⣺
	1�������ж���ӿڣ����Ƽ�ʹ�þ��������Load�ķ���������������һ�����Load��������ļ���
	2���洢�ļ�ʱ������ʹ��CSPROStore()���Ƽ�ʹ��CSPROStoreToFile()

modify:
Version     Date            Author          Description
--------    ----------    --------          ------------
1.0          2009.08.26                     create
1.1          2010.03.05                     ���Ӵ浽ָ���ļ���ɾ�����Ĺ���
*******************************************************************************/

#ifndef _PROPERTIES_PARSE_H_
#define _PROPERTIES_PARSE_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "mmcp_typedef.h"

/**
 * @brief  ����properities�����ļ�������
 * @param[in]  pcFileName  ����Ҫ�򿪵������ļ�
 * @param[out]   phHandle   �����ɹ��Ľ�������������ں�������
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ�ܣ�ʧ��ʱphHandle��ֵ����Ч��
 * @note  ����ļ������ڻᴴ���ļ�
 * @note  ��Ҫ����CSPRODestory�ͷ�
 */
BOOL CSPROCreate(char *pcFileName, HCSHANDLE *phHandle);
/**
 * @brief       ��buf����properties�����ļ�������
 * @param[in]   pcBuf   Ҫ������buf
 * @param[in]   dwSize  buf�ĳ���
 * @param[out]   phHandle   �����ɹ��Ľ�������������ں�������
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ�ܣ�ʧ��ʱphHandle��ֵ����Ч��
 * @note  ���ñ����������Ľ��������ܵ���CSPROStore���浽�ļ�
 * @note  ��Ҫ����CSPRODestory�ͷ�
 */
BOOL CSPROCreateFromBuf(char* pcBuf, DWORD dwSize, HCSHANDLE *phHandle);
/**
 * @brief  �������
 * @param[out]  phHandle  �����ľ��
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ�ܣ�ʧ��ʱphHandle��ֵ����Ч��
 * @note  ����ֻ���������֮����װ������
 * @note  �Ƽ�ʹ�ø÷�����CSPROLoadFromBuf
 */
 BOOL   CSPROCreateHandle(HCSHANDLE *phHandle);
/**
 * @brief       װ�����ݵ�������
 * @param[in]   hHandle   ���������
 * @param[in]   pcBuf   Ҫ������buf
 * @param[in]   dwSize  buf�ĳ���
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ��
 * @note  hHandle���������ⷽ�������ľ��
 * @note  ���Զ�ε��ñ���������������ʵ�ֽ�������úϲ��Ĺ���
 */
BOOL CSPROLoadFromBuf(HCSHANDLE  hHandle, char* pcBuf, DWORD dwSize);

/**
 * @brief       ���ļ�װ�����ݵ�������
 * @param[in]   hHandle   ���������
 * @param[in]   pcFileName  Ҫװ�ص��ļ�
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ��
 * @note  hHandle���������ⷽ�������ľ��
 * @note  ���Զ�ε��ñ���������������ʵ�ֽ�������úϲ��Ĺ���
 */
BOOL CSPROLoadFromFile(HCSHANDLE  hHandle, char* pcFileName);


/**
 * @brief  ����
 * @param[in]  hHandle  ���
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ��
 * @note   ����CSPROCreate()ʱ�����ľ��������֮������ʹ��
*/
BOOL CSPRODestory(HCSHANDLE hHandle);

/**
 * @brief  ��ȡ����ֵ
 * @param[in]  hHandle  ���
 * @param[in]  pcKey  Ҫ��ȡ��������
 * @param[out]   pcValue   ����ֵ���ڴ����ⲿ����
 * @param[in/out]   pdwLen   ����ֵ�ĳ��ȣ���Ϊ�������ʱ��ʾpcValue�ĳ��ȣ���Ϊ�������ʱ��ʾʵ��д��pcValue���ֽ���
 * @return  TRUE��ʾ�ɹ���FALSE��ʾδ�ҵ���δ�ҵ�ʱpcValue��pdwLen����Ч
 * @note  ���ر�ע��pwdLen�ǿɶ���д�����������ÿ�ε���CSPROGetProperty֮ǰ��Ҫ���¸�ֵ
 */
BOOL CSPROGetProperty(HCSHANDLE hHandle, char *pcKey, char *pcValue, DWORD *pdwLen);

/**
 * @brief  ��ȡ����ֵ�ĳ���
 * @param[in]  hHandle  ���
 * @param[in]  pcKey  Ҫ��ȡ��������
 * @return  
 	���keyû�ж�Ӧ�Ļ�������������0��
	�����ļ������ڡ�key�����ڣ�����-1��
 * @note  
 */
int CSPROGetValueLen(HCSHANDLE hHandle, char *pcKey);

/**
 * @brief  ��������ֵ
 * @param[in]  hHandle  ���
 * @param[in]  pcKey  Ҫ���õ�������
 * @param[out]   pcValue   Ҫ���õ�����ֵ������'\0'��β���ַ���
 * @return  TRUE��ʾ���óɹ���FALSE��ʾ����ʧ��
 * @note    pcValue���Դ��գ���ʾ���ö�Ӧ������Ϊ��
 * @note    ��������ֻ���޸����ڴ��е�ֵ�������޸��ļ���Ҫ���޸ı��浽�ļ������CSPROStore()��CSPROStoreToFile()
 * @note    �����㲻���ڣ��ᴴ���ý��
 */
BOOL CSPROSetProperty(HCSHANDLE hHandle, char* pcKey, char *pcValue);

/**
 * @brief  ɾ��һ�����
 * @param[in]  hHandle  ���
 * @param[in]  pcKey  Ҫɾ���ļ���
 * @return  TRUE��ʾ���óɹ���FALSE��ʾ����ʧ��
 * @note    ɾ�����ֻ��ɾ�����ڴ��еĽ�㣬����ɾ���ļ���Ҫ��ɾ��ͬ�����ļ������CSPROStore()��CSPROStoreToFile()
 * @note    ����㲻����ʱ���سɹ�
 */
BOOL CSPRODelItem(HCSHANDLE hHandle, char* pcKey);

/**
 * @brief  ��ȡ���Ե���Ŀ
 * @param[in]  hHandle  ���
 * @param[in]   pnCount  ���Եĸ���
 * @return  TRUE��ʾ��ȡ�ɹ���FALSE��ʾ��ȡʧ�� 
 */
BOOL  CSPROGetCount(HCSHANDLE hHandle, int  *pnCount);

/**
 * @brief  ���������Ż�ȡ������������ֵ
 * @param[in]  hHandle  ���
 * @param[in]  nIndex   �����ţ�ȡֵ��ΧΪ0��Key�������Ŀ��1��������ܹ���10��key����Ϸ�������Ϊ0��9
 * @param[out] pckey    ����������'\0'�������ڴ���Ҫ�ⲿȥ���䣬����Ĵ�СӦ����ʵ�����������ȼ�1��
        ������䲻���޷������������ȡʧ�ܣ������֪���������ж೤��Ӧ�����ܷ����һЩ
 * @param[int] dwKeyLen ��ʾ�ڴ����Ĵ�С������Ϊ'\0'������ڴ�
 * @param[out] pcValue   ����ֵ����'\0'�������ڴ���Ҫ�ⲿȥ���䣬����Ĵ�СӦ����ʵ������ֵ���ȼ�1��
        ������䲻���޷�������ֵ���ȡʧ�ܣ������֪������ֵ�ж೤��Ӧ�����ܷ����һЩ
 * @param[int] dwValueLen ��ʾ�ڴ����Ĵ�С������Ϊ'\0'������ڴ�
 * @return  TRUE��ʾ��ȡ�ɹ���FALSE��ʾ��ȡʧ�� 
 * @note  ��������CSPROGetCount()���ʹ�ã����Ա��������е�����
 * @note  ע���ڱ����Ĺ���Ӧ���������ģ�������ɾ�����������CSPROSetProperty����һ�������ڵ����ԣ��������ǲ���Ԥ�ڵ�
          ��Ϊ��ɾ����Ӱ�쵽������Ŀ������
*/
BOOL CSPROGetItem(HCSHANDLE hHandle, int  nIndex, char *pckey, DWORD dwKeyLen, char *pcValue, DWORD dwValueLen);

/**
 * @brief  �洢���ļ�
 * @param[in]  hHandle  ���
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ��
 * @note   ������ļ�ΪCSPROCreate()ʱ������ļ�
 * @note   �����ļ���������Ч����CSPROCreate��Ч����CSPROParseFromBuf��Ч
 * @note   ���ӿڵĹ��ܽ��𲽱�CSPROStoreToFileȡ�������Ƽ�ʹ�ñ�����
 */
BOOL CSPROStore(HCSHANDLE hHandle);

/**
 * @brief  �洢��ָ���ļ�
 * @param[in]  hHandle  ���
 * @param[in]  pcFileName  ָ���ļ�·��
 * @return  TRUE��ʾ�ɹ���FALSE��ʾʧ��
 * @note   ���ñ��ӿڿ���ʵ�ֶ�������ļ��ϲ���浽ͬһ���ļ�
 * @note   ������������ļ��Ѿ��������ɾ��ԭ�ļ�������û���Ҫȷ��ԭ�ļ��ǿ��Ա����ǵ�
 */
BOOL CSPROStoreToFile(HCSHANDLE hHandle, char * pcFileName);


#ifdef __cplusplus
}
#endif
#endif

