/******************************************************************

@defgroup  不分CA规范,   提供应用程序与coship规范Bootloader交互的接口。
@brief 本模块对应头文件<csbl_syssetting.h>，定义了数据交互部分的接口。
现对该模块进行如下说明：


(1)  	PAL/ NTSC制式等制式同步, app修改后制式后，存储在flash或eeprom中的syssetting数据区域，重启后，bootloader能修改成app一样的制式
(2)	tuner类型同步等等,比如目前有些项目bootloader是同时支持AnnxB, AnnxA，需要在syssetting里面设置参数，这样loader初始化tuner的时候，会选择对应的参数初始化tuner.
(3)   目前(2011.07.10) booter, loader对syssetting区域是只读取，  不写入的原则.
(4)   app对此区域可读，可写。
(5)   烧录机顶盒程序的时候，如果没有syssetting,则需要app开机后写入项目的缺省值


@version    1.0.0   bootloader     2006/3/22
@version    2.0.0   bootloader     2011/3/22
@version    3.0.0   bootloader     2011/7/10
***********************************************************************/

#ifndef _CSBL_SYSSETTING_H_
#define _CSBL_SYSSETTING_H_

#include "udiplus_debug.h"
#include "csbl_gconfig.h"
#include "udi2_screen.h"

#ifdef	__cplusplus
extern "C" {
#endif


#define M_SYS_SETTING_TAG    "BLSYSSETTING"
#define M_SYS_SETTING_TAG_LENGTH   (16)


///SYSSETTING结构体最大长度,不要超过0xf0字节  2012.01.20,
///因为syssetting有可能存储在模拟eeprom, 而模拟eeprom都比较小
#define M_SYS_SETTING_LENGTH       (0xF0)


/**@brief LIB操作错误类型定义*/
typedef enum
{
	EM_CSBL_SYSSETTING_SUCCESS=0, ///< 成功
	EM_CSBL_SYSSETTING_FAILURE =1 , ///< 失败,未知的失败
	EM_CSBL_SYSSETTING_FLASH_ERROR_R=2, ///< 读flash错误
	EM_CSBL_SYSSETTING_FLASH_ERROR_W=3, ///< 写flash错误	
	EM_CSBL_SYSSETTING_ADDR=4, ///未设置syssetting地址,同方规范可能是sysconfig.flash配置表中无syssetting地址,   irdeto规范可能是未调用BLSetSysSettingSize(),或者是存储类型错误
	EM_CSBL_SYSSETTING_NOT_EXIST=5, ///flash或eeprom并没有syssetting, 是通过m_aTag判断
	EM_CSBL_SYSSETTING_LENGTH =6,   ///yssetting的长度m_dwLength过长,大于M_SYS_SETTING_LENGTH - 24
	EM_CSBL_SYSSETTING_CRC= 7 ,       ///syssetting的CRC错误
	EM_CSBL_SYSSETTING_POINTER_ZERO = 8 ,       ///传入的参数为0
	EM_CSBL_SYSSETTING_NUM            ///上限
} CSBLSyssettingErr_E;



/**
@brief 有线数字电视(QAM)tuner的调制类型枚举定义
@note:bootloader  QAM tuner类型同步,比如目前有些项目是同时支持AnnxB, AnnxA，需要在syssetting里面设置参数，这样loader初始化tuner的时候，会选择对应的tuner.
@note: 默认都是AnnexA, 无此兼容的bootloader则忽略此选项
*/
typedef enum 							
{
	EM_CSBL_TUNER_INVALID,
	EM_CSBL_TUNER_ANNEXA,    ///Annex A -    16, 32, 64, 128, 256 QAM
	EM_CSBL_TUNER_ANNEXB,    ///Annex B – 64/256 QAM (Optional)
	EM_CSBL_TUNER_ANNEXC,   ///Annex C -     16, 32, 64, 128, 256 QAM
	EM_CSBL_TUNER_NUM         ///< 上限。
}CSBLTunerQAM_E;


/**
@brief bootloader串口打印级别枚举定义
@note  : 比如conax 规范要求bootloader关闭串口,但app可以通过syssetting打开bl串口.
@note  : 如果规范没有要求，则不起作用
*/
typedef enum 							
{
	EM_CSBL_UART_INVALID,
	EM_CSBL_UART_ON,    ///串口打开
	EM_CSBL_UART_OFF,    ///串口关闭, 
	EM_CSBL_UART_NUM         ///< 上限。
}CSBLUart_E;


/****
@brief    bootloader与app之间同步数据的结构体syssetting定义

@note:此结构体后续可以扩充，但最大不能超过M_SYS_SETTING_LENGTH,    2011.07.10

****/
typedef struct
{
	char m_aTag[M_SYS_SETTING_TAG_LENGTH]; /***标志 M_SYS_SETTING_TAG ***/
	unsigned int m_dwCrc32;	/***mpegcrc32, 计算范围为m_eBLScreenMode开始字段,长度为m_dwLength, ***/
	unsigned int m_dwLength;	/***syssetting数据的长度,不包括本字段,m_dwLength = sizeof(CSBLSyssettingInfo_S) -  sizeof(m_aTag) - sizeof(m_dwCrc32) - sizeof(m_dwLength) ***/
	CSUDISCREENResolution_E	m_eBLScreenMode;/**@brief 标清输出SD:视频输出分辨率udi2.0接口*/
	CSBLTunerQAM_E			m_eBLQAMType;/**@brief 有线数字电视(QAM)tuner的调制类型枚举定义*/
	CSBLUart_E				m_eBLUart;	/**@brief bootloader串口打印级别枚举定义**/
	CSBLUILanguageType_E		m_eBLLanguage;	/**@brief bootloader语言显示枚举定义**/	
	unsigned int  				m_dwRFChannelNum; /**** bootloade的RF输出频道CH3/4,   21---69  ,    直接输入频道的数字,3,4,21,22,23....68,69****/
	CSUDISCREENResolution_E	m_eBLHDscreenMode;/**@brief 高清输出HD:视频输出分辨率udi2.0接口*/
} CSBLSyssettingInfo_S;


/************************************************************
@brief 获取bootloader中存储的syssetting

@return 返回CSBLSyssettingErr_E  中定义的错误

@note: 从flash或者eeprom中读取 syssetting，读取不到数据，则不改变psBLSyssetting任何值
@note: 
************************************************************/
CSBLSyssettingErr_E CSBLSyssettingRead(CSBLSyssettingInfo_S * psBLSyssetting);


/************************************************************
@brief 设置bootloader中存储的syssetting

@return 返回CSBLSyssettingErr_E  中定义的错误

@note: 将syssetting数据写入flash,或者eeprom
@note: 此内部函数会自动修改:
@note: CSBLSyssettingInfo_S->m_aTag[] 
@note: CSBLSyssettingInfo_S->m_dwCrc32 
@note: CSBLSyssettingInfo_S->m_dwLength
@note: 所以外部调用者,只需要修改m_eBLScreenMode字段开始的变量
************************************************************/
CSBLSyssettingErr_E CSBLSyssettingWrite(CSBLSyssettingInfo_S * psBLSyssetting);






/************************************************************
booter,  loader,  app读写syssetting例子, 目前(2011.07.10) booter, loader对syssetting区域是只读取，  不写入的原则.

void CSBL_Syssetting_Read(void)
{	
	CSBLSyssettingErr_E  eRet;
	CSBLSyssettingInfo_S  sSysetting;


	
	////如果syssetting没有烧入,则flash空间中无此数据
	////这里你可以读syssetting,设置bl的视频制式.............
	eRet=CSBLSyssettingRead(&sSysetting);



	
	printf("CSBLSyssettingRead()=%d\r\n", eRet);	
	if(EM_CSBL_SYSSETTING_SUCCESS == eRet)
	{
		printf("m_eBLScreenMode=%d\r\n",sSysetting.m_eBLScreenMode);
		printf("m_eBLQAMType=%d\r\n",sSysetting.m_eBLQAMType);
		printf("m_eBLUart=%d\r\n",sSysetting.m_eBLUart);	
		这里可以进行你需要的处理.
	}
	else
	{
		printf("error!\r\n");
	}
}

void CSBL_Syssetting_Write(CSUDISCREENResolution_E eType,CSBLTunerQAM_E eQam, CSBLUart_E eUart)
{	
	CSBLSyssettingErr_E  eRet;
	CSBLSyssettingInfo_S  sSysetting;
	
	sSysetting.m_eBLScreenMode = eType;
	sSysetting.m_eBLQAMType =eQam;
	sSysetting.m_eBLUart = eUart;
	
	eRet=CSBLSyssettingWrite(&sSysetting);

	
	printf("CSBLSyssettingWrite()=%d\r\n", eRet);	
	if(EM_CSBL_SYSSETTING_SUCCESS == eRet)
	{
		printf("write  successfully!\r\n");	
	}
	else
	{
		printf("write error!\r\n");
	}
}
************************************************************/



#ifdef	__cplusplus
}
#endif


#endif


