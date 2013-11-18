/****************************************************************************************************************************
* Notice:      Copyright Shenzhen Coship Electronics Co., Ltd., 
*              All rights reserved.
* Filename:    CS_IRDETO_Loader.h  
* Author:      wansuanlin
* Date:        Match 2006 
* Description: this file provide loader function
*            
*****************************************************************************************************************************/
#include "udiplus_typedef.h"

/****************************************************************************************************************************
* 			                                           Revision History                                                     *
*****************************************************************************************************************************
*   Revision  : V1.0
*   Programmer: Wan suanlin
*   Create    : 2006-03-17
*
*   V2.0         bl              2010-11-11
*
*****************************************************************************************************************************/
#ifndef _CS_IRDETO_LOADER_H_
#define _CS_IRDETO_LOADER_H_

#ifdef  __cplusplus
extern "C" {
#endif


#define CA_MAX_SIZE_REV                     (32) //增加的
typedef struct _CpcbInfo
{
    BYTE      m_cLoadNum;  /*当前下载号*/
    DWORD     m_dwBin2irdVer;
    DWORD     m_dwBin2irdSerialNum;
    BYTE      m_acSignTimeDate[5];
    BYTE      m_acDownloadTimeDate[5];
    WORD      m_wDownloadDuration;
    DWORD     m_dwImageSize;
}T_CpcbInfo;

typedef struct _BbcbInfo
{
	WORD      m_wCRC;
	BYTE      m_cManufactureCode;  /*厂商代号*/                  
	BYTE      m_cHW_Version;       /*硬件版本号*/
	BYTE      m_cVerMajor;
	BYTE      m_cVerMinor;              
	BYTE      m_acSerialNumber[4];  
	BYTE      m_acRandomNumber[4];   
	BYTE      m_acRev[9];
}T_BbcbInfo; 

typedef struct _KeyStatusEx
{
    BYTE      m_cKeyStatus;
    BYTE      m_cLastImage;
    WORD      m_wSysID;
    WORD      m_wKeyVer;
    WORD      m_wVariant; /*市场号*/
    WORD      m_wSigVer;
    DWORD     m_dwRandomNum;
}T_KeyStatusEx;

typedef struct _LoaderIrdInfo
{
    WORD      m_wManufacturerCode;  /*厂商代号*/
    WORD      m_wHardwareCode;      /*硬件版本号*/
    WORD      m_wVariant;           /*市场号，由Irdeto分配*/
    WORD      m_wLoadNum;           /*当前下载号*/
}T_LoaderIrdInfo;

typedef struct
{
   WORD u16SysID;               /* System ID                                 */
   WORD s16SysIDValid;          /* 0=u16SysID valid, -1=u16SysID invalid     */
   WORD u16KeyVer;              /* Key version                               */
   WORD s16KeyVerValid;         /* 0=u16KeyVer valid, -1=u16KeyVer invalid   */
   WORD u16SigVer;              /* Signature version                         */
   WORD s16SigVerValid;         /* 0=u16SigVer valid, -1=u16SigVer invalid   */
   WORD u16Variant;             /* Operator variant                          */
   WORD s16VariantValid;        /* 0=u16Variant valid, -1=u16Variant invalid */
   BYTE   u8BlockFailMask;      /* Bit mask of failed Irdeto NVRAM partition */
                                /* blocks (block0 = bit0)                    */
} IHL_STATUS, *IHL_PSTATUS;

typedef enum _CaFtaType
{
	E_CA_FTA_SAT   = 0x43,         //卫星,
	E_CA_FTA_CABLE = 0x44,      // 有线
	E_CA_FTA_TER   = 0x5A,        // 地面
	E_CA_FTA_T2    = 0x7F,	   //DVB-T2
	E_CA_FTA_MAX
}T_CaFtaType;


//IRDStatus
#define   CA_MAX_SIZE_IRD_SERIALNUMBER      (32)
#define	CA_MAX_SIZE_SC_STATUS               (32)
#define	CA_MAX_SIZE_IRD_VER                 (64)
#define	CA_MAX_SIZE_FIRMWARE                (64)
#define	CA_MAX_SIZE_CAM_VER                 (64)
#define	CA_MAX_SIZE_SC_VER                  (32)
#define	CA_MAX_SIZE_LOASER                  (64)
#define	CA_MAX_SIZE_SW_VER                  (64)

typedef struct _CaIrdStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szRandomNumber[CA_MAX_SIZE_IRD_SERIALNUMBER]; //Random NO.
	BYTE	m_szScStatus[CA_MAX_SIZE_SC_STATUS];
	BYTE	m_szIrdVer[CA_MAX_SIZE_IRD_VER];
	BYTE	m_szFirmware[CA_MAX_SIZE_FIRMWARE];
	BYTE	m_szCamVer[CA_MAX_SIZE_CAM_VER];
	BYTE	m_szScVer[CA_MAX_SIZE_SC_VER];			//no used
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE    m_szRev[CA_MAX_SIZE_REV];             //增加的
}T_CaIrdStatus;

//LoaderStatus
#define	CA_MAX_SIZE_MANUFACTURE_ID          (8)
#define	CA_MAX_SIZE_HW_VER                  (8)
#define	CA_MAX_SIZE_LD_SEQNO                (8)
#define	CA_MAX_SIZE_KEY_SEQNO               (8)
#define	CA_MAX_SIZE_VARIANT                 (8)
#define	CA_MAX_SIZE_KEY_SYSID               (8)
#define	CA_MAX_SIZE_KEYSTATUS               (8)
#define	CA_MAX_SIZE_KEY_VER                 (8)
#define	CA_MAX_SIZE_BIN2IRD_VER             (16)
#define	CA_MAX_SIZE_BIN2IRD_SER             (16)
#define	CA_MAX_SIZE_SIGN_DATE               (16)
#define	CA_MAX_SIZE_DL_DATE                 (16)
#define	CA_MAX_SIZE_SIGN_TIME               (16)
#define	CA_MAX_SIZE_DL_TIME                 (16)
#define	CA_MAX_SIZE_DL_DUR                  (16)
#define	CA_MAX_SIZE_DL_IMAGE                (16)

typedef struct _CaLoaderStatus
{
	BYTE	m_szSerialNumber[CA_MAX_SIZE_IRD_SERIALNUMBER];
	BYTE	m_szRandomNumber[CA_MAX_SIZE_IRD_SERIALNUMBER]; //Random NO.
	BYTE	m_szLoader[CA_MAX_SIZE_LOASER];
	BYTE	m_szManuID[CA_MAX_SIZE_MANUFACTURE_ID];
	BYTE	m_szHWVer[CA_MAX_SIZE_HW_VER];
	BYTE	m_szLDSeqNo[CA_MAX_SIZE_LD_SEQNO];
	BYTE	m_szKEYSeqNo[CA_MAX_SIZE_KEY_SEQNO];
	BYTE	m_szVARIANT[CA_MAX_SIZE_VARIANT];
	BYTE	m_szKEYSysId[CA_MAX_SIZE_KEY_SYSID];
	BYTE	m_szKEYStatus[CA_MAX_SIZE_KEYSTATUS];
	BYTE	m_szKEYVer[CA_MAX_SIZE_KEY_VER];
	BYTE	m_szBIN2IRDVer[CA_MAX_SIZE_BIN2IRD_VER];
	BYTE	m_szBIN2IRDSer[CA_MAX_SIZE_BIN2IRD_SER];
	BYTE	m_szSignDate[CA_MAX_SIZE_SIGN_DATE];
	BYTE	m_szDLDate[CA_MAX_SIZE_DL_DATE];
	BYTE	m_szSignTime[CA_MAX_SIZE_SIGN_TIME];
	BYTE	m_szDLTime[CA_MAX_SIZE_DL_TIME];
	BYTE	m_szDLDur[CA_MAX_SIZE_DL_DUR];
	BYTE	m_szImageSize[CA_MAX_SIZE_DL_IMAGE];
	BYTE       m_szRev[CA_MAX_SIZE_REV];             //增加的
}T_CaLoaderStatus;


typedef struct _QpskSwitch
{
	BYTE	m_cSwitch0_12V;     //SWITCH_0V | SWITCH_12V
	BYTE	m_c22K;             //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBPower;        //SWITCH_ON | SWITCH_OFF
	BYTE	m_cLNBType;         //SINGLE | UNIVERSAL
	DWORD	m_dwLNBLowFreq;     //low LNB frequency in Universal Mod or LNB frequency in Single Mod [unit:KHz]
	DWORD	m_dwLNBMidFreq;     //middle LNB frequency in Universal Mod  [unit:KHz]
	DWORD	m_dwLNBHighFreq;    //high LNB frequency in Universal Mod   [unit:KHz]
	BYTE	m_cDiSEqC10;        //DiSEqC1.0开关	( PORT_A | PORT_B | PORT_C | PORT_D )
	BYTE	m_cDiSEqC11;        //DiSEqC1.0 ( 0-16 and 0 for not used )
	BYTE	m_cToneBurst;       //Tone Burst开关 ( TONEBURST_NONE / TONEBURST_A / TONEBURST_B )
	BYTE	m_cDiSEqCRepeat;    //重复次数 ( NoRepeat / OnceRepeat / TwiceRepeat )
	DWORD	m_dwFreqencyOffset; //channel frequency offset in KHz
	BYTE	m_cIQ;              //0-Normal; 1-Invers
	BYTE 	m_cPilot;
	BYTE	m_cOption;        //m_dwOption:2;
	BYTE	m_cModulation; //m_dwModulation:2;1:qpsk,2:8psk
	BYTE  	m_cStandard;    //m_dwstandard:1; 0:s, 1:s2
	BYTE       m_cRev[3];
}T_QpskSwitch;

typedef struct _SatelliteSystem
{
	DWORD      m_dwFrequency;///< 频率，单位KHz
	WORD        m_wPrbitalPosition;
	BYTE          m_cWestEastFlag;
	BYTE          m_cPolarization;///驱动信号极化方向0:水平极化, 1:垂直极化, 2:左旋极化,3:右旋极化
	BYTE          m_cModulation;///	0: QPSK和8PSK自动识别方式,1 : QPSK调制方式,2 : 8PSK调制方式
	DWORD      m_dwSymbolRate;///< 符号率，单位Kbps
	BYTE       m_cFecInner;///驱动中的FEC的类型,	0: FEC方式自动选择 ,1: 1/2方式, 2: 2/3方式, 3: 3/4方式, 4: 5/6方式,5:7/8方式,6: 8/9方式的
	T_QpskSwitch  m_stQpskSwitch;
}T_SatelliteSystem;

typedef struct _TerrestrialSystem
{
	DWORD   m_dwCentreFrequency;
	BYTE       m_cBandWidth;
	BYTE       m_cConstellation;
	BYTE       m_cHierarchyInfo;
	BYTE       m_cCodeRateHpStream;
	BYTE       m_cCodeRateLpStream;
	BYTE       m_cGuardInterval;
	BYTE       m_cTransmissionMode;
	BYTE       m_cOtherFrequencyFlag;	
}T_TerrestrialSystem;

typedef struct _T2System
{
	DWORD   m_dwCentreFrequency;
	BYTE       m_cPlpID;
	BYTE       m_cT2SystemID;
	BYTE       m_cSISO_MISO;
	BYTE       m_cBandWidth;
	BYTE       m_cGuardInterval;
	BYTE       m_cTransmissionMode;
	BYTE       m_cOtherFrequencyFlag;
	BYTE       m_cTfsFlag;
	DWORD      m_cCellID;	
}T_T2System;

//只需要m_dwFrequency,     m_cModulation,   m_dwSymbolRate   三个
typedef struct _CableSystem
{
	DWORD      m_dwFrequency;          /* 频率(unit:KHz),频率范围(48-866)*/
	BYTE          m_cFecOuter;
	BYTE          m_cModulation;       /**< 0 Not Define  QAM星座 ( 0:Not Define   1:16QAM	    2:32QAM	   3:64QAM	  4:128QAM	 5:256QAM)*/
	DWORD      m_dwSymbolRate;       /* 符号率(unit:KHz)       */
	BYTE       m_cFecInner;
}T_CableSystem;


typedef union _DeliverySystem 
{
   	T_CableSystem              m_stCableStytle;        //有线
	T_SatelliteSystem           m_stSatelliteStytle;      //卫星
	T_TerrestrialSystem     m_stTerrestrialStytle;   //地面
	T_T2System                    m_stT2Stytle;                //DVB-T2
}T_DeliverySystem;


typedef struct _OtaCfg     // OTA升级参数
{
	WORD    m_wOtaPid;         //PID
	BYTE    m_cOtaTableId;    // table ID
	INT     m_nOtaTime;      //tunnerId
	
	T_CaFtaType         m_eType;         //tuner类型: 卫星,地面,有线
	T_DeliverySystem    m_stDlySys;  //频点,符号率, 调整方式......
}T_OtaCfg;




//库返回值错误代码,  int 类型 
////----start-----
#define ERROR_CS_IRDETOCA_BL_SUCCESS        0          //成功
#define ERROR_CS_IRDETOCA_BL_FAILURE        10         //其他失败 



//模拟EEPROM错误 
#define ERROR_CS_IRDETOCA_BL_NVM        	    20		//模拟eeprom  初始化错误
#define ERROR_CS_IRDETOCA_BL_NVM_BIG       	21		//模拟eeprom  数据块大于0x2000
#define ERROR_CS_IRDETOCA_BL_NVM_UER_BIG    22     //模拟eeprom  数据块大于  eeprom 占用的flash块
#define ERROR_CS_IRDETOCA_BL_NVM_ADDR_ZERO   23     //dwVEEPROMSize =0错误. 或者  dwVEEPROMUserSize=0错误
#define ERROR_CS_IRDETOCA_BL_NVM_INIT_REPEAT  24   //模拟EEPROM 重复初始化.
#define ERROR_CS_IRDETOCA_BL_NVM_NO_INIT     25   //模拟EEPROM 没有初始化

#define ERROR_CS_IRDETOCA_BL_KEY                  30    //Irdeto KEY   错误 ,  没有升级 KEY，或者key是错误的
//CPCB错误
#define ERROR_CS_IRDETOCA_BL_CPCB_PART            40  //读取 CPCB[1] 比app实际占用的扇区空间大
#define ERROR_CS_IRDETOCA_BL_CPCB_SIZE          41  //cpcb大小错误
#define ERROR_CS_IRDETOCA_BL_CPCB_CRC          42  //cpcb校验失败

//函数未调用错误
#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetFlashEntireSize  50  //BLSetFlashEntireSize函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetBooterSize       51  //BLSetBooterSize函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetLoaderSize       52  //BLSetLoaderSize函数未调用 

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetBBCBSize       53  //BLSetBBCBSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetCPCBSize       54  //BLSetCPCBSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetVEEPROMSize       55  //BLSetVEEPROMSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetVEEPROMUserSize       56 //BLSetVEEPROMUserSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetAppSize       57 //BLSetAppSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetFirstPartitionSize      58 //BLSetFirstPartitionSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetPartitionSize     59 //BLSetPartitionSize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetIrdetoKeySize    60 //BLSetIrdetoKeySize 函数未调用

#define ERROR_CS_IRDETOCA_BL_BL_No_BLSetLoaderptSize    61 //BLSetLoaderptSize 函数未调用
///


#define ERROR_CS_IRDETOCA_BL_FLASH_READ         70   //读flash失败.
#define ERROR_CS_IRDETOCA_BL_FLASH_WRITE        71   //写flash失败.
#define ERROR_CS_IRDETOCA_BL_FLASH_ERASE        72   //擦除flash 失败
///---end----













/**********************************************************

1. App 必须先设置各种数据的地址,大小等基本信息,按照项目flash map填写 
2. App 必须初始化库
3. App 写OTA参数,OTA标志

**********************************************************/


//(1)--app设置地址开始--------------------------------------------------------------------------

/**********************************************************
功能：设置flash的基地址和大小
输入：flash的基地址和大小
说明：此接口由应用程序来调用, 
**********************************************************/
void BLSetFlashEntireSize(DWORD dwFlashBaseAddr, DWORD dwFlashSize);

/**********************************************************
功能：设置Booter的起始地址和大小
输入：Booter的大小和起始地址
说明：此接口由应用程序来调用
		  必须存储在flash里面
**********************************************************/
void BLSetBooterSize(DWORD dwBooterAddr, DWORD dwBooterSize);

/**********************************************************
功能：设置Loader的地址和大小
输入：Loader的地址和大小
*说明：此接口由应用程序来调用
		必须存储在flash里面
**********************************************************/
void BLSetLoaderSize(DWORD dwLoaderAddr, DWORD dwLoaderSize);

/**********************************************************
*功能：设置BBCB的地址和大小
*输入：BBCB的地址和大小
*说明：此接口由应用程序来调用
	必须存储在flash里面
**********************************************************/
void BLSetBBCBSize(DWORD dwBBCBAddr, DWORD dwBBCBSize);

/**********************************************************
*功能：设置CPCB的地址和大小
*输入：CPCB的地址和大小
*说明：此接口由应用程序来调用
	必须存储在flash里面
**********************************************************/
void BLSetCPCBSize(DWORD dwCPCBAddr, DWORD dwCPCBSize);

/**********************************************************
*功能：设置模拟EEPROM的地址和大小,模拟eeprom的块大小,如果
			项目没有模拟eeprom,则不调用本函数。 

*输入：模拟EEPROM的地址和大小
*说明：此接口由应用程序来调用

dwVEEPROMAddr:模拟eeprom在flash中的起始地址
dwVEEPROMSize:模拟eeprom在flash中的长度
dwVEEPROMUserSize:模拟eeprom本身的长度
**********************************************************/
void BLSetVEEPROMSize(DWORD dwVEEPROMAddr, DWORD dwVEEPROMSize,DWORD dwVEEPROMUserSize);

/**********************************************************
*功能：设置App的地址和大小
*输入：App的地址和大小
*说明：此接口由应用程序来调用
	必须存储在flash里面
**********************************************************/
void BLSetAppSize(DWORD dwAppAddr, DWORD dwAppSize);

/**********************************************************
*功能：设置flash第1个扇区的大小
*输入：扇区的大小
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetFirstPartitionSize(DWORD dwPartitionSize);

/**********************************************************
*功能：设置flash扇区的大小
*输入：扇区的大小
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetPartitionSize(DWORD dwPartitionSize);

/**********************************************************
*功能：设置Irdeto KEY的存储在EEPROM.    缺省状态下是存储在flash里面
*输入： 
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetIrdetoKeyOnEEPROM(void);

/**********************************************************
*功能：设置Irdeto KEY的地址和大小,缺省状态下是存储在flash里面
*输入：Irdeto Key的地址和大小
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetIrdetoKeySize(DWORD dwIrdetoKeyAddr, DWORD dwIrdetoKeySize);

/**********************************************************
*功能：设置loaderpt的地址和大小，缺省状态下是存储在flash里面
*输入：loaderpt的地址和大小
*说明：此接口由应用程序来调用,
**********************************************************/
void BLSetLoaderptSize(DWORD dwLoaderptAddr, DWORD dwLoaderptSize);

/**********************************************************
*功能：设置loaderpt存放在模拟eeprom中,缺省状态下是存储在flash里面
*输入： 
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetLoaderptOnEEPROM(void);

/**********************************************************
*功能：设置syssetting的地址和大小缺省状态下是存储在flash里面
*输入：syssetting的地址和大小,
*说明：此接口由应用程序来调用,如果没有syssetting则不需要调用这个
**********************************************************/
void BLSetSysSettingSize(DWORD dwSysSettingAddr, DWORD dwSysSettingSize);

/**********************************************************
*功能：设置syssetting存放在模拟eeprom中，缺省状态下是存储在flash里面
*输入： 
*说明：此接口由应用程序来调用
**********************************************************/
void BLSetSysSettingOnEEPROM(void);

/**********************************************************
*功能：设置USB升级需要使用到的ResetType缺省状态下是存储在flash里面
*输入：dwAddr: 请参考项目的flash map中的reset type区域,地址
*说明：如果项目没有USB升级要求,则不需要调用这个
**********************************************************/
void BLSetUSBSize(DWORD dwAddr);
/**********************************************************
*功能：设置USB升级参数存放在模拟eeprom中，缺省状态下是存储在flash里面
*输入： 
*说明:如果项目没有USB升级要求,则不需要调用这个
**********************************************************/
void BLSetUSBOnEEPROM(void);
//-----设置地址结束-----------------------------------------------------------------------


























//(2)---app初始化库------------------------------------------------------

/**********************************************************
功能：app初始化OTA 库
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/
INT CS_CA_Loader_Init(void);
//-----app初始化库结束--------------------------------------------------

















//(3) app读取数据或信息,     
/**********************************************************
功能：app读取CPCB的数据
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/
INT CS_CA_Loader_GetCPCB(T_CpcbInfo*  pstBuf );
/**********************************************************
功能：app读取BBCB的数据
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/

INT CS_CA_Loader_GetBBCB(T_BbcbInfo*  pstBuf);
/**********************************************************
功能：app读取Irdeto key的状态
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/
INT CS_CA_Loader_GetKeyStatus(T_KeyStatusEx*  pstKeyStatus);
/**********************************************************
功能：app读取Irdeto的配置状态,包括(厂商代号,硬件版本号,市场号,当前下载号)
输入：
输出：
返回：                         1表示成功,       0表示失败
说明：app调用
**********************************************************/
BOOL CS_CA_Loader_ReadIrdCfg(T_LoaderIrdInfo*  pIrdCfg);

/**********************************************************
功能：获取APP.bin文件在flash中的实际长度
输入：dwAddr:   应用程序在flash中的地址 
输出：pdwRealSize:实际大小
返回：SUCCESS(0):成功,FAILURE(1)
说明：此接口由应用程序来调用
**********************************************************/
DWORD CSBL_GetAppRealSize(DWORD dwAddr, PDWORD pdwRealSize);
/**********************************************************
功能：获取Irdeto BL 库的软件版本号。
输入：pcBuf[] 数组长度必须大于50个字节
输出：BL库的软件版本号。字符串模式。
说明：此接口由应用程序来调用
**********************************************************/
void CS_CA_Loader_Version(char * pcBuf);











///(4) 写OTA频点,符号率,PID, tableID等参数信息
/**********************************************************
功能：app写OTA升级参数,并且置loaderpt当中的OTA有效标志, 重启后会进入loader;
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/
INT CS_CA_Loader_SaveOtaInfo(T_OtaCfg*  pstOtaInfo);


/**********************************************************
功能：app写USB 升级标志
输入：
输出：
返回：0表示成功,  其他失败
说明：app调用
**********************************************************/
INT CSBLSetIrdetoUsbUpgrade(void);


#ifdef  __cplusplus
}
#endif

#endif




