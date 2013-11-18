#ifndef CSSHELL_BL_PRIVATE_H
#define CSSHELL_BL_PRIVATE_H
#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum 
{
	EM_CSSHELL_BL_SUCCESS = 0,	///<成功获取或保存信息
	EM_CSSHELL_BL_TYPE_ERROR,	///<要获取或保存的信息类型不存在
	EM_CSSHELL_BL_LENGTH_ERROR, ///<保存信息的数据类型长度错误
	EM_CSSHELL_BL_NULL_POINT,	///<保存信息的结构体为空
	EM_CSSHELL_BL_INVALID_VALUE,///<要写入的信息非法。
	EM_CSSHELL_BL_READ_ERROR,	///<读取flash时错误
	EM_CSSHELL_BL_WRITE_ERROR	///<信息写入flash时错误，一般是写flash失败
}CSShelllBLErroCode_E;

/**@brief 获取bootloader信息接口.bootloader信息可能在界面上显示，也可能在ota升级时用到

@param[in] pcType 信息类型，由具体shell确定。
@param[in] pvValue 指向存储获取信息的内存地址。返回的可能是单个值，也可能是一个数据结构
@param[in] nLength value值的长度
@return 成功返回EM_CSSHELL_BL_SUCCESS；失败则返回错误代码值
@note 根据各具体的bootloader不同，该实现也不同。
*/
CSShelllBLErroCode_E CSShellBLGetSysInfo(char* pcType ,void* pvValue, int nlength);

/**@brief 设置ota信息接口.
@param[in] pcType 信息类型，由具体shell确定。
@param[in] pvValue 指向存储获取信息的内存地址。
@param[in] nLength value值的长度
@return 成功返回EM_CSSHELL_BL_SUCCESS；失败则返回错误代码值
@note 根据各具体的bootloader不同，该实现也不同。
*/

CSShelllBLErroCode_E CSShellBLSetSysInfo(char* pcType ,void* pvValue, int nlength);


#ifdef __cplusplus
}
#endif

#endif

