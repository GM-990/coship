#ifndef CSSHELL_TELETEXT_H
#define CSSHELL_TELETEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "USPTeletextInterface.h"

/*@brief teletext��ʼ������*/
typedef struct _CSShellTeletextInitParam
{
	int 	m_nSize;	///<���ṹ���С������Ϊsizeof(CSShellTeletextInitParam_S)
	void*	m_pvUserData;	///<�û����ݣ�Ŀǰ��δʹ��
	
	BYTE*	m_pvMemAddr;///<teletext�ڴ�صĵ�ַ
	int		m_nMemSize;///< �ڴ�ش�С	
	
	BYTE* 	m_pvFontAddr;	///�ֿ��ַ
	int		m_nFontSize;///�ֿ��С

	CSTELERect_S   m_sTeleDisplayRect;
	BOOL bIsHD;         //true:���壬false:����
}CSShellTeletextInitParam_S;

/**
@brief ����teletext��ʼ������
@param [in] psTeletext ��ʼ�������ṹ��ʵ��
@return �ɹ�����TRUE,���򷵻�FALSE
@note 
*/
BOOL CSShellTeletextSetParam(const CSShellTeletextInitParam_S* psTeletext);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_TELETEXT_H */

