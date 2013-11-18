/**@defgroup IRDETOLOAD IRDETOLOAD 提供irdeto规范loader接口
@brief IRDETOLOAD模块对应头文件<csbl_irdetoload.h>，定义了irdeto规范loader所有的功能及配置接口。

@version 1.0.0 2011/3/7 
@{
*/
#ifndef _CSBL_IRDETOLOAD_H_
#define _CSBL_IRDETOLOAD_H_

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief Irdeto规范loader入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryIrdetoLoad(void);

#ifdef  __cplusplus
}
#endif

#endif



