/**@defgroup COSHIPBOOT COSHIPBOOT 提供coship规范booter接口
@brief COSHIPBOOT模块对应头文件<csbl_coshipboot.h>，定义了coship规范booter所有的功能及配置接口。

@version 1.0.0 2011/3/11  
@{
*/

#ifndef _CSBL_COSHIPBOOT_H_
#define _CSBL_COSHIPBOOT_H_

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief Coship规范booter入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryCoshipBoot(void);

#ifdef  __cplusplus
}
#endif

#endif



