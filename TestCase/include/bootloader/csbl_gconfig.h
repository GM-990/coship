/**@defgroup CONFIG CONFIG 提供BootLoader的功能配置接口
@brief CONFIG模块对应头文件<csbl_gconfig.h>，定义了BootLoader中功能配置相关的配置接口。

@version 1.0.0 2011/3/10  
@version 1.0.1 2011/3/31   对应bl_ver:2.1.6.0
@version 1.0.2 2011/4/11   对应bl_ver:2.1.6.1
@version 1.0.3 2011/4/11   对应bl_ver:2.1.6.2
@version 1.0.4 2011/4/21   对应bl_ver:2.2.0.0
@version 1.0.5 2011/4/29   对应bl_ver:02.02.01.00

@{
*/
#ifndef _CSBL_GCONFIG_H_
#define _CSBL_GCONFIG_H_


#ifdef  __cplusplus
extern "C" {
#endif

//#include "udiplus_typedef.h"

/**@brief 界面语言种类*/
typedef enum
{
	EM_CSBL_UI_LANGUAGE_NONE,
	EM_CSBL_UI_LANGUAGE_CHS, ///< 中文简体
	EM_CSBL_UI_LANGUAGE_ENG, ///< 英文
	EM_CSBL_UI_LANGUAGE_GER, ///< 德文
	EM_CSBL_UI_LANGUAGE_POR, ///< 葡萄牙文
	EM_CSBL_UI_LANGUAGE_SPA, ///<西班牙语
	
	EM_CSBL_UI_LANGUAGE_CNT
} CSBLUILanguageType_E;

/**@brief boot启动界面类型*/
typedef enum
{
	EM_CSBL_UI_PROMPT_COSHIP,	//coship boot启动界面
	EM_CSBL_UI_PROMPT_UNITEND,   //Unitend boot启动界面
	EM_CSBL_UI_PROMPT_NDS,		//NDS boot启动界面
	EM_CSBL_NO_UI_PROMPT,		//没有启动界面(当开机显示图片、视频时可设置此项)

	EM_CSBL_UI_PROMPT_CNT
}CSBLUIPromptType_E;

/**@brief booter解压时校验的类型*/
typedef enum
{
	EM_CSBL_DECCHECK_NONE, ///< 不校验
	EM_CSBL_DECCHECK_OCRC = 0x01, ///< 解压后的原始文件校验
	EM_CSBL_DECCHECK_CCRC = 0x10, ///< 解压前的压缩文件校验校验
	EM_CSBL_DECCHECK_BOTH = 0x11 ///< 解压前解压后都校验
} CSBLDecompressCrcType_E;


/**@brief 双loader校验方式*/
typedef enum
{
	EM_CSBL_LOADRECOV_NONE, ///< 不校验
	EM_CSBL_LOADRECOV_EVER, ///< 每次启动都做双loader校验并恢复
	EM_CSBL_LOADRECOV_LOAD, ///< 只在启动loader时做双loader校验并恢复

	EM_CSBL_LOADRECOV_CNT
} CSBLLoadRecoverType_E;

/**@brief FLASH类型*/
typedef enum
{
	EM_CSBL_FLSTYPE_NONE,
	EM_CSBL_FLSTYPE_NOR_SPI,
	EM_CSBL_FLSTYPE_NOR_CFI,
	EM_CSBL_FLSTYPE_NAND,
	EM_CSBL_FLSTYPE_CNT
} CSBLFlashType_E;


/**@brief 存储数据的设备类型*/
typedef enum
{	
	EM_CSBL_STORAGE_RAM = -1, ///< ram
	EM_CSBL_STORAGE_FLASH = 0, ///< flash
	EM_CSBL_STORAGE_E2PROM = 1, ///< 模拟eeprom
	EM_CSBL_STORAGE_MAX = 2, ///错误,历史原因导致外部很多工具写死了值为2是错误的数据类型。
	EM_CSBL_STORAGE_REAL_E2PROM = 3  ///真实eeprom
} CSBLStorageType_E;

/**@brief 数据在存储设备中的位置信息*/
typedef struct 
{
	unsigned int dwAddr; ///< 设备中的偏移地址
	unsigned int dwSize; ///< 大小
	unsigned int dwStoreType; ///< 存储设备，见CSBLStorageType_E定义
} CSBLDataPos_S;

#define LOADEER_VERSION_MAX_LENGTH (40)  ///< loader软件版本号最大长度
#define M_CSBL_MAX_TUNER_CNT (4)  ///< 能支持的最多的tuner个数

/**@brief 校验解密的方式*/
typedef enum
{
	EM_CSBL_CHECK_DECRYPT_TYPE_NONE, ///< none
	EM_CSBL_CHECK_DECRYPT_TYPE_UNKNOWN, ///< 未知
	EM_CSBL_CHECK_DECRYPT_TYPE_CUSTOM, ///< 外部设置
	EM_CSBL_CHECK_DECRYPT_TYPE_COSHIP, ///< coship规范要求
	EM_CSBL_CHECK_DECRYPT_TYPE_CONAX, ///< conax规范要求
	EM_CSBL_CHECK_DECRYPT_TYPE_VERIMATRIX, ///< verimatrix规范要求
	EM_CSBL_CHECK_DECRYPT_TYPE_VIACCESS, ///< viaccess规范要求
	EM_CSBL_CHECK_DECRYPT_TYPE_NAGRA, ///< nagra规范要求
	
	EM_CSBL_CHECK_DECRYPT_TYPE_CNT
} CSBLCheckDecryptType_E;

/**@brief 校验解密的相关信息*/
typedef struct
{
	unsigned int m_dwHeadLength; ///< 校验解密描述头的长度
	unsigned int m_dwSignatureLength; ///< 签字部分的长度
	unsigned int m_dwDataLength; ///< 数据部分的总长度，head、signature以及Data三长度之和为总的文件长度
	unsigned int m_dwSignDataLength; ///< Data中签字部分长度
	unsigned int m_dwEncryptDataLength; ///< Data中加密部分长度
	CSBLCheckDecryptType_E m_eCheckDecryptType; ///< 校验解密的方式，见CSBLCheckDecryptType_E定义	
} CSBLCheckDecryptInfo_S;


/**@brief 设置CA规范*/
typedef enum
{
	EM_CSBL_CA_NONE,////
	EM_CSBL_CA_TF,    ////tf同方规范 
	EM_CSBL_CA_IRDETO, ////irdeto规范 
	EM_CSBL_CA_NDS,   ////nds规范  
	EM_CSBL_CA_CONAX, ////conax 规范
	EM_CSBL_CA_VERIMATRIX, ////verimatrix 规范	
	EM_CSBL_CA_NAGRA, ////nagra 规范
	EM_CSBL_CA_TIANJIN, ////天津广电规范
	EM_CSBL_CA_HENAN,  ////河南省规范
	EM_CSBL_CA_UNITEND,	/////数字太和
	EM_CSBL_CA_GDSW,	////广东省网(南方传媒规范)
	EM_CSBL_CA_TIANDA,
	EM_CSBL_CA_JIANGXI,
	EM_CSBL_CA_LATENS,////Latens规范
	EM_CSBL_CA_DEZHOU, ///德州N8001C项目，双CA
	
	EM_CSBL_CA_CNT
} CSBLCAType_E;

/**@brief 设置芯片类型*/
typedef enum
{
	EM_CSBL_CHIP_DEFAULT,	
	EM_CSBL_CHIP_ALI3603,
	EM_CSBL_CHIP_ALI3281,
	EM_CSBL_CHIP_ALI3701,
	EM_CSBL_CHIP_CNT
} CSBLCHIPType_E;

/**@brief 设置数字太和相关地址*/
typedef struct 
{
	unsigned int dwUnitendConAddr; ///数字太和config表地址
	unsigned int dwUnitendConBakAddr; ///数字太和config表的备份地址
	unsigned int dwUnitendSigAddr; ///存放数字太和签字的地址
	unsigned int dwUnitendCertAddr; ///存放数字太和证书的地址
} CSBLUnitendConf_S;


/**@brief 向BootLoader中设置的回调函数定义

@param[in] Param1 参数一，根据CSBLSetConfig中设置的配置类型不同而不同，见CSBLConfig_E的定义中说明
@param[in] Param2 参数二，根据CSBLSetConfig中设置的配置类型不同而不同，见CSBLConfig_E的定义中说明
@return 成功返回SUCCESS，失败返回FAILURE
*/
typedef int (*CSBLCustomCallBack_F)(PVOID Param1, PVOID Param2);



/**@brief 向BootLoader中设置的回调函数定义

@return 成功返回SUCCESS，失败返回FAILURE
*/
typedef int (*CSBLCustomCallBack_F_P4)(PVOID Param1, PVOID Param2,PVOID Param3, PVOID Param4);



/**@brief CSBLSetConfig接口可以配置的类型*/
typedef enum
{
	EM_CSBL_CONFIG_NONE = 0,
	EM_CSBL_CONFIG_UI_XOFFSET, ///< BootLoader界面X轴方向偏移，Param1为偏移值，Param2无效
	EM_CSBL_CONFIG_UI_YOFFSET, ///< BootLoader界面Y轴方向偏移，Param1为偏移值，Param2无效
	EM_CSBL_CONFIG_UI_LANGUAGE, ///< BootLoader界面语种选择，Param1为语种，见CSBLUILanguageType_E定义，Param2无效
	EM_CSBL_CONFIG_FPDISPLAY_NEEDSCAN, ///< Booter中前面板显示时是否需要scan，Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_DEC_FPDISPLAY, ///< Booter解压时前面板是否显示进度，Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_DEC_CRCCHECK, ///< Booter解压前后crc check类型选择，Param1见CSBLDecompressCrcType_E定义，Param2无效
	EM_CSBL_CONFIG_LOADRECOVER, ///< 程序加载时双loader备份机制的选择，Param1见CSBLLoadRecoverType_E，Param2无效
	EM_CSBL_CONFIG_SYSINFO, ///< Coship规范设置sysinfo表地址，Param1为sysinfo表地址，Param2无效
	EM_CSBL_CONFIG_IRDETO_BOOTINFO, ///< Irdeto规范设置Booter信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_LOADINFO, ///< Irdeto规范设置Loader信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_BBCBINFO, ///< Irdeto规范设置BBCB信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_KEYINFO, ///< Irdeto规范设置Key信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_LOADPTINFO, ///< Irdeto规范设置loadpt信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_RESETINFO, ///< Irdeto规范设置reset信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_CPCBINFO, ///< Irdeto规范设置CPCB信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_APPINFO, ///< Irdeto规范设置App信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_1STPARTITIONINFO, ///< Irdeto规范设置First partition信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_PARTITIONINFO, ///< Irdeto规范设置Partition信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_IRDETO_BOOTUARTUPG, ///< Irdeto规范Booter中是否有串口升级功能，Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_WRITEFS_NEEDERASE, ///< 写Flash的等相关数据时是否需要先擦除，Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_VERSIONSTRING, ///< 外部设置版本号，Param1为char指针，Param2为char指针
	EM_CSBL_CONFIG_CHECKDECRYPT_BOOT, ///注册解压函数,booter引导app的时候使用,CONAX(RSA校验app,AES解密app),NAGRA校验启动数据
	EM_CSBL_CONFIG_CHECKDECRYPT_LOAD, ///< 设置下载时使用的解签字和解密方式。Param1为外部设置的校验解密的函数，见CSBLCustomCallBack_F定义。
	EM_CSBL_CONFIG_COSHIPLOAD_CUSTOMWRITEFIRST, ///< Coship loader下载完数据并完成校验后，将某某一数据类型的数据通过外部回调函数处理。Param1为数据类型，见CSBLDataType_E定义，Param2为回调函数，见CSBLCustomCallBack_F定义。回调函数的Param1为数据buf，Param2为数据长度
	EM_CSBL_CONFIG_LOAD_VERIFY_SWVER, ///< 设置Coship loader正常下载过程中，是否进行软件版本号校验。Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_UPGRADE_QAM, ///< 设置是否增加Qam升级方式。Param1为外部设置函数，用于从界面获取Qam升级参数，见CSBLCustomCallBack_F定义。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行Qam升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_UPGRADE_COFDM, ///< 设置是否增加Qam升级方式。Param1为外部设置函数，用于从界面获取Cofdm升级参数，见CSBLCustomCallBack_F定义。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行Cofdm升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_UPGRADE_QPSK, ///< 设置是否增加Qam升级方式。Param1为外部设置函数，用于从界面获取Qpsk升级参数，见CSBLCustomCallBack_F定义。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行Qpsk升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_UPGRADE_USB, ///< 设置是否增加usb升级方式。Param1为外部设置函数，用于从界面获取usb升级参数，见CSBLCustomCallBack_F定义，为NULL则无该功能。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行USB升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_UPGRADE_IP_OLD, ///< 设置是否增加旧版ip升级方式。Param1为外部设置函数，用于从界面获取ip升级参数，见CSBLCustomCallBack_F定义，为NULL则无该功能。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行ip升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_UPGRADE_IP, ///< 设置是否增加ip升级方式。Param1为外部设置函数，用于从界面获取ip升级参数，见CSBLCustomCallBack_F定义，为NULL则无该功能。回调函数的Param1和Param2在调用时均传递NULL。Param2为外部设置的函数，用于进行ip升级，见CSBLCustomCallBack_F定义，函数的Param1和Param2在调用时均传递NULL。
	EM_CSBL_CONFIG_CA_TYPE, ////设置CA规范，Param1为CSBLCAType_E定义, Param2注册OTASTART处理函数参考Henan_OTA_Start
	EM_CSBL_CONFIG_OSG_NEEDBLIT, ///< OSG显示时是否需要blit，Param1见BOOL定义，Param2无效
	EM_CSBL_CONFIG_CACHE_PROCESS,///<设置是否需要开关CACHE处理，目前CACHE开关只在FLASH读写时有使用，其中PARAM1为CSBLCustomCallBack_F定义为开CACHE函数，PARAM2也为CSBLCustomCallBack_F定义为关CACHE函数
	EM_CSBL_CONFIG_IRDETO_SYSSETTINGINFO,///Irdeto规范设置syssetting信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_SHOW_PROGRESSBAR,////设置Booter启动时是否显示进度条，Param1为TRUE时显示，FALSE不显示。Param2无效
	EM_CSBL_CONFIG_SET_MASK, //设置掩码，Param1为设置的掩码，param2无效.若没有设置，默认掩码为0xffffffff(位与前后，不会又影响),若有需要可设置。目前在按键回调时使用(blgui.c:InputCallBack).
	EM_CSBL_CONFIG_IRDETO_CFG_APPINFO, ///< Irdeto规范设置cfg里面 App信息，Param1为CSBLDataPos_S指针，Param2无效
	EM_CSBL_CONFIG_UDI2_SECTION_REQUEST, ///<设置OTA下载数据的时候，过滤section数据包时候直接使用UDI2_section.h接口, 不使用DTV库，Param1为无效，Param2无效
	EM_CSBL_CONFIG_SYSDATA_CHECK,//设置校验sysdata及sysdatabak; PARAM1为sysdata的地址,PARAM2为sysdatabak的地址(逻辑地址);若未设置,或者其中一个地址为0,则不做校验.
	EM_CSBL_CONFIG_INFLATE_FUN,//设置解压过程中使用新的解压函数,通过外部注册进来的函数 PARAM1为注册的函数地址.见CSBLCustomCallBack_F定义。  PARAM2无效
	EM_CSBL_CONFIG_BOOT_IR_FORCE_UPGRADE,////设置是否注册遥控器强制进入loader升级，Param1为遥控器检测函数，Param2为NULL。	
	EM_CSBL_CONFIG_UNITEND_CONFIG,//设置数字太和相关的地址信息，Param1为CSBLUnitendConf_S指针,Param2无效
	EM_CSBL_CONFIG_UI_PROMPT_TYPE,//设置booter启动界面，Param1为要设置的启动界面类型，Param2为NULL。若未设置，默认使用Coship booter启动界面。
	EM_CSBL_CONFIG_FRONTPANEL_INFO,//设置机顶盒是否有前面板，Param1若为TRUE，代表有前面板，FALSE即没有前面板；Param2为NULL
	EM_CSBL_CONFIG_UPDATE_BLPARAM_HMAC,//设置机顶盒配置表(sysinfo, sysconfig, loaderpt等更新后)，Hmac 是否重新写入，Param1 为更新的配置的地址，Param2 为NULL
	EM_CSBL_CONFIG_FLS_TYPE,//用来配置FLASH类型，目前用于控制是按BLOCK擦除，还是整片擦除，Param1类型为CSBLSetConfigErr_E，Param2为NULL
	EM_CSBL_CONFIG_DVB_T2_INFO,//设置地面机tuner类型为T2，参数无意义
	EM_CSBL_CONFIG_AES_FUNC,//设置AES算法，Param1若为AES新注册函数，默认为NULL；Param2为NULL
	EM_CSBL_CONFIG_SET_AES_RSA_KEY,//设置获取AES、RSA算法使用的key和mod；Param1为获取AES key的接口，Param2为获取RSA算法使用的key和mod的接口
	EM_CSBL_CONFIG_ZSL_WRITE_FUNC, //设置同洲规范OTA/USB写flash数据函数，Param1为新注册函数，Param2为NULL
	EM_CSBL_CONFIG_OTA_LED_FLICKER_FUNC, //设置同洲规范OTA下载,写函数时候前面板信号灯闪烁的函数，Param1为新注册函数，Param2为NULL
	EM_CSBL_CONFIG_IRDETO_MAC_FUNC, //设置irdeto规范,芯片平台计算MAC的函数，Param1为新注册函数，Param2为NULL
	EM_CSBL_CONFIG_MSTAR_USB_FUNC,//设置Mstar平台下USB升级时触发挂载U盘的初始化函数,Param1为新注册函数,Param2暂为NULL
	EM_CSBL_CONFIG_GET_RSA_KEY, //设置conax规范获取RSAKEY所使用的回调函数，Param1为新注册函数,Param2暂为NULL
	EM_CSBL_CONFIG_HISIOTA_WAIT_FUNC,//HeFei项目需要将锁频成功后下载数据的等待时间从5分钟修改为2分钟,通过此外部注册方式处理实现
	EM_CSBL_CONFIG_CHIP_TYPE,//设置对BL 有特殊要求的芯片的类型,Param1为芯片类型，具体类型见CSBLCHIPType_E
	EM_CSBL_CONFIG_USBUPGRADE_CMPSW,//设置左键进入usb升级是否需要比较软件版本号
	EM_CSBL_CONFIG_CNT
} CSBLConfig_E;

/**@brief CSBLSetConfig接口操作结果返回*/
typedef enum
{
	EM_CSBL_SETCONFIG_SUCCESS, ///< 成功
	EM_CSBL_SETCONFIG_FAILURE, ///< 失败
	EM_CSBL_SETCONFIG_BADPARAM ///< 参数错误
	
} CSBLSetConfigErr_E;

/**
@brief 设置BootLoader配置

@param[in] eCfg 配置类型，见CSBLConfig_E定义
@param[in] Param1 参数一，具体意义根据eCfg不同而变化，参考CSBLConfig_E定义中各个说明
@param[in] Param2 参数二，具体意义根据eCfg不同而变化，参考CSBLConfig_E定义中各个说明
@return 返回CSBLSetConfigErr_E中定义的错误
*/
CSBLSetConfigErr_E CSBLSetConfig(CSBLConfig_E eCfg, PVOID Param1, PVOID Param2);


#ifdef  __cplusplus
}
#endif

#endif



