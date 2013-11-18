/**@defgroup INPUT INPUT 提供输入设备交互接口
@brief INPUT模块对应头文件<udi2_input.h>，定义了所有输入设备的控制接口。现对该模块
进行如下说明：

- 无论哪个模块的按键（遥控器、前面板等）均通过此模块通知到Eastwin。
- 遥控器的实际按键与功能键的对应关系根据项目的不同由运营商、COSHIP共同确定。例如：
某项目需要在按F1键时调出输入法，则需要在收到F1键时发送CS_VK_INPUT键值。
- 该模块未定义的键值一般会透传到应用层，如JavaScript或者Java应用中。但Eastwin内部
会有部分按键与逻辑相对应，如CSUDI_VK_OK对应进入超链接等，而不是完全透传。所以不可
假设Eastwin仅仅只做透传，而自行改变这些键值。

@{
*/

#ifndef _CSSHELL_IRCARD_H_
#define _CSSHELL_IRCARD_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#include "JSMessage.h"

#define  IRCARD_BASE					( JSCALL_USERDEF_MSG_START+100 )//项目独立使用问题
//IRCARD 消息
#define  JSCALL_IRCARD_EMPTY					( IRCARD_BASE+ 1 )//用于初始化
#define  JSCALL_IRCARD_DEVIC_ID					( IRCARD_BASE+ 2 )//设备ID
#define  JSCALL_IRCARD_KEYDOWN				( IRCARD_BASE+ 3 )//按键通知
#define  JSCALL_IRCARD_BRUSH_OK				( IRCARD_BASE+ 4 )//刷卡成功
#define  JSCALL_IRCARD_BRUSH_FAILED			( IRCARD_BASE+ 5 )//刷卡失败
#define  JSCALL_IRCARD_BRUSH_TIMEOUT			( IRCARD_BASE+ 6 )//刷卡超时
#define  JSCALL_IRCARD_DATA_ERROR				( IRCARD_BASE+ 7 )//数据错误
#define  JSCALL_IRCARD_NO_BRUSH				( IRCARD_BASE+ 8)//数据错误
#define  JSCALL_IRCARD_UNDER_VOLTAGE			( IRCARD_BASE+ 9)//电池欠压
#define  JSCALL_IRCARD_PASSWORD_NOT_ENOUGH	( IRCARD_BASE+ 10)//密码未输入或密码不足
#define  JSCALL_IRCARD_PASSWORD_OVERFLOW	( IRCARD_BASE+ 11)//密码超过6位
#define  JSCALL_IRCARD_PASSWORD_SHOW			( IRCARD_BASE+ 12)//显示密码
#define  JSCALL_IRCARD_SEND_ENCRYPT_DATA		( IRCARD_BASE+ 13)//发送密文数据
#define  JSCALL_IRCARD_SEND_FREE_DATA			( IRCARD_BASE+ 14)//发送明文数据

#define  JSCALL_IRCARD_UNKNOWN				( IRCARD_BASE+ 15)//未知通知

#ifdef __cplusplus
extern "C" {
#endif



CSUDI_Error_Code CSShellIrCardInit(void);
CSUDI_Error_Code CSShellIrCardDestroy(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif  



