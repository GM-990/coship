/**@defgroup COSHIPLIB COSHIPLIB 提供应用程序与coship规范Bootloader交互的接口。
@brief COSHIPLIB模块对应头文件<csbl_coshiplib.h>，定义了系统数据读取部分、数据交互部分的接口。
现对该模块进行如下说明：

- 系统数据读取部分提供应用程序读取coship规范BootLoader系统数据的能力。
- 数据交互部分提供应用程序与coship规范BootLoader的数据交互能力。如loadpt等。

@version 1.0.0 2011/3/11  
@{
*/
#ifndef _CSBL_COSHIPLIB_H_
#define _CSBL_COSHIPLIB_H_

#ifdef  __cplusplus
extern "C" {
#endif



/**@brief coship规范数据类型*/
typedef enum
{
	EM_CSBL_EXTEND, ///< 扩展类型,用于coship规范中外部命令方式的串口升级类型
	EM_CSBL_LOADER, ///< loader
	EM_CSBL_APP, ///< app
	EM_CSBL_DEBUGER, ///< loader bak
	EM_CSBL_FONT, ///< font
	EM_CSBL_PROGRAMPARAM, ///< programparam
	EM_CSBL_BOOTER, ///< booter
	EM_CSBL_SYSCONFIG, ///< sysconfig
	EM_CSBL_IFRAME, ///< iframe
	EM_CSBL_GUIRC, ///< guirc
	EM_CSBL_CA_OTP, ///< ca_otp
	EM_CSBL_SYSSETTING, ///< syssetting
	EM_CSBL_SYSINFO, ///< sysinfo
	EM_CSBL_EMAIL, ///< email
	EM_CSBL_SN, ///< sn
	EM_CSBL_CA_COSHIP, ///< ca_coship
	
	EM_CSBL_DATA_CNT
} CSBLDataType_E;

enum data_length__
{
	EM_CSBL_MANUFACTURE_DES_LEN = 7, 
	EM_CSBL_MACHINE_DES_LEN = 16, 
	EM_CSBL_VERSION_LEN = 12, 
	EM_CSBL_SN_LEN = 8, 
	EM_CSBL_STRUCTION_DES_LEN = 10, 
	EM_CSBL_MAX_SN_LENGTH = 0xF0
};


/**@brief LIB操作错误类型定义*/
typedef enum
{
	EM_CSBL_LIBERROR_SUCCESS, ///< 成功
	EM_CSBL_LIBERROR_FAILURE, ///< 失败
	EM_CSBL_LIBERROR_BAD_PARAMETER, ///< 参数错误
	EM_CSBL_LIBERROR_INVALID, ///< lib错误
	EM_CSBL_SYSINFO_ADDR_INVALID, ///< sysinfo表地址无效
	EM_CSBL_INITSYSINFO_FAILURE, ///< sysinfo初始化失败
	EM_CSBL_INITSYSCFG_FAILURE, ///< syscfg初始化失败
	
} CSBLCoshipLibErr_E;

/**@brief  天大app触发及页面升级类型定义(天大专用)*/
typedef enum
{
	EM_CSBL_TD_ENABLE = 0x0,        /* 天大app正常触发升级升级  */ 
	EM_CSBL_TD_APP_FORCE = 0x1,			/*天大app强制升级界面强制升级，手动升级:是*/
	EM_CSBL_TD_APP_FORCE_NIT = 0x2,		/*天大app强制升级界面强制升级，手动升级:否*/	
} CSBLTDOtaStatus_E;

/**@brief  江西规范app触发及页面升级类型定义*/
typedef enum
{
	EM_CSBL_JX_ENABLE = 0x0,        /* app正常触发升级升级  */ 
	EM_CSBL_JX_APP_FORCE = 0x1,			/*app强制升级界面强制升级，手动升级:是*/
	EM_CSBL_JX_APP_FORCE_NIT = 0x2,		/*app强制升级界面强制升级，手动升级:否*/	
} CSBLJXOtaStatus_E;

/**@brief Coship规范的系统信息*/
typedef struct
{
	unsigned char m_aManuDes[EM_CSBL_MANUFACTURE_DES_LEN];  ///< 厂商描述符
	unsigned char m_aMachineDes[EM_CSBL_MACHINE_DES_LEN];   ///< 机型描述符
	unsigned char m_aHwVer[EM_CSBL_VERSION_LEN]; ///< 硬件版本号
	unsigned char m_aSwVer[EM_CSBL_VERSION_LEN]; ///< 机顶盒升级软件版本号
	unsigned char m_aSn[EM_CSBL_SN_LEN]; ///< 信息表中的SN 序列号
	unsigned char m_aSnMask[EM_CSBL_SN_LEN]; ///< 信息表中的SN MASK
	unsigned char m_aSnEx[EM_CSBL_MAX_SN_LENGTH];    ///< 存放在SN数据类型对应的flash地址中的数据
	unsigned int m_dwManuCode; ///< 厂商代码
} CSBLCoshipDataInfo_S;

/**@brief Coship规范OTA的info部分信息*/
typedef struct
{
	unsigned short m_wPId; ///< PID
	unsigned char m_ucTableId; ///< TableID
	unsigned char m_ucTunerId; ///< Tuner索引
	unsigned char m_aReserve[32]; ///< 保留
} CSBLCoshipOtaInfo_S;


/**@brief Coship规范Usb升级的Data部分信息*/
typedef struct
{
	char m_acFilePathName[256]; ///< 升级文件挂载点后路径及文件名
	unsigned char m_aReserve[64]; ///< 保留
} CSBLCoshipUsbData_S;


/**@brief Coship规范Ip升级信息，应用传给loader的文件存储路径*/
typedef struct
{
	char		m_acUrl[256];	//升级文件全路径
	BYTE 	m_aReserve[64];
} CSBLCoshipIpData_S;


typedef enum
{
	EM_CSBL_GETIP_BY_STATIC= 0,
	EM_CSBL_GETIP_BY_DHCP,
	EM_CSBL_GETIP_BY_PPPOE
}CSBLGetIpType_E;


/**@brief Coship规范Ip升级，应用传给loader的静态ip等信息*/
typedef struct
{
	CSBLGetIpType_E m_acGetIpType;
	char m_acMacAddr[13];
	char m_acReserve[3];
	
	union {
		
		struct {
			char m_acIPAddr[16];
			char m_acDNS[16];
			char m_acDNSBak[16];
			char m_acNetMask[16];
			char m_acGateWay[16];
		}staticIpInfo;

		struct {
			char m_aAcount[40];
			char m_aPassword[40];
		}ppoeInfo;

		struct {
			char m_acReserve[80];
		}dhcpInfo;
		
	}IpTypeInfo;
} CSBLCoshipIpInfo_S;


#define BUFFSIZE (160)

typedef struct
{
	unsigned char  m_aManuDes[BUFFSIZE * 2];  ///< 厂商描述符
	unsigned char  m_aMachineDes[BUFFSIZE * 2];   ///< 机型描述符
	unsigned char  m_aHwVer[BUFFSIZE * 2]; ///< 硬件版本号
	unsigned char  m_aSwVer[BUFFSIZE * 2]; ///< 机顶盒升级软件版本号
	unsigned char  m_aSn[BUFFSIZE * 2]; ///< 序列号范围
	unsigned char  m_aIp[BUFFSIZE * 2]; ///< IP范围
	unsigned char  m_aRegion[BUFFSIZE * 2];    ///<区域码范围
	unsigned char  m_aMac[BUFFSIZE * 2];    ///<Mac范围
	unsigned char  m_aManuCode[BUFFSIZE * 2]; ///< 厂商代码	
	unsigned char  m_aFilePath[BUFFSIZE * 2];//升级文件路径
}CSBLXmlCfgInfo_S;


typedef struct
{
	char Name[BUFFSIZE];
	unsigned char Value[BUFFSIZE * 2];
}Item_S;

typedef struct
{

	DWORD dwItemCnt;
	Item_S XmlItem[];
	
}CSBLXmlItemInfo_S;



/**(仅仅部分项目支持)
@brief模拟eeprom需要先设置模拟的flash范围
@note:目前只支持在nor flash,  spi flash上面模拟eeprom
@note:不支持nand flash
@param[in] dwFlashAddr 需要模拟的flash空间的逻辑地址。
@param[in] dwFlashSize 需要模拟的flash范围长度(单位：BYTE)。
@param[in] dwEEPROMSize 模拟eeprom的大小(单位：BYTE)。
@return CSUDI_SUCCESS表示成功
*/
CSUDI_Error_Code CSBLVirEEPRomInit(DWORD dwFlashAddr, DWORD dwFlashSize, DWORD dwEEPROMSize);



/**
@brief 系统数据初始化

@return 返回CSBLCoshipSysErr_E中定义的错误
@note 
- 调用本接口前必须先调用接口
CSBLSetConfig(EM_CSBL_CONFIG_SYSINFO, xxxxxx, NULL);配置info位置
- 只有本接口调用并成功后,其他功能接口才可以正常使用
*/
CSBLCoshipLibErr_E CSBLCoshipSysInit(void);


/**
@brief 获取子版本号

@param[in] eDataType 数据类型。具体定义见CSBLDataType_E枚举。
@param[out] pdwVer 成功返回对应数据类型的子版本。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLGetSubVersion(CSBLDataType_E eDataType, PDWORD pdwVer);


/**
@brief 获取STB系统信息

@param[out] psDataInfo 成功返回STB系统信息。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLGetCoshipDataInfo(CSBLCoshipDataInfo_S *psDataInfo);



/**
@brief 保存OTA升级信息

@param[in] pOTAPara 频点部分的信息，见udi2_tuner.h中CSUDITunerSRCDeliver_U 的定义。
@param[in] pParaEx 扩展部分的信息，目前没有使用,可以传NULL。
@param[in] pOTAInfo info部分信息，见CSBLCoshipOtaInfo_S定义。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo);


/**
@brief 保存OTA升级信息---------适用于 天大规范,

@param[in] pOTAPara 频点部分的信息，见udi2_tuner.h中CSUDITunerSRCDeliver_U 的定义。
@param[in] pParaEx 扩展部分的信息，目前没有使用,可以传NULL。
@param[in] pOTAInfo info部分信息，见CSBLCoshipOtaInfo_S定义。
@param[in] eTDOtaStatus 升级类型，见CSBLTDOtaStatus_E定义。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLTDSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo,CSBLTDOtaStatus_E eTDOtaStatus);


/**
@brief 保存OTA升级信息	----适用于江西规范

@param[in] pOTAPara 频点部分的信息，见udi2_tuner.h中CSUDITunerSRCDeliver_U 的定义。
@param[in] pParaEx 扩展部分的信息，目前没有使用,可以传NULL。
@param[in] pOTAInfo info部分信息，见CSBLCoshipOtaInfo_S定义。
@param[in] eTDOtaStatus 升级类型，见CSBLTDOtaStatus_E定义。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLJXSetOTAUpgrade(unsigned char * pOTAPara, unsigned char * pParaEx, unsigned char * pOTAInfo,CSBLJXOtaStatus_E eJXOtaStatus);


/**
@brief 保存Usb升级信息

@param[in] pUsbPara usb升级Data部分的信息，见CSBLCoshipUsbData_S 的定义。
@param[in] pParaEx 扩展部分的信息，目前没有使用,可以传NULL。
@param[in] pUsbInfo info部分信息，目前没有使用,可以传NULL。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLSetUsbUpgrade(unsigned char * pUsbPara, unsigned char * pParaEx, unsigned char * pUsbInfo);


/**
@brief 保存V1 IPLOADER升级信息( 目前只有江苏使用)

@param[in] pstIPUpgradePara 升级数据类型，升级软件版本，以及升级文件路径,结构参见struct DL_IP_STRUCT
@param[in] pIPconf 存储IP地址，MAC地址，以及网关信息参见struct IP_Config
@param[in] nIPStrCount 升级文件个数
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLSetIpLoaderV1Upgrade(const char* pstIPUpgradePara, const char* pIPconf, int nIPStrCount);


/**
@brief 保存HTTP 升级信息

@param[in] pHttpPara Xml文件所在的路径，见CSBLCoshipIpData_S 的定义。
@param[in] pParaEx 扩展部分的信息，目前没有使用,可以传NULL。
@param[in] pHttpInfo info部分信息，见CSBLCoshipIpInfo_S定义。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLSetHTTPUpgrade(unsigned char * pHttpPara, unsigned char * pParaEx, unsigned char * pHttpInfo);


/**
@brief 解析、获取XML文件中的信息(HTTP升级)

@param[in] pXmlBuf 储存XML文件的buf
@param[in] dwXmlBufLen为pXmlBuf的长度
@param[in] psDataInfo用来存储从XML中获取的信息，具体可见其结构体
@param[in] psItemInfo暂未使用，传NULL即可。
@return 返回CSBLCoshipLibErr_E中定义的错误
*/
CSBLCoshipLibErr_E CSBLGetXmlCfgInfo(char *pXmlBuf, DWORD dwXmlBufLen, CSBLXmlCfgInfo_S *psDataInfo, CSBLXmlItemInfo_S *psItemInfo);

#ifdef  __cplusplus
}
#endif

#endif



