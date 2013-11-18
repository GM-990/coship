#ifndef CSSHELL_TELETEXT_H
#define CSSHELL_TELETEXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"
#include "USPTeletextInterface.h"

/*@brief teletext初始化参数*/
typedef struct _CSShellTeletextInitParam
{
	int 	m_nSize;	///<本结构体大小，必须为sizeof(CSShellTeletextInitParam_S)
	void*	m_pvUserData;	///<用户数据，目前暂未使用
	
	BYTE*	m_pvMemAddr;///<teletext内存池的地址
	int		m_nMemSize;///< 内存池大小	
	
	BYTE* 	m_pvFontAddr;	///字库地址
	int		m_nFontSize;///字库大小

	CSTELERect_S   m_sTeleDisplayRect;
	BOOL bIsHD;         //true:高清，false:标清
}CSShellTeletextInitParam_S;

/**
@brief 设置teletext初始化参数
@param [in] psTeletext 初始化参数结构体实例
@return 成功返回TRUE,否则返回FALSE
@note 
*/
BOOL CSShellTeletextSetParam(const CSShellTeletextInitParam_S* psTeletext);


#ifdef __cplusplus
}
#endif

#endif  /* SHELL_TELETEXT_H */

