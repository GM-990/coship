
/******************************************************************************
FileName:  cs_lrcparse.h
Copyright (c) 2009�� Shen Zhen Coship Electronics CO.,LTD.

Version:       Ver 1.0
Author:        
description: LRC����ļ�����
	1��֧���̰߳�ȫ����
	2��֧�ַǷ�����ж�
modify:
Version     Date            Author          Description
--------    ----------    --------          ------------

*******************************************************************************/

#ifndef _CSLRC_PARSE_H_
#define _CSLRC_PARSE_H_
#ifdef __cplusplus
extern "C"{
#endif



/*!
	��������ֵ
*/
typedef enum _CSLrc_RetCode_E
{
	EM_CSLRC_SUCCESS,
	EM_CSLRC_FAILED,              //  һ��ʧ��
	EM_CSLRC_PARM_ERR,       //�����������
	EM_CSLRC_FILE_ERR,         //�ļ�·������
	EM_CSLRC_NO_MEM,          //�ڴ治��
	EM_CSLRC_PARSE_FAILED,   //����lrc���ʧ��
	EM_CSLRC_INVALID_HANDLE   // �Ƿ��ľ��
}CSLrc_RetCode_E;

/*!
	����Ϣ	
*/
typedef struct  _CSLrcLineInfo_S
{
	int      m_nLineNumber;      //�к�
 	char  	*m_pcBuf;                 //�е����ݣ���'\0'����
 	int      m_nBufSize;            //pcBuf����Ĵ�С
	int      m_nStartTime;        // ��ʼʱ�䣬��msΪ��λ
	int      m_nEndTime;          //����ʱ�䣬��msΪ��λ��ע�����һ����û�н���ʱ�䣬�ó�Ա��Ч
}CSLrcLineInfo_S;

/*!
      ������ʽ�����
@param[in]   pcFileName ����LRC����ļ�
@param[out]  phHandle  �����ɹ����ڴ�����ں��������ľ��
\return     �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
\note  pnHandle�����ں�������
*/
CSLrc_RetCode_E  CSLrcCreateFromFile(char *pcFileName, int *pnHandle);

/*!
      ������ʽ�����
@param[in]   pcBuf     �������LRC��Buf
@param[in]   nBufSize  pcBuf�Ĵ�С
@param[out]  phHandle  �����ɹ����ڴ�����ں��������ľ��
\return     �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
\note  pnHandle�����ں�������
*/
CSLrc_RetCode_E  CSLrcCreateFromBuf(char *pcBuf, int  nBufSize, int *pnHandle);

/*!
      ��ø�ʵ�������
@param[in]   nHandle    ��Ч�ľ������CSLrcCreateFromFile()�õ�
@param[out]  pnLineCount   �������
\return  �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
\note  ���Ը����кŵ���CSLrcGetLineInfoByIndex()���һ�е�������Ϣ
*/
CSLrc_RetCode_E  CSLrcGetLineCount(int nHandle, int *pnLineCount);

/*!
	��ȡһ�е�������Ϣ
@param[in]   nHandle  ��Ч�ľ��
@param[in]   nIndex   Ҫ��ȡ���У���0��ʼ�������������10������Ч���к���0��9
@param[out]  psLineInfo   �е�������Ϣ����¼���кš��е����ݡ���ʼʱ��ͽ���ʱ��
\return   �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
\note     �ṹ��psLineInfo�ĳ�ԱpcBuf��Ҫ���ⲿ�����ڴ棬nBufSize��ʾ�����˶���ڴ�,��С�Դ���256�ֽ�Ϊ��
          ���ڴ���䲻������ֻ���ز���  
\note     ѭ�����ñ��������Ի�ȡ�����׸�ʵ�����
\note     ���һ��û�н���ʱ��¾���û������ɴ����������ø�Ĭ��ʱ��¾
*/
CSLrc_RetCode_E CSLrcGetLineInfoByIndex(int nHandle, int nIndex, CSLrcLineInfo_S *psLineInfo);

/*!
      ���ĳʱ����Ӧ����������Ϣ
@param[in]    nHandle      ���
@param[in]    nMillSec     ����ʱ�䣬��msΪ��λ
@param[out]   psLineInfo   �е�������Ϣ����¼���кš��е����ݡ���ʼʱ�䡢����ʱ�䡢��ǰ�ٷֱ�
\return   �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
\note  ���ʱ��̫С����Ϊ�����Ҳ�����Ӧ�ĸ�ʣ��򷵻ص�һ����
       ���ʱ��̫���Ҳ�����Ӧ�ĸ�ʣ��򷵻����һ����
	  ̫С��̫��İٷ���������
\note  �ṹ��psLineInfo�ĳ�ԱpcBuf��Ҫ���ⲿ�����ڴ棬nBufSize��ʾ�����˶���ڴ�,��С�Դ���256�ֽ�Ϊ��
       ���ڴ���䲻������ֻ���ز���  
\note  ������ͨ�����ڶ�̬��ʾ��ǰ��,psLineInfo�ĳ�ԱnLineNumber��������ʾ���׸����н���
\note  ���һ��û�н���ʱ��¾���û������ɴ����������ø�Ĭ��ʱ��¾
*/

CSLrc_RetCode_E CSLrcGetLineInfoByTime(int nHandle, int nMillSec, CSLrcLineInfo_S *psLineInfo);

/*!
      �ͷž��
@param[in]  nHandle  Ҫ���ٵľ��
\return     �ɹ�����EM_CSLRC_SUCCESS������Ϊʧ��
*/
CSLrc_RetCode_E CSLrcDestory(int nHandle);

#ifdef __cplusplus
}
#endif
#endif
