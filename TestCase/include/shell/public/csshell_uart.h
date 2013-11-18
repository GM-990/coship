/**@defgroup ͨ���첽���շ�����ؽӿ�(һ����˵�Ǵ���)��csshell_uart��ģ���ƣ�CSSHELLUART
	
ģ����UART
@{
*/
#ifndef _CSSHELL_UART_H_
#define _CSSHELL_UART_H_

#include "udi2_uart.h"
#include "mmcp_typedef.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct 
{
	int m_nSize;	///<���ṹ��С��һ������sizeof(CSShellUARTInitParam_S)

	/**
	@brief ��ȡ��ƽ̨֧�ֵĴ��ڸ���
	@param[out] pdwUartDeviceCount  ��ƽ̨֧�ֵĴ��ڸ���
	@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
	*/
	CSUDI_Error_Code (* m_fnGetCount)(unsigned int* pdwUartDeviceCount);
	
	/**
	@brief ��ȡָ�����ڵ�����
	@param[in] nUartIndex ��������ֵ
	@param[out] psUartAttribute  ���洮�����Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
	@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
	*/
	CSUDI_Error_Code (* m_fnGetAttribute)(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);
	
	/**
	@brief ����ָ�����ڵ�����
	@param[in] nUartIndex ��������ֵ
	@param[in] psUartAttribute  �������Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
	@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
	*/
	CSUDI_Error_Code (* m_fnSetAttribute)(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

	/**
	@brief ��uart�ӿڶ�ȡ���uLength�ֽ�����
	@param[in] nUartIndex Ҫ�����Ĵ���������
	@param[in] pucBuffer ������Ŷ������ݵĻ��档
	@param[in] uLength ���������ȡ�
	@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
	@return ����ʵ�ʶ��������ݵĳ��ȣ�0��ʾû�ж����κ����ݡ�
	@note ��������ʱ���߻�����������֮�󣬸ýӿڷ��ء�
	*/
	int (* m_fnRead)(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);
	
	/**
	@brief ��uart�ӿڷ������uLength�ֽ�����
	@param[in] nUartIndex Ҫ�����Ĵ���������
	@param[in] pucBuffer ���Ҫ�������ݵĻ��档
	@param[in] uLength Ҫ�������ݵĳ��ȡ�
	@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
	@return ����ʵ�ʷ��͵����ݵĳ��ȣ�0��ʾû�з����κ����ݡ�
	@note ��������ʱ���߻���������ȫ�����ͳ��󣬸ýӿڷ��ء�
	*/
	int (* m_fnWrite)(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);
	
	/**
	@brief ����Ѿ���uart�յ����д��ڻ�������û�б�CSUDIUartRead���ߵ�����
	@param[in] nUartIndex Ҫ�����Ĵ���������
	@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
	*/
	CSUDI_Error_Code (* m_fnClear)(int nUartIndex);

}CSShellUARTInitParam_S;

/**
@brief ���� UART �ӿڲ���
@param psParam  UART  Shell��Ҫʵ�ֵĽӿ�
@�ɹ�����TRUE,ʧ�ܷ���FALSE
*/
BOOL CSShellUARTSetParam(CSShellUARTInitParam_S * psParam);

/**
@brief ��ȡ UART �ӿڲ���
@param ��
@�ɹ�����CSShellUARTInitParam_Sָ��,ʧ�ܷ���NULL
*/
const CSShellUARTInitParam_S* CSShellUARTGetParam();

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif



