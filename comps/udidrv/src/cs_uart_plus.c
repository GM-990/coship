/****************************************************************************/
/*
 * Filename:        cs_uart_plus.c
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

#include "cs_uart_plus.h"


/**
@brief �Ե�nUartIndex·���ڽ��г�ʼ������ʹ�ô���ǰ����Ը�·���ڽ��г�ʼ��������û�Ҫ����common clock��ֵ������ڳ�ʼ������ǰ���ú���CSHDIUartSetCustomParam��

@param[in] nUartIndex  Ҫ��ʼ�����ڵ����������ڲ�ͬ��ƽ̨�����ڵ�������һ�����������ڹ��ܵ�ASCҲ��ͬ�����Ӿ���Ӳ����ƶ��������µ�Ӳ��ƽ̨�Ͻ��п���ʱ������Ӳ������ʱʦ��������������ʦѯ�ʾ����ȡֵ��
@param[in] dwBaud ���ڵĴ������ʡ�һ��Ϊ115200bps
@return
- CSHDI_ERROR_BAD_PARAMETER������������Ϸ���
- CSHDI_ERROR_ALREADY_INITIALIZED����·������֮ǰ�Ѿ�����ʼ�����ˡ�
- CSHDI_FAILURE�����ڳ�ʼ��ʧ�ܡ�
- CSHDI_SUCCESS�����ڳ�ʼ���ɹ���
@note ���ڳ�ʼ������gpio��ʼ��֮��
*/
CSHDI_Error_t CSHDIUartInitial  ( int  nUartIndex, UINT dwBaud)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ����Ĭ�ϵĴ��ڡ�

@param[in] nUartIndex Ҫ���ó�Ĭ�ϴ��ڵĴ���������
@return ����ɹ��򷵻��û��趨�Ĵ���������������ɹ����򷵻��û���ǰ�趨��Ĭ�ϴ��ڵ�����������û���ǰû�����ù�Ĭ�ϵĴ��ڣ��򷵻�������ʼ��ʱĬ�ϵĴ�������(2)��
@note ����Ĭ�ϴ������ڶ�д����ǰ��
*/
int CSHDIUartSetDefault ( int  nUartIndex)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief �Ӵ���nUartIndex��ȡu32Length�����ݣ���ȡ�����ݴ�ŵ� pucBuffer����ʱʱ��Ϊu32TimeOut��

@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ���ڴ�Ŷ�ȡ�����ݵĻ��档
@param[in] u32Length Ҫ��ȡ���ݵĳ��ȡ�
@param[in] u32TimeOut ��ʱʱ�䣬�����ַ�֮��ĳ�ʱʱ�䡣
@return ����ʵ�ʶ��������ݵĳ��ȡ�
*/
ULONG  CSHDIUartReadFromWithTimeout ( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief �򴮿�nUartIndex����u32Length�����ݣ�Ҫ���͵����ݴ���� pucBuffer�У���ʱʱ��Ϊu32TimeOut��

@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ���Ҫ�������ݵĻ��档
@param[in] u32Length Ҫ�������ݵĳ��ȡ�
@param[in] u32TimeOut ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䡣
@return ����ʵ�ʷ��͵����ݵĳ��ȡ�
*/
ULONG  CSHDIUartWriteToWithTimeout( int  nUartIndex ,PBYTE pucBuffer, ULONG u32Length, ULONG u32TimeOut)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief ����ָ�����ڵĲ����������T_UartParamType����

@param[in] nUartIndex ��������
@param[in] enmType ��������
@param[in] pVaule �������ݵ��׵�ַ
@return 
	CSHDI_SUCCESS:���óɹ�
	CSHDI_FAILURE:����ʧ��
	CSHDI_ERROR_BAD_PARAMETER: ��������
*/
CSHDI_Error_t CSHDIUartSetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��ȡָ�����ڵĲ���

@param[in] nUartIndex ��������
@param[in] enmType ��������
@param[out] pVaule  ��Ų���ֵ���׵�ַ
@return  
	CSHDI_SUCCESS:���óɹ�
	CSHDI_FAILURE:����ʧ��
	CSHDI_ERROR_BAD_PARAMETER: ��������
*/
CSHDI_Error_t CSHDIUartGetParam(int  nUartIndex, T_UartParamType enmType, void *pVaule)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}


/**
@brief �ر�ָ������,�رպ����ͨ�����ó�ʼ���ӿڷ��ɻָ����ڹ�����

@param[in] nUartIndex ��������
@return
	CSHDI_SUCCESS: �رճɹ�
	CSHDI_FAILURE:  �ر�ʧ��
	CSHDI_ERROR_BAD_PARAMETER: ��������
*/
CSHDI_Error_t CSHDIUartTerm(int nUartIndex)
{
    CSHDIComErrType Retcode = CSHDI_SUCCESS;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}

/**
@brief ��ȡUARTģ��汾��Ϣ����ʾ��ʽΪ:
  [ģ����]-[ģ������]-[�汾��],��UART_OS_T_01.00.00, UART��ʾģ����,OS��ʾ+os������NOOS��ʾ-os������T��ʾtest�汾,01.00.00Ϊ�汾��

@param[out] pchVer ��Ű汾��Ϣ�Ļ������׵�ַ
@param[in] nSize  ����������
@return 
   1> �����һ������pchVerΪ�գ����ذ汾��Ϣ��ʵ�ʳ���
   2> �����һ��������Ϊ�գ��Һ������óɹ����򷵻�ʵ
        �ʻ�ȡ�İ汾��Ϣ����
   3> ���ʧ�ܣ�����0
*/

int CSUARTGetVersion( PCHAR pucVer, int nSize )
{
    int Retcode = 0;	
    UDIDRV_LOGI("%s %s (Retcode =%d)\n", __FUNCTION__, UDIDRV_NOT_REQUIRED, Retcode);    
    return Retcode;
}



