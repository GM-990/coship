/**@defgroup 数字太和规范，提供给应用读取数字太和config表中的信息
@brief 模块对应头文件<csbl_UnitendLib.h>。

@version 1.0.0 2011/12/15 
@{
*/
#ifndef _CSBL_UNITENDLIB_H_
#define _CSBL_UNITENDLIB_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define BLUNITENDDESCRIPT_MAX_LEN (14)

typedef struct
{
	WORD  wConfigLen;
	BYTE	aucUConfigDes[BLUNITENDDESCRIPT_MAX_LEN];
	WORD 	wOperateId;
	WORD 	wRegionId;
	DWORD 	dwLoaderVer;
	DWORD dwModuleId;
	DWORD   dwCrc;
}T_BLUnitendConfig;


/**
@brief 初始化数字太和config表

@return 成功返回TRUE;如果失败返回FALSE

@note 调用本接口前，需要先通过CSBLSetConfig(具体见csbl_gconfig.h)设置数字太和config表的地址
*/
BOOL CSBLInitUnitendConfig(void);


/**
@brief 获取数字太和config表中信息

@Param[1]:保存获取到的信息
@Param[2]:传入的buf的长度

@return 成功返回TRUE;如果失败返回FALSE

@note 调用本接口前，需要先调用CSBLInitUnitendConfig初始化config表
*/
BOOL CSBLGetUnitendConfig(T_BLUnitendConfig* pUnitendCfg,DWORD dwLen );

#ifdef  __cplusplus
}
#endif

#endif



