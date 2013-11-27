/**@defgroup TYPEDEF TYPEDEF 定义UDI乃至整个EastWin系列软件中的基本数据类型

@brief Typedef模块对应头文件<udi2_typedef.h>，主要定义UDI乃至整个Eastwin系列软件中
的基本数据类型，如CSUDI_BOOL、CSUDI_HANDLE等，没有接口需要实现。现对定义的32位整
数型变量和64位整数型变量的定义做如下说明：

- 本模块虽然有定义CSUDI_INT32、CSUDI_UINT32等32位整数型变量，但除非需要精确控制
变量的长度，如位运行，否则Eastwin会尽量使用C语言原生的数据结构而不是这里的定义。\n
- 如果平台不支持原生64位整数（这样的平台已经越来越少了），或者对64位整数的运算不
符合C99规范，则不需要定义MMCP_SUPPORT_LL宏，这种情况下UDI会通过一个Struct模
拟64位整数，并在Eastwin内部提供该模拟的64位数学运算方法。需要特别注意的是，部分
UDI接口中已包含了CSUDI_UINT64类型的参数，在实现这些接口时，若需要涉及64位整数的
运算，需移植层自行实现。\n

@note 本模块仅定义结构，没有接口需要实现。
@note 除非特别说明，整个UDI接口必须保证线程安全。

@{
*/

//这里描述整个UDI的总体描述及版本信息，所有涉及UDI的改动，请在这里记录

/**@mainpage
@version <B>UDI_V2.2.13</B> 2013/05/22
-增加udi2_smc模块错误码枚举类型，解决当智能卡不支持某命令时，没有准确错误码返回的问题

@version <B>UDI_V2.2.12</B> 2013/03/21
-增加CSUDISYSSetStandbyAttribute 接口，用于设置待机属性

@version <B>UDI_V2.2.11</B> 2013/03/20
- 修改CSUDIWLanApInfo_S 结构体注释，明确了nSignalStrength 和nLinkQuality 成员的取值范围

@version <B>UDI_V2.2.10</B> 2013/02/23
- 增加CSUDIOTAWIFIStart接口，控制wifi升级的启动
- 增加EM_UDIOTA_WIFI_ALL_FINISH消息，用于获取wifi升级状态

@version <B>UDI_V2.2.9</B> 2013/01/23
- 增加CSUDIRECPause 和CSUDIRECResume接口，控制录制过程

@version <B>UDI_V2.2.8</B> 2013/01/09
- 修改CSUDIOTAWIFICakllback_F参数，增加返回具体消息类型的参数
- 修改CSUDIEtherCallback_F参数，增加返回无线网卡名字的参数
- 修改CSUDIIPCFGGetAPSignalQulityStrength注释，建议废弃掉此接口
- 增加CSUDIOTASetInfo传入的url类型

@version <B>UDI_V2.2.7</B> 2012/11/23
- 增加可拔插网卡消息监听接口

@version <B>UDI_V2.2.6</B> 2012/10/15
- 增加获取播放文件属性的接口
- 增加设置播放文件属性的接口

@version <B>UDI_V2.2.5</B> 2012/08/13
- 增加WIFI升级相关接口

@version <B>UDI_V2.2.4</B> 2012/07/09
- CSUDIFilePlayerEventType_E中增加显示字幕相关事件类型

@version <B>UDI_V2.2.3</B> 2012/06/05
- 增加文件播放专用的回调和各种事件类型

@version <B>UDI_V2.2.2</B> 2012/05/05
- 增加获取和设置串口属性的接口

@version <B>UDI_V2.2.1</B> 2012/04/18
- CSUDIPLAYERType_E增加EM_UDIPLAYER_PVR(PVR录制播放模式)

@version <B>UDI_V2.2.0</B> 
- 增加OS,HDMI,USB接口
- 增加开机boot显示相关接口
- 增加蓝牙功能相关接口
- 增加AP功能相关接口

@version <B>UDI_V2.1.3</B> 2010/11/11
- 增加PPPOE功能相关接口

@version <B>UDI_V2.1.2</B> 
- 增加BooterLoader相关接口，部分接口被标注为Booter专用。
- 增加NDS OTA待机状态选项，用于NDS待机状态下OTA。
- 增加EW200Service静音及音量信息选项，去掉VOD版本及模式选项。
- 增加Inject模块PCM注入大小端默认值说明
- 增加IPv6相关接口
- 修正大量文字描述，合并使用指南文档
- 细化Player、Inject设备选取方面的一些原则说明
- 细化播放时PID设置相关的说明
- 增加任何时候都要能够获取分辨率的说明
- 修改help键的键值
- 增加增加认证、IPQAM等交互选项
- 增加I帧注入专用接口
- 增加假待机接口，原有接口仅用作真待机

@version <B>UDI_V2.1.1</B> 2009/09/26
- 增加更详细的模块总体说明及要点说明
- 调整大量接口语言描述使之更清晰更容易理解
- 去掉枚举最后一个成员的逗号，以解决部分平台无法编译通过的问题
- 添加Tuner,Demux连接状态查询功能；添加Tuner和Demux不可连接的错误代码说明
- 修改获取Tuner信息接口，使得现在可以单独获取部分信息以提高效率
- 修正一些文档语言描述性的错误

@version <B>UDI_V2.1.0</B> 2009/09/09
- 增加可移动设备的支持，包括Tuner,存储设备;统一了可移动存储设备的接口

@version <B>UDI_V2.0.3</B> 2009/08/28
- 增加了图片硬解码接口；增加了demux连接tuner的接口

@version <B>UDI_V2.0.2</B> 2009/08/25
- 经过第一次综合评审

@version <B>UDI_V2.0.1</B> 2009/08/16
- 初始版本
*/

#ifndef _UDI2_TYPEDEF_H_
#define _UDI2_TYPEDEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********以下内容在绝大多数平台上,不需要改动*********************/

typedef int                 CSUDI_BOOL;           ///< 布尔类型
typedef void *			    CSUDI_HANDLE;         ///< 句柄类型,其长度等于指针长度。@note Eastwin会将CSUDI_NULL当做一个非法的句柄，请在实现时特别注意所有Handle的取值范围

//除非需要精确控制长度,否则不建议使用以下定义
typedef signed char	        CSUDI_INT8;             ///< 8位有符号数
typedef unsigned char	    CSUDI_UINT8;            ///< 8位无符号数

typedef signed short	    CSUDI_INT16;            ///< 16位有符号数
typedef unsigned short	    CSUDI_UINT16;           ///< 16位无符号数

typedef signed long	        CSUDI_INT32;            ///< 32位有符号数
typedef unsigned long	    CSUDI_UINT32;           ///< 32位无符号数,注意禁止使用CSUDI_UINT32存储指针

//gcc,VC提供的头文件均按此方式定义
#ifdef __cplusplus
#define CSUDI_NULL    		0
#else
#define CSUDI_NULL    		((void *)0)
#endif

#define CSUDI_FALSE         (0 == 1)
#define CSUDI_TRUE          (!(CSUDI_FALSE))


/***********以下内容可能会根据平台的不同,经常改变,定义以下结构仅为兼容部分第三方代码而设,请尽量不要使用*********************/


/**@brief 若平台支持64位数,则必须定义以下类型,否则可不关注

@note 该宏定义必须由平台决定是否支持，并进行定义
*/

//#define MMCP_SUPPORT_LL

#ifdef MMCP_SUPPORT_LL

typedef long long           CSUDI_INT64;            ///< 64位有符号数,当定义MMCP_SUPPORT_LL时有效
typedef unsigned long long  CSUDI_UINT64;          ///< 64位无符号数,当定义MMCP_SUPPORT_LL时有效

#else

/**@brief 64位有符号数结构体*/
typedef	struct
{
	CSUDI_UINT32	low; ///< 低32位
	CSUDI_INT32	    high;///< 高32位
}CSUDI_INT64;

/**@brief 64位无符号数结构体*/
typedef	struct
{
	CSUDI_UINT32	low;///< 低32位
	CSUDI_UINT32	high;///< 高32位
}CSUDI_UINT64;

#endif

#ifdef __cplusplus
}
#endif
/** @} */
#endif  //_UDI2_TYPEDEF_H_

