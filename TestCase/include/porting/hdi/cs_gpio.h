/**@defgroup CSGPIO[CSGPIO]同洲项目专用GPIO驱动接口
@brief 本模块主要定义了同洲项目专用平台无关GPIO驱动接口
@brief GPIO的意思为通用输入输出口(general-purpose I/O port) GPIO (通用输入/输出)。基于udi的MMCP中间层不会用到这个模块，但是在具体的基于MMCP的项目的配置文件中应该会用到。

@note 定义0-0x7f为标准GPIO范围，由芯片提供，但实际有几个由芯片决定，一般不会有0x7f这么多
@note 定义0x80-0xfe为非标准GPIO范围，由外围硬件提供，但实际有几个有硬件板决定，一般不会到0xfe这么多
@note 定义0xff为特殊GPIO，所有相关操作都会直接返回成功

@version 0.2 2008/10/17 评审后的第一个版本
@version 0.1 2008/10/16 演示版本
@{
*/

#ifndef _CS_GPIO_H_
#define _CS_GPIO_H_

#include "cs_gpio_plus.h"

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

/** @} */

#endif


