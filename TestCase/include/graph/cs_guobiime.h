/**
文件名称：CSGUOBIIME.H
版权所有(c)  2010， 同洲电子。
 
@version  Ver 1.0    
@author   龙式荣 (longshirong@coship.com)
@file
		文件功能说明
 
History:
版本号		日期		作者		修改内容
---------	     --------        -------   	-------------
1.0			2010-10-26	龙式荣		创建
*/

#ifndef CSGUOBIIME_H
#define CSGUOBIIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cs_imm.h"

/**
@brief  功能描述:
		将国笔输入法注册到系统
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		如果成功注册，返回输入法的编号，否则返回0
@exception
@note
*/
DWORD CSRegisterGuobiIme(void);

/**
@brief  功能描述:
		以插件方式注册输入法接口
 
@param[in]
		void *pUserdata 输入参数，暂未用
 
@param[out] 
		无
 
@return
 		如果成功注册，返回输入法的编号，否则返回0
@exception
@note
*/
DWORD CSPluginIme_GuobiInit(void *pUserdata);

/**
@brief  功能描述:
		获取国笔输入法注册模块名称
 
@param[in]
		无
 
@param[out] 
		无
 
@return
 		输入法模块名称
@exception
@note
*/
char * CSPluginIme_GuobiGetName(void);

/**
@brief  功能描述:
		设置国笔输入法自造词保存路径
 
@param[in]
		const char * pcFilePath 自造词路径,该值只需设置到目录即可，例如:"workdir/mnt/nfs/ime"
 
@param[out] 
		无
 
@return
 		成功设置，返回E_IMM_RETURN_OK，否则返回E_IMM_RETURN_FALSE
@exception
@note
*/
int CSSetGuobiImeUDBPath(const char * pcFilePath);
#ifdef __cplusplus
}
#endif

#endif  /* CSGUOBIIME_H */

/* 文件结束 */

