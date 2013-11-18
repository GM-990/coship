
#include  "mmcp_typedef.h"


#ifndef CS_ENVIRONMENT_H
#define CS_ENVIRONMENT_H

#ifdef __cplusplus
extern "C" {
#endif



/**@brief MMCP环境变量*/



/**@brief 智能卡/CA提供商名称*/
#define CSENV_SMARTCARD_PROVIDER                    ("SmartCard.provider")

/**@brief 智能卡卡号*/
#define CSENV_SMARTCARD_SN                    ("SmartCard.serialNumber")

/**@brief 系统级芯片型号*/
#define CSENV_SOC_MODEL                                       ("SOC.model")

/**@brief 系统级芯片提供商*/
#define CSENV_SOC_PROVIDER                                 ("SOC.provider")

/**@brief 内存大小，单位MB*/
#define CSENV_RAM_SIZE                                          ("RAM.size")

/**@brief 内存工作频率，单位MHz*/
#define CSENV_RAM_FREQUENCY                               ("RAM.frequency")

/**@brief Flash大小，单位MB*/
#define CSENV_FLASH_SIZE                                       ("Flash.size")



/**@brief 机顶盒提供商*/
#define CSENV_STB_PROVIDER                                  ("STB.provider")

/**@brief 机顶盒类型
"DVB-C"  "OneWay" "DualWay"  "SD"  "HD"的组合。*/
#define CSENV_STB_TYPE                                          ("STB.type")

/**@brief 机顶盒品牌*/
#define CSENV_STB_BRAND                                        ("STB.brand")

/**@brief 机顶盒型号*/
#define CSENV_STB_MODEL                                        ("STB.model")

/**@brief 机顶盒序列号*/
#define CSENV_STB_SERIAL_NUMBER                         ("STB.serialNumber")

/**@brief 机顶盒MAC个数*/
#define CSENV_STB_MAC_COUNT                                  ("STB.macCount")

/**@brief 机顶盒MAC地址，默认为第一张，可以通过STB.macAddress_0, STB.macAddress_1等方式取第N张网卡地址*/
#define CSENV_STB_MAC_ADDRESS                                  ("STB.macAddress")

/**@brief 节目列表版本*/
#define CSENV_STB_PROGRAMS_VERSION                  ("STB.programsVersion")

/**@brief 系统路径，通常用来存放配置文件，
相当于windows的system32目录，该目录可写。*/
#define CSENV_STB_SYSTEM_PATH                             ("STB.systemPath")

/**@brief 内置路径，通常用来存放只读的配置
文件，这些文件不可修改。这些配置文件
出厂是内置在设备中，当STB.systemPath下的配置
文件不存在是，使用该目录下的同相对路径、
同名文件做为配置文件。*/
#define CSENV_STB_INNER_PATH                               ("STB.innerPath")

/**@brief 机顶盒设备类型，暂时留空字符串""*/
#define CSENV_STB_DEVICE_TYPE                              ("STB.deviceType")

/**@brief 机顶盒分辨率："SD"-标清；"HD"-高清*/
#define CSENV_STB_DEFINITION                                ("STB.definition")



/**@brief Loader名称*/
#define CSENV_LOADER_NAME                                    ("Loader.name")

/**@brief Loader提供商*/
#define CSENV_LOADER_PROVIDER                             ("Loader.provider")

/**@brief Loader版本*/
#define CSENV_LOADER_VERSION                               ("Loader.version")

/**@brief Loader系统大小，单位：KB*/
#define CSENV_LOADER_SIZE                                      ("Loader.size")



/**@brief 中间件版权所有者*/
#define CSENV_MW_COPYRIGHT                                 ("Middleware.copyright")

/**@brief 中间件名称*/
#define CSENV_MW_NAME                                           ("Middleware.name")

/**@brief 中间件版本*/
#define CSENV_MW_VERSION                                      ("Middleware.version")

/**@brief 中间件发布日期*/
#define CSENV_MW_RELEASE_DATE                            ("Middleware.releaseDate")

/**@brief 是否强制启动中间件内置的BaseApp，
"true"or"false"，未设置为"false"*/
#define CSENV_MW_START_BASEAPP_FORCIBLY         ("Middleware.startBaseAppForcibly")

/**@brief 应用管理系统所使用的机顶盒
类型编号（非常重要，如天威：NSP高清:5101；
老LOADER商用高清:5102；远教高清5191），
该编号有运营商分配，通常与机顶盒型号绑定
（如通过CA模块配置的厂商号、硬件号、模块号）*/
#define CSENV_MW_STB_TYPE_CODE                          ("Middleware.stbTypeCode")

/**@brief 应用管理器使用UDP包请求
前端应用部署系统中的应用的版本号时，
失败后重试的时间间隔。*/
#define CSENV_MW_CHECK_APPSVERSION_INTERVAL ("Middleware.checkAppsVersionInterval")

/**@brief 中间件针对的客户名*/
#define CSENV_MW_CUSTOMER                                   ("Middleware.customer")



/**@brief 软件版本号*/
#define CSENV_SW_VERSION                                       ("Software.version")

/**@brief 硬件信息*/
#define CSENV_HW_INFO                                             ("Hardware.info")

/**@brief 硬件版本号*/
#define CSENV_HW_VERSION                                       ("Hardware.version")


#define CSENV_HW_VIDEO_TYPE                                 ("Hardware.videoType")

/**@brief 传递给时移网页的时移频道数据（取消）*/
#define CSENV_TIMESHIFT_CHANNEL_INFO                  ("TimeshiftChannelInfo")


/**@brief 音量调节模式，是统一("UNIQUE")还是分散("MULTIPLE")*/
#define CSENV_SYS_VOLUME_MODE                             ("SYS.VolumeMode")

/**@brief 声道调节模式，是统一("UNIQUE")还是分散("MULTIPLE")*/
#define CSENV_SYS_AUDIO_TRACK_MODE                    ("SYS.AudioTrackMode")
	
/**是否支持安全校验,"true" 或"false", 如果没有配置，默认为"false"不支持安全校验*/
#define CSENV_MW_SECURITY_SUPPORT   ( "Middleware.securitySupport")

/**单向部署频点信息，如：deliver://522000000.6875.64?type=qam */
#define CSENV_MW_DEPLOY_DELIVER    ("Middleware.AutodeployDELIVER")

/**应用部署方式，"oc" 、 "ip"  或 "auto",不分大小写。如果没有配置，默认为"ip"双向部署*/
#define CSENV_MW_DEPLOY_TYPE    ("Middleware.stbDeployType")

/**机顶盒区域码*/
#define CSENV_MW_AREA_CODE  ("Middleware.AreaCode")

/**机顶盒所有群组信息*/
#define CSENV_MW_GROUP_ID  ("Middleware.GroupID")	

/**双向应用部署时是否使用http虚拟文件系统直接解压前端应用zip包*/
#define CSENV_DEPLOY_USE_HTTPFS ("Middleware.deployUseHttpFS")

/**单向部署是否可恢复,默认不可恢复*/
#define CSENV_DEPLOY_OC_REVERABLE ("Middleware.isOCReverable")


/**
@brief  环境变量初始化

@param[in] 无
@return   成功返回TRUE;否则返回FALSE
*/

BOOL CSENVInit(void);

/**
环境变量变化通知
@param szKey - 环境变量.
@param szValue - 变化后的值
@param pUserData 用户自定义参数(注册回调函数时传入的参数)
*/
typedef void (*CSOnEnvChanged_F)(const char* szKey,const char* szValue,void* pUserData);

/**
注册环境变量变化监听者
@param szKey - 需要监听的环境变量，(NULL表示监听所有环境变量变化)
@param fnCb - 监听者回调函数
@param pUserData 用户自定义参数(回调通知时回传给回调函数)
*/
BOOL CSENVAddListner(const char* szKey,CSOnEnvChanged_F fnCb,void* pUserData);

/**
移除监听者
@param fnCb - 监听者回调函数
*/
void CSENVRemoveListner(CSOnEnvChanged_F fnCb);

/**
移除监听者
@param szKey - 监听者监听的环境变量 [不允许为空]
@param fnCb - 监听者用于处理指定环境变量的回调函数[不允许为空]
*/
void CSENVRemoveKeyListner(const char* szKey, CSOnEnvChanged_F fnCb);

/*!
设置环境变量
@param szKey - 环境变量.不允许为NULL,否则返回为NULL
@param szValue - 要设置的值
@return : 设置成功返回TRUE;否则返回FALSE
*/
BOOL CSENVSetValue(const char *szKey,const char *szValue);

/*!
@param szKey - 要求获取的环境变量,不允许为NULL,否则返回NULL
@return : 环境变量的值.存在则返回环境变量的值，否则返回NULL
*/
const char * CSENVGetValue(const char * szKey);

/**
@brief 销毁环境变量模块

@param[in] 无
@return 成功返回TRUE;否则返回FALSE
*/

BOOL CSENVClearUp(void);


/**
@brief 打印环境变量的信息，供调试使用，请打开"env"的打印

@param[in] 无
*/
void CSENVPrintAllNode(void);

#ifdef __cplusplus
}
#endif

#endif


