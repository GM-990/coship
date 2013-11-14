/******************************************************************************
FileName:  CS_CLI.H
Copyright (c) 2009�� Shen Zhen Coship Electronics CO.,LTD.��

Version:       Ver 1.0
Author:        shenshaohui (shenshaohui@coship.com)

File Description:
        File Description, Please add you description here.

Modify History:
Version     Date        Author          Description
--------    ----------  --------        ------------
1.0         2009-04-20  shenshaohui     Create
*******************************************************************************/
/******************************************************************************
-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------
*******************************************************************************/

#ifndef _CS_CLI_H_
#define _CS_CLI_H_

/******************************************************************************
 *                                 Header File Include                                 *
 ******************************************************************************/
#include "udiplus_debug.h"
#include "udiplus_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *                                 Macro/Define/Structus                           *
 ******************************************************************************/



/**
@brief opens the file whose name is the string pointed to by path and associates a stream with it.


@param[in] path point to the read data
@param[in] mode The argument mode points to a string beginning with one of the following sequences
@return  successful completion fopen() return a FILE pointer.  Otherwise, NULL is returned
@note same with FILE *fopen(const CHAR *path, const CHAR *mode);
*/

typedef void * (*cli_fopen)(const CHAR *path, const CHAR *mode);

/**
@brief function will flush the stream pointed to by fp


@param[in] fp point to the opened stream
@return  successful completion fopen() return a FILE pointer.  Otherwise, NULL is returned
@note INT cli_fclose(void *fp) same with int fclose(FILE *fp);
*/
typedef INT (*cli_fclose)(void *fp);

/**
@brief writes nmemb elements of data, each size bytes long, to the stream pointed by  stream, obtaining them from the location given by ptr.


@param[in] ptr point to the write data
@param[in] size the write data unit lenth
@param[in] nmemb writes nmemb elements of data
@param[in] FileContext the stream handle,eg:fp
@return  return the number of items successfully written
@note UINT cli_fwrite(void *ptr,UINT size,UINT nmemb,void *FileContext)
*/
typedef UINT (*cli_fwrite)(void *ptr,UINT size,UINT nmemb,void *FileContext);

/**
@brief read nmemb elements of data, each size bytes long, to the stream pointed by	stream, obtaining them from the location given by ptr.


@param[in] ptr point to the read data
@param[in] size the read data unit lenth
@param[in] nmemb read nmemb elements of data
@param[in] FileContext the stream handle,eg:fp
@return  return the number of items successfully read
@note DWORD cli_fread(void *ptr,UINT size,UINT nmemb,void *FileContext) 
*/
typedef UINT (*cli_fread)(void *ptr,UINT size,UINT nmemb,void *FileContext);

/**
@brief local function to define a low level write to uart or other device

@param[in] pubBuffer point to the data buffer to be write
@param[in] dwToWrite the data lenth to be write
@return  the number of data writed
@note
*/
typedef UINT (*cli_write)(BYTE* pubBuffer, UINT dwToWrite);

/**
@brief local function to define a low level read from uart or other device

@param[in] pubBuffer point to the data buffer to be store read data
@param[in] dwToRead the data lenth want to be read from device
@return  the number of data read
@note this func should not return when have not read data
*/
typedef UINT (*cli_read)(BYTE* pubBuffer, UINT dwToRead);

/**
@brief opens the file whose name is the string pointed to by path and associates a stream with it.


@param[in] pars_p point to the parameter data
@param[out] result_p pointer to result string to output,fixed 512 bytes memory
@return  TRUE  when error ocurres, or FALSE.
@note CLI will output result_p just after then end of the action over
*/
typedef BOOL (*cli_action) (void * pars_p, char *result_p);


/**@brief _CSCLIInitParm����*/
typedef struct _CSCLIInitParm
{
	cli_write m_fnWrite;	/**< CLIģ���ַ���������ӿڣ�����ʵ��*/
	cli_read m_fnRead;		/**< CLIģ�������ȡ����ӿڣ�����ʵ�֣�����Ϊ����ģʽ*/
	cli_fopen m_fnFopen;	/**< CLIģ���ļ�������ӿڣ���ѡ����֧�ֽű��ļ�ʱ����ʵ�ִ˽ӿڣ�����NULL*/
	cli_fclose m_fnFclose;	/**< CLIģ���ļ��ر�����ӿڣ���ѡ����֧�ֽű��ļ�ʱ����ʵ�ִ˽ӿڣ�����NULL*/
	cli_fwrite m_fnFwrite;	/**< CLIģ���ļ�д����ӿڣ���ѡ����֧�ֽű��ļ�ʱ����ʵ�ִ˽ӿڣ�����NULL*/
	cli_fread m_fnFread;	/**< CLIģ���ļ�������ӿڣ���ѡ����֧�ֽű��ļ�ʱ����ʵ�ִ˽ӿڣ�����NULL*/
}CSCLIInitParm_S;

/******************************************************************************
 *                                 Global Function Declare                                   *
 ******************************************************************************/
/**
@brief init function of cli

@param[in] pstCSCLIInitParm: point to the initparam, if NULL use default serial com to read command,else use user defined  
@return  0 success, else: failure
@note cli module does not support multi-instance
*/
INT CSCLIInit(CSCLIInitParm_S* pstCSCLIInitParm);

/**
@brief run  of cli

@param[in] bSynchronous  CLI����ģʽ��TRUEΪͬ��ģʽ��FALSEΪ�첽ģʽ
@param[in] bUsePasswd  CLI����ģʽ��TRUEΪʹ�����뱣��ģʽ��FALSEΪ��ʹ�����뱣��ģʽ

@return  TRUE success, FALSE: failure
@note
*/
BOOL CSCLIRun(BOOL bSynchronous,BOOL bUsePasswd);

/**
@brief register command to cli

@param[in] token_p: cmmand line
@param[in] action: ����ִ�к���
@param[in] help_p: �����а�����Ϣ
@return  0 success, else: failure,��֧��
@note
*/
INT CSCLIRegisterCommand(CHAR *token_p, cli_action action,CHAR *help_p);

/**
@brief creates or updates an integer symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ������ֵ
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignInteger(CHAR *token_p, INT value, BOOL constant);

/**
@brief creates or updates an DOUBLE symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ��˫����ֵ
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignFloat(CHAR *token_p, DOUBLE value, BOOL constant);

/**
@brief creates or updates an string symbol table entry

@param[in] token_p: symbol ����
@param[in] value: symbol ��Ӧ���ַ�������
@param[in] constant: symbol ��Ӧ��ֵ�Ƿ�Ϊֻ���ģ�TRUEΪֻ����FALSE ������޸�
@return  0 success, else: failure
@note
*/
INT CSCLIAssignString(CHAR *token_p, CHAR *value, BOOL constant);

/**
@brief incremental reading of an input string via the parsing structure 

@param[in] pars_p: ��������
@param[in] default_p: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ���Ļ����������Ȳ�����max_len
@param[in] max_len:result_p����������󳤶�
@return  0 success, else: failure
@note
*/
INT CSCLIGetString(void * pars_data, CHAR *default_p, CHAR *result_p,INT16 max_len);

/**
@brief ���ݲ���ȡ�ö�Ӧ������������

@param[in] pars_p: ��������
@param[in] default_p: Ĭ��ֵ��
@param[out] result_p: ��ȡ���Ļ����������Ȳ�����max_len
@param[in] max_len:result_p����������󳤶�
@return  0 success, else: failure
@note
*/
INT CSCLIGetCmdName(void * pars_data, CHAR *default_p, CHAR *result_p,INT16 max_len);

/**
@brief ����cmd����cli��ע�ᵽ�����ִ��

@param[in] pars_p: ��������
@param[in] cmd: ������
@param[in] result: Ҫ�����ʵ����Ϣ
@return FALSE no error ocurres, else: there is error ocurres
@note
*/
BOOL CSCLIExecCmd(void * pars_data,CHAR*cmd,CHAR *result_p);
/**
@brief incremental reading of an input int via the parsing structure 

@param[in] pars_p: ��������
@param[in] def_int: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ��������
@return  0 success, else: failure
@note
*/
INT CSCLIGetInteger(void * pars_data, INT def_int, INT * result_p);

/**
@brief incremental reading of an input DOUBLE via the parsing structure 

@param[in] pars_p: ��������
@param[in] def_flt: Ĭ��ֵ����pars_p->tok_lenΪ0ʱresult_pʹ��Ĭ��ֵ
@param[out] result_p: ��ȡ��������
@return  0 success, else: failure
@note
*/
INT CSCLIGetFloat(void * pars_data, DOUBLE def_flt, DOUBLE * result_p);

/**
@brief Return the number of parameters provided on the testtool command line

@param[in] pars_p: ��������
@param[out] result_p: ��ȡ���Ĳ�������
@return  0 success, else: failure
@note
*/
INT CSCLICGetTokenCount(void * pars_data, INT16 *result_p);

/**
@brief displays current tokeniser string and tags the position of last token.

@param[in] pars_p: ��������
@param[in] message_p: ��ʾ��Ϣ
@return:  0 success, else: failure
@note
*/
INT CSCLITagCurrentLine(void * pars_data, CHAR *message_p);

/**
@brief put string to output port.

@param[in] format: output format and parameters, same as printf
@note disabled if DFT mode
*/
void CSCLIPrint(const CHAR *format,...);

/**
@brief put string to output port.

@param[in] format: output format and parameters, same as printf
@note disabled if not DFT mode
*/
void CSCLIReply(const CHAR *format,...);


#ifdef __cplusplus
	}
#endif

#endif  /* CS_CLI_H */

/* End of File */


