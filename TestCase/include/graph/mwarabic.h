
#ifndef __MWARABIC_H
#define __MWARABIC_H

#include "mwcommon.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**
@brief 判断字符是否是阿拉伯文

@param[Uint16] pusInText : 要判断的Unicode字符串

@return 是返回TRUE 否则返回FALSE
@note   无
*/
BOOL GrFont_IsArabicText(Uint16 * pusInText);

/**
@brief 将传入的阿拉伯文转换成显示时的变体字符

@param[Uint16] pusInText   : 要转换的阿拉伯文字符串
@param[Uint16] pusOutText  : 转换后的阿拉伯文变体字符串存储首地址
@param[Uint16] pnOutTextLen: 初始化值为pusOutText能存储的字符长度,输出实际转换后的变体字符个数

@return 成功返回0，否则返回-1
@note   传入和输入的字符串都是Unicode字符串
*/
int GrFont_ChangeArabicText(Uint16 * pusInText, Uint16 * pusOutText, int *pnOutTextLen);


#ifdef  __cplusplus
}
#endif

#endif
