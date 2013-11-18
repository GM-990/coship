/**@defgroup IRDETOBOOT IRDETOBOOT 提供irdeto规范booter接口
@brief IRDETOBOOT模块对应头文件<csbl_irdetoboot.h>，定义了irdeto规范booter所有的功能及配置接口。

@version 1.0.0 2011/3/11
@{
*/
#ifndef _CSBL_IRDETOBOOT_H_
#define _CSBL_IRDETOBOOT_H_

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief Irdeto规范booter入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryIrdetoBoot(void);


#ifdef  __cplusplus
}
#endif

#endif



