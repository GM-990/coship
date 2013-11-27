/****************************************************************************/
/*                   Entropic (Shanghai) Co, LTD                            */
/*                        SOFTWARE FILE/MODULE HEADER                       */
/*                 Copyright Entropic Co, LTD                               */
/*                            All Rights Reserved                           */
/****************************************************************************/
/*
 * Filename:        cs_cli.c
 *
 *
 * Description:     API implementation for COSHIP interface layer .
 *
 *
 *-------------------------------------------------------------------------------
 *ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	 2013/11/06
		 The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
 *-------------------------------------------------------------------------------

 ****************************************************************************/
#include "udi2_error.h"
#include "udi2_public.h"
#include "udi2_typedef.h"
#include "udidrv_log.h"

#include "cs_cli.h"

/**
@brief init function of cli

@param[in] pstCSCLIInitParm: point to the initparam, if NULL use default serial com to read command,else use user defined  
@return  0 success, else: failure
@note cli module does not support multi-instance
*/
INT CSCLIInit(CSCLIInitParm_S* pstCSCLIInitParm)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief run  of cli

@param[in] bSynchronous  CLI����ģʽ��TRUEΪͬ��ģʽ��FALSEΪ�첽ģʽ
@param[in] bUsePasswd  CLI����ģʽ��TRUEΪʹ�����뱣��ģʽ��FALSEΪ��ʹ�����뱣��ģʽ

@return  TRUE success, FALSE: failure
@note
*/
BOOL CSCLIRun(BOOL bSynchronous,BOOL bUsePasswd)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief register command to cli

@param[in] token_p: cmmand line
@param[in] action: ����ִ�к���
@param[in] help_p: �����а�����Ϣ
@return  0 success, else: failure,��֧��
@note
*/
INT CSCLIRegisterCommand(CHAR *token_p, cli_action action,CHAR *help_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief creates or updates an integer symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ������ֵ
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignInteger(CHAR *token_p, INT value, BOOL constant)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief creates or updates an DOUBLE symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ��˫����ֵ
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignFloat(CHAR *token_p, DOUBLE value, BOOL constant)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief creates or updates an string symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ���ַ�������
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignString(CHAR *token_p, CHAR *value, BOOL constant)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief incremental reading of an input string via the parsing structure 

@param[in] pars_p: ��������
@param[in] default_p: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ���Ļ����������Ȳ�����max_len
@param[in] max_len:result_p����������󳤶�
@return  0 success, else: failure
@note
*/
INT CSCLIGetString(void * pars_data, CHAR *default_p, CHAR *result_p,INT16 max_len)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ���ݲ���ȡ�ö�Ӧ������������

@param[in] pars_p: ��������
@param[in] default_p: Ĭ��ֵ��
@param[out] result_p: ��ȡ���Ļ����������Ȳ�����max_len
@param[in] max_len:result_p����������󳤶�
@return  0 success, else: failure
@note
*/
INT CSCLIGetCmdName(void * pars_data, CHAR *default_p, CHAR *result_p,INT16 max_len)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief ����cmd����cli��ע�ᵽ�����ִ��

@param[in] pars_p: ��������
@param[in] cmd: ������
@param[in] result: Ҫ�����ʵ����Ϣ
@return FALSE no error ocurres, else: there is error ocurres
@note
*/
BOOL CSCLIExecCmd(void * pars_data,CHAR*cmd,CHAR *result_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}
/**
@brief incremental reading of an input int via the parsing structure 

@param[in] pars_p: ��������
@param[in] def_int: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ��������
@return  0 success, else: failure
@note
*/
INT CSCLIGetInteger(void * pars_data, INT def_int, INT * result_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief incremental reading of an input DOUBLE via the parsing structure 

@param[in] pars_p: ��������
@param[in] def_flt: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ��������
@return  0 success, else: failure
@note
*/
INT CSCLIGetFloat(void * pars_data, DOUBLE def_flt, DOUBLE * result_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief Return the number of parameters provided on the testtool command line

@param[in] pars_p: ��������
@param[out] result_p: ��ȡ���Ĳ�������
@return  0 success, else: failure
@note
*/
INT CSCLICGetTokenCount(void * pars_data, INT16 *result_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief displays current tokeniser string and tags the position of last token.

@param[in] pars_p: ��������
@param[in] message_p: ��ʾ��Ϣ
@return:  0 success, else: failure
@note
*/
INT CSCLITagCurrentLine(void * pars_data, CHAR *message_p)
{
	CSUDI_Error_Code Retcode = CSUDI_SUCCESS;	
	UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
	return Retcode;
}

/**
@brief put string to output port.

@param[in] format: output format and parameters, same as printf
@note disabled if DFT mode
*/
void CSCLIPrint(const CHAR *format,...)
{
	UDIDRV_LOGI("%s %s \n", __FUNCTION__, UDIDRV_NOT_REQUIRED);    
}

/**
@brief put string to output port.

@param[in] format: output format and parameters, same as printf
@note disabled if not DFT mode
*/
void CSCLIReply(const CHAR *format,...)
{
	UDIDRV_LOGI("%s %s \n", __FUNCTION__, UDIDRV_NOT_REQUIRED);    
}


