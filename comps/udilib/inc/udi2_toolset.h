/**@defgroup TOOLSET TOOLSET定义平台无关的工具集接口
@brief Toolset模块对应头文件<udi2_toolset.h>，定义了包括序列号、系统待机重启以及
OTA控制等基础的综合接口。其内容主要包括序列号部分、重启待机部分。\n

@par 序列号部分
序列号部分包括获取芯片ChipID及产品序列号两个接口。\n
 	1、芯片ChipID为芯片唯一标识，其格式及内容由芯片决定。若所选芯片确实不支持获
 	取芯片ChipID，则需向COSHIP提出，若确认确实无法实现则可返回
 	CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED。\n
 	2、产品序列号为机顶盒生产前，COSHIP配合运营商给出的机顶盒上运行的Eastwin软件
 	的唯一标识号，每台机顶盒的序列号都是唯一的。即如果生产10W台包含Eastwin软件的
 	机顶盒，COSHIP会提供10W个序列号，每台机顶盒需在生产时升级一个序列号，并通过
 	获取产品序列号接口提供给Eastwin软件。\n

@par 重启待机部分
重启待机部分包括关闭机顶盒、重启机顶盒以及待机三个接口。\n
需特别注意的是待机接口可能是真待机，也可能是假待机。Eastwin对真假待机的操作顺序
不同。\n
 	1、如果是假待机，待机接口会马上返回，Eastwin进入待机状态，此情况下若需要退出
 	待 机状态需再次以CSUDI_FALSE为参数调用待机接口。\n 
 	2、如果是真待机，待机接口返回时，待机过程已经结束，UDI的实现会处理唤醒的过程，
 	而对于Eastwin来说，这个过程将是完全透明的，甚至根本感觉不到有这个过程存在。退
 	出待机状态后所有状态（包括内存数据，设备的连接等）都保持不变。

-------------------------------------------------------------------------------
ENTROPIC COMMENTS ON COSHIP HEADER FILE:
	2013/11/06
		The APIs in this header file are NOT required for Android DVB-S2 plus OTT project. 
-------------------------------------------------------------------------------

@version 2.0.3 2009/09/01 取消初始化接口,增加OTA接口
@version 2.0.2 2009/08/25 把初始化接口分开成全部和基本初始
@version 2.0.1 2009/08/16
@{
*/
#ifndef _UDI2_TOOLSET_H_
#define _UDI2_TOOLSET_H_

#include "udi2_typedef.h"
#include "udi2_public.h"
#include "udi2_error.h"

#ifdef  __cplusplus
extern "C" {
#endif

#define CSUDI_CHIPID_MAX_LENGTH   (32)    ///< 主芯片的ID的最大长度
#define CSUDI_SERIAL_MAX_LENGTH   (64)    ///< 系统序列号的最大长度
#define CSUDI_KEY_MAX_LENGTH  	  (512)   ///< 校验密钥的最大长度

/**@brief 工具集模块相关错误代码值。*/
enum
{
	CSUDITOOLSET_ERROR_BAD_PARAMETER = CSUDI_TOOLSET_ERROR_BASE,///< 错误参数
	CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED     				///< 不支持操作
};

/**@brief 主芯片的ID信息。*/
typedef struct 
{
	int 	m_nLength;						///< 主芯片的ID的长度，其长度不超过CSUDI_CHIPID_MAX_LENGTH
	char    m_szChipID[(CSUDI_CHIPID_MAX_LENGTH+1+3) & (~3)];	///< 主芯片的ID。在许多具有高级安全的主芯片中，存在唯一的ID号。	
}CSUDICHIPID_S;

/**@brief 机器的序列号和Key值。

该序列号需在生产前由同洲配合运营商给出，并需要升级到STB中
*/
typedef struct 
{
	char m_szSerial[(CSUDI_SERIAL_MAX_LENGTH+1+3) & (~3)];	///< 系统序列号标识，一台机器只具有一个唯一标识，用于校验
	char m_szPairsKey[(CSUDI_KEY_MAX_LENGTH+1+3) & (~3)];	///< 序列号配对码，一台机器只具有一个唯一的加密，用于校验
}CSUDISerialKey_S;

/**@brief 引导类型枚举*/
typedef enum
{
	EM_UDITOOLSET_EXECTYPE_UNKONWN,         ///< 边界值
	EM_UDITOOLSET_EXECTYPE_APP,             ///< 启动目标为应用
	EM_UDITOOLSET_EXECTYPE_LOADER           ///< 启动目标为Loader
}CSUDIToolsetExecType_E;

/**@brief 引导信息结构*/
typedef struct
{
	CSUDIToolsetExecType_E  m_eType;        ///< 引导类型
	void*                   m_pCodeAddr;    ///< 需引导代码段内存首地址或Flash首地址
	int                     m_nCodeLength;	///< 需引导代码段长度
}CSUDIToolsetExecInfo_S;

/**@brief 设置待机属性枚举*/
typedef enum
{
	EM_SYS_STANDBY_ATTR_INFO,         ///< 设置机顶盒待机后前面板显示值，对应的pvValue 为CSUDIPanelLightValue_S 类型的指针，若未设置此属性，默认前面板不做特殊处理。
	EM_SYS_STANDBY_ATTR_ALL,          ///< 设置机顶盒所有模块待机, 包括外围设备，如WIFI、CM等设备, pvValue为NULL。
	EM_SYS_STANDBY_ATTR_WIFI,         ///< 设置机顶盒WIFI模块待机, pvValue为NULL。
	EM_SYS_STANDBY_ATTR_STB           ///< 设置机顶盒自身模块待机, pvValue为NULL。
}CSUDISYSStandbyAttribute_E;

/**
@brief 获取主芯片的ID。

@param[out] psChipID  用于存放获取到的Chip ID及其长度,请参见CSUDICHIPID_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 芯片若不支持获取ChipId该项需根据芯片情况与Coship协商处理,协商后若确认确实无法实现则返回CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED。
*/
CSUDI_Error_Code CSUDIGetChipID(CSUDICHIPID_S * psChipID);


/**
@brief 获取目标机器的序列号和Key。

@param[out] psSerialKey 用于存放获取到的序列号和Key，序列号和Key均是以0结尾的字符串，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDIGetSerialKey(CSUDISerialKey_S * psSerialKey);

/**
@brief 设置目标机器的序列号和Key。

@param[in] psSerialKey 用于存放设置的序列号和Key，请参见CSUDISerialKey_S。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 该接口目前仅使用m_szPairsKey 域。
*/
CSUDI_Error_Code CSUDISetSerialKey(const CSUDISerialKey_S * psSerialKey);

/**
@brief 软重启目标机器。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
*/
CSUDI_Error_Code CSUDISYSReboot(void);

/**
@brief 实现系统的关机。

@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
- 执行成功，该函数可以不返回。
- 若不支持该功能可返回CSUDITOOLSET_ERROR_FEATURE_NOT_SUPPORTED。
*/
CSUDI_Error_Code CSUDISYSHalt(void);	

/**
@brief 设置待机属性。
@param[in] eAttribute 为CSUDISYSStandbyAttribute_E 类型，表示需要设置的待机属性类型
@param[in] pValue 为任意指针类型，具体类型需根据eAttribute 确定，可参考CSUDISYSStandbyAttribute_E 注释
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note
-若设置的待机属性为EM_SYS_STANDBY_ATTR_INFO ，集成厂家需根据项目需求决定待机后是否变化。( 如时间)
*/
CSUDI_Error_Code CSUDISYSSetStandbyAttribute(CSUDISYSStandbyAttribute_E eAttribute, void* pValue);

/**
@brief 使系统进入待机状态或者退出待机状态。

@param[in] bStandby 为CSUDI_TRUE时表示进入待机状态;为CSUDI_FALSE时表示退出待机状态。
@param[in] nStandbyTime 待机持续时间，单位为秒。即nStandbyTime秒后自动唤醒，为0表示不自动唤醒。仅当真待机时有效，假待机时忽略该参数。
@param[out] pbReal 为CSUDI_TRUE时表示是真待机;为CSUDI_FALSE时表示是假待机状态。
@return 成功返回CSUDI_SUCCESS；失败则返回错误代码值。
@note 
- 若为假待机，则该接口马上返回，并且*pbReal被置为CSUDI_FALSE，此情况下若需要退出待机状态需再次以bStandby=CSUDI_FALSE调用本接口。\n
- 若为真待机，则该接口在退出待机状态时返回，并且*pbReal被置为CSUDI_TRUE，此情况下不需要再以bStandby=CSUDI_FALSE调用本接口，且退出待机状态时所有状态（包括内存数据，设备的连接等）保持不变。
- 无论真假待机均会事先关闭音视频播放。
*/
CSUDI_Error_Code CSUDISYSStandby(CSUDI_BOOL bStandby,int nStandbyTime,CSUDI_BOOL *pbReal);

/**
@brief 程序引导接口，Booter版专用，其它版无需实现

将被引导代码放置到指定位置后，使用该接口使得程序跳转到此地址。
@param[in] psExecInfo 存储被引导代码段信息
@return 该接口调用后直接进入被引导代码的主函数，无需返回。
@note 被引导代码段可能在内存中，也可能在Flash上，根据需求的差异，选择实现即可。
*/
void CSUDIToolsetExec(CSUDIToolsetExecInfo_S *psExecInfo);

#ifdef  __cplusplus
}
#endif
/**@}*/
#endif


