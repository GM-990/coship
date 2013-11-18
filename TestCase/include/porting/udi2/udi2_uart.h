/**@defgroup UART UART����ͨ���첽���շ�����ؽӿ�(һ����˵�Ǵ���)

@brief UARTģ���Ӧͷ�ļ�<udi2_uart.h>��������ͨ���첽���շ������ݵĽӿڣ�
һ����˵�����Ǵ��ڵ����ݽ��ռ����ͣ������ų������ӿڵĿ����ԡ�\n

@version 2.0.0 2009/04/01 ����
@{
*/
#ifndef _UDI2_UART_H_
#define _UDI2_UART_H_

#include "udi2_typedef.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**@brief UART��ش������ֵ*/
enum
{
	CSUDIUART_ERROR_BAD_PARAMETER = CSUDI_UART_ERROR_BASE,	///< ��������
	CSUDIUART_ERROR_FEATURE_NOT_SUPPORTED,						      ///< ������֧�� 
	CSUDIUART_ERROR_TIMEOUT                                 ///< ��ʱ
};

/**@brief ��żУ������ö��ֵ */
typedef enum {
	EM_UDIURART_PARITY_NONE,		///< ��У��
	EM_UDIURART_PARITY_EVEN,		///< ż��У��
	EM_UDIURART_PARITY_ODD,			///< ����У��
	EM_UDIURART_PARITY_NUM
} CSUDIUartParity_E;


/**@brief �������Խṹ*/
typedef struct
{
	int m_nBaudrate;		///< ������,��������115200,38400 ��
	int m_nDataBits;		///< ����λ,������5,6,7,8 �������
	int m_nStopBits;		///< ֹͣλ,������1, 1.5,2 �������
	CSUDIUartParity_E m_eParity;		///< ��żУ��,���CSUDIUartParity_E����
}CSUDIUartAttribute_S;	

#define CSUDI_DEFAULT_UART_INDEX	(-1)	///< Ĭ�ϵĴ���������Ŀǰ��ʹ��Ĭ�ϴ���

/**
@brief ��ȡ��ƽ̨֧�ֵĴ��ڸ���
@param[out] pdwUartDeviceCount  ��ƽ̨֧�ֵĴ��ڸ���
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUartGetCount(unsigned int* pdwUartDeviceCount);

/**
@brief ��ȡָ�����ڵ�����
@param[in] nUartIndex ��������ֵ
@param[out] psUartAttribute  ���洮�����Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUartGetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

/**
@brief ����ָ�����ڵ�����
@param[in] nUartIndex ��������ֵ
@param[in] psUartAttribute  �������Եĵ�ַ������ṹ�ο�CSUDIUartAttribute_S���ݽṹ��
@return �ɹ���ȡ����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ
*/
CSUDI_Error_Code CSUDIUartSetAttribute(int nUartIndex, CSUDIUartAttribute_S *psUartAttribute);

/**
@brief ��uart�ӿڶ�ȡ���uLength�ֽ�����
@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ������Ŷ������ݵĻ��档
@param[in] uLength ���������ȡ�
@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
@return ����ʵ�ʶ��������ݵĳ��ȣ�0��ʾû�ж����κ����ݡ�
@note ��������ʱ���߻�����������֮�󣬸ýӿڷ��ء�
*/
int CSUDIUartRead(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);

/**
@brief ��uart�ӿڷ������uLength�ֽ�����

@param[in] nUartIndex Ҫ�����Ĵ���������
@param[in] pucBuffer ���Ҫ�������ݵĻ��档
@param[in] uLength Ҫ�������ݵĳ��ȡ�
@param[in] nTimeout ��ʱ��ʱ�䣬�����ַ�֮��ĳ�ʱʱ�䣬��λ���롣����nTimeoutδ�յ����ݱ�ʾ��ʱ��
@return ����ʵ�ʷ��͵����ݵĳ��ȣ�0��ʾû�з����κ����ݡ�
@note ��������ʱ���߻���������ȫ�����ͳ��󣬸ýӿڷ��ء�
*/
int CSUDIUartWrite(int nUartIndex ,unsigned char* pucBuffer, unsigned int uLength, int nTimeout);

/**
@brief ����Ѿ���uart�յ����д��ڻ�������û�б�CSUDIUartRead���ߵ�����

@param[in] nUartIndex Ҫ�����Ĵ���������

@return �ɹ�����CSUDI_SUCCESS��ʧ���򷵻ش������ֵ��
*/
CSUDI_Error_Code CSUDIUartClear(int nUartIndex);


#ifdef  __cplusplus
}
#endif
/**@}*/
#endif

