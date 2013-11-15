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

@param[in] bSynchronous  CLI运行模式，TRUE为同步模式，FALSE为异步模式
@param[in] bUsePasswd  CLI运行模式，TRUE为使用密码保护模式，FALSE为不使用密码保护模式

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
@param[in] action: 命令执行函数
@param[in] help_p: 命令行帮助信息
@return  0 success, else: failure,不支持
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

@param[in] token_p: symbol 名称
@param[in] value: symbol 对应的整数值
@param[in] constant: symbol 对应的值是否为只读的，TRUE为只读，FALSE 则可以修改
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

@param[in] token_p: symbol 名称
@param[in] value: symbol 对应的双精度值
@param[in] constant: symbol 对应的值是否为只读的，TRUE为只读，FALSE 则可以修改
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

@param[in] token_p: symbol 名称
@param[in] value: symbol 对应的字符串内容
@param[in] constant: symbol 对应的值是否为只读的，TRUE为只读，FALSE 则可以修改
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

@param[in] pars_p: 命令数据
@param[in] default_p: 默认值，当pars_p->tok_len为0时result_p使用默认值
@param[out] result_p: 获取到的缓冲区，长度不超过max_len
@param[in] max_len:result_p缓冲区的最大长度
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
@brief 根据参数取得对应的命令字名称

@param[in] pars_p: 命令数据
@param[in] default_p: 默认值，
@param[out] result_p: 获取到的缓冲区，长度不超过max_len
@param[in] max_len:result_p缓冲区的最大长度
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
@brief 根据cmd查找cli中注册到的命令并执行

@param[in] pars_p: 命令数据
@param[in] cmd: 命令字
@param[in] result: 要输出的实际信息
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

@param[in] pars_p: 命令数据
@param[in] def_int: 默认值，当pars_p->tok_len为0时result_p使用默认值
@param[out] result_p: 获取到的数据
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

@param[in] pars_p: 命令数据
@param[in] def_flt: 默认值，当pars_p->tok_len为0时result_p使用默认值
@param[out] result_p: 获取到的数据
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

@param[in] pars_p: 命令数据
@param[out] result_p: 获取到的参数个数
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

@param[in] pars_p: 命令数据
@param[in] message_p: 提示信息
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


