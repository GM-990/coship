#ifndef _CS_SAVEBOOTPIC_H_
#define _CS_SAVEBOOTPIC_H_



#ifdef  __cplusplus
extern "C" {
#endif

#include "mmcp_typedef.h"

/**
@brief 	写BOOTER 开机画面
@Input: pcUrl 标识开机画面的路径
@return 成功返回TRUE,失败则返回FALSE
*/
int CSSaveBootScreen(const char *pcUrl);
/**
@brief 	是否显示默认开机画面还是更新的开机画面
@input:nFlag 0表示显示默认，1表示显示更新的开机画面
@return 成功返回TRUE,失败则返回FALSE
*/
BOOL CSSaveBootTag(int nFlag);

#ifdef	__cplusplus
}
#endif

#endif 
