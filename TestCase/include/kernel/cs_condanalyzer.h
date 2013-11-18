
//cs_condanalyzer.h
#ifndef _CS_COND_ANALYZER_H
#define _CS_COND_ANALYZER_H

#ifdef  __cplusplus
extern "C" {
#endif



#include "mmcp_typedef.h"


//������
typedef enum _CSCondResult_e
{
	CS_COND_RESULT_INVALID = 0,	//�Ƿ�ֵ
	CS_COND_RESULT_TRUE,			//TRUE
	CS_COND_RESULT_FALSE			//FALSE
}CSCondResult_E;



/**
@brief ɨ��Cond�ʷ�����ȡ��Ӧ�Ĵʷ��������

@param[in] pcCond  Cond�ʷ��ַ���
@param[in] nLen  Cond�ַ�������
@return �ɹ�����Cond�ʷ���Ӧ�Ĵʷ����������ʧ�ܷ���NULL
*/
HCSHANDLE CSCondScan(const char *pcCond, int nLen);


/**
@brief ����ɨ�赽�Ĵʷ���������͸������ݼ���������

@param[in] hCond  �ʷ��������
@param[in] pvData �������㵥Ԫ��������
@return ������
*/
CSCondResult_E CSCondAnalyze(HCSHANDLE hCond, void *pvData);


/**
@brief �ͷŴʷ��������

@param[in] hCond  �ʷ��������
@return �ɹ�����TRUE��ʧ�ܷ���FALSE
*/
BOOL CSCondRelease(HCSHANDLE hCond);


/**
@brief �������㵥Ԫ����ص�����

@param[in] pcBase  �������㵥Ԫ�ַ���
@param[in] nLen  �������㵥Ԫ�ַ�������
@param[in] pvData  �������㵥Ԫ��������
@return ������
*/
typedef CSCondResult_E (*CSCondBaseOperator_Fn)(const char *pcBase, int nLen, void *pvData);


/**
@brief ע��������㵥Ԫ����ص�����

@param[in] hCond  �ʷ��������
@param[in] fnBaseOperator  �ص�����
@return �ɹ�����TRUE��ʧ���򷵻�FALSE
*/
BOOL CSCondRegistBaseOperator(HCSHANDLE hCond, CSCondBaseOperator_Fn fnBaseOperator);

#ifdef  __cplusplus
}
#endif

#endif

