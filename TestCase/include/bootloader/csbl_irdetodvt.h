/**@defgroup IRDETODVT IRDETODVT 提供irdeto规范dvt接口
@brief IRDETODVT模块对应头文件<csbl_irdetodvt.h>，定义了irdeto规范booter所有的功能及配置接口。

@version 1.0.0 2011/3/7 
@{
*/

#ifndef _CSBL_IRDETODVT_H_
#define _CSBL_IRDETODVT_H_

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief Irdeto规范dvt入口函数

@return 如果成功函数不会返回;如果失败返回FAILURE
*/
int CSBLEntryIrdetoDvt(void);


#ifdef  __cplusplus
}
#endif

#endif



