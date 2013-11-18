/**@defgroup CSSHELL_FONT_H_
@brief 本模块为百汇中间件的shell字库接口
@note 用户可以使用shell内部的字库，也可以使用用户自己的字库
@{
*/
#ifndef CSSHELL_FONT_H_
#define CSSHELL_FONT_H_

#include "mmcp_typedef.h"

#ifdef __cplusplus
	extern "C"
	{
#endif
/**
@brief 定义初始化字体用到的结构 
*/
typedef struct _CSShellFont
{
	const unsigned char* m_pcMatrixData;	///<字模数据
	int m_nLength;			///<字模数据的长度
}CSShellFont_S;

/**
@brief 用文鼎字体引擎初始化字体字库
@param[in] 字库
@note 必须在graph初始化之前调用该接口
@note 通常情况下，应用可以通过get接口获取对应的字体结构体，当需要自己设置该psFont结构体时，初始化后不能释放该结构体
@see CSShellFontGetDefaultArpFont,CSShellFontGetGBKArpFont,CSShellFontGetAsciiArpFont,CSShellFontGetBig5ArpFont
@code
//以使用默认的文鼎字库为例
CSShellFontInitArpFont(CSShellFontGetDefaultArpFont());
@endcode
*/
void CSShellFontInitArpFont(CSShellFont_S * psFont);

/**
@brief 用华文字体引擎初始化字体字库
@param[in] 字库
@note 必须在graph初始化之前调用该接口
@note 通常情况下，应用可以通过get接口获取对应的字体结构体，当需要自己设置该psFont结构体时，初始化后不能释放该结构体
@note 由于版权问题，shell默认不提供华文字库。但提供了初始化华文字库的接口。如果项目要使用华文字库，请确认获得授权后，将华文字库加入项目中编译即可
@see CSShellFontGetDefaultStfFont
@code 
//以使用默认的华文字库为例
CSShellFontInitStfFont(CSShellFontGetDefaultStfFont());
@endcode
*/
void CSShellFontInitStfFont(CSShellFont_S * psFont);

/**
@brief 获取默认的文鼎字体字库,即GB2312Plus
@return 文鼎字库GB2312Plus
*/
CSShellFont_S * CSShellFontGetDefaultArpFont(void);	//

/**
@brief 获取的文鼎字体的GBK字库
@return 文鼎字库GBK
*/
CSShellFont_S * CSShellFontGetGBKArpFont(void);

/**
@brief 获取的文鼎晶栩中黑字体的GB2312字库
@return 文鼎晶栩中黑字体的GB2312字库
*/
CSShellFont_S * CSShellFontGetJXZHArpFont(void);

/**
@brief 获取的文鼎字体Ascii字库
@return 文鼎字库Ascii
*/
CSShellFont_S * CSShellFontGetAsciiArpFont(void);

/**
@brief 获取的文鼎字体Ascii字库
@return 文鼎字库Ascii
*/
CSShellFont_S * CSShellFontGetBig5ArpFont(void);

/**
@brief 获取默认的华文字库
@return 华文默认字库
@note  华文字库要慎用。涉及商务问题。除非运营商要求使用该字库，且已付费，否则不能使用
*/
CSShellFont_S * CSShellFontGetDefaultStfFont(void);

#ifdef __cplusplus
}
#endif

/** @} */

#endif//_CS_EW_FONTREGISTER_H_

