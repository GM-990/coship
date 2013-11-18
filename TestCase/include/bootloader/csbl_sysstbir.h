/******************************************************************

@defgroup   bl自动读取遥控器键值，识别多种遥控器。
@应用场景:


当bl开发时候，可以知道需要支持多少种遥控器。
但是， 当bl开发完成后，如果还需要添加新的遥控器，为了不需要修改bl,
可以直接生成1个新的ir_key.bin烧录到遥控器配置区域，即可。

@version    1.0.0   bootloader     2011/9/14
***********************************************************************/

#ifndef _CSBL_SYSSTB_IR_H_
#define _CSBL_SYSSTB_IR_H_

#include "udiplus_debug.h"
#include "cs_ir_plus.h"

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************************
@brief 从flash里面获取遥控器键值，用户ID等等参数
@dwAdrr: ir_key.bin文件在flash的地址
@g_CSIREquipFlashList[out]:  输出的遥控器结构体
@pdwCount[out]: ir_key.bin里面存在的遥控器个数

@return 成功返回TRUE, 失败返回FALSE
************************************************************/
BOOL  CSBLGetSTBIRInfo(DWORD dwAdrr, CSIREquipList_t * g_CSIREquipFlashList, DWORD * pdwCount);


#ifdef	__cplusplus
}
#endif


#endif


