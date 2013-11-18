/**@defgroup PublicShellCA PublicShellCA 定义ShellCA公共对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_DM_H
#define CSSHELL_DM_H

#include "mmcp_typedef.h"
#include "CSUSPDMCommonType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
@brief 获取usp-dm的handle
@param 无
@return usp-dm handle
@note shell模块未提供usp-dm，usp-section的的初始化接口。这两个模块的初始化需要应用自行调用usp-dm,usp-section的接口来做。
考虑到usp提供的获取以初始化过的dm handle的方法不好用，故提供此接口。获取section的handle请调用Section2CGetSectionSourceHandle。
*/
HCSHANDLE CSShellDMGetContainerHandle();

/**
@brief 保存节目的音量
@param [in] hService 节目句柄
@param [in] ucVolume 音量大小
@note 统一音量保存并不能满足每个项目的需求。对于每个节目都需要保存音量的项目提供此接口。
节目音量目前保存在dm的userData区中第一个位置。该做法是兼容shell 1.0版本的
在mmmcp hmc工程中，应用对dm用户数据区的使用是受限制的，原来这些userData区被用来保存固定的内容.
根据尹工的建议，如果用户需要保存自己的数据，最好以文件形式来保存。
*/
void CSShellDMSaveVolInfo(int hService, BYTE ucVolume);

/**
@brief 获取节目的音量
@param [in] hService 节目句柄
@return 节目音量
@note 该接口不适用于统一音量保存时，获取节目音量的情况。
*/
BYTE CSShellDMGetVolInfo(int hService) ;

/**
@brief 获取节目的名称
@param[in] hService 节目句柄
@param[out] pemCodingType 节目名称的编码类型
@param[out] pcServiceName 节目名称的存储首地址,输出参数
@param[in] nLength 输出参数pcServiceName的长度
@return 成功返回TRUE,失败返回FALSE
@note 当因为节目名称太长导致DM存储不完整时，由应用自己存储，此时中间件
调用此方法来获取节目时，应用可自己实现此方法，从自己的userdata中获取完整的
节目名称来传给中间件
*/
typedef BOOL (*CSShellDMGetServiceName_F)(HCSHANDLE hService, CSUSPDMCodingType *pemCodingType, char *pcServiceName, int nLength);


/**
本结构定义了获取DM中的信息时用到的信息。
，最后可获取CSShellDMFunc_S
*/
typedef struct _CSShellDMFunc_S
{
    int                                 m_nSize;            ///< 本结构体的大小，必须赋值为sizeof(CSShellDMFunc_S)
    CSShellDMGetServiceName_F           m_fnGetServiceName; ///< 获取节目名称的函数	
}CSShellDMFunc_S;

/**
@brief 设置DM相关操作接口给MMCP
@param[in] psDMFunc  DM信息及相关操作结构体实例
@return 成功返回TRUE,失败返回FALSE
@code
//绝大多是情况下按此方式调用此接口即可。
CSShellDMSetFunc(CSShellDMGetDefaulFunc());
@endcode
*/
BOOL CSShellDMSetFunc(const CSShellDMFunc_S* psDMFunc);

	
/**
@brief 获取DM默认实现参数
@return MMCP提供DM Shell接口的默认实现
@note 本接口实际上实现了典型项目的配置信息，项目若不需要修改则可以直接将此典型实现设置给MMCP Shell模块
@note MMCP内部(dtvm native或jsext)不得直接使用本接口，若MMCP需要获得DM的对象须使用csshell_dm_private.h中的CSShellDMGetFunc接口
@see CSShellDMGetFunc
*/
const CSShellDMFunc_S* CSShellDMGetDefaulFunc(void);

#ifdef __cplusplus
}
#endif
/** @} */
#endif   



