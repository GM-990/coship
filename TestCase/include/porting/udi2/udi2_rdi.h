/**@defgroup RDI RDI Removable Device Interface提供可移动设备发现的通用接口
@brief RDI模块对应头文件<udi2_rdi.h>，主要定义统一的可移动设备通用发现、移除等流程
以及通用操作接口和相关数据结构、设备Id规则等内容，其本身不包括任何需实现的接口。
Eastwin中间件移植不需要关注可移动设备的项目，即无需关注本模块。

对于所有可移动设备，我们定义其Id为一个32位无符号数，其中高16位表示其主设备号，
一种设备类型，对应一个主设备号。低16位表示其从设备号，一个具体的设备对应一个
从设备号。同一个设备，反复拔插，对应的从设备号是不同的。\n
当有可移动设备插入到终端时（主要是USB设备），对应设备的UDI模块会通过注册的本模块
中定义的CSUDIRDICallback_F类型的回调函数通知Eastwin。由于Eastwin还没有启动或者完
成初始化之前，就可能有设备插入，并且相同的设备还可能包括非可移动设备（如内置硬盘
等设备），所以每个可能包含可移动设备的UDI模块都会包含一个获取所有已存在设备的Id的
接口，这意味着部分固定设备及开机前插入的设备即使不产生通知也是可以接受的，同样后
续注册的通知回调函数，无需补发之前已插入设备的相关消息。\n

- 一个正常的可移动设备插入后，消息抛出的流程应该为：Found->Ready，如果设备有包
含关系，则应先抛出父设备的Ready消息，再抛出子设备的Found消息，如U盘应先完成存
储设备的消息抛出，再抛出分卷设备的消息。如果设备需要自检，其Found和Ready消息之间，
可插入若干个Checking消息。如果设备由于某些不可修复的故障而无法工作，则其Ready消息
可被Error消息替代。\n
- 一个正常的可移动设备被拔出后，应抛出对应设备的Plugout消息，若设备有包含关系，
则应先抛出子设备的Plugout消息，再抛出父设备的Plugout消息，如U盘应先抛出分卷设备
的拔出消息，再抛出存储设备的拔出消息。

@note 所有的可移动设备还可以包括一套读写控制接口（IORead、IOWrite、IOCtrl），但
这套接口的实现并不是必须的，大多数情况下，我们会使用可移动设备的设备Id做为参数，
使用对应固定设备的接口来操作这些设备。

@version 2.0.3 2009/09/15 增加IORead,IOWrite接口应立刻返回的描述
@version 2.0.2 2009/09/10 修改CSUDIRDIEvent_E的参数，解决之前DeviceId重复的问题
@version 2.0.1 2009/09/09 添加ioctrl,ioread,iowrite等通用设备操作接口，并且允许部分模块不提供该功能
@version 2.0.0 2009/09/08 初稿
@{
*/

#ifndef _UDI2_RDI_H_
#define _UDI2_RDI_H_

#include "udi2_typedef.h"
#include "udi2_error.h"
#ifdef __cplusplus
extern "C" {
#endif

/**@brief 可移动设备主设备号掩码，所有可移动设备的设备号与该值相与得到其主设备号，主设备号即代表了该设备的类型，可移动设备的主设备号具体由设备操作模块定义，其值从0x00010000开始，即所有大于0x00010000的设备号都表示可移动设备。*/
#define CSUDI_RDI_MAJOR_DEVICEID_MASK		(0xFFFF0000)

/**@brief 可移动设备从设备号掩码，所有可移动设备的设备号与该值相与得到其从设备号，从设备号即代表了该类型设备的一个具体设备。 具体类型的可移动设备的从设备号均从0开始，单个设备每次插入拔出，从设备号都会发生变化。*/
#define CSUDI_RDI_MINOR_DEVICEID_MASK		(0x0000FFFF)

/**@brief 通过DeviceId获取对应设备是否为可移动设备*/
#define CSUDI_ISREMOVEABLE(dwDeviceId)		((dwDeviceId) & CSUDI_RDI_MAJOR_DEVICEID_MASK)

enum
{
	CSUDI_RDI_STORAGE_MAJOR_DEVICEID		=  1 << 16,		///< 存储设备主设备号，由udi2_fs模块进行控制
	CSUDI_RDI_VOLUME_MAJOR_DEVICEID			=  2 << 16,		///< 分卷设备主设备号，由udi2_fs模块进行控制
	CSUDI_RDI_TUNER_MAJOR_DEVICEID			=  3 << 16		///< 高频头设备主设备号，由udi2_tuner模块进行控制
};

typedef enum
{
    EM_UDIRDI_EVENT_UNKNOWN,       ///< 未知设备事件,非法值
    EM_UDIRDI_EVENT_FOUND,         ///< 发现设备已可获得设备信息,但尚不可访问访问,pEventData为CSUDI_NULL
    EM_UDIRDI_EVENT_CHECKING,      ///< 进入就绪状态前，需进行设备自检、安装等耗时较长的准备工作，发此消息通知应用耐心等待，pEventData为int类型，表示自检进度，范围为0-100，-1表示进度未知但仍在正常进行，如果无法知道状态，则请赋值为-1，自检过程中该消息应约每秒钟1次的频率反复发生。 
    EM_UDIRDI_EVENT_READY,         ///< 设备就绪,已可进行访问,pEventData为CSUDI_NULL
    EM_UDIRDI_EVENT_ERROR,         ///< 设备不可恢复错误,设备将无法访问,pEventData为错误代码(具体错误代码由模块定义，若未定义则仅用于显示和错误排查，不用于后续处理的依据)
    EM_UDIRDI_EVENT_PLUGOUT       ///< 设备移除,pEventData为CSUDIRDIPlugOutType_E，即拔除类型,该消息通知返回后将不可再通过该设备Id获取设备信息
}CSUDIRDIEvent_E;

/**@brief 引起PLUGOUT事件的动作类型

@note 由于许多动作会产生PLUGOUT事件，例如用户调用设备移除接口安全移除设备，用户直接拔除设备（非法），用户插入过多设备导致设备总线电流超过阀值而系统自动去移除该设备等等。
*/
typedef enum
{
    EM_UDIRDI_PLUGOUT_UNKNOWN,       ///< 未知plugout动作类型,非法值
    EM_UDIRDI_PLUGOUT_SAFE,          ///< 用户调用设备移除接口Remove安全移除设备产生的拔出消息
    EM_UDIRDI_PLUGOUT_UNSAFE,        ///< 用户直接非法拔除设备产生的拔出消息
    EM_UDIRDI_PLUGOUT_OVERFLOW      ///< 用户挂载设备过多导致超过负载阀值，比如超过负载电流时，最后一个插入的设备被系统自动移除，产生的拔出消息
}CSUDIRDIPlugOutType_E;

/**
@brief 定义设备通知函数原形

当有设备状态发生变化(插入、拔出等)时,移植层通过该类型的回调函数通知用户。
@param[in] eType 事件名称,详细定义见CSUDIRDIEvent_E说明.
@param[in] dwDeviceId：设备id号，即代表了产生该消息的设备，包括主从设备号。
@param[in] pvEventData：与事件关联的数据,详细定义见CSUDIRDIEvent_E的枚举值说明。
@param[in] pvUserData 用户数据,调用CSUDIRDIAddCallback_F注册该回调时传入的pvUserData。
@note pvEventData在函数返回后完全失效(包括其内部指针指向的内容)，需应用在上层自行进行拷贝。
*/
typedef  void ( *CSUDIRDICallback_F )( CSUDIRDIEvent_E eType, CSUDI_UINT32 dwDeviceId,const void * pvEventData, void * pvUserData);

/**
@brief 添加设备通知回调函数原形。

所有可移动设备操作模块均需要实现一个此形式的函数，用于添加该类型设备的监听函数。
 
@param[in] fnCallback：回调函数，当设备状态发生变化时调用该函数。
@param[in] pvUserData：用户数据，将作为回调函数的pvUserData参数返回。

@return 成功添加返回CSUDI_SUCCESS；失败返回错误代码值。
@note 添加回调前产生的设备变化信息，不会补充通知。即若开机时这些设备就存在，调用本函数时不会得到通知。
*/
typedef CSUDI_Error_Code ( *CSUDIRDIAddCallback_F )(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 删除设备通知回调函数原形

所有可移动设备操作模块均需要实现一个此形式的函数，用于删除该类型设备的监听函数。
 
@param[in] fnCallback：回调函数，之前注册的函数。
@param[in] pvUserData：用户数据，必须与注册时的一致。
@return 成功删除返回CSUDI_SUCCESS；失败返回错误代码值。
@note fnCallback和pvUserData一起唯一确定一个回调函数。
*/
typedef CSUDI_Error_Code ( *CSUDIRDIRemoveCallback_F )(CSUDIRDICallback_F fnCallback,const void * pvUserData);

/**
@brief 删除存储设备原形。

所有可移动设备操作模块均需实现一个此形式的函数，用于移出该类型设备的实例。
@param[in] dwDeviceId：目标设备Id。
@return 成功则返回CSUDI_SUCCESS;失败返回错误代码值
@note 
- 只能删除可移动设备类型的设备，试图删除其它设备类型将返回错误。
- 删除设备后不可再通过该设备Id获取设备信息。
- 用户调用该接口会触发EM_UDIRDI_PLUGOUT_SAFE类型的EM_UDIRDI_EVENT_PLUGOUT消息。
*/
typedef CSUDI_Error_Code ( *CSUDIRDIRemoveDevice_F )(CSUDI_UINT32 dwDeviceId);

/**
@brief 从可移动设备读取数据原形

可移动设备操作模块可实现一个此形式的函数，用于从指定该类设备实例中读取数据，
这个接口是可选的，如果不需要从设备中读取数据，可不实现该接口。
 
@param[in] dwDeviceId：目标设备Id。
@param[in] pvData：指向读入数据的Buffer。
@param[in] nBuffLength：pvData的长度。
@param[out] pnReadLength：实际读到的数据长度。
@return 成功则返回CSUDI_SUCCESS；失败返回错误代码值。
@note
- 接口应在任何情况下均立刻返回，若不能读到任何数据，则*pnReadLength==0，但仍返回成功
- 只能读取本类型可移动设备类型的设备，试图读取其它设备类型将返回错误
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIORead_F)(CSUDI_UINT32 dwDeviceId,void* pvData,int nBuffLength,int* pnReadLength);

/**
@brief 从可移动设备写入数据原型

可移动设备操作模块可实现一个此形式的函数，用于向指定该类设备实例中写入数据，
这个接口是可选的，如果不需要向设备中写入数据，可不实现该接口。
@param[in]  dwDeviceId 目标设备Id。
@param[in] pvData 指向写入数据的Buffer。
@param[in]  nBuffLength pvData的长度。
@param[out]  pnWriteLength：实际写入的数据长度。
@return 成功则返回CSUDI_SUCCESS；失败返回错误代码值。
@note
- 接口应在任何情况下均立刻返回，若数据不能一次写完，则*pnWriteLength<nBuffLength，甚至*pnWriteLength=0，但仍返回成功
- 只能往本类型可移动设备类型的设备写入数据，试图写入其它设备类型将返回错误
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIOWrite_F)(CSUDI_UINT32 dwDeviceId,void* pvData,int nBuffLength,int* pnWriteLength);

/**
@brief 可移动设备控制接口原形。

可移动设备操作模块可实现一个此形式的函数，用于控制设备信息，这个接口是可选的，如果不需要进行设备控制或通过其它专用接口控制设备，可不实现该接口。 
@param[in] dwDeviceId 目标设备Id。
@param[in] cmd 命令，具体命令由可移动设备模块自行定义。
@return 成功则返回CSUDI_SUCCESS,失败返回错误代码值
@note
- 接口应在任何情况下均立刻返回，若不能读到任何数据，则*pnReadLength==0，但仍返回成功
- 只能控制本类型可移动设备类型的设备，试图控制其它设备类型将返回错误
*/
typedef CSUDI_Error_Code ( *CSUDIRDIIOCtrl_F)(CSUDI_UINT32 dwDeviceId,int cmd,...);


#ifdef __cplusplus
}
#endif

/** @} */

#endif   
