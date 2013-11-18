/**@defgroup PublicShellSAU PublicShellSAU 定义ShellSAU公共对外接口

@version 2.0.0 2009/12/16 初稿
@{
*/
#ifndef CSSHELL_SAU_H
#define CSSHELL_SAU_H

#include "mmcp_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "mmcp_typedef.h"
#include "mmcp_os.h"
#include "CSUSPDMCommonType.h"
#include "SectionInterface2C.h"
#include "SICommonType.h"
/**
@brief 将ts流中的原始节目类型转化为dm中的节目类型
@param [in] index ts流sdt表中描述的原始节目类型
@return 对应的dm中的节目类型。由项目根据原始节目类型来获取
*/
typedef CSUSPDMServiceType (*GetServiceType_F)(BYTE index);

/**
@brief 判断节目是否需要被过滤
@param [in] hSrv 节目句柄
@return 需要过滤返回TRUE，否则返回FALSE
*/
typedef BOOL (*IsNeedFilterService_F)(HCSHANDLE hSrv);

/**
@brief 比较两个节目,以决定节目排序位置
@param [in] hSrv1 节目1的句柄
@param [in] hSrv2 节目2的句柄
@return 如果节目1排在节目2之前，则返回-1；如果节目1排在节目2之后，则返回1；节目相同则返回0
*/
typedef int  (*CompareService_F)(HCSHANDLE hSrv1,HCSHANDLE hSrv2);

/**
@brief 对dm中所有节目进行过滤
@return 无
@note 如果不需要过滤节目，该接口可以不实现或者直接返回
*/
typedef void (*FilterAllService_F)();

/**
@brief 解析节目更新标志
@param [in] pbData 更新标志所在的section数据
@param [in] nLength  pbData的长度
@return 得到更新标志返回TRUE,否则返回FALSE
*/
typedef BOOL  (*ParseUpdateFlag_F)(PBYTE pbData, DWORD nLength);

/**
@brief 保存更新标志。目前更新标志统一保存在环境变量中关键字为UpdateFlag

*/
typedef void (*SaveUpdateFlag_F)();

/**breif 对nit，sdt，bat表私有描述符解析的回调*/
typedef void (*ParseDescriptor_F)(HCSHANDLE hTP, PBYTE pDescriptorData, WORD dwDesLen);

/**
@brief sau回调注册结构。sau提供多个函数指针然应用实现，根据项目不同这些实现而不同。应用是先后，在系统初始化时将这些实现的函数注册进sau。
*/
typedef struct _CSShellSAUCallback
{
	int m_nSize;		//回调注册结构指针大小，必须等于sizeof(CSShellSAUCallback)

	GetServiceType_F m_fnGetServiceType;
	IsNeedFilterService_F m_fnIsNeedFilter;
	CompareService_F m_fnCompareService;
	FilterAllService_F m_fnFilterAllService;
	ParseUpdateFlag_F m_fnParseupdateFlag;
	ParseDescriptor_F  m_fnParseNitDescriptor;
	SaveUpdateFlag_F m_fnSaveUpdateFlag;
}CSShellSAUCallback;

/**
@brief 注册sau模块用到的函数指针的实现
@param [in] sCallback 函数回调注册结构
@return 无
*/
void CSShellSAURegisterCallBack(CSShellSAUCallback sCallback);

/**
@brief 设置sau时更新数据处理状态。
@param [in] bState 如果正在处理更新数据为TRUE，否则为FALSE
*/
void CSShellSAUSetProcessState(BOOL bState);

/**
@brief 获取sau更新数据处理状态
@param 无
@return 正在处理更新数据为TRUE，否则为FALSE
*/
BOOL CSShellSAUGetProcessState();

#ifdef __cplusplus
}
#endif
/** @} */
#endif   



